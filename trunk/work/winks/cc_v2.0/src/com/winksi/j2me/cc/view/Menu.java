package com.winksi.j2me.cc.view;

import java.util.*;

import javax.microedition.lcdui.Graphics;

import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.tools.ConstImage;
import com.winksi.j2me.cc.control.GUIController;

/** 菜单 */
public class Menu {

	private int stateNum;
	private int menuState;
	private int menuHeight;
	private int menuWidth;
	private int menu_x;
	private int menu_y;
	private int moveHeight;
	private int moveWidth;
	private boolean menuVisible;
	private boolean focus;
	private Vector menuItems = new Vector();
	private MenuItemListener menuItemListener;
	private GUIController controller;
	private static final int MAINEVENT = 0;
	private static final int SUBEVENT = 1;

	public Menu(GUIController controller) {
		this.controller = controller;
		init();
	}

	public void init() {
		stateNum = 0;
		menuState = 0;
		menuHeight = 0;
		menuWidth = 0;
		menu_x = 0;
		menu_y = 0;
		moveHeight = 0;
		moveWidth = 0;
		menuVisible = false;
		focus = false;
		menuItems = null;
		menuItems = new Vector();
	}

	public void addMenuItem(MenuItem item) {
		menuItems.addElement(item);
		stateNum = menuItems.size();
		int x = (CPProperty.defaultFont.stringWidth(item.toString()) + 2 * CPProperty.ROW_SPACE + ConstImage.tri
				.getWidth());
		if (x > this.menuWidth) {
			this.menuWidth = x;
		}
		this.menuHeight = CPProperty.COL_SPACE + menuItems.size()
				* (CPProperty.dfheight + CPProperty.COL_SPACE);

		this.menu_y = CPProperty.SCREENHEIGHT - this.menuHeight;
	}

	public void removeAll() {
		menuItems.removeAllElements();
		init();
	}

	public void resetPosition(int x, int y) {
		this.menu_x = x;
		this.menu_y = y;
	}

	public void resetSize(int w, int h) {
		this.menuWidth = w;
		this.menuHeight = h;
	}

	public int getMenuState() {
		return this.menuState;
	}

	public boolean isVisible() {
		return this.menuVisible;
	}

	public int getWidth() {
		return this.menuWidth;
	}

	public int getHeight() {
		return this.menuHeight;
	}

	public int getItemPosition() {
		return this.menu_y + (this.menuState + 1) * (CPProperty.dfheight + CPProperty.COL_SPACE);
	}

	public void toShow() {
		menuVisible = true;
		focus = true;
	}

	public void toHide() {
		menuVisible = false;
		focus = false;
	}

	public void setFocus(boolean b) {
		this.focus = b;
	}

	public boolean isFocus() {
		return this.focus;
	}

	public void toDown() {
		menuState++;
		if (menuState > stateNum - 1) {
			menuState = 0;
		}
	}

	public void toUp() {
		menuState--;
		if (menuState < 0) {
			menuState = stateNum - 1;
		}
	}

	public void setMenuItemListener(MenuItemListener menuItemListener) {
		this.menuItemListener = menuItemListener;
	}

	public void process(int keyCode) {
		if (keyCode == CPProperty.KEY_UP) {
			toUp();
			System.out.println("menuState: " + menuState);
		} else if (keyCode == CPProperty.KEY_DOWN) {
			toDown();
			System.out.println("menuState: " + menuState);
		} else if (keyCode == CPProperty.KEY_LEFT) {
			this.menuItemListener.itemAction(((MenuItem) menuItems.elementAt(menuState))
					.getEvent(MAINEVENT));
		} else if (keyCode == CPProperty.KEY_RIGHT) {
			this.menuItemListener.itemAction(((MenuItem) menuItems.elementAt(menuState))
					.getEvent(SUBEVENT));
		} else if (keyCode == CPProperty.KEY_OK || keyCode == CPProperty.KEY_SOFT_LEFT) {
			this.menuItemListener.itemAction(((MenuItem) menuItems.elementAt(menuState))
					.getEvent(SUBEVENT));
		} else if (keyCode == CPProperty.KEY_SOFT_RIGHT || keyCode == CPProperty.KEY_CLEAR
				|| keyCode == CPProperty.KEY_BACK) {
			this.menuItemListener.itemAction(((MenuItem) menuItems.elementAt(menuState))
					.getEvent(MAINEVENT));
		}

	}

	public void drawMenu(Graphics g) {
		if (!menuItems.isEmpty()) {
			g.setColor(CPProperty.COLOR_MENU_FRAME);
			g.drawRect(menu_x + moveWidth, menu_y + moveHeight, menuWidth, menuHeight);
			g.setColor(CPProperty.COLOR_MENU_BG);
			g.fillRect(menu_x + 1, menu_y + 1 + moveHeight, menuWidth - 1, menuHeight - 1);

			for (int i = 0; i < stateNum; i++) {
				if (menuState == i) {
					g.setColor(CPProperty.COLOR_MENU_CHAR_SELECT);
					g.fillRect(menu_x + 1, menu_y + (CPProperty.COL_SPACE + CPProperty.dfheight)
							* i + CPProperty.COL_SPACE + moveHeight - 2, menuWidth - 1,
							CPProperty.dfheight + 4);
				} else {
					//                    g.setColor(CPProperty.COLOR_CHAR_WHITE);
				}
				g.setColor(CPProperty.COLOR_CHAR_WHITE);
				g.drawString(menuItems.elementAt(i).toString(), menu_x + CPProperty.ROW_SPACE,
						menu_y + (CPProperty.COL_SPACE + CPProperty.dfheight) * i
								+ CPProperty.COL_SPACE + moveHeight, 20);
				if (((MenuItem) menuItems.elementAt(i)).hasSubMenu()) {
					g.drawImage(ConstImage.tri, menu_x + CPProperty.ROW_SPACE
							+ CPProperty.defaultFont.stringWidth(menuItems.elementAt(i).toString())
							+ 1, menu_y + (CPProperty.COL_SPACE + CPProperty.dfheight) * i
							+ CPProperty.COL_SPACE + moveHeight, 20);
				}
			}
		}
	}
}
