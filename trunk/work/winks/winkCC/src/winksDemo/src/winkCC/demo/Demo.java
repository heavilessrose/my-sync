package winkCC.demo;

import java.util.Timer;
import java.util.TimerTask;

import javax.microedition.lcdui.Display;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

public class Demo extends MIDlet {

	protected static Display display;
	private boolean firstStarted = false;
	private Thread thread;
	private int timerInterval;
	private Timer timer;
	private TimerTask task;

	public Demo() {
		System.out.println("Constructor executed");

		String interval = getAppProperty("Timer-Interval");
		timerInterval = Integer.parseInt(interval);
		System.out.println("Timer interval is " + interval);
	}

	protected void startApp() throws MIDletStateChangeException {
		// In all cases, start a background thread.
		synchronized (this) {
			if (thread == null) {
				thread = new Thread() {
					public void run() {
						System.out.println("Thread running");
						while (thread == this) {
							try {
								Thread.sleep(500);
								System.out.println("Thread still active");
							} catch (InterruptedException ex) {
							}
						}
						System.out.println("Thread terminating");
					}
				};
			}
		}
		if (!firstStarted) {
			firstStarted = true;
			thread.start();
			System.out.println("startApp called for the first time");
			startTimer();
		} else {
			System.out.println("startApp called following pause");
		}
	}

	protected void pauseApp() {
		System.out.println("pauseApp called.");
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
		if (thread != null) {
			Thread bgThread = thread;
			thread = null; // Signal thread to die
			try {
				bgThread.join();
				System.err.println("thread is die");
			} catch (InterruptedException ex) {
			}
		}
		stopTimer();
	}

	private void startTimer() {
		task = new TimerTask() {
			private boolean isPaused;
			private int count;

			public void run() {
				// Pause or resume the MIDlet.
				System.out.println("Timer scheduled");
				if (count++ == 4) {
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
					System.out.println(">> Resuming �ָ�MIDlet");
					// MidpAppTest.this.resumeRequest();
					resume();
					isPaused = false;
				} else {
					System.out.println(">> Pausing ��ͣMIDlet");
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
			timer.cancel();
		}
	}

	/**
	 * �����˳�����. ��ֹ��destroyApp()�е���
	 */
	public final void exit() {
		try {
			display = null;
			// �ͷ���Դ.
			destroyApp(false);
			// ֪ͨJAM����MIDlet, JAM�������destroyApp(),�������������ǰһ��Ӧ��������destroyApp().
			// ���MIDletҪ��Դ��ֹ����ô˷�����Ψһ�ķ�ʽ.
			notifyDestroyed();
		} catch (MIDletStateChangeException e) {
			// MIDlet����ϣ���ر�, ��ʱ���ܻ�����Ҫ��������ݵȹ���.
			System.out.println("MIDlet����ϣ���ر�, ��ʱ���ܻ�����Ҫ��������ݵȹ���.");
		}
	}

	/**
	 * ������ͣ����. ��ֹ��pauseApp()�е���.
	 */
	public final void pause() {
		pauseApp();
		// ֪ͨJAM,MIDletϣ��ת��paused state, ��JAM������������pauseApp().
		notifyPaused();
	}

	/**
	 * ϣ���ص��״̬(JAM���ڴ˵��ú���MIDlet�ص��״̬������startApp()).</br>
	 * ͨ����һ��,��MIDlet������ͣʱ�Դ��ڻ״̬�ĺ�̨�̻߳�ʱ������.
	 */
	public final void resume() {
		// ֪ͨJAM,MIDletϣ����Paused stateת��Active state.
		resumeRequest();
	}
}
