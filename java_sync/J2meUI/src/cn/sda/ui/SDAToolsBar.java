package cn.sda.ui;

import cn.sda.event.KeybordEvent;
import cn.sda.event.PointerEvent;
import cn.sda.event.ToolsBarClickEvent;
import cn.sda.event.ToolsBarOnChangeEvent;
import java.util.Vector;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.Font;

/**
 *
 * @author Administrator
 */
public class SDAToolsBar extends SDABaseControl {

    private byte buttonAlignType = SDAConsts.ktHorizontal;
    private int borderColor = SDAConsts.clBlack;
    //选中按钮颜色
    private int focusButtonBackColor = SDAConsts.clWhite;
    private int focusButtonForeColor = SDAConsts.clBlack;
    //是否显示标题
    private boolean showCaption = false;
    //是否显示按钮的边框
    private boolean showButtonRect = true;
    //是否按照标题自动伸缩按钮
    private boolean autoSizeButton = true;
    //组件列表
    private Vector controlList = null;
    //当前位置
    private SDAToolButton curButton = null;
    //事件
    private ToolsBarClickEvent onClickEvent = null;
    private ToolsBarOnChangeEvent onChangeEvent = null;
    //背景图
    private Image backImage = null;
    private boolean stretchImage = true;
    //按钮图片排布方式
    private int glyphAlignType = SDAConsts.blGlyphTop;
    //按钮的缺省尺寸
    private int buttonSize = 40;

    public SDAToolsBar() {
        super();
        setWidth(120);
        setHeight(22);
        setDock(SDAConsts.dsTop);
        controlList = new Vector();
        setOnKeyDown(new KeybordEvent() {

            public void Event(SDABaseControl ctrl, int keyCode) {
                //键盘
                doKeydown(keyCode);
            }
        });
        setOnPointerPressed(new PointerEvent() {

            public void Event(SDABaseControl ctrl, int x, int y) {
                //点
                doPointerPress(x, y);
            }
        });
    }

