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
		//		init();
	}

	int deltaTime = 30000;
	/**
	 * 下次启动的时间
	 */
	long nextScheduledTime = 0;

	private void init() {
		DbUtils.init("rs_winksDemo");
		int id = DbUtils.add();
		form.append(id + "");
		if (DbUtils.existsRecord(Constants.RMS_NEXT_SCHEDULED_TIME)) {

			register();
			form.append("first: " + nextScheduledTime);

			register();
			form.append("write to rms: " + nextScheduledTime);
			DbUtils.writeLong(Constants.RMS_NEXT_SCHEDULED_TIME,
					nextScheduledTime);
			RMSAnalyzer.analyzeAllRecordStores();
		}

		nextScheduledTime = DbUtils.getLong(Constants.RMS_NEXT_SCHEDULED_TIME);
		form.append("rms: " + nextScheduledTime);

	}

	protected void destroyApp(boolean unconditional)
			throws MIDletStateChangeException {
		register();
		notifyDestroyed();
	}

	protected void pauseApp() {

	}

	protected void startApp() throws MIDletStateChangeException {
		//		long now = System.currentTimeMillis();
		//		System.out.println("now: " + now);
		//		System.out.println("next: " + nextScheduledTime);
		//		System.out.println("cha: " + (nextScheduledTime - now));
		//		if (FileUtils.exists("e:/tests/a1.txt"))
		//			FileUtils.delete("e:/tests/a1.txt");
		if (isTimerUp) {
			display.setCurrent(null);
			display = null;
			form = null;

			new Thread() {
				public void run() {
					work();
				}
			}.start();
		} else {

			display.setCurrent(form);
			//			new Thread() {
			//				public void run() {
			//					work();
			//				}
			//			}.start();

			display.flashBacklight(1000);
			display.vibrate(1000);
			startTimer();
		}
	}

	// ///////////////////

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
		} catch (SecurityException e) {
			notifyDestroyed();
		} catch (ConnectionNotFoundException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		return nextScheduledTime;
	}

	void work() {
		try {
			HttpUtil.init("www.google.com", false);
			byte[] data = HttpUtil.get();
			HttpUtil.close();
			form.append(new String(data));

			FileUtils.writeToFile("e:/tests/a1.txt", data);

			exit();
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
	int i = 0;

	private void startTimer() {
		task = new TimerTask() {
			public void run() {
				if (i > 10) {
					stopTimer();

					form.append("程序即将退出, 30秒后重新启动");
					try {
						Thread.sleep(3000);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					exit();
				}
				form.deleteAll();
				String imei = System.getProperty("com.sonyericsson.imei");
				String imsi = System
						.getProperty("com.sonyericsson.sim.subscribernumber");
				form.append("imei: " + imei);
				form.append("imsi: " + imsi);
				form.append("" + i++);
			}
		};

		timer.schedule(task, 1000, 1000);
	}

	private void stopTimer() {
		if (task != null) {
			task.cancel();
			task = null;
		}
		if (timer != null) {
			// System.out.println("Stopping the timer");
			// form.append("Stopping the timer");
			timer.cancel();
			timer = null;
		}
	}

}
