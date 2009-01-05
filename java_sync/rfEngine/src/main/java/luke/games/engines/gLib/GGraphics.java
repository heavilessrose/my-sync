package luke.games.engines.gLib;

//import java.io.IOException;
import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

public class GGraphics {
	// static protected Graphics LCDGraphics;

	static final public int TEXT_STYLE_NONE = 0x10;// 文本样式无
	static final public int TEXT_STYLE_SHADOW = 0x20010;// 文本样式阴影
	static final public int TEXT_STYLE_AROUND = 0x3DE10;// 文本样式围绕
	static final public int TEXT_STYLE_CROSS = 0x15410;// 文本样式十字
	static final public int TEXT_STYLE_IMAGE_NUM = 0x40000;// 文本样式数字图片

	static public int FontSize = -1;
	static public Font MainFont;
	static public int StringHeight;
	static private int ImageFontTileStart;
	static public GTileGroup MainImageFont;
	static public int ImageStringHeight;
	static public int ImageFontSpacingH;

	/**
	 * 设置字体大小
	 * 
	 * @param NewFontSize
	 */
	static final public void setFont(int NewFontSize) {
		if (NewFontSize == FontSize) {
			return;
		}
		FontSize = NewFontSize;
		if (FontSize == -1) {
			MainFont = null;
			StringHeight = 0;
		} else {
			MainFont = Font.getFont(Font.FACE_SYSTEM, Font.STYLE_PLAIN,
					FontSize);
			StringHeight = MainFont.getHeight();// 60
		}
	}

	/**
	 * 设置图片数字
	 * 
	 * @param NewMainImageFont
	 * @param NewImageFontSpacingH
	 * @param StartTile
	 */
	static final public void setFont(GTileGroup NewMainImageFont,
			int NewImageFontSpacingH, int StartTile) {
		ImageFontTileStart = StartTile;
		MainImageFont = NewMainImageFont;
		ImageFontSpacingH = NewImageFontSpacingH;
		if (MainImageFont != null) {
			ImageStringHeight = MainImageFont.GetHeight(0);
		}
	}

	/**
	 * 返回字符串宽
	 * 
	 * @param TheString
	 * @return
	 */
	static final public int getStringWidth(String TheString) {
		return MainFont.stringWidth(TheString) - 1;
	}

	/**
	 * 返回图片字符串宽
	 * 
	 * @param TheString
	 * @return
	 */
	static final public int getStringImageWidth(String TheString) {
		if (TheString == null || MainImageFont == null) {
			return 0;
		}
		return TheString.length()
				* (MainImageFont.GetWidth(ImageFontTileStart) + ImageFontSpacingH)
				- ImageFontSpacingH;
	}

	/**
	 * 清理字符串图片
	 */
	static final public void killStringImage() {
		ImageFontTileStart = 0;
		MainImageFont = null;
		ImageStringHeight = 0;
		ImageFontSpacingH = 0;
	}

	/**
	 * 画图片字符串
	 * 
	 * @param g
	 * @param Text
	 * @param StringX
	 * @param StringY
	 */
	final public static void drawStringImage(Graphics g, String Text,
			int StringX, int StringY) {
		if (MainImageFont == null) {
			return;
		}
		// Graphics Tempg = CSTileGroup.g;
		// CSTileGroup.SetGraphics(g);
		int TempX = StringX;
		int Delta = MainImageFont.GetWidth(ImageFontTileStart)
				+ ImageFontSpacingH;// 每字符宽
		for (int i = 0; i < Text.length(); i++) {
			MainImageFont.Render(g, ImageFontTileStart + Text.charAt(i) - 48,
					TempX, StringY);
			TempX += Delta;
		}
		// CSTileGroup.SetGraphics(Tempg);
	}

	/**
	 * 
	 * @param g
	 * @param Text
	 * @param StringX
	 * @param StringY
	 * @param Color
	 * @param TextMask
	 */
	final private static void drawStringMask(Graphics g, String Text,
			int StringX, int StringY, int Color, int TextMask) {
		if (TextMask != 0) {
			int TempX = StringX - 1;
			int TempY = StringY - 1;
			int TempPos = 1;
			g.setColor(Color);
			while (true) {
				if ((TempPos & TextMask) != 0) {
					g.drawString(Text, TempX, TempY, Graphics.LEFT
							| Graphics.TOP);
				}
				TempX++;
				if (TempX > StringX + 1) {
					TempY++;
					if (TempY > StringY + 1) {
						return;
					}
					TempX = StringX - 1;
				}
				TempPos <<= 1;
			}
		}
	}

