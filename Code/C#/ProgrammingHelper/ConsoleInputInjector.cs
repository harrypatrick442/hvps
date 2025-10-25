using System.Management;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using Microsoft.Win32;
namespace ProgrammingHelper
{
    using System;
    using System.Diagnostics;
    using System.Runtime.InteropServices;

    public static class ConsoleInputInjector
    {
        private const uint ATTACH_PARENT_PROCESS = 0xFFFFFFFF;
        private const int STD_INPUT_HANDLE = -10;

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool AttachConsole(uint dwProcessId);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool FreeConsole();

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr GetStdHandle(int nStdHandle);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool WriteConsoleInput(IntPtr hConsoleInput,
            INPUT_RECORD[] lpBuffer, uint nLength, out uint lpNumberOfEventsWritten);

        // Structures for WriteConsoleInput
        [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode)]
        private struct INPUT_RECORD
        {
            [FieldOffset(0)] public short EventType;
            [FieldOffset(4)] public KEY_EVENT_RECORD KeyEvent;
            // (other event types omitted -- we only use key events)
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        private struct KEY_EVENT_RECORD
        {
            [MarshalAs(UnmanagedType.Bool)] public bool bKeyDown;
            public ushort wRepeatCount;
            public ushort wVirtualKeyCode;
            public ushort wVirtualScanCode;
            public char UnicodeChar;
            public uint dwControlKeyState;
        }

        private const short KEY_EVENT = 0x0001;

        /// <summary>
        /// Inject text into the console belonging to a process (same visible window).
        /// The method attaches to the console, writes the characters (keydown + keyup),
        /// sends an Enter if includeNewline is true, then detaches.
        /// </summary>
        /// <param name="targetProcessId">Process.Id of cmd.exe (or other console process).</param>
        /// <param name="text">Text to send (no trailing newline unless you want one).</param>
        /// <param name="includeNewline">If true, an Enter is appended.</param>
        /// <returns>True if injected successfully; false otherwise.</returns>
        public static bool InjectTextIntoConsole(
            int targetProcessId, 
            string text, 
            bool includeNewline = true)
        {
            if (targetProcessId <= 0) return false;

            // Must FreeConsole first if this process already has one attached.
            // Usually a WinForms app has no console, but be defensive.
            try { FreeConsole(); } catch { /* ignore */ }

            if (!AttachConsole((uint)targetProcessId))
            {
                // attach failed
                return false;
            }

            try
            {
                IntPtr hIn = GetStdHandle(STD_INPUT_HANDLE);
                if (hIn == IntPtr.Zero || hIn == new IntPtr(-1))
                    return false;

                string toSend = text ?? string.Empty;
                if (includeNewline) toSend += "\r";

                // Build INPUT_RECORD[] with keydown + keyup for each char
                var records = new System.Collections.Generic.List<INPUT_RECORD>(toSend.Length * 2);

                foreach (char ch in toSend)
                {
                    // Key down
                    INPUT_RECORD down = new INPUT_RECORD
                    {
                        EventType = KEY_EVENT,
                        KeyEvent = new KEY_EVENT_RECORD
                        {
                            bKeyDown = true,
                            wRepeatCount = 1,
                            wVirtualKeyCode = 0,
                            wVirtualScanCode = 0,
                            UnicodeChar = ch,
                            dwControlKeyState = 0
                        }
                    };

                    // Key up
                    INPUT_RECORD up = new INPUT_RECORD
                    {
                        EventType = KEY_EVENT,
                        KeyEvent = new KEY_EVENT_RECORD
                        {
                            bKeyDown = false,
                            wRepeatCount = 1,
                            wVirtualKeyCode = 0,
                            wVirtualScanCode = 0,
                            UnicodeChar = ch,
                            dwControlKeyState = 0
                        }
                    };

                    records.Add(down);
                    records.Add(up);
                }

                if (records.Count == 0) return true;

                uint written;
                bool ok = WriteConsoleInput(hIn, records.ToArray(), (uint)records.Count, out written);
                return ok && written == records.Count;
            }
            finally
            {
                // Always detach so subsequent calls can attach again
                try { FreeConsole(); } catch { /* ignore */ }
            }
        }
    }



}
