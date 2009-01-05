package luke.games.engines.gLib;

import javax.microedition.lcdui.Graphics;

/**
 * 组合精灵类
 * <p>
 * 一个精灵由多个子精灵组成<br>
 * 每个子精灵有自己的tile图片,碰撞参数,xy轴偏移,翻转参数等参数,这些参数放入相应数组,用来组成一个精灵
 * 
 */
public class GSprite {
	// -------------------------------------------------------------------------------------------------
	// Field
	// -------------------------------------------------------------------------------------------------
	/*
	 * 翻转参数
	 */
	/** 不翻转 */
	final public static byte TRANS_NONE = 0;
	/** 水平翻转 */
	final public static byte TRANS_H = 2;
	/** 垂直翻转 */
	final public static byte TRANS_V = 1;
	/** 180度翻转 */
	final public static byte TRANS_HV = 3;
	/** 逆时针90度翻转 */
	final public static byte TRANS_90 = 6;
	/** 逆时针270度翻转 */
	final public static byte TRANS_270 = 5;
	/** 先逆时针90度翻转，然后在水平翻转 */
	final public static byte TRANS_H90 = 4;
	/** 先逆时针90度翻转，然后在垂直翻转 */
	final public static byte TRANS_V90 = 7;
	/** 180度翻转 */
	final public static byte TRANS_180 = 3; // 180 = HV

	// -------基本属性---------------------------------------------------------------------------------
	private GTileGroup tile;
	/** ID号 */
	public byte ID;
	/** 类型 */
	public byte Type;
	/** 整个的高度 */
	protected short Height = 0;
	/** 整个的宽度 */
	protected short Width = 0;
	/** 翻转参考点X */
	public short FX = 0;
	/** 翻转参考点Y */
	public short FY = 0;

	/** 在世界中的水平起始坐标 */
	volatile public int HPos = 0;
	/** 在世界中的垂直起始坐标 */
	volatile public int VPos = 0;
	/** 翻转参数 */
	volatile public byte Flip = 0;

	/** 是否显示 */
	volatile public boolean Visible = true;
	/** 是否活动 */
	volatile public boolean Active = true;
	/** 是否可以穿透（凳子） */
	volatile public boolean isCross = true;
	/** 是否和精灵有碰撞 */
	volatile public boolean haveSprCD = true;
	/** 是否和地图有碰撞 */
	volatile public boolean haveMapCD = true;

	/** 是否在屏幕内 */
	volatile protected boolean OnScreen = true;
	/** 精灵在世界中可能的x偏移 * */
	volatile protected int OffsetX = 0;
	/** 精灵在世界中可能的y偏移 * */
	volatile protected int OffsetY = 0;

	// ------子精灵参数----------------------------------------------------------------------
	/** 当前子精灵标号 */
	private short SubIndex;
	/** 组合精灵的子精灵数 */
	private short SubCount;
	/** 子精灵宽 */
	protected short[] SW;
	/** 子精灵高 */
	protected short[] SH;
	/** 子精灵和坐标的X偏移 */
	protected short[] SX;
	/** 子精灵和坐标的Y偏移 */
	protected short[] SY;
	/** 子精灵TILE号 */
	protected byte[] STileID;
	/** 子精灵的翻转参数 */
	protected byte[] SFlip;

	// ------碰撞范围//判定块-----------------------------------------------------------------------
	private short CDIndex;
	private short CDCount;
	/**
	 * @see gLib.GWorld#GWorld() ?
	 */
	public short CDStatus[];// CDStatus[CDIndex], CDIndex碰撞块的状态
	public short CDX[];
	public short CDY[];
	public short CDW[];
	public short CDH[];

	// ----------动画相关-------------------------------------------------------------------------
	/**
	 * 组合帧 Frames[FrameID][帧序列] render时用到的最外层
	 */
	private byte Frames[][];

	/**
	 * 所有动画数据Animates[AnimateID][Sequence]
	 */
	private byte Animates[][];

	/** 当前动画序列 * */
	volatile private short CurAnimate;
	/** 当前帧 * */
	volatile private short CurFrame;

