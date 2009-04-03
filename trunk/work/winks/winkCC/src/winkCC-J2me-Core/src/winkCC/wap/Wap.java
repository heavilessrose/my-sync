/**
 * 
 */
package winkCC.wap;

import javax.microedition.io.ConnectionNotFoundException;
import javax.microedition.midlet.MIDlet;

/**
 * 封装了软件升级等调用wap的操作.
 * 
 * @author WangYinghua
 * 
 */
public class Wap {

	private Wap() {
	}

	/**
	 * 调用手机wap浏览器.
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
	 * 关闭程序, 执行软件升级.
	 * 
	 * @param midlet
	 * @param url
	 *            升级地址.
	 * @throws ConnectionNotFoundException
	 */
	public static void executeUpgrade(MIDlet midlet, String url)
			throws ConnectionNotFoundException {
		openWap(midlet, url);
		// 经测试索爱调用手机wap浏览器不会自动退出程序切退出浏览器后可在此回到程序界面. 需手动结束程序.
		// midlet._display.setCurrent(null);
		// midlet.exit();
		midlet.notifyDestroyed();
	}
}
