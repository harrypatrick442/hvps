using Core.Events;
using Logging;
using Logging.Writers;
using Microsoft.Web.WebView2.Core;
using Microsoft.Web.WebView2.WinForms;
using Microsoft.Web.WebView2.Wpf;
using Native.WebViewInterface;
using Shutdown;
using System.IO;
using System.Reflection;
using System.Text;
using System.Windows;

namespace HVPSUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private MessageHandler _MessageHandler;
        public MainWindow()
        {
            string executingDirectory = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location)!;
            string logFilePath = Path.Join(executingDirectory, "log.txt");
            Logs.Initialize(logFilePath);
            Logs.Default.AddLogWriter(new DiagnosticsConsoleWriter());
            ShutdownManager.Initialize(Application.Current.Shutdown, () => Logs.Default);
            InitializeComponent();
            InitAsync();
        }

        private async void InitAsync()
        {
            await webView.EnsureCoreWebView2Async();
            webView.CoreWebView2.Navigate("localhost:3000");
            var webViewMessagingInterface = new WebViewMessagingInterface(new WPFWebViewHandler(webView));
            _MessageHandler = new MessageHandler(webViewMessagingInterface);
        }

    }
}