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

import java.util.Vector;

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