package net.sf.mvc.mobile;

import javax.microedition.lcdui.Displayable;

public interface Model {
	public void load(Object parameter) throws Exception;
	public void setView(Displayable view);
}
