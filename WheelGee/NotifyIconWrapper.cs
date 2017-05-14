using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Runtime.InteropServices;

namespace WheelGee
{
    public partial class NotifyIconWrapper : Component
    {
        private bool isScrollMode = false;

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
