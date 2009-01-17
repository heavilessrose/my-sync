package cn.sda.ui;

import java.io.*;
import javax.microedition.lcdui.*;

/**
 * @author not attributable
 * @version 1.0
 */
public class SDAImage extends SDABaseControl {

    private Image image = null;
    private Image scaleImage = null;
    private boolean stretch = false;
    private boolean autoSize = false;
    private boolean center = false;

    public SDAImage() {
        this.tabStop = false;
        this.width = 50;
        this.height = 50;
    }

    private void DrawImage(Graphics g) {
        if (this.autoSize) {
            this.setWidth(image.getWidth());
            this.setHeight(image.getHeight());
            drawImage(g, image, 0, 0, 0);
            return;
        }
        if (this.stretch) {
            if (scaleImage == null || scaleImage.getWidth() != this.getWidth() ||
                    scaleImage.getHeight() != this.getHeight()) {
                scaleImage = SDAImageUtils.processImage(image, this.width, this.height, SDAImageUtils.MODE_STRETCH);
            }
            drawImage(g, scaleImage, 0, 0, 0);
            return;
        }
        if (this.center) {
            int cx = (this.getWidth() - image.getWidth()) / 2;
            int cy = (this.getHeight() - image.getHeight()) / 2;
            drawImage(g, image, cx, cy, 0);
            return;
        }
        drawImage(g, image, 0, 0, 0);
    }

    /**
     * paint
     *
     * @todo Implement this cn.sdaui.cldc.SDABaseControl method
     */
    public void paint() {
        if (!IsCanPaint()) {
            return;
        }
        Graphics g = form.getGraphics();
        SetClip(g);
        if (!transparent) {
            g.setColor(backColor);
            fillRect(g, 0, 0, width, height);
        }
        if (image != null) {
            DrawImage(g);
            System.out.println("w:"+image.getWidth());
            System.out.println("h:"+image.getHeight());
        }
        PaintChilds();
    }

    public boolean LoadImage(String imageName) {
        try {
            image = Image.createImage(imageName);
            //this.paint();
            return true;
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        return false;
    }

    public void LoadImage(byte[] imageData, int imageOffset, int imageLength) {
        image = Image.createImage(imageData, imageOffset, imageLength);
        this.paint();
    }

    public void LoadImage(Image image) {
        this.image = image;
        this.paint();
    }

    public void CreateEmptyImage(int width, int height) {
        image = Image.createImage(width, height);
        this.paint();
    }

    public Image getImage() {
        return image;
    }

    public boolean isAutoSize() {
        return autoSize;
    }

    public boolean isStretch() {
        return stretch;
    }

    public boolean isCenter() {
        return center;
    }

    public void setStretch(boolean stretch) {
        this.stretch = stretch;
    }

    public void setAutoSize(boolean autoSize) {
        this.autoSize = autoSize;
        if (autoSize) {
            if (getParent() != null) {
                getParent().paint();
            }
        }
        paint();
    }

    public void setCenter(boolean center) {
        this.center = center;
    }
}
