package org.kalmeo.apps.kaprice;

import org.kalmeo.apps.kaprice.core.frame.MainFrame;
import org.kalmeo.kuix.core.Kuix;
import org.kalmeo.kuix.core.KuixMIDlet;
import org.kalmeo.kuix.widget.Desktop;

/**
 * @author omarino
 */
public class Kaprice extends KuixMIDlet {

	/**
	 * (non-Javadoc)
	 * 
	 * @see org.kalmeo.kuix.core.KuixMIDlet#initDesktopStyles()
	 */
	public void initDesktopStyles() {
		Kuix.loadCss("/css/style.css");
	}

	/**
	 * (non-Javadoc)
	 * 
	 * @see
	 * org.kalmeo.kuix.core.KuixMIDlet#initDesktopContent(org.kalmeo.kuix.widget
	 * .Desktop)
	 */
	public void initDesktopContent(Desktop desktop) {
		Kuix.getFrameHandler().pushFrame(MainFrame.instance);
	}

}
