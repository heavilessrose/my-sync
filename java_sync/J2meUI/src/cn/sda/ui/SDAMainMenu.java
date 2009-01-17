package cn.sda.ui;

import cn.sda.event.HotKeyEvent;
import cn.sda.event.MainMenuOnButtonEvent;
import cn.sda.event.PointerEvent;
import cn.sda.event.PopMenuOnClickEvent;
import cn.sda.pinyin.SDASelectPointForm;
import cn.sda.pinyin.SDASelectSymbolForm;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.Font;

/**
 *
 * @author Administrator
 */
public class SDAMainMenu extends SDABaseControl {

    //背景图
    private Image backImage = null;
    //左右菜单
    private SDAPopMenu leftMemu = null;
    private SDAPopMenu rightMemu = null;
    //左右主菜单项名称
    private String leftMenuName = "";
    private String rightMenuName = "";
    //是不是显示外框
    private boolean showRect = false;
    //外框颜色
    private int borderColor = SDAConsts.clBlack;
    //背景图拉伸
    private boolean stretchImage = true;
    //显示输入法按钮
    private boolean showInputMethod = true;
    //输入法菜单
    private SDAPopMenu inputMethodMemu = null;
    //选择的当前菜单
    private String menuItemName = "拼";
    //热键事件
    HotKeyEvent hotKeyEvent = null;
    //热键是否注册
    private boolean reg = false;
    //按钮
    private MainMenuOnButtonEvent onMainMenuButtonEvent = null;
    //加入菜单
    private boolean additem = false;

    public SDAMainMenu() {
        super();
        width = 100;
        height = 25;
        tabStop = false;
        setDock(SDAConsts.dsBottom);
        inputMethodMemu = new SDAPopMenu();
        inputMethodMemu.setBackColor(SDAConsts.clWhite);
        AddControl(inputMethodMemu);
        hotKeyEvent = new HotKeyEvent() {

            public void Event(String keyName) {
                //热键处理
                doHotKeyPressed(keyName);
            }
        };
        inputMethodMemu.setOnClick(new PopMenuOnClickEvent() {

            public void Event(SDAMenuItem item, String itemName, String itemCaption) {
                //菜单设置
                if (form == null) {
                    return;
                }
                //处理
                if (itemName.equals("back")) {
                    char ch = 8;
                    form.Application.inputPanel.selectedChar = ch;
                    form.Application.inputPanel.inputFuncChar();
                    return;
                }
                if (itemName.equals("delete")) {
                    char ch = 46;
                    form.Application.inputPanel.selectedChar = ch;
                    form.Application.inputPanel.inputFuncChar();
                    return;
                }
                if (itemName.equals("enter")) {
                    char ch = 13;
                    form.Application.inputPanel.selectedChar = ch;
                    form.Application.inputPanel.inputFuncChar();
                    return;
                }
                if (itemName.equals("ABC")) {
                    form.Application.inputPanel.setImeType(SDAInputPanel.imUpperCase);
                }
                if (itemName.equals("abc")) {
                    form.Application.inputPanel.setImeType(SDAInputPanel.imLowerCase);
                }
                if (itemName.equals("123")) {
                    form.Application.inputPanel.setImeType(SDAInputPanel.imDigit);
                }
                if (itemName.equals("拼音")) {
                    form.Application.inputPanel.setImeType(SDAInputPanel.imPinYin);
                }
                if (itemName.equals("标点")) {
                    form.Application.inputPanel.setImeType(SDAInputPanel.imPoint);
                    SDASelectPointForm.getInstance().open(form.focusControl);
                    return;
                }
                if (itemName.equals("符号")) {
                    form.Application.inputPanel.setImeType(SDAInputPanel.imSymbol);
                    SDASelectSymbolForm.getInstance().open(form.focusControl);
                    return;
                }
                if (itemName.equals("全")) {
                    form.Application.inputPanel.setSBCCase(!form.Application.inputPanel.isSBCCase());
                    item.setChecked(form.Application.inputPanel.isSBCCase());
                } else {
                    item.setChecked(true);
                }
                //刷新
                form.repaintControl();
            }
        });
        //设置点击的菜单选项
        setOnPointerReleased(new PointerEvent() {

            public void Event(SDABaseControl ctrl, int x, int y) {
                doPointerPressed(x, y);
            }
        });
    }

