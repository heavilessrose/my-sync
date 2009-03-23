package winkCC.rms.other.fieldBased;

import java.io.*;

// A version of ByteArrayOutputStream that gives you
// direct access to the underlying byte array if
// you need it.
public class DirectByteArrayOutputStream extends ByteArrayOutputStream {

	public DirectByteArrayOutputStream() {
		super();
	}

	public DirectByteArrayOutputStream(int size) {
		super(size);
	}

	// Returns a reference to the underlying byte array.
	// The actual amount of data in the byte array is
	// obtained via the size method.
	public synchronized byte[] getByteArray() {
		return buf;
	}

	// Swaps in a new byte array for the old one, resetting
	// the count as well.
	public synchronized byte[] swap(byte[] newBuf) {
		byte[] oldBuf = buf;
		buf = newBuf;
		reset();
		return oldBuf;
	}
}
