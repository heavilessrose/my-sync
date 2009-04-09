package winkCC.core.DIY;

public class Diy {
	// 事件:
	/** DIY上传成功 */
	public static final int DIY_EVENT_UPLOAD_FINISH = 10;
	/** DIY上传异常或错误 */
	public static final int DIY_EVENT_UPLOAD_ERROR = 10 << 1;
	/** 创建定制成功 */
	public static final int DIY_EVENT_SCRIPT_FINISH = 10 << 2;
	/** 创建定制异常或错误 */
	public static final int DIY_EVENT_SCRIPT_ERROR = 10 << 3;

	/**
	 * 调用http联网模块, 上传DIY彩像.(协议6.2.6)
	 * 
	 * @param uid
	 * @param fileName
	 *            彩像文件名.
	 * @param data
	 *            要上传的彩像文件数据.
	 */
	public static void uploadWinks(String uid, String fileName, byte[] FileData) {
		//		String url = POST /user/{uid}/winks/upload?fn={fileName}
	}

	/**
	 * 调用http联网模块, 创建定制. 协议6.2.8
	 * 
	 * @param uid
	 * @param scriptData
	 *            定制信息xml的二进制数据.
	 */
	public static void createWinksScript(String uid, byte[] scriptData) {
		//		String url = POST /user/{uid}/script
	}
}
