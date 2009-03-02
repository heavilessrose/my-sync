package com.winksi.j2me.cc.view;

import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.control.GUIController;
import com.winksi.j2me.cc.model.main.MenuEvent;
import com.winksi.j2me.cc.model.main.MenuItemChangeManager;
import com.winksi.j2me.cc.tools.ConstImage;
import com.winksi.j2me.cc.tools.StringTools;
import java.util.Vector;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Graphics;

/**
 * 提示用户第一次使用软件时进行手机相关设置
 * @author jerry
 */
public class SettingCanvas extends Canvas {

	private GUIController controller;
	private Menu mainMenu;
	private MenuItem continueMenuItem, exitMenuItem;
	private MenuItemChangeManager menuItemChangeManager;
	private int keyCode = -100;
	//    private String info = "you must setting!";
	private Vector infoVector;
	private int infoindex = 0;
	private final int maxline = 7;
	private final int showWidthMax = 200;
	private final int showHeightMax = 200;
	private final int info_x = 20;
	private final int info_y = 72;

	private String title = "";

	public SettingCanvas(GUIController controller) {
		setFullScreenMode(true);
		this.controller = controller;

		mainMenu = new Menu(this.controller);
		initMenuItemSetting();
		menuItemChangeManager = new MenuItemChangeManager(this.controller, mainMenu);
	}

	public void initMenuItemSetting() {
		//        int[] itemevent1 = {MenuEvent.BACK, MenuEvent.SETTING_NEXT};
		//        continueMenuItem = new MenuItem(CPProperty.COMMAND_NEXT[controller.getLanguage()], itemevent1, false);
		int[] itemevent2 = { MenuEvent.BACK, MenuEvent.EXIT };
		exitMenuItem = new MenuItem(CPProperty.COMMAND_EXIT[controller.getLanguage()], itemevent2,
				false);
		//        mainMenu.addMenuItem(continueMenuItem);
		mainMenu.addMenuItem(exitMenuItem);
	}

	public void initMenuItemSureSet() {
		int[] itemevent1 = { MenuEvent.BACK, MenuEvent.SURESET_NEXT };
		continueMenuItem = new MenuItem(CPProperty.COMMAND_NEXT[controller.getLanguage()],
				itemevent1, false);
		int[] itemevent2 = { MenuEvent.BACK, MenuEvent.EXIT };
		exitMenuItem = new MenuItem(CPProperty.COMMAND_EXIT[controller.getLanguage()], itemevent2,
				false);
		mainMenu.addMenuItem(continueMenuItem);
		mainMenu.addMenuItem(exitMenuItem);
	}

	/** 初始化免责声明界面 */
	public void initMenuItemDeclare() {
		int[] itemevent1 = { MenuEvent.BACK, MenuEvent.DECLARE_NEXT };
		continueMenuItem = new MenuItem(CPProperty.COMMAND_NEXT[controller.getLanguage()],
				itemevent1, false);
		int[] itemevent2 = { MenuEvent.BACK, MenuEvent.EXIT };
		exitMenuItem = new MenuItem(CPProperty.COMMAND_EXIT[controller.getLanguage()], itemevent2,
				false);
		mainMenu.addMenuItem(continueMenuItem);
		mainMenu.addMenuItem(exitMenuItem);
	}

	public void drawButton(Graphics g) {
		if (mainMenu != null && mainMenu.isVisible()) {
			g.drawString(CPProperty.COMMAND_BACK[controller.getLanguage()], CPProperty.SCREENWIDTH
					- CPProperty.defaultFont.stringWidth(CPProperty.COMMAND_BACK[controller
							.getLanguage()]) - 1,
					CPProperty.SCREENHEIGHT - CPProperty.dfheight - 2, 20);
		} else {
			g.setColor(CPProperty.COLOR_CHAR_WHITE);
			g.drawString(CPProperty.COMMAND_OPTIONS[controller.getLanguage()], 1,
					CPProperty.SCREENHEIGHT - CPProperty.dfheight - 2, 20);
		}
	}