	// ----------事件,no method------------------------------------------
	/** 精灵的事件表 */
	volatile public int Events[];
	/** 精灵事件所用的时间 */
	volatile public int EventTime[];
	/** 附加属性 */
	volatile public int Extra[];

	// -----物理相关 no method---------------------------------------------
	/** X方向速度，256倍定点数 */
	volatile public int SpeedX256; // 8位定点数
	/** Y方向速度，256倍定点数 */
	volatile public int SpeedY256; // 8位定点数
	/** X方向 */
	volatile public int DirectX;
	/** Y方向 */
	volatile public int DirectY;
	/** 水平坐标的256倍幻影 */
	volatile public int HPos256;
	/** 垂直坐标的256倍幻影 */
	volatile public int VPos256;

	// -------------------------------------------------------------------------------------------------
	// Method
	// -------------------------------------------------------------------------------------------------
	/**
	 * 构造空精灵，用于克隆
	 */
	private GSprite() {
	}

	/**
	 * 构造化精灵
	 * 
	 * @param t
	 *            精灵用的TILE组
	 * @param type
	 *            精灵类型
	 * @param count
	 *            子精灵数
	 * @param cdcount
	 *            碰撞块数
	 */
	public GSprite(GTileGroup t, int type, int count, int cdcount) {
		tile = t;
		Type = (byte) type;
		// --------------子精灵--------------
		SubCount = (short) count;// 子精灵个数
		SubIndex = 0;
		STileID = new byte[count];
		SFlip = new byte[count];
		SW = new short[count];
		SH = new short[count];
		SX = new short[count];
		SY = new short[count];
		// --------------判定块--------------
		CDCount = (short) cdcount;// 判定块个数
		CDIndex = 0;
		CDStatus = new short[cdcount];
		CDX = new short[cdcount];
		CDY = new short[cdcount];
		CDW = new short[cdcount];
		CDH = new short[cdcount];
		// --------------动画--------------
		CurAnimate = 0;
		CurFrame = 0;
		Frames = new byte[count][1];
	}

	/**
	 * 添加一个子精灵,并且设置默认帧号（按照添加的顺序）
	 * 
	 * @param px
	 *            x方向偏移
	 * @param py
	 *            y方向偏移
	 * @param tileid
	 *            tile组
	 * @param trans
	 *            翻转属性
	 */
	final public void addSubspr(int px, int py, int tileid, int trans) {
		if (SubIndex > 127) {
			GUtil.print("Out Of Sprite Max Count !");
			return;
		}
		if (SubIndex < SubCount) {
			STileID[SubIndex] = (byte) tileid;
			SH[SubIndex] = (short) tile.GetHeight(tileid);
			SW[SubIndex] = (short) tile.GetWidth(tileid);
			if (SubIndex == 0) {
				Height = SH[0];
				Width = SW[0];
				setFlipRefPos(Width >> 1, Height >> 1);// 以中心点作为翻转参考点
				addCDRect(0, px, py, SW[0], SH[0]);
				addAnimate(new byte[] { 0 });
			}
			SX[SubIndex] = (short) px;
			SY[SubIndex] = (short) py;
			SFlip[SubIndex] = (byte) trans;
			Frames[SubIndex][0] = (byte) SubIndex;// 设置帧号
			SubIndex++;
		}
	}

	// -----------动画相关-------------------------------------------------------------------
	/**
	 * 添加一个动画序列
	 * 
	 * @param animate
	 *            动画序列
	 */
	final public void addAnimate(byte[] animate) {
		if (Animates == null) {
			Animates = new byte[1][];
			Animates[0] = animate;
			// GUtil.print("Create a Animate");
		} else {
			byte[][] temp = new byte[Animates.length + 1][];
			for (int i = 0; i < Animates.length; i++) {
				temp[i] = new byte[Animates[i].length];
				System
						.arraycopy(Animates[i], 0, temp[i], 0,
								Animates[i].length);
			}
			Animates = temp;
			Animates[Animates.length - 1] = new byte[animate.length];
			System.arraycopy(animate, 0, Animates[Animates.length - 1], 0,
					animate.length);
		}
		System.gc();
	}

