package org.kalmeo.demo.kuix;

import org.kalmeo.demo.kuix.frames.KuixDemoMainFrame;
import org.kalmeo.kuix.core.Kuix;
import org.kalmeo.kuix.core.KuixMIDlet;
import org.kalmeo.kuix.widget.Desktop;

/**
 * @author omarino
 */
public class KuixDemo extends KuixMIDlet {

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.kalmeo.kuix.core.KuixMIDlet#initDesktopStyles()
	 */
	public void initDesktopStyles() {
		Kuix.loadCss("/css/style.css");
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * org.kalmeo.kuix.core.KuixMIDlet#initDesktopContent(org.kalmeo.kuix.widget
	 * .Desktop)
	 */
	public void initDesktopContent(Desktop desktop) {
		// Push the MainMenuFrame
		Kuix.getFrameHandler().pushFrame(KuixDemoMainFrame.instance);
	}

}
