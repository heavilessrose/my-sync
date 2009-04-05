package cn.sda.ui;

import cn.sda.event.KeybordEvent;
import cn.sda.event.PointerEvent;
import cn.sda.pinyin.InputInterface;
import javax.microedition.lcdui.Graphics;

/**
 * <p>Title: </p>
 *
 * <p>Description: </p>
 *
 * <p>Copyright: Copyright (c) 2008</p>
 *
 * <p>Company: </p>
 *
 * @author not attributable
 * @version 1.0
 */
public class ComboBoxUI extends BaseControl implements InputInterface {
    //边框
    private int borderStyle = ConstsUI.bsNone;
    //隐藏选定内容
    private boolean hideSelection = true;
    //最大长度
    private int maxLength = 32767;
    //文字对齐方式
    private int textAlign = ConstsUI.alignLeft;
    //边框线颜色
    private int borderColor = ConstsUI.clBlack;
    //显示行数
    private int dropDownCount = 4;
    //下拉类型
    private int dropDownStyle = ConstsUI.stDropDowm;
    //内部变量
    //下拉选择框,用ListBox
    private Items items = null;
    //文字显示的Edit
    private BaseEdit TextEdit = null;


    //选择项
    public class Items extends ListBoxUI {
    }

    public ComboBoxUI() {
        super();
        internalSDAComboBox();
    }

    //项目处理
    public void addItem(String value) {
        items.addItem(value, null, false);
    }

    public void deleteItem(int itemIndex) {
        items.deleteItem(itemIndex);
    }

    public void clear() {
        items.clear();
    }

    public int getCount() {
        return items.getItemCount();
    }

    public int getItemIndex() {
        return getItemIndex();
    }

    public String getValue(int itemIndex) {
        return items.getItemString(itemIndex);
    }

    public void setItemIndex(int itemIndex) {
        items.setItemIndex(itemIndex);
    }

    public void setItemString(int itemIndex, String itemString) {
        items.setItemString(itemIndex, itemString);
    }

    private void internalSDAComboBox() {
        //导航
        setVisible(true);
        setLeft(0);
        setTop(0);
        setBorderStyle(ConstsUI.bsFixedSingle);
        super.setWidth(60);
        tabStop = false;
        //下拉框
        items = new Items();
        items.setVisible(false);
        items.setBorderStyle(ConstsUI.bsFixedSingle);
        items.setScrollBars(ConstsUI.srVertical);
        //文字显示
        TextEdit = new BaseEdit();
        TextEdit.setBorderStyle(ConstsUI.srNone);
        AddControl(TextEdit);
        //高度根据字体高度来
        setHeight(getFont().getHeight() + 4);
        super.setBackColor(ConstsUI.clWhite);
        onPointerPressed = new PointerEvent() {

            public void Event(BaseControl ctrl, int x, int y) {
                doPointerPressed(x, y);
            }
        };
        onPointerReleased = new PointerEvent() {

            public void Event(BaseControl ctrl, int x, int y) {
                doPointerReleased(x, y);
            }
        };
        onKeyUp = new KeybordEvent() {

            public void Event(BaseControl ctrl, int keyCode) {
                doKeyUp(keyCode);
            }
        };
    }

    public void paint() {
        if (IsCanPaint()) {
            internalPaint();
            PaintChilds();
        }
    }

