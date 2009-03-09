package org.kalmeo.kuix.transition;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

/**
 * A transition is an effect apply when switching between two screens.
 * Implements this interface to write your own transition.
 * 
 * @author bbeaulant
 */
public interface Transition {

	/**
	 * This method is called before the transition starts.
	 * 
	 * @param oldImage
	 * @param newImage
	 */
	public void init(Image oldImage, Image newImage);

	/**
	 * This method is called each frame during the transition.
	 * 
	 * @param g
	 * @return <code>true</code> if the transition is finished
	 */
	public boolean process(Graphics g);

}