    public void paint() {
        if (!IsCanPaint()) {
            return;
        }
        Graphics g = form.getGraphics();
        SetClip(g);
        g.setFont(getFont());
        int thisWidth = getWidth();
        int thisHeight = getHeight();
        //框
        if (!transparent) {
            g.setColor(backColor);
            fillRect(g, 0, 0, thisWidth, thisHeight);
        }
        g.setColor(borderColor);
        drawRect(g, 0, 0, thisWidth, thisHeight);
        //画子组件
        PaintChilds();
        //画按钮和分割线
        paintButton(g);
        //焦点
        if (isFoucsed()) {
            g.setColor(SDAConsts.clFocusShadow);
            drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
        }
        PaintChilds();
    }
    //计算设置按钮的宽度
    private void setButtonSize() {
        SDAToolButton button = null;
        int buttonHeight = getHeight() - 4;
        int buttonWidth = getWidth() - 4;
        Font ft = getFont();
        int fontheight = ft.getHeight();
        int num = 0;
        int buttonPos = 2;
        for (int i = 0; i < controlList.size(); i++) {
            if (controlList.elementAt(i) instanceof SDAToolButton) {
                button = (SDAToolButton) controlList.elementAt(i);
                if (autoSizeButton) {
                    if (buttonAlignType == SDAConsts.ktHorizontal) {
                        button.setHeight(buttonHeight);
                        button.setTop(2);
                        button.setLeft(buttonPos);
                        if (button.getImage() != null && button.getCaption().length() > 0 &&
                                (glyphAlignType == SDAConsts.blGlyphLeft || glyphAlignType == SDAConsts.blGlyphRight)) {
                            button.setWidth(ft.stringWidth(button.getCaption() + "xx") + button.getImage().getWidth());
                        } else {
                            num = ft.stringWidth(button.getCaption() + "xx");
                            if (button.getImage() != null) {
                                if (num < button.getImage().getWidth()) {
                                    num = button.getImage().getWidth();
                                }
                            }
                            button.setWidth(num);
                        }
                        buttonPos += button.getWidth() + 2;
                    } else {
                        button.setLeft(2);
                        button.setTop(buttonPos);
                        button.setWidth(buttonWidth);
                        if (button.getImage() != null && button.getCaption().length() > 0 &&
                                (glyphAlignType == SDAConsts.blGlyphTop || glyphAlignType == SDAConsts.blGlyphBottom)) {
                            button.setHeight(fontheight + 4 + button.getImage().getHeight());
                        } else {
                            num = fontheight + 2;
                            if (button.getImage() != null) {
                                if (num < button.getImage().getHeight() + 2) {
                                    num = button.getImage().getHeight() + 2;
                                }
                            }
                            button.setHeight(num);
                        }
                        buttonPos += button.getHeight() + 2;
                    }
                } else {
                    if (buttonAlignType == SDAConsts.ktHorizontal) {
                        button.setTop(2);
                        button.setLeft(buttonPos);
                        button.setHeight(buttonHeight);
                        button.setWidth(buttonSize);
                        buttonPos += button.getWidth() + 2;
                    } else {
                        button.setLeft(2);
                        button.setTop(buttonPos);
                        button.setWidth(buttonWidth);
                        button.setHeight(buttonSize);
                        buttonPos += button.getHeight() + 2;
                    }
                }
            } else {
                if (buttonAlignType == SDAConsts.ktHorizontal) {
                    buttonPos += ((SDAToolSeperator) controlList.elementAt(i)).getWidth();
                } else {
                    buttonPos += ((SDAToolSeperator) controlList.elementAt(i)).getHeight();
                }
            }
        }
    }
    //画按钮和分割
    private void paintButton(Graphics g) {
        int thisWidth = getWidth();
        int thisHeight = getHeight();
        Font ft = getFont();
        int fontHeight = ft.getHeight();
        //设置大小
        setButtonSize();
        //画
        int buttonPos = 2;
        SDAToolButton button = null;
        SDAToolSeperator sp = null;
        Image image = null;
        String caption = "";
        //画背景
        if (backImage != null) {
            if (stretchImage) {
                image = SDAImageUtils.processImage(backImage, thisWidth, thisHeight, SDAImageUtils.MODE_STRETCH);
            } else {
                image = backImage;
            }
            drawImage(g, image, 0, 0, 0);
        }
        if (buttonAlignType == SDAConsts.ktHorizontal) {
            for (int i = 0; i < controlList.size(); i++) {
                if (controlList.elementAt(i) instanceof SDAToolButton) {
                    button = (SDAToolButton) controlList.elementAt(i);
                    caption = button.getCaption();
                    if (showButtonRect) {
                        g.setColor(borderColor);
                        drawRect(g, buttonPos, 2, button.getWidth(), button.getHeight());
                    }
                    SetClip(g, buttonPos, 2, button.getWidth(), button.getHeight());
                    image = button.getImage();
                    //焦点按钮
                    if (button.equals(curButton)) {
                        g.setColor(focusButtonBackColor);
                        fillRect(g, buttonPos + 1, 3, button.getWidth() - 1, button.getHeight() - 1);
                        g.setColor(focusButtonForeColor);
                    }
                    if (image != null) {
                        if (showCaption && caption.length() > 0) {
                            if (glyphAlignType == SDAConsts.blGlyphTop) {
                                drawImage(g, image, buttonPos + (button.getWidth() - image.getWidth()) / 2 + 1,
                                        (button.getHeight() - image.getHeight() - fontHeight) / 2 + 3, 0);
                                drawString(g, caption, buttonPos + (button.getWidth() - ft.stringWidth(caption)) / 2 + 1,
                                        (button.getHeight() - image.getHeight() - fontHeight) / 2 + image.getHeight() + 3);
                            }
                            if (glyphAlignType == SDAConsts.blGlyphBottom) {
                                drawImage(g, image, buttonPos + (button.getWidth() - image.getWidth()) / 2 + 1,
                                        (button.getHeight() - image.getHeight() - fontHeight) / 2 + fontHeight + 3, 0);
                                drawString(g, caption, buttonPos + (button.getWidth() - ft.stringWidth(caption)) / 2 + 1,
                                        (button.getHeight() - image.getHeight() - fontHeight) / 2 + 3);
                            }
                            if (glyphAlignType == SDAConsts.blGlyphLeft) {
                                drawImage(g, image, buttonPos + (button.getWidth() - image.getWidth() - ft.stringWidth(caption) - 2) / 2 + 1,
                                        (button.getHeight() - image.getHeight()) / 2 + 3, 0);
                                drawString(g, caption, buttonPos + (button.getWidth() - ft.stringWidth(caption) - image.getWidth() - 2) / 2 + image.getWidth() + 3,
                                        (button.getHeight() - fontHeight) / 2 + 3);
                            }
                            if (glyphAlignType == SDAConsts.blGlyphRight) {
                                drawImage(g, image, buttonPos + (button.getWidth() - image.getWidth() - ft.stringWidth(caption) - 2) / 2 + ft.stringWidth(caption) + 3,
                                        (button.getHeight() - image.getHeight()) / 2 + 3, 0);
                                drawString(g, caption, buttonPos + (button.getWidth() - ft.stringWidth(caption) - image.getWidth() - 2) / 2 + 1,
                                        (button.getHeight() - fontHeight) / 2 + 3);
                            }

                        } else {
                            drawImage(g, image, buttonPos + (button.getWidth() - image.getWidth()) / 2 + 1,
                                    (button.getHeight() - image.getHeight()) / 2 + 3, 0);
                        }
                    } else {
                        if (showCaption) {
                            g.setColor(foreColor);
                            drawString(g, button.getCaption(), buttonPos + (button.getWidth() - ft.stringWidth(button.getCaption())) / 2 + 1,
                                    (button.getHeight() - fontHeight) / 2 + 3);
                        }
                    }
                    SetClip(g);
                    buttonPos += button.getWidth() + 2;
                } else {
                    sp = (SDAToolSeperator) controlList.elementAt(i);
                    if (!showButtonRect) {
                        g.setColor(borderColor);
                        drawLine(g, buttonPos + sp.getWidth() / 2 - 1, 4, buttonPos + sp.getWidth() / 2 - 1, thisHeight - 4);
                        g.setColor(0x00ffffff);
                        drawLine(g, buttonPos + sp.getWidth() / 2, 4, buttonPos + sp.getWidth() / 2, thisHeight - 4);
                    }
                    buttonPos += sp.getWidth();
                }
            }
        }

        if (buttonAlignType == SDAConsts.ktVertical) {
            for (int i = 0; i <
                    controlList.size(); i++) {
                if (controlList.elementAt(i) instanceof SDAToolButton) {
                    button = (SDAToolButton) controlList.elementAt(i);
                    caption = button.getCaption();
                    if (showButtonRect) {
                        g.setColor(borderColor);
                        drawRect(g, 2, buttonPos, button.getWidth(), button.getHeight());
                    }
                    SetClip(g, 2, buttonPos, button.getWidth(), button.getHeight());
                    image = button.getImage();
                    //焦点按钮
                    if (button.equals(curButton)) {
                        g.setColor(focusButtonBackColor);
                        fillRect(g, 3, buttonPos + 1, button.getWidth() - 1, button.getHeight() - 1);
                        g.setColor(focusButtonForeColor);
                    }
                    if (image != null) {
                        if (showCaption && caption.length() > 0) {
                            if (glyphAlignType == SDAConsts.blGlyphTop) {
                                drawImage(g, image, (button.getWidth() - image.getWidth()) / 2 + 3,
                                        buttonPos + (button.getHeight() - image.getHeight() - fontHeight) / 2 + 1, 0);
                                drawString(g, caption, (button.getWidth() - ft.stringWidth(caption)) / 2 + 3,
                                        buttonPos + (button.getHeight() - image.getHeight() - fontHeight) / 2 + image.getHeight() + 1);
                            }
                            if (glyphAlignType == SDAConsts.blGlyphBottom) {
                                drawImage(g, image, (button.getWidth() - image.getWidth()) / 2 + 3,
                                        buttonPos + (button.getHeight() - image.getHeight() - fontHeight) / 2 + fontHeight + 1, 0);
                                drawString(g, caption, (button.getWidth() - ft.stringWidth(caption)) / 2 + 3,
                                        buttonPos + (button.getHeight() - image.getHeight() - fontHeight) / 2 + 1);
                            }
                            if (glyphAlignType == SDAConsts.blGlyphLeft) {
                                drawImage(g, image, (button.getWidth() - image.getWidth() - ft.stringWidth(caption) - 2) / 2 + 3,
                                        buttonPos + (button.getHeight() - image.getHeight()) / 2 + 1, 0);
                                drawString(g, caption, (button.getWidth() - ft.stringWidth(caption) - image.getWidth() - 2) / 2 + image.getWidth() + 5,
                                        buttonPos + (button.getHeight() - fontHeight) / 2 + 1);
                            }
                            if (glyphAlignType == SDAConsts.blGlyphRight) {
                                drawImage(g, image, (button.getWidth() - image.getWidth() - ft.stringWidth(caption) - 2) / 2 + ft.stringWidth(caption) + 5,
                                        buttonPos + (button.getHeight() - image.getHeight()) / 2 + 1, 0);
                                drawString(g, caption, (button.getWidth() - ft.stringWidth(caption) - image.getWidth() - 2) / 2 + 3,
                                        buttonPos + (button.getHeight() - fontHeight) / 2 + 1);
                            }

                        } else {
                            drawImage(g, image, (button.getWidth() - image.getWidth()) / 2 + 3,
                                    buttonPos + (button.getHeight() - image.getHeight()) / 2 + 1, 0);
                        }
                    } else {
                        if (showCaption) {
                            g.setColor(foreColor);
                            //打字
                            drawString(g, String.valueOf(caption), (button.getWidth() - ft.stringWidth(caption)) / 2 + 3,
                                    buttonPos + 2);
                        }
                    }
                    SetClip(g);
                    buttonPos += button.getHeight() + 2;
                } else {
                    sp = (SDAToolSeperator) controlList.elementAt(i);
                    if (!showButtonRect) {
                        g.setColor(borderColor);
                        drawLine(g, 4, buttonPos + sp.getHeight() / 2 - 1, thisWidth - 4, buttonPos + sp.getHeight() / 2 - 1);
                        g.setColor(0x00ffffff);
                        drawLine(g, 4, buttonPos + sp.getHeight() / 2, thisWidth - 4, buttonPos + sp.getHeight() / 2);
                    }
                    buttonPos += sp.getHeight();
                }
            }
        }
    }

