package luke.games.engines.tests;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

import luke.games.engines.gLib.GUtil;

import luke.games.engines.pLib.math.Vector2f;

/** 子弹跟踪 */
public class ScreenTest1 implements SubScreen {
	// ---------------------------------------按键变量---------
	final int keyLeft = GAME_KEY_LEFT | GAME_KEY_4;
	final int keyRight = GAME_KEY_RIGHT | GAME_KEY_6;
	final int keyUp = GAME_KEY_UP | GAME_KEY_2;
	final int keyDown = GAME_KEY_DOWN | GAME_KEY_8;
	final int keyAction = GAME_KEY_C | GAME_KEY_5;
	final int keyAny = keyLeft | keyRight | keyUp | keyDown | keyAction
			| GAME_KEY_SHARP | GAME_KEY_STAR;

	/** image */
	Image ball_1 = null;// player
	Image ball_2 = null;// bullet

	/** 构造 */
	ScreenTest1() {
		GameCanvas.keyEnable = false;
		GameCanvas.logicEnable = false;
		GameCanvas.renderEnable = false;
		init();
		GameMIDlet.TIMER_PRE_FRAME = 50;// 50 = 20 fps
		GameCanvas.Timer = 0;
		GameCanvas.keyEnable = true;
		GameCanvas.logicEnable = true;
		GameCanvas.renderEnable = true;
	}

	// ---------------------------------------逻辑计算---------
	static Vector2f player = null;// 玩家位置分量-向量
	static Vector2f bullet = null;// 导弹位置分量-向量
	static Vector2f bullet_v = null;// 导弹的速度分量-向量
	float v_rate; // 导弹的速率缩放比率

	/** 子弹跟踪--计算速度向量,改变子弹坐标,逼近目标 */
	private void tail() {
		// 计算一下玩家与导弹之间的位置向量
		bullet_v = Vector2f.sub(player, bullet); // 向量减法，得到子弹方向(由导弹指向玩家)

		// 归一化导弹的速度向量
		bullet_v.normalise();// 得x就是这个向量与X轴的夹角的余弦值，而y就是正弦值

		// 此时导弹的速率为1，注意这里用速率
		// 导弹的速度分量满足x^2+y^2=1
		// 好！现在导弹的速度方向已经被修正，它指向玩家
		// 由于现在的导弹速度太快，要给导弹减速
		v_rate = 1.8f; // 减速比率
		bullet_v.scale(v_rate);// 这里的速率缩放比率，你可以任意调整大小.可以加速：v_rate大于1；减速v_rate大于0小于1

		// 导弹行进！导弹勇敢的冲向玩家
		bullet.add(bullet_v);
		GUtil.print(bullet.toString());
		// 然后判断是否攻击成功
	}

	/** 目标自动躲避子弹 */
	private void shelter() {

	}

	/**
	 * 游戏初始化
	 * 
	 * @see tests.GSubScreen#init()
	 */
	public void init() {
		player = new Vector2f(150, 180);// 玩家位置分量-向量
		bullet = new Vector2f(10, 100);// 导弹位置分量-向量
		bullet_v = new Vector2f();
		initImg();
		GUtil.print("ScreenMain init() Free Memory = "
				+ (Runtime.getRuntime().freeMemory() / 1024) + "(K byte)");
		changeState(GAME_PLAYING);
		System.gc();
	}

	/** 初始化图片 */
	void initImg() {
		ball_1 = GUtil.loadImage("/img/ball_1.png");
		ball_2 = GUtil.loadImage("/img/ball_2.png");
		GUtil.print("ScreenMain initImg() Free Memory = "
				+ (Runtime.getRuntime().freeMemory() / 1024) + "(K byte)");
	}

	/** @see tests.GSubScreen#logic() */
	public void logic() {
		if (GameCanvas.isKeyHold(keyLeft)) {
			GUtil.print("keyLeft " + keyLeft);
			player.x -= 3;
			// player.sub(Vector2f.normalise(player));// 减去单位向量
		} else if (GameCanvas.isKeyHold(keyRight)) {
			player.x += 3;
		} else if (GameCanvas.isKeyHold(keyUp)) {
			player.y -= 3;
		} else if (GameCanvas.isKeyHold(keyDown)) {
			player.y += 3;
		}
		tail();
	}

	/** @see tests.GSubScreen#render(Graphics) */
	public void render(Graphics g) {
		switch (curState) {
		case GAME_PLAYING:
			g.setColor(0);
			g.fillRect(0, 0, GameCanvas.SCREEN_WIDTH, GameCanvas.SCREEN_HEIGHT);
			g.drawImage(ball_1, (int) player.x, (int) player.y, 20);
			g.drawImage(ball_2, (int) bullet.x, (int) bullet.y, 20);
		}
	}

	/** @see tests.GSubScreen#destroy() */
	public void destroy() {
		ball_1 = null;
		ball_2 = null;
	}

	// ---------------------------------------游戏状态变量---------
	final int GAME_GAMEINTRO = 0;
	final int GAME_DELETING = 1;
	final int GAME_LEVEL_UP = 2;
	final int GAME_LOSE = 3;
	final int GAME_SHOWTIP = 6;
	final int GAME_RETURN = 7;
	final int GAME_REFRES = 8;

	final int GAME_GAMEWIN = 94;
	final int GAME_PLAYING = 95;
	final int GAME_SHOWACTION = 96;
	final int GAME_TOP10 = 97;
	final int GAME_NOTE = 98;
	final int GAME_MENU = 99;
	final int GAME_PAUSE = 100;

	int curState = 0;

	/** @see tests.GSubScreen#changeState(int) */
	public void changeState(int newState) {
		curState = newState;
	}
}
