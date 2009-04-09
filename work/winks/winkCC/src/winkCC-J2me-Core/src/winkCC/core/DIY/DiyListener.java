package winkCC.core.DIY;

public interface DiyListener {

	/**
	 * �ϴ�DIY�����ļ����, ������������������, ����ʼ��������.
	 * 
	 * @param result
	 *            ���������ص�����.���������ݵõ�����ID�����ڴ�������.
	 */
	public void onUploadFinish(byte[] result);

	/**
	 * �ϴ�DIY�����ļ�ʱ�����쳣�����.
	 */
	public void onUploadError();

	/**
	 * �����������, ���ݷ��ص������ж��Ƿ�ɹ�.
	 */
	public void onBindFinish(byte[] result);

	/**
	 * �������ƹ����з����쳣�����.
	 */
	public void onBindError();
}
