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
import com.astrientlabs.fonts.Fonts;
import com.astrientlabs.i18n.TextMap;
import com.astrientlabs.midlet.Yipi;
import com.astrientlabs.text.Strings;
import com.astrientlabs.util.Images;

//TODO: why abstract
public abstract class Window extends Canvas {
	protected PopupWidget popup;
	protected Vector children = new Vector();

	protected Widget focus;
	protected Widget prevFocus;

	protected int width;
	protected int height;
	protected int padding = 4;

	protected int headerHeight;
	protected int footerHeight;
	protected boolean showPopup = false;
	protected int startX = 0;
	protected int startY = 0;

	public Window(String title) {
		this.setTitle(title);
		this.setFullScreenMode(true);

		width = getWidth();
		height = getHeight();
		headerHeight = padding + Images.TITLE.getHeight() + padding;
		footerHeight = padding;
	}

	public void setPopup(PopupWidget popup) {
		if (!(focus instanceof PopupWidget))
			this.prevFocus = focus;
		this.popup = popup;
		this.showPopup = true;
		this.focus = popup;
		repaint();
	}

	public void clearPopup() {
		this.showPopup = false;
		this.popup = null;
		this.focus = prevFocus;
		repaint();
	}

	protected void sizeChanged(int w, int h) {
		width = w;
		height = h;
	}

	public int getWindowWidth() {
		return width;
	}

	protected void paint(Graphics g) {
		g.setFont(Fonts.TEXT);
		g.setColor(Colors.BACKGROUND);
		g.fillRect(startX, startY, width, height);
		// g.drawImage(Images.BACKGROUND,0,height,Graphics.LEFT|Graphics.BOTTOM);

		drawFrame(g);
		drawHeader(g);
		drawFooter(g);
	}

	protected void drawHeader(Graphics g) {
		g.setColor(Colors.HEADER_BGCOLOR);
		g.fillRect(startX, startY, width, headerHeight);

		g.drawImage(Images.TITLE, width >> 1, padding, Graphics.HCENTER
				| Graphics.TOP);
	}

	protected void drawFooter(Graphics g) {
		g.setColor(Colors.FOOTER_BGCOLOR);
		g.fillRect(startX, startY + height - footerHeight, getWidth(),
				footerHeight + 1);

		g.setColor(Colors.SOFTKEY_TEXT);
		g.setFont(Fonts.TEXT);

		if (showPopup && popup != null) {
			popup.paint(g, width - popup.widgetWidth - 2, height - footerHeight
					- popup.widgetHeight - 1 - 2, getWidth() - 2,
					getHeight() - 2, (focus == popup));
		}
	}

	public void repaintChild(Widget child, boolean force) {
		repaint(startX, child.startY, child.getWidth(), child.widgetHeight + 1);
		if (force)
			serviceRepaints();
	}

	protected void keyPressed(int keyCode) {
		if (getFocus() == null) {
			handleKeyPressed(keyCode);
		} else {
			focus.keyPressed(keyCode);
		}
	}

	protected void keyRepeated(int keyCode) {
		if (getFocus() == null) {
			handleKeyPressed(keyCode);
		} else {
			focus.keyRepeated(keyCode);
		}
	}

	protected Widget getFocus() {
		if ((focus == null) || (!showPopup && (popup == focus))) {
			Widget temp;
			for (int stop = children.size(), i = 0; i < stop; i++) {
				temp = (Widget) children.elementAt(i);
				if (temp.supportsFocus()) {
					focus = temp;
					break;
				}
			}
		}

		return focus;
	}

	protected void handleKeyPressed(int keyCode) {
	}

	public void moveFocus(boolean forward) {
		Widget oldFocus = focus;

		if (!children.isEmpty()) {
			int index = (oldFocus == null) ? 0 : children.indexOf(oldFocus);
			if (index == -1) {
				focus = (Widget) children.firstElement();
			} else {
				Widget temp;
				if (forward) {
					for (int stop = children.size(), i = index + 1; i < stop; i++) {
						temp = (Widget) children.elementAt(i);
						if (temp.supportsFocus()) {
							focus = temp;
							break;
						}
					}
				} else {
					for (int i = index - 1; i >= 0; i--) {
						temp = (Widget) children.elementAt(i);
						if (temp.supportsFocus()) {
							focus = temp;
							break;
						}
					}
				}
			}
		}

		if (oldFocus != focus) {
			if (oldFocus != null) {
				repaintChild(oldFocus, true);
			}

			if (focus != null) {
				repaintChild(focus, true);
			}

			oldFocus.onFocusExit();
			focus.onFocusEnter();
			focusChanged(oldFocus, focus);
		}
	}

	public void focusChanged(Widget oldFocus, Widget focus) {
	}

	public void widgetSelected(Widget who, Object arg) {

	}

	protected void drawFrame(Graphics g) {
		Widget w;
		for (int i = 0, stop = children.size(), y = startY + headerHeight; i < stop
				&& startY < height; i++) {
			w = (Widget) children.elementAt(i);
			w.paint(g, startX, y, getWidth(), height - footerHeight,
					(focus == w));
			if (w.xyFloat == Widget.POSITION_RELATIVE)
				y += (w.widgetHeight);
		}
	}

	public void navigateTo(Window window) {
		window.getFocus();
		Yipi.instance.display.setCurrent(window);
	}

	public void setMessage(String message) {
		PopupWidget popup = new PopupWidget(this, (int) (getWidth() * .90),
				(int) (getHeight() * .60), Colors.POPUP_BG);
		popup.setText(message);
		setPopup(popup);
	}

	public void setError(Throwable t) {
		setError((Strings.isNull(t.getMessage())) ? TextMap.instance
				.get("error.0") : TextMap.instance.get("error.0") + "\n\n"
				+ t.getMessage());
	}

	public void setError(String text) {
		PopupWidget popup = new PopupWidget(this, (int) (getWidth() * .90),
				(int) (getHeight() * .60), Colors.POPUP_ERROR_BG);
		popup.setText(text);
		popup.modal = false;
		setPopup(popup);
	}
}