    //增加button
    public void addButton(SDAToolButton button) {
        if (!controlList.contains(button)) {
            controlList.addElement(button);
        }

    }

    public SDAToolButton addButton() {
        SDAToolButton button = new SDAToolButton();
        controlList.addElement(button);
        repaintControl();
        return button;
    }

    public SDAToolButton addButton(
            String caption) {
        SDAToolButton button = new SDAToolButton();
        button.setCaption(caption);
        controlList.addElement(button);
        repaintControl();
        return button;
    }

    public SDAToolButton addButton(Image image) {
        SDAToolButton button = new SDAToolButton();
        button.setImage(image);
        controlList.addElement(button);
        repaintControl();
        return button;
    }

    public SDAToolButton addButton(String caption, Image image) {
        SDAToolButton button = new SDAToolButton();
        button.setCaption(caption);
        button.setImage(image);
        controlList.addElement(button);
        repaintControl();
        return button;
    }
//增加分割
    public void addSeparator(SDAToolSeperator seperator) {
        if (!controlList.contains(seperator)) {
            controlList.addElement(seperator);
            repaintControl();
        }
    }

    public SDAToolSeperator addSeparator() {
        SDAToolSeperator sp = new SDAToolSeperator();
        controlList.addElement(sp);
        repaintControl();
        return sp;
    }

