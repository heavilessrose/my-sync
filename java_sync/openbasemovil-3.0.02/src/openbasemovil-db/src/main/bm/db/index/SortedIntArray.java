package bm.db.index;

import bm.core.io.Serializable;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;

/*
 * File Information
 *
 * Created on       : 05-jul-2005 18:08:26
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-04-26 00:53:43 +0200 (jue, 26 abr 2007) $
 * Revision         : $Revision: 4 $
 */

/**
 * Sorted array of int values.<br/>
 * This array works as a vector, allowing to add new values to it, but always
 * doing it in a sorted way (ascending order).<br/>
 * The array can have a limited size or can be let to grow.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 4 $
 */
class SortedIntArray implements Serializable {
	private int[] array;
	private int size;
	private int growthFactor;

	/**
	 * Constructs a fixed size array.
	 * 
	 * @param maxSize
	 *            maximum size
	 */
	public SortedIntArray(final int maxSize) {
		array = new int[maxSize];
	}

	/**
	 * Constructs a variable size array.
	 * 
	 * @param initialSize
	 *            initial size
	 * @param growthFactor
	 *            entries allocated when more space is needed
	 */
	public SortedIntArray(final int initialSize, final int growthFactor) {
		array = new int[initialSize];
		this.growthFactor = growthFactor;
	}

	/**
	 * Get the name of the class to be used for serialization/deserialization of
	 * complex/nested objects.
	 * 
	 * @return class name
	 */
	public String getSerializableClassName() {
		return "bm.db.index.SortedIntArray";
	}

	/**
	 * Find the position int the array of a given value.
	 * 
	 * @param value
	 *            value to find
	 * @return position in the array (0 to size-1) or -1 if not found
	 */
	public int findIndex(final int value) {
		if (size > 0) {
			return findIndex(value, 0, size - 1);
		} else {
			return -1;
		}
	}

	/**
	 * Insert a new value into the array.<br/>
	 * If the value is already on the array nothing is done, and it's index is
	 * returned.
	 * 
	 * @param value
	 *            value to insert.
	 * @return if the array has fixed length and no more space is available this
	 *         method returns -1 indicating that the value could not be
	 *         inserted, otherwise the index at which the value was inserted is
	 *         returned.
	 */
	public int insert(final int value) {
		final int growthFactor = this.growthFactor;
		final int size = this.size;
		final int[] array = this.array;
		if (growthFactor == 0 && size == array.length) {
			return -1;
		}
		if (growthFactor > 0 && size == array.length) {
			final int[] newArray = new int[size + growthFactor];
			System.arraycopy(array, 0, newArray, 0, size);
			this.array = newArray;
		}
		final int index = findIndex(value);
		if (index == -1) {
			final int insertionPoint = findInsertionPoint(value);
			return insertAt(value, insertionPoint);
		} else {
			return index;
		}
	}

	/**
	 * Remove a value from the array. If the array does not contain the value
	 * nothing is done.
	 * 
	 * @param value
	 *            value to remove
	 * @return true if the value was found and removed, false otherwise
	 */
	public boolean remove(final int value) {
		int size = this.size; // Local variable to improve speed
		if (size > 0) {
			final int index = findIndex(value);
			if (index != -1) {
				if (size > 1) {
					System.arraycopy(array, index + 1, array, index, size
							- index - 1);
					size--;
				} else {
					size = 0;
				}
				this.size = size; // Reflect changes to instance property
				return true;
			}
		}
		return false;
	}

	/**
	 * Get the value at the given index.
	 * 
	 * @param index
	 *            position
	 * @return the value at the given index
	 * @throws ArrayIndexOutOfBoundsException
	 *             if the index is invalid
	 */
	public int getElementAt(final int index)
			throws ArrayIndexOutOfBoundsException {
		if (index >= 0 && index < size) {
			return array[index];
		} else {
			throw new ArrayIndexOutOfBoundsException(Integer.toString(index));
		}
	}

	/**
	 * Get the current size of the array (the actual number of elements in it,
	 * not the internal buffer size).
	 * 
	 * @return number of elements in the array
	 */
	public int size() {
		return size;
	}

	/**
	 * Get the current growth factor, 0 means fixed size.
	 * 
	 * @return growth factor
	 */
	public int getGrowthFactor() {
		return growthFactor;
	}

