package nio;

import java.nio.*;
import java.nio.channels.*;
import java.nio.charset.*;
import java.io.*;

/** ��buffer�еõ���ȷ���������ַ� */
public class BufferToText {
	private static final int BSIZE = 1024;

	public static void main(String[] args) throws Exception {
		FileChannel fc = new FileOutputStream("c:/data2.txt").getChannel();
		fc.write(ByteBuffer.wrap("һЩtext".getBytes()));
		fc.close();
		fc = new FileInputStream("c:/data2.txt").getChannel();
		ByteBuffer buff = ByteBuffer.allocate(BSIZE);
		fc.read(buff);
		buff.flip();
		// Doesn't work:
		System.out.println(buff.asCharBuffer());
		// Decode using this system's default Charset:
		buff.rewind();
		String encoding = System.getProperty("file.encoding");
		System.out.println("Decoded using " + encoding + ": "
				+ Charset.forName(encoding).decode(buff));
		// Or, we could encode with something that will print:
		fc = new FileOutputStream("c:/data2.txt").getChannel();
		fc.write(ByteBuffer.wrap("һЩText".getBytes("UTF-16BE")));
		fc.close();
		// Now try reading again:
		fc = new FileInputStream("c:/data2.txt").getChannel();
		buff.clear();
		fc.read(buff);
		buff.flip();
		System.out.println(buff.asCharBuffer());
//		// Use a CharBuffer to write through:
//		fc = new FileOutputStream("c:/data2.txt").getChannel();
//		buff = ByteBuffer.allocate(24); // More than needed
//		buff.asCharBuffer().put("һЩText");
//		fc.write(buff);
//		fc.close();
//		// Read and display:
//		fc = new FileInputStream("c:/data2.txt").getChannel();
//		buff.clear();
//		fc.read(buff);
//		buff.flip();
//		System.out.println(buff.asCharBuffer());
	}
} /* Output: ???? Decoded using Cp1252: Some text Some text Some text */// :~