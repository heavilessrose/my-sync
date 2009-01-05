package luke.games.engines.gLib;

import javax.microedition.lcdui.Graphics;

/**
 * 
 * 游戏世界。地图，精灵管理。统一坐标系。<br>
 * 支持:<br>
 * 单层卷轴(Map)<br>
 * 多个摄象机分屏(Camera)<br>
 * 多个精灵(Sprite)<br>
 * <p>
 * 每个精灵至少有3种碰撞块(GameSprite.CDStatus[])<br>
 * (id) 0:未定义<br>
 * (id) 1:精灵身体的碰撞块;和地图互相阻挡,和精灵之间互相阻挡.<br>
 * (id) 2:精灵身体下的碰撞块;和地图凳子的阻挡.<br>
 * <p>
 * 每个地图标志至少有3种碰撞<br>
 * CD[0]:未定义<br>
 * CD[1]:实体块<br>
 * CD[2]:凳子<br>
 */
public class GWorld {
	// static final public int CD_SUBSTANCE = 1 ;
	// static final public int CD_CROSSING = 2 ;
	/** 时钟 */
	public int Timer;
	/** 是否DEBUG模式 */
	public boolean isDebug = false;

	private GSprite[] SubSpr;
	private GMap[] SubMap;
	private GCamera[] SubCam;

	// private short MapCount;
	// private short SprCount;
	// private short CamCount;
	/** DEBUG背景色 */
	public int BackColor = 0x00ffffff;
	/** DEBUG地图块色 */
	public int MapColor = 0xffff0000;
	/** DEBUG精灵色 */
	public int SprColor = 0xffff0000;

	/** 重力X方向 */
	public short GX = 0;
	/** 重力Y方向 */
	public short GY = 1;

	private short SprBlockCD = 1;
	private short MapBlockCD = 1;
	private short SprStandCD = 2;
	private short MapStandCD = 2;

	// -------------------------------------------------------------------------------------
	/**
	 * 构造一个游戏世界
	 */
	public GWorld() {
		isDebug = false;
	}

	/**
	 * 添加一个精灵到GAME WORLD
	 * 
	 * @param spr
	 *            精灵
	 */
	final public void addSprite(GSprite spr) {
		if (SubSpr == null) {
			SubSpr = new GSprite[1];
		} else {
			GSprite[] temp = new GSprite[SubSpr.length + 1];
			System.arraycopy(SubSpr, 0, temp, 0, SubSpr.length);
			SubSpr = temp;
		}
		SubSpr[SubSpr.length - 1] = spr;
	}

	/**
	 * 添加一个地图到GAME WORLD
	 * 
	 * @param map
	 *            地图
	 */
	final public void addMap(GMap map) {
		if (SubMap == null) {
			SubMap = new GMap[1];
		} else {
			// return;
			// GameMap[] temp = new GameMap[SubMap.length+1];
			// System.arraycopy(SubMap,0,temp,0,SubMap.length);
			// SubMap = temp ;
		}
		SubMap[SubMap.length - 1] = map;
	}

	/**
	 * 添加一个摄象机到GAME WORLD
	 * 
	 * @param camera
	 */
	final public void addCamera(GCamera camera) {
		if (SubCam == null) {
			SubCam = new GCamera[1];
		} else {
			GCamera[] temp = new GCamera[SubCam.length + 1];
			System.arraycopy(SubCam, 0, temp, 0, SubCam.length);
			SubCam = temp;
		}
		SubCam[SubCam.length - 1] = camera;
	}

	/**
	 * 按组添加地图
	 * 
	 * @param map
	 */
	public void addMap(GMap[] map) {
		SubMap = map;
	}

	/**
	 * 按组添加摄象机
	 * 
	 * @param camera
	 */
	final public void addCamera(GCamera[] camera) {
		SubCam = camera;
	}

	/**
	 * 按组添加精灵
	 * 
	 * @param spr
	 */
	final public void addSprite(GSprite[] spr) {
		SubSpr = spr;
	}

