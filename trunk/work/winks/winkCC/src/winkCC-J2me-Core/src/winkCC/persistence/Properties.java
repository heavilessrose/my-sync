package winkCC.persistence;

public class Properties {

	public String confServer;
	public String commServer;
	public String ccServer;
	public String smsAddr;
	public String smsPort;
	public String imsi;
	public String uid;
	public String version;
	public String step;
	public String ccPath;

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

	public static void setUserState(byte userState) {
		_userState = userState;
	}

	public static byte getUserState(byte userState) {
		return _userState;
	}

	public static byte getSubscribeState() {
		return _subscribeState;
	}

	public static void setSubscribeState(byte subscribeState) {
		_subscribeState = subscribeState;
	}
}
