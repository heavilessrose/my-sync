package com.google.code.yanf4j.test.example.tcp;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import com.google.code.yanf4j.config.Configuration;
import com.google.code.yanf4j.nio.TCPController;
import com.google.code.yanf4j.nio.impl.*;
import com.google.code.yanf4j.nio.impl.StringCodecFactory;
import com.google.code.yanf4j.statistics.Statistics;
import java.net.InetSocketAddress;

public class EchoServer {
	public static void main(String[] args) throws IOException {
		Configuration configuration = new Configuration();
		configuration.setStatisticsServer(true);
		configuration.setStatisticsInterval(20);
		configuration.setTcpSessionReadBufferSize(256 * 1024); // 设置读的缓冲区大小
		TCPController controller = new TCPController(configuration,
				new StringCodecFactory());
		controller.setLocalSocketAddress(new InetSocketAddress("localhost",
				8080));
		controller.setReadThreadCount(1); // 设置读线程数，通常为1
		controller.setReceiveBufferSize(16 * 1024); // 设置socket接收缓冲区大小
		controller.setReuseAddress(false); // 设置是否重用端口
		controller.setHandler(new EchoHandler()); // 设置handler
		controller.setHandleReadWriteConcurrently(true); // 设置是否允许读写并发处理
		controller.addStateListener(new ServerStateListener());
		controller.setSessionTimeout(10000); // 设置session超时
		controller.setSoTimeout(2000); // 设置accept超时
		controller.setBacklog(1000); // 设置backlog队列大小
		// 设置接收频率上限，packet个/秒
		// controller.setReceivePacketRate(2000);
		// 启用session的发送缓冲队列流量控制
		// controller.setSessionWriteQueueLowWaterMark(8 * 1024);
		// controller.setSessionWriteQueueHighWaterMark(4000 * 1024);
		controller.start();
		/**
		 * 更简化的启动方式： Controller controller = new TCPController();
		 * controller.open(configuration,new EchoHandler(),new
		 * StringCodecFactory());
		 * 
		 * open方法将初始化配置并自动调用start()方法
		 */
		BufferedReader reader = new BufferedReader(new InputStreamReader(
				System.in));
		String line = reader.readLine();
		while (line != null) {
			if ("stats".equals(line)) {
				Statistics statistics = controller.getStatistics();
				System.out.println("recv message total size:"
						+ statistics.getRecvMessageTotalSize());
				System.out.println("recv message count:"
						+ statistics.getRecvMessageCount());
				System.out.println("recv message average size:"
						+ statistics.getRecvMessageAverageSize());
				System.out.println("recv message per second:"
						+ statistics.getRecvMessageCountPerSecond());
				System.out.println("write message total size:"
						+ statistics.getWriteMessageTotalSize());
				System.out.println("write message count:"
						+ statistics.getWriteMessageCount());
				System.out.println("write message average size:"
						+ statistics.getWriteMessageAverageSize());
				System.out.println("write message per second:"
						+ statistics.getWriteMessageCountPerSecond());
				System.out.println("processed message count:"
						+ statistics.getProcessedMessageCount());
				System.out.println("processed message average time(ms):"
						+ statistics.getProcessedMessageAverageTime());
				System.out.println("accept new session per second:"
						+ statistics.getAcceptCountPerSecond());
				System.out.println("session size:"
						+ ((AbstractController) controller).getSessionSet()
								.size());

				System.out.println("****************************************");
			}
			if ("quit".equals(line)) {
				controller.stop();
				break;
			}
			line = reader.readLine();
		}
	}
}
