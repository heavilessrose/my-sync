package org.kalmeo.kuix.layout;

import org.kalmeo.kuix.util.Alignment;
import org.kalmeo.util.MathFP;

/**
 * @author bbeaulant
 */
public class StaticLayoutData implements LayoutData {

	// Full instance
	public static final StaticLayoutData instanceFull = new StaticLayoutData(
			Alignment.TOP_LEFT, MathFP.ONE, MathFP.ONE);

	public int x;
	public int y;

	public Alignment alignment;

	// width and height are fixed-point values
	public int width;
	public int height;

	/**
	 * Construct a {@link StaticLayoutData}
	 * 
	 * @param alignment
	 */
	public StaticLayoutData(Alignment alignment) {
		this(alignment, -1, -1);
	}

	/**
	 * Construct a {@link StaticLayoutData} If <code>width</code> or
	 * <code>height</code> is < 1 it is considered as percentage else pixels
	 * size. Both <code>width</code> and <code>height</code> are representing by
	 * fixed-point integer. {@see org.kalmeo.util.FPUtil} for more informations
	 * 
	 * @param alignment
	 * @param width
	 *            a fixed-point integer representing the width
	 * @param height
	 *            a fixed-point integer representing the height
	 */
	public StaticLayoutData(Alignment alignment, int width, int height) {
		this.alignment = alignment;
		this.width = width;
		this.height = height;
	}

}
