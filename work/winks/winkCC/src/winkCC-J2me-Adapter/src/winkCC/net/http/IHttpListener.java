package winkCC.net.http;

/**
 * 定义了一些监控联网操作中可能需要用到的回调方法.
 * 
 * @author WangYinghua
 * 
 */
public interface IHttpListener {
	/**
	 * 更新联网进程.
	 * 
	 * @param progress
	 */
	public void onProgress(int progress);

	/**
	 * 联网结束.
	 */
	public void onFinish(byte[] data, int size);

	/**
	 * 发生错误.
	 */
	public void onError();

	/**
	 * 用户点击中断联网.
	 */
	public void onCancel();

	/**
	 * 回收资源等.
	 */
	public void dispose();
}
