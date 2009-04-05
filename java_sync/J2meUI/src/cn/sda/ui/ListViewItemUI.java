package cn.sda.ui;

import javax.microedition.lcdui.Image;

/**
 *
 * @author Administrator
 */
public class ListViewItemUI {

    private String itemString="" ;
    private Image image=null;
    private Image selectedImage=null;
    private int col=-1;
    private int row=-1;

    public Image getImage() {
        return image;
    }

    public void setImage(Image image) {
        this.image = image;
    }

    public String getItemString() {
        return itemString;
    }

    public void setItemString(String itemString) {
        this.itemString = itemString;
    }

    public Image getSelectedImage() {
        return selectedImage;
    }

    public void setSelectedImage(Image selectedImage) {
        this.selectedImage = selectedImage;
    }

    public int getCol() {
        return col;
    }

    protected void setCol(int col) {
        this.col = col;
    }

    public int getRow() {
        return row;
    }

    protected void setRow(int row) {
        this.row = row;
    }
    
}
