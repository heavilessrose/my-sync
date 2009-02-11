package com.google.code.yanf4j.test.example.tcp;

import java.util.concurrent.CyclicBarrier;

public class ClientTest {
	/*
	 * 
	 * @param args
	 */
	private static int NUM = 1;

	public static void main(String[] args) throws Exception {
		CyclicBarrier barrier = new CyclicBarrier(NUM + 1);
		EchoClient[] clients = new EchoClient[NUM];
		for (int i = 0; i < NUM; i++) {
			clients[i] = new EchoClient(barrier);
			clients[i].start();
		}
		barrier.await();
		long start = System.currentTimeMillis();
		barrier.await();
		long end = System.currentTimeMillis();
		for (EchoClient client : clients) {
			client.stopController();
		}
		System.out.println(NUM + " threads:" + (end - start));
	}

}
