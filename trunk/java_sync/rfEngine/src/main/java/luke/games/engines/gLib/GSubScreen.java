package luke.games.engines.gLib;

/**
 * 屏幕接口<br>
 * TODO 其中定义了按键值,屏幕类型,切屏时的效果类型,logic和render方法<br>
 * 所有游戏中的屏都实现该接口
 */
import javax.microedition.lcdui.Graphics;

public interface GSubScreen {
	// 按键常量
	static final public int GAME_KEY_ANY = 0xffffffff;
	static final public int GAME_KEY_0 = 1;
	static final public int GAME_KEY_1 = 2;
	static final public int GAME_KEY_2 = 4;
	static final public int GAME_KEY_3 = 8;
	static final public int GAME_KEY_4 = 16;
	static final public int GAME_KEY_5 = 32;
	static final public int GAME_KEY_6 = 64;
	static final public int GAME_KEY_7 = 128;
	static final public int GAME_KEY_8 = 256;
	static final public int GAME_KEY_9 = 512;
	static final public int GAME_KEY_POUND = 1024;
	static final public int GAME_KEY_SHARP = 1024;
	static final public int GAME_KEY_STAR = 2048;
	static final public int GAME_KEY_UP = 4096;
	static final public int GAME_KEY_DOWN = 8192;
	static final public int GAME_KEY_LEFT = 16384;
	static final public int GAME_KEY_RIGHT = 32768;
	static final public int GAME_KEY_A = 65536;
	static final public int GAME_KEY_B = 131072;
	static final public int GAME_KEY_C = 262144;
	static final public int GAME_KEY_SEND = 524288;
	// --------------------------------------------------------------------------------------------
	static final public int SCREEN_TYPE_NONE = 0;
	static final public int SCREEN_TYPE_LOGO = 1;
	static final public int SCREEN_TYPE_MENU = 2;
	static final public int SCREEN_TYPE_HELP = 3;
	static final public int SCREEN_TYPE_TOP10 = 4;
	static final public int SCREEN_TYPE_ABOUT = 5;
	static final public int SCREEN_TYPE_MAIN = 6;
	static final public int SCREEN_TYPE_OVER = 7;
	static final public int SCREEN_TYPE_OPTION = 8;
	static final public int SCREEN_TYPE_CONFIG = 9;
	static final public int SCREEN_TYPE_TITLE = 10;
	static final public int SCREEN_TYPE_STORY = 11;
	static final public int SCREEN_TYPE_MAIN2 = 12;
	// -----------------------------------------------------------------------------------------------------
	static final public int TRANSITION_TYPE_COUNT = 4;
	static final public int TRANSITION_TYPE_NONE = 0;
	static final public int TRANSITION_TYPE_RANDOM_IN = 1;
	static final public int TRANSITION_TYPE_RANDOM_OUT = 2;
	static final public int TRANSITION_TYPE_ALPHA_IN = 3;
	static final public int TRANSITION_TYPE_ALPHA_OUT = 4;
	static final public int TRANSITION_TYPE_BLOCK_IN = 5;
	static final public int TRANSITION_TYPE_BLOCK_OUT = 6;
	static final public int TRANSITION_TYPE_CURTAIN_V_IN = 7;
	static final public int TRANSITION_TYPE_CURTAIN_V_OUT = 8;
	static final public int TRANSITION_TYPE_CURTAIN_H_IN = 9;
	static final public int TRANSITION_TYPE_CURTAIN_H_OUT = 10;

	// -------------------------------------------------------------------------------------------------------
	public void Logic();

	public void Render(Graphics g);
}