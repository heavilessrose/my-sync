package luke.games.engines.gLib;

import javax.microedition.lcdui.Graphics;

/**
 * 地图。 用于把tile拼成完整的地图
 */
public class GMap {
	/** ID号 */
	public short ID;
	/** 是否支持凳子 */
	public boolean isCross;

	private GTileGroup tile;

	protected byte CellW; // 图块宽
	protected byte CellH; // 图块高

	// 0号为图块, 1号默认为不可穿越实体, 2号默认为登子
	private short CDCount;
	private short CDIndex;
	protected short CDStatus[];
	protected short CDX[];
	protected short CDY[];
	protected short CDW[];// 碰撞块宽
	protected short CDH[];// 碰撞块高

	protected byte[][] MatrixTile; // 图块TILE号表
	protected byte[][] MatrixStyle; // 图块翻转表
	protected byte[][] MatrixTag; // 图块碰撞块索引表

	private byte[][] Animate;
	public byte[] AnimateIndex;
	private short[] AnimateCellX;
	private short[] AnimateCellY;

	// ------Base Method---------------------------------------------
	/**
	 * 创造一个地图
	 * 
	 * @param t
	 *            地图用的TILE组
	 * @param tile_matrix
	 *            地图的TILE的矩阵数据，里面存储TILE图片的索引
	 * @param tag_matrix
	 *            地图的标志的矩阵数据，里面存储碰撞块索引,默认的碰撞为单元格的大小
	 * @param cellw
	 *            地图格的宽度
	 * @param cellh
	 *            地图格的高度
	 * @param crossx
	 *            地图凳子在格子里的X坐标
	 * @param crossy
	 *            地图凳子在格子里的Y坐标
	 * @param crossw
	 *            地图凳子在格子里的宽度
	 * @param crossh
	 *            地图凳子在格子里的高度
	 * @param appendCDCount
	 *            说明地图还可以添加多少种碰撞，默认0
	 */
	public GMap(GTileGroup t, byte[][] tile_matrix, byte[][] tag_matrix,
			int cellw, int cellh, int crossx, int crossy, int crossw,
			int crossh, int appendCDCount) {
		tile = t;
		MatrixTile = tile_matrix;
		MatrixTag = tag_matrix;
		CellW = (byte) cellw;
		CellH = (byte) cellh;
		CDCount = (short) (3 + appendCDCount);
		CDIndex = 0;
		CDStatus = new short[CDCount];
		CDX = new short[CDCount];
		CDY = new short[CDCount];
		CDW = new short[CDCount];// 碰撞块宽
		CDH = new short[CDCount];// 碰撞块高

		addCDBlock(0, 0, 0, cellw, cellh);
		addCDBlock(3, 0, 0, cellw, cellh);
		addCDBlock(3, crossx, crossy, crossw, crossh);

		// 地图中是否有凳子
		if (crossw == 0 || crossh == 0)
			isCross = false;
		else
			isCross = true;

	}

	/**
	 * 设置地图碰撞块
	 * 
	 * @param id
	 *            碰撞块索引
	 * @param status
	 *            碰撞类型
	 * @param x
	 *            每块的X坐标
	 * @param y
	 *            每块的Y坐标
	 * @param w
	 *            每块的宽度
	 * @param h
	 *            每块的高度
	 */
	final public void setCDBlock(int id, int status, int x, int y, int w, int h) {
		CDStatus[id] = (short) status;
		CDX[id] = (short) x;
		CDY[id] = (short) y;
		CDW[id] = (short) w;
		CDH[id] = (short) h;
	}

	/**
	 * 在地图的appendCDCount属性不为0时，添加地图碰撞块，最多可以添加appendCDCount个碰撞块
	 * 
	 * @param status
	 *            碰撞类型
	 * @param x
	 *            每块的X坐标
	 * @param y
	 *            每块的Y坐标
	 * @param w
	 *            每块的宽度
	 * @param h
	 *            每块的高度
	 */
	final public void addCDBlock(int status, int x, int y, int w, int h) {
		CDStatus[CDIndex] = (short) status;
		CDX[CDIndex] = (short) x;
		CDY[CDIndex] = (short) y;
		CDW[CDIndex] = (short) w;
		CDH[CDIndex] = (short) h;
		CDIndex++;
	}

	/**
	 * 得到整个地图宽度
	 * 
	 * @return 地图宽度
	 */
	final public int getWidth() {
		return MatrixTile[0].length * CellW;
	}

	/**
	 * 得到整个地图高度
	 * 
	 * @return 地图高度
	 */
	final public int getHeight() {
		return MatrixTile.length * CellH;
	}

	// -----Matrx Method--------------------------------------------------
	/**
	 * 根据数组创建MAP数据
	 * 
	 * @param tile_matrix
	 *            TILE数组
	 * @param style_matrix
	 *            翻转标志数组
	 * @param tag_matrix
	 *            阻挡标志数组
	 * @param animate_tile
	 */
	final public void newMatrix(byte[][] tile_matrix, byte[][] style_matrix,
			byte[][] tag_matrix, int cellw, int cellh, int crossx, int crossy,
			int crossw, int crossh) {
		MatrixTile = tile_matrix;
		MatrixStyle = style_matrix;
		MatrixTag = tag_matrix;

		if (style_matrix == null) {
			MatrixStyle = new byte[MatrixTile.length][MatrixTile[0].length];
		}

		CellW = (byte) cellw;
		CellH = (byte) cellh;
		addCDBlock(0, 0, 0, cellw, cellh);
		addCDBlock(3, 0, 0, cellw, cellh);
		addCDBlock(3, crossx, crossy, crossw, crossh);
		if (crossw == 0 || crossh == 0)
			isCross = false;
		else
			isCross = true;
	}

