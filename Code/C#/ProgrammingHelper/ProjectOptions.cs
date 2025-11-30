using System.ComponentModel;
using System.Reflection;

namespace ProgrammingHelper
{
    public partial class ProjectOptions : UserControl
    {
        private const string UNKNOWN_COM_PLACEHOLDER = "[Unknown COM]";
        private Esp32ConsoleBridge? _ConsoleBridge;
        private Esp32ConsoleBridge ConsoleBridge
        {
            get
            {
                if (_ConsoleBridge == null)
                {
                    string workingDirectory = Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location)!, ProjectRelativePath);
                    _ConsoleBridge = new Esp32ConsoleBridge(workingDirectory, () => _ConsoleBridge = null);
                }
                return _ConsoleBridge;
            }
        }
        private string? _CurrentComPort;
        public ProjectOptions()
        {
            InitializeComponent();
        }

        private void _ButtonSetup_Click(object sender, EventArgs e)
        {
            ConsoleBridge.BringToFront();
            ConsoleBridge.SendConfigureEnvironmentCommands();
        }

        private void _ButtonBuild_Click(object sender, EventArgs e)
        {
            ConsoleBridge.BringToFront();
            ConsoleBridge.SendBuild();
        }

        private void _ButtonFlash_Click(object sender, EventArgs e)
        {
            ConsoleBridge.BringToFront();
            if (!GetComPort(out string? comPort))
            {
                comPort = UNKNOWN_COM_PLACEHOLDER;
            }
            ConsoleBridge.SendFlash(comPort!);
        }

        private void _ButtonMonitor_Click(object sender, EventArgs e)
        {
            ConsoleBridge.BringToFront();
            if (!GetComPort(out string? comPort))
            {
                comPort = UNKNOWN_COM_PLACEHOLDER;
            }
            ConsoleBridge.SendMonitor(comPort!);
        }
        private void ButtonAll_Click(object sender, EventArgs e)
        {
            if (!GetComPort(out string? comPort))
            {
                comPort = UNKNOWN_COM_PLACEHOLDER;
            }
            ConsoleBridge.BringToFront();
            ConsoleBridge.SendConfigureEnvironmentCommands();
            ConsoleBridge.SendBuild();
            ConsoleBridge.SendFlash(comPort!);
            ConsoleBridge.SendMonitor(comPort!);
        }
        private bool GetComPort(out string? comPort)
        {
            int? c = ComPortHelper.GetComPortForPnpDeviceId(PnpDeviceId);
            if (c == null)
            {
                comPort = null;
                return false;
            }
            comPort = $"COM{c}";
            _CurrentComPort = comPort;
            return true;
        }

        private string _ProjectName;
        [Category("Custom")]
        [Description("The name of the project.")]
        public string ProjectName
        {
            get => _ProjectName;
            set
            {
                _ProjectName = value;
                LabelProjectName.Text = value; // example usage
            }
        }

        private string _ProjectRelativePath;
        [Category("Custom")]
        [Description("Relative path to the project.")]
        public string ProjectRelativePath
        {
            get => _ProjectRelativePath;
            set
            {
                _ProjectRelativePath = value;
            }
        }
        private string _PnpDeviceId;

        private void buttonCtlBracket_Click(object sender, EventArgs e)
        {
            ConsoleBridge.SendControlPlusBracket();
        }
        private void buttonCtlPlusC_Click(object sender, EventArgs e)
        {
            ConsoleBridge.SendControlPlusC();
        }

        private void buttonUpdateComPort_Click(object sender, EventArgs e)
        {
            _CurrentComPort = null;
            GetComPort(out string? ignore);
        }

        private void buttonBringToFront_Click(object sender, EventArgs e)
        {
            ConsoleBridge.BringToFront();
        }

        [Category("Custom")]
        [Description("The PNPDeviceId")]
        public string PnpDeviceId
        {
            get => _PnpDeviceId;
            set
            {
                _PnpDeviceId = value;
            }
        }
    }
}
