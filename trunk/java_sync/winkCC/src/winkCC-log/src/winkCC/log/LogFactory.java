package winkCC.log;

/**
 * �����ض�ʵ�ִ���һ��ILogʵ��
 * 
 * @author WangYinghua
 * 
 */
public class LogFactory {

	public static ILog getLog(final String name) {
		try {
			final Class clazz = Class.forName("bm.core.log.ConsoleLogImpl"); // [Commenter-Don't]
			final ILog log = (ILog) clazz.newInstance(); // [Commenter-Don't]
			((ILogConfigurator) log).setName(name);
			return log;
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}
}
