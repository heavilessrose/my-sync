package com.google.code.yanf4j.nio;

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
import java.io.IOException;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import com.google.code.yanf4j.config.Configuration;
import com.google.code.yanf4j.nio.impl.AbstractController;
import com.google.code.yanf4j.nio.impl.DefaultUDPSession;
import com.google.code.yanf4j.nio.util.EventType;
import com.google.code.yanf4j.util.Queue;

/**
 * udp核心控制类
 * 
 * @author dennis
 * 
 */
public class UDPController extends AbstractController {
	private DatagramChannel channel;

	private Session udpSession;

	private int maxDatagramPacketLength;

	public int getMaxDatagramPacketLength() {
		return maxDatagramPacketLength;
	}

	@Override
	public void setReadThreadCount(int readThreadCount) {
		if (readThreadCount > 1)
			throw new IllegalArgumentException(
					"UDP controller could not have more than 1 read thread");
		super.setReadThreadCount(readThreadCount);
	}

	public void setMaxDatagramPacketLength(int maxDatagramPacketLength) {
		if (started)
			throw new IllegalStateException();
		String osName = System.getProperties().getProperty("os.name");
		if (isLinux(osName) && maxDatagramPacketLength > 9216)
			throw new IllegalArgumentException(
					"The maxDatagramPacketLength could not be larger than 9216 bytes on linux");
		else if (maxDatagramPacketLength > 65507)
			throw new IllegalArgumentException(
					"The maxDatagramPacketLength could not be larger than 65507 bytes");
		this.maxDatagramPacketLength = maxDatagramPacketLength;
	}

	private boolean isLinux(String osName) {
		return osName.indexOf("linux") >= 0 || osName.indexOf("Linux") >= 0;
	}

	public void closeChannel() throws IOException {
		if (this.udpSession != null && !this.udpSession.isClose())
			this.udpSession.close();
		if (channel != null)
			channel.close();
	}

	public void onWrite(SelectionKey key) {
		this.udpSession.onEvent(EventType.WRITEABLE, selector);
	}

	public UDPController(Configuration configuration) {
		super(configuration, null, null);
		setMaxDatagramPacketLength(configuration.getTcpSessionReadBufferSize());
	}

	@Override
	protected Runnable getReadHandler(SelectionKey key) {
		return new Runnable() {
			public void run() {
				udpSession.onEvent(EventType.READABLE, selector);
			}
		};
	}

	public UDPController() {
		super();
	}

	@SuppressWarnings("unchecked")
	public UDPController(Configuration configuration, CodecFactory codecFactory) {
		super(configuration, null, codecFactory);
		setMaxDatagramPacketLength(configuration.getTcpSessionReadBufferSize());
	}

	@SuppressWarnings("unchecked")
	public UDPController(Configuration configuration, Handler handler,
			CodecFactory codecFactory) {
		super(configuration, handler, codecFactory);
		setMaxDatagramPacketLength(configuration.getTcpSessionReadBufferSize());
	}

	protected void doStart() throws IOException {
		this.channel = DatagramChannel.open();
		this.channel.socket().setSoTimeout(timeout);
		this.channel.socket().setReuseAddress(reuseAddress);
		if (this.receiveBufferSize > 0)
			this.channel.socket().setReceiveBufferSize(this.receiveBufferSize);
		this.channel.socket().bind(this.socketAddress);
		this.channel.configureBlocking(false);
		this.selectionKey = this.channel.register(this.selector, SelectionKey.OP_READ);
		initializeReactor();
		Queue<Session.WriteMessage> queue = buildQueue();
		udpSession = new DefaultUDPSession(selectionKey, channel, reactor, handler,
				this.maxDatagramPacketLength, statistics, getCodecFactory(),
				queue, handleReadWriteConcurrently);
		this.selectionKey.attach(udpSession);
		udpSession.start();
	}
}
