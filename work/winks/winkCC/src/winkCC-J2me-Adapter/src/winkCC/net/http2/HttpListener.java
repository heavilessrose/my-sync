package winkCC.net.http2;

/**
 * HttpListener�ӿ�ʵ��Observer���۲��ߣ�ģʽ,�Ա�HttpThread����ʾ�۲������ؿ�ʼ,���ؽ���,���½�������.<br>
 * ʵ��HttpListener�ӿڵ���Ļ��ʾһ����������һЩ��ʾ��Ϣ,�������û���ʱ�ж�����.
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
	 * �������������ص�����.
	 * 
	 * @param data
	 * @param size
	 */
	void onFinish(byte[] data, int size);

	/**
	 * ���½������ص�����.
	 * 
	 * @param percent
	 */
	void onProgress(int percent);

	/**
	 * �д������ص�����.
	 * 
	 * @param code
	 * @param message
	 */
	void onError(int code, String message);
}