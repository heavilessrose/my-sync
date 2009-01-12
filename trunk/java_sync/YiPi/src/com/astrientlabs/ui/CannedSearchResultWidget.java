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
import javax.microedition.lcdui.Image;

import com.astrientlabs.colors.Colors;
import com.astrientlabs.fonts.Fonts;
import com.astrientlabs.text.Strings;

public class CannedSearchResultWidget extends Widget {
	private int padding = 3;
	private int thumbnailWidth = 30;
	private int maxTextLen;
	private int borderSize = 3;
	private Image image;

	private Object value;

	public CannedSearchResultWidget(Window parent, Image image, String name,
			Object value) {
		super(parent);

		this.name = name;
		this.value = value;
		this.image = image;

		widgetWidth = getWidth();
		widgetHeight = (2 * padding) + (2 * Fonts.TEXT.getHeight())
				+ (2 * padding);

		// TODO: large display vs small;
		maxTextLen = (widgetWidth >= 240) ? 35 : 15;
	}

	public void paint(Graphics g, int x, int y, int maxX, int maxY,
			boolean hasFocus) {
		startY = y;
		startX = x;

		g.setFont(Fonts.TEXT);
		if (hasFocus) {
			// draw bg
			g.setColor(Colors.HIGHTLIGHT_BG);
			g.fillRect(x, y, maxX - x, Math.min(widgetHeight, maxY - y));

			// draw border
			g.setColor(0xFFFFFF);
			g.drawRect(x, y, maxX - x, Math.min(widgetHeight, maxY - y));

			// draw text
			textColor = 0x000000;
		} else {
			g.setColor(Colors.HORIZONTAL_RULE);
			g.drawLine(x, y + Math.min(widgetHeight, maxY - y), maxX - x, y
					+ Math.min(widgetHeight, maxY - y));
			textColor = Colors.HIGHLIGHT_TEXT;
		}

		g.drawImage(image, x + padding, y + (widgetHeight >> 1), Graphics.LEFT
				| Graphics.VCENTER);

		g.setColor(textColor);
		g.drawString(Strings.getText(name, maxTextLen, "..."), x + padding
				+ thumbnailWidth + padding + 1, y + padding + borderSize,
				Graphics.TOP | Graphics.LEFT);
		g.drawString(Strings.getText(value.toString(), maxTextLen, "..."), x
				+ padding + thumbnailWidth + padding + 1, y + borderSize
				+ padding + g.getFont().getHeight() + padding, Graphics.TOP
				| Graphics.LEFT);
	}

	public Object getValue() {
		return value;
	}
}