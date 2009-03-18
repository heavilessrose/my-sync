package luke.java.practice.concurrent;

import java.util.Random;

/**
 * һ������ȴ��������
 */
public class MultipleNotify {

	/**
	 * �������
	 * 
	 * @param args
	 *            �����в���
	 */
	public static void main(String[] args) {
		new MultipleNotify().go();
	}

	/**
	 * ��ʾ���ʹ�� wait() �� notify() ʵ��һ������ȴ��������
	 */
	public void go() {
		// �������а�����һ����ֵ  
		final Lock lock = new Lock();

		// ���� 20 ���߳�  
		Notifier[] notifiers = new Notifier[20];
		for (int i = 0; i < notifiers.length; i++) {
			notifiers[i] = new Notifier(lock);
			notifiers[i].start();
		}

		// ѭ���ȴ���ͨ�� lock ��ֵ���ж��Ƿ�����ȴ�  
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
	 * ִ�е��̡߳����߳�Ҫ�ȴ����� Notifier ִ����ϲŻ����
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
			// ִ�� lock.notify() ��ͬʱ�� lock �����ֵ�� 1  
			synchronized (lock) {
				lock.setValue(lock.getValue() + 1);
				System.out.println("lock = " + lock.getValue() + ", sleep:"
						+ sleepTime);
				lock.notify();
			}
		}
	}

	/**
	 * ����һ����ֵ����
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