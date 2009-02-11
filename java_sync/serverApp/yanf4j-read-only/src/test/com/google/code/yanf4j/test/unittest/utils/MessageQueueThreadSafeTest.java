package com.google.code.yanf4j.test.unittest.utils;

import java.util.concurrent.*;
import java.util.concurrent.atomic.*;

import com.google.code.yanf4j.util.*;

class PushThread implements Runnable {
	Queue<TestMessage> queue;
	CyclicBarrier barrier;

	public PushThread(Queue<TestMessage> queue, CyclicBarrier barrier) {
		this.queue = queue;
		this.barrier = barrier;
	}

	public void run() {
		try {
			barrier.await();
			int i = 1;
			while (i <= 1000) {
				if (queue.push(new TestMessage(i), 1000))
					i++;
			}
			barrier.await();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}

class PopThread implements Runnable {
	AtomicLong sum;
	Queue<TestMessage> queue;
	CyclicBarrier barrier;

	public PopThread(AtomicLong sum, Queue<TestMessage> queue,
			CyclicBarrier barrier) {
		super();
		this.sum = sum;
		this.queue = queue;
		this.barrier = barrier;
	}

	public void run() {

		try {
			barrier.await();
			int i = 0;
			while (i < 1000) {
				TestMessage t = queue.pop(1000);
				if (t != null) {
					sum.addAndGet(t.i);
					i++;
				}
			}

			barrier.await();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}

public class MessageQueueThreadSafeTest {
	static int num = 2000;

	public static void main(String args[]) throws Exception {
		CyclicBarrier barrier = new CyclicBarrier(2 * num + 1);
		//Queue<TestMessage> queue = new MessageQueue<TestMessage>();
		//Queue<TestMessage> queue = new MessageQueue<TestMessage>(true);
		//Queue<TestMessage> queue = new MessageQueue<TestMessage>(4096,4096);
		Queue<TestMessage> queue = new MessageQueue<TestMessage>(10*1024,10*1024);
		// Queue<TestMessage> queue = new MessageQueue<TestMessage>(1024 * 1024,
		// 1024 * 1024);
		AtomicLong sum = new AtomicLong(0);
		for (int i = 0; i < num; i++)
			new Thread(new PushThread(queue, barrier)).start();
		for (int i = 0; i < num; i++)
			new Thread(new PopThread(sum, queue, barrier)).start();
		long start = System.currentTimeMillis();
		barrier.await();

		barrier.await();
		System.out.println("timed:" + (System.currentTimeMillis() - start));
		assert (sum.get() == (num * 1000 * (1000 + 1) / 2));
		assert(queue.size()==0);
		System.out.println(sum.get() == (num * 1000 * (1000 + 1) / 2));
		System.out.println(queue.size()==0);
	}
}
