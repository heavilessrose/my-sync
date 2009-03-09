package org.kalmeo.util;

/**
 * @author bbeaulant
 */
public interface LinkedListItem {

	/**
	 * @return The next {@link LinkedListItem}
	 */
	public LinkedListItem getNext();

	/**
	 * @param next
	 */
	public void setNext(LinkedListItem next);

	/**
	 * @return The previous {@link LinkedListItem}
	 */
	public LinkedListItem getPrevious();

	/**
	 * @param previous
	 */
	public void setPrevious(LinkedListItem previous);

	/**
	 * Compares this {@link LinkedListItem} with the specified <code>item</code>
	 * for order.
	 * 
	 * @param item
	 *            the {@link LinkedListItem} to be compared.
	 * @param flag
	 *            the flag witch is transmit from the {@link LinkedList} sort
	 *            method
	 * @return a negative integer, zero, or a positive integer as this item is
	 *         less than, equal to, or greater than the specified item.
	 */
	public int compareTo(LinkedListItem item, int flag);

}
