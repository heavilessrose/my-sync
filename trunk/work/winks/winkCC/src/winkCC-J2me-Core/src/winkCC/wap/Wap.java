/**
 * 
 */
package winkCC.wap;

import javax.microedition.io.ConnectionNotFoundException;
import javax.microedition.midlet.MIDlet;

/**
 * ��װ����������ȵ���wap�Ĳ���.
 * 
 * @author WangYinghua
 * 
 */
public class Wap {

	private Wap() {
	}

	/**
	 * �����ֻ�wap�����.
	 * 
	 * @param midlet
	 * @param url
	 */
	public static void openWap(MIDlet midlet, String url) {
		try {
			boolean is = midlet.platformRequest(url);
			//			if (is) {
			//				midlet.notifyDestroyed();
			//			}
		} catch (Exception e) {
			System.out.println("Open Wap Err: " + e);
		}
	}

	/**
	 * �رճ���, ִ���������.
	 * 
	 * @param midlet
	 * @param url
	 *            ������ַ.
	 * @throws ConnectionNotFoundException
	 */
	public static void executeUpgrade(MIDlet midlet, String url)
			throws ConnectionNotFoundException {
		openWap(midlet, url);
		// ���������������ֻ�wap����������Զ��˳��������˳����������ڴ˻ص��������. ���ֶ���������.
		// midlet._display.setCurrent(null);
		// midlet.exit();
		midlet.notifyDestroyed();
	}
}