	/**
	 * 指定一个动画序列
	 * 
	 * @param id
	 *            动画索引
	 * @param animate
	 *            动画序列
	 */
	final public void setAnimate(int id, byte[] animate) {
		if (Animates == null) {
			addAnimate(animate);
		} else {
			Animates[id] = animate;
		}
	}

	/**
	 * 设置当前要播放的动画序列
	 * 
	 * @param id
	 *            动画索引
	 */
	final public void setCurrentAnimate(int id) {
		if (SubIndex <= 1)
			return;

		CurAnimate = (short) id;
		CurFrame = (short) (CurFrame % Animates[CurAnimate].length);
	}

	/**
	 * 设置当前帧
	 * 
	 * @param id
	 *            动画索引
	 * @param index
	 *            帧号
	 */
	final public void setCurrentFrame(int id, int index) {
		if (SubIndex <= 1)
			return;
		CurAnimate = (short) id;
		CurFrame = (short) index;
	}

	/**
	 * 设置当前贞
	 * 
	 * @param id
	 *            动画索引
	 * @param index
	 *            贞号
	 * @param restart
	 *            循环重播的位置
	 */
	final public void setCurrentFrame(int id, int index, int restart) {
		if (SubIndex <= 1)
			return;
		CurAnimate = (short) id;
		CurFrame = (short) index;
	}

	/**
	 * 到下一帧
	 * 
	 * @return is End of Frames
	 */
	final public boolean nextFrame() {
		if (SubIndex <= 1)
			return true;
		if (CurFrame < Animates[CurAnimate].length - 1) {
			CurFrame++;
		} else {
			return true;
		}
		return false;
	}

	/**
	 * 到下一帧,如果到最后一帧,则循环
	 */
	final public void nextCycFrame() {
		if (SubIndex <= 1)
			return;
		CurFrame++;
		if (CurFrame < Animates[CurAnimate].length) {
		} else {
			CurFrame = 0;
		}
	}

	/**
	 * 到下一帧,如果到最后一帧,则在指定位置循环
	 * 
	 * @param restart
	 *            重播的位置
	 */
	final public void nextCycFrame(int restart) {
		if (SubIndex <= 1)
			return;
		CurFrame++;
		if (CurFrame < Animates[CurAnimate].length) {
		} else {
			CurFrame = (byte) (restart % Animates[CurAnimate].length);
		}
	}

	/**
	 * 设置一个组图帧
	 * 
	 * @param index
	 *            帧号
	 * @param frame
	 *            组合帧
	 */
	final public void setComboFrame(int index, byte[] frame) {
		Frames[index] = frame;
	}

	// ---------------------------------------------------------------------------------------
	/**
	 * 在世界中移动整个精灵(动作游戏中不推荐使用,会出现滑步)
	 * 
	 * @param offsetX
	 * @param offsetY
	 */
	final public void mov(int offsetX, int offsetY) {
		HPos += offsetX;
		VPos += offsetY;
	}

	// /**
	// * 按照速度移动整个精灵
	// * @param offsetx
	// * @param offsety
	// */
	// final public void movs() {
	// HPos += (SpeedX256>>8);
	// VPos += (SpeedY256>>8);
	// }

	/**
	 * 设置坐标
	 * 
	 * @param x
	 * @param y
	 */
	final public void setPos(int x, int y) {
		HPos = x;
		VPos = y;
	}

	/**
	 * 设置长宽
	 * 
	 * @param w
	 * @param h
	 */
	final public void setArea(int w, int h) {
		Width = (short) w;
		Height = (short) h;
	}

	/**
	 * 得到高度
	 * 
	 * @return height
	 */
	final public int getHeight() {
		return Height;
	}

	/**
	 * 得到宽度
	 * 
	 * @return width
	 */
	final public int getWidth() {
		return Width;
	}

	/**
	 * 得到在世界中的中心点(以参考点为中心)水平坐标
	 * 
	 * @return h center
	 */
	final public int getHC() {
		return HPos + FX;
	}

	/**
	 * 得到在世界中的中心点(以参考点为中心)垂直坐标
	 * 
	 * @return v center
	 */
	final public int getVC() {
		return VPos + FY;
	}

	// /**
	// * 通过TILE大小设置长宽
	// *
	// * @param tile
	// * @param id
	// */
	// final public void setArea(CSTileGroup tile, int id) {
	// Width = (short)tile.GetWidth(id);
	// Height = (short)tile.GetHeight(id);
	// }

