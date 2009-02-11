package ixstrim.net;

/**
 * Factory which produce ConnectionHandlers for handling TCP connection
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public interface ConnectionHandlerFactory {
    /**
     * Create ConnectionHandler for handling connection
     */
    public ConnectionHandler create();
}