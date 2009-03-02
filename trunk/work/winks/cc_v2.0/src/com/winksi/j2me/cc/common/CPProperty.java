package com.winksi.j2me.cc.common;

import javax.microedition.lcdui.*;

public class CPProperty {

	public static final boolean CMWAP = false;
	public static final String UTF8 = "UTF-8";
	//数据库id
	public static final int RECORDID_RUNTIMES = 0;
	public static final int RECORDID_SPECIALNUM = 1;
	public static final int RECORDID_NumAndImage = 2;
	public static final int RECORDID_DOWNLOAD_CCID = 3;
	public static final int RECORDID_DOWNLOAD_CMCCCCID = 4;
	public static final int RECORDID_DOWNLOAD_IDS = 5;
	public static final int RECORDID_MyPhoneNum = 6;
	public static final int RECORDID_TIME_CONNECTION = 7;
	public static final int RECORDID_NUMANDPIC = 8;
	public static final int RECORDID_TIMESTAMP = 9;
	public static final int RECORDID_ALERMTIME = 10;
	// 数据库名称
	public static final String[] RECORDSTORE_NAME = { "runtimes", "specailnum", "contract", "downloadccid",
			"downloadcmccccid", "totaldownloadid", "phonenumber", "timing", "numandpic", "timestamp", "alermtime" };
	//本地彩像存放路径
	public static final String SAVEPATH = "c:/other/ccpicture";
	//    public static final String SAVEPATH="c:/photos";
	public static final String DIYPATH_C = "c:/camera/100MSDCF";
	public static final String[] DIYPATH_C_NAME = { "(手机记忆库)", "(Phone memeory)", "(行動電話記憶庫)" };
	public static final String DIYPATH_E = "e:/DCIM/100MSDCF";
	public static final String[] DIYPATH_E_NAME = { "(Memory Stick)", "(Memory Stick)", "(Memory Stick)" };
	//    public static final String DIYPATH="c:/image/camera_semc";
	public static final String SMSURL = "sms://:";

	public static final String OPM = "OPM";
	public static final String IMG = "IMG";

//	public static final String CONFIGSERVER_URL = "http://219.143.46.226:5051";//开发confServer
	public static final String CONFIGSERVER_URL = "http://ip.izhayan.com:8100";//测试confServer

	//屏幕信息
	public static final int SCREENWIDTH = 240;
	public static final int SCREENHEIGHT = 320;

	//按键值
	public static final int KEY_NUM0 = 48;
	public static final int KEY_NUM1 = 49;
	public static final int KEY_NUM2 = 50;
	public static final int KEY_NUM3 = 51;
	public static final int KEY_NUM4 = 52;
	public static final int KEY_NUM5 = 53;
	public static final int KEY_NUM6 = 54;
	public static final int KEY_NUM7 = 55;
	public static final int KEY_NUM8 = 56;
	public static final int KEY_NUM9 = 57;
	public static final int KEY_STAR = 42;
	public static final int KEY_POUND = 35;
	public static final int KEY_UP = -1;
	public static final int KEY_DOWN = -2;
	public static final int KEY_LEFT = -3;
	public static final int KEY_RIGHT = -4;
	public static final int KEY_OK = -5;
	public static final int KEY_SOFT_LEFT = -6;
	public static final int KEY_SOFT_RIGHT = -7;
	public static final int KEY_CLEAR = -8;
	public static final int KEY_BACK = -11;
	public static final int KEY_SOUND_UP = -36;
	public static final int KEY_SOUND_DWON = -37;