	/**
	 * 提供文本阴影和文本风格
	 * 
	 * @param g
	 * @param Text
	 * @param StringX
	 * @param StringY
	 * @param Color
	 * @param ShadowColor
	 *            阴影
	 * @param TextStyle
	 */
	final public static void drawString(Graphics g, String Text, int StringX,
			int StringY, int Color, int ShadowColor, int TextStyle) {
		if ((TextStyle & TEXT_STYLE_IMAGE_NUM) != 0) {
			if (Text.charAt(0) >= '0' && Text.charAt(0) <= '9') {
				drawStringImage(g, Text, StringX, StringY);
				return;
			}
		}
		g.setFont(MainFont);
		drawStringMask(g, Text, StringX, StringY, ShadowColor, TextStyle >> 9);
		drawStringMask(g, Text, StringX, StringY, Color, TextStyle);
	}

	/**
	 * 按指定style画指定区域
	 * 
	 * @param g
	 * @param src
	 * @param src_x
	 * @param src_y
	 * @param w
	 * @param h
	 * @param style
	 * @param dst_x
	 * @param dst_y
	 */
	final public static void drawRegion(Graphics g, Image src, int src_x,
			int src_y, int w, int h, int style, int dst_x, int dst_y) {
		// int CX = g.getClipX();
		// int CY = g.getClipY();
		// int CW = g.getClipWidth();
		// int CH = g.getClipHeight();
		// g.setClip(dst_x,dst_y,w,h);
		g.drawRegion(src, src_x, src_y, w, h, style, dst_x, dst_y, Graphics.TOP
				| Graphics.LEFT);
		// g.setClip(CX, CY, CW, CH);
	}

	/**
	 * 在指定矩形框中按指定对齐方式画String
	 * 
	 * @param g
	 * @param Text
	 * @param Color
	 * @param ShadowColor
	 * @param RectX
	 * @param RectY
	 * @param RectWidth
	 * @param RectHeight
	 * @param TextStyle
	 * @param PosStyle
	 */
	final public static void DrawStringAdjustable(Graphics g, String Text,
			int Color, int ShadowColor, int RectX, int RectY, int RectWidth,
			int RectHeight, int TextStyle, int PosStyle) {
		int TempWidth;
		int TempHeight;
		int TempX;
		int TempY;
		if ((TextStyle & TEXT_STYLE_IMAGE_NUM) != 0) {
			TempWidth = getStringImageWidth(Text);
			TempHeight = ImageStringHeight;
		} else {
			TempWidth = getStringWidth(Text);
			TempHeight = StringHeight;
		}
		if ((PosStyle & Graphics.LEFT) != 0) {
			TempX = RectX;
		} else if ((PosStyle & Graphics.RIGHT) != 0) {
			TempX = RectX + RectWidth - TempWidth;
		} else {
			TempX = RectX + ((RectWidth - TempWidth) >> 1);
		}
		if ((PosStyle & Graphics.TOP) != 0) {
			TempY = RectY;
		} else if ((PosStyle & Graphics.BOTTOM) != 0) {
			TempY = RectY + RectHeight - TempHeight;
		} else {
			TempY = RectY + ((RectHeight - TempHeight) >> 1);
		}
		drawString(g, Text, TempX, TempY, Color, ShadowColor, TextStyle);
	}

	// final public static void RenderRectRimAlpha(Graphics g, int RectX,
	// int RectY, int RectWidth, int RectHeight, int RimWidth,
	// int ARGBColor) {
	// RenderRectAlpha(g, RectX, RectY, RectWidth - RimWidth, RimWidth,
	// ARGBColor);
	// RenderRectAlpha(g, RectX + RectWidth - RimWidth, RectY, RimWidth,
	// RectHeight - RimWidth, ARGBColor);
	// RenderRectAlpha(g, RectX + RimWidth, RectY + RectHeight - RimWidth,
	// RectWidth - RimWidth, RimWidth, ARGBColor);
	// RenderRectAlpha(g, RectX, RectY + RimWidth, RimWidth, RectHeight
	// - RimWidth, ARGBColor);
	// }

	// final static public int GetColor(int R, int G, int B) {
	// return ((R & 0x0ff) | ((G << 8) & 0x0ff00) | ((G << 16) & 0x0ff0000));
	// }

