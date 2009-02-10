package luke.java.practice.deadLinkChecker.util;

import java.io.FileOutputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharsetEncoder;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/** 页面工具 */
public class PageUtil implements Runnable {

	String host;
	int port;
	String res;

	public PageUtil() {
	}

	private void splitUrl(String pageUrl) {
		String regEx = "http://[\\w|.|\\d|:]+";
		Pattern p = Pattern.compile(regEx);

		String[] uris = pageUrl.split(regEx);
		res = uris[uris.length - 1];

		Matcher matcher = p.matcher(pageUrl);
		matcher.find();
		host = matcher.group();

		String[] hosts = host.split(":");
		port = hosts.length > 2 ? Integer.parseInt(hosts[hosts.length - 1])
				: 80;

		if (hosts.length > 2) {
			host = hosts[hosts.length - 2].split("//")[1];
		} else
			host = hosts[hosts.length - 1].split("//")[1];
		System.out.println("host: " + host + " res: " + res + " port: " + port);
	}

	// 
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		new PageUtil().getPage("http://java.sun.com/docs/books/tutorial/essential/regex/intro.html");
	}

	public void run() {

	}

	public String getPage(String pageUrl) {
		splitUrl(pageUrl);
		String str = null;

		// 准备连接
		SocketChannel channel = null;
		InetSocketAddress socketAddress = new InetSocketAddress(host, port);
		Charset charset = Charset.forName("UTF-8");
		CharsetDecoder decoder = charset.newDecoder();
		CharsetEncoder encoder = charset.newEncoder();

		// 准备buffer
		ByteBuffer buffer = ByteBuffer.allocateDirect(1024);
		CharBuffer charBuffer = CharBuffer.allocate(1024);

		try {
			// 连接
			channel = SocketChannel.open();
			channel.connect(socketAddress);

			// 构造http header
			String request = "HEAD " + res + " HTTP/1.1\r\n";
			request += "Host: " + host + "\r\n";
			request += "Accept:*/*\r\n";
			request += "User-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)\r\n";
			request += "Connection:Keep-Alive\r\n\r\n";
			System.out.println("request: " + request);
			
			// 发送GET请求
			channel.write(encoder.encode(CharBuffer.wrap(request)));

			// 读取response
			System.out.println("waiting for response");
			FileOutputStream fs = new FileOutputStream("c:/knowledge.htm");
			FileChannel fc = fs.getChannel();
			while ((channel.read(buffer)) != -1) {
				buffer.flip();
				fc.write(buffer);
				// Decode buffer
				buffer.flip();
				decoder.decode(buffer, charBuffer, false);
				// Display
				charBuffer.flip();
				System.out.println(charBuffer);
				buffer.clear();
				charBuffer.clear();
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (channel != null) {
				try {
					channel.close();
				} catch (IOException ignored) {
				}
			}
		}

		return str;
	}

}
