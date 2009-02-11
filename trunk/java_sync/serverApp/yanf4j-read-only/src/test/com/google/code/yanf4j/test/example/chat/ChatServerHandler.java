package com.google.code.yanf4j.test.example.chat;

import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.atomic.AtomicInteger;

import com.google.code.yanf4j.nio.Session;
import com.google.code.yanf4j.nio.impl.HandlerAdapter;

public class ChatServerHandler extends HandlerAdapter<String> {
	List<Session> sessionList = new CopyOnWriteArrayList<Session>();
	AtomicInteger userId = new AtomicInteger();

	@Override
	public void onReceive(Session session, String t) {
		if (t.equals("quit"))
			session.close();
		else {
			try {
				broadcast(session, "[" + session.attachment() + "] say:" + t);
			} catch (InterruptedException e) {

			}
		}

	}

	@Override
	public void onSessionClosed(Session session) {
		sessionList.remove(session);
		try {
			broadcast(session, session.attachment() + " leave room");
		} catch (InterruptedException e) {

		}
	}

	private void broadcast(Session s, String msg) throws InterruptedException {
		for (Session session : this.sessionList) {
			session.send(msg);
		}
	}

	@Override
	public void onSessionCreated(Session session) {
		sessionList.add(session);
		// 给session取名
		session.attach(generateUserName());
		// 广播某人进来
		try {
			broadcast(session, session.attachment() + " enter room");
		} catch (InterruptedException e) {

		}
	}

	private String generateUserName() {
		return "user" + userId.incrementAndGet();
	}

}
