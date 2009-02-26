package winkCC.push;

import java.util.Date;

import javax.microedition.io.ConnectionNotFoundException;
import javax.microedition.io.PushRegistry;

import winkCC.log.ILog;
import winkCC.log.LogFactory;

public class PushUtil {
	private static ILog log = LogFactory.getLog("PushUtil");

	/**
	 * �Ƿ�Ϊ������push����(�޷��ж��Ƿ��Ƕ�ʱalarm����)
	 * 
	 * @return
	 */
	public static boolean isPushActivated() {
		// �Ƿ���push inbound connection, ����б�����push����, ����Ϊ�û������������.
		String[] connections = PushRegistry.listConnections(true);
		if (connections != null && connections.length > 0) {
			log.info("push activated");
			return true;
		}
		log.info("not push activated");
		return false;
	}

	/**
	 * ���ö�ʱ����
	 * 
	 * @param et
	 *            �����ĺ���ʱ��
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
