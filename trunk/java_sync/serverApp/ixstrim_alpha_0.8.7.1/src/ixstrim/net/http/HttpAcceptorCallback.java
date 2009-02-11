package ixstrim.net.http;

import ixstrim.net.AcceptorCallback;
import ixstrim.net.ConnectionHandlerFactory;
import ixstrim.net.SelectorHandler;
import ixstrim.context.ConnectionContext;
import ixstrim.context.ApplicationContext;
import ixstrim.config.HttpAcceptorCallbackConfig;
import ixstrim.event.ServerErrorEvent;
import ixstrim.event.EventDispatcher;

import java.io.IOException;
import java.nio.channels.SocketChannel;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class HttpAcceptorCallback implements AcceptorCallback {

    private static String SELECTOR_HANDLER_TREAD_NAME = "HTTP-SelectorHandlerTread";

    private EventDispatcher eventDispatcher
            = ApplicationContext.getDefaultApplicationContext().getEventDispatcher();

    private SelectorHandler[] handlers;

    public HttpAcceptorCallback(HttpAcceptorCallbackConfig config,HttpRequestDispatcher httpRequestDispatcher) throws IOException {
        int selectorsCount = config.getSelectorHandlersCount();
        this.handlers = new SelectorHandler[selectorsCount];
        for(int i = 0; i < handlers.length; i++) {
            ConnectionContext connectionContext = new ConnectionContext(config.getConnectionContextConfig());
            connectionContext.setRequestDispatcher(httpRequestDispatcher);
            ConnectionHandlerFactory factory = new HttpConnectionHandlerFactory(connectionContext);
            handlers[i] = new SelectorHandler(config.getSelectorHandlerConfig(),factory);
            Thread t = new Thread(handlers[i], SELECTOR_HANDLER_TREAD_NAME);
            t.start();
        }
    }

    public void accepted(SocketChannel channel) {
        try {
            channel.configureBlocking(false);
            int i = (int)(System.currentTimeMillis() % handlers.length);
            handlers[i].register(channel);
        } catch(Exception e) {
            eventDispatcher.errorOccured(new ServerErrorEvent(this,e));
        }
    }
}