package winkCC.demo;

import java.io.IOException;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import javax.microedition.io.ConnectionNotFoundException;
import javax.microedition.io.PushRegistry;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Form;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

import winkCC.fs.FileUtils;
import winkCC.net.http.HttpUtil;
import winkCC.rms.DbUtils;
import winkCC.rms.RMSAnalyzer;

public class WinksDemo extends MIDlet {

	Display display = null;
	Form form = new Form("test");
	boolean isTimerUp = false;

	public WinksDemo() {
		display = Display.getDisplay(this);
	}

	private void init() {

		register();
		register();
		DbUtils.init("rs_winksDemo");
		DbUtils.add(Constants.NEXT_SCHEDULED_TIME);
		DbUtils.writeLong(Constants.NEXT_SCHEDULED_TIME, nextScheduledTime);
		RMSAnalyzer.analyzeAllRecordStores();
	}

	protected void destroyApp(boolean unconditional)
			throws MIDletStateChangeException {
		// register();
		notifyDestroyed();
	}

	protected void pauseApp() {

	}

	protected void startApp() throws MIDletStateChangeException {
		long now = System.currentTimeMillis();
		System.out.println("now: " + now);
		System.out.println("next: " + nextScheduledTime);
		System.out.println("cha: " + (nextScheduledTime - now));
		if (FileUtils.exists("e:/tests/a1.txt"))
			FileUtils.delete("e:/tests/a1.txt");
		if (isTimerUp) {
			new Thread() {
				public void run() {
					work();
				}
			}.start();
		} else {
			display.setCurrent(form);
			new Thread() {
				public void run() {
					work();
				}
			}.start();
		}
	}

	// ///////////////////
	int deltaTime = 30 * 1000;
	/**
	 * 下次启动的时间
	 */
	long nextScheduledTime = 0;

	// int waitTime =

	// 注册定时启动
	long register(/* String waitTime */)/*
									 * throws ClassNotFoundException,
									 * ConnectionNotFoundException,
									 * SecurityException
									 */{
		String className = this.getClass().getName();
		Date alarm = new Date();
		try {
			nextScheduledTime = PushRegistry.registerAlarm(className, alarm
					.getTime()
					+ deltaTime);
			System.out.println(nextScheduledTime);
			form.append("" + nextScheduledTime);
		} catch (ConnectionNotFoundException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		return nextScheduledTime;
	}

	void work() {
		HttpUtil net = new HttpUtil();
		try {
			net.init("www.google.com", false);
			byte[] data = net.get();
			net.close();
			form.append(new String(data));

			FileUtils.writeToFile("e:/tests/a1.txt", data);

			// exit();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public final void exit() {
		try {
			display = null;
			destroyApp(false);
			notifyDestroyed();
		} catch (MIDletStateChangeException e) {
		}
	}

	// //////////////
	Timer timer = new Timer();
	private TimerTask task;

	private void startTimer() {
		final Date date = new Date();
		task = new TimerTask() {
			public void run() {
				long cur = date.getTime();
				System.out.println(cur);
				if (nextScheduledTime == cur) {
					register();
				}
			}
		};

		// timer.schedule(task, firstTime, period);
	}

	private void stopTimer() {
		if (timer != null) {
			// System.out.println("Stopping the timer");
			// form.append("Stopping the timer");
			timer.cancel();
		}
	}

}
