package cn.sda.ui;

import cn.sda.event.KeybordEvent;
import cn.sda.event.ListViewSelectChangeEvent;
import cn.sda.event.ListViewSelectItemEvent;
import cn.sda.event.PointerEvent;
import java.util.Vector;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.Font;

/**
 * @author not attributable
 * @version 1.0
 */
public class SDAListView extends SDABaseControl {
    //边框
    private int borderStyle = SDAConsts.bsFixedSingle;
    //滚动边框(多行)
    private int scrollBars = SDAConsts.srNone;
    private int barwidth = 10;
    //边框线颜色
    private int borderColor = SDAConsts.clBlack;
    //滚动条颜色
    private int scrollBarColor = SDAConsts.clBtnFace;
    //选中的项目的颜色
    private int itemFocusedRectColor = SDAConsts.clBtnShadow;
    private int itemFocusedFontColor = SDAConsts.clActiveCaption;
    //列
    private int colCount = 3;
    //行
    private int rowHeight = 40;
    private boolean autoRowHeight = false;
    //内部变量
    //行数，行最大长度
    private int maxLenght = 0;
    private int maxHeight = 0;
    //垂直开始显示位置
    private int startTop = 0;
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
    //项目管理
    private Vector itemList = null;
    //但前选中项
    private SDAListViewItem curItem = null;
    //最大列，最大行
    private int maxCol = 0;
    private int maxRow = 0;
    //是否显示文字
    private boolean showItemText = false;
    private boolean showItemRect = true;
    //事件
    private ListViewSelectChangeEvent onSelectChange = null;
    private ListViewSelectItemEvent onSelectItem = null;

    public SDAListView() {
        super();
        internalSDAListView();
    }

