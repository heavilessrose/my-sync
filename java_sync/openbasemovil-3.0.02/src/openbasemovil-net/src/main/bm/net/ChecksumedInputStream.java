package bm.net;

import bm.core.tools.Adler32;

import java.io.IOException;
import java.io.InputStream;

/*
 * File Information
 *
 * Created on       : 19-dic-2006 9:34:32
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-02-03 19:40:06 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision: 3 $
 */

/**
 * This input stream performs an Adler32 checksum over it's contents, and also
 * calculates the size in bytes of the data read.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 3 $
 */
public class ChecksumedInputStream extends InputStream {
	private InputStream in;
	private Adler32 crc = new Adler32();
	private long size;

	public ChecksumedInputStream(final InputStream out) {
		this.in = out;
	}

	public long getCRC() {
		return crc.getValue();
	}

	public long getSize() {
		return size;
	}

	public int read() throws IOException {
		int c = in.read();
		if (c != -1) {
			crc.update(c);
			size++;
		}
		return c;
	}

	public int read(byte[] bytes) throws IOException {
		return read(bytes, 0, bytes.length);
	}

	public int read(byte[] bytes, int offset, int length) throws IOException {
		final int total = in.read(bytes, offset, length);
		if (total > 0) {
			size += total;
			crc.update(bytes, offset, total);
		}
		return total;
	}

	public void close() throws IOException {
		in.close();
	}
}
