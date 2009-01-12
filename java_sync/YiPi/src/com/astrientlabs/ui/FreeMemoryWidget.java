/*
 * Copyright (C) 2006 Astrient Labs, LLC
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.ui;

import javax.microedition.lcdui.Graphics;

import com.astrientlabs.text.Strings;

public class FreeMemoryWidget extends LabelWidget {
	private String prefix;

	public FreeMemoryWidget(Window parent, String text) {
		super(parent, text);
		prefix = text;
	}

	public void paint(Graphics g, int x, int y, int maxX, int maxY,
			boolean hasFocus) {
		text = prefix
				+ Strings.getSizeString(Runtime.getRuntime().freeMemory());
		super.paint(g, x, y, maxX, maxY, hasFocus);
	}
}