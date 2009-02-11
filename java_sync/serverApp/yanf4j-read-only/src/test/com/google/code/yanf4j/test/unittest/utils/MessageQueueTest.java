package com.google.code.yanf4j.test.unittest.utils;

import junit.framework.TestCase;
import com.google.code.yanf4j.util.*;
import com.google.code.yanf4j.nio.*;

class TestMessage implements Message {

	int i;

	public int getLength() {
		return 4;
	}

	public TestMessage(int i) {
		super();
		this.i = i;
	}

}

public class MessageQueueTest extends TestCase {
	Queue<TestMessage> queue;

	/**
	 * 测试push pop
	 */
	public void testPushAndPop() throws InterruptedException {
		queue = new MessageQueue<TestMessage>();
		assertTrue(queue.isEmpty());
		assertFalse(queue.isFull());
		for (int i = 0; i < 1000; i++)
			assertTrue(queue.push(new TestMessage(i)));

		assertFalse(queue.isEmpty());
		assertFalse(queue.isFull());
		assertEquals(1000, queue.size());

		TestMessage t = queue.pop();
		assertEquals(0, t.i);
		assertFalse(queue.isEmpty());
		assertFalse(queue.isFull());
		assertEquals(queue.size(), 999);

		queue.pop();
		assertEquals(queue.size(), 998);

		for (int i = 0; i < 1000; i++)
			assertTrue(queue.push(new TestMessage(i)));

		assertFalse(queue.isEmpty());
		assertFalse(queue.isFull());
		assertEquals(queue.size(), 1998);

		for (int i = 0; i < 1998; i++)
			assertNotNull(queue.pop());
		assertTrue(queue.isEmpty());
		assertFalse(queue.isFull());
		assertNull(queue.pop());
	}

	public void testBlockPushAndPop() throws InterruptedException {
		// 启用流量控制,将进行阻塞push和pop
		queue = new MessageQueue<TestMessage>(true);

		assertTrue(queue.isEmpty());
		assertFalse(queue.isFull());
		for (int i = 0; i < 1000; i++)
			assertTrue(queue.push(new TestMessage(i)));

		assertFalse(queue.isEmpty());
		assertFalse(queue.isFull());
		assertEquals(1000, queue.size());

		for (int i = 0; i < 3096; i++)
			assertTrue(queue.push(new TestMessage(i)));

		assertFalse(queue.isEmpty());
		assertTrue(queue.isFull());
		assertEquals(4096, queue.size());
		// 启动线程，1秒后取出一个元素，解除push的阻塞
		new Thread() {
			public void run() {
				try {
					Thread.sleep(1000);
					queue.pop();
				} catch (InterruptedException e) {

				}
			}
		}.start();

		assertTrue(queue.push(new TestMessage(-1)));
		assertFalse(queue.isEmpty());
		assertTrue(queue.isFull());
		assertEquals(4096, queue.size());

		queue.clear();
		assertTrue(queue.isEmpty());
		assertFalse(queue.isFull());

		// 启动线程，1秒后放入一个元素，解除pop的阻塞
		new Thread() {
			public void run() {
				try {
					Thread.sleep(1000);
					queue.push(new TestMessage(-1));
				} catch (InterruptedException e) {

				}
			}
		}.start();
		assertEquals(queue.pop().i, -1);
		assertTrue(queue.isEmpty());
		assertEquals(0, queue.size());
	}

	public void testFlowControll() throws InterruptedException {
		// 启用流量控制,将进行阻塞push和pop
		queue = new MessageQueue<TestMessage>(100, 400);

		assertTrue(queue.isEmpty());
		assertFalse(queue.isFull());
		for (int i = 0; i < 25; i++)
			queue.push(new TestMessage(i));
		assertFalse(queue.isEmpty());
		assertFalse(queue.isFull());

		assertEquals(queue.size(), 25);
		for (int i = 0; i < 75; i++)
			queue.push(new TestMessage(i));
		assertFalse(queue.isEmpty());
		assertTrue(queue.isFull());
		assertEquals(queue.size(), 100);
		
		Thread[] threads = new Thread[10];
        //启动10个线程插入数据，由于队列已满，因此线程将被阻塞
		for (int i = 0; i < 10; i++)
			threads[i] = new Thread() {
				public void run() {
					try {
						queue.push(new TestMessage(10));
					} catch (Exception e) {

					}
				}
			};
		for (Thread thread : threads) {
			thread.start();
		}
		for (Thread thread : threads) {
			thread.join(200);
		}
		//确认线程还存活
		for (Thread thread : threads) {
			assertTrue(thread.isAlive());
		}
		assertFalse(queue.isEmpty());
		assertTrue(queue.isFull());
		assertEquals(queue.size(), 100);
		//弹出10个元素，仍然高于lowWaterMark
		for(int i=0;i<10;i++)
			assertNotNull(queue.pop());
		//弹出10个元素，仍然高于lowWaterMark，继续阻塞
		assertFalse(queue.isEmpty());
		assertFalse(queue.isFull());
		assertEquals(queue.size(), 90);
		//弹出另外65个元素，也就是说总共已经出队75个，仍然高于低水位标记，继续阻塞
		for(int i=0;i<65;i++)
			assertNotNull(queue.pop());
		
		assertFalse(queue.isEmpty());
		assertFalse(queue.isFull());
		assertEquals(queue.size(), 25);
		
		//弹出10个元素，开始低于lowWaterMark
		for(int i=0;i<10;i++)
			assertNotNull(queue.pop());
		//等待线程结束
		for (Thread thread : threads) {
			thread.join();
		}
		assertFalse(queue.isEmpty());
		assertFalse(queue.isFull());
		assertEquals(queue.size(), 25);
	}

	public void testTimeoutPushAndPop() throws InterruptedException {
		queue = new MessageQueue<TestMessage>(1024, 4 * 1024);
		assertTrue(queue.isEmpty());
		assertFalse(queue.isFull());

		for (int i = 0; i < 256; i++)
			assertTrue(queue.push(new TestMessage(i), 1000));

		assertFalse(queue.isEmpty());
		assertFalse(queue.isFull());
		assertEquals(256, queue.size());

		for (int i = 256; i < 1024; i++)
			assertTrue(queue.push(new TestMessage(i), 1000));
		assertFalse(queue.isEmpty());
		assertTrue(queue.isFull());
		assertEquals(1024, queue.size());

		long start = System.currentTimeMillis();
		// 阻塞1秒，无法加入
		assertFalse(queue.push(new TestMessage(-1), 1000));
		assertEquals(System.currentTimeMillis() - start, 1000, 20);
		assertEquals(1024, queue.size());

		for (int i = 0; i < 1024; i++)
			assertEquals(queue.pop(1000).i, i);

		assertTrue(queue.isEmpty());
		assertFalse(queue.isFull());
		start = System.currentTimeMillis();
		// 阻塞1秒，无元素可供pop
		assertNull(queue.pop(1000));
		assertEquals(System.currentTimeMillis() - start, 1000, 20);
		assertTrue(queue.isEmpty());
	}

}
