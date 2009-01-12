package net.sf.mvc.mobile;

import javax.microedition.lcdui.Displayable;

public interface ProgressListner {
	Displayable getDisplay();
	void setMaxValue(int i);
	void setValue(int i);
}
