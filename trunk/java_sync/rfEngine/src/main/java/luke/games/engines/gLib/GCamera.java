package luke.games.engines.gLib;

import javax.microedition.lcdui.*;

/**
 * 
 * 游戏卷轴的摄象机</br>
 * 
 * 用于拍摄世界中的某个区域显示在屏幕上
 * 
 */
public class GCamera {
	// 是否画出摄像机框
	public boolean isDebug = true;
	/** ID号 */
	public short ID;
	/** 摄象机在世界中的水平起始坐标 */
	public int HPos;
	/** 摄象机在世界中的垂直起始坐标 */
	public int VPos;
	/** 摄象机在屏幕上的窗口的X坐标 */
	public short CameraX;
	/** 摄象机在屏幕上的窗口的Y坐标 */
	public short CameraY;
	/** 摄象机在屏幕上的窗口宽度 */
	public short CameraW;
	/** 摄象机在屏幕上的窗口高度 */
	public short CameraH;

	private int OldHPos;
	private int OldVPos;

	protected short CameraMatrixW;
	protected short CameraMatrixH;

	private short SrcBlankX;
	private short DstBlankX;
	private short SrcBlankY;
	private short DstBlankY;

	/**
	 * 水平方向,放置
	 */
	protected short MatrixW; 
	protected short MatrixH;
	protected short CellW;
	protected short CellH;

	protected short buffW;
	protected short buffH;
	protected short buffPX;
	protected short buffPY;

	protected short XCount;
	protected short YCount;
	protected short curBuffBlockPX;
	protected short curBuffBlockPY;
	protected short curMapBlockX;
	protected short curMapBlockY;

	protected boolean LOAD_LEFT;
	protected boolean LOAD_RIGHT;
	protected boolean LOAD_UP;
	protected boolean LOAD_DOWN;

	protected Image BackBuffer;
	protected Graphics bg;
	// protected int BackColour;
	protected boolean IsBackBuffer;

	// -----------------------------------------------------------------------------------
	/**
	 * 创建一个摄象机
	 * 
	 * @param x
	 *            在屏幕上的X坐标
	 * @param y
	 *            在屏幕上的Y坐标
	 * @param w
	 *            在屏幕上的宽度
	 * @param h
	 *            在屏幕上的高度
	 * @param cellw
	 *            对应地图格子的宽度
	 * @param cellh
	 *            对应地图格子的高度
	 * @param colour
	 *            背景色
	 * @param isBackBuffer
	 *            是否使用缓冲方式渲染
	 */
	public GCamera(int x, int y, int w, int h, int cellw, int cellh,
			int colour, boolean isBackBuffer) {
		CameraX = (short) x;
		CameraY = (short) y;
		CameraW = (short) w;
		CameraH = (short) h;
		IsBackBuffer = isBackBuffer;

		if (!IsBackBuffer)
			return;

		CameraMatrixW = (short) (w / cellw + 1);
		CameraMatrixH = (short) (h / cellh + 1);
		CellW = (short) cellw;
		CellH = (short) cellh;
		MatrixW = (short) (w / cellw + 2);
		MatrixH = (short) (h / cellh + 2);
		buffW = (short) ((w / cellw + 2) * cellw);
		buffH = (short) ((h / cellh + 2) * cellh);
		BackBuffer = Image.createImage(buffW, buffH);
		GUtil.print("Create BackBuffer : " + buffW + " x " + buffH);
		bg = BackBuffer.getGraphics();
		// BackColour = colour;
		bg.setColor(colour);
		bg.fillRect(0, 0, buffW, buffH);
		init();
	}

	/**
	 * 设置在屏幕上的位置
	 * 
	 * @param x
	 *            在屏幕上的X坐标
	 * @param y
	 *            在屏幕上的Y坐标
	 */
	final public void setWinPos(int x, int y) {
		CameraX = (short) x;
		CameraY = (short) y;
	}

