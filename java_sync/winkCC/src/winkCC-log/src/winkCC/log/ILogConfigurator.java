package winkCC.log;

/**
 * LogImpl 必须实现此接口. 为了保持ILog接口清晰, 独立出来
 * 
 * @author WangYinghua
 * 
 */
public interface ILogConfigurator {
	/**
	 * Set the name (class or package) that this instance represents.
	 * 
	 * @param name
	 *            instance name
	 */
	void setName(String name);
}
