package org.kalmeo.kuix.util;

/**
 * An <code>Insets</code> object is a representation of the borders of a
 * container(容器边界的插入物). It specifies the space that a container must leave at
 * each of its edges. The space can be a margin(边缘的空白), a border(边界) or a
 * padding(填充物).
 */
public class Insets {

	public int top;
	public int left;
	public int bottom;
	public int right;

	/**
	 * Creates and initializes a new <code>Insets</code> object with null values
	 */
	public Insets() {
		this(0, 0, 0, 0);
	}

	/**
	 * Creates and initializes a new <code>Insets</code> object with the
	 * specified top, left, bottom, and right insets.
	 * 
	 * @param top
	 *            the inset from the top.
	 * @param right
	 *            the inset from the right.
	 * @param bottom
	 *            the inset from the bottom.
	 * @param left
	 *            the inset from the left.
	 */
	public Insets(int top, int right, int bottom, int left) {
		this.top = top;
		this.right = right;
		this.bottom = bottom;
		this.left = left;
	}

}
