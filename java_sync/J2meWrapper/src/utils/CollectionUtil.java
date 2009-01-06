package utils;

/**
 * A collection of static utility methods for
 * working with arrays.
 *
 * Note: No reflection sucks
 *
 * @author Luke
 */

import java.util.Vector;
import java.util.Enumeration;

public class CollectionUtil {

	/**
	 * As the Vector.toArray method is not available in
	 * CLDC, this method will take a reference to a Vector
	 * and return an array containing all the contained objects.
	 *
	 * @param v The vector to turn into an array
	 * @param a The array
	 */
	public static void vectorToArray(Vector v, Object[] a) {
		Enumeration e = v.elements();

		int x = 0;

		while (e.hasMoreElements()) {
			a[x] = e.nextElement();
			x++;
		}
	}
}