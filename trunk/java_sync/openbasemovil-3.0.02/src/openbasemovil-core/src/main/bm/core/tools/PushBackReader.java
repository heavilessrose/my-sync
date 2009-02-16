package bm.core.tools;

import java.io.Reader;
import java.io.IOException;

/*
 * File Information
 *
 * Created on       : 19-oct-2007 14:34:09
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * The pushback reader allows to reinsert characters into the stream.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class PushBackReader extends Reader {
	private Reader in;
	private StringBuffer buffer = new StringBuffer();
	private int lastChar;

	public PushBackReader(final Reader in) {
		this.in = in;
	}

	public void revert(final char c) {
		if (c != -1) {
			buffer.append(c);
		}
	}

	public int read() throws IOException {
		if (buffer.length() > 0) {
			final int last = buffer.length() - 1;
			final char c = buffer.charAt(last);
			buffer.deleteCharAt(last);
			return lastChar = c;
		} else {
			return lastChar = in.read();
		}
	}

	public int read(char[] chars, int offset, int length) throws IOException {
		int c = read();
		int i = 0;
		while (c != -1 && i < length) {
			chars[i++] = (char) c;
			c = read();
		}
		return i;
	}

	public void close() throws IOException {
		in.close();
	}

	public int getLastChar() {
		return lastChar;
	}
}