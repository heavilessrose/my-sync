package com.google.code.yanf4j.test.example.udp;

import java.net.*;

public class EchoClient {
	DatagramSocket datagramSocket;

	public EchoClient(String host, int port) throws SocketException,
			UnknownHostException {
		datagramSocket = new DatagramSocket();
		datagramSocket.connect(InetAddress.getByName(host), port);
		datagramSocket.setSoTimeout(1000);
	}

	public void sendUdp(byte[] buf) {
		try {

			DatagramPacket packet = new DatagramPacket(buf, buf.length);
			datagramSocket.send(packet);
			datagramSocket.receive(packet);
			System.out.println("recv:" + new String(packet.getData()));
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void close() {
		this.datagramSocket.close();
	}

	public static void main(String[] args) throws Exception {
		EchoClient ec = new EchoClient("localhost", 8090);
		byte[] b = null;
		for (int i = 1; i < 10000; i++) {
			b = (ec.getClass().getName() + ":" + i).getBytes();
			ec.sendUdp(b);
		}
		ec.close();
	}
}