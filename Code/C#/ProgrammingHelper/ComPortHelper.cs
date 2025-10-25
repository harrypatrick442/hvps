using System.Management;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using Microsoft.Win32;
namespace ProgrammingHelper
{

    public static partial class ComPortHelper
    {
        public static int? GetComPortForPnpDeviceId(string pnpDeviceId)
        {
            // Escape backslashes and single quotes for WQL
            string escapedId = pnpDeviceId.Replace("\\", "\\\\").Replace("'", "\\'");

            string query = $"SELECT Name FROM Win32_SerialPort WHERE PNPDeviceID = '{escapedId}'";

            using var searcher = new ManagementObjectSearcher(query);
            foreach (ManagementObject mo in searcher.Get())
            {
                string? name = mo["Name"]?.ToString();
                var match = Regex.Match(name, @"COM(\d+)", RegexOptions.IgnoreCase);
                if (!match.Success)
                {
                    throw new Exception("Failed to extract COM port number");
                }
                return int.Parse(match.Groups[1].Value);
                //return mo["DeviceID"]?.ToString();
            }

            return null;
        }
        public static void UpdateComPortMappings(Dictionary<string, int> mapPnpDeviceIdToComPort)
        {

            var searcher = new ManagementObjectSearcher("SELECT DeviceID, PNPDeviceID FROM Win32_SerialPort");
            foreach (ManagementObject mo in searcher.Get())
            {
                string? pnp = mo["PNPDeviceID"]?.ToString();
                string? deviceId = mo["DeviceID"]?.ToString();
                if (string.IsNullOrEmpty(pnp) || string.IsNullOrEmpty(deviceId))
                {
                    continue;
                }
                if (!mapPnpDeviceIdToComPort.TryGetValue(pnp, out int desiredComPort))
                {
                    continue;
                }
                string desiredComPortString = $"COM{desiredComPort}";
                if (deviceId == desiredComPortString)
                {
                    continue;
                }
                bool ok = SetComPortForPnpDevice(pnp, desiredComPortString, out string err);
                if (!ok)
                {
                    throw new Exception(err);
                }
            }
        }
        // --- Public helper -------------------------------------------------
        /// <summary>
        /// Sets the COM port name (PortName) for a given PNPDeviceID and requests Windows to apply the change.
        /// pnpDeviceId MUST be the same string you see in Device Manager -> Details -> Device instance path
        /// Example: "USB\\VID_10C4&PID_EA60\\0001"
        /// </summary>
        public static bool SetComPortForPnpDevice(string pnpDeviceId, string newComName, out string? error)
        {
            error = null;
            try
            {
                if (string.IsNullOrWhiteSpace(pnpDeviceId))
                {
                    error = "pnpDeviceId empty";
                    return false;
                }
                if (!newComName.StartsWith("COM", StringComparison.OrdinalIgnoreCase))
                {
                    error = "newComName must start with 'COM'";
                    return false;
                }

                // Build registry path: HKLM\SYSTEM\CurrentControlSet\Enum\<pnpDeviceId>\Device Parameters
                // In registry the backslashes are actual path separators; on string we must use as passed.
                string basePath = @"SYSTEM\CurrentControlSet\Enum\" + pnpDeviceId;
                using (RegistryKey? devKey = Registry.LocalMachine.OpenSubKey(basePath, writable: true))
                {
                    if (devKey == null)
                    {
                        error = $"Device registry key not found: HKLM\\{basePath}";
                        return false;
                    }

                    using (RegistryKey paramsKey = devKey.OpenSubKey("Device Parameters", writable: true) ?? devKey.CreateSubKey("Device Parameters"))
                    {
                        if (paramsKey == null)
                        {
                            error = $"Could not open or create Device Parameters: HKLM\\{basePath}\\Device Parameters";
                            return false;
                        }

                        paramsKey.SetValue("PortName", newComName, RegistryValueKind.String);
                    }
                }

                // Request Windows to apply change for the given device instance id
                // We need to locate the device by instance id and call SetupDiCallClassInstaller(DIF_PROPERTYCHANGE)
                bool res = RequestDevicePropertyChange(pnpDeviceId, out string setupApiErr);
                if (!res)
                {
                    error = "Registry updated, but SetupAPI reapply failed: " + setupApiErr;
                    // still return true? often the registry change is enough but let's return false so caller knows.
                    return false;
                }

                return true;
            }
            catch (Exception ex)
            {
                error = ex.Message;
                return false;
            }
        }

