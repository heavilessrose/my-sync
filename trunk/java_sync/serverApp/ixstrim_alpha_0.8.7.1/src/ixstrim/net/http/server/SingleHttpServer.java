package ixstrim.net.http.server;

import ixstrim.net.Acceptor;
import ixstrim.net.AcceptorCallback;
import ixstrim.net.http.HttpAcceptorCallback;
import ixstrim.net.http.HttpRequestDispatcher;
import ixstrim.config.HttpServerConfig;

import java.io.IOException;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class SingleHttpServer {

    public SingleHttpServer(HttpServerConfig config, HttpRequestDispatcher dispatcher) throws IOException {
        Acceptor acceptor = new Acceptor(config.getAcceptorConfig());
        AcceptorCallback httpCallback = new HttpAcceptorCallback(
                config.getHttpAcceptorCallbackConfig(),dispatcher);
        acceptor.register(config.getPort(), httpCallback);
        Thread t = new Thread(acceptor);
        t.setPriority(Thread.MAX_PRIORITY);
        t.start();
    }
}
