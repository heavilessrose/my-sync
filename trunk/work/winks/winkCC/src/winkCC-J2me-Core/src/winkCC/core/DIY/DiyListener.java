package winkCC.core.DIY;

public interface DiyListener {

	/**
	 * 上传DIY彩像文件完成, 解析服务器返回数据, 并开始创建定制.
	 * 
	 * @param result
	 *            服务器返回的数据.解析此数据得到彩像ID后用于创建定制.
	 */
	public void onUploadFinish(byte[] result);

	/**
	 * 上传DIY彩像文件时发生异常或错误.
	 */
	public void onUploadError();

	/**
	 * 创建定制完成, 根据返回的数据判断是否成功.
	 */
	public void onBindFinish(byte[] result);

	/**
	 * 创建定制过程中发生异常或错误.
	 */
	public void onBindError();
}
