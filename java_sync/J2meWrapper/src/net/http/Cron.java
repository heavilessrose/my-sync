package net.http;

import java.util.Vector;

/**
 * 后台管理 管理发送接收等http操作
 * 
 * @author WangYinghua
 * 
 */
public class Cron implements Runnable {
	public static final Cron instance = new Cron();
	private Vector runnables = new Vector();
	private Thread thread;
	private int stop, i;

	private Cron() {
		(thread = new Thread(this)).start();
	}

	public void register(Runnable a) {
		if (!runnables.contains(a)) {
			runnables.addElement(a);
		}
	}

	public void unregister(Runnable a) {
		runnables.removeElement(a);
	}

	public void run() {
		while (thread != null) {
			try {
				for (stop = runnables.size(), i = 0; i < stop; i++) {
					((Runnable) runnables.elementAt(i)).run();
				}

			} catch (Throwable t) {
			} finally {
				try {
					Thread.sleep(200);
				} catch (Exception e) {
				}
			}
		}
	}

}
