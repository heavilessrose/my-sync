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
	 * 根据参数找到文件, 调用http联网模块, 上传彩像文件.(协议6.2.6)
	 * 
	 * @param uid
	 * @param fileName
	 *            彩像文件名.
	 */
	public static void uploadWinks(String uid, String fileName) {
		//		String url = POST /user/{uid}/winks/upload?fn={fileName}
	}

	/**
	 * 组织为xml文件, 调用http联网模块, 创建定制. 协议6.2.8
	 * 
	 * @param uid
	 */
	public static void createWinksScript(String uid) {
		//		String url = POST /user/{uid}/script
	}
}
