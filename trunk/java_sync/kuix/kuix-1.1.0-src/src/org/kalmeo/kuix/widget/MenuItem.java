package org.kalmeo.kuix.widget;

import org.kalmeo.kuix.core.KuixConstants;
import org.kalmeo.kuix.widget.Menu.MenuPopup;

/**
 * This class represents a menu item. <br>
 * <br>
 * <strong>For further informations, visit the <a
 * href="http://www.kalmeo.org/files/kuix/widgetdoc/index.html"
 * target="new">Kuix widgets reference page</a></strong>.
 * 
 * @author bbeaulant
 */
public class MenuItem extends ListItem {

	/**
	 * Construct a {@link MenuItem}
	 */
	public MenuItem() {
		this(KuixConstants.MENU_ITEM_WIDGET_TAG);
	}

	/**
	 * Construct a {@link MenuItem}
	 * 
	 * @param tag
	 */
	public MenuItem(String tag) {
		super(tag);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.kalmeo.kuix.widget.AbstractActionWidget#processActionEvent()
	 */
	public boolean processActionEvent() {

		// Keep the parent instance
		Widget tmpParent = parent;

		// Process the onAction event
		super.processActionEvent();

		// Caution the action is processed before hiding the menu tree !
		if (tmpParent != null && tmpParent instanceof MenuPopup) {
			((MenuPopup) tmpParent).getMenu().hideMenuTree();
		}

		return true;
	}

}
