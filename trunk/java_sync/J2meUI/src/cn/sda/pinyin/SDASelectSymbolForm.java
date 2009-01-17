package cn.sda.pinyin;

import cn.sda.event.ListViewSelectItemEvent;
import cn.sda.ui.SDABaseControl;
import cn.sda.ui.SDAConsts;
import cn.sda.ui.SDAForm;
import cn.sda.ui.SDAListView;
import cn.sda.ui.SDAListViewItem;

/**
 *
 * @author Administrator
 */
public class SDASelectSymbolForm extends SDAForm {

    //用ListView来选择
    private SDAListView listView = null;
    private SDAListView SBClistView = null;
    private char[] symbolChars = null;
    private char[] symbolSBCChars = null;
    private SDABaseControl control = null;
    //单实例
    private static SDASelectSymbolForm instance;

    synchronized public static SDASelectSymbolForm getInstance() {
        if (instance == null) {
            instance = new SDASelectSymbolForm();
        }
        return instance;
    }

    protected SDASelectSymbolForm() {
        super("插入符号");
        listView = new SDAListView();
        listView.setAutoRowHeight(true);
        listView.setDock(SDAConsts.dsFill);
        listView.setVisible(false);
        listView.setShowItemText(true);
        listView.setShowItemRect(true);
        listView.setColCount(10);
        AddControl(listView);
        SBClistView = new SDAListView();
        SBClistView.setAutoRowHeight(true);
        SBClistView.setDock(SDAConsts.dsFill);
        SBClistView.setVisible(false);
        SBClistView.setShowItemText(true);
        SBClistView.setShowItemRect(true);
        SBClistView.setColCount(10);
        AddControl(SBClistView);
        //选择
        listView.setOnSelectItem(new ListViewSelectItemEvent() {

            public void Event(SDAListViewItem item) {
                //选择item,插入当前组件
                if (control != null) {
                    if (control instanceof InputInterface) {
                        ((InputInterface) control).InputVisibleString(item.getItemString());
                    }
                }
                form.Close();
            }
        });
        SBClistView.setOnSelectItem(new ListViewSelectItemEvent() {

            public void Event(SDAListViewItem item) {
                //选择item,插入当前组件
                if (control != null) {
                    if (control instanceof InputInterface) {
                        ((InputInterface) control).InputVisibleString(item.getItemString());
                    }
                }
                form.Close();
            }
        });
    }

    //打开
    public void open(SDABaseControl control) {
        if (symbolChars == null) {
            symbolChars = Application.inputPanel.getSymbolChars();
        }
        if (symbolSBCChars == null) {
            symbolSBCChars = Application.inputPanel.getSBCSymbolChars();
        }
        if (!Application.inputPanel.isSBCCase()) {
            if (listView.getItemCount() == 0) {
                for (int i = 0; i < symbolChars.length; i++) {
                    listView.addItem(String.valueOf(symbolChars[i]));
                }
            }
            listView.setVisible(true);
            SBClistView.setVisible(false);
            listView.setItemIndex(0);
        } else {
            if (SBClistView.getItemCount() == 0) {
                for (int i = 0; i < symbolSBCChars.length; i++) {
                    SBClistView.addItem(String.valueOf(symbolSBCChars[i]));
                }
            }
            listView.setVisible(false);
            SBClistView.setVisible(true);
            SBClistView.setItemIndex(0);
        }
        this.control = control;
        Show();
    }
}
