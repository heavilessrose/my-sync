/*
 * Copyright (C) 2006 Astrient Labs, LLC Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance 
 * with the License. You may obtain a copy of the License at 
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the 
 * License for the specific language governing permissions and limitations under
 * the License.
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.threads;

import java.util.Enumeration;
import java.util.Vector;

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

	public Runnable getNextJob() {
		semaphore.acquire();
		if (!isEmpty()) {
			Object o = this.firstElement();
			removeElement(o);
			return (Runnable) o;
		}

		return null;
	}

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