    //获取名
    private void getMenuItemName() {
        if (form.Application.inputPanel.ImeType == SDAInputPanel.imDigit) {
            menuItemName = "123";
        }
        if (form.Application.inputPanel.ImeType == SDAInputPanel.imLowerCase) {
            menuItemName = "abc";
        }
        if (form.Application.inputPanel.ImeType == SDAInputPanel.imPinYin) {
            menuItemName = "拼音";
        }
        if (form.Application.inputPanel.ImeType == SDAInputPanel.imPoint) {
            menuItemName = "标点";
        }
        if (form.Application.inputPanel.ImeType == SDAInputPanel.imSymbol) {
            menuItemName = "符号";
        }
        if (form.Application.inputPanel.ImeType == SDAInputPanel.imUpperCase) {
            menuItemName = "ABC";
        }
        SDAMenuItem item = inputMethodMemu.getMenuItem(inputMethodMemu, menuItemName);
        if (item != null && !item.itemName.equals("back") && !item.itemName.equals("delete") && !item.itemName.equals("enter")) {
            item.setChecked(true);
        }
        inputMethodMemu.getMenuItem(inputMethodMemu, "全").setChecked(form.Application.inputPanel.isSBCCase());
    }

    public void paint() {
        if (!IsCanPaint()) {
            return;
        }
        Graphics g = form.getGraphics();
        //加入
        if (!additem) {
            if (!form.Application.hasPointer()) {
                inputMethodMemu.AddItem("back", "退格");
                inputMethodMemu.AddItem("delete", "删除");
                inputMethodMemu.AddItem("enter", "插入回车");
                inputMethodMemu.AddItem("", "-");
            }
            inputMethodMemu.AddItem("ABC", "大写字母").setRadioItem(true);
            inputMethodMemu.AddItem("abc", "小写字母").setRadioItem(true);
            inputMethodMemu.AddItem("123", "数字").setRadioItem(true);
            inputMethodMemu.AddItem("拼音", "中文拼音").setRadioItem(true);
            inputMethodMemu.AddItem("标点", "插入标点").setRadioItem(true);
            inputMethodMemu.AddItem("符号", "插入符号").setRadioItem(true);
            inputMethodMemu.AddItem("", "-");
            inputMethodMemu.AddItem("全", "全角");
            additem = true;
        }
        g.setFont(getFont());
        //注册键盘信息
        if (!reg) {
            form.registerHotKey(SDAConsts.KEY_SOFT1, hotKeyEvent);
            form.registerHotKey(SDAConsts.KEY_SOFT2, hotKeyEvent);
            reg = true;
        }
        //判断是否显示输入法（触摸屏的显示）
        SetClip(g);
        //画
        if (!transparent) {
            g.setColor(backColor);
            fillRect(g, 0, 0, width, height);
        }
        if (showRect) {
            g.setColor(borderColor);
            drawRect(g, 0, 0, width, height);
        }
        paintMainMenu(g);
    }
    //画主菜单
    private void paintMainMenu(Graphics g) {
        Font ft = getFont();
        //背景
        if (backImage != null) {
            Image image = null;
            if (stretchImage) {
                image = SDAImageUtils.processImage(backImage, width, height, SDAImageUtils.MODE_STRETCH);
            } else {
                image = backImage;
            }
            drawImage(g, image, 0, 0, 0);
        }
        //左右菜单内容
        g.setColor(foreColor);
        drawString(g, leftMenuName, (2 * width / 5 - ft.stringWidth(leftMenuName)) / 2, (height - ft.getHeight()) / 2);
        drawString(g, rightMenuName, 3 * width / 5 + (2 * width / 5 - ft.stringWidth(rightMenuName)) / 2, (height - ft.getHeight()) / 2);
        //画输入法
        if (showInputMethod) {
            getMenuItemName();
            g.setColor(foreColor);
            int strLeft = 2 * width / 5 + (width / 5 - ft.stringWidth(menuItemName)) / 2 - 6;
            drawString(g, menuItemName, strLeft, (height - ft.getHeight()) / 2);
            //箭头
            fillTriangle(g, strLeft + ft.stringWidth(menuItemName) + 6, height / 2 - 2,
                    strLeft + ft.stringWidth(menuItemName) + 2, height / 2 + 2,
                    strLeft + ft.stringWidth(menuItemName) + 10, height / 2 + 2);
        }
    }
    //设置左右的菜单项目
    public SDAPopMenu getLeftMemu() {
        return leftMemu;
    }

    public void setLeftMemu(SDAPopMenu leftMemu) {
        this.leftMemu = leftMemu;
    }

    public SDAPopMenu getRightMemu() {
        return rightMemu;
    }

    public void setRightMemu(SDAPopMenu rightMemu) {
        this.rightMemu = rightMemu;
    }

    public String getLeftMenuName() {
        return leftMenuName;
    }

