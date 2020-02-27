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

        public OrdersViewModel()
        {
            IList<Order> orders = new Orders();
            OrdersView = CollectionViewSource.GetDefaultView(orders);
            OrdersView.GroupDescriptions.Add(new PropertyGroupDescription("PathSrv"));

            groupByPIDCommand = new GroupByPIDCommand(this);
            groupByYearMonthCommand = new GroupByYearMonthCommand(this);
            removeGroupCommand = new RemoveGroupCommand(this);
        }

        public void RemoveGroup()
        {
            OrdersView.GroupDescriptions.Clear();
            OrdersView.GroupDescriptions.Add(new PropertyGroupDescription("PathSrv"));
        }

        public void GroupByPID()
        {
            OrdersView.GroupDescriptions.Clear();
            OrdersView.GroupDescriptions.Add(new PropertyGroupDescription("PID"));
        }

        public void GroupByYearMonth()
        {
            OrdersView.GroupDescriptions.Clear();
            OrdersView.GroupDescriptions.Add(new PropertyGroupDescription("GroupSystem"));
        }

        public ICommand groupByPIDCommand
        {
            get;
            private set;
        }

        public ICommand groupByYearMonthCommand
        {
            get;
            private set;
        }

        public ICommand removeGroupCommand
        {
            get;
            private set;
        }
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
