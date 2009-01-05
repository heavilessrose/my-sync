package luke.games.engines.gLib.test;

// import java.io.InputStream;
import java.util.Random;

import javax.microedition.lcdui.*;

import luke.games.engines.gLib.GGraphics;
import luke.games.engines.gLib.GUtil;

//import javax.microedition.lcdui.game.GameCanvas;
//import javax.microedition.lcdui.game.*;

public class ScreenController extends Canvas implements SubScreen {
	// 按键变量
	static private int KeyState = 0;// 按键即时状态,如果有按下的键就保存按下的键值,如果没有按下的键则这个值为0
	static private int KeyDowned = 0;// 记录按下的是哪个键
	static private int KeyUped = 0;// 记录弹起的是哪个键

	static private boolean PointerDragState = false;
	static private boolean PointerDownState = false;
	static private boolean PointerUpState = false;

	static private int CurKeyState = 0;// 按键即时状态
	static private int CurKeyDowned = 0;// 当前按下的按键
	static private int CurKeyUped = 0;// 当前弹起的按键
	static private boolean CurPointerDragState = false;
	static private boolean CurPointerDownState = false;
	static private boolean CurPointerUpState = false;

	static private SubScreen CurSubScreen = null;
	static private boolean DoChangeScreen = false; // 是否改变屏幕
	static private int SubScreenType = SCREEN_TYPE_NONE;// 屏幕类型(改变这个值再由TryChangeSubscreen()完成切换)

	static public Graphics ScreenGraphics = null;// 屏幕绘图Graphics
	// static public DirectGraphics ScreenDirectGraphics =
	// null;//屏幕绘图DirectGraphics

	static public boolean KeyEnable = true;// 是否接受按键
	static public boolean LogicEnable = true;// 
	static public boolean RenderEnable = true;
	// static public boolean AlphaEnable = false;
	static public boolean TransitionEnable = false;// 
	/** 切换效果类型* */
	static public int TransitionType = 0;
	static public int TransitionTimer = 0;
	static public String TransitionText = null;

	static public int Timer = 0;// 刷新的次数(每重绘一次加一)
	static public boolean PAUSE = false; // 是否暂停

	// ---------------屏幕物理参数-----------------------------------------------------------------------------
	static public int SCREEN_START_X;// 屏幕绘制起点x
	static public int SCREEN_START_Y;// 屏幕绘制起点y
	static public int SCREEN_WIDTH;// 屏幕物理宽度
	static public int SCREEN_HEIGHT;// 屏幕物理高度
	static public int SCREEN_HCENTER;// 屏幕水平中心坐标
	static public int SCREEN_VCENTER;// 屏幕垂直中心坐标
	// ---------------实际使用的屏幕参数-------------------------------------------------------------------------
	static public int LOGIC_START_X;// 屏幕绘制起点x
	static public int LOGIC_START_Y;// 屏幕绘制起点y
	static public int LOGIC_WIDTH;// 屏幕宽
	static public int LOGIC_HEIGHT;// 屏幕高

	// -------------------------------------------------------------------------------------------------------
	// 改变显示屏幕尺寸的大小
	protected void sizeChanged(int w, int h) {
		// SCREEN_WIDTH = w;
		// SCREEN_HEIGHT = h;
		// SCREEN_HCENTER = SCREEN_WIDTH >> 1;
		// SCREEN_VCENTER = SCREEN_HEIGHT >> 1;
		// LOGIC_WIDTH = (SCREEN_WIDTH >= 176) ? 176 : SCREEN_WIDTH;
		// LOGIC_HEIGHT = (SCREEN_HEIGHT >= 208) ? 208 : SCREEN_HEIGHT;
		// SCREEN_START_X = SCREEN_WIDTH >= LOGIC_WIDTH ? ((SCREEN_WIDTH -
		// LOGIC_WIDTH) >> 1): SCREEN_WIDTH - LOGIC_WIDTH;
		// SCREEN_START_Y = SCREEN_HEIGHT >= LOGIC_HEIGHT ? ((SCREEN_HEIGHT -
		// LOGIC_HEIGHT) >> 1):SCREEN_HEIGHT - LOGIC_HEIGHT;
		// LOGIC_START_X = GameCanvas.SCREEN_START_X;
		// LOGIC_START_Y = GameCanvas.SCREEN_START_Y;
		// CSUtil.print("W=" + SCREEN_WIDTH);
		// CSUtil.print("H=" + SCREEN_HEIGHT);
		// CSUtil.print("Size Changed !");
	}

