package bm.net;

import java.io.OutputStream;
import java.io.IOException;

import bm.core.tools.Adler32;

/*
 * File Information
 *
 * Created on       : 26-nov-2006 12:11:12
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:40:06 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:3 $
 */

/**
 * This output stream performs an Adler32 checksum over it's contents, and also
 * calculates the size in bytes of the data written.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision:3 $
 */
public class ChecksumedOutputStream extends OutputStream {
	private OutputStream out;
	private Adler32 crc = new Adler32();
	private long size;

	public ChecksumedOutputStream(final OutputStream out) {
		this.out = out;
	}

	public long getCRC() {
		return crc.getValue();
	}

	public long getSize() {
		return size;
	}

	public void write(int i) throws IOException {
		crc.update(i);
		out.write(i);
		size++;
	}

	public void write(byte[] bytes) throws IOException {
		crc.update(bytes);
		out.write(bytes);
		size += bytes.length;
	}

	public void write(byte[] bytes, int offset, int length) throws IOException {
		crc.update(bytes, offset, length);
		out.write(bytes, offset, length);
		size += length;
	}

	public void close() throws IOException {
		out.close();
	}
}
