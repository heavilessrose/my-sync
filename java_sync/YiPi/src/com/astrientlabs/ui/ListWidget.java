/*
 * Copyright (C) 2006 Astrient Labs, LLC
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.ui;

import java.util.Vector;

import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Graphics;

import com.astrientlabs.colors.Colors;

public class ListWidget extends Widget {
	protected Vector model = new Vector();

	protected boolean positionChanged = true;
	protected int position;
	protected int windowStart = 0;

	protected int padding = 2;
	protected int scrollbarWidth = 2;

	protected Widget widget;
	protected boolean modal = true;

	public ListWidget(Window parent, int widgetHeight, int bgColor) {
		super(parent);
		this.widgetWidth = getWidth();
		this.widgetHeight = widgetHeight;
		this.bgColor = bgColor;
	}

	public boolean contains(Widget widget) {
		return model.contains(widget);
	}

	public void add(Widget widget) {
		model.addElement(widget);
		parent.repaintChild(this, false);
	}

	public void add(Widget widget, int index) {
		model.insertElementAt(widget, index);
		parent.repaintChild(this, false);
	}

	public void setSize(int size) {
		// TODO: check doc
		if (model.size() > size)
			model.setSize(size);
	}

	public void remove(Widget widget) {
		model.removeElement(widget);
		parent.repaintChild(this, true);
	}

	public void clear() {
		model.removeAllElements();
		reset();
	}

	public void reset() {
		windowStart = 0;
		position = 0;
		parent.repaintChild(this, true);
	}

	public void previous() {
		if (position > 0) {
			position--;
			positionChanged = true;
			parent.repaintChild(this, true);
		} else if (modal) {
			parent.moveFocus(false);
		}
	}

	public void next() {
		if (position < model.size() - 1) {
			position++;
			positionChanged = true;
			parent.repaintChild(this, true);
		} else if (modal) {
			parent.moveFocus(true);
		}
	}

	public void keyPressed(int keyCode) {
		int action = parent.getGameAction(keyCode);

		switch (action) {
		case Canvas.UP:
			previous();
			break;
		case Canvas.DOWN:
			next();
			break;
		default:
			widgetEvent(keyCode);
		}
	}

	public void keyRepeated(int keyCode) {
		keyPressed(keyCode);
	}

	public int size() {
		return model.size();
	}

	public void widgetEvent(int keyCode) {
		if (position < model.size()) {
			Widget widget = (Widget) model.elementAt(position);
			widget.keyPressed(keyCode);
		} else
			super.keyPressed(keyCode);
	}

	public void determineWindowStart() {
		if (positionChanged) {
			positionChanged = false;
			int positionEndY;
			int postitionStartY = startY;

			for (int i = windowStart, stop = model.size(); i < position
					&& i < stop; i++) {
				postitionStartY += ((Widget) model.elementAt(i)).widgetHeight;
			}

			positionEndY = postitionStartY
					+ ((Widget) model.elementAt(position)).widgetHeight;

			if (postitionStartY <= startY) {
				// move up
				windowStart = position;
			} else if (positionEndY > (startY + widgetHeight)) {
				// move down
				for (int i = windowStart, stop = model.size(); positionEndY > (startY + widgetHeight)
						&& i <= position && i < stop; i++) {
					windowStart++;
					positionEndY -= ((Widget) model.elementAt(i)).widgetHeight;
				}
			}

		}
	}

	public void paint(Graphics g, int x, int y, int maxX, int maxY,
			boolean hasFocus) {
		startY = y;
		startX = x;
		int[] clip = { g.getClipX(), g.getClipY(), g.getClipWidth(),
				g.getClipHeight() };
		g.setClip(x, y, widgetWidth, widgetHeight);

		if (!model.isEmpty()) {
			int count = model.size();

			g.setColor(bgColor);
			// g.fillRect(x,y,widgetWidth,widgetHeight);

			determineWindowStart();

			y += padding;
			for (int i = windowStart; i < count
					&& y < (startY + widgetHeight - padding - padding); i++) {
				widget = (Widget) model.elementAt(i);
				widget.paint(g, x + padding, y, widgetWidth - scrollbarWidth
						- padding - padding - 1, startY + widgetHeight,
						hasFocus && (position == i));
				y += widget.widgetHeight;
			}

			if (windowStart > 0
					|| y > ((startY + widgetHeight - padding - padding))) {
				double sliceHeight = (((double) widgetHeight) / count);
				int sliceY = (int) Math.floor((position * sliceHeight) + .5)
						+ startY + 1;

				g.setColor(Colors.SCROLLBAR);
				g.fillRect(x + widgetWidth - scrollbarWidth, sliceY,
						scrollbarWidth - 1, (int) Math.floor(.5 + sliceHeight));
			}
		}

		g.setClip(clip[0], clip[1], clip[2], clip[3]); // finally
	}

	public boolean supportsFocus() {
		return model.size() > 0;
	}
}