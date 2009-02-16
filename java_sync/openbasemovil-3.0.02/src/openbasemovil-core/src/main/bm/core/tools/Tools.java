package bm.core.tools;

import bm.core.CoreConstants;
import bm.core.J2MEException;
import bm.core.log.Log;
import bm.core.log.LogFactory;

import javax.microedition.lcdui.Font;
import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 31-may-2005 0:10:15
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:34:57 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:4 $
 */

/**
 * Miscellaneous tools.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public final class Tools {
	private static Log log = LogFactory.getLog("Tools");

	public static final String BLANK = " \t\r\n,.:;(){}";

	/**
	 * Securely alloca a byte buffer. Tries to allocate the buffer, and if a
	 * OutOfMemoryError raises, invokes gc and then tries to allocate the buffer
	 * again. If it can't allocate the buffer, an OutOfMemoryError is thrown.
	 * 
	 * @param size
	 *            size of buffer in bytes
	 * @return allocated buffer
	 * @throws bm.core.J2MEException
	 *             if cant allocate memory
	 */
	public static byte[] secureAlloc(final int size) throws J2MEException {
		try {
			return new byte[size];
		} catch (OutOfMemoryError e) {
			log.warn("Could not allocate " + size + " bytes, free mem: "
					+ Runtime.getRuntime().freeMemory());
			System.gc();
			try {
				return new byte[size];
			} catch (OutOfMemoryError e1) {
				log.error("Can't allocate " + size + " bytes, free mem: "
						+ Runtime.getRuntime().freeMemory());
				throw new J2MEException(CoreConstants.ERR_TOL_SECURE_ALLOC, e1);
			}
		}
	}

	/**
	 * Convert an array of objects into a vector of them.
	 * 
	 * @param array
	 *            array
	 * @return null if the array is null, otherwise a vector with the array
	 *         elements in the same order.
	 */
	public static Vector arrayAsVector(final Object[] array) {
		if (array == null) {
			return null;
		} else {
			final int length = array.length;
			final Vector v = new Vector(length);
			for (int i = 0; i < length; i++) {
				v.addElement(array[i]);
			}
			return v;
		}
	}

	/**
	 * Checks equality of two objects, checking null values and object classes.
	 * 
	 * @param o1
	 *            first object
	 * @param o2
	 *            second object
	 * @return true if both are null or are of the same class and o1.equals( o2
	 *         ) returns true, false otherwise
	 */
	public static boolean objectEquals(final Object o1, final Object o2) {
		return o1 == null && o2 == null || o1 != null && o2 != null
				&& o1.getClass().isInstance(o2) && o1.equals(o2);
	}

	/**
	 * Create a string repeating a given one until it gets of at least a given
	 * length.<br/>
	 * For example, repeat( &quot;abc&quot;, 7 ) will return
	 * &quot;abcabcabc&quot;.
	 * 
	 * @param s
	 *            source string to repeat
	 * @param length
	 *            length to achieve
	 * @return repeated string.
	 */
	public static String repeat(final String s, final int length) {
		final StringBuffer buffer = new StringBuffer();
		while (buffer.length() < length) {
			buffer.append(s);
		}
		return buffer.toString();
	}

	public static long power(final long number, final int exponent) {
		long value = 1;
		for (int i = 0; i < exponent; i++) {
			value *= number;
		}
		return value;
	}

	/**
	 * Split a string into a list of words. To find word boundaries, the
	 * characters in the default separator string are used, and separators are
	 * not preserved.
	 * 
	 * @param str
	 *            string
	 * @return a list of words
	 */
	public static Vector getWords(final String str) {
		return getWords(str, BLANK, false);
	}

	/**
	 * Split a string into a list of words. To find word boundaries, the
	 * characters in the default separator string are used.
	 * 
	 * @param str
	 *            string
	 * @param preserveSeparators
	 *            preserve separator characters as words
	 * @return a list of words
	 */
	public static Vector getWords(final String str,
			final boolean preserveSeparators) {
		return getWords(str, BLANK, preserveSeparators);
	}

	/**
	 * Split a string into a list of words. To find word boundaries, the
	 * characters in the separator string are used.
	 * 
	 * @param str
	 *            string
	 * @param separators
	 *            a string with all the characters to use as word boundaries
	 * @param preserveSeparators
	 *            preserve separator characters as words
	 * @return a list of words
	 */
	public static Vector getWords(final String str, final String separators,
			final boolean preserveSeparators) {
		final char[] chars = str.toCharArray();
		final int length = chars.length;
		final StringBuffer buffer = new StringBuffer();
		final Vector words = new Vector(10);
		for (int i = 0; i < length; i++) {
			if (separators.indexOf(chars[i]) > -1) {
				if (buffer.length() > 0) {
					words.addElement(buffer.toString());
					buffer.delete(0, buffer.length());
				}
				if (preserveSeparators) {
					words.addElement(new String(chars, i, 1));
				}
			} else {
				buffer.append(chars[i]);
			}
		}
		if (buffer.length() > 0) {
			words.addElement(buffer.toString());
		}
		return words;
	}

	/**
	 * This method separates a string into different lines, using words, with a
	 * given width in pixels and fonts for making calculations. Two fonts can be
	 * given, one for the first line and other for the rest.<br/>
	 * If wrapping is disabled only new line characters are used to jump to the
	 * next line and exceeding characters are discarded, but if it's enabled the
	 * line is broken into different lines at word boundaries.
	 * 
	 * @param text
	 *            text to split
	 * @param font1
	 *            font for the first line
	 * @param font2
	 *            font for the second line
	 * @param width
	 *            witdh of region to use in pixels
	 * @param lines
	 *            maximum number to split the text into
	 * @param wrap
	 *            if wrapping should happen when a line is too long
	 * @return array of lines
	 */
	public static String[] splitInLines(final String text, final Font font1,
			final Font font2, final int width, final int lines,
			final boolean wrap) {
		final String[] aux = new String[lines];
		final Vector words = getWords(text, true);
		final int count = words.size();
		final StringBuffer line = new StringBuffer();
		int lineWidth = 0;
		int index = 0;
		boolean skipping = false;
		Font font = font1;
		for (int i = 0; i < count && index < lines; i++) {
			final String word = (String) words.elementAt(i);
			if (!skipping) {
				final int stringWidth = font.stringWidth(word);
				boolean newLine = false;
				if (word.charAt(0) == '\n') {
					newLine = true;
				} else if (lineWidth + stringWidth > width) {
					newLine = true;
					if (!wrap) {
						skipping = true;
					}
				}
				if (!newLine) {
					line.append(word);
					lineWidth += stringWidth;
				} else {
					aux[index++] = line.toString();
					line.delete(0, line.length());
					if (word.charAt(0) != '\n' && !skipping) {
						line.append(word);
						lineWidth = stringWidth;
					} else {
						lineWidth = 0;
					}
					font = font2;
				}
			} else {
				skipping = word.charAt(0) != '\n';
			}
		}
		if (line.length() > 0 && index < lines) {
			aux[index] = line.toString();
		}
		if (index < lines - 1) {
			final String[] retval = new String[index + 1];
			System.arraycopy(aux, 0, retval, 0, index + 1);
			return retval;
		} else {
			return aux;
		}
	}
}
