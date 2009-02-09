package net.xdevelop.nioserver;

import net.xdevelop.nioserver.event.ServerListener;
import java.util.ArrayList;

/**
 * <p>
 * Title: 事件触发器
 * 
 * </p>
 * <p>
 * Description:
 * 用于在适当的时候通过触发服务器事件，通知在册的事件处理器对事件做出响应。触发器以Singleton模式实现，统一控制整个服务器端的事件，避免造成混乱
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
	 * 获取事件触发器
	 * 
	 * @return 返回事件触发器
	 */
	public static synchronized Notifier getNotifier() {
		if (instance == null) {
			instance = new Notifier();
			return instance;
		} else
			return instance;
	}

	/**
	 * 添加事件监听器
	 * 
	 * @param l
	 *            监听器
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
