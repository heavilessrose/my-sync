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
public class ToolsBarUI extends BaseControl {

    private byte buttonAlignType = ConstsUI.ktHorizontal;
    private int borderColor = ConstsUI.clBlack;
    //选中按钮颜色
    private int focusButtonBackColor = ConstsUI.clWhite;
    private int focusButtonForeColor = ConstsUI.clBlack;
    //是否显示标题
    private boolean showCaption = false;
    //是否显示按钮的边框
    private boolean showButtonRect = true;
    //是否按照标题自动伸缩按钮
    private boolean autoSizeButton = true;
    //组件列表
    private Vector controlList = null;
    //当前位置
    private ToolButtonUI curButton = null;
    //事件
    private ToolsBarClickEvent onClickEvent = null;
    private ToolsBarOnChangeEvent onChangeEvent = null;
    //背景图
    private Image backImage = null;
    private boolean stretchImage = true;
    //按钮图片排布方式
    private int glyphAlignType = ConstsUI.blGlyphTop;
    //按钮的缺省尺寸
    private int buttonSize = 40;

    public ToolsBarUI() {
        super();
        setWidth(120);
        setHeight(22);
        setDock(ConstsUI.dsTop);
        controlList = new Vector();
        setOnKeyDown(new KeybordEvent() {

            public void Event(BaseControl ctrl, int keyCode) {
                //键盘
                doKeydown(keyCode);
            }
        });
        setOnPointerPressed(new PointerEvent() {

            public void Event(BaseControl ctrl, int x, int y) {
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
            g.setColor(ConstsUI.clFocusShadow);
            drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
        }
        PaintChilds();
    }
    //计算设置按钮的宽度
    private void setButtonSize() {
        ToolButtonUI button = null;
        int buttonHeight = getHeight() - 4;
        int buttonWidth = getWidth() - 4;
        Font ft = getFont();
        int fontheight = ft.getHeight();
        int num = 0;
        int buttonPos = 2;
        for (int i = 0; i < controlList.size(); i++) {
            if (controlList.elementAt(i) instanceof ToolButtonUI) {
                button = (ToolButtonUI) controlList.elementAt(i);
                if (autoSizeButton) {
                    if (buttonAlignType == ConstsUI.ktHorizontal) {
                        button.setHeight(buttonHeight);
                        button.setTop(2);
                        button.setLeft(buttonPos);
                        if (button.getImage() != null && button.getCaption().length() > 0 &&
                                (glyphAlignType == ConstsUI.blGlyphLeft || glyphAlignType == ConstsUI.blGlyphRight)) {
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
                                (glyphAlignType == ConstsUI.blGlyphTop || glyphAlignType == ConstsUI.blGlyphBottom)) {
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
                    if (buttonAlignType == ConstsUI.ktHorizontal) {
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
                if (buttonAlignType == ConstsUI.ktHorizontal) {
                    buttonPos += ((ToolSeperatorUI) controlList.elementAt(i)).getWidth();
                } else {
                    buttonPos += ((ToolSeperatorUI) controlList.elementAt(i)).getHeight();
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
        ToolButtonUI button = null;
        ToolSeperatorUI sp = null;
        Image image = null;
        String caption = "";
        //画背景
        if (backImage != null) {
            if (stretchImage) {
                image = ImageUtils.processImage(backImage, thisWidth, thisHeight, ImageUtils.MODE_STRETCH);
            } else {
                image = backImage;
            }
            drawImage(g, image, 0, 0, 0);
        }
        if (buttonAlignType == ConstsUI.ktHorizontal) {
            for (int i = 0; i < controlList.size(); i++) {
                if (controlList.elementAt(i) instanceof ToolButtonUI) {
                    button = (ToolButtonUI) controlList.elementAt(i);
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
                            if (glyphAlignType == ConstsUI.blGlyphTop) {
                                drawImage(g, image, buttonPos + (button.getWidth() - image.getWidth()) / 2 + 1,
                                        (button.getHeight() - image.getHeight() - fontHeight) / 2 + 3, 0);
                                drawString(g, caption, buttonPos + (button.getWidth() - ft.stringWidth(caption)) / 2 + 1,
                                        (button.getHeight() - image.getHeight() - fontHeight) / 2 + image.getHeight() + 3);
                            }
                            if (glyphAlignType == ConstsUI.blGlyphBottom) {
                                drawImage(g, image, buttonPos + (button.getWidth() - image.getWidth()) / 2 + 1,
                                        (button.getHeight() - image.getHeight() - fontHeight) / 2 + fontHeight + 3, 0);
                                drawString(g, caption, buttonPos + (button.getWidth() - ft.stringWidth(caption)) / 2 + 1,
                                        (button.getHeight() - image.getHeight() - fontHeight) / 2 + 3);
                            }
                            if (glyphAlignType == ConstsUI.blGlyphLeft) {
                                drawImage(g, image, buttonPos + (button.getWidth() - image.getWidth() - ft.stringWidth(caption) - 2) / 2 + 1,
                                        (button.getHeight() - image.getHeight()) / 2 + 3, 0);
                                drawString(g, caption, buttonPos + (button.getWidth() - ft.stringWidth(caption) - image.getWidth() - 2) / 2 + image.getWidth() + 3,
                                        (button.getHeight() - fontHeight) / 2 + 3);
                            }
                            if (glyphAlignType == ConstsUI.blGlyphRight) {
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
                    sp = (ToolSeperatorUI) controlList.elementAt(i);
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

        if (buttonAlignType == ConstsUI.ktVertical) {
            for (int i = 0; i <
                    controlList.size(); i++) {
                if (controlList.elementAt(i) instanceof ToolButtonUI) {
                    button = (ToolButtonUI) controlList.elementAt(i);
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
                            if (glyphAlignType == ConstsUI.blGlyphTop) {
                                drawImage(g, image, (button.getWidth() - image.getWidth()) / 2 + 3,
                                        buttonPos + (button.getHeight() - image.getHeight() - fontHeight) / 2 + 1, 0);
                                drawString(g, caption, (button.getWidth() - ft.stringWidth(caption)) / 2 + 3,
                                        buttonPos + (button.getHeight() - image.getHeight() - fontHeight) / 2 + image.getHeight() + 1);
                            }
                            if (glyphAlignType == ConstsUI.blGlyphBottom) {
                                drawImage(g, image, (button.getWidth() - image.getWidth()) / 2 + 3,
                                        buttonPos + (button.getHeight() - image.getHeight() - fontHeight) / 2 + fontHeight + 1, 0);
                                drawString(g, caption, (button.getWidth() - ft.stringWidth(caption)) / 2 + 3,
                                        buttonPos + (button.getHeight() - image.getHeight() - fontHeight) / 2 + 1);
                            }
                            if (glyphAlignType == ConstsUI.blGlyphLeft) {
                                drawImage(g, image, (button.getWidth() - image.getWidth() - ft.stringWidth(caption) - 2) / 2 + 3,
                                        buttonPos + (button.getHeight() - image.getHeight()) / 2 + 1, 0);
                                drawString(g, caption, (button.getWidth() - ft.stringWidth(caption) - image.getWidth() - 2) / 2 + image.getWidth() + 5,
                                        buttonPos + (button.getHeight() - fontHeight) / 2 + 1);
                            }
                            if (glyphAlignType == ConstsUI.blGlyphRight) {
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
                    sp = (ToolSeperatorUI) controlList.elementAt(i);
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
    public void addButton(ToolButtonUI button) {
        if (!controlList.contains(button)) {
            controlList.addElement(button);
        }

    }

    public ToolButtonUI addButton() {
        ToolButtonUI button = new ToolButtonUI();
        controlList.addElement(button);
        repaintControl();
        return button;
    }

    public ToolButtonUI addButton(
            String caption) {
        ToolButtonUI button = new ToolButtonUI();
        button.setCaption(caption);
        controlList.addElement(button);
        repaintControl();
        return button;
    }

    public ToolButtonUI addButton(Image image) {
        ToolButtonUI button = new ToolButtonUI();
        button.setImage(image);
        controlList.addElement(button);
        repaintControl();
        return button;
    }

    public ToolButtonUI addButton(String caption, Image image) {
        ToolButtonUI button = new ToolButtonUI();
        button.setCaption(caption);
        button.setImage(image);
        controlList.addElement(button);
        repaintControl();
        return button;
    }
//增加分割
    public void addSeparator(ToolSeperatorUI seperator) {
        if (!controlList.contains(seperator)) {
            controlList.addElement(seperator);
            repaintControl();
        }
    }

    public ToolSeperatorUI addSeparator() {
        ToolSeperatorUI sp = new ToolSeperatorUI();
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
                if (controlList.elementAt(i) instanceof ToolButtonUI) {
                    curButton = (ToolButtonUI) controlList.elementAt(i);
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
                if (controlList.elementAt(i) instanceof ToolButtonUI) {
                    curButton = (ToolButtonUI) controlList.elementAt(i);
                    doChange();
                    break;
                }
            }
        }
    }

    private void doKeydown(int keyCode) {
        String key = form.getKeyName(keyCode).toUpperCase();
        if ((key.equals(ConstsUI.KEY_LEFT) && (buttonAlignType == ConstsUI.ktHorizontal)) ||
                (key.equals(ConstsUI.KEY_UP) && (buttonAlignType == ConstsUI.ktVertical))) {
            //左
            prior();
            repaintControl();
        }

        if ((key.equals(ConstsUI.KEY_RIGHT) && (buttonAlignType == ConstsUI.ktHorizontal)) ||
                (key.equals(ConstsUI.KEY_DOWN) && (buttonAlignType == ConstsUI.ktVertical))) {
            //右
            next();
            repaintControl();
        }
        if (key.equals(ConstsUI.KEY_SELECT)) {
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
        ToolButtonUI button = null;
        if (buttonAlignType == ConstsUI.ktHorizontal) {
            //水平方向，判断x(相对坐标)
            for (int i = 0; i <
                    controlList.size(); i++) {
                if (controlList.elementAt(i) instanceof ToolButtonUI) {
                    button = (ToolButtonUI) controlList.elementAt(i);
                    //判断是否在中间
                    if (x > buttonPos && x < buttonPos + button.getWidth()) {
                        curButton = button;
                        findButton = true;
                        break;
                    }
                    buttonPos += button.getWidth() + 2;
                } else {
                    buttonPos += ((ToolSeperatorUI) controlList.elementAt(i)).getWidth();
                }
            }
        }
        if (buttonAlignType == ConstsUI.ktVertical) {
            //水平方向，判断x(相对坐标)
            for (int i = 0; i <
                    controlList.size(); i++) {
                if (controlList.elementAt(i) instanceof ToolButtonUI) {
                    button = (ToolButtonUI) controlList.elementAt(i);
                    //判断是否在中间
                    if (y > buttonPos && y < buttonPos + button.getHeight()) {
                        curButton = button;
                        findButton = true;
                        break;
                    }
                    buttonPos += button.getHeight() + 2;
                } else {
                    buttonPos += ((ToolSeperatorUI) controlList.elementAt(i)).getHeight();
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
            if (controlList.elementAt(i) instanceof ToolButtonUI) {
                result = false;
            }
        }
        return result;
    }

    private boolean isLast() {
        boolean result = true;
        int id = controlList.indexOf(curButton);
        for (int i = id + 1; i < controlList.size(); i++) {
            if (controlList.elementAt(i) instanceof ToolButtonUI) {
                result = false;
            }
        }
        return result;
    }

    protected boolean canDownTabNext() {
        if (buttonAlignType == ConstsUI.ktHorizontal) {
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
        if (buttonAlignType == ConstsUI.ktHorizontal) {
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
        if (buttonAlignType == ConstsUI.ktHorizontal) {
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
        if (buttonAlignType == ConstsUI.ktHorizontal) {
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
