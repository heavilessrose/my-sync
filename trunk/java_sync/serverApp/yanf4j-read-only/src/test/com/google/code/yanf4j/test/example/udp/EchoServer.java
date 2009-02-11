package com.google.code.yanf4j.test.example.udp;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.InetSocketAddress;

import com.google.code.yanf4j.config.Configuration;
import com.google.code.yanf4j.nio.UDPController;
import com.google.code.yanf4j.statistics.Statistics;

public class EchoServer {
	public static void main(String[] args) throws Exception {
		Configuration configuration = new Configuration();
		configuration.setReuseAddress(false);
		configuration.setStatisticsServer(true);
		configuration.setReadThreadCount(1);
		UDPController controller = new UDPController(configuration);
		controller.setMaxDatagramPacketLength(1024);
		controller.setHandler(new EchoHandler());
		controller.setLocalSocketAddress(new InetSocketAddress("localhost",8090));
		controller.start();
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
				System.out.println("****************************************");
			} else if ("quit".equals(line)) {
				controller.stop();
				break;
			}
			line = reader.readLine();
		}
	}
}
