package winkCC.modules.store;

/**
 * �洢ģ��ĵ�������ʵ�ִ˽ӿ���������صĴ洢ģ���¼�.
 * 
 * @author WangYinghua
 * 
 */
public interface IStoreListener {
	/**
	 * ���ȡ�������.
	 */
	public void onFinish();

	/**
	 * ���ȡ������������.
	 */
	public void onError();
}