    private void internalSDAListView() {
        itemList = new Vector();
        this.setVisible(true);
        setLeft(0);
        setTop(0);
        setBorderStyle(SDAConsts.bsFixedSingle);
        super.setWidth(160);
        setHeight(100);
        super.setBackColor(SDAConsts.clWhite);
        this.onPointerPressed = new PointerEvent() {

            public void Event(SDABaseControl ctrl, int x, int y) {
                doPointerPressed(x, y);
            }
        };
        this.onPointerReleased = new PointerEvent() {

            public void Event(SDABaseControl ctrl, int x, int y) {
                doPointerReleased(x, y);
            }
        };
        this.onKeyUp = new KeybordEvent() {

            public void Event(SDABaseControl ctrl, int keyCode) {
                doKeyUp(keyCode);
            }
        };
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
        g.setFont(getFont());
        InternalPaint(g);
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
            g.setFont(super.getFont());
            //外框
            SetClip(g);
            //透明
            if (!isTransparent()) {
                g.setColor(backColor);
                fillRect(g, 0, 0, thisWidth, thisHeight);
            }
            if (borderStyle == SDAConsts.bsFixed3D) {
                g.setColor(SDAConsts.clGray);
                drawLine(g, 0, 0, thisWidth, 0);
                drawLine(g, 0, 0, 0, thisHeight);
                g.setColor(SDAConsts.clBlack);
                drawLine(g, 1, 1, thisWidth - 1, 1);
                drawLine(g, 1, 1, 1, thisHeight - 1);
                g.setColor(SDAConsts.clBtnFace);
                drawLine(g, 2, thisHeight - 1, thisWidth - 2, thisHeight - 1);
                drawLine(g, thisWidth - 1, 2, thisWidth - 1, thisHeight - 1);
                g.setColor(SDAConsts.clWhite);
                drawLine(g, 1, thisHeight, thisWidth, thisHeight);
                drawLine(g, thisWidth, 1, thisWidth, thisHeight);
                if (isFoucsed()) {
                    g.setColor(SDAConsts.clFocusShadow);
                    drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
                }
            }
            if (borderStyle == SDAConsts.bsFixedSingle) {
                //边框
                g.setColor(this.getBorderColor());
                drawRect(g, 0, 0, thisWidth, thisHeight);
                if (isFoucsed()) {
                    g.setColor(SDAConsts.clFocusShadow);
                    drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
                }
            }
            //画项目
            paintItems(g);
            //滚动条
            if (borderStyle == SDAConsts.bsFixed3D) {
                SetClip(g, 2, 2, thisWidth - 2, thisHeight - 2);
            } else {
                SetClip(g, 1, 1, thisWidth - 1, thisHeight - 1);
            }
            if ((scrollBars == SDAConsts.srHorizontal) || (scrollBars == SDAConsts.srBoth)) {
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

                if (maxLenght > HBarWidth) {
                    pwidth = ((HBarWidth) * (HBarWidth - 2 * barwidth)) / maxLenght;
                }

                //计算块位置
                int ppos = (startLeft * (HBarWidth - 2 * barwidth)) / maxLenght;
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
            if ((scrollBars == SDAConsts.srVertical) || (scrollBars == SDAConsts.srBoth)) {
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
                if (maxHeight > VBarHeight) {
                    pheight = ((VBarHeight) * (VBarHeight - 2 * barwidth) / maxHeight);
                }
                //计算块位置
                int ppos = barwidth;
                if (maxHeight > 0) {
                    ppos = (startTop * (VBarHeight - 2 * barwidth)) / maxHeight;
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
            PaintChilds();
        }
    }
    //画项目
    private void paintItems(Graphics g) {
        getMaxInfo();
        setColRow();
        SetClip(g, 2, 2, getHBarWidth() - 2, getVBarHeight() - 4);
        //单个大小
        int itemWidth = maxLenght / colCount;
        int itemHeight = rowHeight;
        int itemLeft = 0;
        int itemTop = 0;
        Image image = null;
        int imageLeft = 0;
        int imageTop = 0;
        Font ft = getFont();
        int fontHeight = showItemText ? getFont().getHeight() : 0;
        SDAListViewItem item = null;
        for (int i = 0; i < itemList.size(); i++) {
            item = (SDAListViewItem) itemList.elementAt(i);
            if (item.getImage() != null) {
                if (item.equals(curItem)) {
                    image = item.getSelectedImage() != null ? item.getSelectedImage() : item.getImage();
                    itemLeft = item.getCol() * itemWidth;
                    itemTop = item.getRow() * itemHeight;
                    imageLeft = (itemWidth - image.getWidth()) / 2 + itemLeft - startLeft;
                    imageTop = (itemHeight - image.getHeight() - fontHeight) / 2 + itemTop - startTop;
                    drawImage(g, image, imageLeft, imageTop, 0);
                    if (showItemText) {
                        g.setColor(itemFocusedFontColor);
                        drawString(g, item.getItemString(),
                                (itemWidth - ft.stringWidth(item.getItemString())) / 2 + itemLeft - startLeft,
                                imageTop + image.getHeight() + 2);
                    }
                    if (showItemRect) {
                        g.setColor(itemFocusedRectColor);
                        drawRect(g, item.getCol() * itemWidth - startLeft, item.getRow() * rowHeight - startTop, itemWidth, itemHeight);
                    }
                } else {
                    image = item.getImage();
                    itemLeft = item.getCol() * itemWidth;
                    itemTop = item.getRow() * itemHeight;
                    imageLeft = (itemWidth - image.getWidth()) / 2 + itemLeft - startLeft;
                    imageTop = (itemHeight - image.getHeight() - fontHeight) / 2 + itemTop - startTop;
                    drawImage(g, image, imageLeft, imageTop, 0);
                    if (showItemText) {
                        g.setColor(foreColor);
                        drawString(g, item.getItemString(),
                                (itemWidth - ft.stringWidth(item.getItemString())) / 2 + itemLeft - startLeft,
                                imageTop + image.getHeight() + 2);
                    }
                }
            } else {
                if (item.equals(curItem)) {
                    itemLeft = item.getCol() * itemWidth;
                    itemTop = item.getRow() * itemHeight;
                    if (showItemText) {
                        g.setColor(itemFocusedFontColor);
                        drawString(g, item.getItemString(),
                                (itemWidth - ft.stringWidth(item.getItemString())) / 2 + itemLeft - startLeft, itemTop + (itemHeight - ft.getHeight()) / 2);
                    }
                    if (showItemRect) {
                        g.setColor(itemFocusedRectColor);
                        drawRect(g, item.getCol() * itemWidth - startLeft, item.getRow() * rowHeight - startTop, itemWidth, itemHeight);
                    }
                } else {
                    itemLeft = item.getCol() * itemWidth;
                    itemTop = item.getRow() * itemHeight;
                    if (showItemText) {
                        g.setColor(foreColor);
                        drawString(g, item.getItemString(),
                                (itemWidth - ft.stringWidth(item.getItemString())) / 2 + itemLeft - startLeft, itemTop + (itemHeight - ft.getHeight()) / 2);
                    }
                }
            }
        }
    }
    //获取最大长度和高度
    private void getMaxInfo() {
        //根据滚动条类型来自动安排
        int rowCount = 1;
        if (itemList.size() > 0) {
            rowCount = itemList.size() % colCount > 0 ? (itemList.size() / colCount + 1) : (itemList.size() / colCount);
        }
        if (!autoRowHeight) {
            if (rowCount * rowHeight > getVBarHeight()) {
                scrollBars = SDAConsts.srVertical;
            } else {
                scrollBars = SDAConsts.srNone;
            }
        } else {
            scrollBars = SDAConsts.srNone;
            rowHeight = getVBarHeight() / rowCount;
        }
        if (scrollBars == SDAConsts.srVertical) {
            maxLenght = getHBarWidth();
            maxHeight = rowCount * rowHeight;
        }
        if (scrollBars == SDAConsts.srNone) {
            maxLenght = getHBarWidth();
            maxHeight = getVBarHeight();
        }
    }

    //设置行列属性
    private void setColRow() {
        SDAListViewItem item = null;
        int id = 0;
        maxCol = 0;
        maxRow = 0;
        for (int i = 0; i < itemList.size(); i++) {
            item = (SDAListViewItem) itemList.elementAt(i);
            id = (i + 1) % colCount == 0 ? colCount - 1 : (i + 1) % colCount - 1;
            if (maxCol < id) {
                maxCol = id;
            }
            item.setCol(id);
            id = i / colCount;
            if (maxRow < id) {
                maxRow = id;
            }
            item.setRow(id);
        }
    }
    //处理项目
    public SDAListViewItem addItem() {
        SDAListViewItem item = new SDAListViewItem();
        itemList.addElement(item);
        repaintControl();
        return item;
    }

    public SDAListViewItem addItem(String itemString) {
        SDAListViewItem item = new SDAListViewItem();
        item.setItemString(itemString);
        itemList.addElement(item);
        repaintControl();
        return item;
    }

    public int getItemCount() {
        return itemList.size();
    }

    public SDAListViewItem addItem(String itemString, Image image, Image selectedImage) {
        SDAListViewItem item = new SDAListViewItem();
        item.setItemString(itemString);
        item.setImage(image);
        item.setSelectedImage(selectedImage);
        itemList.addElement(item);
        repaintControl();
        return item;
    }

    public void addItem(SDAListViewItem item) {
        if (!itemList.contains(item)) {
            itemList.addElement(item);
            repaintControl();
        }
    }

    public void removeItem(SDAListViewItem item) {
        if (itemList.contains(item)) {
            itemList.removeElement(item);
            repaintControl();
        }
    }

    public void removeItem(int itemIndex) {
        if (itemIndex > -1 && itemIndex < itemList.size()) {
            itemList.removeElementAt(itemIndex);
            repaintControl();
        }
    }

    public int getBorderStyle() {
        return borderStyle;
    }

    public void setBorderStyle(int borderStyle) {
        this.borderStyle = borderStyle;
        internalPaint();
    }

    public int getScrollBarWidth() {
        return barwidth;
    }

    public void setScrollBarWidth(int barwidth) {
        this.barwidth = barwidth;
        internalPaint();
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

    public int getColCount() {
        return colCount;
    }

    public void setColCount(int colCount) {
        this.colCount = colCount;
    }

    public int getRowHeight() {
        return rowHeight;
    }

    public void setRowHeight(int rowHeight) {
        this.rowHeight = rowHeight;
    }

    public int getItemSelectedColor() {
        return itemFocusedRectColor;
    }

    public void setItemSelectedColor(int itemSelectedColor) {
        this.itemFocusedRectColor = itemSelectedColor;
    }

    public SDAListViewItem getCurItem() {
        return curItem;
    }

    public int getItemIndex() {
        if (curItem == null) {
            return -1;
        } else {
            return itemList.indexOf(curItem);
        }
    }

    public void setItemIndex(int itemIndex) {
        if (itemIndex > -1 && itemIndex < itemList.size()) {
            curItem = (SDAListViewItem) itemList.elementAt(itemIndex);
        } else {
            curItem = null;
        }
    }

    public boolean isAutoRowHeight() {
        return autoRowHeight;
    }

    public void setAutoRowHeight(boolean autoRowHeight) {
        this.autoRowHeight = autoRowHeight;
    }

    public boolean isShowItemText() {
        return showItemText;
    }

    public void setShowItemText(boolean showItemText) {
        this.showItemText = showItemText;
    }

    public int getItemFocusedColor() {
        return itemFocusedRectColor;
    }

    public void setItemFocusedColor(int itemFocusedColor) {
        this.itemFocusedRectColor = itemFocusedColor;
    }

    public int getItemFocusedFontColor() {
        return itemFocusedFontColor;
    }

    public void setItemFocusedFontColor(int itemFocusedFontColor) {
        this.itemFocusedFontColor = itemFocusedFontColor;
    }

    public boolean isShowItemRect() {
        return showItemRect;
    }

    public void setShowItemRect(boolean showItemRect) {
        this.showItemRect = showItemRect;
    }

    private int getHBarWidth() {
        int swidth = 0;
        if ((scrollBars == SDAConsts.srHorizontal) || (scrollBars == SDAConsts.srNone)) {
            swidth = getWidth();
        }
        if ((scrollBars == SDAConsts.srBoth) || (scrollBars == SDAConsts.srVertical)) {
            swidth = getWidth() - barwidth;
        }
        return swidth;
    }

    private int getVBarHeight() {
        int sheight = 0;
        if ((scrollBars == SDAConsts.srVertical) || (scrollBars == SDAConsts.srNone)) {
            sheight = getHeight();
        }
        if ((scrollBars == SDAConsts.srBoth) || (scrollBars == SDAConsts.srHorizontal)) {
            sheight = getHeight() - barwidth;
        }
        return sheight;
    }

    private void getItemByPos(int x, int y) {
        int itemWidth = maxLenght / colCount;
        int col = (x + startLeft) / itemWidth;
        int row = (y + startTop) / rowHeight;
        SDAListViewItem item = getItemByColRow(col, row);
        if (item != null) {
            curItem = item;
        }
    }

    private SDAListViewItem getItemByColRow(int col, int row) {
        SDAListViewItem item = null;
        int index = row * colCount + col;
        if ((index < itemList.size()) && index > -1) {
            item = (SDAListViewItem) itemList.elementAt(index);
        }
        return item;
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
        //确定点击了滚动条区域
        if ((scrollBars == SDAConsts.srHorizontal) || (scrollBars == SDAConsts.srBoth)) {
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
                if (maxLenght - startLeft > HBarWidth) {
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
                    startLeft = ((tpos - barwidth) * (maxLenght)) / (HBarWidth - 2 * barwidth);
                }
            }
        }

        if ((scrollBars == SDAConsts.srVertical) || (scrollBars == SDAConsts.srBoth)) {

            //只有垂直滚动条
            //判断是否点击了上箭头
            if (InClientRect(posx, posy, thisWidth - barwidth, 0, barwidth, barwidth)) {
                //向下滚动
                if (startTop > 0) {
                    startTop--;
                }
            } else //下箭头
            if (InClientRect(posx, posy, thisWidth - barwidth, VBarHeight - barwidth, barwidth, barwidth)) {
                //向上滚动
                startTop = (maxHeight - startTop > VBarHeight) ? startTop + 1 : startTop;
            } else //滚动条
            if (InClientRect(posx, posy, VSLeft, VSTop, VSWidth, VSHeight)) {
                //记录位置
                oldScrollPointx = posx;
                oldScrollPointy = posy;
                isscrollbarpointdown = true;
                scrollbardownHV = 1;
                oldStartLine = startTop;
            } else {
                if (InClientRect(posx, posy, thisWidth - barwidth, 0, barwidth, VBarHeight)) {
                    //空白的
                    //计算滚动块要到位置
                    int tpos = posy > VSTop ? (posy - VSHeight) : (posy);
                    //计算StartLine
                    int oldline = startTop;
                    startTop = ((tpos - barwidth) * maxHeight) / (VBarHeight - 2 * barwidth);
                    if (oldline == startTop) {
                        startTop = posy > VSTop ? startTop + 1 : startTop - 1;
                    }
                }
            }
        }
        //点到空白按钮位置，获取当前Item
        if (InClientRect(posx, posy, 0, 0, getHBarWidth(), getVBarHeight())) {
            getItemByPos(posx, posy);
            doSelectChange();
            doSelectItem();
        }
    }
    //拖动事件处理
    protected void doPointerReleased(int x, int y) {
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        int VBarHeight = getVBarHeight();
        int HBarWidth = getHBarWidth();
        //根据点击的位置,判断滚动的多少
        if ((scrollBars == SDAConsts.srHorizontal) || ((scrollBars == SDAConsts.srBoth) && (scrollbardownHV == 0))) {
            if (isscrollbarpointdown) {
                int stepx = posx - oldScrollPointx;
                //根据滚动多少来重新定位
                //计算滚动块要到位置
                int tpos = HSLeft + stepx;
                HSLeft = tpos < barwidth ? barwidth : tpos;
                HSLeft = HSLeft + HSWidth > HBarWidth - barwidth ? HBarWidth - barwidth - HSWidth : HSLeft;
                //计算StartLeft
                startLeft = ((HSLeft - barwidth) * (maxLenght)) / (HBarWidth - 2 * barwidth);
                if (oldStartLeft != startLeft) {
                    oldScrollPointx = posx;
                    oldStartLeft = startLeft;
                } else {
                    oldStartLeft = startLeft;
                    if (stepx > 0) {
                        startLeft = maxLenght - startLeft < getHBarWidth() ? startLeft + 1 : startLeft;
                    }
                    if (stepx < 0) {
                        startLeft = startLeft == 0 ? 0 : startLeft - 1;
                    }
                }
            }
        }
        if ((scrollBars == SDAConsts.srVertical) || ((scrollBars == SDAConsts.srBoth) && (scrollbardownHV == 1))) {
            if (isscrollbarpointdown) {
                int stepy = posy - oldScrollPointy;
                //根据滚动多少来重新定位
                //计算滚动块要到位置
                int tpos = VSTop + stepy;
                VSTop = tpos < barwidth ? barwidth : tpos;
                VSTop = VSTop + VSHeight > VBarHeight - barwidth ? VBarHeight - barwidth - VSHeight : VSTop;
                //计算StartLine
                startTop = ((VSTop - barwidth) * (maxHeight * getFont().getHeight())) / (VBarHeight - 2 * barwidth) / getFont().getHeight();
                if (oldStartLine == startTop) {
                    if (stepy > 0) {
                        startTop = ((maxHeight - startTop) * getFont().getHeight() > getVBarHeight()) ? startTop + 1 : startTop;
                    }
                    if (stepy < 0) {
                        startTop = startTop > 0 ? startTop - 1 : startTop;
                    }
                    oldStartLine = startTop;
                } else {
                    if ((stepy > 0) && (startTop < oldStartLine)) {
                        startTop = oldStartLine;
                    }
                    oldStartLine = startTop;
                }
            }
        }
        isscrollbarpointdown = false;
    }
    //键盘事件处理
    protected void doKeyUp(int keyCode) {
        String key = form.getKeyName(keyCode).toUpperCase();
        SDAListViewItem item = null;
        if (key.equals(SDAConsts.KEY_UP)) {
            if (curItem != null) {
                item = getItemByColRow(curItem.getCol(), curItem.getRow() - 1);
                if (item != null) {
                    curItem = item;
                    doSelectChange();
                    if (curItem.getRow() * rowHeight < startTop) {
                        startTop = curItem.getRow() * rowHeight;
                    }
                }
            }
        }
        if (key.equals(SDAConsts.KEY_DOWN)) {
            if (curItem != null) {
                item = getItemByColRow(curItem.getCol(), curItem.getRow() + 1);
                if (item != null) {
                    curItem = item;
                    doSelectChange();
                    if (curItem.getRow() * rowHeight - startTop > getVBarHeight() - rowHeight) {
                        startTop = curItem.getRow() * rowHeight - getVBarHeight() + rowHeight;
                    }
                }
            }
        }
        if (key.equals(SDAConsts.KEY_LEFT)) {
            if (curItem != null) {
                item = getItemByColRow(curItem.getCol() - 1, curItem.getRow());
                if (item != null) {
                    curItem = item;
                    doSelectChange();
                    if (curItem.getRow() * rowHeight < startTop) {
                        startTop = curItem.getRow() * rowHeight;
                    }
                }
            }
        }
        if (key.equals(SDAConsts.KEY_RIGHT)) {
            if (curItem != null) {
                item = getItemByColRow(curItem.getCol() + 1, curItem.getRow());
                if (item != null) {
                    curItem = item;
                    doSelectChange();
                    if (curItem.getRow() * rowHeight - startTop > getVBarHeight() - rowHeight) {
                        startTop = curItem.getRow() * rowHeight - getVBarHeight() + rowHeight;
                    }
                }
            }
        }
        if (key.equals(SDAConsts.KEY_SELECT) || key.equals(SDAConsts.KEY_ENTER)) {
            doSelectItem();
        }
        repaintControl();
    }

    public void setOnSelectChange(ListViewSelectChangeEvent onSelectChange) {
        this.onSelectChange = onSelectChange;
    }

    private void doSelectChange() {
        if (this.onSelectChange != null) {
            if (curItem != null) {
                onSelectChange.Event(curItem);
            }
        }
    }

    public void setOnSelectItem(ListViewSelectItemEvent onSelectItem) {
        this.onSelectItem = onSelectItem;
    }

    private void doSelectItem() {
        if (this.onSelectItem != null) {
            if (curItem != null) {
                onSelectItem.Event(curItem);
            }
        }
    }

    protected boolean canDownTabNext() {
        return false;
    }

    protected boolean canLeftTabPrior() {
        return false;
    }

    protected boolean canRightTabNext() {
        return false;
    }

    protected boolean canUpTabPrior() {
        return false;
    }
    
}

