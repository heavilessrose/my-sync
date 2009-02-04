package net.xdevelop.timeserver;

import net.xdevelop.nioserver.event.EventAdapter;
import net.xdevelop.nioserver.*;
import java.util.Date;

/**
 * ÈÕÖ¾¼ÇÂ¼
 */
public class LogHandler extends EventAdapter {
    public LogHandler() {
    }

    public void onClosed(Request request) throws Exception {
        String log = new Date().toString() + " from " + request.getAddress().toString();
        System.out.println(log);
    }

    public void onError(String error) {
        System.out.println("Error: " + error);
    }
}
