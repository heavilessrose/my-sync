package net.xdevelop.timeserver;

import net.xdevelop.nioserver.event.EventAdapter;
import net.xdevelop.nioserver.*;
import java.util.Date;

/**
 * 日志记录处理器
 */
public class LogHandler extends EventAdapter {
	public LogHandler() {
	}

	public void onClosed(Request request) throws Exception {
		System.out.println("LogHandler: onClosed()");
		String log = new Date().toString() + " from "
				+ request.getAddress().toString();
		System.out.println(log);
	}

	public void onError(String error) {
		System.out.println("LogHandler: onError(): " + error);
	}
}