	/**
	 * 构造函数<br>
	 * TODO 设置屏幕参数,系统环境测试
	 */
	ScreenController() {
		setFullScreenMode(true);

		SCREEN_WIDTH = getWidth();
		SCREEN_HEIGHT = getHeight();
		SCREEN_HCENTER = SCREEN_WIDTH >> 1;
		SCREEN_VCENTER = SCREEN_HEIGHT >> 1;
		LOGIC_WIDTH = (SCREEN_WIDTH >= 176) ? 176 : SCREEN_WIDTH;
		LOGIC_HEIGHT = (SCREEN_HEIGHT >= 208) ? 208 : SCREEN_HEIGHT;

		// 保持绘制窗口处在屏幕中心
		SCREEN_START_X = SCREEN_WIDTH >= LOGIC_WIDTH ? ((SCREEN_WIDTH - LOGIC_WIDTH) >> 1)
				: SCREEN_WIDTH - LOGIC_WIDTH;
		SCREEN_START_Y = SCREEN_HEIGHT >= LOGIC_HEIGHT ? ((SCREEN_HEIGHT - LOGIC_HEIGHT) >> 1)
				: SCREEN_HEIGHT - LOGIC_HEIGHT;

		LOGIC_START_X = ScreenController.SCREEN_START_X;
		LOGIC_START_Y = ScreenController.SCREEN_START_Y;

		GUtil.print("W=" + SCREEN_WIDTH);
		GUtil.print("H=" + SCREEN_HEIGHT);
		GUtil.print("profiles     :"
				+ System.getProperty("microedition.profiles"));
		GUtil.print("configuration:"
				+ System.getProperty("microedition.configuration"));
		GUtil.print("locale       :"
				+ System.getProperty("microedition.locale"));
		GUtil.print("platform     :"
				+ System.getProperty("microedition.platform"));
		GUtil.print("encoding     :"
				+ System.getProperty("microedition.encoding"));
		if (this.hasRepeatEvents())
			GUtil.print("support Hold Key !");
		if (this.isDoubleBuffered())
			GUtil.print("support Double Buffered !");
		if (this.hasPointerEvents())
			GUtil.print("support Pointer Events !");
		if (this.hasPointerMotionEvents())
			GUtil.print("support Pointer Dragged !");

		System.gc();

		ChangeSubSreen(SCREEN_TYPE_LOGO);
	}