	// ----------------------------------------------------------------------------------------------------
	/**
	 * 得到在世界中的水平中心点坐标
	 * 
	 * @return 在世界中的水平中心点坐标
	 */
	final public int getPosHC() {
		return HPos + (CameraW >> 1);
	}

	/**
	 * 在世界中的垂直坐标中心点
	 * 
	 * @return 在世界中的垂直坐标中心点
	 */
	final public int getPosVC() {
		return VPos + (CameraH >> 1);
	}

	// /**
	// * TODO 得到后备缓冲的Graphics,往后备缓冲的绘画都通过它.
	// * @return 后备缓冲的Graphics
	// */
	// final protected Graphics getGraphics(){
	// return bg;
	// }
	//	
	// ------------------------------------------------------------------------------------------

	/**
	 * TODO 初始化摄象机(清0)
	 */
	final protected void init() {
		OldHPos = 0;
		OldVPos = 0;
		HPos = 0;
		VPos = 0;

		SrcBlankX = 0;
		DstBlankX = (short) (buffW - CameraW);
		SrcBlankY = 0;
		DstBlankY = (short) (buffH - CameraH);
		buffPX = 0;
		buffPY = 0;

		XCount = 0;
		YCount = 0;
		curBuffBlockPX = 0;
		curBuffBlockPY = 0;
		curMapBlockX = 0;
		curMapBlockY = 0;
	}

	// -------------------------------------------------------------------------------------------------
	/**
	 * 移动摄象机
	 * 
	 * @param px
	 *            移动的水平像素
	 * @param py
	 *            移动的垂直像素
	 */
	final protected void mov(int px, int py) {
		OldHPos = HPos;
		OldVPos = VPos;
		HPos += px;
		VPos += py;
		if (!IsBackBuffer)
			return;
		SrcBlankX += px;
		DstBlankX -= px;
		SrcBlankY += py;
		DstBlankY -= py;
		XCount = 0;
		YCount = 0;
		buffPX = (short) adjustBuffPX(HPos);
		buffPY = (short) adjustBuffPY(VPos);
		if (DstBlankX < 0) {
			// turn right
			LOAD_RIGHT = true;
			XCount = (short) (Math.abs(DstBlankX) / CellW + 1);
			curMapBlockX += XCount;
			curBuffBlockPX = (short) adjustBuffBlockPX(curBuffBlockPX + XCount);
			SrcBlankX -= CellW * XCount;
			DstBlankX += CellW * XCount;
		}
		if (SrcBlankX < 0) {
			// turn left
			LOAD_LEFT = true;
			XCount = (short) (Math.abs(SrcBlankX) / CellW + 1);
			curMapBlockX -= XCount;
			curBuffBlockPX = (short) adjustBuffBlockPX(curBuffBlockPX - XCount);
			SrcBlankX += CellW * XCount;
			DstBlankX -= CellW * XCount;
		}

		if (DstBlankY < 0) {
			// turn down
			LOAD_DOWN = true;
			YCount = (short) (Math.abs(DstBlankY) / CellH + 1);
			curMapBlockY += YCount;
			curBuffBlockPY = (short) adjustBuffBlockPY(curBuffBlockPY + YCount);
			SrcBlankY -= CellH * YCount;
			DstBlankY += CellH * YCount;
		}
		if (SrcBlankY < 0) {
			// turn up
			LOAD_UP = true;
			YCount = (short) (Math.abs(SrcBlankY) / CellH + 1);
			curMapBlockY -= YCount;
			curBuffBlockPY = (short) adjustBuffBlockPY(curBuffBlockPY - YCount);
			SrcBlankY += CellH * YCount;
			DstBlankY -= CellH * YCount;
		}
	}

	/**
	 * 
	 * @param sx
	 * @return
	 */
	final protected int adjustBuffBlockPX(int sx) {
		if (sx >= 0)
			return (sx % MatrixW);
		else
			return (MatrixW + sx % MatrixW);
	}

