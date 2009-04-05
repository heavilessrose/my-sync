package cn.sda.ui;

import cn.sda.event.KeybordEvent;
import cn.sda.event.PointerEvent;
import javax.microedition.lcdui.Graphics;
import java.util.Vector;
import javax.microedition.lcdui.Image;

/**
 * @author not attributable
 * @version 1.0
 */
public class ListBoxUI extends BaseControl {
    //边框
    private int borderStyle = ConstsUI.bsFixedSingle;
    //隐藏选定内容
    private boolean hideSelection = true;
    //滚动边框(多行)
    private int scrollBars = ConstsUI.srVertical;
    private int barwidth = 10;
    //边框线颜色
    private int borderColor = ConstsUI.clBlack;
    //滚动条颜色
    private int scrollBarColor = ConstsUI.clBtnFace;
    //选中后字体颜色
    private int selectedForeColor = ConstsUI.clWhite;
    private int selectedBackColor = ConstsUI.clFocusShadow;
    //内部变量
    //行数，行最大长度
    private int maxLineLenght = 0;
    private int maxLineNum = 0;
    //最大可见完整行数
    private int maxVisibleLineNum = 0;
    //垂直开始显示行
    private int startLine = 0;
    //水平开始显示位置
    private int startLeft = 0;
    //变动前位置
    private int oldStartLine = 0;
    private int oldStartLeft = 0;
    //水平滚条位置
    private int HSLeft = 0;
    private int HSTop = 0;
    private int HSWidth = 0;
    private int HSHeight = 0;
    //垂直滚动条位置
    private int VSLeft = 0;
    private int VSTop = 0;
    private int VSWidth = 0;
    private int VSHeight = 0;
    //点击的滚动条的位置
    private int oldScrollPointx = 0;
    private int oldScrollPointy = 0;
    //按下滚动条
    private boolean isscrollbarpointdown = false;
    //记录按下的滚动条0:水平，1：垂直的
    private byte scrollbardownHV = 0;
    //行的列表
    private Vector itemList;
    //当前选择行
    private int itemIndex = -1;
    //显示选择框
    private boolean showCheckBox = false;
    //显示图
    private boolean showItemImage = false;

    public ListBoxUI() {
        super();
        internalSDAListBox();
    }

    private void internalSDAListBox() {
        itemList = new Vector();
        this.setVisible(true);
        setLeft(0);
        setTop(0);
        setBorderStyle(ConstsUI.bsFixedSingle);
        super.setWidth(160);
        setHeight(100);
        super.setBackColor(ConstsUI.clWhite);
        this.onPointerPressed = new PointerEvent() {

            public void Event(BaseControl ctrl, int x, int y) {
                doPointerPressed(x, y);
            }
        };
        this.onPointerReleased = new PointerEvent() {

            public void Event(BaseControl ctrl, int x, int y) {
                doPointerReleased(x, y);
            }
        };
        this.onKeyUp = new KeybordEvent() {

            public void Event(BaseControl ctrl, int keyCode) {
                doKeyUp(keyCode);
            }
        };
    }
    //增加项
    public ListItemUI addItem(String itemString, Image image, boolean isChecked) {
        return internalAddItem(itemString, image, isChecked);
    }

    private ListItemUI internalAddItem(String itemString, Image image, boolean isChecked) {
        ListItemUI listitem = new ListItemUI();
        listitem.setImage(image);
        listitem.setChecked(isChecked);
        listitem.setItemString(itemString);
        itemList.addElement(listitem);
        if (itemIndex == -1) {
            itemIndex = 0;
        }
        internalPaint();
        return listitem;
    }
    //删除项
    public void deleteItem(int itemIndex) {
        internalDeleteItem(itemIndex);
    }

    private void internalDeleteItem(int itemIndex) {
        if (itemList.size() > itemIndex) {
            itemList.removeElementAt(itemIndex);
            if (itemList.size() == 0) {
                itemIndex = -1;
            }
            internalPaint();
            System.gc();
        }

    }
    //清除所有项目
    public void clear() {
        internalClear();
    }