        static bool RequestDevicePropertyChange(string pnpDeviceId, out string errorMessage)
        {
            errorMessage = null;
            IntPtr deviceInfoSet = IntPtr.Zero;
            try
            {
                // Get all present devices
                deviceInfoSet = SetupDiGetClassDevs(IntPtr.Zero, null, IntPtr.Zero, DIGCF_PRESENT | DIGCF_ALLCLASSES);
                if (deviceInfoSet == IntPtr.Zero || deviceInfoSet.ToInt64() == -1)
                {
                    errorMessage = "SetupDiGetClassDevs failed";
                    return false;
                }

                uint index = 0;
                SP_DEVINFO_DATA devInfo = new SP_DEVINFO_DATA();
                devInfo.cbSize = (uint)Marshal.SizeOf(typeof(SP_DEVINFO_DATA));
                while (SetupDiEnumDeviceInfo(deviceInfoSet, index, ref devInfo))
                {
                    index++;

                    // Get instance id
                    StringBuilder sb = new StringBuilder(260);
                    int needed;
                    if (!SetupDiGetDeviceInstanceId(deviceInfoSet, ref devInfo, sb, sb.Capacity, out needed))
                    {
                        continue;
                    }

                    string instanceId = sb.ToString();
                    // Compare normalized forms (pnpDeviceId may have single backslashes)
                    if (string.Equals(NormalizeDeviceInstanceId(instanceId), NormalizeDeviceInstanceId(pnpDeviceId), StringComparison.OrdinalIgnoreCase))
                    {
                        // Prepare property-change params
                        SP_PROPCHANGE_PARAMS propParams = new SP_PROPCHANGE_PARAMS();
                        propParams.ClassInstallHeader.cbSize = (uint)Marshal.SizeOf(typeof(SP_CLASSINSTALL_HEADER));
                        propParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
                        propParams.StateChange = 0x00000001; // DICS_ENABLE / DICS_DISABLE? For property change we use DICS_ENABLE
                        propParams.Scope = DICS_FLAG_GLOBAL;
                        propParams.HwProfile = 0;

                        int structSize = Marshal.SizeOf(propParams);
                        if (!SetupDiSetClassInstallParams(deviceInfoSet, ref devInfo, ref propParams, structSize))
                        {
                            int err = Marshal.GetLastWin32Error();
                            errorMessage = $"SetupDiSetClassInstallParams failed Win32Err={err}";
                            return false;
                        }

                        if (!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, deviceInfoSet, ref devInfo))
                        {
                            int err = Marshal.GetLastWin32Error();
                            errorMessage = $"SetupDiCallClassInstaller failed Win32Err={err}";
                            return false;
                        }

                        // success
                        return true;
                    }
                }

                // if we get here we didn't find a match
                errorMessage = "Device instance id not found in current device list";
                return false;
            }
            finally
            {
                if (deviceInfoSet != IntPtr.Zero)
                {
                    SetupDiDestroyDeviceInfoList(deviceInfoSet);
                }
            }
        }

        static string NormalizeDeviceInstanceId(string id)
        {
            // Device instance IDs can have different slash escaping. Normalize to single backslashes
            // Example input: "USB\\VID_10C4&PID_EA60\\0001"
            return id?.Replace(@"\\", @"\") ?? id;
        }

    }


}