	/**
	 * 
	 * @param sy
	 * @return
	 */
	final protected int adjustBuffBlockPY(int sy) {
		if (sy >= 0)
			return (sy % MatrixH);
		else
			return (MatrixH + sy % MatrixH);
	}

	/**
	 * 
	 * @param sx
	 * @return
	 */
	final protected int adjustBuffPX(int sx) {
		if (sx >= 0)
			return (sx % buffW);
		else
			return (buffW + sx % buffW);
	}

	/**
	 * 
	 * @param sy
	 * @return
	 */
	final protected int adjustBuffPY(int sy) {
		if (sy >= 0)
			return (sy % buffH);
		else
			return (buffH + sy % buffH);
	}

	// ----------------------------------------------------------------------------------------
	final protected void render(Graphics g) {
		// buffPX=adjustBuffPX(HPos);
		// buffPY=adjustBuffPY(VPos);
		int w1 = buffW - buffPX;
		int h1 = buffH - buffPY;
		if (w1 > CameraW)
			w1 = CameraW;
		if (h1 > CameraH)
			h1 = CameraH;
		int w2 = CameraW - w1;
		int h2 = CameraH - h1;
		if (w1 > 0 && h1 > 0) {
			GGraphics.drawRegion(g, BackBuffer, buffPX, buffPY, w1, h1, 0,
					CameraX + 0, CameraY + 0);
		}
		if (w2 > 0 && h2 > 0) {
			GGraphics.drawRegion(g, BackBuffer, 0, 0, w2, h2, 0, CameraX + w1,
					CameraY + h1);
		}
		if (w1 > 0 && h2 > 0) {
			GGraphics.drawRegion(g, BackBuffer, buffPX, 0, w1, h2, 0,
					CameraX + 0, CameraY + h1);
		}
		if (w2 > 0 && h1 > 0) {
			GGraphics.drawRegion(g, BackBuffer, 0, buffPY, w2, h1, 0, CameraX
					+ w1, CameraY + 0);
		}
		if (isDebug) {
			renderDebugWindow(g, 32768);
		}
	}

	/**
	 * 把后备缓冲直接画出来(DEBUG)
	 * 
	 * @param g
	 *            绘图Graphics
	 * @param x
	 *            X坐标
	 * @param y
	 *            Y坐标
	 * @param c
	 *            颜色
	 */
	final protected void renderDebugBackBuffer(Graphics g, int x, int y, int c) {
		if (!IsBackBuffer)
			return;
		g.drawImage(BackBuffer, x, y, Graphics.LEFT | Graphics.TOP);
		g.setColor(c);
		// buffPX=adjustBuffPX(HPos);
		// buffPY=adjustBuffPY(VPos);
		int w1 = buffW - buffPX;
		int h1 = buffH - buffPY;
		if (w1 > CameraW)
			w1 = CameraW;
		if (h1 > CameraH)
			h1 = CameraH;
		int w2 = CameraW - w1;
		int h2 = CameraH - h1;
		if (w1 > 0 && h1 > 0)
			g.drawRect(x + buffPX, y + buffPY, w1, h1);
		if (w2 > 0 && h2 > 0)
			g.drawRect(x + 0, y + 0, w2, h2);
		if (w1 > 0 && h2 > 0)
			g.drawRect(x + buffPX, y + 0, w1, h2);
		if (w2 > 0 && h1 > 0)
			g.drawRect(x + 0, y + buffPY, w2, h1);

	}

	/**
	 * 画摄象机的窗口范围
	 * 
	 * @param g
	 *            绘图Graphics
	 * @param color
	 *            颜色 32768绿
	 */
	public final void renderDebugWindow(Graphics g, int color) {
		g.setColor(color);
		g.drawRect(CameraX - 1, CameraY - 1, CameraW + 1, CameraH + 1);
	}
}
