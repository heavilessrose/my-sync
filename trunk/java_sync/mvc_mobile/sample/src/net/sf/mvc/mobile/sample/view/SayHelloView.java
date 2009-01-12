package net.sf.mvc.mobile.sample.view;

import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.StringItem;

public class SayHelloView  extends Form {

	private StringItem stringItem;

	public SayHelloView() {
		super("Hello");
		stringItem = new StringItem("Hello","");
		append(stringItem);
	}
	
	public void setText(String text){
		stringItem.setText(text);		
	}

}
