package com.google.code.yanf4j.test.example.tcp;

import com.google.code.yanf4j.nio.Dispatcher;
import com.google.code.yanf4j.nio.Session;
import com.google.code.yanf4j.nio.impl.HandlerAdapter;
import com.google.code.yanf4j.nio.util.DispatcherFactory;

public class EchoHandler extends HandlerAdapter<String> {
	static boolean DEBUG = false;

	Dispatcher dispatcher = DispatcherFactory.newDispatcher(0);

	@Override
	public void onException(Session session, Throwable t) {
		t.printStackTrace();
	}

	@Override
	public void onMessageSent(Session session, String t) {
		if (DEBUG)
			System.out.println("sent " + t + " to "
					+ session.getRemoteSocketAddress());
	}

	@Override
	public void onSessionStarted(Session session) {
		if (DEBUG)
			System.out.println("session started");
		session.setUseBlockingRead(true);
		session.setUseBlockingWrite(false);
	}

	public void onSessionCreated(Session session) {
		if (DEBUG)
			System.out.println(session.getRemoteSocketAddress().toString()
					+ " connected");
	}

	public void onSessionClosed(Session session) {
		if (DEBUG)
			System.out.println(session.getRemoteSocketAddress().toString()
					+ " disconnected");

	}

	public void onReceive(final Session session, final String msg) {
		if (DEBUG)
			System.out.println("recv:" + msg);
		if (msg != null)
			dispatcher.dispatch(new Runnable() {
				public void run() {
					try {
						//for (int i = 0; i < 1000; i++)
							session.send(msg);
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			});
	}

	@Override
	public void onIdle(Session session) {
		if (DEBUG)
			System.out.println("session is idle");
	}

	@Override
	public void onExpired(Session session) {
		if (DEBUG)
			System.out.println("session " + session.getRemoteSocketAddress()
					+ " timeout");
	}

}
