package luke.games.engines.gLib;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

/**
 * 最底层的对图片的处理。 里面包含了一组图片的集合。 可以把一个大Image作为原图切成若干块小Image，也可以使用若干Image.
 * 
 * 
 */
public class GTileGroup {
	// public static Graphics g;
	/** TILE块的个数 */
	public int Count;

	private int CurIndex;
	private Image TileImage;
	private Image[] Tiles;

	// --------------------------------------------------------------------------------------------------------
	/**
	 * 生成一个TileGroup
	 * 
	 * @param NewTileImage
	 *            原图片
	 * @param NewTileCount
	 *            切出的图的个数
	 */
	public GTileGroup(Image NewTileImage, int NewTileCount) {
		Count = NewTileCount;
		Tiles = new Image[NewTileCount];
		CurIndex = 0;
		SetTileImage(NewTileImage);
	}

	/**
	 * 清除垃圾内存。 初始化完TILE后必须做的事情
	 * 
	 */
	final public void gc() {
		TileImage = null;
		System.gc();
		try {
			Thread.sleep(1);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		GUtil.print(this.toString() + " TileImage is Killed !");
	}

	/**
	 * 得到子图的宽度
	 * 
	 * @param Index
	 *            子图序号
	 * @return 宽度
	 */
	final public int GetWidth(int Index) {
		return Tiles[Index].getWidth();
	}

	/**
	 * 得到子图的高度
	 * 
	 * @param Index
	 *            子图序号
	 * @return 高度
	 */
	final public int GetHeight(int Index) {
		return Tiles[Index].getHeight();
	}

	/**
	 * 设置另一张图片做为原图
	 * 
	 * @param NewTileImage
	 */
	final public void SetTileImage(Image NewTileImage) {
		TileImage = NewTileImage;
		System.gc();
		try {
			Thread.sleep(1);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	/**
	 * 把原图整张的添加到组里
	 * 
	 * @return 是否未装满
	 */
	final public boolean AddTile() {
		if (CurIndex < Count) {
			if (TileImage == null) {
				Tiles[CurIndex] = Image.createImage(1, 1);
			} else {
				Tiles[CurIndex] = TileImage;
			}
			CurIndex++;
		}
		if (CurIndex >= Count) {
			gc();
			return false;
		} else {
			return true;
		}
	}

	/**
	 * 把原图的一个区域切成Count个小图，放进组里
	 * 
	 * @param TileX
	 *            切原图的x起点
	 * @param TileY
	 *            切原图的y起点
	 * @param TileWidth
	 *            切原图的宽
	 * @param TileHeight
	 *            切原图的高
	 * @return false 是否未装满
	 */
	final public boolean AddTile(int TileX, int TileY, int TileWidth,
			int TileHeight) {
		if (CurIndex < Count) {
			if (TileWidth <= 0 || TileHeight <= 0) {
				if (TileImage == null) {
					Tiles[CurIndex] = Image.createImage(1, 1);
				} else {
					Tiles[CurIndex] = Image.createImage(TileImage, 0, 0, 1, 1,
							0);
				}
			} else {
				Tiles[CurIndex] = Image.createImage(TileImage, TileX, TileY,
						TileWidth, TileHeight, 0);
			}
			CurIndex++;
		}
		if (CurIndex >= Count) {
			gc();
			return false;
		} else {
			return true;
		}
	}

	/**
	 * 把原图按照网格切成若干小图，放进组里
	 * 
	 * @param ClipX
	 *            切原图的x起点
	 * @param ClipY
	 *            切原图的y起点
	 * @param ClipWidth
	 *            切原图的整个宽
	 * @param ClipHeight
	 *            切原图的整个高
	 * @param TileWidth
	 *            每块的宽
	 * @param TileHeight
	 *            每块的高
	 */
	final public void AddTile(int ClipX, int ClipY, int ClipWidth,
			int ClipHeight, int TileWidth, int TileHeight) {
		if (CurIndex < Count) {
			for (int j = 0; j < ClipHeight / TileHeight; j++) {
				for (int i = 0; i < ClipWidth / TileWidth; i++) {
					if (!AddTile(ClipX + TileWidth * i, ClipY + TileHeight * j,
							TileWidth, TileHeight)) {
						return;
					}
				}
			}
		}
	}

	/**
	 * 绘制到指定的Graphics上
	 * 
	 * @param g
	 *            指定的graphics
	 * @param Index
	 *            序号
	 * @param PosX
	 *            X坐标
	 * @param PosY
	 *            Y坐标
	 */
	final public void Render(Graphics g, int Index, int PosX, int PosY) {// 不可选择对齐方式,不可旋转
		g.drawRegion(//
				Tiles[Index], //
				0, //
				0, //
				Tiles[Index].getWidth(), //
				Tiles[Index].getHeight(), //
				0, //
				PosX,//
				PosY,//
				Graphics.TOP | Graphics.LEFT//
		);
	}

	/**
	 * 绘制到指定的Graphics上
	 * 
	 * @param g
	 *            指定的graphics
	 * @param Index
	 *            序号
	 * @param PosX
	 *            X坐标
	 * @param PosY
	 *            Y坐标
	 * @param Style
	 *            翻转方式，参考 CSSprite 里的翻转参数
	 */
	final public void Render(Graphics g, int Index, int PosX, int PosY,
			int Style) {
		g.drawRegion(Tiles[Index], //
				0, 0, //
				Tiles[Index].getWidth(), Tiles[Index].getHeight(), //
				Style, //
				PosX, PosY,//
				Graphics.TOP | Graphics.LEFT//
		);
	}
}