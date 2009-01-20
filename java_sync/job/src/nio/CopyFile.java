package nio;
// $Id$

import java.io.*;
import java.nio.*;
import java.nio.channels.*;

public class CopyFile {
	static public void main(String args[]) throws Exception {
		if (args.length < 2) {
			System.err.println("Usage: java CopyFile infile outfile");
			System.exit(1);
		}

		String infile = args[0];
		String outfile = args[1];

		// 第一步: 获取通道
		FileInputStream fin = new FileInputStream(infile);
		FileOutputStream fout = new FileOutputStream(outfile);

		FileChannel fcin = fin.getChannel();
		FileChannel fcout = fout.getChannel();

		// 第二步: 创建缓冲区
		ByteBuffer buffer = ByteBuffer.allocate(1024);

		// 第三步: 缓冲区读写
		while (true) {
			// resets the buffer, making it ready to have data read into it
			// 重设缓冲区，使它可以接受读入的数据
			buffer.clear();

			int r = fcin.read(buffer);

			// done
			if (r == -1) {
				break;
			}
			// prepares the buffer to have the newly-read data written to
			// another channel
			// 让缓冲区可以将新读入的数据写入另一个通道
			buffer.flip();

			fcout.write(buffer);
		}
	}
}
