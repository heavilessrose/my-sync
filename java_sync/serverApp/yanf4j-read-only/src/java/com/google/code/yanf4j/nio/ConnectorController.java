package com.google.code.yanf4j.nio;

import java.io.IOException;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;
import java.net.SocketAddress;

import com.google.code.yanf4j.config.Configuration;
import com.google.code.yanf4j.nio.impl.AbstractTCPController;
import com.google.code.yanf4j.nio.util.EventType;

/**
 * 用于客户端连接的Controller主控类
 * 
 * @author Administrator
 * 
 */
public class ConnectorController extends AbstractTCPController {
	private SocketChannel socketChannel;

	private SocketAddress remoteAddr;

	private boolean connected;

	public ConnectorController() {
		super();
	}

	public ConnectorController(Configuration configuration) {
		super(configuration, null, null);

	}

	@SuppressWarnings("unchecked")
	public ConnectorController(Configuration configuration,
			CodecFactory codecFactory) {
		super(configuration, null, codecFactory);
	}

	public void Connect(SocketAddress remoteAddr) throws IOException {
		this.remoteAddr = remoteAddr;
		this.start();
	}

	public boolean isConnected() {
		return connected;
	}

	@Override
	protected void doStart() throws IOException {
		this.socketChannel = SocketChannel.open();
		this.socketChannel.configureBlocking(false);
		socketChannel.socket().setSoTimeout(timeout);
		socketChannel.socket().setReuseAddress(reuseAddress); // 重用端口
		if (this.receiveBufferSize > 0)
			socketChannel.socket().setReceiveBufferSize(receiveBufferSize); // 设置接收缓冲区
		socketChannel.socket().bind(this.socketAddress);
		this.connected = this.socketChannel.connect(this.remoteAddr);
		if (!this.connected)
			this.selectionKey = socketChannel.register(selector,
					SelectionKey.OP_CONNECT);
		else {
			createSession(this.selectionKey);
		}

	}

	public void onConnect(SelectionKey key) throws IOException {
		key.interestOps(key.interestOps() & ~SelectionKey.OP_CONNECT);
		if (!this.socketChannel.finishConnect())
			throw new IOException("Connect Fail");
		connected = true;
		createSession(key);
	}

	private void createSession(SelectionKey key) {
		Session session = buildSession(this.socketChannel, key);
		session.onEvent(EventType.ENABLE_READ, selector);
		key.attach(session);
		session.start();
		session.onEvent(EventType.CONNECTED, selector);
		selector.wakeup();
	}

	public void closeChannel() throws IOException {
		if (socketChannel != null)
			socketChannel.close();
	}
}
