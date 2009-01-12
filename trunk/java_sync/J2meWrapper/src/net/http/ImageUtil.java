package net.http;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

/**
 * Uitlity class to handle Image, httpme 0.1 does not use it.
 * 
 * @author mingjava
 * @version 0.1 05/06/2006
 */
public class ImageUtil {

	/** Creates a new instance of ImageUtil */
	public ImageUtil() {
	}

	/**
	 * create a thumbnail image from orignal image
	 * 
	 * @param image
	 *            the orignal image
	 * @param width
	 *            new image's width
	 * @return the new thumbnail image
	 */
	public static Image createThumbnail(Image image, int width) {
		int sourceWidth = image.getWidth();
		int sourceHeight = image.getHeight();

		int thumbWidth = width;
		int thumbHeight = -1;

		if (thumbHeight == -1)
			thumbHeight = thumbWidth * sourceHeight / sourceWidth;

		Image thumb = Image.createImage(thumbWidth, thumbHeight);
		Graphics g = thumb.getGraphics();

		for (int y = 0; y < thumbHeight; y++) {
			for (int x = 0; x < thumbWidth; x++) {
				g.setClip(x, y, 1, 1);
				int dx = x * sourceWidth / thumbWidth;
				int dy = y * sourceHeight / thumbHeight;
				g
						.drawImage(image, x - dx, y - dy, Graphics.LEFT
								| Graphics.TOP);
			}
		}
		Image immutableThumb = Image.createImage(thumb);
		return immutableThumb;
	}

	private static byte[] int2byte(int[] data) {
		byte[] buffer = new byte[4 * data.length];
		for (int i = 0; i < data.length; i++) {
			int p = data[i];
			buffer[i * 4 + 0] = (byte) ((p & 0xff000000) >>> 24);
			buffer[i * 4 + 1] = (byte) ((p & 0xff0000) >>> 16);
			buffer[i * 4 + 2] = (byte) ((p & 0xff00) >>> 8);
			buffer[i * 4 + 3] = (byte) ((p & 0xff));
		}
		return buffer;
	}

	private static int[] byte2int(byte[] data) {
		int[] buffer = new int[data.length / 4];
		for (int i = 0; i < data.length / 4; i++) {
			int a = 0xff & data[i * 4 + 0];
			int r = 0xff & data[i * 4 + 1];
			int g = 0xff & data[i * 4 + 2];
			int b = 0xff & data[i * 4 + 3];
			buffer[i] = (a << 24) | (r << 16) | (g << 8) | b;
		}
		return buffer;
	}

}