    public void setLeftMenuName(String leftMenuName) {
        this.leftMenuName = leftMenuName;
        repaintControl();
    }

    public String getRightMenuName() {
        return rightMenuName;
    }

    public void setRightMenuName(String rightMenuName) {
        this.rightMenuName = rightMenuName;
        repaintControl();
    }

    public Image getBackImage() {
        return backImage;
    }

    public void setBackImage(Image backImage) {
        this.backImage = backImage;
        repaintControl();
    }

    public void setDock(int dock) {
        super.setDock(SDAConsts.dsBottom);
    }

    public int getBorderColor() {
        return borderColor;
    }

    public void setBorderColor(int borderColor) {
        this.borderColor = borderColor;
        repaintControl();
    }

    public boolean isShowRect() {
        return showRect;
    }

    public void setShowRect(boolean showRect) {
        this.showRect = showRect;
        repaintControl();
    }

    public boolean isStretchImage() {
        return stretchImage;
    }

    public void setStretchImage(boolean stretchImage) {
        this.stretchImage = stretchImage;
        repaintControl();
    }
    //点击
    private void doPointerPressed(int x, int y) {
        Font ft = getFont();
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        int fontHeight = ft.getHeight();
        //判断位置
        if (showInputMethod) {
            int strLeft = 2 * width / 5 + (width / 5 - ft.stringWidth(menuItemName)) / 2 - 6;
            //字母
            if (InClientRect(posx, posy, strLeft, 0, ft.stringWidth(menuItemName), fontHeight)) {
                form.Application.inputPanel.visible = !form.Application.inputPanel.isVisible();
                form.repaintControl();
            }
            //箭头
            if (InClientRect(posx, posy, strLeft + ft.stringWidth(menuItemName), 0, 8, fontHeight)) {
                if (inputMethodMemu.isVisible()) {
                    inputMethodMemu.visible = false;
                    form.closePopCtrl();
                    form.repaintControl();
                } else {
                    if (dock == SDAConsts.dsBottom) {
                        inputMethodMemu.PopMenu(getOriginLeft() + (width - inputMethodMemu.getClientWidth()) / 2, getOriginTop());
                    }
                }
            }
        }
        //判断左右菜单
        if (InClientRect(posx, posy, 0, 0, 2 * width / 5, height)) {
            doHotKeyPressed(SDAConsts.KEY_SOFT1);
        }
        if (InClientRect(posx, posy, 3 * width / 5, 0, 2 * width / 5, height)) {
            doHotKeyPressed(SDAConsts.KEY_SOFT2);
        }
    }
    //键盘处理
    private void doHotKeyPressed(String keyName) {
        //处理热键
        if (keyName.equals(SDAConsts.KEY_SOFT1)) {
            //左键
            if (leftMemu != null) {
                if (leftMemu.isVisible()) {
                    leftMemu.visible = false;
                    form.closePopCtrl();
                    form.repaintControl();
                } else {
                    leftMemu.PopMenu(getOriginLeft(), getOriginTop());
                }
                if (onMainMenuButtonEvent != null) {
                    onMainMenuButtonEvent.Event(leftMemu, leftMenuName);
                }
            } else {
                if (leftMenuName.length() > 0) {
                    if (onMainMenuButtonEvent != null) {
                        onMainMenuButtonEvent.Event(null, leftMenuName);
                    }
                }
            }
        }
        if (keyName.equals(SDAConsts.KEY_SOFT2)) {
            if (rightMemu != null) {
                if (rightMemu.isVisible()) {
                    rightMemu.visible = false;
                    form.closePopCtrl();
                    form.repaintControl();
                } else {
                    rightMemu.PopMenu(getOriginLeft() + width - inputMethodMemu.getClientWidth(), getOriginTop());
                }
                if (onMainMenuButtonEvent != null) {
                    onMainMenuButtonEvent.Event(rightMemu, rightMenuName);
                }
            } else {
                if (rightMenuName.length() > 0) {
                    if (onMainMenuButtonEvent != null) {
                        onMainMenuButtonEvent.Event(null, rightMenuName);
                    }
                }
            }
        }
    }

    public void setOnMainMenuButtonEvent(MainMenuOnButtonEvent onMainMenuButtonEvent) {
        this.onMainMenuButtonEvent = onMainMenuButtonEvent;
    }

    public boolean isShowInputMethod() {
        return showInputMethod;
    }

    public void setShowInputMethod(boolean showInputMethod) {
        this.showInputMethod = showInputMethod;
    }
    //获取输入法菜单
    public SDAPopMenu getInputMemu() {
        return inputMethodMemu;
    }
}
