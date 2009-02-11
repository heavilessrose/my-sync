/**
 *Copyright [2008-2009] [dennis zhuang]
 *Licensed under the Apache License, Version 2.0 (the "License");
 *you may not use this file except in compliance with the License. 
 *You may obtain a copy of the License at 
 *             http://www.apache.org/licenses/LICENSE-2.0 
 *Unless required by applicable law or agreed to in writing, 
 *software distributed under the License is distributed on an "AS IS" BASIS, 
 *WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, 
 *either express or implied. See the License for the specific language governing permissions and limitations under the License
 */

package com.google.code.yanf4j.nio;

import java.io.*;
import java.nio.channels.*;
import com.google.code.yanf4j.config.Configuration;
import com.google.code.yanf4j.nio.impl.AbstractTCPController;

/**
 * 核心类，负责select并派发事件，管理服务器生命周期
 * 
 * @author dennis
 */
public class TCPController extends AbstractTCPController {

	public ServerSocketChannel serverSocketChannel;

	private boolean tcpNoDelay = false;

	private int backlog = -1;

	public int getBacklog() {
		return backlog;
	}

	public void setBacklog(int backlog) {
		if (this.started)
			throw new IllegalStateException();
		if (backlog <= 0)
			throw new IllegalArgumentException();
		this.backlog = backlog;
	}

	public TCPController() {
		super();
	}

	public TCPController(Configuration configuration) {
		super(configuration, null, null);

	}

	@SuppressWarnings("unchecked")
	public TCPController(Configuration configuration, CodecFactory codecFactory) {
		super(configuration, null, codecFactory);
	}

	@SuppressWarnings("unchecked")
	public TCPController(Configuration configuration, Handler handler,
			CodecFactory codecFactory) {
		super(configuration, handler, codecFactory);
		setTcpNoDelay(configuration.isTcpNoDelay());
	}

	protected void doStart() throws IOException {
		serverSocketChannel = ServerSocketChannel.open();
		serverSocketChannel.socket().setSoTimeout(timeout);
		serverSocketChannel.socket().setReuseAddress(reuseAddress); // 重用端口
		if (this.receiveBufferSize > 0)
			serverSocketChannel.socket()
					.setReceiveBufferSize(receiveBufferSize); // 设置接收缓冲区
		if (this.backlog > 0)
			serverSocketChannel.socket().bind(this.socketAddress, backlog);
		else
			serverSocketChannel.socket().bind(this.socketAddress);
		serverSocketChannel.configureBlocking(false);
		selectionKey = serverSocketChannel.register(selector,
				SelectionKey.OP_ACCEPT);
	}

	private void configureChannel(SocketChannel sc) throws IOException {
		sc.configureBlocking(false);
		sc.socket().setTcpNoDelay(tcpNoDelay);
	}

	public void onAccept(SelectionKey sk) throws IOException {
		SocketChannel sc = null;
		try {
			sc = serverSocketChannel.accept();
			configureChannel(sc);
			SelectionKey selectionKey = sc.register(selector, 0);
			selectionKey.interestOps(selectionKey.interestOps()
					| SelectionKey.OP_READ);
			Session session = buildSession(sc, selectionKey);
			selectionKey.attach(session);
			session.start();
			selector.wakeup();
			super.onAccept(sk); // 统计
		} catch (IOException e) {
			sk.cancel();
			if (sc != null) {
				sc.socket().setSoLinger(true, 0); // 避免TIME_WAIT状态
				sc.close();
			}
			log.error(e, e);
			notifyException(e);
		}
	}

	public void closeChannel() throws IOException {
		if (serverSocketChannel != null)
			serverSocketChannel.close();
	}

}