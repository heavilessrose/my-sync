package bm.core.tools;

/*
 * File Information
 *
 * Created on       : 14-ene-2006 1:02:55
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * Adler32 Checksum, from java sources.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class Adler32 {
	/**
	 * largest prime smaller than 65536
	 */
	private static final int BASE = 65521;

	private int checksum; // we do all in int.

	// Note that java doesn't have unsigned integers,
	// so we have to be careful with what arithmetic
	// we do. We return the checksum as a long to
	// avoid sign confusion.

	/**
	 * Creates a new instance of the <code>Adler32</code> class. The checksum
	 * starts off with a value of 1.
	 */
	public Adler32() {
		reset();
	}

	/**
	 * Resets the Adler32 checksum to the initial value.
	 */
	public void reset() {
		checksum = 1; // Initialize to 1
	}

	/**
	 * Updates the checksum with the byte b.
	 * 
	 * @param bval
	 *            the data value to add. The high byte of the int is ignored.
	 */
	public void update(final int bval) {
		// We could make a length 1 byte array and call update again, but I
		// would rather not have that overhead
		int s1 = checksum & 0xffff;
		int s2 = checksum >>> 16;

		s1 = (s1 + (bval & 0xFF)) % BASE;
		s2 = (s1 + s2) % BASE;

		checksum = (s2 << 16) + s1;
	}

	/**
	 * Updates the checksum with the bytes taken from the array.
	 * 
	 * @param buffer
	 *            an array of bytes
	 */
	public void update(final byte[] buffer) {
		update(buffer, 0, buffer.length);
	}

	/**
	 * Updates the checksum with the bytes taken from the array.
	 * 
	 * @param buf
	 *            an array of bytes
	 * @param off
	 *            the start of the data used for this update
	 * @param len
	 *            the number of bytes to use for this update
	 */
	public void update(final byte[] buf, int off, int len) {
		int s1 = checksum & 0xffff;
		int s2 = checksum >>> 16;

		while (len > 0) {
			// We can defer the modulo operation:
			// s1 maximally grows from 65521 to 65521 + 255 * 3800
			// s2 maximally grows by 3800 * median(s1) = 2090079800 < 2^31
			int n = 3800;
			if (n > len) {
				n = len;
			}
			len -= n;
			while (--n >= 0) {
				s1 = s1 + (buf[off++] & 0xFF);
				s2 = s2 + s1;
			}
			s1 %= BASE;
			s2 %= BASE;
		}
		checksum = (s2 << 16) | s1;
	}

	/**
	 * Returns the Adler32 data checksum computed so far.
	 */
	public long getValue() {
		return (long) checksum & 0xffffffffL;
	}
}
