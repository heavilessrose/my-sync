/*
 * Copyright (C) 2007-2008 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

package org.geometerplus.zlibrary.core.view;

import java.util.*;
import org.geometerplus.zlibrary.core.util.*;

import org.geometerplus.zlibrary.core.image.ZLImageData;

abstract public class ZLPaintContext {
	private int myX = 0;
	private int myY = 0;

	private final ArrayList myFamilies = new ArrayList();

	public interface LineStyle {
		int SOLID_LINE = 0;
		int DASH_LINE = 1;
	};

	public interface FillStyle {
		int SOLID_FILL = 0;
		int HALF_FILL = 1;
	};
	
	protected ZLPaintContext() {
	}

	abstract public void clear(ZLColor color);

	private boolean myResetFont = true;
	private String myFontFamily = "";
	private int myFontSize;
	private boolean myFontIsBold;
	private boolean myFontIsItalic;

	public final int getX() {
		return myX;
	}
	
	public final int getY() {
		return myY;
	}

	public final void moveXTo(int x) {
		myX = x;
	}
	
	public final void moveX(int deltaX) {
		myX += deltaX;
	}

	public final void moveYTo(int y) {
		myY = y;
	}
	
	public final void moveY(int deltaY) {
		myY += deltaY;
	}

	public final void resetFont() {
		myResetFont = true;
	}

	public final void setFont(String family, int size, boolean bold, boolean italic) {
		if ((family != null) && !myFontFamily.equals(family)) {
			myFontFamily = family;
			myResetFont = true;
		}
		if (myFontSize != size) {
			myFontSize = size;
			myResetFont = true;
		}
		if (myFontIsBold != bold) {
			myFontIsBold = bold;
			myResetFont = true;
		}
		if (myFontIsItalic != italic) {
			myFontIsItalic = italic;
			myResetFont = true;
		}
		if (myResetFont) {
			myResetFont = false;
			setFontInternal(myFontFamily, myFontSize, myFontIsBold, myFontIsItalic);
			mySpaceWidth = -1;
			myStringHeight = -1;
			myDescent = -1;
		}
	}

	abstract protected void setFontInternal(String family, int size, boolean bold, boolean italic);

	final public void setColor(ZLColor color) {
		setColor(color, LineStyle.SOLID_LINE);
	}
	abstract public void setColor(ZLColor color, int style);

	final public void setFillColor(ZLColor color) {
		setFillColor(color, FillStyle.SOLID_FILL);
	}
	abstract public void setFillColor(ZLColor color, int style);

	abstract public int getWidth();
	abstract public int getHeight();
	
	abstract public int getStringWidth(char[] string, int offset, int length);

	private int mySpaceWidth = -1;
	public final int getSpaceWidth() {
		int spaceWidth = mySpaceWidth;
		if (spaceWidth == -1) {
			spaceWidth = getSpaceWidthInternal();
			mySpaceWidth = spaceWidth;
		}
		return spaceWidth;
	}
	abstract protected int getSpaceWidthInternal();

	private int myStringHeight = -1;
	public final int getStringHeight() {
		int stringHeight = myStringHeight;
		if (stringHeight == -1) {
			stringHeight = getStringHeightInternal();
			myStringHeight = stringHeight;
		}
		return stringHeight;
	}
	abstract protected int getStringHeightInternal();

	private int myDescent = -1;
	public final int getDescent() {
		int descent = myDescent;
		if (descent == -1) {
			descent = getDescentInternal();
			myDescent = descent;
		}
		return descent;
	}
	abstract protected int getDescentInternal();

	abstract public void drawString(int x, int y, char[] string, int offset, int length);

	abstract public int imageWidth(ZLImageData image);
	abstract public int imageHeight(ZLImageData image);
	abstract public void drawImage(int x, int y, ZLImageData image);

	abstract public void drawLine(int x0, int y0, int x1, int y1);
	abstract public void fillRectangle(int x0, int y0, int x1, int y1);
	abstract public void drawFilledCircle(int x, int y, int r);

	public ArrayList fontFamilies() {
		if (myFamilies.isEmpty()) {
			fillFamiliesList(myFamilies);
		}
		return myFamilies;
	}	
	
	abstract public String realFontFamilyName(String fontFamily);
	abstract protected void fillFamiliesList(ArrayList families);
}
