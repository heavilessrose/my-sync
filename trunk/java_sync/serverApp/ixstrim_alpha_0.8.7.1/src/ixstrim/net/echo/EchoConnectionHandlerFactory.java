package ixstrim.net.echo;

import ixstrim.net.ConnectionHandler;
import ixstrim.net.ConnectionHandlerFactory;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class EchoConnectionHandlerFactory implements ConnectionHandlerFactory {
    public EchoConnectionHandlerFactory() {
    }

    public ConnectionHandler create() {
        return new EchoConnectionHandler();
    }
}