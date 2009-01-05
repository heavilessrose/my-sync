package luke.games.engines.tests;


import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Graphics;

import luke.games.engines.gLib.GUtil;

public class GameCanvas extends Canvas implements SubScreen {

	static public Graphics screenGraphics = null;// 屏幕绘图Graphics
	static public boolean keyEnable = true;
	static public boolean logicEnable = true;
	static public boolean renderEnable = true;
	static public boolean alphaEnable = false;
	static public boolean transitionEnable = false;
	static public int transitionType = 0;
	static public int transitionTimer = 0;
	static public String transitionText = null;
	static public int Timer = 0;
	static public boolean pause = false;
	static int DIFFICULTY = 0;
	// ---------------------------------------屏幕参数---------
	static public int SCREEN_START_X;
	static public int SCREEN_START_Y;
	static public int SCREEN_WIDTH;
	static public int SCREEN_HEIGHT;
	static public int SCREEN_HCENTER;
	static public int SCREEN_VCENTER;

	static public int LOGIC_START_X;
	static public int LOGIC_START_Y;
	static public int LOGIC_WIDTH;
	static public int LOGIC_HEIGHT;

	protected void sizeChanged(int w, int h) {
		SCREEN_WIDTH = w;
		SCREEN_HEIGHT = h;
		SCREEN_HCENTER = SCREEN_WIDTH >> 1;// 屏幕中心点
		SCREEN_VCENTER = SCREEN_HEIGHT >> 1;
		LOGIC_WIDTH = (SCREEN_WIDTH >= 176) ? 176 : SCREEN_WIDTH;// 屏幕逻辑尺寸
		LOGIC_HEIGHT = (SCREEN_HEIGHT >= 208) ? 208 : SCREEN_HEIGHT;
		SCREEN_START_X = SCREEN_WIDTH >= LOGIC_WIDTH ? ((SCREEN_WIDTH - LOGIC_WIDTH) >> 1)
				: SCREEN_WIDTH - LOGIC_WIDTH;
		SCREEN_START_Y = SCREEN_HEIGHT >= LOGIC_HEIGHT ? ((SCREEN_HEIGHT - LOGIC_HEIGHT) >> 1)
				: SCREEN_HEIGHT - LOGIC_HEIGHT;
		LOGIC_START_X = SCREEN_START_X;// 屏幕起始点
		LOGIC_START_Y = SCREEN_START_Y;
		GUtil.print("W=" + SCREEN_WIDTH);
		GUtil.print("H=" + SCREEN_HEIGHT);
		GUtil.print("Size Changed !");
	}

	/** 构造方法 */
	GameCanvas() {
		setFullScreenMode(true);
		SCREEN_WIDTH = getWidth();
		SCREEN_HEIGHT = getHeight();
		sizeChanged(SCREEN_WIDTH, SCREEN_HEIGHT);
		SCREEN_HCENTER = SCREEN_WIDTH >> 1;
		SCREEN_VCENTER = SCREEN_HEIGHT >> 1;
		changeSubSreen(SCREEN_TYPE_MAIN);
	}

	// ---------------------------------------切换屏幕参数---------
	static private SubScreen curSubScreen = null;
	static private boolean doChangeScreen = false;
	static private int subScreenType = SCREEN_TYPE_NONE;
	static private boolean doChangeState = false;
	static private int nextStateType = 0;

	/** 切换屏幕的转换的方法 */
	final static public void changeSubSreen(int newSubScreenType) {
		keyEnable = false;
		clearKey();
		doChangeScreen = true;
		subScreenType = newSubScreenType;
	}

	/***/
	final static private void tryChangeSubSreen() {
		if (doChangeScreen) {
			keyEnable = false;
			clearKey();
			doChangeScreen = false;
			if (curSubScreen != null) {
				curSubScreen.destroy();
				curSubScreen = null;
			}
			System.gc();
			// clearCommands();
			switch (subScreenType) {
			// case SCREEN_TYPE_LOGO: {
			// CurSubScreen = new ScreenLogo();
			// break;
			// }
			// case SCREEN_TYPE_MENU : {
			// CurSubScreen = new ScreenMenu();
			// break;
			// }
			case SCREEN_TYPE_MAIN: {
				curSubScreen = new ScreenTest1();
				break;
			}
				// case SCREEN_TYPE_MAIN2: {
				// CurSubScreen = new ScreenMain2();
				// break;
				// }
			default: {
				curSubScreen = null;
			}
			}
			System.gc();
		} else if (doChangeState) {
			// clearCommands();
			doChangeState = false;
			curSubScreen.changeState(nextStateType);
			// setCommands();
		}
	}