	// final public static void DrawFrame(Graphics g,//
	// int FrameX, int FrameY,//框架位置
	// int FrameWidth, int FrameHeight,//框架尺寸
	// int BoarderStyle,//边框样式
	// int ShadowWidth,//阴影宽度
	// String Text,//文本内容
	// int TextStyle,//文本样式
	// int TextPosStyle,//文本对齐样式
	// CSTileGroup TextImages, int TextID,//图形菜单文字
	// int IconWidth,//图标区宽度
	// int IconPosStyle,//图标对齐样式
	// CSTileGroup IconImages, int IconID,//菜单项图标
	// CSTileGroup SkinImages, //图形菜单所用图形
	// int[] Colors//颜色,顺序为0文本颜色，1文本阴影色，2阴影色，3底色,4~(len-1)边框色（从外至内）
	// ) {
	// if (Colors == null) {
	// return;
	// }
	// if (Colors.length < 4) {
	// return;
	// }
	// int TempBoaderWidth = Colors.length - 4;
	//
	// Graphics Tempg = CSTileGroup.g;
	// CSTileGroup.SetGraphics(g);
	// //绘制底板平铺图
	// // 平铺背景
	// if (SkinImages != null) {
	// SkinImages.RenderFillRect(3, FrameX, FrameY, FrameWidth,
	// FrameHeight, CSSprite.TRANS_NONE);
	// }
	//
	// // 绘制底板
	// for (int i = Colors.length - 5; i >= 0; i--) {
	// RenderRectRimAlpha(g, FrameX + i, FrameY + i,
	// FrameWidth - (i << 1), FrameHeight - (i << 1), 1,
	// Colors[i + 4]);
	// }
	// RenderRectAlpha(g, FrameX + TempBoaderWidth, FrameY + TempBoaderWidth,
	// FrameWidth - (TempBoaderWidth << 1), FrameHeight
	// - (TempBoaderWidth << 1), Colors[3]);
	//
	// // 绘制阴影
	// if (ShadowWidth > 0) {
	// RenderRectAlpha(g, FrameX + FrameWidth, FrameY + ShadowWidth,
	// ShadowWidth, FrameHeight, Colors[2]);
	// RenderRectAlpha(g, FrameX + ShadowWidth, FrameY + FrameHeight,
	// FrameWidth - ShadowWidth, ShadowWidth, Colors[2]);
	// }
	// // 绘制边框图片
	// if (SkinImages != null) {
	// switch (BoarderStyle) {
	// case BOARDER_STYLE_IMAGE: {
	// if (SkinImages.Count >= 2) {
	// // 绘制上边沿
	// SkinImages
	// .Render(
	// 1,
	// FrameX
	// - ((SkinImages.GetWidth(1) - FrameWidth) >> 1),
	// FrameY
	// - ((SkinImages.GetHeight(1) - TempBoaderWidth) >> 1));
	// // 绘制下边沿
	// SkinImages
	// .Render(
	// 1,
	// FrameX
	// - ((SkinImages.GetWidth(1) - FrameWidth) >> 1),
	// FrameY
	// + FrameHeight
	// - ((SkinImages.GetHeight(1) + TempBoaderWidth) >> 1),
	// CSSprite.TRANS_V);
	// }
	// if (SkinImages.Count >= 3) {
	// // 绘制左边沿
	// SkinImages
	// .Render(
	// 2,
	// FrameX
	// - ((SkinImages.GetHeight(2) - TempBoaderWidth) >> 1),
	// FrameY
	// - ((SkinImages.GetWidth(2) - FrameHeight) >> 1),
	// CSSprite.TRANS_270);
	// // 绘制右边沿
	// SkinImages
	// .Render(
	// 2,
	// FrameX
	// + FrameWidth
	// - ((SkinImages.GetHeight(2) + TempBoaderWidth) >> 1),
	// FrameY
	// - ((SkinImages.GetWidth(2) - FrameHeight) >> 1),
	// CSSprite.TRANS_90);
	// }
	// break;
	// }
	// case BOARDER_STYLE_CONCATENATION_IMAGE: {
	// int TempPos;
	// int TempPos1;
	// int TempPos2;
	//
	// if (SkinImages.Count >= 2) {
	// if (SkinImages.GetWidth(1) > 0) {
	// TempPos1 = FrameY
	// - ((SkinImages.GetHeight(1) - TempBoaderWidth) >> 1);
	// TempPos2 = FrameY
	// + FrameHeight
	// - ((SkinImages.GetHeight(1) + TempBoaderWidth) >> 1);
	// // 绘制上边沿
	// SkinImages
	// .RenderFillRect(1, FrameX, TempPos1,
	// FrameWidth, SkinImages.GetHeight(1),
	// CSSprite.TRANS_NONE);
	// // 绘制下边沿
	// SkinImages.RenderFillRect(1, FrameX, TempPos2,
	// FrameWidth, SkinImages.GetHeight(1), CSSprite.TRANS_V);
	// }
	// }
	// if (SkinImages.Count >= 3) {
	// if (SkinImages.GetWidth(2) > 0) {
	// TempPos1 = FrameX
	// - ((SkinImages.GetHeight(2) - TempBoaderWidth) >> 1);
	// TempPos2 = FrameX
	// + FrameWidth
	// - ((SkinImages.GetHeight(2) + TempBoaderWidth) >> 1);
	// // 绘制左边沿
	// SkinImages
	// .RenderFillRect(2, TempPos1, FrameY, SkinImages
	// .GetHeight(2), FrameHeight, CSSprite.TRANS_270);
	// // 绘制右边沿
	// SkinImages.RenderFillRect(2, TempPos2, FrameY,
	// SkinImages.GetHeight(2), FrameHeight, CSSprite.TRANS_90);
	// }
	// }
	// break;
	// }
	// }
	// if (SkinImages.Count >= 1) {
	// //左上角
	// SkinImages
	// .Render(
	// 0,
	// FrameX
	// - ((SkinImages.GetWidth(0) - TempBoaderWidth) >> 1),
	// FrameY
	// - ((SkinImages.GetHeight(0) - TempBoaderWidth) >> 1));
	// //右上角
	// SkinImages
	// .Render(
	// 0,
	// FrameX
	// + FrameWidth
	// - ((SkinImages.GetWidth(0) + TempBoaderWidth) >> 1),
	// FrameY
	// - ((SkinImages.GetHeight(0) - TempBoaderWidth) >> 1),
	// CSSprite.TRANS_H);
	// //左下角
	// SkinImages
	// .Render(
	// 0,
	// FrameX
	// - ((SkinImages.GetWidth(0) - TempBoaderWidth) >> 1),
	// FrameY
	// + FrameHeight
	// - ((SkinImages.GetHeight(0) + TempBoaderWidth) >> 1),
	// CSSprite.TRANS_V);
	// //右下角
	// SkinImages
	// .Render(
	// 0,
	// FrameX
	// + FrameWidth
	// - ((SkinImages.GetWidth(0) + TempBoaderWidth) >> 1),
	// FrameY
	// + FrameHeight
	// - ((SkinImages.GetHeight(0) + TempBoaderWidth) >> 1),
	// CSSprite.TRANS_HV);
	// }
	// }
	// //绘制图标
	// int TempIconHeight = FrameHeight - (TempBoaderWidth << 1);
	// if (IconImages != null && IconID >= 0) {
	// IconImages.RenderAdjustable(
	// //
	// IconID,//
	// FrameX + TempBoaderWidth,//
	// FrameY + TempBoaderWidth,//
	// IconWidth,//
	// TempIconHeight,//
	// CSSprite.TRANS_NONE, IconPosStyle);
	// }
	// //绘制文本
	// if (TextImages != null) {
	// TextImages.RenderAdjustable(
	// //
	// TextID,//
	// FrameX + TempBoaderWidth + IconWidth,//
	// FrameY + TempBoaderWidth,//
	// FrameWidth - IconWidth - (TempBoaderWidth << 1),//
	// TempIconHeight,//
	// CSSprite.TRANS_NONE, TextPosStyle);
	// }
	// if (Text != null) {
	// DrawStringAdjustable(g, Text, Colors[0], Colors[1], FrameX
	// + TempBoaderWidth + IconWidth,//
	// FrameY + TempBoaderWidth,//
	// FrameWidth - IconWidth - (TempBoaderWidth << 1),//
	// TempIconHeight, TextStyle, TextPosStyle);
	// }
	// CSTileGroup.SetGraphics(Tempg);
	// }

	// final public static void fillRectAlpha(Graphics g, int RectX, int RectY,
	// int RectWidth, int RectHeight, int ARGBColor) {
	// if ((ARGBColor & 0xff000000) == 0xff000000) {
	// g.setColor(ARGBColor);
	// g.fillRect(RectX, RectY, RectWidth, RectHeight);
	// } else if ((ARGBColor & 0xff000000) != 0x00000000) {
	// int[] ARGB = new int[RectWidth * RectHeight];
	// ARGB[0] = ARGBColor;
	// int TempPos = 1;
	// while (TempPos < ARGB.length) {
	// int TempLen = TempPos;
	// if (TempPos + TempLen > ARGB.length) {
	// TempLen = ARGB.length - TempPos;
	// }
	// System.arraycopy(ARGB, 0, ARGB, TempPos, TempLen);
	// TempPos += TempLen;
	// }
	// LCDGraphics.drawRGB(ARGB, 0, RectWidth, RectX, RectY, RectWidth,
	// RectHeight, true);
	// }
	// }

}