	// --------------------------------------------------------------------------------------------------
	/**
	 * 初始化游戏世界,使用GAME WORLD前做的最后一件事. <br>
	 * 每次使用GAME WORLD的时候必须初始化,<br>
	 * 该游戏世界必须包含至少一个精灵、地图、摄象机.
	 */
	final public void init() {
		for (int i = 0; i < SubCam.length; i++) {
			SubCam[i].init();
			initCamera(i, 0);
		}
	}

	/**
	 * 初始化摄象机
	 * 
	 * @param CamID
	 * @param MapID
	 */
	final private void initCamera(int CamID, int MapID) {
		if (!SubCam[CamID].IsBackBuffer)
			return;
		Graphics buff = SubCam[CamID].bg;
		// CSTileGroup.SetGraphics(buff);

		int MapBlockX = SubCam[CamID].curMapBlockX;
		int MapBlockY = SubCam[CamID].curMapBlockY;
		int CamBlockX = SubCam[CamID].curBuffBlockPX;
		int CamBlockY = SubCam[CamID].curBuffBlockPY;
		for (int i = 0; i < SubCam[CamID].MatrixW; i++) {
			for (int j = 0; j < SubCam[CamID].MatrixH; j++) {
				if (!isDebug) {
					SubMap[MapID].renderDirectCell(buff, MapBlockX + i,
							MapBlockY + j, SubCam[CamID]
									.adjustBuffBlockPX(CamBlockX + i),
							SubCam[CamID].adjustBuffBlockPY(CamBlockY + j));
				} else {
					SubMap[MapID].renderDirectDebugCellCD(buff, MapBlockX + i,
							MapBlockY + j, SubCam[CamID]
									.adjustBuffBlockPX(CamBlockX + i),
							SubCam[CamID].adjustBuffBlockPY(CamBlockY + j),
							MapColor);
				}
			}
		}
	}

	// ------------------------------------------------------------------------------------
	/**
	 * 在当前GameWorld中移动SprID号精灵, 该精灵和地图和阻挡块相等的精灵发生碰撞,和凳子发生碰撞
	 * 
	 * @param SprID
	 *            GameWorld中的精灵号
	 * @param MapID
	 *            第几号地图
	 * @param px
	 *            移动PX个象素
	 * @param py
	 *            移动PY个象素
	 * @return true:被阻挡 false:没被阻挡 注意：外部逻辑需要控制速度不能超过本身大小
	 */
	final public boolean tryMovSpr(int SprID, int MapID, int px, int py) {
		if (SubSpr[SprID].Active == false)
			return false;
		// 身体防卡处理
		if (collideAll(SprID, SprBlockCD, SprBlockCD, SprBlockCD, MapID,
				MapBlockCD)) {
			SubSpr[SprID].mov(-GX, -GY);
			GUtil.print("Anti Cut Body !");
		}
		// 凳子防卡处理
		if (SubMap[MapID].isCross
				&& collideMap(SprID, SprStandCD, MapID, MapStandCD)) {
			SubSpr[SprID].mov(-GX, -GY);
			GUtil.print("Anti Cut Step !");
		}
		if (px == 0 && py == 0)
			return false;
		// Debug
		// 直接判断目标位置是否碰撞
		// 注意：外部逻辑需要控制速度不能超过本身大小
		SubSpr[SprID].mov(px, py);
		if (!collideAll(SprID, SprBlockCD, SprBlockCD, -1, MapID, -1))
			return false;
		SubSpr[SprID].mov(-px, -py);
		// 微调至目标
		int mx = Math.abs(px);
		int my = Math.abs(py);
		int rate = mx > my ? mx : my;
		int dx8 = px / rate;
		int dy8 = py / rate;
		for (int k = 0; k < rate; k++) {
			SubSpr[SprID].mov(dx8, dy8);
			if (collideMap(SprID, SprBlockCD, MapID, MapBlockCD)
					|| collideSprite(SprID, SprBlockCD, SprBlockCD, SprBlockCD)
					|| (SubMap[MapID].isCross && collideMap(SprID, SprStandCD,
							MapID, MapStandCD))
					|| (SubSpr[SprID].isCross && collideSprite(SprID,
							SprStandCD, SprStandCD, SprStandCD))) {
				SubSpr[SprID].mov(-dx8, -dy8);
				return true;
			}
		}
		return false;
	}

