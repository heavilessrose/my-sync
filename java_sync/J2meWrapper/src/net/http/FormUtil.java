package net.http;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Item;

import net.http.view.MyChoiceGroup;
import net.http.view.MyDateField;
import net.http.view.MyImageItem;
import net.http.view.MyTextField;

/**
 * Utility class to collect data from a lcdui form automaticlly.if you want to use the class you must use item
 * defined in com.j2medev.httpme.view package.if the form contained MyImageItem then use collectMultipart() method
 * ,httpme 0.1 only support one MyImageItem in a form.
 *
 * @author mingjava
 * @version 0.1 05/06/2005
 * @since httpme 0.1
 */
public class FormUtil {
    
    public static final String MY_TEXT_FIELD = "com.j2medev.httpme.view.MyTextField";
    public static final String MY_DATE_FIELD = "com.j2medev.httpme.view.MyDateField";
    public static final String MY_CHOICE_GROUP = "com.j2medev.httpme.view.MyChoiceGroup";
    public static final String MY_IMAGE_ITEM = "com.j2medev.httpme.view.MyImageItem";

    public FormUtil() {
    }
    
    /**
     * collect the data contained in the form,if the form contains a MyImageItem,you should use this method
     * @param form the form contains data
     * @return encoded data in byte[]
     */
    public static byte[] collectMultipart(Form form){
        int size = form.size();
        if(size == 0){
            throw new IllegalArgumentException("No item in the form");
        }
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(baos);
        try{
            for(int i=0;i<size;i++){
                Item item = form.get(i);
                if(item.getClass().getName().equals(MY_TEXT_FIELD)){
                    dos.write(("--"+HttpUtil.BOUNDARY+"\r\n").getBytes());
                    MyTextField field = (MyTextField)item;
                    dos.write(("Content-Disposition: form-data; name=\""+field.getFieldName()+"\"\r\n\r\n").getBytes());
                    dos.write(URLEncoder.encode(field.getString(),"UTF-8").getBytes());
                    dos.write("\r\n".getBytes());
                }else if(item.getClass().getName().equals(MY_DATE_FIELD)){
                    dos.write(("--"+HttpUtil.BOUNDARY+"\r\n").getBytes());
                    MyDateField field = (MyDateField)item;
                    dos.write(("Content-Disposition: form-data; name=\""+field.getFieldName()+"\"\r\n\r\n").getBytes());
                    dos.write(URLEncoder.encode(field.getDateByString(),"UTF-8").getBytes());
                    dos.write("\r\n".getBytes());
                }else if(item.getClass().getName().equals(MY_CHOICE_GROUP)){
                    MyChoiceGroup field = (MyChoiceGroup)item;
                    dos.write(("--"+HttpUtil.BOUNDARY+"\r\n").getBytes());
                    dos.write(("Content-Disposition: form-data; name=\""+field.getFieldName()+"\"\r\n\r\n").getBytes());
                    String[] data = field.getSelectedItem();
                    String fieldName = field.getFieldName();
                    for(int j = 0;j<data.length;j++){
                        dos.write((fieldName+"="+URLEncoder.encode(data[j],"UTF-8")+"&").getBytes());
                    }
                    dos.write("\r\n".getBytes());
                }else if(item.getClass().getName().equals(MY_IMAGE_ITEM)){
                    MyImageItem field = (MyImageItem)item;
                    dos.write(("--"+HttpUtil.BOUNDARY+"\r\n").getBytes());
                    dos.write(("Content-Disposition: form-data; name=\""+field.getFieldName()+"\";filename=\""+field.getFileName()+"\"\r\n").getBytes());
                    dos.write("Content-Type: application/octet-stream\r\n\r\n".getBytes());
                    dos.write(field.getImageData(),0,1024);
                    dos.write("\r\n".getBytes());
                }
            }
            dos.write(("--"+HttpUtil.BOUNDARY+"--").getBytes());
            dos.write("\r\n".getBytes());
            dos.close();
        }catch(IOException ex){
            ex.printStackTrace();
        }
        return baos.toByteArray();
    }
    
    /**
     * collect the data contained in the form,if the form contains a MyImageItem,you should use collectMultipart()
     * @param form the form contains data
     * @return encoded data in byte[]
     */
    public static byte[] collect(Form form){
        StringBuffer sb = new StringBuffer();
        int size = form.size();
        if(size == 0){
            throw new IllegalArgumentException("No item in the form");
        }
        for(int i = 0;i<size;i++){
            Item item = form.get(i);
            System.out.println(item.getClass().getName());
            if(item.getClass().getName().equals(MY_TEXT_FIELD)){
                MyTextField field = (MyTextField)item;
                sb.append(field.getFieldName()+"="+URLEncoder.encode(field.getString(),"UTF-8")+"&");
            }else if(item.getClass().getName().equals(MY_DATE_FIELD)){
                MyDateField field = (MyDateField)item;
                sb.append(field.getFieldName()+"="+URLEncoder.encode(field.getDateByString(),"UTF-8")+"&");
            }else if(item.getClass().getName().equals(MY_CHOICE_GROUP)){
                MyChoiceGroup field = (MyChoiceGroup)item;
                String[] data = field.getSelectedItem();
                String fieldName = field.getFieldName();
                for(int j = 0;j<data.length;j++){
                    sb.append(fieldName+"="+URLEncoder.encode(data[j],"UTF-8")+"&");
                }
            }
        }
        return sb.toString().getBytes();
    }
}
