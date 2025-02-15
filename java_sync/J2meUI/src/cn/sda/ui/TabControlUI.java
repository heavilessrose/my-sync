package cn.sda.ui;

import cn.sda.event.KeybordEvent;
import cn.sda.event.PointerEvent;
import cn.sda.event.TabSheetChangeEvent;
import java.util.Vector;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.Font;

/**
 *
 * @author Administrator
 */
public class TabControlUI extends BaseControl {

    private byte buttonAlignType = ConstsUI.ktHorizontal;
    private int borderColor = ConstsUI.clBlack;
    //是否显示标题
    private boolean showCaption = true;
    //是否按照标题自动伸缩按钮
    private boolean autoSizeButton = true;
    //组件列表
    private Vector controlList = null;
    //当前位置
    private TabSheetUI curSheet = null;
    //事件
    private TabSheetChangeEvent onChangeEvent = null;
    //背景图
    private Image backImage = null;
    private boolean stretchImage = true;
    //按钮图片排布方式
    private int glyphAlignType = ConstsUI.blGlyphLeft;
    //按钮位置
    private int buttonPosition = ConstsUI.tabButtonsTop;
    //按钮的缺省尺寸
    private int buttonSize = 40;
    //按钮区大小
    private int SelectbuttonSize = 0;
    //偏移
    private int offset = 0;
    //按钮总长度（判断是否显示滚动按钮）
    private int buttonsLen = 0;
    //开始显示的按钮
    private int StartButtonPos = 0;
    //最后完整显示按钮index
    private int endButtonPos = 0;
    //滚动按钮背景色
    private int scrollButtonColor = ConstsUI.clWhite;
    //滚动按钮大小
    private int scrollButtonSize = 14;
    //是否可以继续向后滚动
    private boolean canScroll = false;

