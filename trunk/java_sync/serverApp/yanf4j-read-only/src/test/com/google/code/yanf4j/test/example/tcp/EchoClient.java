package com.google.code.yanf4j.test.example.tcp;

import java.net.InetSocketAddress;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.atomic.AtomicLong;

import com.google.code.yanf4j.config.Configuration;
import com.google.code.yanf4j.nio.ConnectorController;
import com.google.code.yanf4j.nio.Session;
import com.google.code.yanf4j.nio.impl.HandlerAdapter;
import com.google.code.yanf4j.nio.impl.StringCodecFactory;

public class EchoClient extends Thread {
	static final boolean DEBUG = true;
	private static int NUM = 10000;
	static final int MESSAGE_LEN = 10;
	private CyclicBarrier barrier;
	private AtomicLong recvSum = new AtomicLong();
	ConnectorController connector;

	public EchoClient(CyclicBarrier barrier) {
		this.barrier = barrier;
	}

	public void run() {
		Configuration configuration = new Configuration();
		configuration.setTcpSessionReadBufferSize(256 * 1024); // 设置读的缓冲区大小
		connector = new ConnectorController(configuration,
				new StringCodecFactory());
		connector.setHandler(new EchoHandler());
		connector.setCodecFactory(new StringCodecFactory());
		try {
			connector.Connect(new InetSocketAddress("localhost", 8080));
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void stopController() {
		connector.stop();
	}

	class EchoHandler extends HandlerAdapter<String> {

		@Override
		public void onConnected(Session session) {
			try {
				barrier.await();
				for (int i = 0; i < NUM; i++)
					session.send(generateString(i));
			} catch (Exception e) {

			}

		}

		public String generateString(int len) {
			StringBuffer sb = new StringBuffer();
			for (int i = 0; i < MESSAGE_LEN; i++)
				sb.append(i);
			return sb.toString();
		}

		@Override
		public void onException(Session session, Throwable t) {
			try {
				barrier.await();
			} catch (Exception e) {
				e.printStackTrace();
			}
			super.onException(session, t);
		}

		@Override
		public void onReceive(Session session, String t) {
			if (DEBUG)
				System.out.println("recv:" + t);
			recvSum.incrementAndGet();
			if (recvSum.get() == NUM)
				try {
					barrier.await();
				} catch (Exception e) {
					e.printStackTrace();
				}
		}

	}
}
