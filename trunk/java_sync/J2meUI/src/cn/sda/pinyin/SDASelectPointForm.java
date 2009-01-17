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
public class SDASelectPointForm extends SDAForm {

    //用ListView来选择
    private SDAListView listView = null;
    private SDAListView SBClistView = null;
    private char[] pointChars = null;
    private char[] pointSBCChars = null;
    private SDABaseControl control = null;

    //单实例
    private static SDASelectPointForm instance;

    synchronized public static SDASelectPointForm getInstance() {
        if (instance == null) {
            instance = new SDASelectPointForm();
        }
        return instance;
    }

    protected SDASelectPointForm() {
        super("插入标点");
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
