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

import com.astrientlabs.cache.Cache;

public class CacheSizeWidget extends LabelWidget {
	String prefix;

	public CacheSizeWidget(Window parent, String text) {
		super(parent, text);

		this.prefix = text;
	}

	public void paint(Graphics g, int x, int y, int maxX, int maxY,
			boolean hasFocus) {
		text = prefix + String.valueOf(Cache.instance.size());
		super.paint(g, x, y, maxX, maxY, hasFocus);
	}
}