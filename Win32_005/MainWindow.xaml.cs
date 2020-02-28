using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Win32_005.Models;

namespace Win32_005
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            DataContext = new ViewModels.OrdersViewModel();            
        }

        private void DataGrid_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            // 28 febrary
            //https://stackoverflow.com/questions/13449413/how-to-acess-datagridcell-on-right-click-on-wpf-datagrid

            var hit = VisualTreeHelper.HitTest((Visual)sender, e.GetPosition((IInputElement)sender));
            DependencyObject cell = VisualTreeHelper.GetParent(hit.VisualHit);
            while (cell != null && !(cell is System.Windows.Controls.DataGridCell)) cell = VisualTreeHelper.GetParent(cell);
            System.Windows.Controls.DataGridCell targetCell = cell as System.Windows.Controls.DataGridCell;
            //Order o = (Order)targetCell.GetValue();
            string nn= ((TextBlock)targetCell.Content).Text;
            //string nn = targetCell.Content;
            //ContentControl.Content ct = targetCell.Content;
            if(nn=="Running")
            {

            }
            else if (nn == "Stopped")
            {
                //DataContext.mestart_menu
                //this.
            }



        }
    }
}
