package com.google.code.yanf4j.test.example.udp;

import java.net.DatagramPacket;
import com.google.code.yanf4j.nio.Session;
import com.google.code.yanf4j.nio.UDPSession;
import com.google.code.yanf4j.nio.impl.HandlerAdapter;

@SuppressWarnings("unchecked")
public class EchoHandler extends HandlerAdapter {

	public void onReceive(Session udpSession, Object t) {
		DatagramPacket datagramPacket = (DatagramPacket) t;
		// System.out.println("recv:" + new String(datagramPacket.getData()));
		try {
			((UDPSession) udpSession).send(datagramPacket);
		} catch (InterruptedException e) {

		}
	}

	public void onMessageSent(Session session, Object t) {

	}

	public void onSessionStarted(Session session) {

	}

	public void onSessionCreated(Session session) {

	}

	public void onSessionClosed(Session session) {

	}

}