	//字体信息
	public static Font sFont = Font.getFont(Font.FACE_SYSTEM, Font.STYLE_PLAIN, Font.SIZE_SMALL);
	public static int sfheight = sFont.getHeight();
	public static int sfweight = sFont.stringWidth("我");
	public static Font defaultFont = Font.getDefaultFont();
	public static int dfheight = defaultFont.getHeight();
	public static int dfweight = defaultFont.stringWidth("我");
	public static Font lFont = Font.getFont(Font.FACE_SYSTEM, Font.STYLE_BOLD, Font.SIZE_LARGE);
	public static int lfheight = lFont.getHeight();
	public static int lfweight = lFont.stringWidth("我");
	public static Font mFont = Font.getFont(Font.FACE_SYSTEM, Font.STYLE_PLAIN, Font.SIZE_MEDIUM);
	public static int mfheight = mFont.getHeight();
	public static int mfweight = mFont.stringWidth("我");
	//行列间距
	public static final int MAX_LINE = 8;
	public static final int ROW_SPACE = 3;
	public static final int COL_SPACE = 3;
	public static final int TITLE_OFF_X = 52;
	public static final int TITLE_OFF_Y = 36;
	//配色方案
	public static final int COLOR_BG = 0Xeff0fb; //
	public static final int COLOR_CHAR_BLACK = 0X000000; //
	public static final int COLOR_CHAR_WHITE = 0XFFFFFF; //
	public static final int COLOR_CHAR_BLUE = 0X0000ff; //
	public static final int COLOR_CHAR_RED = 0XFF0000; //
	public static final int COLOR_BUTTON_BG = 0x4460a3;//
	public static final int COLOR_BUTTON_FRAME = 0x333433;//
	public static final int COLOR_MENU_FRAME = 0x333433;//
	public static final int COLOR_MENU_CHAR_SELECT = 0x5e6567;
	public static final int COLOR_MENU_BG = COLOR_CHAR_BLACK;//
	//电话薄标签
	public static final int CN = 0;
	public static final int EN = 1;
	public static final int TW = 2;

	public static final int MATCH_MAX = 7;
	public static final int UID = 0;
	public static final int FULLNAME = 1;
	public static final int TITLE = 2;
	public static final int COMPANY = 3;
	public static final int EMAIL = 4;
	public static final int WEBADDRESS = 5;
	public static final int INFO = 6;
	public static final int NUMBER = 7;
	public static final int PICTURE = 8;
	public static final int NAME = 9;
	public static final int ADDRESS = 10;
	public static final String[][] CONTRACT_TAG = {
			{ "UID", "全名", "职务", "公司", "电子邮件", "网址", "信息", "号码", "图片", "姓名", "地址" },
			{ "UID", "Full name", "Title", "Company", "Email", "Web address", "Info", "Number", "Picture", "Name",
					"Address" }, { "UID", "全名", "職務", "公司", "電子郵件", "網址", "信息", "號碼", "圖片", "姓名", "地址" } };

	//    public static final String[] PUSH_EXAMPLE={"10086","Mixer.jpg"};//number+id just test
	public static final String[] MAIN_TITLE = { "彩像", "Winks", "綵像" };
	public static final String[][] MAIN_LIST = { { "自定义", "检查更新", "激活" }, { "DIY", "Update", "Activate" },
			{ "自定義", "檢查更新", "啟動" } };
	//command title
	public static final String[] COMMAND_OPTIONS = { "选项", "Option", "選項" };
	public static final String[] COMMAND_OK = { "完成", "Finish", "完成" };
	public static final String[] COMMAND_SURE = { "确定", "Sure", "確定" };
	public static final String[] COMMAND_CANCEL = { "取消", "Cancel", "取消" };
	public static final String[] COMMAND_REVIEW = { "预览", "Review", "預覽" };
	public static final String[] COMMAND_REGISTER = { "激活", "Activate", "啟動" };
	public static final String[] COMMAND_NEXT = { "下一步", "Next", "下一步" };
	public static final String[] COMMAND_SKIP = { "跳过", "Skip", "跳過" };
	public static final String[] COMMAND_HELP = { "帮助", "Help", "幫助" };
	public static final String[] COMMAND_ABOUT = { "关于", "About", "關于" };
	public static final String[] COMMAND_UPLOAD = { "上传", "Upload", "上傳" };
	public static final String[] COMMAND_BACK = { "返回", "Back", "返回" };
	public static final String[] COMMAND_EXIT = { "退出", "Exit", "退出" };

