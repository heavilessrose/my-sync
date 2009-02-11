package ixstrim.net.echo;

import ixstrim.net.AcceptorCallback;
import ixstrim.net.ConnectionHandlerFactory;
import ixstrim.net.SelectorHandler;
import ixstrim.config.SelectorHandlerConfig;
import ixstrim.event.EventDispatcher;
import ixstrim.event.ServerErrorEvent;
import ixstrim.context.ApplicationContext;

import java.io.IOException;
import java.nio.channels.SocketChannel;
import java.util.Random;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class EchoAcceptorCallback implements AcceptorCallback {

    private EventDispatcher eventDispatcher
            = ApplicationContext.getDefaultApplicationContext().getEventDispatcher();

    private static String SELECTOR_HANDLER_TREAD_NAME = "ECHO-SelectorHandlerTread";
    private SelectorHandler[] handlers;
    private Random r = new Random();

    public EchoAcceptorCallback(int selectorsCount) throws IOException {
        this.handlers = new SelectorHandler[selectorsCount];
        for(int i = 0; i < handlers.length; i++) {
            ConnectionHandlerFactory factory = new EchoConnectionHandlerFactory();
            handlers[i] = new SelectorHandler(
                    new SelectorHandlerConfig(){
                        public int getSelectorBufferSize() {
                            return 8192;
                        }
                        public int getSelectorTimeout() {
                            return 5;
                        }
                    },
                    factory);
            Thread t = new Thread(handlers[i], SELECTOR_HANDLER_TREAD_NAME);
            t.start();
        }
    }

    public void accepted(SocketChannel channel) {
        try {
            channel.configureBlocking(false);
            int i = (int)(r.nextDouble() * handlers.length);
            handlers[i].register(channel);
        } catch(Exception e) {
            eventDispatcher.errorOccured(new ServerErrorEvent(this,e));
        }
    }

}