package winkCC.net.http;

/**
 * ������һЩ������������п�����Ҫ�õ��Ļص�����.
 * 
 * @author WangYinghua
 * 
 */
public interface IHttpListener {
	/**
	 * ������������.
	 * 
	 * @param progress
	 */
	public void onProgress(int progress);

	/**
	 * ��������.
	 */
	public void onFinish(byte[] data, int size);

	/**
	 * ��������.
	 */
	public void onError();

	/**
	 * �û�����ж�����.
	 */
	public void onCancel();

	/**
	 * ������Դ��.
	 */
	public void dispose();
}
