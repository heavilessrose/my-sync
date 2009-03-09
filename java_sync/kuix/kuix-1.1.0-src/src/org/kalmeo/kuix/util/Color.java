package org.kalmeo.kuix.util;

/**
 * @author bbeaulant
 */
public class Color {

	// Default colors
	public static final Color RED = new Color(0xFF0000);
	public static final Color GREEN = new Color(0x00FF00);
	public static final Color BLUE = new Color(0x0000FF);
	public static final Color BLACK = new Color(0x000000);
	public static final Color WHITE = new Color(0xFFFFFF);

	private int rgb;

	/**
	 * Construct a {@link Color}
	 * 
	 * @param rgb
	 */
	public Color(int rgb) {
		this.rgb = rgb;
	}

	/**
	 * Construct a {@link Color}
	 * 
	 * @param rgbHex
	 *            The RGB value in Hexadecimal format
	 */
	public Color(String rgbHex) {
		if (rgbHex.startsWith("#")) {
			rgbHex = rgbHex.substring(1);
		}
		this.rgb = Integer.parseInt(rgbHex, 16);
	}

	/**
	 * @return the rgb
	 */
	public int getRGB() {
		return rgb;
	}

}