    public int getBorderColor() {
        return borderColor;
    }

    public void setBorderColor(int borderColor) {
        this.borderColor = borderColor;
    }

    public boolean isAutoSizeButton() {
        return autoSizeButton;
    }

    public void setAutoSizeButton(boolean autoSizeButton) {
        this.autoSizeButton = autoSizeButton;
    }

    public byte getButtonAlignType() {
        return buttonAlignType;
    }

    public void setButtonAlignType(byte buttonAlignType) {
        this.buttonAlignType = buttonAlignType;
    }

    public boolean isShowCaption() {
        return showCaption;
    }

    public void setShowCaption(boolean showCaption) {
        this.showCaption = showCaption;
    }

    public boolean isShowButtonRect() {
        return showButtonRect;
    }

    public void setShowButtonRect(boolean showButtonRect) {
        this.showButtonRect = showButtonRect;
    }

    public Image getBackImage() {
        return backImage;
    }

    public void setBackImage(Image backImage) {
        this.backImage = backImage;
    }

    public boolean isStretchImage() {
        return stretchImage;
    }

    public void setStretchImage(boolean stretchImage) {
        this.stretchImage = stretchImage;
    }

    public int getGlyphAlignType() {
        return glyphAlignType;
    }

    public void setGlyphAlignType(int glyphAlignType) {
        this.glyphAlignType = glyphAlignType;
    }

