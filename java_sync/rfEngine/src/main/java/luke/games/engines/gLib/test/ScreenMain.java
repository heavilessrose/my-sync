package luke.games.engines.gLib.test;

import java.util.Random;
import javax.microedition.lcdui.Graphics;
import luke.games.engines.gLib.*;

//import javax.microedition.lcdui.Graphics;

public class ScreenMain implements SubScreen {
	static GSoundPlayer soundman1;
	static String[] MapFileList = GUtil
			.getResFileNameList("/gLibTest/MapData.pak");
	static int MapIndex = 0;
	static int AllClear = 0;
	/**
	 * 精灵的类型
	 */
	final int OBJ_TYPE_ACTOR = 101;

	final int OBJ_TYPE_STEP_1 = 1;
	final int OBJ_TYPE_STEP_2 = 2;
	final int OBJ_TYPE_STEP_3 = 3;
	final int OBJ_TYPE_BOARD_1 = 4;
	final int OBJ_TYPE_BOARD_2 = 5;
	final int OBJ_TYPE_BOARD_3 = 6;

	final int OBJ_TYPE_BUTTON_1 = 7;
	final int OBJ_TYPE_BUTTON_2 = 8;
	final int OBJ_TYPE_BUTTON_3 = 9;
	final int OBJ_TYPE_MAGICAL_1 = 10;
	final int OBJ_TYPE_MAGICAL_2 = 11;
	final int OBJ_TYPE_MAGICAL_3 = 12;

	final int OBJ_TYPE_ENEMY = 13;
	final int OBJ_TYPE_EXIT = 14;

	final int[] OBJ_TYPE_1 = { OBJ_TYPE_STEP_1, OBJ_TYPE_BOARD_1 };
	final int[] OBJ_TYPE_2 = { OBJ_TYPE_STEP_2, OBJ_TYPE_BOARD_2 };
	final int[] OBJ_TYPE_3 = { OBJ_TYPE_STEP_3, OBJ_TYPE_BOARD_3 };

	/**
	 * 地图精灵的事件
	 */
	final int OBJ_EVENT_0_MOVE = 1;
	final int OBJ_EVENT_0_HOLD = 2;

	final int OBJ_EVENT_2_DIP_ON = 1;
	final int OBJ_EVENT_2_DIP_OFF = 2;

	final int OBJ_EVENT_3_TO_P1 = 1;
	final int OBJ_EVENT_3_TO_P2 = 2;

	Random random = new Random();

	GWorld world;
	GCamera camera[];
	GMap map[];
	GSprite spr[];
	GSprite stars[] = new GSprite[10];
	GSprite door[];

	int ActorIndex;

	final int CameraX = ScreenController.LOGIC_START_X + 1;
	final int CameraY = ScreenController.LOGIC_START_Y + 1;
	final int CameraW = ScreenController.LOGIC_WIDTH - 2;
	final int CameraH = ScreenController.LOGIC_HEIGHT - 2;

	int G = 200;

	int StepH = 3;
	int JumpV = 12 * 200;// default = 12*200
	int MaxGSpeed = 30 * G;

	int DeltaX = 4;
	int DeltaY = 4;

	boolean FreeCamera = false;

	int holdtime1 = 0;
	int holdtime2 = 0;

	public ScreenMain() {
		if (MapIndex == MapFileList.length - 1)
			JumpV = 12 * 200 * 2;

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

		ScreenController.ClearScreenAndClip(0);
		loadLevel();

		world.setCamera(0, 0, spr[ActorIndex].getHC()
				- (camera[0].CameraW >> 1), spr[ActorIndex].getVC()
				- (camera[0].CameraH >> 1));

		GUtil.print("Init End : Free Memory = "
				+ (Runtime.getRuntime().freeMemory() / 1024) + "(K byte)");
		GUtil.print();

		if (soundman1 == null) {

			soundman1 = new GSoundPlayer("/bgm/bgm01.mid", "audio/midi", -1);
			soundman1.setVolume(100);
			soundman1.play();
		}

		ScreenController.KeyEnable = true;
		ScreenController.LogicEnable = true;
		ScreenController.RenderEnable = true;
	}

	// ------------------------------------------------------------------------------------------------------

