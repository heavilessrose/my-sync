package luke.games.engines.gLib.test;

import java.util.Random;
import javax.microedition.lcdui.Graphics;

import luke.games.engines.gLib.GCamera;
import luke.games.engines.gLib.GGraphics;
import luke.games.engines.gLib.GMap;
import luke.games.engines.gLib.GSoundPlayer;
import luke.games.engines.gLib.GSprite;
import luke.games.engines.gLib.GTileGroup;
import luke.games.engines.gLib.GUtil;
import luke.games.engines.gLib.GWorld;

/**
 * @author Luke
 * 
 * TODO 游戏演示2
 */
public class ScreenMain2 implements SubScreen {
	static GSoundPlayer soundman1;

	Random random = new Random();

	GWorld world; // 所有的东西都放到世界中
	GCamera camera[];// 摄象机组
	GMap map[];// 地图组
	GSprite spr[];// 主角的精灵

	int actorIndex = 0; // 主角的索引的编号

	// camera 初始化
	final int CameraX = ScreenController.LOGIC_START_X + 1;
	final int CameraY = ScreenController.LOGIC_START_Y + 1;
	final int CameraW = ScreenController.LOGIC_WIDTH - 2;
	final int CameraH = ScreenController.LOGIC_HEIGHT - 2;

	int G = 500;// 重力

	int stepH = 3;
	int jumpV = 12 * G;// default = 12*200，跳起来的速度
	int maxGSpeed = 30 * G; // 最大的速度

	public ScreenMain2() {
		GameMIDlet.TIMER_PRE_FRAME = 40;// 50 = 20 fps
		ScreenController.KeyEnable = false;
		ScreenController.LogicEnable = false;
		ScreenController.RenderEnable = false;
		ScreenController.Timer = 0;
		GUtil.print();
		GUtil.print("Init Begin : Total Memory = "
				+ (Runtime.getRuntime().totalMemory() / 1024) + "(K byte)");
		GUtil.print("Init Begin : Free  Memory = "
				+ (Runtime.getRuntime().freeMemory() / 1024) + "(K byte)");

		loadLevel();

		if (soundman1 == null) {
			soundman1 = new GSoundPlayer("/gLibTest/bgm/bgm01.mid",
					"audio/midi", -1);
			soundman1.setVolume(100);
			soundman1.play();
		}

		GUtil.print("Init End : Free Memory = "
				+ (Runtime.getRuntime().freeMemory() / 1024) + "(K byte)");
		GUtil.print();

		ScreenController.KeyEnable = true;
		ScreenController.LogicEnable = true;
		ScreenController.RenderEnable = true;
	}

	// ------------------------------------------------------------------------------------------------------
	public void Logic() {
		if (ScreenController.PAUSE) {
			if (soundman1.isPlaying()) {
				soundman1.pause();
			}
			if (ScreenController.IsKeyDown(GAME_KEY_A)) {
				ScreenController.ClearScreenAndClip(0);
				ScreenController.PAUSE = false;
			}
			if (ScreenController.IsKeyDown(GAME_KEY_B)) {
				ScreenController.PAUSE = false;
				soundman1.destroy();
				// ScreenMenu.putSave();
				ScreenController.ChangeSubSreen(SCREEN_TYPE_LOGO);
			}
			return;
		} else {
			if (!soundman1.isPlaying()) {
				soundman1.resume();
			}
			if (ScreenController.IsKeyDown(GAME_KEY_A | GAME_KEY_B)) {
				ScreenController.PAUSE = true;
			}
		}

		processActor(actorIndex);

		// 尽量使spr处于camera中心
		// 摄像机在世界中移动的相应像素
		int dx = spr[actorIndex].getHC()
				- (camera[0].HPos + (camera[0].CameraW >> 1));
		int dy = spr[actorIndex].getVC()
				- (camera[0].VPos + (camera[0].CameraH >> 1));
		// System.out.println("dx = " + dx);
		// System.out.println("spr[ActorIndex].getHC() = "
		// + spr[ActorIndex].getHC() + "\t camera[0].HPos = "
		// + camera[0].HPos + "\t camera[0].CameraW = " + camera[0].CameraW);
		world.movCamera(0, 0, dx, dy);

//		world.update();
	}

