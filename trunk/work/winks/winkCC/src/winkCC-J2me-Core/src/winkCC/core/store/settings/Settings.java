package winkCC.core.store.settings;

public class Settings {
	/**
	 * 彩像文件保存路径, 默认为SD卡winks_cc目录下.
	 */
	public String CC_PATH = "e:/winks_cc/";
	/**
	 * 彩像联系人文件winksBook.xml.
	 */
	public String CC_CONTACTS_FILE = "c:/winks_property/winksBook.xml";

	/**
	 * 设置保存彩像的路径.
	 * 
	 * @param path
	 */
	public void setCCPath(String path) {
		CC_PATH = path;
	}

	/** 配置服务器地址 */
	public String _confServer;
	/** 通信服务器地址 */
	public String _commServer;
	/** 彩像服务器地址 */
	public String _ccServer;
	/** 短信地址 */
	public String _smsAddr;
	/** 短信端口 */
	public String _smsPort;
	/** IMSI号 */
	public String _imsi;
	/** UID */
	public String _uid;
	/** 软件版本 */
	public String _version;
	/** 第几次启动 */
	public String _step;

	// -------------用户状态
	/** 用户状态 */
	public static byte _userState = 0;
	/** 未激活 */
	public static final byte USR_STATE_NONACTIVED = 0;
	/** 激活 */
	public static final byte USR_STATE_ACTIVED = 1;
	/** 沉默 */
	public static final byte USR_STATE_SILENT = 2;
	/** 退订 */
	public static final byte USR_STATE_QUIT = 3;
	/** 禁用 */
	public static final byte USR_STATE_FORBIDEN = 4;

	// -------------订阅状态
	/** 订阅状态 */
	public static byte _subscribeState = 0;
	/** 受限 */
	public static final byte SUB_STATE_LIMITED = 1;
	/** 试用 */
	public static final byte SUB_STATE_TRIAL = 2;
	/** 接受广告 */
	public static final byte SUB_STATE_ADWARE = 3;
	/** 订阅者 */
	public static final byte SUB_STATE_SUBSCRIBER = 4;

	// --------------------------

	/**
	 * 配置模块初始化. 软件启动时载入软件设置, 和国际化设置.
	 */
	public void init() {

	}

	/**
	 * 设定配置项
	 * 
	 * @param name
	 *            配置项
	 * @param value
	 *            配置值
	 */
	public static void setProperty(String name, String value) {

	}

	/**
	 * 读取配置
	 * 
	 * @param name
	 *            配置名
	 * @return 配置的值
	 */
	public static String getProperty(String name) {
		return null;
	}

	/**
	 * 设置用户状态.
	 * 
	 * @param userState
	 */
	public static void setUserState(byte userState) {
		_userState = userState;
	}

	/**
	 * 获取用户状态.
	 * 
	 * @return 用户状态
	 */
	public static byte getUserState() {
		return _userState;
	}

	/**
	 * 获取用户订阅状态.
	 * 
	 * @return 订阅状态
	 */
	public static byte getSubscribeState() {
		return _subscribeState;
	}

	/**
	 * 设置用户订阅状态.
	 * 
	 * @param subscribeState
	 */
	public static void setSubscribeState(byte subscribeState) {
		_subscribeState = subscribeState;
	}
}