	// --------------按键相关函数--------------------------------------------------------------------
	/**
	 * 将传入的设备键值转换为SubScreen定义的键值
	 * 
	 * @param keyCode
	 *            设备键值
	 */
	final static private int GetKeyValue(int keyCode) {
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
			/*
			 * case KEY_SEND: return GAME_KEY_SEND;
			 */
		default:
			return 0;
		}
	}

	// -------------------------------------------------------------------------------------
	/**
	 * Canvas键盘事件,当有按键按下时自动调用<br>
	 * 当按下某个键,使KeyDowned标记该按键
	 * 
	 * @param keyCode
	 *            设备传入的键值
	 */
	final protected void keyPressed(int keyCode) {
		if (!KeyEnable) {
			KeyStateClear();
		} else {
			int Code = GetKeyValue(keyCode);// 将设备传入的键值转换为subScreen定义的键值
			KeyDowned |= Code;
			KeyState |= Code;
			System.out.println("---------keyPressed -----");
			System.out.println("pressed code : " + Code);
			System.out.println("KeyDowned : " + KeyDowned);
			System.out.println("KeyState : " + KeyState);
		}
	}

	/**
	 * Canvas键盘事件,当有按键弹起时自动调用<br>
	 * 
	 * @param keyCode
	 *            设备传入的键值
	 */
	final protected void keyReleased(int keyCode) {
		if (!KeyEnable) {
			KeyStateClear();
		} else {
			int Code = GetKeyValue(keyCode);
			KeyUped |= Code;
			KeyState &= (~Code); // 设置为 0
			System.out.println("---------keyReleased -----");
			System.out.println("Code : " + Code);
			System.out.println("KeyUped : " + KeyUped);
			System.out.println("KeyState : " + KeyState);
		}
	}

	/**
	 * Canvas键盘连发事件,当有按键一直被按下时自动调用(有些机器不支持此事件)<br>
	 * 
	 * @param keyCode
	 *            设备传入的键值
	 */
	final protected void keyRepeated(int keyCode) {
		if (hasRepeatEvents()) {// 支持连发
			if (!KeyEnable) {
				KeyStateClear();
			} else {
				int Code = GetKeyValue(keyCode);
				// KeyUpState |= Code;
				KeyState = Code;
				System.out.println("---------keyRepeated -----");
				System.out.println("Code : " + Code);
				System.out.println("KeyState : " + KeyState);
			}
			// if(!KeyEnable)return;
			// int Code = GetKeyValue(keyCode);
			// KeyState = Code;
		}
	}

	/**
	 * 按键状态清零
	 */
	final static private void KeyStateClear() {
		KeyState = 0;
		KeyDowned = 0;
		KeyUped = 0;
	}

	/**
	 * 得到当前按键状态 .. 这套东西做的有点冗繁 以后考虑精简一下
	 */
	final static private void GetCurKey() {
		CurKeyDowned = KeyDowned;
		CurKeyUped = KeyUped;
		CurKeyState = KeyState;

		KeyDowned = 0;
		KeyUped = 0;
		// KeyState = 0 ;
	}

	/**
	 * 当前按键状态清零
	 */
	final static private void CurKeyClear() {
		CurKeyDowned = 0;
		CurKeyState = 0;
		CurKeyUped = 0;
	}

	/**
	 * 判断某个指定键是否被按下
	 * 
	 * @param TheKey
	 *            SubScreen中定义的键值!!非设备键值
	 * @return
	 */
	final static public boolean IsKeyDown(int TheKey) {
		return (CurKeyDowned & TheKey) != 0;
	}

	/**
	 * 判断某个指定键是否弹起
	 * 
	 * @param TheKey
	 * @return
	 */
	final static public boolean IsKeyUp(int TheKey) {
		return (CurKeyUped & TheKey) != 0;
	}

	/**
	 * 是否hold住某个按键
	 * 
	 * @param TheKey
	 * @return
	 */
	final static public boolean IsKeyHold(int TheKey) {
		return (CurKeyState & TheKey) != 0;
	}

	// -------------------------------------------------------------------------------------------------
	/**
	 * 尝试改变屏幕,由update不断尝试
	 */
	final static private void TryChangeSubSreen() {
		if (DoChangeScreen && !TransitionEnable) {
			KeyEnable = false;
			KeyStateClear();
			DoChangeScreen = false;
			CurSubScreen = null;
			System.gc();
			SetTransitionType(TRANSITION_TYPE_RANDOM_IN);
			switch (SubScreenType) {
			case SCREEN_TYPE_LOGO: {
				CurSubScreen = new ScreenLogo();
				break;
			}
				// case SCREEN_TYPE_MENU: {
				// CurSubScreen = new ScreenMenu();
				// break;
				// }
			case SCREEN_TYPE_MAIN: {
				CurSubScreen = new ScreenMain();
				break;
			}
			case SCREEN_TYPE_MAIN2: {
				CurSubScreen = new ScreenMain2();
				break;
			}
				// case SCREEN_TYPE_HELP: {
				// CurSubScreen = new ScreenHelp();
				// break;
				// }
				// case SCREEN_TYPE_ABOUT: {
				// CurSubScreen = new ScreenAbout();
				// break;
				// }
				// case SCREEN_TYPE_TITLE: {
				// CurSubScreen = new ScreenTitle();
				// break;
				// }
				// case SCREEN_TYPE_OVER: {
				// CurSubScreen = new ScreenGameOver();
				// break;
				// }
				// case SCREEN_TYPE_CONFIG: {
				// CurSubScreen = new ScreenConfig();
				// break;
				// }
			default: {
				CurSubScreen = null;
			}
			}
			System.gc();
			try {
				Thread.sleep(1);
			} catch (Exception e) {
			}
		}
	}

	/**
	 * 切换屏幕
	 * 
	 * @param NewSubScreenType
	 *            要切换到的屏幕
	 */
	final static public void ChangeSubSreen(int NewSubScreenType) {
		KeyEnable = false;
		KeyStateClear();
		DoChangeScreen = true;
		SubScreenType = NewSubScreenType;
		if (CurSubScreen != null) {
			SetTransitionType(TRANSITION_TYPE_RANDOM_OUT, "读取数据中!请稍后...");
		}
	}

	/**
	 * 切换屏幕,可另行设置切换时的提示信息
	 * 
	 * @param NewSubScreenType
	 * @param NewTransitionText
	 *            切换时的提示信息
	 */
	final static public void ChangeSubSreen(int NewSubScreenType,
			String NewTransitionText) {
		KeyEnable = false;
		KeyStateClear();
		DoChangeScreen = true;
		SubScreenType = NewSubScreenType;
		if (CurSubScreen != null) {
			SetTransitionType(TRANSITION_TYPE_RANDOM_OUT, NewTransitionText);
		}
	}

	/**
	 * 设置切屏间的效果
	 * 
	 * @param NewTransitionType
	 */
	final static private void SetTransitionType(int NewTransitionType) {
		TransitionEnable = true;
		TransitionType = NewTransitionType;
		TransitionTimer = 0;
		TransitionText = null;
		if (TransitionType == TRANSITION_TYPE_RANDOM_OUT) {
			TransitionType = TRANSITION_TYPE_ALPHA_OUT;
		} else if (TransitionType <= TRANSITION_TYPE_RANDOM_OUT) {
			TransitionType += (((Math.abs(new Random().nextInt()) % (TRANSITION_TYPE_COUNT - 1)) + 2) << 1);
		}
		if (TransitionType == TRANSITION_TYPE_ALPHA_IN) {
			TransitionTimer = 1;
		} else if (TransitionType == TRANSITION_TYPE_ALPHA_OUT
				|| TransitionType == TRANSITION_TYPE_BLOCK_OUT
				|| TransitionType == TRANSITION_TYPE_CURTAIN_V_OUT
				|| TransitionType == TRANSITION_TYPE_CURTAIN_H_OUT) {
			KeyEnable = false;
			LogicEnable = false;
			RenderEnable = false;
		}
	}

	/**
	 * 设置切屏间的效果及提示信息
	 * 
	 * @param NewTransitionType
	 * @param NewTransitionText
	 */
	final static private void SetTransitionType(int NewTransitionType,
			String NewTransitionText) {
		SetTransitionType(NewTransitionType);
		TransitionText = NewTransitionText;
	}

	// -------------------------------------------------------------------------------------------------

	final public void Logic() {

	}

	final public void Render(Graphics g) {

	}

	final protected void paint(Graphics g) {
		try {
			ScreenGraphics = g;
			// CSGraphics.setLCDGraphics(g);

			if (CurSubScreen != null) {
				if (KeyEnable) {
					GetCurKey();
					// getPointerState();
				} else {
					KeyStateClear();
					// pointerDestroy();
				}
				if (LogicEnable) {
					CurSubScreen.Logic();
				}
				if (RenderEnable) {
					CurSubScreen.Render(g);
				}

				Timer++;

				// 显示切屏效果
				if (TransitionEnable) {
					int clipCubeW = 16;
					boolean NeedWriteText = false;
					ClearClip();
					ScreenGraphics.setColor(0);
					switch (TransitionType) {
					case TRANSITION_TYPE_BLOCK_IN:
					case TRANSITION_TYPE_ALPHA_IN:
					case TRANSITION_TYPE_CURTAIN_H_IN:
					case TRANSITION_TYPE_CURTAIN_V_IN:
					case TRANSITION_TYPE_RANDOM_IN:
						for (int j = SCREEN_HEIGHT / clipCubeW; j >= 0; j--) {
							for (int i = SCREEN_WIDTH / clipCubeW; i >= 0; i--) {
								ScreenGraphics.fillRect(//
										i * clipCubeW + TransitionTimer,//
										j * clipCubeW + TransitionTimer,//
										clipCubeW - (TransitionTimer << 1),//
										clipCubeW - (TransitionTimer << 1));
							}
						}
						if (TransitionTimer << 1 >= clipCubeW) {
							TransitionEnable = false;
						}
						break;

					case TRANSITION_TYPE_BLOCK_OUT:
					case TRANSITION_TYPE_ALPHA_OUT:
					case TRANSITION_TYPE_CURTAIN_H_OUT:
					case TRANSITION_TYPE_CURTAIN_V_OUT:
					case TRANSITION_TYPE_RANDOM_OUT:
						for (int j = SCREEN_HEIGHT / clipCubeW; j >= 0; j--) {
							for (int i = SCREEN_WIDTH / clipCubeW; i >= 0; i--) {
								ScreenGraphics.fillRect(//
										i * clipCubeW + (clipCubeW>>1) - TransitionTimer, //
										j * clipCubeW + (clipCubeW>>1) - TransitionTimer,//
										TransitionTimer << 1,//
										TransitionTimer << 1);
							}
						}
						if (TransitionTimer << 1 > clipCubeW) {
							NeedWriteText = true;
							TransitionEnable = false;
						}
						break;

					// case TRANSITION_TYPE_CURTAIN_V_IN: {
					// ScreenGraphics.fillRect(0, 0, SCREEN_WIDTH,
					// (SCREEN_HEIGHT >> 1) - (TransitionTimer << 3));
					// ScreenGraphics.fillRect(0, (SCREEN_HEIGHT >> 1)
					// + (TransitionTimer << 3), SCREEN_WIDTH,
					// (SCREEN_HEIGHT >> 1) - (TransitionTimer << 3));
					// if (TransitionTimer << 3 >= SCREEN_HEIGHT >> 1) {
					// TransitionEnable = false;
					// }
					// break;
					// }
					// case TRANSITION_TYPE_CURTAIN_V_OUT: {
					// ScreenGraphics.fillRect(0, 0, SCREEN_WIDTH,
					// TransitionTimer << 3);
					// ScreenGraphics.fillRect(0, SCREEN_HEIGHT
					// - (TransitionTimer << 3), SCREEN_WIDTH,
					// TransitionTimer << 3);
					// if (TransitionTimer << 3 > SCREEN_HEIGHT >> 1) {
					// NeedWriteText = true;
					// TransitionEnable = false;
					// }
					// break;
					// }
					// case TRANSITION_TYPE_CURTAIN_H_IN: {
					// ScreenGraphics.fillRect(0, 0, (SCREEN_WIDTH >> 1)
					// - (TransitionTimer << 3), SCREEN_HEIGHT);
					// ScreenGraphics.fillRect((SCREEN_WIDTH >> 1)
					// + (TransitionTimer << 3), 0, (SCREEN_WIDTH >> 1)
					// - (TransitionTimer << 3), SCREEN_HEIGHT);
					// if (TransitionTimer << 3 >= SCREEN_WIDTH >> 1) {
					// TransitionEnable = false;
					// }
					// break;
					// }
					// case TRANSITION_TYPE_CURTAIN_H_OUT: {
					// ScreenGraphics.fillRect(0, 0, TransitionTimer << 3,
					// SCREEN_HEIGHT);
					// ScreenGraphics.fillRect(SCREEN_WIDTH
					// - (TransitionTimer << 3), 0, TransitionTimer << 3,
					// SCREEN_HEIGHT);
					// if (TransitionTimer << 3 > SCREEN_WIDTH >> 1) {
					// NeedWriteText = true;
					// TransitionEnable = false;
					// }
					// break;
					// }
					// case TRANSITION_TYPE_ALPHA_IN: {
					// CSGraphics.RenderRectAlpha(ScreenGraphics, 0, 0,
					// SCREEN_WIDTH, SCREEN_HEIGHT,
					// (256 - (TransitionTimer << 4)) << 24);
					// if (TransitionTimer << 4 >= 256) {
					// TransitionEnable = false;
					// }
					// break;
					// }
					// case TRANSITION_TYPE_ALPHA_OUT: {
					// CSGraphics.RenderRectAlpha(ScreenGraphics, 0, 0,
					// SCREEN_WIDTH, SCREEN_HEIGHT, TransitionTimer << 28);
					// NeedWriteText = true;
					// if (TransitionTimer << 4 >= 9 << 4) {
					// TransitionEnable = false;
					// }
					// break;
					// }
					}

					if (NeedWriteText && TransitionText != null) {
						GGraphics.drawString(ScreenGraphics, TransitionText,
								(SCREEN_WIDTH - GGraphics
										.getStringWidth(TransitionText)) >> 1,
								(SCREEN_HEIGHT - GGraphics.StringHeight) >> 1,
								0xffffff, 0, GGraphics.TEXT_STYLE_NONE);
					}
					TransitionTimer++;
				}
			}
		} catch (RuntimeException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	final public void update() {
		TryChangeSubSreen();
		repaint();
		serviceRepaints();
	}

	// -----------------------------------------------------------------------------------------------
	static final public void ClearClip() {
		ScreenGraphics.setClip(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	static final public void ClearScreen(int Color) {
		ScreenGraphics.setColor(Color);
		ScreenGraphics.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	static final public void ClearScreenAndClip(int Color) {
		ClearClip();
		ScreenGraphics.setColor(Color);
		ScreenGraphics.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	protected void hideNotify() {
		PAUSE = true;
	}

	protected void showNotify() {

	}

	// ------------触摸屏输入事件处理-----------------------------------------------------------------------
	final protected void pointerPressed(int x, int y) {
		PointerDownState = true;
		PointerDragState = false;
		PointerX = x;
		PointerY = y;
	}

	final protected void pointerReleased(int x, int y) {
		PointerUpState = true;
		PointerDragState = false;
		PointerX = x;
		PointerY = y;
	}

	final protected void pointerDragged(int x, int y) {
		PointerDragState = true;
		PointerX = x;
		PointerY = y;
	}

	final static private void pointerDestroy() {
		PointerDownState = false;
		PointerUpState = false;
		PointerDragState = false;
	}

	final static private void getPointerState() {
		CurPointerDownState = PointerDownState;
		CurPointerUpState = PointerUpState;
		CurPointerDragState = PointerDragState;
		PointerDownState = false;
		PointerUpState = false;
		PointerDragState = false;
	}

	static public int PointerX;
	static public int PointerY;

	final static public boolean isPointerDown() {
		return (CurPointerDownState);
	}

	final static public boolean isPointerUp() {
		return (CurPointerUpState);
	}

	final static public boolean isPointerDrag() {
		return (CurPointerDragState);
	}
	// -----------------------------------------------------------------------------------------------------------
}