    public int getButtonSize() {
        return buttonSize;
    }

    public void setButtonSize(int buttonSize) {
        this.buttonSize = buttonSize;
    }

//组件处理
    public void clear() {
        controlList.removeAllElements();
    }

    private void prior() {
        //向前滚动
        int index = controlList.indexOf(curButton);
        if (index > 0) {
            for (int i = index - 1; i > -1; i--) {
                if (controlList.elementAt(i) instanceof SDAToolButton) {
                    curButton = (SDAToolButton) controlList.elementAt(i);
                    doChange();
                    break;
                }
            }
        }
    }

    private void next() {
        //向后滚动
        int index = controlList.indexOf(curButton);
        if (index < controlList.size() - 1) {
            for (int i = index + 1; i < controlList.size(); i++) {
                if (controlList.elementAt(i) instanceof SDAToolButton) {
                    curButton = (SDAToolButton) controlList.elementAt(i);
                    doChange();
                    break;
                }
            }
        }
    }

    private void doKeydown(int keyCode) {
        String key = form.getKeyName(keyCode).toUpperCase();
        if ((key.equals(SDAConsts.KEY_LEFT) && (buttonAlignType == SDAConsts.ktHorizontal)) ||
                (key.equals(SDAConsts.KEY_UP) && (buttonAlignType == SDAConsts.ktVertical))) {
            //左
            prior();
            repaintControl();
        }

        if ((key.equals(SDAConsts.KEY_RIGHT) && (buttonAlignType == SDAConsts.ktHorizontal)) ||
                (key.equals(SDAConsts.KEY_DOWN) && (buttonAlignType == SDAConsts.ktVertical))) {
            //右
            next();
            repaintControl();
        }
        if (key.equals(SDAConsts.KEY_SELECT)) {
            if (curButton != null) {
                doClick();
            }
        }
    }

    private void doPointerPress(int x, int y) {
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        if (InClientRect(posx, posy, 0, 0, width, height)) {
            getButtonFromPoint(posx, posy);
            repaintControl();
            if (curButton != null) {
                doClick();
            }
        }
    }

