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

	// �̳߳�
	private int poolCap = 2;
	private Vector threadPool = new Vector(poolCap);

	private void initThreadPool() {
		for (int i = 0; i < poolCap; i++) {
			threadPool.addElement(new Thread());
		}
	}

	// --------------------------------
	/*
	 * ÿ��Timer���Լ���һ����̨�߳�(�̰߳�ȫ��)��˳���ִ�и�timer�ϵ���������(����Ӧ����Ѹ�ٵķ���). ����߳̿ɹ���һ��Timer,
	 * ��ΪTimer���̰߳�ȫ��.
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
	 * ����MIDP�淶, MIDletֻ�����״ε���startApp()�Ƿ��ʶ�Ӧ����Ļ��Display����.
	 * ����ϸ���ѭ�淶�Ļ��漰Display����ĳ�ʼ�����������ڹ��췽�������.
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
	 * ֻ�ڳ����Paused state��Active stateʱ���ô˷���.</br> ��������ִ���, transient(һ����ʱ��������) ��
	 * non-transient(����ʱ��������),
	 * ������non-transient������Ӧ����notifyDestroyed().</br>Ӧ��ϸ�������״����л��Ǻ�������,������Դ��������.
	 * ����startApp()���ܻ��ε���, ����startApp()Ӧ������pauseApp()���ͷŵ���Դ.������ԴӦ�����ڹ��췽���з���.
	 * 
	 * @throws MIDletStateChangeException
	 *             ������transient����ʱ�׳�MIDletStateChangeException,MIDlet���»ص�Paused
	 *             state.
	 */
	// @Override
	protected abstract void startApp() throws MIDletStateChangeException;

	/**
	 * ���ⲿ�¼�(������)����ʱִ�д˷���,MIDlet����Paused state,����̨�̲߳����Զ���ֹ,�Ҷ�ʱ���ᱣ�ֻ.</br>
	 * ͨ��Ӧ�ͷ�������Դ(MIDlet��ѡ������ֹ��̨�̺߳���������,��Ҫ��ʱ��ȡ����Ķ�ʱ��,�����Ƿ�ǿ�Ƶ�)�����浱ǰ״̬,���Ժ�ָ�����.
	 * 
	 */
	// @Override
	protected abstract void pauseApp();

	/**
	 * �ͷ�������Դ.��ֹ���к�̨�߳�,��ֹͣ�κλ�Ķ�ʱ��.</br>
	 * 
	 * @param uncondition
	 *            true: ��������Ҫ��ֹһ��MIDletʱ(Ϊ�������ر�,���ֵ���ʱMIDlet���ܷ�ֹ���̼���ִ��). </br>
	 *            false: MIDlet�л���ѡ����ֹ
	 *            (��ǿ�ƹر�,ͨ���׳�MIDletStateChangeException�쳣������ϣ������ִ��).
	 */
	// @Override
	protected abstract void destroyApp(boolean uncondition)
			throws MIDletStateChangeException;

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
