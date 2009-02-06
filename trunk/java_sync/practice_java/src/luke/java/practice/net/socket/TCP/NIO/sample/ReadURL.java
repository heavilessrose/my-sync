package luke.java.practice.net.socket.TCP.NIO.sample;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharsetEncoder;
import java.util.Iterator;
import java.util.Set;

public class ReadURL {
	public static void main(String args[]) {
		String host = /* args[0] */"www.google.com";
		SocketChannel channel = null;

		try {

			// Setup
			InetSocketAddress socketAddress = new InetSocketAddress(host, 80);
			Charset charset = Charset.forName("GBK");
			CharsetDecoder decoder = charset.newDecoder();
			CharsetEncoder encoder = charset.newEncoder();

			// Allocate buffers
			ByteBuffer buffer = ByteBuffer.allocateDirect(1024);
			CharBuffer charBuffer = CharBuffer.allocate(1024);

			// Connect
			channel = SocketChannel.open();

			channel.connect(socketAddress);

			// Send request
			String request = "GET / \r\n\r\n";
			channel.write(encoder.encode(CharBuffer.wrap(request)));

			// Read response
			while ((channel.read(buffer)) != -1) {
				buffer.flip();
				// Decode buffer
				decoder.decode(buffer, charBuffer, false);
				// Display
				charBuffer.flip();
				System.out.println(charBuffer);
				buffer.clear();
				charBuffer.clear();
			}
		} catch (UnknownHostException e) {
			System.err.println(e);
		} catch (IOException e) {
			System.err.println(e);
		} finally {
			if (channel != null) {
				try {
					channel.close();
				} catch (IOException ignored) {
				}
			}
		}
	}
}
