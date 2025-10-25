using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ProgrammingHelper
{
    public partial class ProjectOptions : UserControl
    {
        private const string UNKNOWN_COM_PLACEHOLDER = "[Unknown COM]";
        private ConsoleBridge? _ConsoleBridge;
        private ConsoleBridge ConsoleBridge { get {
                if (_ConsoleBridge == null)
                {
                    string workingDirectory = Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location)!, ProjectRelativePath);
                    _ConsoleBridge = new ConsoleBridge(workingDirectory, ()=>_ConsoleBridge= null);
                }
                return _ConsoleBridge;
            } 
        }
        public ProjectOptions()
        {
            InitializeComponent();
        }

        private void _ButtonSetup_Click(object sender, EventArgs e)
        {
            ConsoleBridge.BringToFront();
            ConsoleBridge.Send($"set IDF_PATH=C:\\Users\\USER1\\esp\\v5.4\\esp-idf");
            ConsoleBridge.Send($"call %IDF_PATH%\\export.bat");
        }

        private void _ButtonBuild_Click(object sender, EventArgs e)
        {
            ConsoleBridge.BringToFront();
            ConsoleBridge.Send($"idf.py build");
        }

        private void _ButtonFlash_Click(object sender, EventArgs e)
        {
            ConsoleBridge.BringToFront();
            if (!GetComPort(out string? comPort))
            {
                comPort = UNKNOWN_COM_PLACEHOLDER;
            }
            ConsoleBridge.Send($"idf.py -p {comPort} flash");
        }

        private void _ButtonMonitor_Click(object sender, EventArgs e)
        {
            ConsoleBridge.BringToFront();
            if (!GetComPort(out string? comPort))
            {
                comPort = UNKNOWN_COM_PLACEHOLDER;
            }
            ConsoleBridge.Send($"idf.py -p {comPort} monitor");
        }
        private bool GetComPort(out string? comPort) {
            int? c = ComPortHelper.GetComPortForPnpDeviceId(PnpDeviceId);
            if (c == null) {
                comPort = null;
                return false;
            }
            comPort = $"COM{c}";
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
