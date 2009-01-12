/*
 * Copyright (C) 2006 Astrient Labs, LLC Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance 
 * with the License. You may obtain a copy of the License at 
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the 
 * License for the specific language governing permissions and limitations under
 * the License.
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.text;

import java.util.Enumeration;
import java.util.Vector;

import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;

public class StringVector extends Vector {
	public StringVector(Font font, String text, int width) {
		LineEnumeration le = new LineEnumeration(font, text, width);
		while (le.hasMoreElements()) {
			addElement(le.nextElement().toString().trim());
		}
	}

	public int writeTo(Graphics g, int startx, int starty, int maxY, Font font) {
		int fontHeight = font.getHeight() + 1;

		for (Enumeration e = elements(); e.hasMoreElements() && starty < maxY;) {
			g.drawString(e.nextElement().toString(), startx, starty,
					Graphics.TOP | Graphics.LEFT);
			starty += fontHeight;
		}

		return starty;
	}
}