	/**
	 * 设置精灵在世界中可能的偏移
	 */
	final protected void setOffsetPos(int offsetx, int offsety) {
		OffsetX = (short) offsetx;
		OffsetY = (short) offsety;
	}

	// -----------------------------------------------------------------------------------------

	/**
	 * 添加一个判定块
	 * 
	 * @param status
	 * @param cdx
	 * @param cdy
	 * @param cdw
	 * @param cdh
	 */
	final public void addCDRect(int status, int cdx, int cdy, int cdw, int cdh) {
		if (CDIndex > 127) {
			GUtil.print("Out of Max CD Count !");
			return;
		}
		CDStatus[CDIndex] = (short) status;
		CDX[CDIndex] = (short) cdx;
		CDY[CDIndex] = (short) cdy;
		CDW[CDIndex] = (short) cdw;
		CDH[CDIndex] = (short) cdh;
		CDIndex++;
	}

	/**
	 * 添加一个判定块，根据子精灵大小
	 * 
	 * @param status
	 *            碰撞属性
	 * @param subSpr
	 *            部件号
	 */
	final public void addCDRect(int status, int subSpr) {
		addCDRect(status, SX[subSpr], SY[subSpr], SW[subSpr], SH[subSpr]);
	}

	/**
	 * 手动设置一个判定块
	 * 
	 * @param id
	 * @param status
	 * @param x
	 * @param y
	 * @param w
	 * @param h
	 */
	final public void setCDRect(int id, int status, int x, int y, int w, int h) {
		CDStatus[id] = (short) status;
		CDX[id] = (short) x;
		CDY[id] = (short) y;
		CDW[id] = (short) w;
		CDH[id] = (short) h;
	}

	// /**
	// * 移动一个子精灵
	// * @param id 子精灵号
	// * @param px x方向偏移
	// * @param py y方向偏移
	// */
	// final public void movSubPos(int id ,int px,int py){
	// PX[id] += px ;
	// PY[id] += py ;
	// }

	// /**
	// * 设置子精灵翻转
	// * @param id 子精灵号
	// * @param flip 翻转
	// */
	// final public void setSubFlip(int id,int flip){
	// SFlip[id] = (byte)flip ;
	// }

	// ---------------------------------------------------------------------------------------------------
	/**
	 * 设置翻转参考点
	 * 
	 * @param fx
	 * @param fy
	 */
	final public void setFlipRefPos(int fx, int fy) {
		FX = (short) fx;
		FY = (short) fy;
	}