    private void internalClear() {
        itemList.removeAllElements();
        itemIndex = -1;
        System.gc();
        internalPaint();
    }
    //获取项
    public Image getItemImage(int itemIndex) {
        return internalGetItemImage(itemIndex);
    }

    private Image internalGetItemImage(int itemIndex) {
        if (itemList.size() > itemIndex) {
            ListItemUI listitem = (ListItemUI) itemList.elementAt(itemIndex);
            return listitem.getImage();
        } else {
            return null;
        }
    }

    public String getItemString(int itemIndex) {
        return internalGetItemString(itemIndex);
    }

    private String internalGetItemString(int itemIndex) {
        if (itemList.size() > itemIndex) {
            ListItemUI listitem = (ListItemUI) itemList.elementAt(itemIndex);
            return listitem.getItemString();
        } else {
            return null;
        }
    }
    //查找判断是否有指定内容的项
    public boolean findItem(String itemValue) {
        return getItem(itemValue) == null ? false : true;
    }

    public ListItemUI getItem(String itemValue) {
        return internalGetItem(itemValue);
    }

    public ListItemUI getItem() {
        return (ListItemUI) itemList.elementAt(itemIndex);
    }

    public ListItemUI getItem(int Index) {
        return (ListItemUI) itemList.elementAt(Index);
    }

    private ListItemUI internalGetItem(String itemValue) {
        ListItemUI item = null;
        ListItemUI result = null;
        for (int i = 0; i < itemList.size(); i++) {
            item = (ListItemUI) itemList.elementAt(i);
            if (item.getItemString().equals(itemValue)) {
                result = item;
                break;
            }
        }
        return result;
    }

    public boolean getItemChecked(int itemIndex) {
        return internalGetItemChecked(itemIndex);
    }

    private boolean internalGetItemChecked(int itemIndex) {
        if (itemList.size() > itemIndex) {
            ListItemUI listitem = (ListItemUI) itemList.elementAt(itemIndex);
            return listitem.isChecked();
        } else {
            return false;
        }
    }

    public int getItemCount() {
        return itemList.size();
    }

    public boolean isShowCheckBox() {
        return showCheckBox;
    }

    public void setShowCheckBox(boolean showCheckBox) {
        this.showCheckBox = showCheckBox;
        internalPaint();
    }

    public boolean isShowItemImage() {
        return showItemImage;
    }

    public void setShowItemImage(boolean showItemImage) {
        this.showItemImage = showItemImage;
    }

    public void setItemChecked(int itemIndex, boolean Checked) {
        internalSetItemChecked(itemIndex, Checked);
    }

    private void internalSetItemChecked(int itemIndex, boolean Checked) {
        if (itemList.size() > itemIndex) {
            ListItemUI listitem = (ListItemUI) itemList.elementAt(itemIndex);
            listitem.setChecked(Checked);
            internalPaint();
        }
    }

    public void setItemImage(int itemIndex, Image image) {
        internalSetItemImage(itemIndex, image);
    }

    private void internalSetItemImage(int itemIndex, Image image) {
        if (itemList.size() > itemIndex) {
            ListItemUI listitem = (ListItemUI) itemList.elementAt(itemIndex);
            listitem.setImage(image);
            internalPaint();
        }
    }

    public void setItemString(int itemIndex, String itemString) {
        internalSetItemString(itemIndex, itemString);
    }

    private void internalSetItemString(int itemIndex, String itemString) {
        if (itemList.size() > itemIndex) {
            ListItemUI listitem = (ListItemUI) itemList.elementAt(itemIndex);
            listitem.setItemString(itemString);
            internalPaint();
        }
    }

    //画
    public void paint() {
        internalPaint();
    }

    private void internalPaint() {
        if (!IsCanPaint()) {
            return;
        }
        Graphics g = form.getGraphics();
        InternalPaint(g);
        PaintChilds();
    }

