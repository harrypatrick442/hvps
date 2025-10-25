using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ProgrammingHelper
{
    public partial class ConsoleBridge
    {
        private Process _Process;
        public Action _CallbackExited;
        public ConsoleBridge(string workingDirectory, Action callbackExited)
        {
            _CallbackExited = callbackExited;
            var psi = new ProcessStartInfo
            {
                FileName = "cmd.exe",
                RedirectStandardInput = false,
                RedirectStandardOutput = false,
                RedirectStandardError = false,
                UseShellExecute = true,
                CreateNoWindow = false,   // <-- true = hidden window, false = visible normal CMD
                WorkingDirectory = workingDirectory
            };

            _Process = new Process();
            _Process.StartInfo = psi;
            _Process.EnableRaisingEvents = true;             // <-- REQUIRED for Exited to fire
            _Process.Exited += Process_Exited;
            //_Process.OutputDataReceived += Cmd_OutputDataReceived;
            //_Process.ErrorDataReceived += Cmd_OutputDataReceived;
            _Process.Start();

            //_Process.BeginOutputReadLine();
            //_Process.BeginErrorReadLine();
        }
        public void BringToFront()
        {
            if (_Process != null && !_Process.HasExited)
            {
                var handle = _Process.MainWindowHandle;
                if (handle != IntPtr.Zero)
                {
                    // Restore if minimized
                    ShowWindow(handle, SW_RESTORE);
                    // Bring to foreground
                    SetForegroundWindow(handle);
                }
            }
        }
        public void Send(string command)
        {
            try
            {
                bool ok = ConsoleInputInjector.InjectTextIntoConsole(_Process.Id, command, includeNewline: true);
            }
            catch (Exception ex) {
                System.Diagnostics.Debug.WriteLine(ex);
            }
        }
        private void Process_Exited(object? sender, EventArgs e)
        {
            _CallbackExited();
        }
        /*
            private void Cmd_OutputDataReceived(object sender, DataReceivedEventArgs e)
            {
                if (string.IsNullOrEmpty(e.Data)) return;
                Invoke((MethodInvoker)(() =>
                {
                    richTextBox1.AppendText(e.Data + Environment.NewLine);
                }));
            }


            protected override void OnFormClosing(FormClosingEventArgs e)
            {
                try
                {
                    _StreamWriter?.WriteLine("exit");
                    _Process?.WaitForExit(1000);
                }
                catch { }
                base.OnFormClosing(e);
            }*/
    }
}
