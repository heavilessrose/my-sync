package cn.sda.ui;

import java.util.Vector;
import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

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
public class MenuItemUI {

    protected String itemName;
    protected String itemCaption;
    protected int itemIndex = -1;
    protected PopMenuUI popMenu;
    protected int focusedIndex = 0;
    protected MenuItemUI parentItem = null;
    protected Vector subItems = new Vector();
    protected int originLeft;
    protected int originTop;
    protected int width;
    protected int height;
    protected int clientOriginLeft;
    protected int clientOriginTop;
    protected int clientWidth;
    protected int clientHeight;
    protected boolean isSubShow = false;
    //checked
    protected boolean checked = false;
    protected boolean radioItem = false;
    protected Image itemImage = null;

    protected MenuItemUI(String itemName, String itemCaption) {
        this.itemName = itemName;
        this.itemCaption = itemCaption;
    }

    public final MenuItemUI AddItem(String itemName, String itemCaption) {
        MenuItemUI item = new MenuItemUI(itemName, itemCaption);
        item.popMenu = this.popMenu;
        item.parentItem = this;
        item.itemIndex = subItems.size();
        subItems.addElement(item);
        return item;
    }

    public final MenuItemUI AddItem(MenuItemUI item) {
        item.popMenu = this.popMenu;
        item.parentItem = this;
        item.itemIndex = subItems.size();
        subItems.addElement(item);
        return item;
    }

    protected final int getWidth() {
        return this.popMenu.getFont().stringWidth(itemCaption) + 2 * this.popMenu.getFont().getHeight();
    }

    protected final void calcClientWidth() {
        this.clientWidth = 0;
        for (int i = 0; i < subItems.size(); i++) {
            MenuItemUI item = (MenuItemUI) subItems.elementAt(i);
            int itemWidth = item.getWidth();
            if (itemWidth > this.clientWidth) {
                this.clientWidth = itemWidth;
            }
        }
    }

    protected final void calcClientHeight() {
        MenuItemUI item = null;
        int cheight = 0;
        int fontHeight = popMenu.getFont().getHeight();
        for (int i = 0; i < subItems.size(); i++) {
            item = (MenuItemUI) subItems.elementAt(i);
            if (item.itemCaption.equals("-")) {
                cheight += fontHeight / 2;
            } else {
                cheight += fontHeight;
            }
        }
        this.clientHeight = cheight + 1;
    }

    protected final void SetClip(Graphics g, int left, int top, int width,
            int height) {
        g.setClip(this.originLeft + left, this.originTop + top, width, height);
    }

    protected void fillRect(Graphics g, int left, int top, int width,
            int height) {
        g.fillRect(this.originLeft + left, this.originTop + top, width, height);
    }

    protected void drawRect(Graphics g, int left, int top, int width,
            int height) {
        g.drawRect(this.originLeft + left, this.originTop + top, width, height);
    }

    protected void drawString(Graphics g, String text, int left, int top) {
        g.drawString(text, this.originLeft + left, this.originTop + top, 0);
    }

    protected void drawLine(Graphics g, int x1, int y1, int x2, int y2) {
        g.drawLine(this.originLeft + x1, this.originTop + y1, this.originLeft + x2, this.originTop + y2);
    }

    protected void drawImage(Graphics g, Image image, int left, int top) {
        g.drawImage(image, this.originLeft + left, this.originTop + top, 0);
    }

    protected void fillCircle(Graphics g, int left, int top, int r, int startArc, int endArc) {
        g.fillArc(this.originLeft + left, this.originTop + top, 2 * r, 2 * r, startArc, endArc);
    }

    protected final void SetClientClip(Graphics g, int left, int top, int width,
            int height) {
        g.setClip(this.clientOriginLeft + left, this.clientOriginTop + top,
                width, height);
    }

    protected void fillClientRect(Graphics g, int left, int top, int width,
            int height) {
        g.fillRect(this.clientOriginLeft + left, this.clientOriginTop + top,
                width, height);
    }