    protected void InternalPaint(Graphics g) {
        //可视下才画
        if (isVisible()) {
            //初始化引用变量
            int thisWidth = getWidth();
            int thisHeight = getHeight();
            int HBarWidth = getHBarWidth();
            int VBarHeight = getVBarHeight();
            //对齐方式处理后，画组件
            //设置字体
            g.setFont(getFont());
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
                if (isFoucsed()) {
                    g.setColor(ConstsUI.clFocusShadow);
                    drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
                }
            }
            if (borderStyle == ConstsUI.bsFixedSingle) {
                //边框
                g.setColor(this.getBorderColor());
                drawRect(g, 0, 0, thisWidth, thisHeight);
                if (isFoucsed()) {
                    g.setColor(ConstsUI.clFocusShadow);
                    drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
                }
            }
            //打字
            g.setColor(this.getForeColor());
            paintString(g);
            //滚动条
            if (borderStyle == ConstsUI.bsFixed3D) {
                SetClip(g, 2, 2, thisWidth - 2, thisHeight - 2);
            } else {
                SetClip(g, 1, 1, thisWidth - 1, thisHeight - 1);
            }
            if ((scrollBars == ConstsUI.srHorizontal) || (scrollBars == ConstsUI.srBoth)) {
                //水平
                g.setColor(scrollBarColor);
                fillRect(g, 0, thisHeight - barwidth, barwidth, barwidth);
                fillRect(g, HBarWidth - barwidth, thisHeight - barwidth, barwidth, barwidth);
                g.setColor(getBorderColor());
                drawRect(g, 0, thisHeight - barwidth, barwidth, barwidth);
                drawRect(g, HBarWidth - barwidth, thisHeight - barwidth, barwidth, barwidth);
                //滚动块
                g.setColor(backColor);
                fillRect(g, barwidth, thisHeight - barwidth, HBarWidth - 2 * barwidth, barwidth);
                g.setColor(this.getBorderColor());
                drawRect(g, barwidth, thisHeight - barwidth, HBarWidth - 2 * barwidth, barwidth);
                //块大小
                int pwidth = HBarWidth - 2 * barwidth;

                if (maxLineLenght > HBarWidth) {
                    pwidth = ((HBarWidth) * (HBarWidth - 2 * barwidth)) / maxLineLenght;
                }

                //计算块位置
                int ppos = (startLeft * (HBarWidth - 2 * barwidth)) / maxLineLenght;
                //修正位置
                if (ppos + pwidth > HBarWidth - 2 * barwidth) {
                    ppos = HBarWidth - 2 * barwidth - pwidth;
                }
                //画块
                g.setColor(scrollBarColor);
                fillRect(g, barwidth + ppos, thisHeight - barwidth, pwidth, barwidth);
                g.setColor(getBorderColor());
                drawRect(g, barwidth + ppos, thisHeight - barwidth, pwidth, barwidth);
                //记录大小
                HSLeft = barwidth + ppos;
                HSTop = thisHeight - barwidth;
                HSWidth = pwidth;
                HSHeight = barwidth;
                //画块上的线
                int tpos = HSWidth / 2 + HSLeft;
                drawRect(g, tpos, HSTop + 2, 0, HSHeight - 4);
                if (tpos - 2 > HSLeft) {
                    drawRect(g, tpos - 2, HSTop + 2, 0, HSHeight - 4);
                }
                if (tpos + 2 < HSLeft + HSWidth) {
                    drawRect(g, tpos + 2, HSTop + 2, 0, HSHeight - 4);
                }
                //三角
                g.setColor(getBorderColor());
                fillTriangle(g, barwidth / 2 - 2, thisHeight - barwidth / 2,
                        barwidth / 2 + 2, thisHeight - barwidth / 2 - 4,
                        barwidth / 2 + 2, thisHeight - barwidth / 2 + 4);
                fillTriangle(g, HBarWidth - barwidth + barwidth / 2 + 2, thisHeight - barwidth / 2,
                        HBarWidth - barwidth + barwidth / 2 - 2, thisHeight - barwidth / 2 - 4,
                        HBarWidth - barwidth + barwidth / 2 - 2, thisHeight - barwidth / 2 + 4);
            }
            if ((scrollBars == ConstsUI.srVertical) || (scrollBars == ConstsUI.srBoth)) {
                //垂直
                g.setColor(scrollBarColor);
                fillRect(g, thisWidth - barwidth, 0, barwidth, barwidth);
                fillRect(g, thisWidth - barwidth, VBarHeight - barwidth, barwidth, barwidth);
                g.setColor(getBorderColor());
                drawRect(g, thisWidth - barwidth, 0, barwidth, barwidth);
                drawRect(g, thisWidth - barwidth, VBarHeight - barwidth, barwidth, barwidth);
                //滚动块
                g.setColor(backColor);
                fillRect(g, thisWidth - barwidth, barwidth, barwidth, VBarHeight - 2 * barwidth);
                g.setColor(getBorderColor());
                drawRect(g, thisWidth - barwidth, barwidth, barwidth, VBarHeight - 2 * barwidth);
                //块大小                    
                int pheight = VBarHeight - 2 * barwidth;
                if (itemList.size() * getFont().getHeight() > VBarHeight) {
                    pheight = ((VBarHeight) * (VBarHeight - 2 * barwidth) / (itemList.size() * getFont().getHeight()));
                }
                //计算块位置
                int ppos = barwidth;
                if (itemList.size() > 0) {
                    ppos = (startLine * getFont().getHeight() * (VBarHeight - 2 * barwidth)) / (itemList.size() * getFont().getHeight());
                }
                //修正位置
                if (ppos + pheight > VBarHeight - 2 * barwidth) {
                    ppos = VBarHeight - 2 * barwidth - pheight;
                }
                //画块
                g.setColor(scrollBarColor);
                fillRect(g, thisWidth - barwidth, barwidth + ppos, barwidth, pheight);
                g.setColor(getBorderColor());
                drawRect(g, thisWidth - barwidth, barwidth + ppos, barwidth, pheight);
                //记录大小
                VSLeft = thisWidth - barwidth;
                VSTop = barwidth + ppos;
                VSWidth = barwidth;
                VSHeight = pheight;
                //画块上的线
                int tpos = VSHeight / 2 + VSTop;
                drawRect(g, VSLeft + 2, tpos, barwidth - 4, 0);
                if (tpos - 2 > VSTop) {
                    drawRect(g, VSLeft + 2, tpos - 2, VSWidth - 4, 0);
                }
                if (tpos + 2 < VSTop + VSHeight) {
                    drawRect(g, VSLeft + 2, tpos + 2, VSWidth - 4, 0);
                }
                //三角
                g.setColor(getBorderColor());
                fillTriangle(g, thisWidth - barwidth + barwidth / 2, barwidth / 2 - 2,
                        thisWidth - barwidth + barwidth / 2 - 4, barwidth / 2 + 2,
                        thisWidth - barwidth + barwidth / 2 + 4, barwidth / 2 + 2);
                fillTriangle(g, thisWidth - barwidth + barwidth / 2 - 4, VBarHeight - barwidth + barwidth / 2 - 2,
                        thisWidth - barwidth + barwidth / 2 + 4, VBarHeight - barwidth + barwidth / 2 - 2,
                        thisWidth - barwidth + barwidth / 2, VBarHeight - barwidth / 2 + 2);
            }
        }
    }
    //打字
    private void paintString(Graphics g) {
        int thisWidth = getWidth();
        int thisHeight = getHeight();
        int HBarWidth = getHBarWidth();
        maxVisibleLineNum = getmaxVisibleLineNum();
        maxLineNum = getMaxLineNum();
        maxLineLenght = getMaxLineLenght();
        g.setColor(getForeColor());
        if (scrollBars == ConstsUI.srNone) {
            SetClip(g, 2, 2, thisWidth - 3, thisHeight - 3);
        }
        if (scrollBars == ConstsUI.srHorizontal) {
            SetClip(g, 2, 2, thisWidth - 3, thisHeight - barwidth - 3);
        }
        if (scrollBars == ConstsUI.srVertical) {
            SetClip(g, 2, 2, thisWidth - barwidth - 3, thisHeight - 3);
        }
        if (scrollBars == ConstsUI.srBoth) {
            SetClip(g, 2, 2, thisWidth - barwidth - 3, thisHeight - barwidth - 3);
        }
        //打字,根据item打印
        ListItemUI item;
        int fontHeight = getFont().getHeight();
        int CheckWidth = showCheckBox ? fontHeight : 0;
        int ImageWidth = showItemImage ? fontHeight : 0;
        String str = "";
        int endline = maxLineNum - startLine - maxVisibleLineNum > 0 ? startLine + maxVisibleLineNum : maxLineNum;
        for (int i = startLine; i < endline; i++) {
            item = (ListItemUI) itemList.elementAt(i);
            str = item.getItemString();
            //参照select,check,image
            if (itemIndex == i) {
                //画背景
                g.setColor(selectedBackColor);
                fillRect(g, 2, 2 + fontHeight * (i - startLine), HBarWidth, fontHeight);
                //打字
                g.setColor(selectedForeColor);
                drawString(g, item.getItemString(), 2 - startLeft + CheckWidth + ImageWidth, 2 + fontHeight * (i - startLine));
            } else {
                //打字
                g.setColor(getForeColor());
                drawString(g, item.getItemString(), 2 - startLeft + CheckWidth + ImageWidth, 2 + fontHeight * (i - startLine));
            }
            if (showCheckBox) {
                g.setColor(getBackColor());
                fillRect(g, 2, 2 + fontHeight * (i - startLine), fontHeight, fontHeight);
                //选择
                g.setColor(borderColor);
                drawRect(g, 2 + fontHeight / 4, 2 + fontHeight * (i - startLine) + fontHeight / 4, fontHeight / 2, fontHeight / 2);
                if (item.isChecked()) {
                    //选择
                    fillRect(g, 2 + fontHeight / 4 + 2, 2 + fontHeight * (i - startLine) + fontHeight / 4 + 2, fontHeight / 2 - 3, fontHeight / 2 - 3);
                }
            }
            //图
            if (showItemImage && item.getImage() != null) {
                drawImage(g, item.getImage(), 2 - startLeft + CheckWidth, 2 + fontHeight * (i - startLine) + (fontHeight - item.getImage().getHeight()) / 2, 0);
            }
        }
    }

    public int getBorderStyle() {
        return borderStyle;
    }

    public void setBorderStyle(int borderStyle) {
        this.borderStyle = borderStyle;
        internalPaint();
    }

    public boolean isHideSelection() {
        return hideSelection;
    }

    public void setHideSelection(boolean hideSelection) {
        this.hideSelection = hideSelection;
        internalPaint();
    }

    public int getScrollBars() {
        return scrollBars;
    }

    public void setScrollBars(int scrollBars) {
        this.scrollBars = scrollBars;
        internalPaint();
    }

    public int getScrollBarWidth() {
        return barwidth;
    }

    public void setScrollBarWidth(int barwidth) {
        this.barwidth = barwidth;
        internalPaint();
    }

    public void setText(String text) {
        internalSetText(text);
    }

    private void internalSetText(String text) {
        super.setText(text);
        //内容处理为行
        String str = "";
        maxLineLenght = 0;
        while (text.indexOf('\n') > -1) {
            str = text.substring(0, text.indexOf('\n'));
            //加入
            addItem(str, null, false);
            text = text.substring(text.indexOf('\n') + 1);
        }
        if (text.length() > 0) {
            //text，最后一行
            addItem(text, null, false);
        }
        internalPaint();
    }

    private int getMaxLineNum() {
        //获取总行数
        maxLineNum = itemList.size();
        return maxLineNum;
    }

    private int getmaxVisibleLineNum() {
        maxVisibleLineNum = getVBarHeight() / getFont().getHeight();
        getMaxLineNum();
        if (maxVisibleLineNum > maxLineNum) {
            maxVisibleLineNum = maxLineNum;
        }
        return maxVisibleLineNum;
    }

    private int getMaxLineLenght() {
        //获取最大行长度
        int fontHeight = getFont().getHeight();
        int CheckWidth = showCheckBox ? fontHeight : 0;
        int ImageWidth = showItemImage ? fontHeight : 0;
        String str = null;
        ListItemUI item = null;
        maxLineLenght = getHBarWidth();
        for (int i = 0; i < itemList.size(); i++) {
            item = (ListItemUI) itemList.elementAt(i);
            str = item.getItemString();
            if (maxLineLenght < getFont().stringWidth(str) + CheckWidth + ImageWidth) {
                maxLineLenght = getFont().stringWidth(str) + CheckWidth + ImageWidth;
            }
        }
        return maxLineLenght;
    }

    public int getBorderColor() {
        return borderColor;
    }

    public void setBorderColor(int borderColor) {
        this.borderColor = borderColor;
        internalPaint();
    }

    public int getScrollBarColor() {
        return scrollBarColor;
    }

    public void setScrollBarColor(int scrollBarColor) {
        this.scrollBarColor = scrollBarColor;
        internalPaint();
    }

    public int getSelectedBackColor() {
        return selectedBackColor;
    }

    public void setSelectedBackColor(int selectedBackColor) {
        this.selectedBackColor = selectedBackColor;
    }

    public int getSelectedForeColor() {
        return selectedForeColor;
    }

    public void setSelectedForeColor(int selectedForeColor) {
        this.selectedForeColor = selectedForeColor;
        repaintControl();
    }

    private int getHBarWidth() {
        int swidth = 0;
        if ((scrollBars == ConstsUI.srHorizontal) || (scrollBars == ConstsUI.srNone)) {
            swidth = getWidth();
        }
        if ((scrollBars == ConstsUI.srBoth) || (scrollBars == ConstsUI.srVertical)) {
            swidth = getWidth() - barwidth;
        }
        return swidth;
    }

    private int getVBarHeight() {
        int sheight = 0;
        if ((scrollBars == ConstsUI.srVertical) || (scrollBars == ConstsUI.srNone)) {
            sheight = getHeight();
        }
        if ((scrollBars == ConstsUI.srBoth) || (scrollBars == ConstsUI.srHorizontal)) {
            sheight = getHeight() - barwidth;
        }
        return sheight;
    }

    public int getItemIndex() {
        return itemIndex;
    }

    public int getItemIndex(String itemValue) {
        return internalGetItemIndex(itemValue);
    }

    private int internalGetItemIndex(String itemValue) {
        ListItemUI item = null;
        int result = -1;
        for (int i = 0; i < itemList.size(); i++) {
            item = (ListItemUI) itemList.elementAt(i);
            if (item.getItemString().equals(itemValue)) {
                result = i;
                break;
            }
        }
        return result;
    }

    public void setItemIndex(int itemIndex) {
        this.itemIndex = itemIndex;
        internalPaint();
    }
    //处理事件的执行
    //点箭头滚动内容
    protected void doPointerPressed(int x, int y) {
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        int thisWidth = getWidth();
        int thisHeight = getHeight();
        int VBarHeight = getVBarHeight();
        int HBarWidth = getHBarWidth();
        int fontHeight = getFont().getHeight();
        maxLineLenght = getMaxLineLenght();
        //确定点击了滚动条区域
        if ((scrollBars == ConstsUI.srHorizontal) || (scrollBars == ConstsUI.srBoth)) {
            //判断是否点击了左箭头
            if (InClientRect(posx, posy, 0, thisHeight - barwidth, barwidth, barwidth)) {
                //向右滚动
                if (startLeft > 0) {
                    int step = getFont().charWidth('x');
                    startLeft -= step;
                    if (startLeft < 0) {
                        startLeft = 0;
                    }
                }
            } else //右箭头
            if (InClientRect(posx, posy, HBarWidth - barwidth, thisHeight - barwidth, barwidth, barwidth)) {
                //向左滚动
                if (maxLineLenght - startLeft > HBarWidth) {
                    int step = getFont().charWidth('x');
                    startLeft += step;
                }
            } else //滚动条
            if (InClientRect(posx, posy, HSLeft, HSTop, HSWidth, HSHeight)) {
                //记录点击的滚动条位置
                oldScrollPointx = posx;
                oldScrollPointy = posy;
                isscrollbarpointdown = true;
                scrollbardownHV = 0;
                oldStartLeft = startLeft;
            } else {
                if (InClientRect(posx, posy, 0, thisHeight - barwidth, HBarWidth, barwidth)) {
                    //点了空白的，滚动到点击的位置
                    //计算滚动块要到位置
                    int tpos = posx > HSLeft ? (posx - HSWidth) : (posx);
                    //计算StartLeft
                    startLeft = ((tpos - barwidth) * (maxLineLenght)) / (HBarWidth - 2 * barwidth);
                }
            }
        }

        if ((scrollBars == ConstsUI.srVertical) || (scrollBars == ConstsUI.srBoth)) {

            //只有垂直滚动条
            //判断是否点击了上箭头
            if (InClientRect(posx, posy, thisWidth - barwidth, 0, barwidth, barwidth)) {
                //向下滚动
                if (startLine > 0) {
                    startLine--;
                }
            } else //下箭头
            if (InClientRect(posx, posy, thisWidth - barwidth, VBarHeight - barwidth, barwidth, barwidth)) {
                //向上滚动
                startLine = ((itemList.size() - startLine) * fontHeight > VBarHeight) ? startLine + 1 : startLine;
            } else //滚动条
            if (InClientRect(posx, posy, VSLeft, VSTop, VSWidth, VSHeight)) {
                //记录位置
                oldScrollPointx = posx;
                oldScrollPointy = posy;
                isscrollbarpointdown = true;
                scrollbardownHV = 1;
                oldStartLine = startLine;
            } else {
                if (InClientRect(posx, posy, thisWidth - barwidth, 0, barwidth, VBarHeight)) {
                    //空白的
                    //计算滚动块要到位置
                    int tpos = posy > VSTop ? (posy - VSHeight) : (posy);
                    //计算StartLine
                    int oldline = startLine;
                    startLine = ((tpos - barwidth) * (itemList.size() * fontHeight)) / (VBarHeight - 2 * barwidth) / fontHeight;
                    if (oldline == startLine) {
                        startLine = posy > VSTop ? startLine + 1 : startLine - 1;
                    }
                }

            }
        }
        //点了滚动条外       
        if (InClientRect(posx, posy, 0, 0, HBarWidth, VBarHeight)) {
            itemIndex = posy / fontHeight + startLine;
            if (itemIndex > itemList.size() - 1) {
                itemIndex = itemList.size() - 1;
            //处理选择
            }
            if (InClientRect(posx, posy, 2, 2 + fontHeight * (itemIndex - startLine), fontHeight, fontHeight)) {
                setItemChecked(itemIndex, !getItemChecked(itemIndex));
            }
        }

    }
    //拖动事件处理
    protected void doPointerReleased(int x, int y) {
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        int VBarHeight = getVBarHeight();
        int HBarWidth = getHBarWidth();
        //根据点击的位置,判断滚动的多少
        if ((scrollBars == ConstsUI.srHorizontal) || ((scrollBars == ConstsUI.srBoth) && (scrollbardownHV == 0))) {
            if (isscrollbarpointdown) {
                int stepx = posx - oldScrollPointx;
                //根据滚动多少来重新定位
                //计算滚动块要到位置
                int tpos = HSLeft + stepx;
                HSLeft = tpos < barwidth ? barwidth : tpos;
                HSLeft = HSLeft + HSWidth > HBarWidth - barwidth ? HBarWidth - barwidth - HSWidth : HSLeft;
                //计算StartLeft
                startLeft = ((HSLeft - barwidth) * (maxLineLenght)) / (HBarWidth - 2 * barwidth);
                if (oldStartLeft != startLeft) {
                    oldScrollPointx = posx;
                    oldStartLeft = startLeft;
                } else {
                    oldStartLeft = startLeft;
                    if (stepx > 0) {
                        startLeft = maxLineLenght - startLeft < getHBarWidth() ? startLeft + 1 : startLeft;
                    }
                    if (stepx < 0) {
                        startLeft = startLeft == 0 ? 0 : startLeft - 1;
                    }
                }
            }
        }
        if ((scrollBars == ConstsUI.srVertical) || ((scrollBars == ConstsUI.srBoth) && (scrollbardownHV == 1))) {
            if (isscrollbarpointdown) {
                int stepy = posy - oldScrollPointy;
                //根据滚动多少来重新定位
                //计算滚动块要到位置
                int tpos = VSTop + stepy;
                VSTop = tpos < barwidth ? barwidth : tpos;
                VSTop = VSTop + VSHeight > VBarHeight - barwidth ? VBarHeight - barwidth - VSHeight : VSTop;
                //计算StartLine
                startLine = ((VSTop - barwidth) * (itemList.size() * getFont().getHeight())) / (VBarHeight - 2 * barwidth) / getFont().getHeight();
                if (oldStartLine == startLine) {
                    if (stepy > 0) {
                        startLine = ((itemList.size() - startLine) * getFont().getHeight() > getVBarHeight()) ? startLine + 1 : startLine;
                    }
                    if (stepy < 0) {
                        startLine = startLine > 0 ? startLine - 1 : startLine;
                    }
                    oldStartLine = startLine;
                } else {
                    if ((stepy > 0) && (startLine < oldStartLine)) {
                        startLine = oldStartLine;
                    }
                    oldStartLine = startLine;
                }
            }
        }
        isscrollbarpointdown = false;
    }
    //键盘事件处理
    protected void doKeyUp(int keyCode) {
        maxLineLenght = getMaxLineLenght();
        maxVisibleLineNum = getmaxVisibleLineNum();
        String key = form.getKeyName(keyCode).toUpperCase();
        if (key.equals(ConstsUI.KEY_UP)) {
            if (itemIndex > startLine) {
                itemIndex--;
                repaintControl();
                return;
            }
            //上
            if (startLine > 0) {
                startLine--;
                itemIndex = startLine;
            }
        }
        if (key.equals(ConstsUI.KEY_DOWN)) {
            if (itemIndex < startLine + maxVisibleLineNum - 1) {
                itemIndex++;
                repaintControl();
                return;
            }
            //下
            startLine = ((itemList.size() - startLine) * getFont().getHeight() > getVBarHeight()) ? startLine + 1 : startLine;
            itemIndex = startLine + maxVisibleLineNum - 1;
        }
        if (itemIndex < startLine) {
            itemIndex = startLine;
        }
        if (itemIndex > startLine + maxVisibleLineNum) {
            itemIndex = startLine + maxVisibleLineNum;
        }
        if (key.equals(ConstsUI.KEY_LEFT)) {
            //左
            if (startLeft > 0) {
                int step = getFont().charWidth('x');
                startLeft -= step;
                if (startLeft < 0) {
                    startLeft = 0;
                }
            }
        }
        if (key.equals(ConstsUI.KEY_RIGHT)) {
            //右
            if (maxLineLenght - startLeft > getHBarWidth()) {
                int step = getFont().charWidth('x');
                startLeft += step;
            }
        }
        if(key.equals(ConstsUI.KEY_SELECT)||key.equals(ConstsUI.KEY_ENTER)){
            setItemChecked(itemIndex, !getItemChecked(itemIndex));
        }
        repaintControl();
    }

    protected boolean canDownTabNext() {
        boolean result = false;
        if (itemIndex == itemList.size() - 1) {
            result = true;
        }
        return result;
    }

    protected boolean canLeftTabPrior() {
        boolean result = false;
        if (scrollBars == ConstsUI.srHorizontal || scrollBars == ConstsUI.srBoth) {
            if (startLeft > 0) {
                result = false;
            } else {
                result = true;
            }
        } else {
            if (itemIndex == 0 || itemIndex == -1) {
                result = true;
            }
        }
        return result;
    }

    protected boolean canRightTabNext() {
        boolean result = false;
        if (scrollBars == ConstsUI.srHorizontal || scrollBars == ConstsUI.srBoth) {
            if (maxLineLenght - startLeft > getHBarWidth()) {
                result = false;
            } else {
                result = true;
            }
        } else {
            if (itemIndex == itemList.size() - 1) {
                result = true;
            }
        }
        return result;
    }

    protected boolean canUpTabPrior() {
        boolean result = false;
        if (itemIndex == 0 || itemIndex == -1) {
            result = true;
        }
        return result;
    }
}