	/**
	 * 以参考点翻转整个精灵
	 * 
	 * @param style
	 */
	final public void transFlip(byte style) {
		short tx, ty, temp;
		Flip = transFlipNext(Flip, style);
		for (int i = SubIndex - 1; i >= 0; i--) {
			tx = (short) (SX[i] + (SW[i] >> 1) - FX);
			ty = (short) (SY[i] + (SH[i] >> 1) - FY);
			switch (style) {
			case TRANS_NONE:
				break;
			case TRANS_H:
				SX[i] = (short) (FX - tx - (SW[i] >> 1));
				break;
			case TRANS_V:
				SY[i] = (short) (FY - ty - (SH[i] >> 1));
				break;
			case TRANS_HV:
				SX[i] = (short) (FX - tx - (SW[i] >> 1));
				SY[i] = (short) (FY - ty - (SH[i] >> 1));
				break;
			case TRANS_90:
				temp = SW[i];
				SW[i] = SH[i];
				SH[i] = (short) temp;
				SX[i] = (short) (FX + ty - (SW[i] >> 1));
				SY[i] = (short) (FY - tx - (SH[i] >> 1));
				break;
			case TRANS_270:
				temp = SW[i];
				SW[i] = SH[i];
				SH[i] = (short) temp;
				SX[i] = (short) (FX - ty - (SW[i] >> 1));
				SY[i] = (short) (FY + tx - (SH[i] >> 1));
				break;
			case TRANS_H90:
				temp = SW[i];
				SW[i] = SH[i];
				SH[i] = (short) temp;
				SX[i] = (short) (FX + ty - (SW[i] >> 1));
				SY[i] = (short) (FY + tx - (SH[i] >> 1));
				break;
			case TRANS_V90:
				temp = SW[i];
				SW[i] = SH[i];
				SH[i] = temp;
				SX[i] = (short) (FX - ty - (SW[i] >> 1));
				SY[i] = (short) (FY - tx - (SH[i] >> 1));
				break;
			}
			SFlip[i] = transFlipNext(SFlip[i], style);
		}
		for (int i = CDIndex - 1; i >= 0; i--) {
			tx = (short) (CDX[i] + (CDW[i] >> 1) - FX);
			ty = (short) (CDY[i] + (CDH[i] >> 1) - FY);
			switch (style) {
			case TRANS_NONE:
				break;
			case TRANS_H:
				CDX[i] = (short) (FX - tx - (CDW[i] >> 1));
				break;
			case TRANS_V:
				CDY[i] = (short) (FY - ty - (CDH[i] >> 1));
				break;
			case TRANS_HV:
				CDX[i] = (short) (FX - tx - (CDW[i] >> 1));
				CDY[i] = (short) (FY - ty - (CDH[i] >> 1));
				break;
			case TRANS_90:
				temp = CDW[i];
				CDW[i] = CDH[i];
				CDH[i] = temp;
				CDX[i] = (short) (FX + ty - (CDW[i] >> 1));
				CDY[i] = (short) (FY - tx - (CDH[i] >> 1));
				break;
			case TRANS_270:
				temp = CDW[i];
				CDW[i] = CDH[i];
				CDH[i] = temp;
				CDX[i] = (short) (FX - ty - (CDW[i] >> 1));
				CDY[i] = (short) (FY + tx - (CDH[i] >> 1));
				break;
			case TRANS_H90:
				temp = CDW[i];
				CDW[i] = CDH[i];
				CDH[i] = temp;
				CDX[i] = (short) (FX + ty - (CDW[i] >> 1));
				CDY[i] = (short) (FY + tx - (CDH[i] >> 1));
				break;
			case TRANS_V90:
				temp = CDW[i];
				CDW[i] = CDH[i];
				CDH[i] = temp;
				CDX[i] = (short) (FX - ty - (CDW[i] >> 1));
				CDY[i] = (short) (FY - tx - (CDH[i] >> 1));
				break;
			}
		}
	}

	/**
	 * 子精灵翻转表
	 */
	static protected final byte SubSprFlipTable[][] = {
			{ // ScreenFlip = NONE
			TRANS_NONE, TRANS_H, TRANS_V, TRANS_HV, TRANS_90, TRANS_270,
					TRANS_H90, TRANS_V90 },
			{
					// ScreenFlip = H
					TRANS_H, TRANS_NONE, TRANS_HV, TRANS_V, TRANS_H90,
					TRANS_V90, TRANS_90, TRANS_270 },
			{
					// ScreenFlip = V
					TRANS_V, TRANS_HV, TRANS_NONE, TRANS_H, TRANS_V90,
					TRANS_H90, TRANS_270, TRANS_90 },
			{
					// ScreenFlip = HV
					TRANS_HV, TRANS_V, TRANS_H, TRANS_NONE, TRANS_270,
					TRANS_90, TRANS_V90, TRANS_H90 },
			{
					// ScreenFlip = 90
					TRANS_90, TRANS_V90, TRANS_H90, TRANS_270, TRANS_HV,
					TRANS_NONE, TRANS_H, TRANS_V, },
			{
					// ScreenFlip = 270
					TRANS_270, TRANS_H90, TRANS_V90, TRANS_90, TRANS_NONE,
					TRANS_HV, TRANS_V, TRANS_H, },
			{
					// ScreenFlip = H 90
					TRANS_H90, TRANS_270, TRANS_90, TRANS_V90, TRANS_V,
					TRANS_H, TRANS_NONE, TRANS_HV, },
			{
					// ScreenFlip = V 90
					TRANS_V90, TRANS_90, TRANS_270, TRANS_H90, TRANS_H,
					TRANS_V, TRANS_HV, TRANS_NONE, } };

