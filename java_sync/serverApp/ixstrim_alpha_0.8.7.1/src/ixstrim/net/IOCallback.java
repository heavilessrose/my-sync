package ixstrim.net;

/**
 * Special callback for interaction connection handler with selector handler.
 * For notifying when connection handler ready for output data, or ready accept new data,
 * and when need close connection.
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public interface IOCallback {
    /**
     * Call this method when you want to close connection.
     */
    public void close();

    /**
     * Call this method when your handler ready for accept data.
     */
    public void readyForInput();

    /**
     * Call this method when data ready for read from you handler.
     */
    public void readyForOutput();
}
