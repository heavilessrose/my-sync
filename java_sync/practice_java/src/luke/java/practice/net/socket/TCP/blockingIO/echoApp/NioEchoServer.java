package luke.java.practice.net.socket.TCP.blockingIO.echoApp;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.CharBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharsetEncoder;
import java.util.Iterator;
import java.util.Set;

public class NioEchoServer {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println("current platform byte order: "
				+ ByteOrder.nativeOrder());

		ServerSocketChannel serverSocketChannel;
		Selector selector;
		InetSocketAddress addr = new InetSocketAddress("127.0.0.1", 4444);
		Charset charset = Charset.forName("GBK");
		CharsetDecoder decode = charset.newDecoder();
		CharsetEncoder encode = charset.newEncoder();

		try {
			selector = Selector.open();
			serverSocketChannel = ServerSocketChannel.open();
			serverSocketChannel.socket().bind(addr);
			serverSocketChannel.configureBlocking(false);
			serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);

			while (selector.select() > 0) {
				Set<SelectionKey> selectedKeys = selector.selectedKeys();
				Iterator it = selectedKeys.iterator();

				while (it.hasNext()) {
					SelectionKey ready = (SelectionKey) it.next();
					it.remove();

					if (ready.isAcceptable()) {// �ͻ�������
						ServerSocketChannel ssc = (ServerSocketChannel) ready
								.channel();

						// ��ȡ�ͻ���socketchannel
						SocketChannel sc = ssc.accept();
						sc.configureBlocking(false);
						SelectionKey rwKey = sc.register(selector,
								SelectionKey.OP_READ | SelectionKey.OP_WRITE,
								sc);
					} else if (ready.isReadable()) {// ��ȡ�ͻ�������
						ByteBuffer bytes = ByteBuffer.allocate(1024);
						CharBuffer chars;

						SocketChannel csc = (SocketChannel) ready.attachment();
						csc.read(bytes);
						bytes.flip();
						csc.write(bytes); // �˴�����ò�Ʋ�̫��, ���������Ӧ����ﵽĿ����..

						// chars = decode.decode(bytes);
						// System.out.println("request: " + chars.toString());
						// bytes.clear();
					} else if (ready.isWritable()) {// д�ͻ���
						// SocketChannel sc = (SocketChannel)
						// ready.attachment();
						// ByteBuffer response = ByteBuffer.wrap("response"
						// .getBytes());
						// sc.write(response);
					}
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
