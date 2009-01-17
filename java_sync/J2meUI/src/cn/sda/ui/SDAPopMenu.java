package cn.sda.ui;

import javax.microedition.lcdui.*;

import cn.sda.event.*;

/**
 * <p>
 * Title:
 * </p>
 * 
 * <p>
 * Description:
 * </p>
 * 
 * <p>
 * Copyright: Copyright (c) 2008
 * </p>
 * 
 * <p>
 * Company:
 * </p>
 * 
 * @author not attributable
 * @version 1.0
 */
public class SDAPopMenu extends SDABaseControl {

	private SDAMenuItem rootItem = new SDAMenuItem("rootMenu", "");
	private SDAMenuItem focusedItem;
	private SDAMenuItem operateItem = null;
	private PopMenuOnClickEvent onMenuClick = null;

	// private int popDirection;
	public SDAPopMenu() {
		visible = false;
		this.rootItem.popMenu = this;
		this.focusedItem = this.rootItem;
		this.rootItem.isSubShow = true;
		tabStop = false;
		this.setOnPointerPressed(new PointerEvent() {

			public void Event(SDABaseControl ctrl, int x, int y) {
				operateItem = findMenuItem(focusedItem, x, y);

				if (operateItem != null) {
					operateItem.parentItem.focusedIndex = operateItem.itemIndex;
					paint();
				}
			}
		});
		this.setOnPointerReleased(new PointerEvent() {

			public void Event(SDABaseControl ctrl, int x, int y) {
				if (operateItem != null) {
					if (operateItem.equals(findMenuItem(focusedItem, x, y))) {
						if (!operateItem.subItems.isEmpty()) {
							operateItem.isSubShow = !operateItem.isSubShow;
							if (operateItem.isSubShow) {
								focusedItem = operateItem;
								focusedItem.focusedIndex = 0;
								paint();
							} else {
								closeMenuItem(focusedItem);
								focusedItem = operateItem.parentItem;
								form.paint();
							}
						} else {
							ctrl.visible = false;
							form.closePopCtrl();
							form.paint();
							if (onMenuClick != null) {
								onMenuClick.Event(operateItem,
										operateItem.itemName,
										operateItem.itemCaption);
							}
						}
					}
					return;
				}
				ctrl.visible = false;
				form.closePopCtrl();
				form.paint();
			}
		});
		this.setOnKeyDown(new KeybordEvent() {

			public void Event(SDABaseControl ctrl, int keyCode) {
				String keyName = form.getKeyName(keyCode);
				SDAMenuItem item = null;
				if (keyName.equals(SDAConsts.KEY_DOWN)) {
					if (focusedItem.focusedIndex < focusedItem.subItems.size() - 1) {
						while (item == null
								&& focusedItem.focusedIndex < focusedItem.subItems
										.size() - 1) {
							focusedItem.focusedIndex++;
							item = focusedItem
									.getMenuItem(focusedItem.focusedIndex);
							if (item != null && !item.itemCaption.equals("-")) {
								paint();
								break;
							} else {
								item = null;
							}
						}
					} else {
						focusedItem.focusedIndex = 0;
						item = focusedItem
								.getMenuItem(focusedItem.focusedIndex);
						if (item != null && !item.itemCaption.equals("-")) {
							paint();
						}
					}
				} else if (keyName.equals(SDAConsts.KEY_UP)) {
					if (focusedItem.focusedIndex > 0) {
						while (item == null && focusedItem.focusedIndex > 0) {
							focusedItem.focusedIndex--;
							item = focusedItem
									.getMenuItem(focusedItem.focusedIndex);
							if (item != null && !item.itemCaption.equals("-")) {
								paint();
								break;
							} else {
								item = null;
							}
						}
					} else {
						focusedItem.focusedIndex = focusedItem.subItems.size() - 1;
						item = focusedItem
								.getMenuItem(focusedItem.focusedIndex);
						if (item != null && !item.itemCaption.equals("-")) {
							paint();
						}
					}
				} else if (keyName.equals(SDAConsts.KEY_RIGHT)) {
					if (focusedItem.focusedIndex > -1) {
						item = ((SDAMenuItem) focusedItem.subItems
								.elementAt(focusedItem.focusedIndex));
						if (!item.subItems.isEmpty()) {
							focusedItem = item;
							focusedItem.focusedIndex = 0;
							focusedItem.isSubShow = true;
						}
						paint();
					}
				} else if (keyName.equals(SDAConsts.KEY_LEFT)) {
					if (!rootItem.equals(focusedItem)) {
						focusedItem.isSubShow = false;
						focusedItem = focusedItem.parentItem;
						form.paint();
					}
				} else if (keyName.equals(SDAConsts.KEY_ENTER)
						|| keyName.equals(SDAConsts.KEY_SELECT)) {
					if (onMenuClick != null) {
						item = (SDAMenuItem) focusedItem.subItems
								.elementAt(focusedItem.focusedIndex);
						if (!item.subItems.isEmpty()) {
							item.isSubShow = !item.isSubShow;
							if (item.isSubShow) {
								focusedItem = item;
								paint();
							} else {
								focusedItem = item.parentItem;
								form.paint();
							}
						} else {
							ctrl.visible = false;
							form.closePopCtrl();
							form.setInputAbort(true);
							form.paint();
							onMenuClick.Event(item, item.itemName,
									item.itemCaption);
						}
					} else {
						ctrl.visible = false;
						form.closePopCtrl();
						form.paint();
					}
				}
				// form.serviceRepaints();
			}
		});
	}

