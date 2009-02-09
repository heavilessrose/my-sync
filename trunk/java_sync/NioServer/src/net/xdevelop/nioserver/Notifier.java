package net.xdevelop.nioserver;

import net.xdevelop.nioserver.event.ServerListener;
import java.util.ArrayList;

/**
 * <p>
 * Title: �¼�������
 * 
 * </p>
 * <p>
 * Description:
 * �������ʵ���ʱ��ͨ�������������¼���֪ͨ�ڲ���¼����������¼�������Ӧ����������Singletonģʽʵ�֣�ͳһ���������������˵��¼���������ɻ���
 * </p>
 * 
 * @author starboy
 * @version 1.0
 */
public class Notifier {
	private static ArrayList<ServerListener> eventListeners = null;
	private static Notifier instance = null;

	private Notifier() {
		eventListeners = new ArrayList<ServerListener>();
	}

	/**
	 * ��ȡ�¼�������
	 * 
	 * @return �����¼�������
	 */
	public static synchronized Notifier getNotifier() {
		if (instance == null) {
			instance = new Notifier();
			return instance;
		} else
			return instance;
	}

	/**
	 * ����¼�������
	 * 
	 * @param l
	 *            ������
	 */
	public void addListener(ServerListener l) {
		synchronized (eventListeners) {
			if (!eventListeners.contains(l))
				eventListeners.add(l);
		}
	}

	public void fireOnAccept() throws Exception {
		for (int i = eventListeners.size() - 1; i >= 0; i--)
			(eventListeners.get(i)).onAccept();
	}

	public void fireOnAccepted(Request request) throws Exception {
		for (int i = eventListeners.size() - 1; i >= 0; i--)
			(eventListeners.get(i)).onAccepted(request);
	}

	void fireOnRead(Request request) throws Exception {
		for (int i = eventListeners.size() - 1; i >= 0; i--)
			(eventListeners.get(i)).onRead(request);

	}

	void fireOnWrite(Request request, Response response) throws Exception {
		for (int i = eventListeners.size() - 1; i >= 0; i--)
			(eventListeners.get(i)).onWrite(request, response);

	}

	public void fireOnClosed(Request request) throws Exception {
		for (int i = eventListeners.size() - 1; i >= 0; i--)
			(eventListeners.get(i)).onClosed(request);
	}

	public void fireOnError(String error) {
		for (int i = eventListeners.size() - 1; i >= 0; i--)
			(eventListeners.get(i)).onError(error);
	}
}
