package cn.sda.pinyin;

import cn.sda.event.ListViewSelectItemEvent;
import cn.sda.ui.BaseControl;
import cn.sda.ui.ConstsUI;
import cn.sda.ui.FormUI;
import cn.sda.ui.ListViewUI;
import cn.sda.ui.ListViewItemUI;

/**
 *
 * @author Administrator
 */
public class SelectSymbolForm extends FormUI {

    //用ListView来选择
    private ListViewUI listView = null;
    private ListViewUI SBClistView = null;
    private char[] symbolChars = null;
    private char[] symbolSBCChars = null;
    private BaseControl control = null;
    //单实例
    private static SelectSymbolForm instance;

    synchronized public static SelectSymbolForm getInstance() {
        if (instance == null) {
            instance = new SelectSymbolForm();
        }
        return instance;
    }

    protected SelectSymbolForm() {
        super("插入符号");
        listView = new ListViewUI();
        listView.setAutoRowHeight(true);
        listView.setDock(ConstsUI.dsFill);
        listView.setVisible(false);
        listView.setShowItemText(true);
        listView.setShowItemRect(true);
        listView.setColCount(10);
        AddControl(listView);
        SBClistView = new ListViewUI();
        SBClistView.setAutoRowHeight(true);
        SBClistView.setDock(ConstsUI.dsFill);
        SBClistView.setVisible(false);
        SBClistView.setShowItemText(true);
        SBClistView.setShowItemRect(true);
        SBClistView.setColCount(10);
        AddControl(SBClistView);
        //选择
        listView.setOnSelectItem(new ListViewSelectItemEvent() {

            public void Event(ListViewItemUI item) {
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

            public void Event(ListViewItemUI item) {
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
    public void open(BaseControl control) {
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
