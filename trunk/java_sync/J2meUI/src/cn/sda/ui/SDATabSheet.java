package cn.sda.ui;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

/**
 *
 * @author Administrator
 */
public class SDATabSheet extends SDABaseControl {

    private String caption = "";
    private Image image = null;
    private int buttonWidth = 40;
    private int buttonHeight = 40;
    private int index=0;

    public SDATabSheet() {
        super();
        visible=false;
        backColor=SDAConsts.clWhite;
    }

    public void paint() {
        if (!IsCanPaint()) {
            return;
        }
        Graphics g = form.getGraphics();
        SetClip(g);
        g.setFont(getFont());
        if(!transparent){
            g.setColor(backColor);
            fillRect(g, 0, 0, width, height);
        }
        PaintChilds();
    }

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

    public int getButtonHeight() {
        return buttonHeight;
    }

    public void setButtonHeight(int buttonHeight) {
        this.buttonHeight = buttonHeight;
    }

    public int getButtonWidth() {
        return buttonWidth;
    }

    public void setButtonWidth(int buttonWidth) {
        this.buttonWidth = buttonWidth;
    }

    public int getIndex() {
        return index;
    }

    protected void setIndex(int index) {
        this.index = index;
    }
    
}
