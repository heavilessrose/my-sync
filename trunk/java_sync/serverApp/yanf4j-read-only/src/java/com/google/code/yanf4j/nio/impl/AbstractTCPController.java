package com.google.code.yanf4j.nio.impl;

import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;

import com.google.code.yanf4j.config.Configuration;
import com.google.code.yanf4j.nio.CodecFactory;
import com.google.code.yanf4j.nio.Handler;
import com.google.code.yanf4j.nio.Session;
import com.google.code.yanf4j.nio.util.EventType;
import com.google.code.yanf4j.util.Queue;

public abstract class AbstractTCPController extends AbstractController {
	protected boolean tcpNoDelay = false;

	public AbstractTCPController() {
		super();
	}

	public boolean isTcpNoDelay() {
		return this.tcpNoDelay;
	}

	public void setTcpNoDelay(boolean tcpNoDelay) {
		this.tcpNoDelay = tcpNoDelay;
	}

	public AbstractTCPController(Configuration configuration) {
		super(configuration, null, null);

	}

	@SuppressWarnings("unchecked")
	public AbstractTCPController(Configuration configuration,
			CodecFactory codecFactory) {
		super(configuration, null, codecFactory);
	}

	@SuppressWarnings("unchecked")
	public AbstractTCPController(Configuration configuration, Handler handler,
			CodecFactory codecFactory) {
		super(configuration, handler, codecFactory);
		setTcpNoDelay(configuration.isTcpNoDelay());
	}

	protected Runnable getReadHandler(final SelectionKey key) {
		final Session session = (Session) key.attachment();
		return new Runnable() {
			public void run() {
				session.onEvent(EventType.READABLE, selector);
			}
		};
	}

	protected Session buildSession(SocketChannel sc, SelectionKey selectionKey) {
		Queue<Session.WriteMessage> queue = buildQueue();
		Session session = new DefaultTCPSession(sc, selectionKey, handler,
				reactor, getCodecFactory(), configuration
						.getTcpRecvBufferSize(), statistics, queue,
				sessionTimeout, handleReadWriteConcurrently);
		return session;
	}

	public void onWrite(SelectionKey key) {
		if (key.attachment() != null)
			((Session) (key.attachment())).onEvent(EventType.WRITEABLE,
					selector);
	}
}
