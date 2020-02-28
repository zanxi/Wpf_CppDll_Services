using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using Win32_005.ViewModels;

namespace Win32_005.Commands
{
    public class GroupByNameSrvCommand: ICommand
    {
        public event EventHandler CanExecuteChanged;
        
        public bool CanExecute(object parameter)
        {
            return true;
        }

        public void Execute(object parameter)
        {
            this._viewModel.GroupByNameSrv();
        }

        private OrdersViewModel _viewModel;

        public GroupByNameSrvCommand(OrdersViewModel viewModel)
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

    public class GroupByDescriptionCommand : ICommand
    {
        public event EventHandler CanExecuteChanged;

        public bool CanExecute(object parameter)
        {
            return true;
        }

        public void Execute(object parameter)
        {
            this._viewModel.GroupByDescription();
        }

        private OrdersViewModel _viewModel;

        public GroupByDescriptionCommand(OrdersViewModel viewModel)
        {
            this._viewModel = viewModel;
        }
    }


    public class GroupByStatusCommand : ICommand
    {
        public event EventHandler CanExecuteChanged;

        public bool CanExecute(object parameter)
        {
            return true;
        }

        public void Execute(object parameter)
        {
            this._viewModel.GroupByStatus();
        }

        private OrdersViewModel _viewModel;

        public GroupByStatusCommand(OrdersViewModel viewModel)
        {
            this._viewModel = viewModel;
        }
    }


    public class GroupByGroupSystemCommand : ICommand
    {
        public event EventHandler CanExecuteChanged;

        public bool CanExecute(object parameter)
        {
            return true;
        }

        public void Execute(object parameter)
        {
            this._viewModel.GroupByGroupSystem();
        }

        private OrdersViewModel _viewModel;

        public GroupByGroupSystemCommand(OrdersViewModel viewModel)
        {
            this._viewModel = viewModel;
        }
    }


    class ContextMenuDelegateCommand : ICommand
    {
        protected readonly Predicate<object> _canExecute;
        protected readonly Action<object> _execute;
        public event EventHandler CanExecuteChanged;
        public ContextMenuDelegateCommand(Action<object> execute) : this(execute, _ => true) { }
        public ContextMenuDelegateCommand(Action<object> execute, Predicate<object> canExecute)
        {
            _execute = execute ?? throw new ArgumentNullException(nameof(execute));
            _canExecute = canExecute ?? throw new ArgumentNullException(nameof(canExecute));
        }
        public bool CanExecute(object parameter) => _canExecute(parameter);
        public void Execute(object parameter) => _execute(parameter);

        public void RaiseCanExecuteChanged()
            => CanExecuteChanged?.Invoke(this, EventArgs.Empty);
    }

    class DataGridDelegateCommand : ICommand
    {
        protected readonly Predicate<object> _canExecute;
        protected readonly Action<object> _execute;
        public event EventHandler CanExecuteChanged;
        public DataGridDelegateCommand(Action<object> execute) : this(execute, _ => true) { }

        public DataGridDelegateCommand(Action<object> execute, Predicate<object> canExecute)
        {
            _execute = execute ?? throw new ArgumentNullException(nameof(execute));
            _canExecute = canExecute ?? throw new ArgumentNullException(nameof(canExecute));
        }

        public bool CanExecute(object parameter) => _canExecute(parameter);

        public void Execute(object parameter) => _execute(parameter);

        public void RaiseCanExecuteChanged()
            => CanExecuteChanged?.Invoke(this, EventArgs.Empty);
    }

    public class BindingProxy : Freezable
    {
        protected override Freezable CreateInstanceCore()
        {
            return new BindingProxy();
        }

        public object Data
        {
            get { return (object)GetValue(DataProperty); }
            set { SetValue(DataProperty, value); }
        }

        public static readonly DependencyProperty DataProperty =
            DependencyProperty.Register("Data", typeof(object), typeof(BindingProxy), new UIPropertyMetadata(null));
    }

}
