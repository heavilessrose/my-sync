package com.google.code.yanf4j.test.example.tcp;

import com.google.code.yanf4j.nio.Controller;
import com.google.code.yanf4j.nio.ControllerStateListener;
import com.google.code.yanf4j.nio.impl.AbstractController;
import com.google.code.yanf4j.statistics.Statistics;

public class ServerStateListener implements ControllerStateListener {

	public void onAllSessionClosed(Controller controller) {
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
				+ ((AbstractController) controller).getSessionSet().size());
		System.out.println("****************************************");
	}

	public void onException(Controller acceptor, Throwable t) {
		t.printStackTrace();
	}

	public void onReady(Controller acceptor) {
		System.out.println("The server is ready");
	}

	public void onStarted(Controller acceptor) {
		System.out.println("The server started");
	}

	public void onStopped(Controller acceptor) {
		System.out.println("The server stopped");

	}

}
