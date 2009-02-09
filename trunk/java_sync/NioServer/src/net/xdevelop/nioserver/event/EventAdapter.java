package net.xdevelop.nioserver.event;

import net.xdevelop.nioserver.event.ServerListener;
import net.xdevelop.nioserver.Request;
import net.xdevelop.nioserver.Response;

/**
 * <p>
 * Title: 事件适配器
 * 
 * </p>
 * 
 * <p>
 * Description: 对Serverlistener接口实现一个适配器(EventAdapter)，这样的好处是最终的事件处理器可以只处理所关心的事件
 * </p>
 * 
 * @author starboy
 * @version 1.0
 */

public abstract class EventAdapter implements ServerListener {
	public EventAdapter() {
	}

	public void onError(String error) {
		System.out.println("EventAdapter: onError()");
	}

	public void onAccept() throws Exception {
		System.out.println("EventAdapter: onAccept()");
	}

	public void onAccepted(Request request) throws Exception {
		System.out.println("EventAdapter: onAccepted()");
	}

	public void onRead(Request request) throws Exception {
		System.out.println("EventAdapter: onRead()");
	}

	public void onWrite(Request request, Response response) throws Exception {
	}

	public void onClosed(Request request) throws Exception {
	}
}
