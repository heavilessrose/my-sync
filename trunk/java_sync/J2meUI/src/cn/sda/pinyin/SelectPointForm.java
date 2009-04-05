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
public class SelectPointForm extends FormUI {

    //用ListView来选择
    private ListViewUI listView = null;
    private ListViewUI SBClistView = null;
    private char[] pointChars = null;
    private char[] pointSBCChars = null;
    private BaseControl control = null;

    //单实例
    private static SelectPointForm instance;

    synchronized public static SelectPointForm getInstance() {
        if (instance == null) {
            instance = new SelectPointForm();
        }
        return instance;
    }

    protected SelectPointForm() {
        super("插入标点");
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
        if (pointChars == null) {
            pointChars = Application.inputPanel.getPointChars();
        }
        if (pointSBCChars == null) {
            pointSBCChars = Application.inputPanel.getSBCPointChars();
        }
        if (!Application.inputPanel.isSBCCase()) {
            if (listView.getItemCount() == 0) {
                for (int i = 0; i < pointChars.length; i++) {
                    listView.addItem(String.valueOf(pointChars[i]));
                }
            }
            listView.setItemIndex(0);
            listView.setVisible(true);
            SBClistView.setVisible(false);
        } else {
            if (SBClistView.getItemCount() == 0) {
                for (int i = 0; i < pointSBCChars.length; i++) {
                    SBClistView.addItem(String.valueOf(pointSBCChars[i]));
                }
            }
            SBClistView.setItemIndex(0);
            listView.setVisible(false);
            SBClistView.setVisible(true);
        }
        this.control = control;
        Show();
    }
}
