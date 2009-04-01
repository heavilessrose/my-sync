package winkCC.DIY;

public class Diy {

	/**
	 * 调用http联网模块, 上传DIY彩像.(协议6.2.6)
	 * 
	 * @param url
	 *            post地址.
	 * @param data
	 *            要上传的彩像文件数据.
	 */
	public static void uploadWinks(String fileName, byte[] data) {
		// 根据fileName组织url
		String url = "/user/{uid}/winks/upload?fn={fileName}";
	}
}
