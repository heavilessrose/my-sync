package net.http.view;

import java.util.Date;
import javax.microedition.lcdui.DateField;

/**
 * MyDateField is the subclass of javax.microedition.lcdui.DateField,it add a field name so that 
 * FormUtil can collect the data in MyDateField.you can get the date in String style by invoke getDateByString().
 *
 * @author mingjava
 * @version 0.1 05/06/2006
 * @since httpme 0.1
 */
public class MyDateField extends DateField {
    
    private String fieldName = "";
    /** Creates a new instance of MyDateField */
    public MyDateField(String _label,int _mode,String _fieldName) {
        super(_label,_mode);
        this.setFieldName(_fieldName);
    }
    /**
     * get the field name of MyDateField
     * @return a string the field name 
     */
    public String getFieldName() {
        return fieldName;
    }
    /**
     * set the field name of MyDateField
     * @param fieldName the field name
     *
     */
    public void setFieldName(String fieldName) {
        this.fieldName = fieldName;
    }
    
    /**
     * get the date from MyDateField
     * @return a string represent the date,in fact it's a String Wrapper of long value
     */
    public String getDateByString(){
        return String.valueOf(getDate().getTime());
    }
}
