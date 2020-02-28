using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Input;
using System.Collections.ObjectModel;
using System.ComponentModel;
using Win32_005.Models;
using Win32_005.Commands;

namespace Win32_005.ViewModels
{
    public class OrdersViewModel
    {
        public ICollectionView OrdersView { get; set; }

        // 28 febrary 2020
        // https://ru.stackoverflow.com/questions/717468/Контекстное-меню-datagrid-Получить-значение?rq=1

        public OrdersViewModel()
        {
            IList<Order> orders = new Orders();
            OrdersView = CollectionViewSource.GetDefaultView(orders);                                                             

            groupByDescriptionCommand = new GroupByDescriptionCommand(this);
            groupByPIDCommand = new GroupByPIDCommand(this);
            groupByStatusCommand = new GroupByStatusCommand(this);
            groupByGroupSystemCommand = new GroupByGroupSystemCommand(this);
            GroupByNameSrvCommand = new GroupByNameSrvCommand(this);
            contextMenuCommand = new ContextMenuDelegateCommand(o=> ContextMenuClick((Order)o));
            //datagridMenuCommand = new DataGridDelegateCommand();
        }

        public void ContextMenuClick(Order order)
        {
            string name = order.NameSrv;
            
        }

        public void GroupByNameSrv()
        {
            OrdersView.GroupDescriptions.Clear();
            OrdersView.GroupDescriptions.Add(new PropertyGroupDescription("NameSrv"));
        }

        public void GroupByPID()
        {
            OrdersView.GroupDescriptions.Clear();
            OrdersView.GroupDescriptions.Add(new PropertyGroupDescription("PID"));
        }

        public void GroupByDescription()
        {
            OrdersView.GroupDescriptions.Clear();            
            OrdersView.GroupDescriptions.Add(new PropertyGroupDescription("Description"));
        }

        public void GroupByStatus()
        {
            OrdersView.GroupDescriptions.Clear();
            //OrdersView.GroupDescriptions.Add(new PropertyGroupDescription("GroupSystem"));
            OrdersView.GroupDescriptions.Add(new PropertyGroupDescription("Status"));
        }

        public void GroupByGroupSystem()
        {
            OrdersView.GroupDescriptions.Clear();
            OrdersView.GroupDescriptions.Add(new PropertyGroupDescription("GroupSystem"));            
        }

        public ICommand groupByDescriptionCommand { get; private set; }
        public ICommand groupByPIDCommand { get; private set; }
        public ICommand groupByStatusCommand { get; private set; }                
        public ICommand groupByGroupSystemCommand { get; private set; }
        public ICommand GroupByNameSrvCommand { get;  private set;  }
        public ICommand contextMenuCommand { get; private set; }
        public ICommand datagridMenuCommand { get; private set; }
    }

    public class GroupsToTotalConverter: IValueConverter
    {
        public object Convert(object value, Type targetValue, object parameter, System.Globalization.CultureInfo culture)
        {
            if(value is ReadOnlyObservableCollection<object>)
            {
                var items = (ReadOnlyObservableCollection<object>)value;
                Decimal total = 0;
                foreach(Order element in items)
                {
                    //total += element.Description;
                }
                return total.ToString();
            }
            return "";
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return value;
        }
    }
}
