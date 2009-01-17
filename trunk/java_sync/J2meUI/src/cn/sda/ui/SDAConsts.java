package cn.sda.ui;

/**
 * @author not attributable
 * @version 1.0
 */
public class SDAConsts {
	// 颜色定义
	public static int clAliceBlue = 0x00F0F8FF;
	public static int clAntiqueWhite = 0x00FAEBD7;
	public static int clGray = 0x00808080;
	public static int clAarkGray = 0x00A9A9A9;
	public static int clMedGray = 0x00A0A0A4;
	public static int clLightslateGray = 0x002F4F4F;
	public static int clBtnFace = 0x00D4D0C8;
	public static int clBtnShadow = clGray;
	public static int clBlack = 0x00000000;
	public static int clWhite = 0x00FFFFFF;
	public static int clActiveCaption = 0x000A246A;
	public static int clFocusShadow = 0x006495ED;
	// 边框
	public static final int bsNone = 0;
	public static final int bsFixedSingle = 1;
	public static final int bsFixed3D = 2;
	// 停靠方式
	public static final int dsNone = 0;
	public static final int dsLeft = 1;
	public static final int dsTop = 2;
	public static final int dsRight = 3;
	public static final int dsBottom = 4;
	public static final int dsFill = 5;
	// 滚动条方式
	public static final int srNone = 0;
	public static final int srHorizontal = 1;
	public static final int srVertical = 2;
	public static final int srBoth = 3;
	// 文字对齐方式
	public static final int alignLeft = 0;
	public static final int alignRight = 1;
	public static final int alignCenter = 2;
	// MessageBox 按钮定义
	public static final int MB_OK = 0;
	public static final int MB_OKCANCEL = 1;
	public static final int MB_YES = 2;
	public static final int MB_YESNO = 4;
	public static final int MB_ICONINFORMATION = 20;
	public static final int MB_ICONQUESTION = 30;
	public static final int MB_ICONWARNING = 40;
	public static final int MB_ICONERROR = 50;
	// MessageBox返回类型
	public static final int mrOk = 1;
	public static final int mrCancel = 2;
	public static final int mrYes = mrOk;
	public static final int mrNo = mrCancel;

	// 键值定义
	/*
	 * RESUME/-43 SUSPEND/-42 USER10/-41 USER9/-40 USER8/-39 USER7/-38 USER6/-37
	 * USER5/-36 USER4/-35 USER3/-34 USER2/-33 USER1/-32 SOFT4/-21 SOFT3/-20
	 * POWER/-12 END/-11 SEND/-10 CLEAR/-8 SOFT2/-7 SOFT1/-6 SELECT/-5 RIGHT/-4
	 * LEFT/-3 DOWN/-2 UP/-1 POUND/35 ASTERISK/42
	 */
	public static final String KEY_UP = "-1";
	public static final String KEY_DOWN = "-2";
	public static final String KEY_LEFT = "-3";
	public static final String KEY_RIGHT = "-4";
	public static final String KEY_ESCAPE = "9";
	public static final String KEY_ASTERISK = "42";
	public static final String KEY_POUND = "35";
	public static final String KEY_SELECT = "-5";
	public static final String KEY_ENTER = "13";
	public static final String KEY_SOFT1 = "-6";
	public static final String KEY_SOFT2 = "-7";
	public static final String KEY_CLEAR = "-8";
	public static final String KEY_SEND = "-10";
	public static final String KEY_END = "-11";
	public static final String KEY_POWER = "-12";
	public static final String KEY_SOFT3 = "-20";
	public static final String KEY_SOFT4 = "-21";
	public static final String KEY_USER1 = "-32";
	public static final String KEY_USER2 = "-33";
	public static final String KEY_USER3 = "-34";
	public static final String KEY_USER4 = "-35";
	public static final String KEY_USER5 = "-36";
	public static final String KEY_USER6 = "-37";
	public static final String KEY_USER7 = "-38";
	public static final String KEY_USER8 = "-39";
	public static final String KEY_USER9 = "-40";
	public static final String KEY_USER10 = "-41";
	public static final String KEY_SUSPEND = "-42";
	public static final String KEY_RESUME = "-43";
	// 下拉类型
	public static final int stDropDowm = 0;
	public static final int stDropDowmList = 1;
	// 布局组件对齐方式
	public static final int amhleft = 0;
	public static final int amhRight = 1;
	public static final int amhCenter = 2;
	public static final int amhSpaceEqually = 3;
	public static final int amvTop = 4;
	public static final int amvBottom = 5;
	public static final int amvCenter = 6;
	public static final int amvSpaceEqually = 7;
	public static final int amWidthEqually = 8;
	public static final int amHeightEqually = 9;
	public static final int amvInCenter = 10;
	public static final int amhInCenter = 11;
	public static final int amParenLeftSpace = 12;
	public static final int amParentTopSpace = 13;
	public static final int amParentRightSpace = 14;
	public static final int amParentBottomSpace = 15;

	// 滚动条等水平垂直等类型
	public static final byte ktHorizontal = 0;
	public static final byte ktVertical = 1;
	// 滚动块的类型
	public static final byte tmTopLeft = 0;
	public static final byte tmBottomRight = 1;
	public static final byte tmBoth = 2;
	// 工具栏里面的滚动按钮的排布
	public static final byte tbsOneSide = 0;
	public static final byte tbsTwoSide = 1;
	// 立体效果
	public static final byte pbNone = 0;
	public static final byte pbLowered = 1;
	public static final byte pbRaised = 2;
	// Bevel外观
	public static int bvTopLine = 0;
	public static int bvLeftLine = 1;
	public static int bvRightLine = 2;
	public static int bvBottomLine = 3;
	public static int bvBox = 4;
	public static int bvFrame = 5;
	public static int bvSpacer = 6;
	// 日期类型
	public static int dkDate = 0;
	public static int dkTime = 1;
	public static int dkDateTime = 2;
	// 按钮图片排列方式
	public static int blGlyphLeft = 0;
	public static int blGlyphRight = 1;
	public static int blGlyphTop = 2;
	public static int blGlyphBottom = 3;
	// 页框按钮位置
	public static int tabButtonsLeft = 0;
	public static int tabButtonsRight = 1;
	public static int tabButtonsTop = 2;
	public static int tabButtonsBottom = 3;
}
