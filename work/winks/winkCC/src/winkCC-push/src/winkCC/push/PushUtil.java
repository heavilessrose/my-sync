package winkCC.push;

import java.util.Date;

import javax.microedition.io.ConnectionNotFoundException;
import javax.microedition.io.PushRegistry;

import winkCC.log.ILog;
import winkCC.log.LogFactory;

public class PushUtil {
	private static ILog log = LogFactory.getLog("PushUtil");

	/**
	 * 是否为服务器push启动(无法判断是否是定时alarm激活)
	 * 
	 * @return
	 */
	public static boolean isPushActivated() {
		// 是否有push inbound connection, 如果有表明是push启动, 否则为用户主动启动软件.
		String[] connections = PushRegistry.listConnections(true);
		if (connections != null && connections.length > 0) {
			log.info("push activated");
			return true;
		}
		log.info("not push activated");
		return false;
	}

	/**
	 * 设置定时启动
	 * 
	 * @param et
	 *            期望的毫秒时长
	 */
	public static void scheduleMIDlet(long et, String midletClassName) {
		Date alarm = new Date();
		try {
			long t = PushRegistry.registerAlarm(midletClassName, alarm.getTime()
					+ et);
		} catch (ConnectionNotFoundException e) {
			log.error("schedule time alarm to start", e);
		} catch (ClassNotFoundException e) {
			log.error("schedule time alarm to start", e);
		}
	}
}
