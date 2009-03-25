package winkCC.net.http2;

/**
 * HttpListener接口实现Observer（观察者）模式,以便HttpThread能提示观察者下载开始,下载结束,更新进度条等.<br>
 * 实现HttpListener接口的屏幕显示一个进度条和一些提示信息,并允许用户随时中断连接.
 * 
 * @author WangYinghua
 * 
 */
public interface HttpListener {
	/**
	 * 
	 * 
	 * @param size
	 */
	void onSetSize(int size);

	/**
	 * 联网操作结束回调方法.
	 * 
	 * @param data
	 * @param size
	 */
	void onFinish(byte[] data, int size);

	/**
	 * 更新进度条回调方法.
	 * 
	 * @param percent
	 */
	void onProgress(int percent);

	/**
	 * 有错误发生回调方法.
	 * 
	 * @param code
	 * @param message
	 */
	void onError(int code, String message);
}