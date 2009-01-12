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

public class ThreadActiveCountWidget extends LabelWidget {
	private String prefix;

	public ThreadActiveCountWidget(Window parent, String text) {
		super(parent, text);
		prefix = text;
	}

	public void paint(Graphics g, int x, int y, int maxX, int maxY,
			boolean hasFocus) {
		text = prefix + Thread.activeCount();
		super.paint(g, x, y, maxX, maxY, hasFocus);
	}
}