    private void internalPaint() {
        Graphics g = form.getGraphics();
        //可视下才画
        if (isVisible()) {
            //初始化引用变量
            int thisWidth = getWidth();
            int thisHeight = getHeight();
            //对齐方式处理后，画组件            
            //设置字体
            g.setFont(super.getFont());
            //外框
            SetClip(g);
            //透明
            if (!isTransparent()) {
                g.setColor(backColor);
                fillRect(g, 0, 0, thisWidth, thisHeight);
            }
            if (borderStyle == ConstsUI.bsFixed3D) {
                g.setColor(ConstsUI.clGray);
                drawLine(g, 0, 0, thisWidth, 0);
                drawLine(g, 0, 0, 0, thisHeight);
                g.setColor(ConstsUI.clBlack);
                drawLine(g, 1, 1, thisWidth - 1, 1);
                drawLine(g, 1, 1, 1, thisHeight - 1);
                g.setColor(ConstsUI.clBtnFace);
                drawLine(g, 2, thisHeight - 1, thisWidth - 2, thisHeight - 1);
                drawLine(g, thisWidth - 1, 2, thisWidth - 1, thisHeight - 1);
                g.setColor(ConstsUI.clWhite);
                drawLine(g, 1, thisHeight, thisWidth, thisHeight);
                drawLine(g, thisWidth, 1, thisWidth, thisHeight);
            /*
            if (isFoucsed()) {
            g.setColor(SDAConsts.clFocusShadow);
            drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
            }
             */
            }
            if (borderStyle == ConstsUI.bsFixedSingle) {
                //边框
                g.setColor(getBorderColor());
                drawRect(g, 0, 0, thisWidth, thisHeight);
            }
            //下拉箭头
            g.setColor(getBorderColor());
            fillTriangle(g, thisWidth - thisHeight + thisHeight / 2 - 4, thisHeight / 2 - 2,
                    thisWidth - thisHeight + thisHeight / 2 + 4, thisHeight / 2 - 2,
                    thisWidth - thisHeight + thisHeight / 2, thisHeight / 2 + 2);

            //打字
            g.setColor(getForeColor());
            setTextEditPos();
            //文字对齐方式
            if (textAlign == ConstsUI.alignLeft) {
                TextEdit.setTextAlign(ConstsUI.alignLeft);
            }
            if (textAlign == ConstsUI.alignCenter) {
                TextEdit.setTextAlign(ConstsUI.alignCenter);
            }
            if (textAlign == ConstsUI.alignRight) {
                TextEdit.setTextAlign(ConstsUI.alignRight);
            }
            if (isFoucsed() && dropDownStyle == ConstsUI.stDropDowmList) {
                TextEdit.backColor = ConstsUI.clFocusShadow;
                TextEdit.foreColor = ConstsUI.clWhite;
            } else {
                TextEdit.backColor = ConstsUI.clWhite;
                TextEdit.foreColor = ConstsUI.clBlack;
            }
            //下拉框
            items.InternalPaint(g);
        }
    }
    //设置位置
    private void setTextEditPos() {
        TextEdit.left = 1;
        TextEdit.top = 1;
        TextEdit.width = width - height;
        TextEdit.height = height - 1;
    }

    public int getHeight() {
        return internalGetHeight();
    }

    private int internalGetHeight() {
        int sheight = getFont().getHeight() + 3;
        return sheight;
    }

    public void setHeight(int height) {
        internalSetHeight(height);
    }

    private void internalSetHeight(int height) {
        int sheight = getFont().getHeight() + 3;
        paint();
        super.setHeight(sheight);
    }

    public int getBorderStyle() {
        return borderStyle;
    }

    public void setBorderStyle(int borderStyle) {
        this.borderStyle = borderStyle;
        paint();
    }

    public boolean isHideSelection() {
        return hideSelection;
    }

    public void setHideSelection(boolean hideSelection) {
        this.hideSelection = hideSelection;
        paint();
    }

    public int getMaxLength() {
        return maxLength;
    }

    public void setMaxLength(int maxLength) {
        this.maxLength = maxLength;
        //内容处理
        TextEdit.setText(TextEdit.getText());
        paint();
    }

    public boolean isReadOnly() {
        return TextEdit.isReadOnly();
    }

    public void setReadOnly(boolean readOnly) {
        TextEdit.setReadOnly(readOnly);
    }

    public int getTextAlign() {
        return textAlign;
    }

    public void setTextAlign(int textAlign) {
        this.textAlign = textAlign;
        paint();
    }

    public String getText(){
        return TextEdit.text;
    }
    public void setText(String text) {
        internalSetText(text);
    }

    private void internalSetText(String text) {
        //判断最大长度
        if (TextEdit.text.length() > getMaxLength()) {
            TextEdit.text = TextEdit.text.substring(0, getMaxLength());
        }
        paint();
    }

    public int getBorderColor() {
        return borderColor;
    }

    public void setBorderColor(int borderColor) {
        this.borderColor = borderColor;
        paint();
    }

    public int getDorpDownCount() {
        return dropDownCount;
    }

    public void setDorpDownCount(int dorpDownCount) {
        this.dropDownCount = dorpDownCount;
    }

    public int getDropDownStyle() {
        return dropDownStyle;
    }

    public void setDropDownStyle(int dropDownStyle) {
        this.dropDownStyle = dropDownStyle;
        if (dropDownStyle == ConstsUI.stDropDowm) {
            tabStop = false;
            TextEdit.tabStop = true;
            TextEdit.enabled = true;
        } else {
            tabStop = true;
            TextEdit.tabStop = false;
            TextEdit.enabled = false;
        }
        paint();
    }

