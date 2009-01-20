package nio;

import java.nio.*;
import java.nio.channels.*;
import java.nio.charset.*;
import java.io.*;

/** 从buffer中得到正确编码解码的字符 */
public class BufferToText {
	private static final int BSIZE = 1024;

	public static void main(String[] args) throws Exception {
		FileChannel fc = new FileOutputStream("c:/data2.txt").getChannel();
		fc.write(ByteBuffer.wrap("一些text".getBytes()));
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
		fc.write(ByteBuffer.wrap("一些Text".getBytes("UTF-16BE")));
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
//		buff.asCharBuffer().put("一些Text");
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