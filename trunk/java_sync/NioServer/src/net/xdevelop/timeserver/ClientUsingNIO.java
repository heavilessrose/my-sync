package net.xdevelop.timeserver;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;

public class ClientUsingNIO {

	/**
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) {
		SocketChannel client;
		try {
			client = SocketChannel.open(new InetSocketAddress(InetAddress
					.getLocalHost(), 5100));
			client.configureBlocking(false);

			Selector selector = Selector.open();
			client.register(selector, SelectionKey.OP_CONNECT
					| SelectionKey.OP_WRITE | SelectionKey.OP_READ);

		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

}
