package net.sf.mvc.mobile.sample.view;

import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.TextField;

public class EnterNameView extends Form {

	private TextField nameField;

	public EnterNameView() {
		super("Enter your name");
		nameField = new TextField("Enter your name","",32, TextField.ANY);
		append(nameField);
	}

	public void setName(String name) {
		nameField.setString(name);
	}
	
	public String getName() {
		return nameField.getString();
	}

	public void setLabel(String label) {
		nameField.setLabel(label);
	}
}
