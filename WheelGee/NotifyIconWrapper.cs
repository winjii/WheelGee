using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using MMFrame.Windows.GlobalHook;
using MMFrame.Windows.Simulation;
using System.Runtime.InteropServices;

namespace WheelGee
{
    public partial class NotifyIconWrapper : Component
    {
        private MouseHook.StateMouse lastState;
        private bool isScrollMode = false;
        private List<InputSimulator.Input> inputs;

        [DllImport("GlobalHook.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int SetHook(int hWnd);
        [DllImport("GlobalHook.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int ResetHook();

        public NotifyIconWrapper()
        {
            InitializeComponent();
            
            toolStripMenuItem_Open.Click += toolStripMenuItem_Open_Click;
            toolStripMenuItem_Exit.Click += toolStripMenuItem_Exit_Click;
            //MouseHook.AddEvent(hookMouseInput);
            //MouseHook.Start();
            inputs = new List<InputSimulator.Input>();
            int ret = SetHook(0);
            Console.WriteLine(ret);
        }

        ~NotifyIconWrapper()
        {
            //MouseHook.Stop();
        }

        public NotifyIconWrapper(IContainer container)
        {
            container.Add(this);

            InitializeComponent();
        }

        private void hookMouseInput(ref MouseHook.StateMouse s)
        {
            //if (s.Stroke == MouseHook.Stroke.X2_DOWN) isScrollMode = !isScrollMode;
            //if (isScrollMode)
            //{
            //    int dy = lastState.Y - s.Y;
            //    inputs.Clear();
            //    InputSimulator.AddMouseInput(ref inputs, InputSimulator.MouseStroke.WHEEL, 120, false, 0, 0);
            //    MouseHook.Pause();
            //    InputSimulator.SendInput(inputs);
            //    MouseHook.Start();
            //}
            //lastState = s;
        }

        private void contextMenuStrip1_Opening(object sender, CancelEventArgs e)
        {

        }

        private void toolStripMenuItem_Open_Click(object sender, EventArgs e)
        {
            var window = new MainWindow();
            window.Show();
        }

        private void toolStripMenuItem_Exit_Click(object sender, EventArgs e)
        {
            int ret = ResetHook();
            Console.WriteLine(ret);
            Application.Current.Shutdown();
        }
    }
}
