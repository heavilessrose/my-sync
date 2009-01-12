package com.astrientlabs.threads;

import java.util.Enumeration;
import java.util.Vector;

/** 管理workers */
public class JobRunner extends Vector implements Runnable {
	public static final JobRunner instance = new JobRunner(2,
			Thread.NORM_PRIORITY);

	private Semaphore semaphore;
	protected Vector workers;

	private int tick = 40 * 1000;
	private long lastCheck;
	private int lastSize;
	private boolean running;
	private int points = 0;
	private int priority;

	protected JobRunner(int c, int priority) {
		semaphore = new Semaphore(0);
		workers = new Vector();
		this.priority = priority;

		for (int i = 0; i < c; i++) {
			workers.addElement(new Worker(this, priority));
		}

		Cron.instance.register(this);
	}

	public void run(Runnable r) {
		addElement(r);
		semaphore.release();
	}

	/** 在Jobrunner中得到下一个任务 */
	public Runnable getNextJob() {
		semaphore.acquire();
		if (!isEmpty()) {
			Object o = this.firstElement();
			removeElement(o);
			return (Runnable) o;
		}

		return null;
	}

	/** 初始化后由cron启动,用来管理worker */
	public void run() {
		if (!running) {
			running = true;
			if (System.currentTimeMillis() > (tick + lastCheck)) {
				if (lastSize != 0) {
					if (size() >= lastSize) {
						points++;
					} else {
						points--;
					}

					if (points > 3) {

						points = 0;
						Worker w;
						Worker deadMan = null;
						for (Enumeration e = workers.elements(); e
								.hasMoreElements();) {
							w = (Worker) e.nextElement();
							if (w.thread == null || !w.thread.isAlive()) {
								deadMan = w;
								break;
							}
						}

						if (deadMan == null) {
							deadMan = (Worker) workers.firstElement();
							workers.removeElement(deadMan);
						}

						deadMan.thread = null;
						workers.addElement(new Worker(this, priority));
					}
				}

				lastCheck = System.currentTimeMillis();
			}
			running = false;
		}
	}
}