	public void Logic() {
		/**
		 * debug
		 */
		// if(GameCanvas.IsKeyDown(GAME_KEY_9)){
		// world.isDebug = !world.isDebug;
		// }
		// if(GameCanvas.IsKeyDown(GAME_KEY_7)){
		// if(JumpV == 12*200){
		// JumpV=32*200;
		// }else{
		// JumpV=12*200 ;
		// }
		// }
		// if(GameCanvas.IsKeyDown(GAME_KEY_SHARP)){
		// if(MapIndex+1>MapFileList.length-1){
		// MapIndex=0;
		// }else{
		// MapIndex+=1;
		// }
		// // ScreenMenu.soundman1.destroy();
		// GameCanvas.ChangeSubSreen(SCREEN_TYPE_MAIN);
		// return;
		// }
		// if(GameCanvas.IsKeyDown(GAME_KEY_STAR)){
		// if(MapIndex-1<0){
		// MapIndex=MapFileList.length-1;
		// }else{
		// MapIndex-=1;
		// }
		// // ScreenMenu.soundman1.destroy();
		// GameCanvas.ChangeSubSreen(SCREEN_TYPE_MAIN);
		// return;
		// }
		// --------------------------------------------------------------
		// 秘籍
		if (ScreenController.IsKeyDown(GAME_KEY_SHARP)) {
			if (JumpV != 12 * 200 * 2) {
				JumpV = 12 * 200 * 2;
			} else {
				JumpV = 12 * 200;
			}

		}
		if (ScreenController.IsKeyDown(GAME_KEY_STAR)) {
			// 跳关
			if (MapIndex + 1 > MapFileList.length - 2) {
				MapIndex = 0;
			} else {
				MapIndex += 1;
			}
			ScreenController.ChangeSubSreen(SCREEN_TYPE_MAIN, "作弊进入 第 "
					+ (ScreenMain.MapIndex + 1) + " 层");
			return;
		}
		// -------------------------------------------------------------------------------------------------------
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

		processActor(ActorIndex);

		if (FreeCamera) {
			if (ScreenController.IsKeyHold(GAME_KEY_UP)) {
				world.movCamera(0, 0, 0, -4);
			}
			if (ScreenController.IsKeyHold(GAME_KEY_DOWN)) {
				world.movCamera(0, 0, 0, 4);
			}
			if (ScreenController.IsKeyHold(GAME_KEY_LEFT)) {
				world.movCamera(0, 0, -4, 0);
			}
			if (ScreenController.IsKeyHold(GAME_KEY_RIGHT)) {
				world.movCamera(0, 0, 4, 0);
			}
		} else {
			int dx = spr[ActorIndex].getHC()
					- (camera[0].HPos + (camera[0].CameraW >> 1));
			int dy = spr[ActorIndex].getVC()
					- (camera[0].VPos + (camera[0].CameraH >> 1));
			world.movCamera(0, 0, dx / DeltaX, dy / DeltaY);
			// world.movCamera(0,0,dx/8,dy/8);
		}

		for (int i = spr.length - 1; i >= 0; i--) {
			processObj(i, ActorIndex);
		}

		world.update();
	}

	// --------------------------------------------------------------------------------------------
	public void Render(Graphics g) {
		// world.render(g,0);
		world.renderMap(g, 0);
		world.renderSprite(g, 0, door);
		world.renderSprite(g, 0, spr);
		world.renderSprite(g, 0, stars);

		if (FreeCamera && world.Timer % 8 > 2) {
			// camera[0].renderDebugWindow(g,random.nextInt());
			ScreenController.ClearClip();
			g.setColor(0xff00ff00);
			g.drawRect(camera[0].CameraX + 16, camera[0].CameraY + 16,
					camera[0].CameraW - 32, camera[0].CameraH - 32);
		}

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

		// debug
		// GGraphics.drawString(
		// GameCanvas.ScreenGraphics, ""
		// + " TPF=" + GameMIDlet.TIMER_PRE_FRAME
		// + " FPS="
		// + (1000 / ((System.currentTimeMillis() - CurTime) == 0
		// ? 1
		// : (System.currentTimeMillis() - CurTime))),
		// 0,0,
		// 0xffffffff,0xff000000,
		// GGraphics.TEXT_STYLE_NONE);
		// CurTime = System.currentTimeMillis();
		// */
	}

	long CurTime = 1000;

