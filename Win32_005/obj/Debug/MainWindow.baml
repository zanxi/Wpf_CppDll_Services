   M S B A M L   `   `   `  ����D  @Win32_005, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null;"clr-namespace:Win32_005.ViewModelsWin32_005.ViewModels  5   	       Win32_005.MainWindow   5   	   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   4local"clr-namespace:Win32_005.ViewModels     5   	     � Title$-  'Windows services (start, stop, restart)��5   	   $	��850��69   $��Center=�6F    � 	Resources 5      1   +Win32_005.ViewModels.GroupsToTotalConverter   groupsConverter  conMenu m� Command �� RelativeSource �� AncestorType m� CommandParameter��%`  &	        &	        5   
   6'   �� 5   
   .@�5      m� $��
start_menu��6   $��Start��6*    5      �� * DataContext.contextMenuCommand+ �� *FindAncestor+$ WindowL� 5      �� m� 5      $��	stop_menu��6   $
��Stop��6(   m� 5      $��restart_menu��6   $
 Copy��6,   $��Restart��6;   5      5      .��5      � .+�5   
   �� .+�5      &   System.Windows.Controls.DataGrid  -   5        $��dgSrv��5      $	��750��6$      SelectionMode$ Single=�5         SelectionUnit$ FullRow=�5       �� EdgeMode$ Aliased=�5      	 � UseLayoutRounding	 . 
   AutoGenerateColumns
 .  $��Stretch=�5       ��5      �� *DataContext.OrdersView+ �� " �   Columns 5!      4  .System.Windows.Controls.DataGridTemplateColumn  5"      ,  &System.Windows.Controls.DataGridColumn   Header$
 Name��6-      CellTemplate 5#      �� 5$      .R�5%   "   ��  �� Orientation$ 
Horizontal=�6-   .+�5&   &   �� $��20��6,   $��20��68   ��6C   ��  �� Path$
 Icon��� 5'   &   ��60   �� $ NameSrv�5(   #   5)      5+        5-      $	 PID��6-    5.      �� 5/      .R�50   "   �� $ 
Horizontal=�6-   .+�51   &   �� ��60   �� $	 PID�52   #   53      55        57      $ Description��6-    58      �� 59      .R�5:   "   �� $ 
Horizontal=�6-   .+�5;   &   �� ��60   �� $ Description�5<   #   5=      5?        5A      $ Status��6-    5B      �� 5C      .R�5D   "   �� $ 
Horizontal=�6-   .+�5E   &   �� ��60   �� $ Status�5F   #   5G      5I        5K      $ GroupSystem��6-    5L      �� 5M      .R�5N   "   �� $ 
Horizontal=�6-   .+�5O   &   �� ��60   �� $ GroupSystem�5P   #   5Q      5S        5U      $ PathSrv��6-    5V      �� 5W      .R�5X   "   �� $ 
Horizontal=�6-   .+�5Y   &   �� ��60   �� $ PathSrv�5Z   #   5[      5]         	CellStyle 5b      �� 5c       �� 
TargetType*  $System.Windows.Controls.DataGridCell"  6   .�5d      ��  �� Property � �� �� Value$ 0,0,1,0q�6<   �� 5e       � ��
 �����68   5f         RowStyle 5h      �� 5i      )  #System.Windows.Controls.DataGridRow"  6   .�5j      ��  �@�ContextMenu# �  68   5k       �� 
GroupStyle 5m      (  "System.Windows.Controls.GroupStyle  5n         ContainerStyle 5o      �� 5p      " ��6$   .�5q   "   ��  � ��$ 0,0,0,5q�6;   �� 5r   "    � �� 5s   &   �� 5t   *    �� 
TargetType" ��6:   .R�5u   .   9� 
��� ��� 9� 
IsExpanded . 
���2͚�$��0,0,0,1q�5x   7   ��5y   2   �� 5z   6   $ 
Horizontal=�6A   .+�5{   :   �� $��Center=�6D   $��[��5|   D   �� 5}   :   $��Center=�6D   $
��Bold"�5~   D   $	��200��5�   D   ��5   D   �� $
 Name�5�   D   �� 5�   :   $��Center=�6D   $��]��5�   D   �� 5�   :   $��Center=�6D   $;��5; Колличество служб в группе: ��5�   D   �� 5�   :   $
��Bold"�6D   ��5�   D   �� *	ItemCount+�� 5�   :   $��Center=�6D    �� TextAlignment$ Right=�5�   D   $��; Description: ��5�   D   $	��100��5�   D   �� 5�   :   $
��Bold"�6D   $��Center=�5�   D   ��5�   D   �� $ Items� �� 	Converter# [  5�   7   ��5�   2   �� 5�   6   5�   /   5�   +   5�   #   5�      5�      5�      �� 5�      $ 
Horizontal=�6   $��25��62   .+�5�      �� $	��250��6G   ��6   �� *groupByDescriptionCommand+.��6S   Sort by Description6j   �� 5�      $	��250��6B   ��6   �� *groupByStatusCommand+.��6N   Sort By Status6`   �� 5�      $	��250��6G   ��6   �� *groupByGroupSystemCommand+.��6S   Sort by GroupSystem6h   5�      5�      5�      5�      