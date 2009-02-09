package net.xdevelop.nioserver.event;

import net.xdevelop.nioserver.event.ServerListener;
import net.xdevelop.nioserver.Request;
import net.xdevelop.nioserver.Response;

/**
 * <p>
 * Title: �¼�������
 * 
 * </p>
 * 
 * <p>
 * Description: ��Serverlistener�ӿ�ʵ��һ��������(EventAdapter)�������ĺô������յ��¼�����������ֻ���������ĵ��¼�
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
