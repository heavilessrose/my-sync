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
package com.astrientlabs.net;

import java.util.NoSuchElementException;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Vector;

import com.astrientlabs.ad.AdCache;
import com.astrientlabs.threads.JobRunner;
import com.astrientlabs.ui.AdWindow;
import com.astrientlabs.util.TimeoutConnection;

public final class Network extends TimerTask {
	public static final Network instance = new Network();

	private Timer timer;
	private Vector connections = new Vector();
	private Vector cancellables = new Vector();

	private TimeoutConnection temp;

	private Network() {
		timer = new Timer();
		timer.scheduleAtFixedRate(this, 0, 1000 * 15);
	}

	public static void preOp() {

	}

	public static void postOp() {
		// TODO: rework add logic
		AdWindow.instance.setMessage("Updating ad cache");
		AdCache.instance.adHeight = AdWindow.instance.adWidget
				.getWidgetHeight();
		AdCache.instance.adWidth = AdWindow.instance.adWidget.getWidgetWidth();
		JobRunner.instance.run(AdCache.instance);
	}

	public static void setTimeout(TimeoutConnection timeoutConnection) {
		instance.connections.addElement(timeoutConnection);
	}

	public static void clearTimeout(TimeoutConnection timeoutConnection) {
		instance.connections.removeElement(timeoutConnection);
	}

	public void run() {
		try {
			// TODO: bug, if items shift, get it on next run()
			for (int i = 0; i < connections.size(); i++) {
				temp = (TimeoutConnection) connections.elementAt(i);
				if (temp.getTimeout() < System.currentTimeMillis())
					temp.close();
			}

			// TODO: check all ops that should be cancelled shouldCancel() ==
			// true
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void add(Cancellable cancellable) {
		cancellables.addElement(cancellable);
	}

	public void remove(Cancellable cancellable) {
		cancellables.removeElementAt(0);
	}

	public void cancelAll() {
		try {
			synchronized (cancellables) {
				// TODO: while ( connections isEmpty )
				for (int i = 0, stop = cancellables.size(); i < stop; i++) {
					// TODO: must not quit, find better pattern here
					try {
						((Cancellable) cancellables.firstElement()).cancel();
						cancellables.removeElementAt(0);
					} catch (NoSuchElementException e) {
						break;
					} catch (Throwable t) {
						t.printStackTrace();
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
