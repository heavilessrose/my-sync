package luke.games.engines.gLib.test;

import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

import luke.games.engines.gLib.GUtil;

final public class GameMIDlet extends MIDlet implements Runnable {

	static public int TIMER_PRE_FRAME = 100;
	static public GameMIDlet CurGameMIDlet = null;
	static public boolean Exit = false;
	static public ScreenController CurGameScreen;

	// ------------------------------------------------------------------------------------------
	public GameMIDlet() {
		GUtil.print("Total Memory = "
				+ (Runtime.getRuntime().totalMemory() / 1024) + "(K byte)");
		CurGameMIDlet = this;
		CurGameScreen = new ScreenController();
		Display.getDisplay(this).setCurrent(CurGameScreen);
		(new Thread(this)).start();
	}

	final public void startApp() {
		GUtil.print("App Start Free Memory = "
				+ (Runtime.getRuntime().freeMemory() / 1024) + "(K byte)");
	}

	final public void pauseApp() {
	}

	final public void destroyApp(boolean unconditional) {
		GUtil.print("App End Free Memory = "
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