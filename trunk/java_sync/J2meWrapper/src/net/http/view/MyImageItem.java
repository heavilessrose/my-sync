package net.http.view;

import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.ImageItem;

/**
 * MyImageItem is the subclass of javax.microedition.lcdui.ImageItem.it add the field name so that FormUtil can 
 * collect the data in MyImageItem.if you want use MyImageItem,you should convert the image to byte[] first.
 *
 * @author mingjava
 * @version 0.1 05/06/2006
 * @since httpme 0.1
 */
public class MyImageItem extends ImageItem{
    
    private byte[] imgData = null;
    private String fieldName = "";
    private String fileName = "";
    /** Creates a new instance of MyImageItem */
    public MyImageItem(String _label,byte[] _img,int _layout,String _altText,String _fieldName,String _fileName) {
        super(_label,Image.createImage(_img,0,_img.length),_layout,_altText);
        this.imgData = _img;
        this.setFieldName(_fieldName);
        this.setFileName(_fileName);
                
    }
    //not open this version
    private MyImageItem(String _label,Image _img,int _layout,String _altText,String _fieldName){
        super(_label,_img,_layout,_altText);
        //fix me
    }
    /**
     * get the image data in MyImageItem
     * @return byte[] image data
     */
    public byte[] getImageData(){
        return imgData;
    }
    /**
     * set the image data
     * @param imgData the image data in byte[]
     */
    public void setImageData(byte[] imgData){
        this.imgData = imgData;
    }
    
    /**
     * get the field name
     * @return the field name of MyImageItem
     */
    public String getFieldName() {
        return fieldName;
    }
    /**
     * set the field name
     * @param fieldName the field name of MyImageItem
     */
    public void setFieldName(String fieldName) {
        this.fieldName = fieldName;
    }

    public String getFileName() {
        return fileName;
    }

    public void setFileName(String filename) {
        this.fileName = filename;
    }

}