	// /**
	// * 在当前GameWorld中移动SprID号精灵,该精灵和地图和阻挡块相等的精灵发身碰撞.
	// * @param SprID GameWorld中的精灵号
	// * @param MapID 第几号地图
	// * @param px 移动PX个象素
	// * @param py 移动PY个象素
	// * 详细指定碰撞关系
	// * @param Spr1Block
	// * @param Spr1Stand
	// * @param Spr2Block
	// * @param Spr2Stand
	// * @param MapBlock
	// * @param MapStand
	// * @return true:被阻挡 false:没被阻挡
	// * 注意：外部逻辑需要控制速度不能超过本身大小
	// */
	// final public boolean tryMovSpr(
	// int SprID,
	// int MapID,
	// int px,
	// int py,
	// int Spr1BlockMap,
	// int Spr1StandMap,
	// int MapBlock,
	// int MapStand,
	// int Spr1BlockSpr2,
	// int Spr1StandSpr2,
	// int Spr2Block,
	// int Spr2Stand
	//
	// ){
	// if(SubSpr[SprID].Active==false)return false;
	// // 身体防卡处理
	// if( touchMap(SprID,Spr1BlockMap,MapID,MapBlock) &&
	// touchSprite(SprID,Spr1BlockSpr2,Spr2Block)
	// ){
	// SubSpr[SprID].mov(-GX,-GY);
	// CSUtil.print("Anti Cut Body !");
	// }
	// // 凳子防卡处理
	// if(SubMap[MapID].isCross && touchMap(SprID,Spr1Stand,MapID,MapStand)){
	// SubSpr[SprID].mov(-GX,-GY);
	// CSUtil.print("Anti Cut Step !");
	// }
	// if(px==0&&py==0)return false;
	// // Debug
	// // 直接判断目标位置是否碰撞
	// // 注意：外部逻辑需要控制速度不能超过本身大小
	// SubSpr[SprID].mov(px,py);
	// if(!touchAll(SprID,Spr1Block,Spr2Block,MapID,-1))return false;
	// SubSpr[SprID].mov(-px,-py);
	// // 微调至目标
	// int mx = Math.abs(px);
	// int my = Math.abs(py);
	// int rate = mx>my?mx:my;
	// int dx8 = px/rate;
	// int dy8 = py/rate;
	// for(int k=0;k<rate;k++){
	// SubSpr[SprID].mov(dx8,dy8);
	// if( touchMap(SprID,Spr1Block,MapID,MapBlock) ||
	// touchSprite(SprID,Spr1Block,Spr2Block) ||
	// (SubMap[MapID].isCross&&touchMap(SprID,Spr1Stand,MapID,MapStand)) ){
	// SubSpr[SprID].mov(-dx8,-dy8);
	// return true;
	// }
	// }
	// return false;
	// }

	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------

	/**
	 * 设置摄象机位置,并且重画摄象机显示的内容
	 * 
	 * @param CamID
	 *            摄象机ID
	 * @param MapID
	 *            摄象机拍摄的地图
	 * @param x
	 *            指定x位置
	 * @param y
	 *            指定y位置
	 */
	final public void setCameraAndInit(int CamID, int MapID, int x, int y) {
		int sx, sy, dx, dy, deltaX = 0, deltaY = 0;
		sx = SubCam[CamID].HPos;
		sy = SubCam[CamID].VPos;
		dx = x;
		dy = y;
		if (dx > sx)
			deltaX = 1;
		if (dx < sx)
			deltaX = -1;
		if (dy > sy)
			deltaY = 1;
		if (dy < sy)
			deltaY = -1;

		int XCount = Math.abs(dx - sx);
		int YCount = Math.abs(dy - sy);

		for (int i = 0; i < YCount; i++) {
			movCamera(CamID, MapID, 0, deltaY);
		}
		for (int i = 0; i < XCount; i++) {
			movCamera(CamID, MapID, deltaX, 0);
		}
	}

