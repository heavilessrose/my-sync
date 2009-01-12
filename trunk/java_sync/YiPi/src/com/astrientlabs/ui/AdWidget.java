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

import com.astrientlabs.ad.Ad;
import com.astrientlabs.ad.AdCache;
import com.astrientlabs.colors.Colors;
import com.astrientlabs.fonts.Fonts;
import com.astrientlabs.log.Logger;

public class AdWidget extends Widget implements Runnable {
	private Ad ad;
	private int tick;
	private int cycle = 100;
	private long lastCycle = 0;
	private int padding = 2;
	private long adChangeTs = 0;
	private String message;

	private boolean running = false;

	public AdWidget(Window parent, int height, String message) {
		super(parent);

		this.message = message;
		widgetHeight = height;
		widgetWidth = getWidth();
	}

	public void setMessage(String message) {
		this.message = message;
	}

	public void paint(Graphics g, int x, int y, int maxX, int maxY,
			boolean hasFocus) {
		startY = y;

		g.setColor(0);
		g.fillRect(x, y, widgetWidth, widgetHeight);

		if (ad != null) {
			g.drawImage(ad.getImage(), x + (getWidth() >> 1), y
					+ (widgetHeight >> 1), Graphics.VCENTER | Graphics.HCENTER);
		}

		if (message != null && ((tick & 4) == 0)) {
			g.setColor(Colors.TEXT);
		} else {
			g.setColor(Colors.HIGHLIGHT_TEXT);
		}

		g.drawString(message, x + (widgetWidth >> 1), y + widgetHeight
				- padding, Graphics.HCENTER | Graphics.BOTTOM);
	}

	public void run() {
		if (running)
			return;

		try {
			running = true;
			// must keep running
			if (ad == null
					|| (adChangeTs + ad.getDisplayTime() < System
							.currentTimeMillis())) {
				ad = AdCache.instance.next(this.widgetWidth, this.widgetHeight
						- Fonts.TEXT.getHeight() - (2 * padding));
				adChangeTs = System.currentTimeMillis();
				parent.repaintChild(this, false);
			}

			if ((lastCycle + cycle) < System.currentTimeMillis()) {
				tick++;
				lastCycle = System.currentTimeMillis();
				parent.repaintChild(this, true);
			}
		} catch (Exception e) {
			Logger.instance.log("adWidget.run()", e);
		} finally {
			running = false;
		}
	}
}