	private void loadLevel() {
		GUtil.print("Map Data Init Begin !");

		// 地图的标志，对应地形
		byte[] MapTagTable = new byte[] { 1, 1, 2, 2, 2, 0, 1, 1, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, };

		/**
		 * Map data struct u8 width;//地图宽度格数 u8 height;//地图高度格数 u8
		 * gridWidth;//地图格宽度 u8 gridHeight;//地图格高度 u8 grid[width][height];//
		 * 
		 * u8 unitCount;//精灵个数 { u8 type;// u16 x1;// u16 y1;// u16 x2;// u16
		 * y2;// }; // 9 * unitCount
		 */
		short MapW;
		short MapH;
		short MapCellW;
		short MapCellH;
		byte MapTileMatrix[][];
		short ObjCount;
		byte ObjType[];
		int ObjX1[];
		int ObjY1[];
		int ObjX2[];
		int ObjY2[];

		/**
		 * Map data ends
		 */
		byte[] buff = GUtil.unpackFile("/gLibTest/MapData.pak",
				MapFileList[MapIndex]);
		// byte[] buff = GUtil.loadFile(MapFile);
		int p = 0;

		MapW = (short) (buff[p++] & 0x00ff);
		MapH = (short) (buff[p++] & 0x00ff);
		MapCellW = (short) (buff[p++] & 0x00ff);
		MapCellH = (short) (buff[p++] & 0x00ff);

		MapTileMatrix = new byte[MapH][MapW];
		byte[][] MapTagMatrix = new byte[MapH][MapW];
		GUtil.print("Creat Map " + MapW + "x" + MapH);
		for (int y = 0; y < MapH; y++) {
			for (int x = 0; x < MapW; x++) {
				MapTileMatrix[y][x] = buff[p++];
				MapTagMatrix[y][x] = MapTagTable[MapTileMatrix[y][x]];
			}
		}

		ObjCount = (short) (buff[p++] & 0x00ff);
		ObjType = new byte[ObjCount];
		ObjX1 = new int[ObjCount];
		ObjY1 = new int[ObjCount];
		ObjX2 = new int[ObjCount];
		ObjY2 = new int[ObjCount];
		GUtil.print("Creat Spr " + ObjCount);

		for (int i = 0; i < ObjCount; i++) {
			ObjType[i] = buff[p++];
			ObjX1[i] = GUtil.readShort(buff, p) & 0x0000ffff;
			p += 2;
			ObjY1[i] = GUtil.readShort(buff, p) & 0x0000ffff;
			p += 2;
			ObjX2[i] = GUtil.readShort(buff, p) & 0x0000ffff;
			p += 2;
			ObjY2[i] = GUtil.readShort(buff, p) & 0x0000ffff;
			p += 2;
		}

		buff = null;
		System.gc();
		GUtil.print("Map Data Init Ends !");
		GUtil.print("MapData End : Free Memory = "
				+ (Runtime.getRuntime().freeMemory() / 1024) + "(K byte)");

		int[] ObjTypeTable = new int[] {
				OBJ_TYPE_STEP_1,// 
				OBJ_TYPE_STEP_2,//
				OBJ_TYPE_STEP_3,//
				OBJ_TYPE_BOARD_1, OBJ_TYPE_BOARD_2, OBJ_TYPE_BOARD_3,
				OBJ_TYPE_BUTTON_1, OBJ_TYPE_BUTTON_2, OBJ_TYPE_BUTTON_3,
				OBJ_TYPE_MAGICAL_1, OBJ_TYPE_MAGICAL_2, OBJ_TYPE_MAGICAL_3,
				OBJ_TYPE_ENEMY, OBJ_TYPE_ACTOR, };
		int[][] ObjTileTable = new int[][] { { 0, 0, 0, 0 }, { 1, 1, 1, 1 },
				{ 2, 2, 2, 2 }, { 3, 4, 4, 4 }, { 5, 6, 6, 6 }, { 7, 8, 8, 8 },
				{ 9, 10, 10, 10 }, { 11, 12, 11, 11 }, { 13, 14, 14, 14 },
				{ 15, 15, 15, 15 }, { 16, 16, 16, 16 }, { 17, 17, 17, 17 },
				{ 33, 34, 35, 34 }, };

		GTileGroup tile = new GTileGroup(null, 37);
		for (int i = 0; i < 37; i++) {
			int d1 = i / 10;
			int d0 = i % 10;
			tile.SetTileImage(GUtil.loadImage("/gLibTest/obj.pak", "" + d1 + ""
					+ d0 + ".png"));
			tile.AddTile();
		}
		tile.gc();

		GSprite to[] = new GSprite[13];
		for (int i = 0; i < to.length; i++) {
			to[i] = createObj(ObjTypeTable[i], tile, ObjTileTable[i]);
		}
		spr = new GSprite[ObjCount];
		for (int i = 0; i < ObjCount; i++) {
			if (ObjTypeTable[ObjType[i]] == OBJ_TYPE_ACTOR) {
				ActorIndex = i;
				System.out.println("Actor Index is " + ActorIndex);
				System.out.println("Actor Type  is " + ObjType[i]);
				spr[i] = createActor(OBJ_TYPE_ACTOR, ObjX1[i], ObjY1[i],
						ObjX2[i], ObjY2[i]);
			} else {
				spr[i] = to[ObjType[i]].clone();
				initObj(spr[i], ObjX1[i], ObjY1[i], ObjX2[i], ObjY2[i]);
			}
		}
		to = null;
		System.gc();

		actionInitAllObjStatus();

		GSprite ts = createStar(tile, 18);
		for (int i = 0; i < stars.length; i++) {
			stars[i] = ts.clone();
		}
		ts = null;
		System.gc();

		// door
		int doorCount = 0;
		int doorX[];
		int doorY[];
		for (int y = 0; y < MapH; y++) {
			for (int x = 0; x < MapW; x++) {
				if (MapTileMatrix[y][x] == 10) {
					doorCount += 1;
				}
			}
		}
		if (doorCount > MapFileList.length - 1) {
			doorCount = MapFileList.length - 1;
		}

		door = new GSprite[doorCount];
		doorX = new int[doorCount];
		doorY = new int[doorCount];

		int Count = 0;
		for (int y = MapH - 1; y >= 0; y--) {
			for (int x = MapW - 1; x >= 0; x--) {
				if (MapTileMatrix[y][x] == 10) {
					if (Count < doorCount) {
						doorX[Count] = x * MapCellW;
						doorY[Count] = y * MapCellH;
						Count += 1;
					}
				}
			}
		}

		GSprite td = new GSprite(tile, 0, 1, 1);
		td.addSubspr(0, 0, 36, 0);// 0
		td.Visible = false;
		for (int i = 0; i < door.length; i++) {
			door[i] = td.clone();
			door[i].Type = (byte) (i);
			door[i].HPos = doorX[i];
			door[i].VPos = doorY[i];
		}

		GUtil.print();
		GUtil.print("Spr End : Free Memory = "
				+ (Runtime.getRuntime().freeMemory() / 1024) + "(K byte)");

		GTileGroup mapTile = new GTileGroup(GUtil.loadImage(
				"/gLibTest/map.pak", "map.png"), 6 * 7);
		mapTile.AddTile(0, 0, 12 * 6, 12 * 7, 12, 12);
		mapTile.gc();

		map = new GMap[1];
		map[0] = new GMap(mapTile, MapTileMatrix, MapTagMatrix, MapCellW,
				MapCellH, 0, 0, MapCellW, 1, 0);

		camera = new GCamera[1];
		camera[0] = new GCamera(CameraW < map[0].getWidth() ? CameraX : CameraX
				+ ((CameraW - map[0].getWidth()) >> 1), CameraH < map[0]
				.getHeight() ? CameraY : CameraY
				+ ((CameraH - map[0].getHeight()) >> 1), CameraW < map[0]
				.getWidth() ? CameraW : map[0].getWidth(), CameraH < map[0]
				.getHeight() ? CameraH : map[0].getHeight(), MapCellW,
				MapCellH, 0, true);

		world = new GWorld();
		world.isDebug = false;
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

	private GSprite createActor(int type, int x1, int y1, int x2, int y2) {
		int ActorCount = 25;
		GTileGroup tile = new GTileGroup(null, ActorCount);
		for (int i = 0; i < ActorCount; i++) {
			int d1 = i / 10;
			int d0 = i % 10;
			tile.SetTileImage(GUtil.loadImage("/gLibTest/actor.pak", "" + d1
					+ "" + d0 + ".png"));
			tile.AddTile();
		}
		GSprite obj = new GSprite(tile, type, ActorCount, 4);
		for (int i = 0; i < ActorCount; i++) {
			obj.addSubspr(0, 0, i, 0);// 12
		}

		// obj.setCDRect(0,4,5,0,12,30);
		obj.addCDRect(3, 6, 0, 10, 30);
		obj.addCDRect(3, 6, 29, 10, 1);
		obj.addCDRect(3, 6, 30, 10, 1);

		obj.setAnimate(0, new byte[] { 0 });// 0 standing pass
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
		obj.addAnimate(new byte[] { 1, 1, 1, 1, 0, 0, 0, 0 });// 9 jump ->
		// stand pass
		obj.addAnimate(new byte[] { 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24,
				24 });// 10 dead pass

		obj.Events = new int[4];
		obj.EventTime = new int[4];
		obj.Extra = new int[4];
		obj.setPos(x1, y1);
		obj.Extra[0] = x1;
		obj.Extra[1] = y1;
		obj.Extra[2] = x2;
		obj.Extra[3] = y2;

		obj.transFlip(GSprite.TRANS_H);
		return obj;
	}

	final int KeyLeft = GAME_KEY_LEFT | GAME_KEY_4 | GAME_KEY_1;
	final int KeyRight = GAME_KEY_RIGHT | GAME_KEY_6 | GAME_KEY_3;
	final int KeyJump = GAME_KEY_UP | GAME_KEY_5 | GAME_KEY_1 | GAME_KEY_2
			| GAME_KEY_3;
	final int KeyDuck = GAME_KEY_DOWN | GAME_KEY_8;

	private void processActor(int index) {
		if (spr[index].Events[2] != 0) {
			actionInitStar(spr[index].Events[2] - 1, spr[index].HPos
					+ Math.abs(random.nextInt() % spr[index].getWidth()),
					spr[index].VPos
							+ Math.abs(random.nextInt()
									% spr[index].getHeight()));
			actionRunStar();
		}
		for (int i = door.length - 1; i >= 0; i--) {
			if (Math.abs(spr[index].HPos - door[i].HPos) < 40
					&& Math.abs(spr[index].VPos - door[i].VPos) < 40) {
				door[i].Visible = true;
			} else {
				door[i].Visible = false;
			}

			if (Math.abs(spr[index].HPos - door[i].HPos) < 5
					&& Math.abs(spr[index].VPos - (door[i].VPos + 10)) < 10) {
				// TODO Stage Clear
				if (MapIndex >= MapFileList.length - 1) {
					MapIndex = door[i].Type % MapFileList.length;
					ScreenController.ChangeSubSreen(SCREEN_TYPE_MAIN, "进入 第 "
							+ (ScreenMain.MapIndex + 1) + " 层");
				} else {
					MapIndex += 1;
					ScreenController.ChangeSubSreen(SCREEN_TYPE_MAIN, "进入 第 "
							+ (ScreenMain.MapIndex + 1) + " 层");
				}
			}
		}
		if (spr[index].Events[1] == STATE_DEAD) {
			GameMIDlet.TIMER_PRE_FRAME = 100;
			if (spr[index].nextFrame()) {
				// TODO Game Over
				soundman1.destroy();
				// ScreenMenu.putSave();
				// ScreenGameOver.EndState=0;
				ScreenController.ChangeSubSreen(SCREEN_TYPE_LOGO);
				// spr[index].Events[1]=STATE_STAND;
				// spr[index].Events[0]=ACTION_STANDING;
				// spr[index].setCurrentFrame(spr[index].Events[0],0);
				// spr[index].haveSprCD = true;
			}
			return;
		} else if (spr[index].Events[1] == STATE_STAND) {
			if (FreeCamera) {
				if (ScreenController.IsKeyDown(GAME_KEY_0)) {
					FreeCamera = false;
				}
				return;
			} else {
				if (ScreenController.IsKeyDown(GAME_KEY_0)) {
					FreeCamera = true;
				}
			}

			/**
			 * On Stand
			 */
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
					world.tryMovSpr(index, 0, -StepH, 0);
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
					world.tryMovSpr(index, 0, StepH, 0);
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
				spr[index].SpeedY256 = -JumpV;
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
				world.tryMovSpr(index, 0, -StepH, 0);
				if (spr[index].Flip == GSprite.TRANS_H) {
					spr[index].transFlip(GSprite.TRANS_H);
				}
			}
			if (ScreenController.IsKeyHold(KeyRight)) {
				world.tryMovSpr(index, 0, StepH, 0);
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
		/**
		 * 
		 */
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
			spr[index].Events[1] = STATE_JUMP;
			if (spr[index].SpeedY256 < MaxGSpeed) {
				spr[index].SpeedY256 += G;
			}
			// DeltaY = 1;
		}
	}

	private GSprite createObj(int type, GTileGroup tile, int[] TileTable) {
		int ActorCount = TileTable.length;

		GSprite obj = new GSprite(tile, type, ActorCount, 4);
		for (int i = 0; i < TileTable.length; i++) {
			obj.addSubspr(0, 0, TileTable[i], 0);// 0
		}

		obj.setCDRect(0, 4, 0, 0, obj.getWidth(), obj.getHeight());
		obj.addCDRect(0, 0);// 1
		obj.addCDRect(3, 0, 0, obj.getWidth(), 1);// 2

		obj.setAnimate(0, new byte[] { 0, 1, 2, 3 });

		obj.Events = new int[4];
		obj.EventTime = new int[4];
		obj.Extra = new int[4];

		return obj;
	}

	private void initObj(GSprite obj, int x1, int y1, int x2, int y2) {
		obj.Events[0] = OBJ_EVENT_3_TO_P1;
		obj.Extra[0] = x1;
		obj.Extra[1] = y1;
		obj.Extra[2] = x2;
		obj.Extra[3] = y2;

		int a = Math.abs(obj.Extra[0] - obj.Extra[2]);
		int b = Math.abs(obj.Extra[1] - obj.Extra[3]);
		int c = GMath.sqrt(a * a + b * b);

		obj.SpeedX256 = (obj.Extra[0] * 256 - obj.Extra[2] * 256)
				/ (c != 0 ? c : 1);
		obj.SpeedY256 = (obj.Extra[1] * 256 - obj.Extra[3] * 256)
				/ (c != 0 ? c : 1);
		obj.HPos256 = obj.Extra[0] * 256;
		obj.VPos256 = obj.Extra[1] * 256;

		obj.DirectX = obj.Extra[0] - obj.Extra[2];
		obj.DirectY = obj.Extra[1] - obj.Extra[3];

		if (obj.DirectX > 0)
			obj.DirectX = 1;
		else if (obj.DirectX < 0)
			obj.DirectX = -1;
		else
			obj.DirectX = 0;

		if (obj.DirectY > 0)
			obj.DirectY = 1;
		else if (obj.DirectY < 0)
			obj.DirectY = -1;
		else
			obj.DirectY = 0;

		obj.setPos(obj.Extra[0], obj.Extra[1]);

		obj.haveMapCD = false;
	}

	private void processObj(int index, int actor) {
		if (spr[index].Active == false)
			return;
		if (index == actor)
			return;

		// switch public events
		switch (spr[index].Events[0]) {
		case OBJ_EVENT_0_MOVE:
			int dx = 0,
			dy = 0;
			switch (spr[index].Events[3]) {
			case OBJ_EVENT_3_TO_P1:
				if (Math.abs(spr[index].HPos - spr[index].Extra[0]) <= 1
						&& Math.abs(spr[index].VPos - spr[index].Extra[1]) <= 1) {
					spr[index].Events[3] = OBJ_EVENT_3_TO_P2;
				} else {
					if (Math.abs(spr[index].HPos - spr[index].Extra[0]) > 1) {
						spr[index].HPos256 += spr[index].SpeedX256;
						dx = spr[index].HPos256 / 256 - spr[index].HPos;
					}
					if (Math.abs(spr[index].VPos - spr[index].Extra[1]) > 1) {
						spr[index].VPos256 += spr[index].SpeedY256;
						dy = spr[index].VPos256 / 256 - spr[index].VPos;
					}
				}
				break;
			case OBJ_EVENT_3_TO_P2:
				if (Math.abs(spr[index].HPos - spr[index].Extra[2]) <= 1
						&& Math.abs(spr[index].VPos - spr[index].Extra[3]) <= 1) {
					spr[index].Events[3] = OBJ_EVENT_3_TO_P1;
				} else {
					if (Math.abs(spr[index].HPos - spr[index].Extra[2]) > 1) {
						spr[index].HPos256 -= spr[index].SpeedX256;
						dx = spr[index].HPos256 / 256 - spr[index].HPos;
					}
					if (Math.abs(spr[index].VPos - spr[index].Extra[3]) > 1) {
						spr[index].VPos256 -= spr[index].SpeedY256;
						dy = spr[index].VPos256 / 256 - spr[index].VPos;
					}
				}
				break;
			}
			if (dx == 0 && dy == 0)
				break;
			// 站立后的处理
			if (world.collideSprite2(index, 2, actor, 3, 3)) {
				map[0].isCross = false;
				if (dy < 0) {// up
					world.tryMovSpr(actor, 0, dx, dy);
					world.tryMovSpr(index, 0, dx, dy);
				} else {// down
					world.tryMovSpr(index, 0, dx, dy);
					world.tryMovSpr(actor, 0, dx, dy);
				}
			} else {
				world.tryMovSpr(index, 0, dx, dy);
			}
			break;
		case OBJ_EVENT_0_HOLD:

			break;
		}

		// switch type events
		switch (spr[index].Type) {
		case OBJ_TYPE_MAGICAL_1:
		case OBJ_TYPE_MAGICAL_2:
		case OBJ_TYPE_MAGICAL_3:
			// if(spr[index].Events[2]!=OBJ_EVENT_2_DIP_ON){
			spr[index].VPos = spr[index].Extra[1]
					+ GMath.sinTimes256(world.Timer * 10) / 64;
			// }else{
			// spr[index].VPos = spr[index].Extra[1];
			// }
			if (world.collideSprite2(index, 0, actor, 1, -1)) {
				actionChangeMagical(index);
			}
			break;
		case OBJ_TYPE_ENEMY:
			spr[index].nextCycFrame();
			if (world.collideSprite2(index, 0, actor, 1, -1)) {
				// attak actor
				spr[actor].Events[0] = ACTION_DEAD;
				spr[actor].Events[1] = STATE_DEAD;
				spr[actor].setCurrentFrame(ACTION_DEAD, 0);
				spr[actor].haveSprCD = false;
			}
			break;
		case OBJ_TYPE_BUTTON_1:
		case OBJ_TYPE_BUTTON_2:
		case OBJ_TYPE_BUTTON_3:
			if (world.collideSprite2(index, 2, actor, 3, 3)) {
				actionChangeButton(index);
			}
			break;
		}
	}

	private void actionInitAllObjStatus() {
		for (int i = 0; i < spr.length; i++) {
			if (i == ActorIndex)
				continue;

			switch (spr[i].Type) {
			case OBJ_TYPE_BOARD_1:
			case OBJ_TYPE_BOARD_2:
			case OBJ_TYPE_BOARD_3:
				spr[i].haveSprCD = false;
			case OBJ_TYPE_STEP_1:
			case OBJ_TYPE_STEP_2:
			case OBJ_TYPE_STEP_3:
				spr[i].setCurrentFrame(0, 1);
				spr[i].Events[3] = OBJ_EVENT_3_TO_P1;
				spr[i].Events[0] = OBJ_EVENT_0_HOLD;
				break;
			case OBJ_TYPE_MAGICAL_1:
			case OBJ_TYPE_MAGICAL_2:
			case OBJ_TYPE_MAGICAL_3:
				spr[i].setCDRect(2, 0, 0, 0, 0, 0);
			case OBJ_TYPE_BUTTON_1:
			case OBJ_TYPE_BUTTON_2:
			case OBJ_TYPE_BUTTON_3:
				spr[i].setCurrentFrame(0, 0);
				spr[i].Events[0] = OBJ_EVENT_0_HOLD;
				break;
			case OBJ_TYPE_ENEMY:
				spr[i].setCurrentFrame(0, 0);
				spr[i].Events[0] = OBJ_EVENT_0_MOVE;
				spr[i].Events[3] = OBJ_EVENT_3_TO_P1;
				break;
			}
		}
	}

	private boolean actionChangeButton(int index) {
		int Type0 = 0, Type1 = 0;
		switch (spr[index].Type) {
		case OBJ_TYPE_BUTTON_1:
			Type0 = OBJ_TYPE_1[0];
			Type1 = OBJ_TYPE_1[1];
			break;
		case OBJ_TYPE_BUTTON_2:
			Type0 = OBJ_TYPE_2[0];
			Type1 = OBJ_TYPE_2[1];
			break;
		case OBJ_TYPE_BUTTON_3:
			Type0 = OBJ_TYPE_3[0];
			Type1 = OBJ_TYPE_3[1];
			break;
		default:
			return false;
		}
		int Type2 = 0, Type3 = 0;
		switch (spr[ActorIndex].Events[STATUS_GET_MAGICAL]) {
		case 1:
			Type2 = OBJ_TYPE_1[0];
			Type3 = OBJ_TYPE_1[1];
			break;
		case 2:
			Type2 = OBJ_TYPE_2[0];
			Type3 = OBJ_TYPE_2[1];
			break;
		case 3:
			Type2 = OBJ_TYPE_3[0];
			Type3 = OBJ_TYPE_3[1];
			break;
		}

		if (spr[index].Events[2] == OBJ_EVENT_2_DIP_ON) {
			return false;
		}

		for (int i = 1; i < spr.length; i++) {
			// close all board action
			switch (spr[i].Type) {
			case OBJ_TYPE_BOARD_1:
			case OBJ_TYPE_BOARD_2:
			case OBJ_TYPE_BOARD_3:
				spr[i].haveSprCD = false;
			case OBJ_TYPE_STEP_1:
			case OBJ_TYPE_STEP_2:
			case OBJ_TYPE_STEP_3:
				spr[i].setCurrentFrame(0, 1);
				spr[i].Events[0] = OBJ_EVENT_0_HOLD;
				break;
			case OBJ_TYPE_BUTTON_1:
			case OBJ_TYPE_BUTTON_2:
			case OBJ_TYPE_BUTTON_3:
				spr[i].CDY[2] = 0;
				// case OBJ_TYPE_MAGICAL_1:
				// case OBJ_TYPE_MAGICAL_2:
				// case OBJ_TYPE_MAGICAL_3:
				spr[i].setCurrentFrame(0, 0);
				spr[i].Events[2] = OBJ_EVENT_2_DIP_OFF;
				break;
			}
			// open correspond board action
			if (spr[i].Type == Type0 || spr[i].Type == Type1
					|| spr[i].Type == Type2 || spr[i].Type == Type3) {

				spr[i].setCurrentFrame(0, 0);
				spr[i].Events[0] = OBJ_EVENT_0_MOVE;
				spr[i].haveSprCD = true;
			}
		}
		spr[index].setCurrentFrame(0, 1);
		spr[index].CDY[2] = (short) (2);
		spr[index].Events[2] = OBJ_EVENT_2_DIP_ON;

		switch (spr[index].Type) {
		case OBJ_TYPE_BUTTON_1:
			spr[ActorIndex].Events[3] = 1;
			break;
		case OBJ_TYPE_BUTTON_2:
			spr[ActorIndex].Events[3] = 2;
			break;
		case OBJ_TYPE_BUTTON_3:
			spr[ActorIndex].Events[3] = 3;
			break;
		}

		return true;
	}

	private boolean actionChangeMagical(int index) {
		int Type0 = 0, Type1 = 0;
		switch (spr[index].Type) {
		case OBJ_TYPE_MAGICAL_1:
			Type0 = OBJ_TYPE_1[0];
			Type1 = OBJ_TYPE_1[1];
			break;
		case OBJ_TYPE_MAGICAL_2:
			Type0 = OBJ_TYPE_2[0];
			Type1 = OBJ_TYPE_2[1];
			break;
		case OBJ_TYPE_MAGICAL_3:
			Type0 = OBJ_TYPE_3[0];
			Type1 = OBJ_TYPE_3[1];
			break;
		default:
			return false;
		}
		int Type2 = 0, Type3 = 0;
		switch (spr[ActorIndex].Events[STATUS_LAST_DIP]) {
		case 1:
			Type2 = OBJ_TYPE_1[0];
			Type3 = OBJ_TYPE_1[1];
			break;
		case 2:
			Type2 = OBJ_TYPE_2[0];
			Type3 = OBJ_TYPE_2[1];
			break;
		case 3:
			Type2 = OBJ_TYPE_3[0];
			Type3 = OBJ_TYPE_3[1];
			break;
		}
		if (spr[index].Events[2] == OBJ_EVENT_2_DIP_ON) {
			return false;
		}

		for (int i = 1; i < spr.length; i++) {
			// close all board action
			switch (spr[i].Type) {
			case OBJ_TYPE_BOARD_1:
			case OBJ_TYPE_BOARD_2:
			case OBJ_TYPE_BOARD_3:
				spr[i].haveSprCD = false;
			case OBJ_TYPE_STEP_1:
			case OBJ_TYPE_STEP_2:
			case OBJ_TYPE_STEP_3:
				spr[i].setCurrentFrame(0, 1);
				spr[i].Events[0] = OBJ_EVENT_0_HOLD;
				break;
			// case OBJ_TYPE_BUTTON_1:
			// case OBJ_TYPE_BUTTON_2:
			// case OBJ_TYPE_BUTTON_3:

			case OBJ_TYPE_MAGICAL_1:
			case OBJ_TYPE_MAGICAL_2:
			case OBJ_TYPE_MAGICAL_3:
				// other magical st.
				spr[i].setCurrentFrame(0, 0);
				spr[i].Events[2] = OBJ_EVENT_2_DIP_OFF;
				break;
			}
			// open correspond board action
			if (spr[i].Type == Type0 || spr[i].Type == Type1
					|| spr[i].Type == Type2 || spr[i].Type == Type3) {

				spr[i].setCurrentFrame(0, 0);
				spr[i].Events[0] = OBJ_EVENT_0_MOVE;
				spr[i].haveSprCD = true;
			}
		}
		// cur magical st.
		spr[index].setCurrentFrame(0, 1);
		spr[index].Events[2] = OBJ_EVENT_2_DIP_ON;

		switch (spr[index].Type) {
		case OBJ_TYPE_MAGICAL_1:
			spr[ActorIndex].Events[2] = 1;
			break;
		case OBJ_TYPE_MAGICAL_2:
			spr[ActorIndex].Events[2] = 2;
			break;
		case OBJ_TYPE_MAGICAL_3:
			spr[ActorIndex].Events[2] = 3;
			break;
		}

		return true;
	}

	// private void actionBoardON(boolean dip, int type) {
	//	    
	// }

	// private void actionBoard

	private GSprite createStar(GTileGroup tile, int start) {
		GSprite obj = new GSprite(tile, 0xff, 15, 1);

		for (int i = 0; i < 15; i++) {
			obj.addSubspr(-tile.GetWidth(start + i) >> 1, -tile.GetHeight(start
					+ i) >> 1, start + i, 0);// 0
		}

		obj.setAnimate(0, new byte[] { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4 });
		obj.addAnimate(new byte[] { 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 });
		obj.addAnimate(new byte[] { 10, 10, 11, 11, 12, 12, 13, 13, 14, 14 });

		obj.haveMapCD = false;
		obj.haveSprCD = false;
		obj.isCross = false;
		obj.Visible = false;
		obj.Active = false;

		return obj;
	}

	private void actionInitStar(int type, int x, int y) {
		for (int i = stars.length - 1; i >= 0; i--) {
			if (stars[i].Visible == false) {
				stars[i].setPos(x, y);
				stars[i].setCurrentFrame(type, 0);
				stars[i].Visible = true;
				return;
			}
		}
	}

	private void actionRunStar() {
		for (int i = stars.length - 1; i >= 0; i--) {
			if (stars[i].Visible == true) {
				if (stars[i].nextFrame()) {
					stars[i].Visible = false;
				}
			}
		}
	}
}