	/**
	 * 得到翻转号
	 * 
	 * @param trans
	 * @return
	 */
	final static private int getFlipIndex(byte trans) {
		switch (trans) {
		case TRANS_NONE:
			return 0;
		case TRANS_H:
			return 1;
		case TRANS_V:
			return 2;
		case TRANS_HV:
			return 3;
		case TRANS_90:
			return 4;
		case TRANS_270:
			return 5;
		case TRANS_H90:
			return 6;
		case TRANS_V90:
			return 7;
		}
		return -1;
	}

	/**
	 * 从当前状态翻转到下一状态
	 * 
	 * @param currentTrans
	 *            上个状态
	 * @param nextTrans
	 *            要翻转的状态
	 * @return 下一个状态
	 */
	final static public byte transFlipNext(byte currentTrans, byte nextTrans) {
		return SubSprFlipTable[getFlipIndex(currentTrans)][getFlipIndex(nextTrans)];
	}

	// --------------------------------------------------------------------------------------------------------
	/**
	 * 渲染精灵
	 * 
	 * @param g
	 */
	final public void render(Graphics g) {
		if (Visible) {
			for (int i = Frames[Animates[CurAnimate][CurFrame]].length - 1; i >= 0; i--) {
				tile.Render(g,
						STileID[Frames[Animates[CurAnimate][CurFrame]][i]],
						HPos + SX[Frames[Animates[CurAnimate][CurFrame]][i]]
								+ OffsetX, //
						VPos + SY[Frames[Animates[CurAnimate][CurFrame]][i]]
								+ OffsetY, //
						SFlip[Frames[Animates[CurAnimate][CurFrame]][i]]);
			}
		}
	}

	/**
	 * 画判定框
	 * 
	 * @param g
	 * @param c
	 *            颜色
	 */
	final public void renderDebugCD(javax.microedition.lcdui.Graphics g, int c) {
		if (Visible)
			for (int i = 0; i < CDIndex; i++) {
				if (CDStatus[i] != 0) {
					g.setColor(c >>> 8 * i);
					g.drawRect(HPos + OffsetX + CDX[i],
							VPos + OffsetY + CDY[i], CDW[i] - 1, CDH[i] - 1);
					g.fillRect(HPos + OffsetX + FX, VPos + OffsetY + FY, 1, 1);
				}
			}
	}

	/**
	 * 画指定判定框
	 * 
	 * @param g
	 * @param i
	 *            碰撞块索引
	 * @param c
	 *            颜色
	 */
	final public void renderDebugCDBlock(javax.microedition.lcdui.Graphics g,
			int i, int c) {
		if (Visible && CDStatus[i] != 0) {
			g.setColor(c >>> (8 * i));
			g.drawRect(HPos + OffsetX + CDX[i], VPos + OffsetY + CDY[i],
					CDW[i] - 1, CDH[i] - 1);
			g.fillRect(HPos + OffsetX + FX, VPos + OffsetY + FY, 1, 1);
		}
	}

	// ----------------------------------------------------------------------------------------------------
	/**
	 * 判断精灵和一个矩形是否相撞
	 * 
	 * @param obj
	 * @param block
	 * @param dx1 
	 * @param dy1
	 * @param dw
	 * @param dh
	 * @return false:true
	 */
	final static public boolean touchRect(GSprite obj, int block, int dx1,
			int dy1, int dw, int dh) {
		int sx1, sy1, sw, sh;
		sx1 = obj.HPos + obj.CDX[block];
		sy1 = obj.VPos + obj.CDY[block];
		sw = obj.CDW[block];
		sh = obj.CDH[block];
		return (GMath.cdRect(sx1, sy1, sw, sh, dx1, dy1, dw, dh));
	}

	/**
	 * 判断2个精灵的判定块是否相撞
	 * 
	 * @param obj1
	 *            精灵1
	 * @param block1
	 *            精灵1的判定块ID
	 * @param obj2
	 *            精灵2
	 * @param block2
	 *            精灵2的判定块ID
	 * @return false:true
	 */
	final static public boolean touchBlock(GSprite obj1, int block1,//
			GSprite obj2, int block2)//
	{
		return (GMath.cdRect(obj1.HPos + obj1.CDX[block1], obj1.VPos
				+ obj1.CDY[block1], obj1.CDW[block1], obj1.CDH[block1],
				obj2.HPos + obj2.CDX[block2], obj2.VPos + obj2.CDY[block2],
				obj2.CDW[block2], obj2.CDH[block2]));
	}

