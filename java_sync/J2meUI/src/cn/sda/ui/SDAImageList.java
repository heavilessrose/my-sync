package cn.sda.ui;

import java.util.Enumeration;
import java.util.Vector;
import javax.microedition.lcdui.Image;

/**
 *
 * @author Administrator
 */
public class SDAImageList {

    private Vector imageList = null;
    //需要刷新的组件列表
    private Vector imageControlList = null;

    public SDAImageList() {
        imageList = new Vector();
        imageControlList = new Vector();
    }

    public void addImage(Image image) {
        if (!imageList.contains(image)) {
            imageList.addElement(image);
            refreshImageControl();
        }
    }

    public void addImage(String imageName) {
        try {
            Image image = Image.createImage(imageName);
            addImage(image);
        } catch (Exception e) {
        }
    }

    public void deleteImage(Image image) {
        if (imageList.contains(image)) {
            imageList.removeElement(image);
            refreshImageControl();
        }
    }

    public void deleteImage(int imageIndex) {
        if (imageIndex > -1 && imageIndex < imageList.size()) {
            imageList.removeElementAt(imageIndex);
            refreshImageControl();
        }
    }

    public Image getImage(int imageIndex) {
        Image result = null;
        if (imageIndex > -1 && imageIndex < imageList.size()) {
            result = (Image) imageList.elementAt(imageIndex);
        }
        return result;
    }

    public int getImageCount() {
        return imageList.size();
    }

    public Enumeration getImageList() {
        return imageList.elements();
    }

    public void clear() {
        imageList.removeAllElements();
        refreshImageControl();
        System.gc();
    }
    //刷新组件
    private void refreshImageControl() {
        for (int i = 0; i < imageControlList.size(); i++) {
            SDABaseControl ctrl = (SDABaseControl) imageControlList.elementAt(i);
            if (ctrl != null) {
                ctrl.paint();
            }
        }
    }
    //设置需要刷新的组件
    protected void setWatchControl(SDABaseControl control, boolean setWatch) {
        if (setWatch) {
            if (!imageControlList.contains(control)) {
                imageControlList.addElement(control);
            }
        } else {
            int id = imageControlList.indexOf(control);
            if (id > -1) {
                imageControlList.removeElementAt(id);
            }
        }
    }
}