	/**
	 * 设置摄象机位置
	 * 
	 * @param CamID
	 *            摄象机索引
	 * @param MapID
	 *            地图索引
	 * @param x
	 *            指定x位置
	 * @param y
	 *            指定y位置
	 */
	final public void setCamera(int CamID, int MapID, int x, int y) {
		int tx, ty;
		tx = x - SubCam[CamID].HPos;
		ty = y - SubCam[CamID].VPos;
		if (tx > SubCam[CamID].CameraW || ty > SubCam[CamID].CameraH)
			setCameraAndInit(CamID, MapID, x, y);
		else
			movCamera(CamID, MapID, tx, ty);
	}

	/**
	 * 移动摄象机,画出摄像机范围的场景
	 * 
	 * @param CamID
	 *            摄象机ID
	 * @param MapID
	 *            地图索引
	 * @param dx
	 *            移动dx个象素
	 * @param dy
	 *            移动dy个象素
	 */
	final public void movCamera(int CamID, int MapID, int dx, int dy) {
		if (dx == 0 && dy == 0)
			return;
		int tx = 0;
		int ty = 0;
		tx = SubCam[CamID].HPos + dx;
		ty = SubCam[CamID].VPos + dy;

		if (tx < 0) {// 移动到地图边沿时
			tx = 0 - SubCam[CamID].HPos;
		} else if ((tx + SubCam[CamID].CameraW) > SubMap[MapID].getWidth()) {
			tx = SubMap[MapID].getWidth() - SubCam[CamID].CameraW;
			tx = tx - SubCam[CamID].HPos;
		} else {
			tx = dx;
		}
		if (ty < 0) {// 移动到地图边沿时
			ty = 0 - SubCam[CamID].VPos;
		} else if ((ty + SubCam[CamID].CameraH) > SubMap[MapID].getHeight()) {
			ty = SubMap[MapID].getHeight() - SubCam[CamID].CameraH;
			ty = ty - SubCam[CamID].VPos;
		} else {
			ty = dy;
		}

		// if(tx==0&&ty==0)return;
		SubCam[CamID].mov(tx, ty);// 改变摄像机Hpos Vpos

		if (!SubCam[CamID].IsBackBuffer)
			return;

		Graphics buff = SubCam[CamID].bg;
		// GTileGroup.SetGraphics(buff);

		if (SubCam[CamID].LOAD_RIGHT) {
			int MapBlockX, MapBlockY, CamBlockX, CamBlockY, XCount, YCount;
			XCount = SubCam[CamID].XCount;
			YCount = SubCam[CamID].MatrixH;
			MapBlockX = SubCam[CamID].curMapBlockX + SubCam[CamID].MatrixW
					- SubCam[CamID].XCount;
			MapBlockY = SubCam[CamID].curMapBlockY;
			CamBlockX = SubCam[CamID].curBuffBlockPX - SubCam[CamID].XCount;
			CamBlockY = SubCam[CamID].curBuffBlockPY;
			for (int i = 0; i < XCount; i++) {
				for (int j = 0; j < YCount; j++) {

					if (!isDebug) {
						SubMap[MapID].renderDirectCell(buff, MapBlockX + i,
								MapBlockY + j, SubCam[CamID]
										.adjustBuffBlockPX(CamBlockX + i),
								SubCam[CamID].adjustBuffBlockPY(CamBlockY + j));
					} else {
						SubMap[MapID].renderDirectDebugCellCD(buff, MapBlockX
								+ i, MapBlockY + j, SubCam[CamID]
								.adjustBuffBlockPX(CamBlockX + i),
								SubCam[CamID].adjustBuffBlockPY(CamBlockY + j),
								MapColor);
					}
				}
			}
			SubCam[CamID].LOAD_RIGHT = false;
		}
		if (SubCam[CamID].LOAD_LEFT) {
			int MapBlockX, MapBlockY, CamBlockX, CamBlockY, XCount, YCount;
			XCount = SubCam[CamID].XCount;
			YCount = SubCam[CamID].MatrixH;
			MapBlockX = SubCam[CamID].curMapBlockX;
			MapBlockY = SubCam[CamID].curMapBlockY;
			CamBlockX = SubCam[CamID].curBuffBlockPX
					+ SubCam[CamID].CameraMatrixW + 1;
			CamBlockY = SubCam[CamID].curBuffBlockPY;
			for (int i = 0; i < XCount; i++) {
				for (int j = 0; j < YCount; j++) {

					if (!isDebug) {
						SubMap[MapID].renderDirectCell(buff, MapBlockX + i,
								MapBlockY + j, //
								SubCam[CamID].adjustBuffBlockPX(CamBlockX + i),//
								SubCam[CamID].adjustBuffBlockPY(CamBlockY + j));
					} else {
						SubMap[MapID].renderDirectDebugCellCD(buff, MapBlockX
								+ i,
								MapBlockY + j,//
								SubCam[CamID].adjustBuffBlockPX(CamBlockX + i),//
								SubCam[CamID].adjustBuffBlockPY(CamBlockY + j),
								MapColor);
					}
				}
			}
			SubCam[CamID].LOAD_LEFT = false;
		}

		if (SubCam[CamID].LOAD_DOWN) {
			int MapBlockX, MapBlockY, CamBlockX, CamBlockY, XCount, YCount;
			YCount = SubCam[CamID].YCount;
			XCount = SubCam[CamID].MatrixW;
			MapBlockY = SubCam[CamID].curMapBlockY + SubCam[CamID].MatrixH
					- SubCam[CamID].YCount;
			MapBlockX = SubCam[CamID].curMapBlockX;
			CamBlockY = SubCam[CamID].curBuffBlockPY - SubCam[CamID].YCount;
			CamBlockX = SubCam[CamID].curBuffBlockPX;
			for (int i = 0; i < XCount; i++) {
				for (int j = 0; j < YCount; j++) {

					if (!isDebug) {
						SubMap[MapID].renderDirectCell(buff, MapBlockX + i,
								MapBlockY + j, SubCam[CamID]
										.adjustBuffBlockPX(CamBlockX + i),
								SubCam[CamID].adjustBuffBlockPY(CamBlockY + j));
					} else {
						SubMap[MapID].renderDirectDebugCellCD(buff, MapBlockX
								+ i, MapBlockY + j, SubCam[CamID]
								.adjustBuffBlockPX(CamBlockX + i),
								SubCam[CamID].adjustBuffBlockPY(CamBlockY + j),
								MapColor);
					}
				}
			}
			SubCam[CamID].LOAD_DOWN = false;
		}
		if (SubCam[CamID].LOAD_UP) {
			int MapBlockX, MapBlockY, CamBlockX, CamBlockY, XCount, YCount;
			YCount = SubCam[CamID].YCount;
			XCount = SubCam[CamID].MatrixW;
			MapBlockY = SubCam[CamID].curMapBlockY;
			MapBlockX = SubCam[CamID].curMapBlockX;
			CamBlockY = SubCam[CamID].curBuffBlockPY
					+ SubCam[CamID].CameraMatrixH + 1;
			CamBlockX = SubCam[CamID].curBuffBlockPX;
			for (int i = 0; i < XCount; i++) {
				for (int j = 0; j < YCount; j++) {

					if (!isDebug) {
						SubMap[MapID].renderDirectCell(buff, MapBlockX + i,
								MapBlockY + j, SubCam[CamID]
										.adjustBuffBlockPX(CamBlockX + i),
								SubCam[CamID].adjustBuffBlockPY(CamBlockY + j));
					} else {
						SubMap[MapID].renderDirectDebugCellCD(buff, MapBlockX
								+ i, MapBlockY + j, SubCam[CamID]
								.adjustBuffBlockPX(CamBlockX + i),
								SubCam[CamID].adjustBuffBlockPY(CamBlockY + j),
								MapColor);
					}
				}
			}
			SubCam[CamID].LOAD_UP = false;
		}
	}

