package winkCC.core.DIY;

public class Diy {
	// �¼�:
	/** DIY�ϴ��ɹ� */
	public static final int DIY_EVENT_UPLOAD_FINISH = 10;
	/** DIY�ϴ��쳣����� */
	public static final int DIY_EVENT_UPLOAD_ERROR = 10 << 1;
	/** �������Ƴɹ� */
	public static final int DIY_EVENT_SCRIPT_FINISH = 10 << 2;
	/** ���������쳣����� */
	public static final int DIY_EVENT_SCRIPT_ERROR = 10 << 3;

	/**
	 * ���ݲ����ҵ��ļ�, ����http����ģ��, �ϴ������ļ�.(Э��6.2.6)
	 * 
	 * @param uid
	 * @param fileName
	 *            �����ļ���.
	 */
	public static void uploadWinks(String uid, String fileName) {
		//		String url = POST /user/{uid}/winks/upload?fn={fileName}
	}

	/**
	 * ��֯Ϊxml�ļ�, ����http����ģ��, ��������. Э��6.2.8
	 * 
	 * @param uid
	 */
	public static void createWinksScript(String uid) {
		//		String url = POST /user/{uid}/script
	}
}
