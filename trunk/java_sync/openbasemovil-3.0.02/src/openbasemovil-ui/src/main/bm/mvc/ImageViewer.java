package bm.mvc;

import bm.util.Util;

import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.Graphics;

/*
 * File Information
 *
 * Created on       : 30-mar-2007 14:22:48
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Image viewer that can stretch, zoom and move over an image.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ImageViewer extends Canvas {
	public static final int ZOOM_FIT = -1;
	public static final int ZOOM_FIT_WIDTH = -2;
	public static final int ZOOM_FIT_HEIGHT = -3;

	private Image image;
	private Image shown;
	private Image scaled;
	private int x;
	private int y;
	private int zoom = ZOOM_FIT;

	public Image getImage() {
		return image;
	}

	public void setImage(final Image image) {
		this.image = image;
		x = y = 0;
		adjustImage();
		if (isShown()) {
			repaint();
		}
	}

	private void adjustImage() {
		final Image image = this.image;
		if (image != null) {
			final int wrRatio = image.getWidth() * 100 / image.getHeight();
			final int rwRatio = image.getHeight() * 100 / image.getWidth();
			switch (zoom) {
			case ZOOM_FIT: {
				int width = getWidth();
				int height = (width * wrRatio) / 100;
				if (height > getHeight()) {
					height = getHeight();
					width = (height * rwRatio) / 100;
				}
				scaled = Util.scale(image, width, height);
			}
				break;

			case ZOOM_FIT_HEIGHT: {
				int height = getHeight();
				int width = (height * rwRatio) / 100;
				scaled = Util.scale(image, width, height);
			}
				break;

			case ZOOM_FIT_WIDTH: {
				int width = getWidth();
				int height = (width * wrRatio) / 100;
				scaled = Util.scale(image, width, height);
			}
				break;

			default:
				if (zoom == 100) {
					shown = Image.createImage(image);
				} else {
					int width = image.getWidth() * zoom / 100;
					int height = (width * wrRatio) / 100;
					scaled = Util.scale(image, width, height);
				}
			}
			shown = Util.crop(scaled, x, y, getWidth(), getHeight());
		}
	}

	public int getZoom() {
		return zoom;
	}

	public void setZoom(final int zoom) {
		this.zoom = zoom;
		x = y = 0;
		adjustImage();
		if (isShown()) {
			repaint();
		}
	}

	public void rotateLeft() {
		x = y = 0;

	}

	public void rotateRight() {
		x = y = 0;

	}

	protected void paint(final Graphics graphics) {
		if (image != null) {
			graphics.drawImage(shown, x, y, Graphics.TOP | Graphics.LEFT);
		}
	}
}
