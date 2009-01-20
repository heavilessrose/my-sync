package nio;

import java.nio.*;
import static util.Print.*;

public class GetData {
	private static final int BSIZE = 1024;

	public static void main(String[] args) {
		ByteBuffer bb = ByteBuffer.allocate(BSIZE);
		// Allocation automatically zeroes the ByteBuffer:
		int i = 0;
		while (i++ < bb.limit())
			if (bb.get() != 0)
				print("nonzero");
		print("i = " + i);
		print("positon: " + bb.position());
		bb.rewind();
		print("positon: " + bb.position());
		// Store and read a char array:
		// 得到char类型的视图, 并在视图上修改(会映射到底层实际存储数据的ByteBuffer)
		bb.asCharBuffer().put("Howdy!");
		char c;
		while ((c = bb.getChar()) != 0)
			printnb(c + " ");
		print();
		bb.rewind();
		// Store and read a short:
		bb.asShortBuffer().put((short) /* 32767 */32768);
		print(bb.getShort());
		bb.rewind();
		// Store and read an int:
		bb.asIntBuffer().put(99471142);
		print(bb.getInt());
		bb.rewind();
		// Store and read a long:
		bb.asLongBuffer().put(99471142);
		print(bb.getLong());
		bb.rewind();
		// Store and read a float:
		bb.asFloatBuffer().put(99471142);
		print(bb.getFloat());
		bb.rewind();
		// Store and read a double:
		bb.asDoubleBuffer().put(99471142);
		print(bb.getDouble());
		bb.rewind();
	}
}