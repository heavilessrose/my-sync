package cn.sda.ui;

import javax.microedition.lcdui.Image;

/**
 *
 * @author Administrator
 */
public class SDAListItem {
    private Image image;
    private String itemString="";
    private boolean checked=false;

    public boolean isChecked() {
        return checked;
    }

    protected void setChecked(boolean checked) {
        this.checked = checked;
    }

    public String getItemString() {
        return itemString;
    }

    protected void setItemString(String itemString) {
        this.itemString = itemString;
    }

    public Image getImage() {
        return image;
    }

    public void setImage(Image image) {
        this.image = image;
    }
    
}
