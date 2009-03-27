package winkCC.modules.store;

/**
 * 存储模块的调用者需实现此接口来处理相关的存储模块事件.
 * 
 * @author WangYinghua
 * 
 */
public interface IStoreListener {
	/**
	 * 存或取操作完成.
	 */
	public void onFinish();

	/**
	 * 存或取操作发生错误.
	 */
	public void onError();
}
