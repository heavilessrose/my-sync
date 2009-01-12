/*
 * Copyright (C) 2006 Astrient Labs, LLC
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.ui;

import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Graphics;

import com.astrientlabs.colors.Colors;
import com.astrientlabs.fonts.Fonts;
import com.astrientlabs.ui.util.TextInputEngine;
import com.astrientlabs.ui.util.TextInputListener;

public class TextInputWidget extends Widget implements Runnable {
	private TextInputEngine textInputEngine;

	private int padding = 3;
	private int borderThickness = 2;

	private String paintText;
	private int flicker;
	private int caretPos;
	private int caretX;
	private int maxLen;

	public TextInputWidget(Window parent, String prompt, String text,
			TextInputListener textInputListener) {
		super(parent);

		this.name = prompt;
		widgetHeight = Fonts.TEXT.getHeight() + (2 * padding);
		widgetWidth = getWidth();
		maxLen = (widgetWidth - (2 * padding)) - Fonts.TEXT.stringWidth(prompt);

		textInputEngine = new TextInputEngine(textInputListener, 1300, 800,
				400, 100);
		textInputEngine.setText(text);
		paintText = createPaintText(text);
	}

	public void paint(Graphics g, int x, int y, int maxX, int maxY,
			boolean hasFocus) {
		startY = y;
		g.setFont(Fonts.TEXT);

		// draw bg
		g.setColor(Colors.TEXTINPUT_BG);
		g.fillRect(x, y, widgetWidth, widgetHeight);

		// draw borders
		g.setColor(Colors.HIGHLIGHT_LINE);
		g.drawLine(x, y, x + widgetWidth, y);
		g.drawLine(x, y + widgetHeight, x + widgetWidth, y + widgetHeight);

		// draw icon
		// g.drawImage(icon,x+padding,y +
		// (widgetHeight>>1),Graphics.VCENTER|Graphics.LEFT);
		g.drawString(name, x + padding, y + padding + borderThickness,
				Graphics.LEFT | Graphics.TOP);

		// draw text
		caretX = x + padding + g.getFont().stringWidth(name) + padding
				+ padding;

		g.setColor(Colors.HIGHLIGHT_TEXT);
		if (paintText == null) {

		} else {
			g.drawString(paintText, caretX, y + padding + borderThickness,
					Graphics.LEFT | Graphics.TOP);
		}

		if (hasFocus && ((flicker++ & 1) == 0)) {
			caretPos = textInputEngine.getCaretPosition();
			if (caretPos > 0 && (paintText != null)) {
				if (caretPos >= (paintText.length())) {
					caretX += g.getFont().stringWidth(paintText);
				} else {
					caretX += g.getFont().stringWidth(
							paintText.substring(0, caretPos));
				}
			}

			g.drawString("_", caretX, y + padding + borderThickness,
					Graphics.LEFT | Graphics.TOP);
		}
	}

	public void textChanged() {
		paintText = createPaintText(textInputEngine.getText());
	}

	// TODO: for text in this gets costly
	int strWidth;

	public String createPaintText(String text) {
		if (text != null) {
			if ((strWidth = Fonts.TEXT.stringWidth(text)) >= maxLen) {
				int i = 0;
				for (int len = text.length(); i < len && strWidth >= maxLen; i++) {
					strWidth -= Fonts.TEXT.charWidth(text.charAt(i));
				}
				return text.substring(i);
			}
		}

		return text;
	}

	public String getText() {
		return textInputEngine.getText();
	}

	public void keyPressed(int keyCode) {
		if (!textInputEngine.keyPressed(keyCode)) {
			switch (parent.getGameAction(keyCode)) {
			case Canvas.LEFT:
				textInputEngine.delete();
				break;
			case Canvas.RIGHT:
				textInputEngine.moveCursor(true);
				break;
			case 505:
				textInputEngine.delete();
				break;
			default:
				super.keyPressed(keyCode);
			}
		}
	}

	public void keyRepeated(int keyCode) {
		keyPressed(keyCode);
	}

	public void onFocusEnter() {
		textInputEngine.onFocusEnter();
	}

	public void onFocusExit() {
		textInputEngine.onFocusExit();
	}

	public void run() {
		textInputEngine.run();
	}
}