package winkCC.rms.other.fieldBased;

import java.io.*;

// A version of ByteArrayInputStream that lets you replace the underlying byte array.
// 扩展ByteArrayInputStream, 提供替换下层字节数组的能力.
public class DirectByteArrayInputStream extends ByteArrayInputStream {

	public DirectByteArrayInputStream(byte buf[]) {
		super(buf);
	}

	public DirectByteArrayInputStream(byte buf[], int offset, int length) {
		super(buf, offset, length);
	}

	// Resets the array the stream reads from
	// 重置流的底层缓冲区数组.
	public synchronized void resetByteArray(byte[] buf) {
		this.buf = buf;
		this.pos = 0;
		this.count = buf.length;
		this.mark = 0;
	}

	// Resets the array the stream reads from
	// 重置流的底层缓冲区数组.
	public synchronized void resetByteArray(byte[] buf, int offset, int length) {
		this.buf = buf;
		this.pos = offset;
		this.count = Math.min(offset + length, buf.length);
		this.mark = offset;
	}
}
