package winkCC.DIY;

public class Diy {

	/**
	 * ����http����ģ��, �ϴ�DIY����.(Э��6.2.6)
	 * 
	 * @param url
	 *            post��ַ.
	 * @param data
	 *            Ҫ�ϴ��Ĳ����ļ�����.
	 */
	public static void uploadWinks(String fileName, byte[] data) {
		// ����fileName��֯url
		String url = "/user/{uid}/winks/upload?fn={fileName}";
	}
}
