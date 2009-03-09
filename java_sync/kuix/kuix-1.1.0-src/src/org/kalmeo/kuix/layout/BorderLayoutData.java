package org.kalmeo.kuix.layout;

/**
 * @author bbeaulant
 */
public class BorderLayoutData implements LayoutData {

	public static final byte CENTER = 0;
	public static final byte NORTH = 1;
	public static final byte EAST = 2;
	public static final byte WEST = 3;
	public static final byte SOUTH = 4;

	// Public static instances
	public static final BorderLayoutData instanceCenter = new BorderLayoutData(
			CENTER);
	public static final BorderLayoutData instanceNorth = new BorderLayoutData(
			NORTH);
	public static final BorderLayoutData instanceEast = new BorderLayoutData(
			EAST);
	public static final BorderLayoutData instanceWest = new BorderLayoutData(
			WEST);
	public static final BorderLayoutData instanceSouth = new BorderLayoutData(
			SOUTH);

	public byte position;

	/**
	 * Construct a {@link BorderLayoutData}
	 * 
	 * @param position
	 */
	private BorderLayoutData(byte position) {
		this.position = position;
	}

}
