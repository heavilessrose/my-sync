/*
 * This file is part of org.kalmeo.apps.kaprice.
 * 
 * org.kalmeo.apps.kaprice is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * org.kalmeo.apps.kaprice is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with org.kalmeo.apps.kaprice.  If not, see <http://www.gnu.org/licenses/>.
 *  
 * Creation date : 12 mars 08
 * Copyright (c) Kalmeo 2007-2008. All rights reserved.
 */

package org.kalmeo.apps.kaprice;

import org.kalmeo.apps.kaprice.core.frame.MainFrame;
import org.kalmeo.kuix.core.Kuix;
import org.kalmeo.kuix.core.KuixMIDlet;
import org.kalmeo.kuix.widget.Desktop;

/**
 * @author omarino
 */
public class Kaprice extends KuixMIDlet {

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
		Kuix.getFrameHandler().pushFrame(MainFrame.instance);
	}

}
