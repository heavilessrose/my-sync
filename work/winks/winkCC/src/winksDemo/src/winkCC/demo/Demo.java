package winkCC.demo;

import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import javax.microedition.io.ConnectionNotFoundException;
import javax.microedition.io.PushRegistry;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Form;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

public class Demo extends MIDlet {

	protected static Display display;
	private boolean firstStarted = false;
	private Thread thread;
	private int timerInterval;
	private Timer timer;
	private TimerTask task;

	Form form = new Form("test");

	public Demo() {
		System.out.println("Constructor executed");

		display = Display.getDisplay(this);
		String interval = getAppProperty("Timer-Interval");
		timerInterval = Integer.parseInt(interval);
		System.out.println("Timer interval is " + interval);
	}

	protected void startApp() throws MIDletStateChangeException {
		display.setCurrent(form);

		// In all cases, start a background thread.
		synchronized (this) {
			if (thread == null) {
				thread = new Thread() {
					public void run() {
						System.out.println("Thread running");
						form.append("Thread running");
						while (thread == this) {
							try {
								Thread.sleep(1500);
								System.out.println("Thread still active");
								form.append("Thread still active");
							} catch (InterruptedException ex) {
							}
						}
						System.out.println("Thread terminating");
						form.append("Thread terminating");
					}
				};
			}
		}
		if (!firstStarted) {
			firstStarted = true;
			thread.start();
			System.out.println("startApp called for the first time");
			form.append("startApp called for the first time");
			startTimer();
		} else {
			System.out.println("startApp called following pause");
			form.append("startApp called following pause");
		}
	}

	protected void pauseApp() {
		System.out.println("pauseApp called.");
		form.append("pauseApp called.");
		// synchronized (this) {
		// if (thread != null) {
		// thread = null;
		// }
		// }
	}

	protected void destroyApp(boolean unconditional)
			throws MIDletStateChangeException {
		System.out.println("destroyApp called - unconditional = "
				+ unconditional);
		form.append("destroyApp called - unconditional = ");
		if (thread != null) {
			Thread bgThread = thread;
			thread = null; // Signal thread to die
			try {
				bgThread.join();
				System.err.println("thread is die");
				form.append("thread is die");
			} catch (InterruptedException ex) {
			}
		}
		stopTimer();

		// 定时启动
		try {
			register();
		} catch (ConnectionNotFoundException e) {
			e.printStackTrace();
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}

	private void startTimer() {
		task = new TimerTask() {
			private boolean isPaused;
			private int count;

			public void run() {
				// Pause or resume the MIDlet.
				System.out.println("Timer scheduled");
				form.append("Timer scheduled");
				if (count++ == 2) {
					// Terminate the MIDlet
					// try {
					// MidpAppTest.this.destroyApp(true);
					// } catch (MIDletStateChangeException ex) {
					// // Ignore pleas for mercy!
					// }
					// MidpAppTest.this.notifyDestroyed();
					exit();
					return;
				}
				if (isPaused) {
					System.out.println(">> Resuming 恢复MIDlet");
					form.append(">> Resuming 恢复MIDlet");
					// MidpAppTest.this.resumeRequest();
					resume();
					isPaused = false;
				} else {
					System.out.println(">> Pausing 暂停MIDlet");
					form.append(">> Pausing 暂停MIDlet");
					isPaused = true;
					// MidpAppTest.this.pauseApp();
					// MidpAppTest.this.notifyPaused();
					pause();
				}
			}
		};

		timer = new Timer();
		timer.schedule(task, timerInterval, timerInterval);
		System.out.println("Timer started.");
	}

	private void stopTimer() {
		if (timer != null) {
			System.out.println("Stopping the timer");
			form.append("Stopping the timer");
			timer.cancel();
		}
	}

	/**
	 * 主动退出程序.
	 */
	public final void exit() {
		try {
			display = null;
			destroyApp(false);
			notifyDestroyed();
		} catch (MIDletStateChangeException e) {
		}
	}

	/**
	 * 主动暂停程序. 禁止在pauseApp()中调用.
	 */
	public final void pause() {
		pauseApp();
		// 通知JAM,MIDlet希望转到paused state, 且JAM不会主动调用pauseApp().
		notifyPaused();
	}

	/**
	 * 希望回到活动状态(JAM会在此调用后安排MIDlet回到活动状态并调用startApp()).</br>
	 * 通常由一个,在MIDlet处于暂停时仍处于活动状态的后台线程或定时器调用.
	 */
	public final void resume() {
		// 通知JAM,MIDlet希望从Paused state转到Active state.
		resumeRequest();
	}

	// ///////////////////
	int deltaTime = 10 * 1000;// 1分钟

	// int waitTime =

	// 注册定时启动
	void register(/* String waitTime */) throws ClassNotFoundException,
			ConnectionNotFoundException, SecurityException {
		String className = this.getClass().getName();
		// 当前时间
		Date alarm = new Date();
		long t = PushRegistry.registerAlarm(className, alarm.getTime()
				+ deltaTime);

	}
}