    private void getButtonFromPoint(int x, int y) {
        int buttonPos = 2;
        boolean findButton = false;
        SDAToolButton button = null;
        if (buttonAlignType == SDAConsts.ktHorizontal) {
            //水平方向，判断x(相对坐标)
            for (int i = 0; i <
                    controlList.size(); i++) {
                if (controlList.elementAt(i) instanceof SDAToolButton) {
                    button = (SDAToolButton) controlList.elementAt(i);
                    //判断是否在中间
                    if (x > buttonPos && x < buttonPos + button.getWidth()) {
                        curButton = button;
                        findButton = true;
                        break;
                    }
                    buttonPos += button.getWidth() + 2;
                } else {
                    buttonPos += ((SDAToolSeperator) controlList.elementAt(i)).getWidth();
                }
            }
        }
        if (buttonAlignType == SDAConsts.ktVertical) {
            //水平方向，判断x(相对坐标)
            for (int i = 0; i <
                    controlList.size(); i++) {
                if (controlList.elementAt(i) instanceof SDAToolButton) {
                    button = (SDAToolButton) controlList.elementAt(i);
                    //判断是否在中间
                    if (y > buttonPos && y < buttonPos + button.getHeight()) {
                        curButton = button;
                        findButton = true;
                        break;
                    }
                    buttonPos += button.getHeight() + 2;
                } else {
                    buttonPos += ((SDAToolSeperator) controlList.elementAt(i)).getHeight();
                }
            }
        }
        if (!findButton) {
            curButton = null;
        }
    }

    public void setOnClickEvent(ToolsBarClickEvent onClickEvent) {
        this.onClickEvent = onClickEvent;
    }

    private void doClick() {
        if (onClickEvent != null) {
            onClickEvent.Event(curButton, controlList.indexOf(curButton));
        }
    }

    public void setOnChangeEvent(ToolsBarOnChangeEvent onChangeEvent) {
        this.onChangeEvent = onChangeEvent;
    }

    private void doChange() {
        if (onChangeEvent != null) {
            onChangeEvent.Event(curButton);
        }
    }

    private boolean isFirst() {
        boolean result = true;
        int id = controlList.indexOf(curButton);
        for (int i = 0; i < id; i++) {
            if (controlList.elementAt(i) instanceof SDAToolButton) {
                result = false;
            }
        }
        return result;
    }

    private boolean isLast() {
        boolean result = true;
        int id = controlList.indexOf(curButton);
        for (int i = id + 1; i < controlList.size(); i++) {
            if (controlList.elementAt(i) instanceof SDAToolButton) {
                result = false;
            }
        }
        return result;
    }

    protected boolean canDownTabNext() {
        if (buttonAlignType == SDAConsts.ktHorizontal) {
            return true;
        } else {
            boolean result = false;
            if (isLast()) {
                result = true;
            }
            return result;
        }
    }

    protected boolean canLeftTabPrior() {
        if (buttonAlignType == SDAConsts.ktHorizontal) {
            boolean result = false;
            if (isFirst()) {
                result = true;
            }
            return result;
        } else {
            return true;
        }
    }

    protected boolean canRightTabNext() {
        if (buttonAlignType == SDAConsts.ktHorizontal) {
            boolean result = false;
            if (isLast()) {
                result = true;
            }
            return result;
        } else {
            return true;
        }
    }

    protected boolean canUpTabPrior() {
        if (buttonAlignType == SDAConsts.ktHorizontal) {
            return true;
        } else {
            boolean result = false;
            if (isFirst()) {
                result = true;
            }
            return result;
        }
    }

    public int getFocusButtonBackColor() {
        return focusButtonBackColor;
    }

    public void setFocusButtonBackColor(int focusButtonBackColor) {
        this.focusButtonBackColor = focusButtonBackColor;
    }

    public int getFocusButtonForeColor() {
        return focusButtonForeColor;
    }

    public void setFocusButtonForeColor(int focusButtonForeColor) {
        this.focusButtonForeColor = focusButtonForeColor;
    }
}