	// --------------------------------------------------------------------------------------------
	public void Render(Graphics g) {
		world.render(g, 0);
		// camera[0].renderDebugWindow(g, 32768);
		if (ScreenController.PAUSE) {
			ScreenController.ClearClip();
			GGraphics.drawString(ScreenController.ScreenGraphics, "继续",
					CameraX, CameraY + CameraH - GGraphics.StringHeight,
					0xffffffff, 0xff000000, GGraphics.TEXT_STYLE_AROUND);

			GGraphics.drawString(ScreenController.ScreenGraphics, "退出", CameraX
					+ CameraW - GGraphics.getStringWidth("退出"), CameraY
					+ CameraH - GGraphics.StringHeight, 0xffffffff, 0xff000000,
					GGraphics.TEXT_STYLE_AROUND);
		}

		GGraphics.drawString(ScreenController.ScreenGraphics, ""
				+ " TPF="
				+ GameMIDlet.TIMER_PRE_FRAME
				+ " FPS="
				+ (1000 / ((System.currentTimeMillis() - CurTime) == 0 ? 1
						: (System.currentTimeMillis() - CurTime))), CameraX,
				CameraY, 0xffffffff, 0xff000000, GGraphics.TEXT_STYLE_NONE);
		CurTime = System.currentTimeMillis();
	}

	long CurTime = 1000;

	/**
	 * 载入关卡
	 */
	private void loadLevel() {
		spr = new GSprite[1];
		spr[0] = createActor(0, 32, 32);// obj

		/**
		 * 地图数据
		 */
		byte[][] MapTileMatrix = {
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
						2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
						2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
						2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, };
		/**
		 * 地形数据--阻挡信息
		 */
		byte[][] MapTagMatrix = {
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
						2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
						2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
						2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, };

		GTileGroup mapTile = new GTileGroup(GUtil
				.loadImage("/gLibTest/map/map.png"), 3);
		mapTile.AddTile(12 * 0, 12 * 2, 12, 12);// tile 0
		mapTile.AddTile(12 * 0, 12 * 0, 12, 12);// tile 1
		mapTile.AddTile(12 * 2, 12 * 0, 12, 12);// tile 2
		mapTile.gc();

		map = new GMap[1];
		map[0] = new GMap(mapTile, MapTileMatrix, MapTagMatrix, 12, 12, 0, 0,
				12, 1, 0);

		camera = new GCamera[1];
		camera[0] = new GCamera(CameraW < map[0].getWidth() ? CameraX : CameraX
				+ ((CameraW - map[0].getWidth()) >> 1),//
				CameraH < map[0].getHeight() ? CameraY : CameraY
						+ ((CameraH - map[0].getHeight()) >> 1),//
				CameraW < map[0].getWidth() ? CameraW : map[0].getWidth(), //
				CameraH < map[0].getHeight() ? CameraH : map[0].getHeight(), //
				12, 12, 0, true);

		world = new GWorld();
		world.isDebug = true;
		world.addCamera(camera);
		world.addMap(map);
		world.addSprite(spr);
		world.init();
	}

	// ------------------------------------------------------------------------------------------------------------------
	// final int ACTOR_EVENT_1_STAND = 1<<0;
	// final int ACTOR_EVENT_1_DEAD = 1<<1;