	//screen infomation
	public static final String[] PHONE_NUMBER = { "手机号", "Phone number", "手機號" };
	//    public static final String[] PASSWORD={"密码","PASSWORD","密碼"};
	public static final String[] EXIT_TITLE = { "退出", "Exit", "退出" };
	public static final String[] EXIT_INFO = { "是否退出彩像软件?", "Exit winks?", "是否退出彩像軟體?" };
	public static final String[] SET_GUIDE = { "设置", "Setting", "設置" };
	public static final String[] SET_GUIDE_SURE = { "确认设置", "Sure Setting", "確認設置" };
	public static final String[] SURE_SETTING = { "是否按照提示步骤设置？", "Follow the prompt steps to set?", "是否按照提示步驟設置？" };
	public static final String[] INPUT_INFO = { "请您输入正确的手机号码，才能使用彩像业务。",
			"Please enter your correct mobile phone number ,allows for the normal use of the Winks service.",
			"請您輸入正確的行動電話號碼，才能使用彩像業務。" };
	public static final String[] DECLARE = { "隐私声明", "Privacy Statement", "隱私聲明" };
	public static final String[] INIT_PIM = { "正在进行系统初始化，请稍等…", "System initializing, please wait...", "正在進行系統初始化，請稍等…" };
	public static final String[] INIT_NET = { "正在进行网络初始化，请稍等…", "Network initializationing，please wait",
			"正在進行網路初始化，請稍等…" };
	public static final String[] WAIT_INFO = { "正在与服务器连接，请稍候…", "Requesting data, please wait", "正在與伺服器連接，請稍後…" };
	public static final String[] SENDSMS_INFO = { "激活中...", "Activate...", "啟動中..." };
	public static final String[] UPLOADPIM_IMFO = { "上传电话簿", "Upload pim", "上傳電話簿" };
	public static final String[] UPLOADIMG_INFO = { "上传", "Upload", "上傳" };
	public static final String[] UPLOADGREETING_INFO = { "上传问候语", "Upload Greetings", "上傳問候語" };
	public static final String[] UPLOADIMG_SUCCEED_INFO = { "上传成功", "Succeed", "上傳成功" };
	public static final String[] UPLOADIMG_FAULT_INFO = { "请选择上传DIY图像或编辑问候语才能设置对方用户.", "No content for uploading.",
			"請選擇上傳DIY圖像或編輯問候語才能設置對方用戶." };
	public static final String[] DOWNLOAD_CCFILE = { "正在处理好友数据，请稍候…", "Downloading...", "正在處理好友資料，請稍候…" };
	public static final String[] ALREADY_REGISTER = { "您已经是彩像用户。", "You are already an Winks user.", "您已經是彩像用戶。" };
	public static final String[] INIT_IMGURL = { "正在进行系统初始化…", "System initializing...", "正在進行系統初始化…" };
	public static final String[] INIT_LOADING_PIM = { "正在读取 ", "Loading ", "正在讀取 " };
	public static final String[] INIT_MODIFY_PIM = { "正在更新 ", "Updating ", "正在更新 " };

	public static final int MAX_INPUT = 100;
	public static final String[] GREETING_TITLE = { "编辑问候语(100字)", "Edit Greetings(100)", "編輯問候語(100字)" };
	public static final String[][] DIY_LIST = { { "上传DIY图片", "Upload DIY winks", "上傳DIY圖片" },
			{ "编辑问候语", "Edit Greetings", "編輯問候語" } };
	public static final String[] DIY_SELECTUSER_LIST = { "选择对方用户", "Select user", "選擇對方用戶" };
	public static final String[] DIY_SELECTNUMBER_LIST = { "选择用户号码", "Select number", "選擇用戶號碼" };

	public static final String[] ERROR_NO_FILE = { "无文件", "No files", "無文件" };
	public static final String[] ERROR_NO_FILE1 = { "该图片不存在，请选择其他图片", "not exist，please select another",
			"該圖片不存在，請選擇其他圖片" };
	public static final String[] ERROR_IMG_NULL = { "无法上传500K以上的图片", "Can not upload pictures of over 500K.",
			"無法上傳500K以上的圖片." };
	public static final String[] ERROR_REGISTER = { "激活超时!请检查输入手机号是否正确",
			"Activate expired! Please check for the correctness of your phone numbe.", "啟動超時!請檢查輸入行動電話號碼是否正確." };
	public static final String[] ERROR_CONTROL = { "操作错误，将返回上次操作", "Operation error, return to the previous operation",
			"操作錯誤，將返回上次操作" };
	public static final String[] ERROR_INPUT = { "手机号码不能为空，请重新输入。",
			"Mobile phone number can not be blank, please re-enter.", "行動電話號碼能為空，請重新輸入。" };
	public static final String[] ERROR_UPDATASOFTWARE = { "软件已是最新版本，无需更新。", "Software is the newest.", "軟體已是最新版本，無需更新。" };
	public static final String[] INFO_UPDATASOFTWARE = { "软件有新版本，请更新。", "New software edition, please update.",
			"軟體有新版本，請更新。" };
}