	// -----------------------------------------------------------------------------------
	/**
	 * 测试一个精灵和所有物体是否碰撞
	 * 
	 * @param SprID
	 *            当前精灵索引
	 * @param spr1_bid
	 *            当前精灵碰撞号
	 * @param spr2_bid
	 *            其他精灵碰撞号
	 * @param spr_tag
	 *            精灵的检测方式（为负，则不考虑碰撞属性）
	 * @param MapID
	 *            地图索引
	 * @param map_tag
	 *            地图的检测方式（为负，则不考虑碰撞属性）
	 * @return false:true
	 */
	final public boolean collideAll(int SprID, int spr1_bid, int spr2_bid,
			int spr_tag, int MapID, int map_tag) {
		if (collideMap(SprID, spr1_bid, MapID, map_tag)
				|| collideSprite(SprID, spr1_bid, spr2_bid, spr_tag)) {
			return true;
		}
		return false;
	}

	/**
	 * 测试2个精灵是否碰撞
	 * 
	 * @param Spr1ID
	 *            当前精灵索引
	 * @param bid1
	 *            当前精灵碰撞号
	 * @param Spr2ID
	 *            其他精灵索引
	 * @param bid2
	 *            其他精灵碰撞号
	 * @param spr_tag
	 *            精灵的检测方式（为负，则不考虑碰撞属性）
	 * @return false:true
	 */
	final public boolean collideSprite2(int Spr1ID, int bid1, int Spr2ID,
			int bid2, int spr_tag) {
		if (!SubSpr[Spr1ID].haveSprCD || !SubSpr[Spr2ID].haveSprCD)
			return false;
		if (spr_tag >= 0
				&& GSprite.touchAdvance(SubSpr[Spr1ID], bid1, SubSpr[Spr2ID],
						bid2)) {
			return true;
		} else if (spr_tag < 0
				&& GSprite.touchBlock(SubSpr[Spr1ID], bid1, SubSpr[Spr2ID],
						bid2)) {
			return true;
		}
		return false;
	}

