package org.kalmeo.kuix.widget;

import org.kalmeo.kuix.core.KuixConstants;
import org.kalmeo.kuix.layout.InlineLayout;
import org.kalmeo.kuix.layout.Layout;
import org.kalmeo.kuix.util.Alignment;

/**
 * This class represents a button. <br>
 * <br>
 * <strong>For further informations, visit the <a
 * href="http://www.kalmeo.org/files/kuix/widgetdoc/index.html"
 * target="new">Kuix widgets reference page</a></strong>.
 * 
 * @author bbeaulant
 */
public class Button extends ActionWidget {

	// Defaults
	private static final Layout BUTTON_DEFAULT_LAYOUT = new InlineLayout(true,
			Alignment.CENTER);

	/**
	 * Construct a {@link Button}
	 */
	public Button() {
		super(KuixConstants.BUTTON_WIDGET_TAG);
	}

	/**
	 * Construct a {@link Button}
	 * 
	 * @param tag
	 */
	public Button(String tag) {
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
			return BUTTON_DEFAULT_LAYOUT;
		}
		if (KuixConstants.ALIGN_STYLE_PROPERTY.equals(name)) {
			return Alignment.CENTER;
		}
		return super.getDefaultStylePropertyValue(name);
	}

}
