package winkCC.persistence;

/**
 * 系统设置与国际化.
 * 
 * @author WangYinghua
 * 
 */
public class Properties {
	/**
	 * 彩像文件保存路径, 默认为SD卡winks_cc目录下.
	 */
	public String CC_PATH = "e:/winks_cc/";
	/**
	 * 彩像联系人文件cc_contacts.xml.
	 */
	public String CC_CONTACTS_FILE = "c:/winks_property/cc_contacts.xml";

	/**
	 * 设置保存彩像的路径.
	 * 
	 * @param path
	 */
	public void setCCPath(String path) {
		CC_PATH = path;
	}

	public String _confServer;
	public String _commServer;
	public String _ccServer;
	public String _smsAddr;
	public String _smsPort;
	public String _imsi;
	public String _uid;
	public String _version;
	public String _step;

	//-------------用户状态
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

	//-------------订阅状态
	public static byte _subscribeState = 0;
	/** 受限 */
	public static final byte SUB_STATE_LIMITED = 1;
	/** 试用 */
	public static final byte SUB_STATE_TRIAL = 2;
	/** 接受广告 */
	public static final byte SUB_STATE_ADWARE = 3;
	/** 订阅者 */
	public static final byte SUB_STATE_SUBSCRIBER = 4;

	//--------------------------
	/** 手机当前语言设定 */
	public static String local = "zh-cn";

	/**
	 * 模块初始化. 软件启动时载入软件设置, 和国际化设置.
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
	 * @param userState
	 * @return
	 */
	public static byte getUserState() {
		return _userState;
	}

	/**
	 * 获取用户订阅状态.
	 * 
	 * @return
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

	//---------------------------------------------
	/**
	 * 根据locale载入相应语言.
	 */
	public static void loadLanguage() {

	}

}
