package com.astrientlabs.threads;

import java.util.Vector;

/**
 * 后台类
 * 
 * 启动后就一直运行, 执行注册的任务
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

	// 200+time to exec
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