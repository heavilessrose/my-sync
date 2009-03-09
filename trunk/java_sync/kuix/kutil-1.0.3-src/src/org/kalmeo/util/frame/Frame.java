package org.kalmeo.util.frame;

/**
 * @author bbeaulant
 */
public interface Frame {

	/**
	 * <b>Since 1.0.1, the old onMessage(String name, Object[] arguments)
	 * signature is deprecated.</b><br>
	 * 
	 * Called when message event occure.
	 * 
	 * @param identifier
	 *            the message identifier
	 * @param arguments
	 *            the message arguments
	 * @return <code>true</code> if the message should be passed to the next
	 *         frame in the stack.
	 */
	public boolean onMessage(Object identifier, Object[] arguments);

	/**
	 * Called when the frame is added to the stack (FrameHandler).
	 */
	public void onAdded();

	/**
	 * Called when the frame is removed from the stack (FrameHandler).
	 */
	public void onRemoved();

}
