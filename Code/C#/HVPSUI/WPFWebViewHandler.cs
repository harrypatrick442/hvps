using Core.Events;
using Microsoft.Web.WebView2.Core;
using Microsoft.Web.WebView2.Wpf;
using Native.WebViewInterface;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HVPSUI
{
    public class WPFWebViewHandler : IWebViewHandler
    {
        public event EventHandler<MessageEventArgs> MessageReceived;
        private WebView2 _WebView;
        public WPFWebViewHandler(WebView2 webView)
        {
            _WebView = webView;
            webView.CoreWebView2.Settings.IsWebMessageEnabled = true;
            webView.CoreWebView2.WebMessageReceived += WebMessageReceived;
        }
        public void SendMessage(string message)
        {
            _WebView.Dispatcher.Invoke(() =>
            {
                _WebView.CoreWebView2.PostWebMessageAsJson(message);
            });
        }
        private void WebMessageReceived(object sender, CoreWebView2WebMessageReceivedEventArgs e)
        {
            string message = e.TryGetWebMessageAsString();
            MessageReceived?.Invoke(this, new MessageEventArgs(message));
        }
    }
}
