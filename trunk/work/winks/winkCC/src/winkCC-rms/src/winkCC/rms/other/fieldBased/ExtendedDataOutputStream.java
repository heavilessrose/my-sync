package winkCC.rms.other.fieldBased;

import java.io.*;

public class ExtendedDataOutputStream extends DataOutputStream {
	public ExtendedDataOutputStream(OutputStream out) {
		super(out);
	}

	public final void writeIntArray(int[] arr) throws IOException {
		int size = arr.length;
		writeInt(size);
		for (int i = 0; i < size; ++i) {
			writeInt(arr[i]);
		}
	}
}
