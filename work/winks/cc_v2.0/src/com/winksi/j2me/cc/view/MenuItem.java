package com.winksi.j2me.cc.view;

/** 菜单项 */
public class MenuItem {

	/** 菜单项的两个事件,在构造这个菜单项时会初始化这个数组 */
	private int[] MenuItemEvent = new int[2];
	private String MenuItemTitle;
	private boolean hasSubMenu;
	private int offsetX, offsetWidth;

	public MenuItem(String title, int[] id, boolean hassubmenu) {
		MenuItemTitle = title;
		this.MenuItemEvent = id;
		this.hasSubMenu = hassubmenu;
	}

	public boolean hasSubMenu() {
		return this.hasSubMenu;
	}

	public void setSubMenu(boolean flag) {
		this.hasSubMenu = flag;
	}

	public int getEvent(int i) {
		return this.MenuItemEvent[i];
	}

	public String toString() {
		return this.MenuItemTitle;
	}
}
