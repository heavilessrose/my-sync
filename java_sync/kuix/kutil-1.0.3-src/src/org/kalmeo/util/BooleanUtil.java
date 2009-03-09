package org.kalmeo.util;

/**
 * @author bbeaulant
 */
public class BooleanUtil {

	public static final String TRUE = "true";
	public static final String FALSE = "false";
	public static final String NOT_FALSE = "!" + FALSE;

	/**
	 * Returns a String object representing this Boolean's value. If this object
	 * represents the value true, a string equal to "true" is returned.
	 * Otherwise, a string equal to "false" is returned.
	 * 
	 * @param value
	 * @return a string representation of this object.
	 */
	public static String toString(boolean value) {
		if (value) {
			return TRUE;
		} else {
			return FALSE;
		}
	}

	/**
	 * Parses the string argument as a boolean.
	 * 
	 * @param s
	 *            a string.
	 * @return the boolean represented by the argument.
	 */
	public static boolean parseBoolean(String s) {
		if (s != null) {
			s = s.trim();
			return (TRUE.equals(s) || NOT_FALSE.equals(s));
		}
		return false;
	}

}
