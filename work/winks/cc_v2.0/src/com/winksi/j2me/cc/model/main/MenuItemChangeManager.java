package com.winksi.j2me.cc.model.main;

import com.winksi.j2me.cc.view.*;

import com.winksi.j2me.cc.control.GUIController;
import com.winksi.j2me.cc.tools.ProtocolTools;

public class MenuItemChangeManager implements MenuItemListener {

	private Menu menu;
	private GUIController controller;

	public MenuItemChangeManager(GUIController controller, Menu menu) {
		this.controller = controller;
		this.menu = menu;
		this.menu.setMenuItemListener(this);
	}

	/** 按键事件处理 */
	public void itemAction(int event) {
		switch (event) {
		case MenuEvent.SETTING_NEXT:
			menu.toHide();
			controller.handleEvent(GUIController.EventID.EVENT_SHOW_SURESET, null);
			break;
		case MenuEvent.SURESET_NEXT:
			menu.toHide();
			controller.handleEvent(GUIController.EventID.EVENT_SHOW_DECLARE, null);
			break;
		case MenuEvent.DECLARE_NEXT:// 免责声明 下一步
			menu.toHide();
			controller.handleEvent(GUIController.EventID.EVENT_REGISTER, null);
			break;
		case MenuEvent.REGISTER:
			menu.toHide();
			controller.handleEvent(GUIController.EventID.EVENT_REGISTER, null);
			break;
		case MenuEvent.HELP:
			menu.toHide();
			controller.handleEvent(GUIController.EventID.EVENT_HELP, null);
			break;
		case MenuEvent.ABOUT:
			menu.toHide();
			controller.handleEvent(GUIController.EventID.EVENT_ABOUT, null);
			break;
		case MenuEvent.BACK:
			menu.toHide();
			break;
		case MenuEvent.UPLOAD:
			menu.toHide();
			if (controller.isEventFileUpload()) {
				if (controller.getCurFileName().length() > 0) {
					String filename = controller.getCurFileName();
					//                    filename=filename.substring(filename.lastIndexOf('/')+1);
					//                        System.out.println("filename : " + filename);
					if (controller.setDiyImg(filename)) {
						controller.handleEvent(GUIController.EventID.EVENT_INIT_UPLOAD_DIY, null);
					}
				} else {
					break;
				}
			} else {
				//                    System.out.println("upload : " + controller.getCurrentFileName());
				//                    if (controller.getCurrentFileName().indexOf(".") > 0) {
				//                        controller.handleEvent(GUIController.EventID.EVENT_INIT_UPLOAD_DIY, null);
				//                    System.out.println("greeting : "+controller.getCurrentFileName());
				//                        controller.setDiyImg(controller.getCurrentFileName());
				//                    } else {
				//                        controller.initDiyModel();
				//                    }
				controller.handleEvent(GUIController.EventID.EVENT_INIT_UPLOAD_DIY, null);
			}
			break;
		case MenuEvent.DIY_IMG_SKIP:
			menu.toHide();
			//                if (controller.getCustom() == HttpTools.PROPERTY_CUSTOM_SUPPORT) {
			controller.setCurrentFileName("");
			controller.initDiyModel();
			controller.pushPreEvent(GUIController.EventID.EVENT_FILELIST);
			controller.handleEvent(GUIController.EventID.EVENT_SHOW_GREETING, null);
			//                }
			break;
		case MenuEvent.DIY_IMG_REVIEW:
			menu.toHide();
			controller.handleEvent(GUIController.EventID.EVENT_SHOW_REVIEW, null);
			break;
		case MenuEvent.DIY_BACK:
			menu.toHide();
			controller.handleEvent(GUIController.EventID.EVENT_BACK, null);
			break;
		case MenuEvent.PAY:
			menu.toHide();
			controller.handleEvent(GUIController.EventID.EVENT_REQUESTFORPAY, null);
			break;
		case MenuEvent.NOTPAY:
			menu.toHide();
			controller.handleEvent(GUIController.EventID.EVENT_SHOW_MAIN, null);
			break;
		case MenuEvent.DIY_NEXTSTEP2GREETING:
			menu.toHide();
			controller.go2Greeting();
			break;
		case MenuEvent.DIY_NEXTSTEP2NUMBER:
			menu.toHide();
			controller.go2Number();
			break;
		case MenuEvent.EXIT:
			controller.handleEvent(GUIController.EventID.EVENT_SURE_EXIT, null);
			break;
		}
	}
}
