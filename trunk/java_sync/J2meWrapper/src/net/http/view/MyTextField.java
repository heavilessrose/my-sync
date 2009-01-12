package net.http.view;

import javax.microedition.lcdui.TextField;

/**
 * MyTextField class is the subclass of javax.microedition.lcdui.TextField,It
 * only adds a field name so that FormUtil can collect the data in the
 * MyTextField.
 * 
 * @author mingjava
 * @version 0.1 05/06/2006
 * @since httpme 0.1
 */
public class MyTextField extends TextField {

	private String fieldName = "";

	public MyTextField(String _label, String _text, int _size, int _constraint,
			String _fieldName) {
		super(_label, _text, _size, _constraint);
		this.setFieldName(_fieldName);
	}

	/**
	 * get the field name of MyTextField
	 * 
	 * @return the field name
	 */
	public String getFieldName() {
		return fieldName;
	}

	/**
	 * set the field name of MyTextField
	 * 
	 * @param fieldName
	 *            a String
	 */
	public void setFieldName(String fieldName) {
		this.fieldName = fieldName;
	}
}
