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

	//-------------�û�״̬
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

	//-------------����״̬
	public static byte _subscribeState = 0;
	/** ���� */
	public static final byte SUB_STATE_LIMITED = 1;
	/** ���� */
	public static final byte SUB_STATE_TRIAL = 2;
	/** ���ܹ�� */
	public static final byte SUB_STATE_ADWARE = 3;
	/** ������ */
	public static final byte SUB_STATE_SUBSCRIBER = 4;

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
