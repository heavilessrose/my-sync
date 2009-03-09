package org.kalmeo.kuix.widget;

import javax.microedition.lcdui.Graphics;

import org.kalmeo.kuix.core.Kuix;

/**
 * This widget is used to mask region of display from Kuix repaint process.<br>
 * Basicly it could be used to implement a video render region.
 * 
 * @author bbeaulant
 */
public class UnpaintableWidget extends Widget {

	/**
	 * Construct a {@link UnpaintableWidget}
	 * 
	 * @param tag
	 */
	public UnpaintableWidget(String tag) {
		super(tag);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * org.kalmeo.kuix.widget.Widget#paintImpl(javax.microedition.lcdui.Graphics
	 * )
	 */
	public void paintImpl(Graphics g) {
		Kuix.getCanvas().addUnpaintableWidget(this);

		// Ignore children painting

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.kalmeo.kuix.widget.Widget#cleanUp()
	 */
	public void cleanUp() {
		super.cleanUp();

		// Remove this widget from the KuixCanvas unpaintable widget list
		Kuix.getCanvas().removeUnpaintableWidget(this);

	}

}