	public void paint(Graphics g) {
		g.setColor(0);
		g.fillRect(0, 0, getWidth(), getHeight());
		g.drawImage(ConstImage.main, 0, 0, 20);

		g.setColor(CPProperty.COLOR_CHAR_WHITE);
		g.drawString(title,
				(CPProperty.SCREENWIDTH - CPProperty.defaultFont.stringWidth(title)) / 2,
				CPProperty.TITLE_OFF_Y, 20);
		//
		//        g.setColor(CPProperty.COLOR_CHAR_WHITE);
		//        g.drawString(title, ((CPProperty.SCREENWIDTH + ConstImage.item[3].getWidth()/2) - CPProperty.defaultFont.stringWidth(title)) >> 1, 0, 20);
		//        
		g.setColor(CPProperty.COLOR_CHAR_WHITE);
		if (!infoVector.isEmpty()) {
			g.setClip(info_x, info_y, showWidthMax, showHeightMax);
			for (int i = 0; i < infoVector.size(); i++) {
				g.drawString((infoVector.elementAt(i).toString()).trim(), info_x, info_y
						+ (CPProperty.dfheight + CPProperty.ROW_SPACE) * (i - infoindex), 20);
			}
			g.setClip(0, 0, CPProperty.SCREENWIDTH, CPProperty.SCREENHEIGHT);
		}

		drawButton(g);

		if (mainMenu != null && mainMenu.isVisible()) {
			mainMenu.drawMenu(g);
		}
	}

	protected void keyPressed(int keycode) {
		keyCode = keycode;

		if (mainMenu.isVisible()) {
			if (mainMenu.isFocus()) {
				mainMenu.process(keyCode);
			}
		} else {
			if (keyCode == CPProperty.KEY_NUM0) {// 0

			} else if (keyCode == CPProperty.KEY_NUM1) {// 1

			} else if (keyCode == CPProperty.KEY_NUM2) {// 2

			} else if (keyCode == CPProperty.KEY_NUM3) {// 3

			} else if (keyCode == CPProperty.KEY_NUM4) {// 4

			} else if (keyCode == CPProperty.KEY_NUM5) {// 5

			} else if (keyCode == CPProperty.KEY_NUM6) {// 6

			} else if (keyCode == CPProperty.KEY_NUM7) {// 7

			} else if (keyCode == CPProperty.KEY_NUM8) {// 8

			} else if (keyCode == CPProperty.KEY_NUM9) {// 9

			} else if (keyCode == CPProperty.KEY_STAR) {// *

			} else if (keyCode == CPProperty.KEY_POUND) {// #

			} else if (keyCode == CPProperty.KEY_SOFT_LEFT) {// left soft key
				if (!mainMenu.isVisible()) {
					controller.handleEvent(GUIController.EventID.EVENT_SETTING_MENU, null);
				}
			} else if (keyCode == CPProperty.KEY_SOFT_RIGHT) {// right soft key

			} else if (keyCode == CPProperty.KEY_BACK) {// back key

			} else if (keyCode == CPProperty.KEY_CLEAR) {// clear key

			} else if (keyCode == CPProperty.KEY_SOUND_UP) {// sound up key

			} else if (keyCode == CPProperty.KEY_SOUND_DWON) {// sound down
				// key

			} else if (keyCode == CPProperty.KEY_UP) {// up
				//                if(infoVector.size()>maxline){
				infoindex--;
				if (infoindex < 0) {
					infoindex = 0;
				}
				//                }
			} else if (keyCode == CPProperty.KEY_DOWN) {// down
				if (infoVector.size() > maxline) {
					infoindex++;
					if (infoindex > infoVector.size() - maxline) {
						infoindex = infoVector.size() - maxline;
					}
				}
			} else if (keyCode == CPProperty.KEY_LEFT) {// left

			} else if (keyCode == CPProperty.KEY_RIGHT) {// right

			} else if (keyCode == CPProperty.KEY_OK) {// ok

			}
		}
		repaint();
	}

	public void showMainMenu() {
		mainMenu.setFocus(true);
		mainMenu.toShow();
	}

	public void removeMenuItem() {
		mainMenu.removeAll();
	}

	public void setInfo(String title, String s) {
		this.title = title;
		infoindex = 0;
		infoVector = new Vector();
		infoVector = StringTools.getSubsection(s, CPProperty.defaultFont, showWidthMax, ".,;?!");
		repaint();
	}
}
