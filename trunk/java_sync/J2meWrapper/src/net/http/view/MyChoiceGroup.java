package net.http.view;

import javax.microedition.lcdui.ChoiceGroup;
import javax.microedition.lcdui.Image;

/**
 * MyChoiceGroup is the subclass of javax.microedition.lcdui.ChoiceGroup,it adds a field name so that
 * FormUtil can collect the data.
 *
 * @author mingjava
 * @version 0.1 05/06/2006
 * @since httpme 0.1
 */
public class MyChoiceGroup extends ChoiceGroup{
    
    private String fieldName;
    /** Creates a new instance of MyChoiceGroup */
    public MyChoiceGroup(String _label,int _type,String[] _elements,Image[] _imgs,String _fieldName) {
        super(_label,_type,_elements,_imgs);
        this.setFieldName(_fieldName);
    }
    
    /**
     * get the field name of MyChoiceGroup
     * @return the field name
     */
    public String getFieldName() {
        return fieldName;
    }

    /**
     * set the field name of MyChoiceGroup
     * @param fieldName 
     */
    public void setFieldName(String fieldName) {
        this.fieldName = fieldName;
    } 
    /**
     * Only when MyChoiceGroup is a EXCLUSIVE or PopUp mode you should invoke this method to get the data selected.
     * or you should invoke method getSelectedItem() instead.
     * @return the selected item
     */
    public String getSelected(){
        return getString(getSelectedIndex());
    }
    
    /**
     * get the selected item
     * @return a String array represent the selected item label
     */
    public String[] getSelectedItem(){
        boolean[] flag = new boolean[size()];
        int num = getSelectedFlags(flag);
        String[] items = new String[num];
        int j = 0;
        for(int i = 0;i<flag.length;i++){
            if(flag[i])
                items[j++] = getString(i);
        }
        return items;
    }
}
