package nio;

import java.nio.*;

public class IntBufferDemo {
	private static final int BSIZE = 1024;

	public static void main(String[] args) {
		// 实际存储数据
		ByteBuffer bb = ByteBuffer.allocate(BSIZE);
		// 提供int类型的视图缓冲器
		IntBuffer ib = bb.asIntBuffer();
		// 通过视图修改特定数据
		// Store an array of int:
		ib.put(new int[] { 11, 42, 47, 99, 143, 811, 1016 });
		// Absolute location read and write:
		System.out.println(ib.get(3));
		ib.put(3, 1811);
		// Setting a new limit before rewinding the buffer.
		ib.flip();
		while (ib.hasRemaining()) {
			int i = ib.get();
			System.out.println(i);
		}
	}
}
