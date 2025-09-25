using System;
using System.IO;
using System.Linq;
using System.Reflection;
using BaseMessages.Constants;
using Core.FileSystem;
using DataMemberNamesClassBuilder;
using VoltageFeedbackMessages;
namespace HVPSCodeGenerator
{
    class Program
    {
        static void Main(string[] args)
        {
            string reposDirectory = Assembly.GetEntryAssembly()!.Location;
            while (Path.GetFileName(reposDirectory).ToLower() != "repos")
            {
                reposDirectory = Directory.GetParent(reposDirectory)!.FullName;
            }
            string hvpsDirectory = Path.Combine(reposDirectory, "hvps");
            Type[] typeInEachNamespace = new Type[] {
                typeof(GlobalConstants.Android),
                typeof(MessageTypes.MessageTypes),
                typeof(Ticketing),
                typeof(BaseMessages.Constants.Ticketing),
                typeof(Native.DataMemberNames.Messages.NativeCloseFileMessageDataMemberNames),
                typeof(CoreNativeAndBackend.DataMemberNames.Messages.FileInfoDataMemberNames),
                typeof(VirtualSockets.DataMemberNames.NewVirtualSocket2DataMemberNames),
                typeof(Bluetooth.DataMemberNames.Messages.BluetoothDeviceDataMemberNames),
                typeof(VoltageFeedbackMessages.DataMemberNames.Requests.SetVoltageThresholdRequestDataMemberNames),
                typeof(HVPSMessages.DataMemberNames.Messages.HVPSStartMessageDataMemberNames),
                typeof(HVPSUIMessages.DataMemberNames.Requests.ConnectToBluetoothDeviceRequestDataMemberNames),
            };
            JavaScriptConstantsBuilderHelper.Run(
                Path.Combine(hvpsDirectory, "Code", "JavaScript", "client", "src", "constants"), "Constants",
                typeInEachNamespace);
            JavascriptClassBuilderHelper.Run(
                    Path.Combine(hvpsDirectory, "Code", "JavaScript", "client", "src", "messages"),
                    "DataMemberNames",
                    typeInEachNamespace);
            MessageTypesBuilderHelper.GenerateMessageTypesJs(
                    Path.Combine(hvpsDirectory, "Code", "JavaScript", "client", "src", "messages"),
                    typeInEachNamespace);
            JavascriptClassBuilderHelper.Run(
                Path.Combine(hvpsDirectory, "Code", "JavaScript", "client", "src", "content_wrappers"),
                "ContentWrapperDataMemberNames",
                typeInEachNamespace);
            string generatedCppDirectory = Path.Combine(hvpsDirectory, "temp", "generated_cpp");
            DirectoryHelper.DeleteRecursively(generatedCppDirectory);
            Directory.CreateDirectory(generatedCppDirectory);
            CPlusPlusClassBuilderHelper.Run(
                generatedCppDirectory,
                "DataMemberNames",
                typeInEachNamespace);
            string[] generatedDirectories = new string[]{
                Path.Combine(
                        reposDirectory,
                        "hvps", "Code", "C++", "HVPSController", "main", "Generated", "Messages"),
                Path.Combine(
                        reposDirectory,
                        "hvps", "Code", "C++", "VoltageFeedbackModule", "main", "Generated", "Messages")
            };
            foreach (string directory in generatedDirectories)
            {
                Directory.CreateDirectory(directory);
                DirectoryHelper.CopyRecurively(generatedCppDirectory, directory);
            }
            DirectoryHelper.DeleteRecursively(generatedCppDirectory);
        }
    }
}
