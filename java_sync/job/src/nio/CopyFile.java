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

		// ��һ��: ��ȡͨ��
		FileInputStream fin = new FileInputStream(infile);
		FileOutputStream fout = new FileOutputStream(outfile);

		FileChannel fcin = fin.getChannel();
		FileChannel fcout = fout.getChannel();

		// �ڶ���: ����������
		ByteBuffer buffer = ByteBuffer.allocate(1024);

		// ������: ��������д
		while (true) {
			// resets the buffer, making it ready to have data read into it
			// ���軺������ʹ�����Խ��ܶ��������
			buffer.clear();

			int r = fcin.read(buffer);

			// done
			if (r == -1) {
				break;
			}
			// prepares the buffer to have the newly-read data written to
			// another channel
			// �û��������Խ��¶��������д����һ��ͨ��
			buffer.flip();

			fcout.write(buffer);
		}
	}
}
