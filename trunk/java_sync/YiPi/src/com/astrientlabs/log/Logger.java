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
package com.astrientlabs.log;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.util.Calendar;
import java.util.Vector;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;

import com.astrientlabs.midlet.Yipi;
import com.astrientlabs.threads.Semaphore;

public class Logger implements Runnable {
	public static final Logger instance = new Logger(15, "y!p!",
			"astrient:1e4e4d:-1985900");

	private static int sequenceNumberMajor = Calendar.getInstance().get(
			Calendar.DAY_OF_MONTH);
	private static int sequenceNumberMinor;

	private int flush;
	private int protocolID = 1;
	private String applicationName;
	private String APIKey;

	private Semaphore semaphore = new Semaphore(0);
	private Vector messages = new Vector();
	private Thread thread = null;
	private boolean running = false;
	private LogListener logListener;

	// ÎÒÉêÇëµÄyahoo application ID: v0btoX7V34HijBf5XX_g3i5skSapWJidi_PUGusP38A6_8oujmphDW1b7e8LqfPfGeKOeqBiKQ--
	public Logger(int flush, String applicationName, String APIKey) {
		this.flush = flush;
		this.applicationName = applicationName;
		this.APIKey = APIKey;

		(thread = new Thread(this)).start();
	}

	public void setListener(LogListener logListener) {
		this.logListener = logListener;
	}

	public void log(String source, Throwable t) {
		log(source, t + ": " + t.getClass().getName() + ": " + t.getMessage());
	}

	public void log(String source, String message) {
		log(source + ": " + message);
	}

	public void log(Throwable t) {
		t.printStackTrace();
		log(t.getClass().getName() + " " + t.getMessage());
	}

	public void log(String message) {
		// TODO: uncomment for debug
		System.out.println(message);
		if (logListener != null)
			logListener.log(message);

		messages.addElement(new LogMessage(message));

		if (messages.size() > flush) {
			semaphore.release();
		}
	}

	public void flush() {
		semaphore.release();
	}

	public void run() {
		while (thread != null) {
			semaphore.acquire();

			if (!running) {
				running = true;
				try {
					uploadMessages();
				} finally {
					running = false;
				}
			}
		}
	}

	protected void uploadMessages() {
		int count = messages.size();

		if (count > 0) {
			HttpConnection c = null;
			DataInputStream dis = null;
			DataOutputStream dos = null;

			String url = Yipi.instance.getAppProperty("server");
			if (url == null)
				url = "http://www.hostj2me.com/"; // url =
													// "http://127.0.0.1/j2me/";
			url += "applog";

			try {
				c = (HttpConnection) Connector.open(url, Connector.READ_WRITE);
				c.setRequestMethod(HttpConnection.POST);

				dos = c.openDataOutputStream();
				dos.writeLong(protocolID);
				dos.writeUTF(APIKey);
				dos.writeUTF(applicationName);
				dos.writeInt(count);

				LogMessage message;

				for (int i = 0; i < count; i++) {
					message = (LogMessage) messages.firstElement();
					messages.removeElement(message);

					dos.writeLong(message.timeStamp);
					dos.writeUTF("SEQN: " + sequenceNumberMajor + "."
							+ (sequenceNumberMinor++));
					dos.writeUTF(message.message);
				}

				dos.flush();

				dis = c.openDataInputStream();
				dis.readInt(); // response code, 0 = OK
			} catch (Throwable e) {
				e.printStackTrace();
			} finally {
				if (dis != null) {
					try {
						dis.close();
					} catch (Exception e) {
					}
				}
				if (dos != null) {
					try {
						dos.close();
					} catch (Exception e) {
					}
				}
				if (c != null) {
					try {
						c.close();
					} catch (Exception e) {
					}
				}
			}
		}
	}

	public void stop() {
		semaphore.release();
		thread = null;
	}
}