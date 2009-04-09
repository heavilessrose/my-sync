package winkCC.core.store.settings;

public class Settings {
	/**
	 * �����ļ�����·��, Ĭ��ΪSD��winks_ccĿ¼��.
	 */
	public String CC_PATH = "e:/winks_cc/";
	/**
	 * ������ϵ���ļ�winksBook.xml.
	 */
	public String CC_CONTACTS_FILE = "c:/winks_property/winksBook.xml";

	/**
	 * ���ñ�������·��.
	 * 
	 * @param path
	 */
	public void setCCPath(String path) {
		CC_PATH = path;
	}

	/** ���÷�������ַ */
	public String _confServer;
	/** ͨ�ŷ�������ַ */
	public String _commServer;
	/** �����������ַ */
	public String _ccServer;
	/** ���ŵ�ַ */
	public String _smsAddr;
	/** ���Ŷ˿� */
	public String _smsPort;
	/** IMSI�� */
	public String _imsi;
	/** UID */
	public String _uid;
	/** ����汾 */
	public String _version;
	/** �ڼ������� */
	public String _step;

	// -------------�û�״̬
	/** �û�״̬ */
	public static byte _userState = 0;
	/** δ���� */
	public static final byte USR_STATE_NONACTIVED = 0;
	/** ���� */
	public static final byte USR_STATE_ACTIVED = 1;
	/** ��Ĭ */
	public static final byte USR_STATE_SILENT = 2;
	/** �˶� */
	public static final byte USR_STATE_QUIT = 3;
	/** ���� */
	public static final byte USR_STATE_FORBIDEN = 4;

	// -------------����״̬
	/** ����״̬ */
	public static byte _subscribeState = 0;
	/** ���� */
	public static final byte SUB_STATE_LIMITED = 1;
	/** ���� */
	public static final byte SUB_STATE_TRIAL = 2;
	/** ���ܹ�� */
	public static final byte SUB_STATE_ADWARE = 3;
	/** ������ */
	public static final byte SUB_STATE_SUBSCRIBER = 4;

	// --------------------------

	/**
	 * ����ģ���ʼ��. �������ʱ�����������, �͹��ʻ�����.
	 */
	public void init() {

	}

	/**
	 * �趨������
	 * 
	 * @param name
	 *            ������
	 * @param value
	 *            ����ֵ
	 */
	public static void setProperty(String name, String value) {

	}

	/**
	 * ��ȡ����
	 * 
	 * @param name
	 *            ������
	 * @return ���õ�ֵ
	 */
	public static String getProperty(String name) {
		return null;
	}

	/**
	 * �����û�״̬.
	 * 
	 * @param userState
	 */
	public static void setUserState(byte userState) {
		_userState = userState;
	}

	/**
	 * ��ȡ�û�״̬.
	 * 
	 * @return �û�״̬
	 */
	public static byte getUserState() {
		return _userState;
	}

	/**
	 * ��ȡ�û�����״̬.
	 * 
	 * @return ����״̬
	 */
	public static byte getSubscribeState() {
		return _subscribeState;
	}

	/**
	 * �����û�����״̬.
	 * 
	 * @param subscribeState
	 */
	public static void setSubscribeState(byte subscribeState) {
		_subscribeState = subscribeState;
	}
}