	/**
	 * Events[0]
	 */
	final int ACTION_STANDING = 0;// 一直站立
	final int ACTION_WALKING = 1;// 一直走
	final int ACTION_WALK_STAND = 2;// 走->站立
	final int ACTION_STAND_DUCK = 3;// 站立->蹲
	final int ACTION_DUCK_STAND = 4;// 蹲->站立
	final int ACTION_TRANS_H = 5;// 转身
	final int ACTION_DUCKING = 6;// 一直蹲着
	final int ACTION_JUMPING = 7;// 跳
	final int ACTION_FALL_DOWN = 8;// 掉下
	final int ACTION_JUMP_STAND = 9;// 落地
	final int ACTION_DEAD = 10;// 挂
	/**
	 * Events[1]
	 */
	final int STATE_STAND = 1;// 站立
	// final int STATE_WALK = 2;//走
	// final int STATE_DUCK = 3;//蹲着
	final int STATE_JUMP = 4;// 空中
	final int STATE_DEAD = 5;
	/**
	 * Events[2]
	 */
	final int STATUS_GET_MAGICAL = 2;
	/**
	 * Events[3]
	 */
	final int STATUS_LAST_DIP = 3;

	/**
	 * 创建主角精灵
	 * 
	 * @param type
	 * @param x1
	 * @param y1
	 * @return
	 */
	private GSprite createActor(int type, int x1, int y1) {// 0,32,32
		int tileCount = 25;

		// 创建精灵用到的图片组
		GTileGroup tile = new GTileGroup(null, tileCount);
		for (int i = 0; i < tileCount; i++) {// 得到创建精灵的图片
			int d1 = i / 10;
			int d0 = i % 10;
			tile.SetTileImage(GUtil.loadImage("/gLibTest/actor/" + d1 + "" + d0
					+ ".png"));
			tile.AddTile();// 添加到图片数组中
		}

		// 创建精灵
		GSprite obj = new GSprite(tile, type, tileCount, 4);// tile,0,25,4;

		// 添加子精灵
		for (int i = 0; i < tileCount; i++) {// 添加25个子精灵
			obj.addSubspr(0, 0, i, 0);// 12
		}

		// 添加碰撞块
		// obj.setCDRect(0,4,5,0,12,30);
		obj.addCDRect(3, 6, 0, 10, 30);
		obj.addCDRect(3, 6, 29, 10, 1);
		obj.addCDRect(3, 6, 30, 10, 1);

		// 设置动画序列
		// obj.setAnimate(0,new byte[]{0});// 0 standing pass
		obj.addAnimate(new byte[] { 2, 2, 2, 2, 3, 3, 3, 3, 14, 14, 15, 15, 16,
				16, 17, 17, 18, 18, 19, 19, 12, 12, 13, 13, });// 1 walking
		// pass
		obj.addAnimate(new byte[] { 20, 20, 21, 21 });// 2 walk -> stop pass
		obj.addAnimate(new byte[] { 9, 9, 10, 10 });// 3 stand -> duck pass
		obj.addAnimate(new byte[] { 10, 10, 11, 11 });// 4 duck -> stand pass
		obj.addAnimate(new byte[] { 2, 2, 3, 3 });// 5 left or right -> trans
		// H undefined
		obj.addAnimate(new byte[] { 9, 9, 10 });// 6 ducking pass
		obj.addAnimate(new byte[] { 4, 5, 5, 6, 6, 7, 7, 8, 8 });// 7 jumping
		// pass
		obj.addAnimate(new byte[] { 4 });// 8 falldown pass
		// 9 jump ->stand pass
		obj.addAnimate(new byte[] { 1, 1, 1, 1, 0, 0, 0, 0 });
		obj.addAnimate(new byte[] { 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24,
				24 });// 10 dead pass

		// 事件及其他属性
		obj.Events = new int[4];
		obj.EventTime = new int[4];
		obj.Extra = new int[4];
		obj.setPos(x1, y1);
		obj.Extra[0] = x1;
		obj.Extra[1] = y1;

		obj.transFlip(GSprite.TRANS_H);
		return obj;
	}

	final int KeyLeft = GAME_KEY_LEFT | GAME_KEY_4 | GAME_KEY_1;
	final int KeyRight = GAME_KEY_RIGHT | GAME_KEY_6 | GAME_KEY_3;
	final int KeyJump = GAME_KEY_UP | GAME_KEY_5 | GAME_KEY_1 | GAME_KEY_2
			| GAME_KEY_3;
	final int KeyDuck = GAME_KEY_DOWN | GAME_KEY_8;

