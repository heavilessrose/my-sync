package bm.core.tools;

import java.util.Enumeration;
import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 15-ago-2006 17:13:33
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Simple implementation of the missing class java.util.StringTokenizer.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class StringTokenizer {
	private static final String DEFAULT_SEPARATORS = " \t\r\n";

	private Enumeration i;

	public StringTokenizer(final String source, final String separators) {
		parse(source != null ? source.toCharArray() : null, separators);
	}

	public StringTokenizer(final char[] source, final String separators) {
		parse(source, separators);
	}

	public StringTokenizer(final String source) {
		parse(source != null ? source.toCharArray() : null, DEFAULT_SEPARATORS);
	}

	public StringTokenizer(final char[] source) {
		parse(source, DEFAULT_SEPARATORS);
	}

	private void parse(final char[] source, final String separators) {
		if (source == null) {
			i = new Vector().elements();
		} else {
			final String seps = separators != null ? separators
					: DEFAULT_SEPARATORS;
			final Vector tokens = new Vector(10);
			final StringBuffer token = new StringBuffer();
			final int length = source.length;
			for (int index = 0; index < length; index++) {
				if (seps.indexOf(source[index]) > -1) {
					tokens.addElement(token.toString());
					token.delete(0, token.length());
				} else {
					token.append(source[index]);
				}
			}
			if (token.length() > 0) {
				tokens.addElement(token.toString());
			}
			i = tokens.elements();
		}
	}

	public boolean hasMoreTokens() {
		return i.hasMoreElements();
	}

	public String nextToken() {
		return (String) i.nextElement();
	}
}