    protected void drawClientRect(Graphics g, int left, int top, int width,
            int height) {
        g.drawRect(this.clientOriginLeft + left, this.clientOriginTop + top,
                width, height);
    }

    protected void drawClientString(Graphics g, String text, int left, int top) {
        g.drawString(text, this.clientOriginLeft + left,
                this.clientOriginTop + top, 0);
    }

    protected boolean InScreenRect(int x, int y) {
        return (x >= this.originLeft && x <= this.originLeft + this.width) &&
                (y >= this.originTop && y <= this.originTop + this.height);
    }

    protected void fillTriangle(Graphics g, int x1, int y1, int x2, int y2,
            int x3,
            int y3) {
        g.fillTriangle(this.originLeft + x1, this.originTop + y1,
                this.originLeft + x2, this.originTop + y2,
                this.originLeft + x3, this.originTop + y3);
    }

    protected void drawItem(Graphics g, boolean isFocused) {
        Font ft = popMenu.getFont();
        int fontHeight = ft.getHeight();
        if (isFocused) {
            g.setColor(ConstsUI.clFocusShadow);
            this.fillRect(g, 1, 1,
                    width - 2,
                    height - 1);
            g.setColor(ConstsUI.clWhite);
        } else {
            g.setColor(ConstsUI.clBlack);
        }
        //判断内容是不是"-"，是的话就是分割线
        if (itemCaption.equals("-")) {
            drawLine(g, 0, fontHeight / 4, width, fontHeight / 4);
        } else {
            //选择
            if (checked) {
                if (radioItem) {
                    fillCircle(g, fontHeight / 2 - 3, fontHeight / 2 - 3, 3, 0, 360);
                } else {
                    this.drawString(g, "√", (fontHeight - ft.charWidth('√')) / 2, 1);
                }
            }
            //图
            if (itemImage != null) {
                drawImage(g, itemImage, 2, (fontHeight-itemImage.getHeight())/2+1);
            }
            //字
            g.setFont(popMenu.getFont());
            this.drawString(g, itemCaption, this.popMenu.getFont().getHeight() + 1, 1);
            if (!subItems.isEmpty()) {
                g.setColor(ConstsUI.clBlack);
                int x1 = this.width - 4;
                int y1 = this.height / 2;
                int x2 = this.width - 8;
                int y2 = y1 + 4;
                int x3 = this.width - 8;
                int y3 = y1 - 4;
                this.fillTriangle(g, x1, y1, x2, y2, x3, y3);
            }
        }
    }

    public MenuItemUI getMenuItem(int index) {
        if (!subItems.isEmpty()) {
            if (index > -1 && index < subItems.size()) {
                return (MenuItemUI) subItems.elementAt(index);
            } else {
                return null;
            }
        } else {
            return null;
        }
    }

    public boolean isChecked() {
        return checked;
    }

    public void setChecked(boolean checked) {
        //设置(radio的时候同级别只能够一个)
        MenuItemUI item = null;
        if (!radioItem) {
            this.checked = checked;
        } else {
            if (checked) {
                if (parentItem != null) {
                    for (int i = 0; i < parentItem.subItems.size(); i++) {
                        item = (MenuItemUI) parentItem.subItems.elementAt(i);
                        if (item.radioItem) {
                            item.checked = false;
                        }
                    }
                }
                this.checked = true;
            }
        }
    }

    public Image getItemImage() {
        return itemImage;
    }

    public void setItemImage(Image itemImage) {
        this.itemImage = itemImage;
    }

    public boolean isRadioItem() {
        return radioItem;
    }

    public void setRadioItem(boolean radioItem) {
        this.radioItem = radioItem;
        MenuItemUI item = null;
        //判断checked
        if (radioItem && checked) {
            for (int i = 0; i < parentItem.subItems.size(); i++) {
                item = (MenuItemUI) parentItem.subItems.elementAt(i);
                if (item.radioItem) {
                    item.checked = false;
                }
            }
        }
    }
}