    //处理事件的执行
    private void setVisibleItems() {
        int thisHeight = super.getHeight();
        int fontHeight = getFont().getHeight();
        this.setPopVisible();
        //计算列表框位置大小
        items.setVisible(false);
        items.setLeft(getLeft()+1);
        items.setWidth(getWidth());
        //计算高度
        int msheight = 0;
        if (dropDownCount > items.getItemCount()) {
            msheight = items.getItemCount() * fontHeight + 3;
            items.setScrollBars(ConstsUI.srNone);
        } else {
            msheight = dropDownCount * fontHeight + 3;
            items.setScrollBars(ConstsUI.srVertical);
        }
        if (msheight > form.getHeight()) {
            msheight = form.getHeight() - 1;
        }
        //计算top
        int stop = 0;
        if (msheight > form.getHeight() - getOriginTop() - thisHeight) {
            stop = getOriginTop() - msheight;
            if (stop < 0) {
                stop = 0;
            }
        } else {
            stop = getOriginTop() + thisHeight - 1;
        }
        items.top = stop;
        items.setHeight(msheight);
        //定位
        int itemIndex = items.getItemIndex(TextEdit.text);
        if (itemIndex > -1) {
            items.setItemIndex(itemIndex);
        }
        items.setVisible(true);
    }
    //点箭头滚动内容
    private void doPointerPressed(int x, int y) {
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        int thisWidth = getWidth();
        int thisHeight = getHeight();
        //区域
        items.form = this.form;
        if (!items.visible) {
            if (InClientRect(posx, posy, dropDownStyle == ConstsUI.stDropDowm ? thisWidth - thisHeight : 0, 0,
                    dropDownStyle == ConstsUI.stDropDowm ? thisHeight : thisWidth, thisHeight)) {
                setVisibleItems();
            }
        } else {
            if (items.InClientRect(items.screenXToClient(x), items.screenYToClient(y), 0, 0, items.getWidth(), items.getHeight())) {
                items.doPointerPressed(x, y);
                if (items.InClientRect(items.screenXToClient(x), items.screenYToClient(y), 0, 0, items.getWidth() - items.getScrollBarWidth(), items.getHeight())) {
                    TextEdit.text = items.getItemString(items.getItemIndex());
                    form.closePopCtrl();
                    items.setVisible(false);
                    form.paint();
                }
            } else {
                form.closePopCtrl();
                items.setVisible(false);
                form.paint();
            }
        }
    }
    //拖动事件处理
    private void doPointerReleased(int x, int y) {
        int posx = items.screenXToClient(x);
        int posy = items.screenYToClient(y);
        int thisWidth = items.getWidth();
        int thisHeight = items.getHeight();
        //滚动条
        if (items.InClientRect(posx, posy, thisWidth - items.getScrollBarWidth(), 0, items.getScrollBarWidth(), thisHeight)) {
            items.doPointerReleased(x, y);
        }
    }
    //键盘事件处理
    private void doKeyUp(int keyCode) {
        items.form = form;
        String key = form.getKeyName(keyCode).toUpperCase();
        if (items.visible) {
            if (key.equals(ConstsUI.KEY_UP)) {
                //上
                items.doKeyUp(keyCode);
            }

            if (key.equals(ConstsUI.KEY_DOWN)) {
                //下
                items.doKeyUp(keyCode);
            }
            if (key.equals(ConstsUI.KEY_LEFT)) {
                //左
                items.doKeyUp(keyCode);
            }
            if (key.equals(ConstsUI.KEY_RIGHT)) {
                //右
                items.doKeyUp(keyCode);
            }
        }
        if (key.equals(ConstsUI.KEY_SELECT)) {
            if (!items.visible) {
                setVisibleItems();
            } else {
                TextEdit.text = items.getItemString(items.getItemIndex());
                form.closePopCtrl();
                items.setVisible(false);
                form.repaintControl();
            }
        }
    }

    public void InputFuncChar(char inChar, boolean sbc) {
        TextEdit.inputFuncChar(inChar, sbc);
    }

    public void InputVisibleChar(char inChar) {
        TextEdit.inputVisibleChar(inChar);
    }

    public void InputVisibleString(String inString) {
        TextEdit.inputVisibleString(inString);
    }
}
