package com.winksi.j2me.cc.view;

import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.control.GUIController;
import com.winksi.j2me.cc.tools.ProtocolTools;
import java.io.UnsupportedEncodingException;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.TextBox;
import javax.microedition.lcdui.TextField;

/**
 * 问候语设置界面
 * @author Administrator
 */
public class GreetingTextBox extends TextBox implements CommandListener {

	private GUIController controller;
	private Command upload, ok, back, skip;

	public GreetingTextBox(GUIController controller, String title, int maxsize) {
		super(title, "", maxsize, TextField.ANY);
		this.controller = controller;
		upload = new Command(CPProperty.COMMAND_UPLOAD[this.controller.getLanguage()], Command.OK, 1);
		ok = new Command(CPProperty.COMMAND_OK[this.controller.getLanguage()], Command.OK, 1);
		back = new Command(CPProperty.COMMAND_BACK[this.controller.getLanguage()], Command.BACK, 1);
		skip = new Command(CPProperty.COMMAND_SKIP[this.controller.getLanguage()], Command.SCREEN, 2);
		addCommand(ok);
		addCommand(upload);
		addCommand(skip);
		addCommand(back);
		setCommandListener(this);
	}

	public void commandAction(Command c, Displayable d) {
		if (c == upload) {
			try {
				//                System.out.println("greeting : "+controller.getCurrentFileName());
				if (controller.getCurFileName().indexOf(".") <= 0) {
					controller.initDiyModel();
				}
				String text = this.getString();
				//                 System.out.println("greeting : "+text);

				if (text.length() > 0 && !text.equals("")) {
					controller.setDiyMsg(text.getBytes(CPProperty.UTF8));
				}

				controller.handleEvent(GUIController.EventID.EVENT_INIT_UPLOAD_DIY, null);
				this.setString("");
			} catch (Exception ex) {
				ex.printStackTrace();
			}
		} else if (c == ok) {
			try {
				String text = this.getString();
				System.out.println("custom : " + text);
				if (controller.getCustom() == ProtocolTools.PROPERTY_CUSTOM_SUPPORT) {
					if (text.length() > 0 && !text.equals("")) {
						controller.setDiyMsg(text.getBytes(CPProperty.UTF8));
						controller.pushPreEvent(GUIController.EventID.EVENT_SHOW_GREETING);
						controller.handleEvent(GUIController.EventID.EVENT_NAMELIST, null);
					}
				} else {
					try {
						if (controller.getCurFileName().indexOf(".") <= 0) {
							controller.initDiyModel();
						}
						//                 System.out.println("greeting : "+text);

						if (text.length() > 0 && !text.equals("")) {
							controller.setDiyMsg(text.getBytes(CPProperty.UTF8));
						}

						controller.handleEvent(GUIController.EventID.EVENT_INIT_UPLOAD_DIY, null);
						this.setString("");
					} catch (Exception ex) {
						ex.printStackTrace();
					}
				}
			} catch (UnsupportedEncodingException ex) {
				ex.printStackTrace();
			}
		} else if (c == skip) {
			//            System.out.println("custom : "+controller.getCustom());
			if ((controller.getCurFileName().indexOf(".") <= 0)) {
				controller.initDiyModel();
			}

			if (controller.getCustom() == ProtocolTools.PROPERTY_CUSTOM_SUPPORT) {
				controller.pushPreEvent(GUIController.EventID.EVENT_SHOW_GREETING);
				controller.handleEvent(GUIController.EventID.EVENT_NAMELIST, null);
			}
		} else if (c == back) {
			controller.handleEvent(GUIController.EventID.EVENT_BACK, null);
		}
	}
}