	private SDAMenuItem findMenuItem(SDAMenuItem startItem, int x, int y) {
		for (int i = 0; i < startItem.subItems.size(); i++) {
			SDAMenuItem item = (SDAMenuItem) startItem.subItems.elementAt(i);
			if (item.InScreenRect(x, y)) {
				if (!item.itemCaption.equals("-")) {
					return item;
				} else {
					return null;
				}
			}
		}
		if (!rootItem.equals(startItem)) {
			return findMenuItem(startItem.parentItem, x, y);
		}
		return null;
	}

	public SDAMenuItem getMenuItem(SDAMenuItem menuItem, String itemName) {
		for (int i = 0; i < menuItem.subItems.size(); i++) {
			SDAMenuItem item = (SDAMenuItem) menuItem.subItems.elementAt(i);
			if (item.itemName.equals(itemName)) {
				return item;
			}
		}
		return null;
	}

	public SDAMenuItem getMenuItem(SDAPopMenu menu, String itemName) {
		for (int i = 0; i < menu.rootItem.subItems.size(); i++) {
			SDAMenuItem item = (SDAMenuItem) menu.rootItem.subItems
					.elementAt(i);
			if (item.itemName.equals(itemName)) {
				return item;
			}
		}
		return null;
	}

	private void closeMenuItem(SDAMenuItem startItem) {
		if (!startItem.equals(rootItem)) {
			startItem.isSubShow = false;
		}
		for (int i = 0; i < startItem.subItems.size(); i++) {
			SDAMenuItem item = (SDAMenuItem) startItem.subItems.elementAt(i);
			if (!item.subItems.isEmpty()) {
				item.focusedIndex = 0;
				closeMenuItem(item);
			}
		}
	}

	private int getItemHeight(SDAMenuItem item) {
		if (!item.itemCaption.equals("-")) {
			return getFont().getHeight();
		} else {
			return getFont().getHeight() / 2;
		}
	}

