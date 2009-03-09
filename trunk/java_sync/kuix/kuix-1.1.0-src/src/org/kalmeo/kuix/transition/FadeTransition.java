package org.kalmeo.kuix.transition;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

import org.kalmeo.util.worker.Worker;

/**
 * This class implements a fade transition between two screens(两屏幕淡入淡出切换效果).
 * This transition could be parametrized with a duration.
 * 
 * @author bbeaulant
 */
public class FadeTransition implements Transition {

	private int duration;
	private long startTime = -1;

	private int alphaIncrement;
	private int argbIncrement;

	private Image newImage;

	private int[] oldRgb;
	private int oldImageWidth;
	private int oldImageHeight;

	private int frameIndex = 0;

	/**
	 * Construct a {@link FadeTransition}
	 * 
	 * @param duration
	 *            the transition duration in milliseconds
	 */
	public FadeTransition(int duration) {
		this.duration = duration;
		int frameDuration = Worker.instance.getFrameDuration();
		alphaIncrement = 0xFF / (duration / frameDuration);
		argbIncrement = alphaIncrement << 24;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * org.kalmeo.kuix.transition.Transition#init(javax.microedition.lcdui.Image
	 * , javax.microedition.lcdui.Image)
	 */
	public void init(Image oldImage, Image newImage) {

		startTime = -1;

		oldImageWidth = oldImage.getWidth();
		oldImageHeight = oldImage.getHeight();
		oldRgb = new int[oldImageWidth * oldImageHeight];
		oldImage.getRGB(oldRgb, 0, oldImageWidth, 0, 0, oldImageWidth,
				oldImageHeight);

		// Save images
		this.newImage = newImage;

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * org.kalmeo.kuix.transition.Transition#process(javax.microedition.lcdui
	 * .Graphics)
	 */
	public boolean process(Graphics g) {
		g.drawImage(newImage, 0, 0, 0);
		if (startTime == -1) {
			startTime = System.currentTimeMillis();
		}
		if (System.currentTimeMillis() - startTime <= duration) {
			if (oldRgb.length != 0
					&& ((oldRgb[0] >> 24) & 0xFF) >= alphaIncrement) {
				int parity = frameIndex++ % 2;
				for (int i = oldRgb.length - 1 - parity; i >= 0; i -= 2) {
					oldRgb[i] -= argbIncrement;
				}
				g.drawRGB(oldRgb, 0, oldImageWidth, 0, 0, oldImageWidth,
						oldImageHeight, true);
				return false;
			}
		}
		return true;
	}

}
