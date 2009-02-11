package ixstrim.net.http;

import ixstrim.net.ConnectionHandler;
import ixstrim.net.ConnectionHandlerFactory;
import ixstrim.context.ConnectionContext;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class HttpConnectionHandlerFactory implements ConnectionHandlerFactory {
    private ConnectionContext connectionContext;

    public HttpConnectionHandlerFactory(ConnectionContext connectionContext) {
        this.connectionContext = connectionContext;
    }

    public ConnectionHandler create() {
        return new HttpConnectionHandler(this.connectionContext);
    }
}