	/**
	 * Set the growth factor. Calling this method with 0 will make a variable
	 * size array become fixed size, and calling it with a value greater than 0
	 * will make a fixed size array become variable size.
	 * 
	 * @param growthFactor
	 *            growth factor
	 */
	public void setGrowthFactor(final int growthFactor) {
		this.growthFactor = growthFactor;
	}

	/**
	 * Check if the array is full.
	 * 
	 * @return true if the array is full (for fixed size arrays), always false
	 *         for variable size arrays
	 */
	public boolean isFull() {
		return growthFactor <= 0 && array.length == size;
	}

	/**
	 * Write the contents of the array into an output stream.
	 * 
	 * @param out
	 *            output stream
	 * @throws SerializationException
	 *             on errors
	 */
	public void serialize(final SerializerOutputStream out)
			throws SerializationException {
		out.writeByte((byte) 1); // Version
		out.writeInt(size);
		for (int i = 0; i < size; i++) {
			out.writeInt(array[i]);
		}
	}

	/**
	 * Read the contents of the array from an input stream.<br/>
	 * This method will enlarge the currently allocated buffer if it's not
	 * enough to hold the values from the stream, but won't change the growth
	 * factor, so the array will remain fixed or variable size, whatever it was
	 * before calling this method.<br/>
	 * What this means is that the growth factor is not persisted.
	 * 
	 * @param in
	 *            input stream
	 * @throws SerializationException
	 *             on errors
	 */
	public void deserialize(final SerializerInputStream in)
			throws SerializationException {
		int i = 0;
		in.readByte(); // Skip version
		final int size = in.readInt();
		this.size = size;
		// log.debug( "size: " + size );
		int[] array = this.array;
		if (array.length < size) {
			array = new int[size];
			this.array = array;
		}
		for (; i < size; i++) {
			array[i] = in.readInt();
		}
	}

	/**
	 * Get the array of ints that this object represents.
	 * 
	 * @return a trimmed copy of the internal buffer, so it can be modified
	 *         without affecting this object, or null if the size is 0.
	 */
	public int[] toIntArray() {
		final int size = this.size;
		if (size == 0) {
			return null;
		} else {
			final int[] result = new int[size];
			System.arraycopy(array, 0, result, 0, size);
			return result;
		}
	}

	/**
	 * Find the index at which to insert the given value.
	 * 
	 * @param value
	 *            value to insert
	 * @return position
	 */
	int findInsertionPoint(final int value) {
		if (size > 0) {
			return findInsertionPoint(value, 0, size - 1);
		} else {
			return 0;
		}
	}

	private int findIndex(final int value, final int start, final int end) {
		final int[] array = this.array;
		if (start == end) {
			if (array[start] == value) {
				return start;
			} else {
				return -1;
			}
		} else if (end - start == 1) {
			if (array[start] == value) {
				return start;
			} else if (array[end] == value) {
				return end;
			} else {
				return -1;
			}
		} else {
			final int middle = start + ((end - start) / 2);
			if (array[middle] == value) {
				return middle;
			} else if (value < array[middle]) {
				return findIndex(value, start, middle - 1);
			} else {
				return findIndex(value, middle + 1, end);
			}
		}
	}

	private int insertAt(final int value, final int index) {
		final int size = this.size;
		final int[] array = this.array;
		if (size < array.length) {
			if (index != size) {
				System.arraycopy(array, index, array, index + 1, size - index);
			}
			array[index] = value;
			this.size++;
			return index;
		} else {
			return -1;
		}
	}

	private int findInsertionPoint(final int value, final int start,
			final int end) {
		final int[] array = this.array;
		if (start == end) {
			if (value < array[start]) {
				return start;
			} else {
				return start + 1;
			}
		} else if (end - start == 1) {
			if (value < array[start]) {
				return start;
			} else if (value < array[end]) {
				return end;
			} else {
				return end + 1;
			}
		} else {
			final int middle = start + ((end - start) / 2);
			if (value < array[middle]) {
				return findInsertionPoint(value, start, middle - 1);
			} else {
				return findInsertionPoint(value, middle + 1, end);
			}
		}
	}

	public String toString() {
		final int size = this.size;
		final StringBuffer buffer = new StringBuffer("SortedIntArray{");
		buffer.append("size=").append(size);
		if (size > 0) {
			buffer.append("[");
			for (int i = 0; i < size; i++) {
				buffer.append(array[i]);
				if (i < (size - 1)) {
					buffer.append(",");
				}
			}
			buffer.append("]");
		}
		buffer.append("}");
		return buffer.toString();
	}
}
