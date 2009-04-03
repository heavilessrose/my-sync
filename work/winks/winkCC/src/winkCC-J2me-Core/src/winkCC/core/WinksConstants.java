/**
 * 
 */
package winkCC.core;

/**
 * @author WangYinghua
 * 
 */
public class WinksConstants {
	public static final String VERSION = "1.1.0";

	public static final String DEFAULT_IMG_RES_FOLDER = "/img/";
	public static final String DEFAULT_I18N_RES_FOLDER = "/i18n/";
	public static final String DEFAULT_I18N_MESSAGES_BUNDLE = DEFAULT_I18N_RES_FOLDER
			+ "messages.properties";

	//=================== 默认 i18n keys
	public static final String SELECT_I18N_KEY = "SELECT";
	public static final String PLEASE_SELECT_I18N_KEY = "PLEASE_SELECT";
	public static final String VALIDATE_I18N_KEY = "VALIDATE";
	public static final String CANCEL_I18N_KEY = "CANCEL";
	public static final String OK_I18N_KEY = "OK";
	public static final String YES_I18N_KEY = "YES";
	public static final String NO_I18N_KEY = "NO";
	public static final String SOFT_LEFT_I18N_KEY = "SOFT_LEFT";
	public static final String SOFT_RIGHT_I18N_KEY = "SOFT_RIGHT";

	//====================平台
	public static final String PLATFORM_SONY_ERICSSON = "sonyericsson";

	//====================内部按键
	public static final int WINKS_KEY_UP = 1 << 0;
	public static final int WINKS_KEY_DOWN = 1 << 1;
	public static final int WINKS_KEY_LEFT = 1 << 2;
	public static final int WINKS_KEY_RIGHT = 1 << 3;
	public static final int WINKS_KEY_FIRE = 1 << 4;

	public static final int WINKS_KEY_SOFT_LEFT = 1 << 5;
	public static final int WINKS_KEY_SOFT_RIGHT = 1 << 6;
	public static final int WINKS_KEY_DELETE = 1 << 7;
	public static final int WINKS_KEY_BACK = 1 << 8;
	public static final int WINKS_KEY_PENCIL = 1 << 9; // Nokia s60有此键

	public static final int WINKS_KEY_1 = 1 << 10;
	public static final int WINKS_KEY_2 = 1 << 11;
	public static final int WINKS_KEY_3 = 1 << 12;
	public static final int WINKS_KEY_4 = 1 << 13;
	public static final int WINKS_KEY_5 = 1 << 14;
	public static final int WINKS_KEY_6 = 1 << 15;
	public static final int WINKS_KEY_7 = 1 << 16;
	public static final int WINKS_KEY_8 = 1 << 17;
	public static final int WINKS_KEY_9 = 1 << 18;
	public static final int WINKS_KEY_0 = 1 << 19;
	public static final int WINKS_KEY_POUND = 1 << 20;
	public static final int WINKS_KEY_STAR = 1 << 21;

	public static final int NOT_DEFINED_KEY = 1 << 30;
}
