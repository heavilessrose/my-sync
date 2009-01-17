package cn.sda.ui;

import javax.microedition.lcdui.Image;

/**
 *
 * @author Administrator
 */
public class SDAToolButton {

    private String caption="";
    private Image image=null;
    private int width=40;
    private int height=40;
    private int left=0;
    private int top=0;

    public String getCaption() {
        return caption;
    }

    public void setCaption(String caption) {
        this.caption = caption;
    }

    public Image getImage() {
        return image;
    }

    public void setImage(Image image) {
        this.image = image;
    }

    public int getHeight() {
        return height;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    protected int getWidth() {
        return width;
    }

    protected void setWidth(int width) {
        this.width = width;
    }

    public int getLeft() {
        return left;
    }

    public int getTop() {
        return top;
    }

    protected void setLeft(int left) {
        this.left = left;
    }

    protected void setTop(int top) {
        this.top = top;
    }
    
}