	private void DrawPopMenuBox(SDAMenuItem menuItem) {
		Graphics g = this.form.getGraphics();
		// form.ConsoleInfo("width=" + menuItem.width + ",height=" +
		// menuItem.height);
		menuItem.SetClientClip(g, 0, 0, menuItem.clientWidth,
				menuItem.clientHeight);
		if (this.form.ctl3d) {
			g.setColor(SDAConsts.clBtnShadow);
			menuItem.drawClientRect(g, 0, 0, menuItem.clientWidth - 1,
					menuItem.clientHeight - 1);
			g.setColor(getBackColor());
			menuItem.fillClientRect(g, 0, 0, menuItem.clientWidth - 1,
					menuItem.clientHeight - 1);
		} else {
			g.setColor(SDAConsts.clBlack);
			menuItem.drawClientRect(g, 0, 0, menuItem.clientWidth - 1,
					menuItem.clientHeight - 1);
			g.setColor(getBackColor());
			menuItem.fillClientRect(g, 1, 1, menuItem.clientWidth - 2,
					menuItem.clientHeight - 2);
		}
		g.setColor(SDAConsts.clBlack);
		int otop = 0;
		for (int i = 0; i < menuItem.subItems.size(); i++) {
			SDAMenuItem item = (SDAMenuItem) menuItem.subItems.elementAt(i);
			item.originLeft = menuItem.clientOriginLeft;
			item.originTop = menuItem.clientOriginTop + otop;
			otop += getItemHeight(item);
			item.height = getItemHeight(item);
			item.width = menuItem.clientWidth;
			item.drawItem(g, menuItem.focusedIndex == i);
		}
		for (int i = 0; i < menuItem.subItems.size(); i++) {
			SDAMenuItem item = (SDAMenuItem) menuItem.subItems.elementAt(i);
			if (!item.subItems.isEmpty() && item.isSubShow) {
				item.calcClientHeight();
				item.calcClientWidth();

				item.clientOriginTop = item.originTop;
				if (item.clientOriginTop + item.clientHeight > form.canvas
						.getHeight()) {
					item.clientOriginTop = form.canvas.getHeight()
							- item.clientHeight - 2;
					if (item.clientOriginTop < 0) {
						item.clientOriginTop = 0;
					}
				}

				item.clientOriginLeft = menuItem.clientOriginLeft
						+ menuItem.clientWidth;
				if (item.clientOriginLeft + item.clientWidth > form.canvas
						.getWidth()) {
					item.clientOriginLeft = menuItem.clientOriginLeft + 20;
					if (item.clientOriginLeft + item.clientWidth > form.canvas
							.getWidth()) {
						item.clientOriginLeft = 0;
					}
					if (item.clientOriginTop == menuItem.clientOriginTop) {
						item.clientOriginTop += getItemHeight(item);
					}
				}

				DrawPopMenuBox(item);
			}
		}
	}

	public final SDAMenuItem AddItem(String itemName, String itemCaption) {
		SDAMenuItem item = new SDAMenuItem(itemName, itemCaption);
		item.popMenu = this;
		item.parentItem = null;
		rootItem.AddItem(item);
		return item;
	}

	public final SDAMenuItem AddItem(SDAMenuItem parentItem, String itemName,
			String itemCaption) {
		return parentItem.AddItem(itemName, itemCaption);
	}

	public final void paint() {
		if (!this.visible) {
			return;
		}
		this.rootItem.calcClientHeight();
		this.rootItem.calcClientWidth();
		if (this.rootItem.clientOriginLeft + this.rootItem.clientWidth > form.canvas
				.getWidth()) {
			this.rootItem.clientOriginLeft = this.rootItem.clientOriginLeft
					- this.rootItem.clientWidth;
			if (this.rootItem.clientOriginLeft < 0) {
				this.rootItem.clientOriginLeft = 0;
			}
		}
		this.rootItem.clientOriginTop = this.rootItem.originTop;
		if (this.rootItem.clientOriginTop + this.rootItem.clientHeight > form.canvas
				.getHeight()) {
			this.rootItem.clientOriginTop = this.rootItem.originTop
					- this.rootItem.clientHeight + 1;
			if (this.rootItem.clientOriginTop < 0) {
				this.rootItem.clientOriginTop = 0;
			}
		}
		DrawPopMenuBox(rootItem);
	}

	public final void PopMenu(int x, int y) {
		this.setOriginLeft(0);
		this.setOriginTop(0);
		this.width = this.form.canvas.getWidth();
		this.height = this.form.canvas.getHeight();
		this.rootItem.originLeft = x;
		this.rootItem.originTop = y;
		this.rootItem.clientOriginLeft = x;
		this.rootItem.clientOriginTop = y;
		this.focusedItem = this.rootItem;
		this.visible = true;
		this.setPopVisible();
		closeMenuItem(rootItem);
		this.rootItem.focusedIndex = 0;
		// this.setFoucsed();
		this.form.setInputAbort(true);
		repaintControl();
	}

	public void setOnClick(PopMenuOnClickEvent onClick) {
		this.onMenuClick = onClick;
	}

	public final int getClientWidth() {
		rootItem.calcClientHeight();
		rootItem.calcClientWidth();
		return this.rootItem.clientWidth;
	}

	public final int getClientHeight() {
		rootItem.calcClientHeight();
		rootItem.calcClientWidth();
		return this.rootItem.clientHeight;
	}

	protected boolean canLeftTabPrior() {
		return !this.isPopVisible();
	}

	protected boolean canDownTabNext() {
		return !this.isPopVisible();
	}

	protected boolean canUpTabPrior() {
		return !this.isPopVisible();
	}

	protected boolean canRightTabNext() {
		return !this.isPopVisible();
	}
}
