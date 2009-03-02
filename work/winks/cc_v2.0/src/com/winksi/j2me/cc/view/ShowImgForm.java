package com.winksi.j2me.cc.view;

import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.control.GUIController;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.CommandListener;

/**
 * 图片预览界面
 * @author Jerry
 */
public class ShowImgForm extends Form implements CommandListener {

	private GUIController controller;
	private Command back;

	public ShowImgForm(GUIController controller) {
		super("");
		this.controller = controller;
		back = new Command(CPProperty.COMMAND_BACK[controller.getLanguage()], Command.BACK, 1);
		addCommand(back);
		setCommandListener(this);
	}

	public void setImage(Image img) {
		append(img);
	}

	public void commandAction(Command c, Displayable d) {
		if (c == back) {
			this.deleteAll();
			controller.handleEvent(GUIController.EventID.EVENT_FILELIST, null);
		}
	}

}
