package winkCC.log;

/**
 * 根据特定实现创建一个ILog实例
 * 
 * @author WangYinghua
 * 
 */
public class LogFactory {

	public static ILog getLog(final String name) {
		try {
			final Class clazz = Class.forName("winkCC.log.ConsoleLogImpl");
			final ILog log = (ILog) clazz.newInstance();
			((ILogConfigurator) log).setName(name);
			return log;
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}
}
