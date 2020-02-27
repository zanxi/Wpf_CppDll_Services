using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Win32_005.ViewModels;

namespace Win32_005.Commands
{
    public class RemoveGroupCommand: ICommand
    {
        public event EventHandler CanExecuteChanged;
        
        public bool CanExecute(object parameter)
        {
            return true;
        }

        public void Execute(object parameter)
        {
            this._viewModel.RemoveGroup();
        }

        private OrdersViewModel _viewModel;

        public RemoveGroupCommand(OrdersViewModel viewModel)
        {
            this._viewModel = viewModel;
        }
    }


    public class GroupByPIDCommand : ICommand
    {
        public event EventHandler CanExecuteChanged;

        public bool CanExecute(object parameter)
        {
            return true;
        }

        public void Execute(object parameter)
        {
            this._viewModel.GroupByPID();
        }

        private OrdersViewModel _viewModel;

        public GroupByPIDCommand(OrdersViewModel viewModel)
        {
            this._viewModel = viewModel;
        }
    }


    public class GroupByYearMonthCommand : ICommand
    {
        public event EventHandler CanExecuteChanged;

        public bool CanExecute(object parameter)
        {
            return true;
        }

        public void Execute(object parameter)
        {
            this._viewModel.GroupByYearMonth();
        }

        private OrdersViewModel _viewModel;

        public GroupByYearMonthCommand(OrdersViewModel viewModel)
        {
            this._viewModel = viewModel;
        }
    }
}
