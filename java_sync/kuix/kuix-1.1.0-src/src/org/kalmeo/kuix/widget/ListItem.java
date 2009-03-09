package org.kalmeo.kuix.widget;

import org.kalmeo.kuix.core.KuixConstants;
import org.kalmeo.kuix.layout.InlineLayout;
import org.kalmeo.kuix.layout.Layout;
import org.kalmeo.kuix.util.Alignment;

/**
 * This class represents a list item. <br>
 * <br>
 * <strong>For further informations, visit the <a
 * href="http://www.kalmeo.org/files/kuix/widgetdoc/index.html"
 * target="new">Kuix widgets reference page</a></strong>.
 * 
 * @author bbeaulant
 */
public class ListItem extends ActionWidget {

	// Defaults
	private static final Layout LIST_ITEM_DEFAULT_LAYOUT = new InlineLayout(
			true, Alignment.FILL);

	/**
	 * Construct a {@link ListItem}
	 */
	public ListItem() {
		this(KuixConstants.LIST_ITEM_WIDGET_TAG);
	}

	/**
	 * Construct a {@link ListItem}
	 * 
	 * @param tag
	 */
	public ListItem(String tag) {
		super(tag);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * org.kalmeo.kuix.widget.Widget#getDefaultStyleAttributeValue(java.lang
	 * .String)
	 */
	protected Object getDefaultStylePropertyValue(String name) {
		if (KuixConstants.LAYOUT_STYLE_PROPERTY.equals(name)) {
			return LIST_ITEM_DEFAULT_LAYOUT;
		}
		return super.getDefaultStylePropertyValue(name);
	}

}