    public TabControlUI() {
        super();
        setWidth(120);
        setHeight(100);
        controlList = new Vector();
        setOnKeyUp(new KeybordEvent() {

            public void Event(BaseControl ctrl, int keyCode) {
                //键盘
                doKeyUp(keyCode);
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
        //画按钮和分割线
        paintButton(g);
        //画滚动按钮
        paintScrollButton(g);
        //画子组件
        setSheetVisible();
        PaintChilds();
        //焦点
        if (isFoucsed()) {
            g.setColor(ConstsUI.clFocusShadow);
            if (buttonAlignType == ConstsUI.ktHorizontal) {
                if (buttonPosition == ConstsUI.tabButtonsTop) {
                    drawRect(g, 1, SelectbuttonSize + 1, thisWidth - 2, thisHeight - 2 - SelectbuttonSize);
                }
                if (buttonPosition == ConstsUI.tabButtonsBottom) {
                    drawRect(g, 1, 1, thisWidth - 2, offset - 2);
                }
            } else {
                if (buttonPosition == ConstsUI.tabButtonsLeft) {
                    drawRect(g, SelectbuttonSize + 1, 1, thisWidth - 2 - SelectbuttonSize, thisHeight - 2);
                }
                if (buttonPosition == ConstsUI.tabButtonsRight) {
                    drawRect(g, 1, 1, offset - 2, thisHeight - 2);
                }
            }
        }
        SetClip(g);
        g.setColor(borderColor);
        drawRect(g, 0, 0, thisWidth, thisHeight);
        PaintChilds();
    }
    //计算设置按钮的宽度
    private void setButtonSize() {
        TabSheetUI sheet = null;
        int buttonHeight = getHeight() - 4;
        int buttonWidth = getWidth() - 4;
        Font ft = getFont();
        int fontheight = ft.getHeight();
        int num = 0;
        int bsize = 0;
        buttonsLen = 0;
        for (int i = 0; i < controlList.size(); i++) {
            sheet = (TabSheetUI) controlList.elementAt(i);
            if (autoSizeButton) {
                if (buttonAlignType == ConstsUI.ktHorizontal) {
                    if (glyphAlignType == ConstsUI.blGlyphTop || glyphAlignType == ConstsUI.blGlyphBottom) {
                        num = fontheight + 2 + (sheet.getImage() != null ? sheet.getImage().getHeight() + 2 : 0);
                        if (bsize < num) {
                            bsize = num;
                        }
                    } else {
                        num = fontheight + 2;
                        if (sheet.getImage() != null) {
                            if (num < sheet.getImage().getHeight() + 2) {
                                num = sheet.getImage().getHeight() + 2;
                            }
                        }
                        if (bsize < num) {
                            bsize = num;
                        }
                    }
                } else {
                    if (glyphAlignType == ConstsUI.blGlyphLeft || glyphAlignType == ConstsUI.blGlyphRight) {
                        num = ft.stringWidth(sheet.getCaption() + "xx") + (sheet.getImage() != null ? sheet.getImage().getWidth() + 2 : 0);
                        if (bsize < num) {
                            bsize = num;
                        }
                    } else {
                        num = ft.stringWidth(sheet.getCaption() + "xx");
                        if (sheet.getImage() != null) {
                            if (num < sheet.getImage().getWidth() + 2) {
                                num = sheet.getImage().getWidth() + 2;
                            }
                        }
                        if (bsize < num) {
                            bsize = num;
                        }
                    }
                }
            }
        }
        SelectbuttonSize = bsize;
        for (int i = 0; i < controlList.size(); i++) {
            sheet = (TabSheetUI) controlList.elementAt(i);
            if (autoSizeButton) {
                if (buttonAlignType == ConstsUI.ktHorizontal) {
                    sheet.setButtonHeight(bsize);
                    if (sheet.getImage() != null && sheet.getCaption().length() > 0 &&
                            (glyphAlignType == ConstsUI.blGlyphLeft || glyphAlignType == ConstsUI.blGlyphRight)) {
                        sheet.setButtonWidth(ft.stringWidth(sheet.getCaption() + "xx") + sheet.getImage().getWidth());
                    } else {
                        num = ft.stringWidth(sheet.getCaption() + "xx");
                        if (sheet.getImage() != null) {
                            if (num < sheet.getImage().getWidth()) {
                                num = sheet.getImage().getWidth();
                            }
                        }
                        sheet.setButtonWidth(num);
                    }
                    buttonsLen += sheet.getButtonWidth();
                } else {
                    sheet.setButtonWidth(bsize);
                    if (sheet.getImage() != null && sheet.getCaption().length() > 0 &&
                            (glyphAlignType == ConstsUI.blGlyphTop || glyphAlignType == ConstsUI.blGlyphBottom)) {
                        sheet.setButtonHeight(fontheight + 4 + sheet.getImage().getHeight());
                    } else {
                        num = fontheight + 2;
                        if (sheet.getImage() != null) {
                            if (num < sheet.getImage().getHeight() + 2) {
                                num = sheet.getImage().getHeight() + 2;
                            }
                        }
                        sheet.setButtonHeight(num);
                    }
                    buttonsLen += sheet.getButtonHeight();
                }
            } else {
                if (buttonAlignType == ConstsUI.ktHorizontal) {
                    sheet.setButtonHeight(buttonHeight);
                    sheet.setButtonWidth(buttonSize);
                    buttonsLen += sheet.getButtonWidth();
                } else {
                    sheet.setButtonWidth(buttonWidth);
                    sheet.setButtonHeight(buttonSize);
                    buttonsLen += sheet.getButtonHeight();
                }
            }
        }
    }
    //画按钮
    private void paintButton(Graphics g) {
        int thisWidth = getWidth();
        int thisHeight = getHeight();
        Font ft = getFont();
        int fontHeight = ft.getHeight();
        //设置大小
        setButtonSize();
        SetClip(g);
        //画
        int buttonPos = 0;
        if (buttonPosition == ConstsUI.tabButtonsTop) {
            offset = 0;
            buttonAlignType = ConstsUI.ktHorizontal;
        }
        if (buttonPosition == ConstsUI.tabButtonsBottom) {
            offset = getHeight() - SelectbuttonSize;
            buttonAlignType = ConstsUI.ktHorizontal;
        }
        if (buttonPosition == ConstsUI.tabButtonsLeft) {
            offset = 0;
            buttonAlignType = ConstsUI.ktVertical;
        }
        if (buttonPosition == ConstsUI.tabButtonsRight) {
            offset = getWidth() - SelectbuttonSize;
            buttonAlignType = ConstsUI.ktVertical;
        }
        TabSheetUI button = null;
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
            //横线
            g.setColor(borderColor);
            int linePos = buttonPosition == ConstsUI.tabButtonsTop ? SelectbuttonSize : offset;
            drawLine(g, 0, linePos, getWidth(), linePos);
            for (int i = StartButtonPos; i < controlList.size(); i++) {
                button = (TabSheetUI) controlList.elementAt(i);
                caption = button.getCaption();
                g.setColor(borderColor);
                drawRect(g, buttonPos, offset, button.getButtonWidth(), button.getButtonHeight());
                //SetClip(g, buttonPos, offset, button.getButtonWidth(), button.getButtonHeight() + 1);
                image = button.getImage();
                //焦点按钮
                if (button.equals(curSheet)) {
                    g.setColor(button.backColor);
                    fillRect(g, buttonPos + 1, offset + (buttonPosition == ConstsUI.tabButtonsTop ? 1 : 0),
                            button.getButtonWidth() - 1, button.getButtonHeight() + (buttonPosition == ConstsUI.tabButtonsTop ? 1 : 0));
                }
                if (image != null) {
                    if (showCaption && caption.length() > 0) {
                        if (glyphAlignType == ConstsUI.blGlyphTop) {
                            drawImage(g, image, buttonPos + (button.getButtonWidth() - image.getWidth()) / 2 + 1,
                                    offset + (button.getButtonHeight() - image.getHeight() - fontHeight) / 2 + 1, 0);
                            g.setColor(foreColor);
                            drawString(g, caption, buttonPos + (button.getButtonWidth() - ft.stringWidth(caption)) / 2 + 1,
                                    offset + (button.getButtonHeight() - image.getHeight() - fontHeight) / 2 + image.getHeight() + 1);
                        }
                        if (glyphAlignType == ConstsUI.blGlyphBottom) {
                            drawImage(g, image, buttonPos + (button.getButtonWidth() - image.getWidth()) / 2 + 1,
                                    offset + (button.getButtonHeight() - image.getHeight() - fontHeight) / 2 + fontHeight + 1, 0);
                            g.setColor(foreColor);
                            drawString(g, caption, buttonPos + (button.getButtonWidth() - ft.stringWidth(caption)) / 2 + 1,
                                    offset + (button.getButtonHeight() - image.getHeight() - fontHeight) / 2 + 1);
                        }
                        if (glyphAlignType == ConstsUI.blGlyphLeft) {
                            drawImage(g, image, buttonPos + (button.getButtonWidth() - image.getWidth() - ft.stringWidth(caption) - 2) / 2 + 1,
                                    offset + (button.getButtonHeight() - image.getHeight()) / 2 + 1, 0);
                            g.setColor(foreColor);
                            drawString(g, caption, buttonPos + (button.getButtonWidth() - ft.stringWidth(caption) - image.getWidth() - 2) / 2 + image.getWidth() + 3,
                                    offset + (button.getButtonHeight() - fontHeight) / 2 + 1);
                        }
                        if (glyphAlignType == ConstsUI.blGlyphRight) {
                            drawImage(g, image, buttonPos + (button.getButtonWidth() - image.getWidth() - ft.stringWidth(caption) - 2) / 2 + ft.stringWidth(caption) + 3,
                                    offset + (button.getButtonHeight() - image.getHeight()) / 2 + 1, 0);
                            g.setColor(foreColor);
                            drawString(g, caption, buttonPos + (button.getButtonWidth() - ft.stringWidth(caption) - image.getWidth() - 2) / 2 + 1,
                                    offset + (button.getButtonHeight() - fontHeight) / 2 + 1);
                        }

                    } else {
                        drawImage(g, image, buttonPos + (button.getButtonWidth() - image.getWidth()) / 2 + 1,
                                offset + (button.getButtonHeight() - image.getHeight()) / 2 + 1, 0);
                    }
                } else {
                    if (showCaption) {
                        g.setColor(foreColor);
                        drawString(g, button.getCaption(), buttonPos + (button.getButtonWidth() - ft.stringWidth(button.getCaption())) / 2 + 1,
                                offset + (button.getButtonHeight() - fontHeight) / 2 + 1);
                    }
                }
                SetClip(g);
                buttonPos += button.getButtonWidth();
                if (buttonPos + 2 * scrollButtonSize < width) {
                    endButtonPos = i;
                }
            }
            if (buttonPos + 2 * scrollButtonSize > width) {
                canScroll = true;
            } else {
                canScroll = false;
            }
        }
        if (buttonAlignType == ConstsUI.ktVertical) {
            //线
            g.setColor(borderColor);
            int linePos = buttonPosition == ConstsUI.tabButtonsLeft ? SelectbuttonSize : offset;
            drawLine(g, linePos, 0, linePos, getHeight());
            for (int i = StartButtonPos; i < controlList.size(); i++) {
                button = (TabSheetUI) controlList.elementAt(i);
                caption = button.getCaption();
                g.setColor(borderColor);
                drawRect(g, offset, buttonPos, button.getButtonWidth(), button.getButtonHeight());
                //SetClip(g, offset, buttonPos, button.getButtonWidth() + 1, button.getButtonHeight());
                image = button.getImage();
                if (button.equals(curSheet)) {
                    g.setColor(button.backColor);
                    fillRect(g, offset + (buttonPosition == ConstsUI.tabButtonsLeft ? 1 : 0), buttonPos + 1,
                            button.getButtonWidth() + (buttonPosition == ConstsUI.tabButtonsLeft ? 1 : 0), button.getButtonHeight() - 1);
                }
                if (image != null) {
                    if (showCaption && caption.length() > 0) {
                        if (glyphAlignType == ConstsUI.blGlyphTop) {
                            drawImage(g, image, offset + (button.getButtonWidth() - image.getWidth()) / 2 + 1,
                                    buttonPos + (button.getButtonHeight() - image.getHeight() - fontHeight) / 2 + 1, 0);
                            g.setColor(foreColor);
                            drawString(g, caption, offset + (button.getButtonWidth() - ft.stringWidth(caption)) / 2 + 1,
                                    buttonPos + (button.getButtonHeight() - image.getHeight() - fontHeight) / 2 + image.getHeight() + 1);
                        }
                        if (glyphAlignType == ConstsUI.blGlyphBottom) {
                            drawImage(g, image, offset + (button.getButtonWidth() - image.getWidth()) / 2 + 1,
                                    buttonPos + (button.getButtonHeight() - image.getHeight() - fontHeight) / 2 + fontHeight + 1, 0);
                            g.setColor(foreColor);
                            drawString(g, caption, offset + (button.getButtonWidth() - ft.stringWidth(caption)) / 2 + 1,
                                    buttonPos + (button.getButtonHeight() - image.getHeight() - fontHeight) / 2 + 1);
                        }
                        if (glyphAlignType == ConstsUI.blGlyphLeft) {
                            drawImage(g, image, offset + (button.getButtonWidth() - image.getWidth() - ft.stringWidth(caption) - 2) / 2 + 1,
                                    buttonPos + (button.getButtonHeight() - image.getHeight()) / 2 + 1, 0);
                            g.setColor(foreColor);
                            drawString(g, caption, offset + (button.getButtonWidth() - ft.stringWidth(caption) - image.getWidth() - 2) / 2 + image.getWidth() + 3,
                                    buttonPos + (button.getButtonHeight() - fontHeight) / 2 + 1);
                        }
                        if (glyphAlignType == ConstsUI.blGlyphRight) {
                            drawImage(g, image, offset + (button.getButtonWidth() - image.getWidth() - ft.stringWidth(caption) - 2) / 2 + ft.stringWidth(caption) + 3,
                                    buttonPos + (button.getButtonHeight() - image.getHeight()) / 2 + 1, 0);
                            g.setColor(foreColor);
                            drawString(g, caption, offset + (button.getButtonWidth() - ft.stringWidth(caption) - image.getWidth() - 2) / 2 + 1,
                                    buttonPos + (button.getButtonHeight() - fontHeight) / 2 + 1);
                        }

                    } else {
                        drawImage(g, image, offset + (button.getButtonWidth() - image.getWidth()) / 2 + 1,
                                buttonPos + (button.getButtonHeight() - image.getHeight()) / 2 + 1, 0);
                    }
                } else {
                    if (showCaption) {
                        g.setColor(foreColor);
                        //打字
                        drawString(g, String.valueOf(caption), offset + (button.getButtonWidth() - ft.stringWidth(caption)) / 2 + 1,
                                buttonPos + 2);
                    }
                }
                SetClip(g);
                buttonPos += button.getButtonHeight();
                if (buttonPos + 2 * scrollButtonSize < height) {
                    endButtonPos = i;
                }
            }
            if (buttonPos + 2 * scrollButtonSize > height) {
                canScroll = true;
            } else {
                canScroll = false;
            }
        }
    }

    //画滚动按钮
    private void paintScrollButton(Graphics g) {
        if (buttonAlignType == ConstsUI.ktHorizontal) {
            //水平的按钮放右边
            if (buttonsLen > width) {
                g.setColor(scrollButtonColor);
                fillRect(g, width - scrollButtonSize * 2, offset, 2 * scrollButtonSize, scrollButtonSize);
                g.setColor(borderColor);
                drawRect(g, width - scrollButtonSize * 2, offset, 2 * scrollButtonSize, scrollButtonSize);
                drawRect(g, width - scrollButtonSize * 2, offset, scrollButtonSize, scrollButtonSize);
                //箭头
                fillTriangle(g, width - scrollButtonSize * 2 + scrollButtonSize / 2 - 2, offset + scrollButtonSize / 2,
                        width - scrollButtonSize * 2 + scrollButtonSize / 2 + 2, offset + scrollButtonSize / 2 - 4,
                        width - scrollButtonSize * 2 + scrollButtonSize / 2 + 2, offset + scrollButtonSize / 2 + 4);
                fillTriangle(g, width - scrollButtonSize + scrollButtonSize / 2 - 2, offset + scrollButtonSize / 2 - 4,
                        width - scrollButtonSize + scrollButtonSize / 2 - 2, offset + scrollButtonSize / 2 + 4,
                        width - scrollButtonSize + scrollButtonSize / 2 + 2, offset + scrollButtonSize / 2);
            }
        }
        if (buttonAlignType == ConstsUI.ktVertical) {
            //垂直的按钮放下边
            if (buttonsLen > height) {
                g.setColor(scrollButtonColor);
                fillRect(g, offset + SelectbuttonSize - scrollButtonSize, height - scrollButtonSize * 2, scrollButtonSize, 2 * scrollButtonSize);
                g.setColor(borderColor);
                drawRect(g, offset + SelectbuttonSize - scrollButtonSize, height - scrollButtonSize * 2, scrollButtonSize, 2 * scrollButtonSize);
                drawRect(g, offset + SelectbuttonSize - scrollButtonSize, height - scrollButtonSize * 2, scrollButtonSize, scrollButtonSize);
                //箭头
                fillTriangle(g, offset + SelectbuttonSize - scrollButtonSize + scrollButtonSize / 2, height - scrollButtonSize * 2 + scrollButtonSize / 2 - 2,
                        offset + SelectbuttonSize - scrollButtonSize + scrollButtonSize / 2 - 4, height - scrollButtonSize * 2 + scrollButtonSize / 2 + 2,
                        offset + SelectbuttonSize - scrollButtonSize + scrollButtonSize / 2 + 4, height - scrollButtonSize * 2 + scrollButtonSize / 2 + 2);
                fillTriangle(g, offset + SelectbuttonSize - scrollButtonSize + scrollButtonSize / 2 - 4, height - scrollButtonSize + scrollButtonSize / 2 - 2,
                        offset + SelectbuttonSize - scrollButtonSize + scrollButtonSize / 2 + 4, height - scrollButtonSize + scrollButtonSize / 2 - 2,
                        offset + SelectbuttonSize - scrollButtonSize + scrollButtonSize / 2, height - scrollButtonSize + scrollButtonSize / 2 + 2);
            }
        }
    }
    //增加button
    public void addTabSheet(TabSheetUI sheet) {
        if (!controlList.contains(sheet)) {
            controlList.addElement(sheet);
            AddControl(sheet);
            if (curSheet == null) {
                curSheet = sheet;
            }
            repaintControl();
        }
    }

    public TabSheetUI addTabSheet() {
        TabSheetUI sheet = new TabSheetUI();
        controlList.addElement(sheet);
        AddControl(sheet);
        if (curSheet == null) {
            curSheet = sheet;
        }
        repaintControl();
        return sheet;
    }

    public TabSheetUI addTabSheet(String caption) {
        TabSheetUI sheet = new TabSheetUI();
        sheet.setCaption(caption);
        controlList.addElement(sheet);
        AddControl(sheet);
        if (curSheet == null) {
            curSheet = sheet;
        }
        repaintControl();
        return sheet;
    }

    public TabSheetUI addTabSheet(Image image) {
        TabSheetUI button = new TabSheetUI();
        button.setImage(image);
        controlList.addElement(button);
        AddControl(button);
        if (curSheet == null) {
            curSheet = button;
        }
        repaintControl();
        return button;
    }

    public TabSheetUI addTabSheet(String caption, Image image) {
        TabSheetUI button = new TabSheetUI();
        button.setCaption(caption);
        button.setImage(image);
        controlList.addElement(button);
        AddControl(button);
        if (curSheet == null) {
            curSheet = button;
        }
        repaintControl();
        return button;
    }

    private void setSheetVisible() {
        TabSheetUI sheet = null;
        for (int i = 0; i < controlList.size(); i++) {
            sheet = (TabSheetUI) controlList.elementAt(i);
            sheet.setIndex(i);
            if (sheet.equals(curSheet)) {
                sheet.visible = true;
                if (buttonPosition == ConstsUI.tabButtonsLeft) {
                    sheet.setLeft(SelectbuttonSize + 1);
                    sheet.setTop(1);
                    sheet.setWidth(width - SelectbuttonSize - 1);
                    sheet.setHeight(height - 1);
                }
                if (buttonPosition == ConstsUI.tabButtonsRight) {
                    sheet.setLeft(1);
                    sheet.setTop(1);
                    sheet.setWidth(width - SelectbuttonSize - 1);
                    sheet.setHeight(height - 1);
                }
                if (buttonPosition == ConstsUI.tabButtonsTop) {
                    sheet.setLeft(1);
                    sheet.setTop(SelectbuttonSize + 1);
                    sheet.setWidth(width - 1);
                    sheet.setHeight(height - SelectbuttonSize - 1);
                }
                if (buttonPosition == ConstsUI.tabButtonsBottom) {
                    sheet.setLeft(1);
                    sheet.setTop(1);
                    sheet.setWidth(width - 1);
                    sheet.setHeight(height - SelectbuttonSize - 1);
                }
            } else {
                sheet.visible = false;
            }
        }
    }

    public int getBorderColor() {
        return borderColor;
    }

    public void setBorderColor(int borderColor) {
        this.borderColor = borderColor;
        repaintControl();
    }

    public boolean isAutoSizeButton() {
        return autoSizeButton;
    }

    public void setAutoSizeButton(boolean autoSizeButton) {
        this.autoSizeButton = autoSizeButton;
        repaintControl();
    }

    public boolean isShowCaption() {
        return showCaption;
    }

    public void setShowCaption(boolean showCaption) {
        this.showCaption = showCaption;
        repaintControl();
    }

    public Image getBackImage() {
        return backImage;
    }

    public void setBackImage(Image backImage) {
        this.backImage = backImage;
        repaintControl();
    }

    public boolean isStretchImage() {
        return stretchImage;
    }

    public void setStretchImage(boolean stretchImage) {
        this.stretchImage = stretchImage;
        repaintControl();
    }

    public int getGlyphAlignType() {
        return glyphAlignType;
    }

    public void setGlyphAlignType(int glyphAlignType) {
        this.glyphAlignType = glyphAlignType;
        repaintControl();
    }

    public int getButtonSize() {
        return buttonSize;
    }

    public void setButtonSize(int buttonSize) {
        this.buttonSize = buttonSize;
        repaintControl();
    }

    public int getButtonPosition() {
        return buttonPosition;
    }

    public int getScrollButtonColor() {
        return scrollButtonColor;
    }

    public void setScrollButtonColor(int scrollButtonColor) {
        this.scrollButtonColor = scrollButtonColor;
        repaintControl();
    }

    public int getScrollButtonSize() {
        return scrollButtonSize;
    }

    public void setScrollButtonSize(int scrollButtonSize) {
        this.scrollButtonSize = scrollButtonSize;
    }

    public void setButtonPosition(int buttonPosition) {
        this.buttonPosition = buttonPosition;
        if (buttonPosition == ConstsUI.tabButtonsTop || buttonPosition == ConstsUI.tabButtonsBottom) {
            buttonAlignType = ConstsUI.ktHorizontal;
        }
        if (buttonPosition == ConstsUI.tabButtonsLeft || buttonPosition == ConstsUI.tabButtonsRight) {
            buttonAlignType = ConstsUI.ktVertical;
        }
        repaintControl();
    }

//组件处理
    public void clear() {
        controlList.removeAllElements();
        repaintControl();
    }

    private void prior() {
        //向前滚动
        int index = controlList.indexOf(curSheet);
        if (index > 0) {
            curSheet = (TabSheetUI) controlList.elementAt(index - 1);
            doChange();
            if (StartButtonPos > curSheet.getIndex()) {
                StartButtonPos--;
            }
        }
    }

    private void next() {
        //向后滚动
        int index = controlList.indexOf(curSheet);
        if (index < controlList.size() - 1) {
            curSheet = (TabSheetUI) controlList.elementAt(index + 1);
            doChange();
            if (endButtonPos < curSheet.getIndex()) {
                StartButtonPos++;
            }
        }
    }

    //设置活动页面
    public void setActivePage(int index) {
        if (index < controlList.size() && index > -1) {
            curSheet = (TabSheetUI) controlList.elementAt(index);
            doChange();
            repaintControl();
        }
    }

    public void setActivePage(TabSheetUI sheet) {
        if (sheet != null && controlList.contains(sheet)) {
            curSheet = sheet;
            doChange();
            repaintControl();
        }
    }

    public TabSheetUI getActivePage() {
        return curSheet;
    }

    public int getPageCount() {
        return controlList.size();
    }

    private void doKeyUp(int keyCode) {
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
    }

    private void doPointerPress(int x, int y) {
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        if (buttonAlignType == ConstsUI.ktHorizontal) {
            if (InClientRect(posx, posy, width - scrollButtonSize * 2, offset, scrollButtonSize, scrollButtonSize)) {
                //左边按钮
                if (StartButtonPos > 0) {
                    StartButtonPos--;
                }
                return;
            }
            if (InClientRect(posx, posy, width - scrollButtonSize, offset, scrollButtonSize, scrollButtonSize)) {
                //右边按钮
                if (canScroll) {
                    StartButtonPos++;
                }
                return;
            }
        }
        if (buttonAlignType == ConstsUI.ktVertical) {
            if (InClientRect(posx, posy, offset + SelectbuttonSize - scrollButtonSize, height - scrollButtonSize * 2, scrollButtonSize, scrollButtonSize)) {
                //左边按钮
                if (StartButtonPos > 0) {
                    StartButtonPos--;
                }
                return;
            }
            if (InClientRect(posx, posy, offset + SelectbuttonSize - scrollButtonSize, height - scrollButtonSize, scrollButtonSize, scrollButtonSize)) {
                //右边按钮
                if (canScroll) {
                    StartButtonPos++;
                }
                return;
            }
        }
        if (buttonAlignType == ConstsUI.ktHorizontal) {
            if (InClientRect(posx, posy, 0, offset, width, SelectbuttonSize)) {
                getButtonFromPoint(posx, posy);
            }
        } else {
            if (InClientRect(posx, posy, offset,0,SelectbuttonSize, height)) {
                getButtonFromPoint(posx, posy);
            }
        }
    }

    private void getButtonFromPoint(int x, int y) {
        int buttonPos = 0;
        TabSheetUI button = null;
        if (buttonAlignType == ConstsUI.ktHorizontal) {
            //水平方向，判断x(相对坐标)
            for (int i = StartButtonPos; i < controlList.size(); i++) {
                button = (TabSheetUI) controlList.elementAt(i);
                //判断是否在中间
                if (x > buttonPos && x < buttonPos + button.getButtonWidth()) {
                    curSheet = button;
                    break;
                }
                buttonPos += button.getButtonWidth();
            }
        }
        //System.out.println(y+"/"+SDAConsts.ktVertical);
        if (buttonAlignType == ConstsUI.ktVertical) {
            //水平方向，判断x(相对坐标)
            for (int i = StartButtonPos; i < controlList.size(); i++) {
                button = (TabSheetUI) controlList.elementAt(i);
                //判断是否在中间
                if (y > buttonPos && y < buttonPos + button.getButtonHeight()) {
                    curSheet = button;
                    break;
                }
                buttonPos += button.getButtonHeight();
            }
        }
    }

    public void setOnChangeEvent(TabSheetChangeEvent onChangeEvent) {
        this.onChangeEvent = onChangeEvent;
    }

    private void doChange() {
        if (onChangeEvent != null) {
            onChangeEvent.Event(curSheet);
        }
    }

    private boolean isFirst() {
        if (controlList.indexOf(curSheet) == 0) {
            return true;
        } else {
            return false;
        }
    }

    private boolean isLast() {
        if (controlList.indexOf(curSheet) == controlList.size() - 1) {
            return true;
        } else {
            return false;
        }
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
}