	/**
	 * 测试一个精灵和所有精灵是否碰撞
	 * 
	 * @param SprID
	 *            当前精灵索引
	 * @param bid1
	 *            当前精灵碰撞号
	 * @param bid2
	 *            其他精灵碰撞号
	 * @param spr_tag
	 *            精灵的检测方式（为负，则不考虑碰撞属性）
	 * @return false:true
	 */
	final public boolean collideSprite(int SprID, int bid1, int bid2,
			int spr_tag) {
		if (!SubSpr[SprID].haveSprCD || SubSpr[SprID].CDStatus[bid1] == 0)
			return false;
		for (int i = SubSpr.length - 1; i >= 0; i--) {
			if (SprID != i && SubSpr[i].Active && SubSpr[i].haveSprCD) {
				if (spr_tag >= 0
						&& GSprite.touchAdvance(SubSpr[SprID], bid1, SubSpr[i],
								bid2)) {
					return true;
				} else if (spr_tag < 0
						&& GSprite.touchBlock(SubSpr[SprID], bid1, SubSpr[i],
								bid2)) {
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * 检查精灵和地图是否相碰
	 * 
	 * @param SprID
	 *            精灵ID
	 * @param sbid
	 *            精灵碰撞块
	 * @param MapID
	 *            地图ID
	 * @param mbid
	 *            地图碰撞块(-1=ALL)
	 * @return false:true
	 */
	final public boolean collideMap(int SprID, int sbid, int MapID, int mbid) {
		if (!SubSpr[SprID].haveMapCD || SubSpr[SprID].CDStatus[sbid] == 0)
			return false;
		int StartX = (SubSpr[SprID].HPos + SubSpr[SprID].CDX[sbid])
				/ SubMap[MapID].CellW;
		int StartY = (SubSpr[SprID].VPos + SubSpr[SprID].CDY[sbid])
				/ SubMap[MapID].CellH;
		int BlockW = ((SubSpr[SprID].CDW[sbid] - 1) / SubMap[MapID].CellW) + 2;
		int BlockH = ((SubSpr[SprID].CDH[sbid] - 1) / SubMap[MapID].CellH) + 2;
		for (int i = StartY; i < StartY + BlockH; i++) {
			for (int j = StartX; j < StartX + BlockW; j++) {
				if (j < 0 || j > SubMap[MapID].MatrixTile[0].length - 1
						|| i < 0 || i > SubMap[MapID].MatrixTile.length - 1)
					continue;
				if ((mbid > 0 && SubMap[MapID].MatrixTag[i][j] == mbid)
						|| (mbid < 0 && SubMap[MapID].MatrixTag[i][j] > 0)) {
					// CSUtil.print(">_<");
					if ((SubSpr[SprID].CDStatus[sbid] & SubMap[MapID].CDStatus[SubMap[MapID].MatrixTag[i][j]]) != 0
							&& GSprite
									.touchRect(
											SubSpr[SprID],
											sbid,
											j
													* SubMap[MapID].CellW
													+ SubMap[MapID].CDX[SubMap[MapID].MatrixTag[i][j]],
											i
													* SubMap[MapID].CellH
													+ SubMap[MapID].CDY[SubMap[MapID].MatrixTag[i][j]],
											SubMap[MapID].CDW[SubMap[MapID].MatrixTag[i][j]],
											SubMap[MapID].CDH[SubMap[MapID].MatrixTag[i][j]])) {
						// CSUtil.print("SprCD["+sbid+"] touch
						// MapCD["+SubMap[MapID].MatrixTag[i][j]+"] !");
						return true;
					}
				}
			}
		}
		return false;
	}

	// ---------------------------------------------------------------------------------------------
	/*
	 * no back buffer
	 */
	final private void updateMap(Graphics g, int CID) {
		g.setColor(MapColor);
		g.fillRect(SubCam[CID].CameraX, SubCam[CID].CameraY,
				SubCam[CID].CameraW, SubCam[CID].CameraH);
		// GTileGroup.SetGraphics(g);
		for (int MapID = SubMap.length - 1; MapID >= 0; MapID--) {
			int StartX = SubCam[CID].HPos / SubMap[MapID].CellW;
			int StartY = SubCam[CID].VPos / SubMap[MapID].CellH;
			int BlockW = (SubCam[CID].CameraW - 1) / SubMap[MapID].CellW + 2;
			int BlockH = (SubCam[CID].CameraH - 1) / SubMap[MapID].CellH + 2;
			int ScreenX = 0;
			int ScreenY = 0;
			for (int i = StartY; i < StartY + BlockH; i++) {
				for (int j = StartX; j < StartX + BlockW; j++) {
					SubMap[MapID].renderNoBufferCell(g, j, i, j
							* SubMap[MapID].CellW - SubCam[CID].HPos
							+ SubCam[CID].CameraX, i * SubMap[MapID].CellH
							- SubCam[CID].VPos + SubCam[CID].CameraY);
					if (isDebug) {
						SubMap[MapID].renderNoBufferDebugCell(g, j, i, j
								* SubMap[MapID].CellW - SubCam[CID].HPos
								+ SubCam[CID].CameraX, i * SubMap[MapID].CellH
								- SubCam[CID].VPos + SubCam[CID].CameraY,
								MapColor);
					}
				}
			}
		}
	}

	// ------------------------------------------------------------------------------------------------
	/**
	 * 渲染整个世界
	 * 
	 * @param g
	 *            GameCanvas的G
	 * @param CameraID
	 *            第几号摄象机
	 */
	final public void render(Graphics g, int CameraID) {
		int cx = g.getClipX();
		int cy = g.getClipY();
		int cw = g.getClipWidth();
		int ch = g.getClipHeight();
		g.setClip(SubCam[CameraID].CameraX, SubCam[CameraID].CameraY,
				SubCam[CameraID].CameraW, SubCam[CameraID].CameraH);
		if (SubCam[CameraID].IsBackBuffer) {
			// render map on BackBuffer
			Graphics buff = SubCam[CameraID].bg;
			// CSTileGroup.SetGraphics(buff);
			SubCam[CameraID].render(g);
		} else {
			updateMap(g, CameraID);
		}
		// render map Animate Tile on Screen
		// CSTileGroup.SetGraphics(g);

		// SubMap[0].renderAnimateCell(
		// SubCam[CameraID].HPos,
		// SubCam[CameraID].VPos,
		// Timer,
		// SubCam[CameraID].CameraX,
		// SubCam[CameraID].CameraY,
		// SubCam[CameraID].CameraW,
		// SubCam[CameraID].CameraH
		// );

		for (int id = SubSpr.length - 1; id >= 0; id--) {
			if (SubSpr[id].Visible
					&& SubSpr[id].HPos < SubCam[CameraID].HPos
							+ SubCam[CameraID].CameraW
					&& SubSpr[id].HPos > SubCam[CameraID].HPos
							- SubSpr[id].Width
					&& SubSpr[id].VPos < SubCam[CameraID].VPos
							+ SubCam[CameraID].CameraH
					&& SubSpr[id].VPos > SubCam[CameraID].VPos
							- SubSpr[id].Height) {
				SubSpr[id].OnScreen = true;
				SubSpr[id].OffsetX = -(SubCam[CameraID].HPos)
						+ SubCam[CameraID].CameraX;
				SubSpr[id].OffsetY = -(SubCam[CameraID].VPos)
						+ SubCam[CameraID].CameraY;
				SubSpr[id].render(g);
				if (isDebug)
					SubSpr[id].renderDebugCD(g, SprColor);
			} else {
				SubSpr[id].OnScreen = false;
			}
		}
		g.setClip(cx, cy, cw, ch);
	}

	/**
	 * 渲染地图
	 * 
	 * @param g
	 *            GameCanvas的G
	 * @param CameraID
	 *            第几号摄象机
	 */
	final public void renderMap(Graphics g, int CameraID) {
		int cx = g.getClipX();
		int cy = g.getClipY();
		int cw = g.getClipWidth();
		int ch = g.getClipHeight();
		g.setClip(SubCam[CameraID].CameraX, SubCam[CameraID].CameraY,
				SubCam[CameraID].CameraW, SubCam[CameraID].CameraH);
		if (SubCam[CameraID].IsBackBuffer) {
			// render map on BackBuffer
			Graphics buff = SubCam[CameraID].bg;
			// CSTileGroup.SetGraphics(buff);
			SubCam[CameraID].render(g);
		} else {
			updateMap(g, CameraID);
		}
		// SubMap[0].renderAnimateCell(
		// SubCam[CameraID].HPos,
		// SubCam[CameraID].VPos,
		// Timer,
		// SubCam[CameraID].CameraX,
		// SubCam[CameraID].CameraY,
		// SubCam[CameraID].CameraW,
		// SubCam[CameraID].CameraH
		// );
		g.setClip(cx, cy, cw, ch);
	}

	/**
	 * 渲染非地图精灵的精灵，根据世界坐标系
	 * 
	 * @param g
	 * @param CameraID
	 * @param spr
	 */
	final public void renderSprite(Graphics g, int CameraID, GSprite spr[]) {
		int cx = g.getClipX();
		int cy = g.getClipY();
		int cw = g.getClipWidth();
		int ch = g.getClipHeight();
		g.setClip(SubCam[CameraID].CameraX, SubCam[CameraID].CameraY,
				SubCam[CameraID].CameraW, SubCam[CameraID].CameraH);
		for (int i = spr.length - 1; i >= 0; i--) {
			if (spr[i].Visible
					&& spr[i].HPos < SubCam[CameraID].HPos
							+ SubCam[CameraID].CameraW
					&& spr[i].HPos > SubCam[CameraID].HPos - spr[i].Width
					&& spr[i].VPos < SubCam[CameraID].VPos
							+ SubCam[CameraID].CameraH
					&& spr[i].VPos > SubCam[CameraID].VPos - spr[i].Height) {
				spr[i].OnScreen = true;
				spr[i].OffsetX = -(SubCam[CameraID].HPos)
						+ SubCam[CameraID].CameraX;
				spr[i].OffsetY = -(SubCam[CameraID].VPos)
						+ SubCam[CameraID].CameraY;
				spr[i].render(g);
				if (isDebug)
					spr[i].renderDebugCD(g, SprColor);
			} else {
				spr[i].OnScreen = false;
			}
		}
		g.setClip(cx, cy, cw, ch);
	}

	// ----------------------------------------------------------------------------------------------------
	/**
	 * 更新世界
	 */
	final public void update() {
		Timer++;
	}
}