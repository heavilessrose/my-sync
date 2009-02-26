package winkCC.core.midp;

import java.util.Timer;
import java.util.Vector;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Display;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

import winkCC.core.event.IEventListener;

public abstract class MidpApp extends MIDlet implements IEventListener,
		CommandFactory {

	// 线程池
	private int poolCap = 2;
	private Vector threadPool = new Vector(poolCap);

	private void initThreadPool() {
		for (int i = 0; i < poolCap; i++) {
			threadPool.addElement(new Thread());
		}
	}

	// --------------------------------
	/*
	 * 每个Timer有自己的一个后台线程(线程安全的)来顺序的执行该timer上的所以任务(任务应该能迅速的返回). 多个线程可共享一个Timer,
	 * 因为Timer是线程安全的.
	 */
	private Timer timer;

	// --------------------------------

	protected static MidpApp instance;
	protected Display display;

	protected static Command backCommand;
	protected static Command helpCommand;
	protected static Command exitCommand;
	protected static Command cancelCommand;
	protected static Command okCommand;
	protected static Command stopCommand;
	protected static Command homeCommand;

	// --------------------------------
	/**
	 * Protected constructor for subclasses. The application management software
	 * is responsible for creating MIDlets and creation of MIDlets is
	 * restricted. MIDlets should not attempt to create other MIDlets.</br>
	 * 
	 * 根据MIDP规范, MIDlet只能在首次调用startApp()是访问对应于屏幕的Display对象.
	 * 因此严格遵循规范的话涉及Display对象的初始化工作不能在构造方法中完成.
	 * 
	 * @throws SecurityException
	 *             unless the application management software is creating the
	 *             MIDlet.
	 */
	protected MidpApp(final int spalashSize) throws SecurityException {

		instance = this;
		display = Display.getDisplay(this);
		// loadProperties();
		// showSplash(bigSplashSize);
		initCommands();
	}

	public static MidpApp getInstance() {
		return instance;
	}

	public void initCommands() {
		backCommand = new Command("Back", Command.BACK, 1);
		helpCommand = new Command("Help", Command.HELP, 2);
		exitCommand = new Command("Exit", Command.EXIT, 1);
		okCommand = new Command("OK", Command.OK, 1);
		stopCommand = new Command("Stop", Command.STOP, 1);
		homeCommand = new Command("Home", Command.SCREEN, 1);
		cancelCommand = new Command("Cancel", Command.CANCEL, 1);
	}

	/**
	 * 只在程序从Paused state到Active state时调用此方法.</br> 会产生两种错误, transient(一个临时错误条件) 和
	 * non-transient(非临时错误条件),
	 * 当产生non-transient错误是应调用notifyDestroyed().</br>应仔细区分是首次运行还是后来调用,避免资源被分配多次.
	 * 由于startApp()可能会多次调用, 所以startApp()应分配在pauseApp()中释放的资源.其他资源应尽量在构造方法中分配.
	 * 
	 * @throws MIDletStateChangeException
	 *             当产生transient错误时抛出MIDletStateChangeException,MIDlet重新回到Paused
	 *             state.
	 */
	// @Override
	protected abstract void startApp() throws MIDletStateChangeException;

	/**
	 * 当外部事件(如来电)来临时执行此方法,MIDlet进入Paused state,但后台线程不会自动终止,且定时器会保持活动.</br>
	 * 通常应释放所有资源(MIDlet会选择性终止后台线程和网络连接,在要求时可取消活动的定时器,但这是非强制的)并保存当前状态,待以后恢复运行.
	 * 
	 */
	// @Override
	protected abstract void pauseApp();

	/**
	 * 释放所有资源.终止所有后台线程,并停止任何活动的定时器.</br>
	 * 
	 * @param uncondition
	 *            true: 当主机需要终止一个MIDlet时(为无条件关闭,这种调用时MIDlet不能防止进程继续执行). </br>
	 *            false: MIDlet有机会选择不终止
	 *            (非强制关闭,通过抛出MIDletStateChangeException异常表明它希望继续执行).
	 */
	// @Override
	protected abstract void destroyApp(boolean uncondition)
			throws MIDletStateChangeException;

	/**
	 * 主动退出程序. 禁止在destroyApp()中调用
	 */
	public final void exit() {
		try {
			display = null;
			// 释放资源.
			destroyApp(false);
			// 通知JAM销毁MIDlet, JAM不会调用destroyApp(),所以在这个方法前一般应主动调用destroyApp().
			// 如果MIDlet要资源终止则调用此方法是唯一的方式.
			notifyDestroyed();
		} catch (MIDletStateChangeException e) {
			// MIDlet并不希望关闭, 此时可能还有需要保存的数据等工作.
			System.out.println("MIDlet并不希望关闭, 此时可能还有需要保存的数据等工作.");
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
}