	// ------Animate Tile Method-----------------------------------------
	final public void addAnimate(byte[] animate, int x, int y) {
		if (Animate == null) {
			Animate = new byte[1][animate.length];
			// AnimateIndex = new byte[1];
			AnimateCellX = new short[1];
			AnimateCellY = new short[1];

			// AnimateIndex[0]=0;
			AnimateCellX[0] = (short) x;
			AnimateCellY[0] = (short) y;
			System.arraycopy(animate, 0, Animate[0], 0, animate.length);
		} else {
			byte[][] temp1 = new byte[Animate.length + 1][];
			// byte[] temp2 = new byte[Animate.length+1];
			short[] temp3 = new short[Animate.length + 1];
			short[] temp4 = new short[Animate.length + 1];

			// System.arraycopy(AnimateIndex,0,temp2,0,AnimateIndex.length);
			// AnimateIndex = temp2;
			// AnimateIndex[AnimateIndex.length-1] = 0 ;

			System.arraycopy(AnimateCellX, 0, temp3, 0, AnimateCellX.length);
			AnimateCellX = temp3;
			AnimateCellX[AnimateCellX.length - 1] = (short) x;

			System.arraycopy(AnimateCellY, 0, temp4, 0, AnimateCellY.length);
			AnimateCellY = temp4;
			AnimateCellY[AnimateCellY.length - 1] = (short) y;

			for (int i = 0; i < Animate.length; i++) {
				temp1[i] = new byte[Animate[i].length];
				System.arraycopy(Animate[i], 0, temp1[i], 0, Animate[i].length);
			}
			Animate = temp1;
			Animate[Animate.length - 1] = new byte[animate.length];
			System.arraycopy(animate, 0, Animate[Animate.length - 1], 0,
					animate.length);
		}
		System.gc();
	}

	// --------Render Method-------------------------------------------
	/**
	 * 直接画一个地图块
	 * 
	 * @param g
	 * @param bx
	 *            MatrixTile 的x
	 * @param by
	 *            MatrixTile 的y
	 * @param dbx
	 * @param dby
	 */
	final protected void renderDirectCell(Graphics g, int bx, int by, int dbx,
			int dby) {
		if (bx < 0 || bx > MatrixTag[0].length - 1 || by < 0
				|| by > MatrixTag.length - 1)
			return;
		tile.Render(g, MatrixTile[by][bx], dbx * CellW, dby * CellH);
	}

	/**
	 * debug状态时,画地图块及碰撞检测框
	 * 
	 * @param g
	 * @param bx
	 *            MatrixTile 的x
	 * @param by
	 *            MatrixTile 的y
	 * @param dbx
	 * 
	 * @param dby
	 *            开始画地图块的y坐标
	 * @param color
	 */
	final protected void renderDirectDebugCellCD(Graphics g, int bx, int by,
			int dbx, int dby, int color) {
		if (bx < 0 || bx > MatrixTag[0].length - 1 || by < 0
				|| by > MatrixTag.length - 1)
			return;
		tile.Render(g, MatrixTile[by][bx], dbx * CellW, dby * CellH);
		// renderDirectCell(g, bx, by, dbx, dby);

		if (CDStatus[MatrixTag[by][bx]] == 0)
			return;
		if (MatrixTag[by][bx] == 0)
			return;
		g.setColor((color >>> (8 * MatrixTag[by][bx])));
		g.drawRect(dbx * CellW + CDX[MatrixTag[by][bx]], dby * CellH
				+ CDY[MatrixTag[by][bx]], CDW[MatrixTag[by][bx]] - 1,
				CDH[MatrixTag[by][bx]] - 1);
	}

	// /**
	// *
	// * @param g
	// * @param x
	// * @param y
	// */
	// final protected void renderAnimateCell(int x, int y, int timer, int CamX,
	// int CamY, int CamW, int CamH) {
	// if (Animate == null)
	// return;
	// for (int i = Animate.length - 1; i >= 0; i--) {
	// if (AnimateCellX[i] * CellW - x + CamX < CamX + CamW
	// && AnimateCellY[i] * CellH - y + CamY < CamY + CamH
	// && AnimateCellX[i] * CellW - x + CamX + CellW > CamX
	// && AnimateCellY[i] * CellH - y + CamY + CellH > CamY) {
	// // GUtil.print("render Animate Tile : "+i+" as timer
	// // :"+Animate[i][timer%Animate[i].length]);
	// tile.Render(Animate[i][timer % Animate[i].length],
	// AnimateCellX[i] * CellW - x + CamX, AnimateCellY[i]
	// * CellH - y + CamY, 0);
	// }
	// }
	// }

	final protected void renderNoBufferCell(Graphics g, int bx, int by, int x,
			int y) {
		if (bx < 0 || bx > MatrixTag[0].length - 1 || by < 0
				|| by > MatrixTag.length - 1)
			return;
		if (MatrixTile[by][bx] == 0)
			return;
		tile.Render(g, MatrixTile[by][bx], x, y);
	}

	final protected void renderNoBufferDebugCell(Graphics g, int bx, int by,
			int x, int y, int color) {
		if (bx < 0 || bx > MatrixTag[0].length - 1 || by < 0
				|| by > MatrixTag.length - 1)
			return;
		if (CDStatus[MatrixTag[by][bx]] == 0)
			return;
		if (MatrixTag[by][bx] == 0)
			return;
		g.setColor((color >>> (8 * MatrixTag[by][bx])));
		g
				.drawRect(x, y, CDW[MatrixTag[by][bx]] - 1,
						CDH[MatrixTag[by][bx]] - 1);
	}
}