	/**
	 * 根据属性判断2个精灵的判定块是否相撞
	 * 
	 * @param obj1
	 *            精灵1
	 * @param block1
	 *            精灵1的判定块ID
	 * @param obj2
	 *            精灵2
	 * @param block2
	 *            精灵2的判定块ID
	 * @return false:true
	 */
	final static public boolean touchAdvance(GSprite obj1, int block1,//
			GSprite obj2, int block2)//
	{
		if ((obj1.CDStatus[block1] & obj2.CDStatus[block2]) == 0)
			return false;
		return (GMath.cdRect(obj1.HPos + obj1.CDX[block1], obj1.VPos
				+ obj1.CDY[block1], obj1.CDW[block1], obj1.CDH[block1],
				obj2.HPos + obj2.CDX[block2], obj2.VPos + obj2.CDY[block2],
				obj2.CDW[block2], obj2.CDH[block2]));
	}

	/**
	 * 克隆该精灵,所有的引用均为对母体的引用,克隆体只有不稳定的属性
	 * 
	 * @return 新的克隆体
	 */
	public GSprite clone(/* int HPos,int VPos */) {
		GSprite clone = new GSprite();
		// 所有的引用均为对母体的引用，克隆体只有挥发属性 volatile
		clone.tile = this.tile;
		clone.ID = this.ID;
		clone.Type = this.Type;
		/** ************************ */
		clone.HPos = this.HPos;
		clone.VPos = this.VPos;
		/** ************************ */
		clone.Height = this.Height;
		clone.Width = this.Width;
		clone.Flip = this.Flip;
		clone.FX = this.FX;
		clone.FY = this.FY;
		clone.OnScreen = this.OnScreen;
		clone.Visible = this.Visible;
		clone.Active = this.Active;
		clone.isCross = this.isCross;
		clone.haveSprCD = this.haveSprCD;
		clone.haveMapCD = this.haveMapCD;
		clone.OffsetX = this.OffsetX;
		clone.OffsetY = this.OffsetY;
		clone.SubIndex = this.SubIndex;
		clone.SubCount = this.SubCount;
		/** ************************************************ */
		clone.STileID = this.STileID; // 子精灵TILE号
		clone.SFlip = this.SFlip; // 子精灵的翻转参数
		clone.SW = this.SW; // 子精灵宽
		clone.SH = this.SH; // 子精灵高
		clone.SX = this.SX; // 子精灵和坐标的X偏移
		clone.SY = this.SY; // 子精灵和坐标的Y偏移
		/** ************************************************ */
		clone.CDIndex = this.CDIndex;
		clone.CDCount = this.CDCount;
		clone.CDStatus = this.CDStatus;
		clone.CDX = this.CDX;
		clone.CDY = this.CDY;
		clone.CDW = this.CDW;
		clone.CDH = this.CDH;

		clone.Frames = this.Frames;
		clone.Animates = this.Animates;
		clone.CurAnimate = this.CurAnimate;
		clone.CurFrame = this.CurFrame;
		/** ************************************* */
		if (this.Events != null) {
			clone.Events = new int[this.Events.length];// 精灵的事件
			System.arraycopy(this.Events, 0, clone.Events, 0,
					this.Events.length);
		}
		if (this.EventTime != null) {
			clone.EventTime = new int[this.EventTime.length];// 精灵事件所用的时间
			System.arraycopy(this.EventTime, 0, clone.EventTime, 0,
					this.EventTime.length);
		}
		if (this.Extra != null) {
			clone.Extra = new int[this.Extra.length];// 附加属性
			System.arraycopy(this.Extra, 0, clone.Extra, 0, this.Extra.length);
		}
		/** ************************************* */
		clone.SpeedX256 = this.SpeedX256; // 8位定点数
		clone.SpeedY256 = this.SpeedY256; // 8位定点数
		clone.DirectX = this.DirectX; // 方向
		clone.DirectY = this.DirectY; // 方向
		return clone;
	}
}