	/**
	 * 处理主角
	 * 
	 * @param index
	 */
	private void processActor(int index) {
		if (spr[index].Events[1] == STATE_STAND) {
			// On Stand
			spr[index].setCDRect(1, 3, 6, 0, 10, 30);
			if (world.collideMap(index, 1, 0, 1)) {
				spr[index].setCDRect(1, 3, 6, 15, 10, 15);
				spr[index].Events[0] = ACTION_DUCKING;
				spr[index].setCurrentFrame(spr[index].Events[0], 2);
				return;
			}

			// 'DOWN' hold
			if (ScreenController.IsKeyHold(KeyDuck)) {
				spr[index].Events[0] = ACTION_DUCKING;
				if (ScreenController.IsKeyHold(KeyLeft)) {
					if (spr[index].Flip == GSprite.TRANS_H) {
						spr[index].transFlip(GSprite.TRANS_H);
					}
				}
				if (ScreenController.IsKeyHold(KeyRight)) {
					if (spr[index].Flip == GSprite.TRANS_NONE) {
						spr[index].transFlip(GSprite.TRANS_H);
					}
				}
			} else {
				if (ScreenController.IsKeyDown(KeyLeft)) {
					spr[index].setCurrentFrame(ACTION_WALKING, 8);
					spr[index].Events[0] = ACTION_WALKING;
				}
				if (ScreenController.IsKeyHold(KeyLeft)) {
					world.tryMovSpr(index, 0, -stepH, 0);
					if (spr[index].Flip == GSprite.TRANS_H) {
						spr[index].transFlip(GSprite.TRANS_H);
						spr[index].setCurrentFrame(ACTION_WALKING, 0);
					}
					spr[index].Events[0] = ACTION_WALKING;
				}
				if (ScreenController.IsKeyUp(KeyLeft)) {
					spr[index].setCurrentFrame(ACTION_WALK_STAND, 0);
					spr[index].Events[0] = ACTION_WALK_STAND;
				}

				if (ScreenController.IsKeyDown(KeyRight)) {
					spr[index].setCurrentFrame(ACTION_WALKING, 8);
					spr[index].Events[0] = ACTION_WALKING;
				}
				if (ScreenController.IsKeyHold(KeyRight)) {
					world.tryMovSpr(index, 0, stepH, 0);
					if (spr[index].Flip == GSprite.TRANS_NONE) {
						spr[index].transFlip(GSprite.TRANS_H);
						spr[index].setCurrentFrame(ACTION_WALKING, 0);
					}
					spr[index].Events[0] = ACTION_WALKING;
				}
				if (ScreenController.IsKeyUp(KeyRight)) {
					spr[index].setCurrentFrame(ACTION_WALK_STAND, 0);
					spr[index].Events[0] = ACTION_WALK_STAND;
				}
			}
			// 'UP' push
			if (ScreenController.IsKeyHold(KeyJump)) {
				spr[index].SpeedY256 = -jumpV;
				spr[index].setCurrentFrame(ACTION_JUMPING, 0);
				spr[index].Events[0] = ACTION_JUMPING;
				spr[index].CDStatus[3] = 0;
				spr[index].CDStatus[2] = 0;
			}
			// 'DOWN' push
			if (ScreenController.IsKeyDown(KeyDuck)) {
				spr[index].setCurrentFrame(ACTION_STAND_DUCK, 0);
				spr[index].Events[0] = ACTION_STAND_DUCK;
			}
			// 'DOWN' release
			if (ScreenController.IsKeyUp(KeyDuck)) {
				spr[index].setCurrentFrame(ACTION_DUCK_STAND, 0);
				spr[index].Events[0] = ACTION_DUCK_STAND;
			}

			// 处理精灵的动作
			spr[index].setCurrentAnimate(spr[index].Events[0]);
			switch (spr[index].Events[0]) {
			case ACTION_STANDING:
				spr[index].nextCycFrame();
				break;
			case ACTION_WALKING:
				spr[index].nextCycFrame(8);
				break;
			case ACTION_DUCKING:
				spr[index].setCDRect(1, 3, 6, 15, 10, 15);
				spr[index].nextCycFrame(2);
				break;
			case ACTION_JUMPING:
				spr[index].setCurrentFrame(ACTION_JUMPING, 0);
				break;
			case ACTION_WALK_STAND:
				if (spr[index].nextFrame()) {
					spr[index].Events[0] = ACTION_STANDING;
					spr[index].setCurrentFrame(spr[index].Events[0], 0);
				}
				break;
			case ACTION_STAND_DUCK:
				if (spr[index].nextFrame()) {
					spr[index].Events[0] = ACTION_DUCKING;
					spr[index].setCurrentFrame(spr[index].Events[0], 0);
				}
				break;
			case ACTION_DUCK_STAND:
				if (spr[index].nextFrame()) {
					spr[index].Events[0] = ACTION_STANDING;
					spr[index].setCurrentFrame(spr[index].Events[0], 0);
				}
				break;
			case ACTION_JUMP_STAND:
				if (spr[index].nextFrame()) {
					spr[index].Events[0] = ACTION_STANDING;
					spr[index].setCurrentFrame(spr[index].Events[0], 0);
				}
				break;
			}
		} else {
			/**
			 * On Air
			 */
			if (ScreenController.IsKeyHold(KeyLeft)) {
				world.tryMovSpr(index, 0, -stepH, 0);
				if (spr[index].Flip == GSprite.TRANS_H) {
					spr[index].transFlip(GSprite.TRANS_H);
				}
			}
			if (ScreenController.IsKeyHold(KeyRight)) {
				world.tryMovSpr(index, 0, stepH, 0);
				if (spr[index].Flip == GSprite.TRANS_NONE) {
					spr[index].transFlip(GSprite.TRANS_H);
				}
			}
			if (spr[index].SpeedY256 > 0) {// DOWN
				spr[index].setCurrentAnimate(ACTION_JUMPING);
				spr[index].nextFrame();
				spr[index].CDStatus[3] = 3;
				spr[index].CDStatus[2] = 3;
				map[0].isCross = true;
			} else if (spr[index].SpeedY256 < 0) {// UP
				spr[index].setCurrentFrame(ACTION_JUMPING, 0);
				spr[index].CDStatus[3] = 0;
				spr[index].CDStatus[2] = 0;
			} else {// TOP
				spr[index].setCurrentFrame(ACTION_JUMPING, 0);
				spr[index].CDStatus[3] = 3;
				spr[index].CDStatus[2] = 3;
				map[0].isCross = true;
			}
		}

		// 自由落体
		if (world.tryMovSpr(index, 0, 0, spr[index].SpeedY256 / 256)) {
			spr[index].SpeedY256 = 0;
		}
		// 处理游戏基本物理状态 自由落体
		// 如果站立
		if (world.collideAll(index, 3, 2, 2, 0, -1)) {
			if (spr[index].Events[1] == STATE_JUMP) {
				if (ScreenController.IsKeyHold(KeyLeft | KeyRight)) {
					spr[index].Events[0] = ACTION_WALKING;
					spr[index].setCurrentFrame(ACTION_WALKING, 8);
				} else {
					spr[index].Events[0] = ACTION_JUMP_STAND;
					spr[index].setCurrentFrame(ACTION_JUMP_STAND, 0);
				}
			}
			spr[index].Events[1] = STATE_STAND;
			// DeltaY = 4;
		} else {
			// 如果在空中就自由落体
			spr[index].Events[1] = STATE_JUMP;
			if (spr[index].SpeedY256 < maxGSpeed) {
				spr[index].SpeedY256 += G;
			}
		}
	}
}
