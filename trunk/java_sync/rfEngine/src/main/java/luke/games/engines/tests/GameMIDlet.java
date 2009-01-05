package luke.games.engines.tests;

import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

final public class GameMIDlet extends MIDlet implements Runnable {

	static public int TIMER_PRE_FRAME = 100;// 每帧至少要花费的时间
	static public int MAX_FPS = 1000 / TIMER_PRE_FRAME;// 最大FPS
	static public GameMIDlet CurGameMIDlet = null;
	static public boolean Exit = false;
	static public GameCanvas CurGameScreen;

	public GameMIDlet() {
		System.out.println("Total Memory = "
				+ (Runtime.getRuntime().totalMemory() / 1024) + "(K byte)");
		CurGameMIDlet = this;
		CurGameScreen = new GameCanvas();
		Display.getDisplay(this).setCurrent(CurGameScreen);
		(new Thread(this)).start();
	}

	final public void startApp() {
		System.out.println("App Start Free Memory = "
				+ (Runtime.getRuntime().freeMemory() / 1024) + "(K byte)");
	}

	final public void pauseApp() {
	}

	final public void destroyApp(boolean unconditional) {
		System.out.println("App End Free Memory = "
				+ (Runtime.getRuntime().freeMemory() / 1024) + "(K byte)");
		notifyDestroyed();
	}

	final public void run() {
		long UsedTime;
		try {
			while (!Exit) {
				UsedTime = System.currentTimeMillis();
				if (CurGameScreen != null) {
					CurGameScreen.update();
				}
				UsedTime = System.currentTimeMillis() - UsedTime;
				if (UsedTime < TIMER_PRE_FRAME) {
					Thread.sleep(TIMER_PRE_FRAME - UsedTime);
				}
			}
		} catch (Exception err) {
			err.printStackTrace();
		}
		destroyApp(true);
	}
}