	void update() {
		tryChangeSubSreen();
		repaint();
		serviceRepaints();
	}

	public void logic() {

	}

	public void render(Graphics g) {

	}

	protected void paint(Graphics g) {
		screenGraphics = g;
		if (curSubScreen != null) {
			if (keyEnable) {
				getKey();
			} else {
				clearKey();
			}
			if (logicEnable) {
				curSubScreen.logic();
			}
			if (renderEnable) {
				curSubScreen.render(g);
			}
		}
		Timer++;
	}

	// ---------------------------------------按键变量---------
	static private int keyState = 0;// 按键即时状态
	static private int keyDownState = 0;// 按键按下状态
	static private int keyUpState = 0;// 按键弹起状态
	static private int curKeyState = 0;// 按键即时状态快照
	static private int curKeyDownState = 0;// 按键按下状态快照
	static private int curKeyUpState = 0;// 按键弹起状态快照

	// ==============================================按键相关函数=============
	final static private int getKeyValue(int keyCode) {
		switch (keyCode) {
		case KEY_NUM0:
			return GAME_KEY_0;
		case KEY_NUM1:
			return GAME_KEY_1;
		case KEY_NUM2:
			return GAME_KEY_2;
		case KEY_NUM3:
			return GAME_KEY_3;
		case KEY_NUM4:
			return GAME_KEY_4;
		case KEY_NUM5:
			return GAME_KEY_5;
		case KEY_NUM6:
			return GAME_KEY_6;
		case KEY_NUM7:
			return GAME_KEY_7;
		case KEY_NUM8:
			return GAME_KEY_8;
		case KEY_NUM9:
			return GAME_KEY_9;
		case KEY_POUND:
			return GAME_KEY_POUND;
		case KEY_STAR:
			return GAME_KEY_STAR;
		case -1:
		case 1:
			return GAME_KEY_UP;
		case -2:
		case 6:
			return GAME_KEY_DOWN;
		case -3:
		case 2:
			return GAME_KEY_LEFT;
		case -4:
		case 5:
			return GAME_KEY_RIGHT;
		case -6:
		case 21:
			return GAME_KEY_A;
		case -7:
		case 22:
			return GAME_KEY_B;
		case -5:
		case 20:
			return GAME_KEY_C;

			// case KEY_SEND:
			// return GAME_KEY_SEND;
		default:
			return 0;
		}
	}

	final protected void keyPressed(int keyCode) {
		if (!keyEnable) {
			keyStateDestroy();
		} else {
			int Code = getKeyValue(keyCode);
			keyDownState |= Code;
			keyState |= Code;
		}
	}

	final protected void keyReleased(int keyCode) {
		if (!keyEnable) {
			keyStateDestroy();
		} else {
			int Code = getKeyValue(keyCode);
			keyUpState |= Code;
			keyState &= (~Code);
		}
	}

	// final protected void keyRepeated(int keyCode){
	// if(!KeyEnable){
	// KeyStateDestroy();
	// }else{
	// int Code = GetKeyValue(keyCode);
	// // KeyUpState |= Code;
	// KeyState = Code;
	// }
	// // if(!KeyEnable)return;
	// // int Code = GetKeyValue(keyCode);
	// // KeyState = Code;
	// }

	// final static private void KeyStateClear() {
	// KeyState = 0;
	// KeyDownState = 0;
	// KeyUpState = 0;
	// }

	final static private void keyStateDestroy() {
		keyState = 0;
		keyDownState = 0;
		keyUpState = 0;
	}

	final static private void getKey() {
		curKeyDownState = keyDownState;
		curKeyState = keyState;
		curKeyUpState = keyUpState;

		keyDownState = 0;
		keyUpState = 0;
		// KeyState = 0 ;
	}

	final static private void clearKey() {
		curKeyDownState = 0;
		curKeyState = 0;
		curKeyUpState = 0;
	}

	final static public boolean isKeyDown(int theKey) {
		return (curKeyDownState & theKey) != 0;
	}

	final static public boolean isKeyUp(int theKey) {
		return (curKeyUpState & theKey) != 0;
	}

	final static public boolean isKeyHold(int theKey) {
		return (curKeyState & theKey) != 0;
	}

	public void destroy() {
		// TODO Auto-generated method stub

	}

	public void changeState(int newState) {
		// TODO Auto-generated method stub

	}

	public void init() {
		// TODO Auto-generated method stub

	}
}
