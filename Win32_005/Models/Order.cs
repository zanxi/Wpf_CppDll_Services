using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Win32_005.Models
{

    public class InfoWindowsDervices
    {
        [DllImport("Dll_Win32_002",
            EntryPoint = "InitInfoServicesWindowSystem")]
        static extern int InitInfo();

        [DllImport("Dll_Win32_002",
            EntryPoint = "GetInfoServicesWindowSystem")]
        static extern System.IntPtr GetInfo(int k);

        static int numSrv = 0;

        public static int GetCountSrv()
        {
            return numSrv;
        }

        public static void Init()
        {
            numSrv= InitInfo();            
        }

        public static string[] GetInfoSrv(int k)
        {

            IntPtr p = GetInfo(k);
            string str_param = Marshal.PtrToStringAnsi(p);
            string[] s_in = str_param.Split(new Char[] { '|' });
            Marshal.FreeHGlobal(p);
            return s_in;
        }

    }

    public class Order
    {
        public string Icon { get { return ".\\Images\\IconSrv.png";  } }
        public string NameSrv { get; set; }
        public int PID { get; set; }
        public string Description { get; set; }
        public string Status { get; set; }
        public string GroupSystem { get; set; }
        public string PathSrv { get; set; }
    }

    public class OrderIcon
    {
        public string Icon { get; set; }
    }

    public class OrdersIcon : ObservableCollection<OrderIcon>
    {
        public OrdersIcon()
        {
            for (int i = 0; i < InfoWindowsDervices.GetCountSrv(); i++)
            {
                this.Add(new OrderIcon { Icon = ".\\Images\\IconSrv.png" });
                if (i > 11 + 0 * InfoWindowsDervices.GetCountSrv() - 2) break;
            }
        }            
    }

    public class Orders : ObservableCollection<Order>
    {
        public Orders()
        {
            {
                InfoWindowsDervices.Init();
                {
                    for (int i = 0; i < InfoWindowsDervices.GetCountSrv(); i++)
                    {
                        string[] s_in = InfoWindowsDervices.GetInfoSrv(i);
                        this.Add(
                            new Order
                            {
                                NameSrv = s_in[0],
                                PID = int.Parse(s_in[1]),
                                Description = s_in[2],
                                Status = s_in[3],
                                GroupSystem = s_in[4],
                                PathSrv = s_in[5]
                            }
                            );
                        if (i > InfoWindowsDervices.GetCountSrv() - 2) break;
                    }
                }
            }
        }
         
    }
}
