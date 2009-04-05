package cn.sda.ui;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

/**
 * 
 * @author Administrator
 */
public class ImageUtils {

	public static int MODE_STRETCH = 0x10;
	public static int MODE_STRETCH_HORIZONTAL = 0x11;
	public static int MODE_STRETCH_VERTICAL = 0x12;
	public static int MODE_REPEAT = 0x20;

	/*
	 * 1.将一张图片pic转换成176208的图
	 * 
	 * pic=transImage(pic,176,208);
	 * 
	 * 2.将一张图片pic转换成原来的两倍大
	 * 
	 * pic=scaleImage(pic,2,1);
	 * 
	 * 3.将一张图片pic转换成原来的三分之二
	 * 
	 * pic=scaleImage(pic,2,3);
	 */

	private static final Image scaleImage(Image src, int scales1, int scales2) {
		return transImage(src, src.getWidth() * scales1 / scales2, src
				.getHeight()
				* scales1 / scales2);
	}

	private static final Image transImage(Image src, int w, int h) {
		int srcW = src.getWidth();
		int srcH = src.getHeight();
		int dstW = w, dstH = h;
		Image tmp = Image.createImage(dstW, srcH);
		Graphics g = tmp.getGraphics();
		int scale = 16;
		int delta = (srcW << scale) / dstW; // 扫描长度

		int pos = delta / 2; // 扫描位置

		for (int x = 0; x < dstW; x++) {
			g.setClip(x, 0, 1, srcH);
			g.drawImage(src, x - (pos >> scale), 0, Graphics.LEFT
					| Graphics.TOP);
			pos += delta;
		}
		Image dst = Image.createImage(dstW, dstH);
		g = dst.getGraphics();
		delta = (srcH << scale) / dstH;
		pos = delta / 2;
		for (int y = 0; y < dstH; y++) {
			g.setClip(0, y, dstW, 1);
			g.drawImage(tmp, 0, y - (pos >> scale), Graphics.LEFT
					| Graphics.TOP);
			pos += delta;
		}
		tmp = null;
		return dst;
	}

	public static Image processImage(Image img, int dstWidth, int dstHeight,
			int mode) {
		if (mode == MODE_STRETCH) {
			return transImage(img, dstWidth, dstHeight);
		}
		if (mode == MODE_STRETCH_HORIZONTAL) {
			return transImage(img, dstWidth, img.getHeight());
		}
		if (mode == MODE_STRETCH_VERTICAL) {
			return transImage(img, img.getWidth(), dstHeight);
		}
		if (mode == MODE_REPEAT) {
			Image result = Image.createImage(dstWidth, dstHeight);
			Graphics g = result.getGraphics();
			int icount = dstWidth / img.getWidth() + 1;
			int jcount = dstHeight / img.getHeight() + 1;
			for (int i = 0; i < icount; i++) {
				for (int j = 0; i < jcount; j++) {
					g
							.drawImage(img, i * img.getWidth(), j
									* img.getHeight(), 0);
				}
			}
			return result;
		}
		if (mode == (MODE_STRETCH_HORIZONTAL | MODE_REPEAT)) {
			Image result = Image.createImage(dstWidth, dstHeight);
			Graphics g = result.getGraphics();
			Image tmp = transImage(img, dstWidth, img.getHeight());
			int count = dstHeight / img.getHeight() + 1;
			for (int i = 0; i < count; i++) {
				g.drawImage(tmp, 0, i * img.getHeight(), 0);
			}
			tmp = null;
			return result;
		}
		if (mode == (MODE_STRETCH_VERTICAL | MODE_REPEAT)) {
			Image result = Image.createImage(dstWidth, dstHeight);
			Graphics g = result.getGraphics();
			Image tmp = transImage(img, img.getWidth(), dstHeight);
			int count = dstWidth / img.getWidth() + 1;
			for (int i = 0; i < count; i++) {
				g.drawImage(tmp, i * img.getWidth(), 0, 0);
			}
			tmp = null;
			return result;
		}
		return img;
	}
}
