package luke.java.practice.concurrent;

import java.util.Random;

/**
 * 一个对象等待多个对象
 */
public class MultipleNotify {

	/**
	 * 程序入口
	 * 
	 * @param args
	 *            命令行参数
	 */
	public static void main(String[] args) {
		new MultipleNotify().go();
	}

	/**
	 * 演示如何使用 wait() 和 notify() 实现一个对象等待多个对象
	 */
	public void go() {
		// 锁对象中包含了一个数值  
		final Lock lock = new Lock();

		// 运行 20 个线程  
		Notifier[] notifiers = new Notifier[20];
		for (int i = 0; i < notifiers.length; i++) {
			notifiers[i] = new Notifier(lock);
			notifiers[i].start();
		}

		// 循环等待，通过 lock 的值来判断是否结束等待  
		boolean enough = false;
		while (!enough) {
			System.out.println("----------");
			try {
				synchronized (lock) {
					lock.wait();
					if (lock.value == notifiers.length) {
						enough = true;
					}
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		System.out.println("OK!");
	}

	/**
	 * 执行的线程。主线程要等待所有 Notifier 执行完毕才会继续
	 */
	private class Notifier extends Thread {

		private final Lock lock;

		private Notifier(Lock lock) {
			this.lock = lock;
		}

		public void run() {
			int sleepTime = 0;
			try {
				sleepTime = 500 + new Random().nextInt(2000);
				Thread.sleep(sleepTime);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			// 执行 lock.notify() 的同时将 lock 对象的值加 1  
			synchronized (lock) {
				lock.setValue(lock.getValue() + 1);
				System.out.println("lock = " + lock.getValue() + ", sleep:"
						+ sleepTime);
				lock.notify();
			}
		}
	}

	/**
	 * 包含一个数值的锁
	 */
	private class Lock {

		private int value = 0;

		public int getValue() {
			return value;
		}

		public void setValue(int value) {
			this.value = value;
		}
	}
}