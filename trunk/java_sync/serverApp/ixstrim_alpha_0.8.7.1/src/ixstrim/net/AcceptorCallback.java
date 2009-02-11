package ixstrim.net;

import java.nio.channels.SocketChannel;

/**
 * Callback for accepting new connected SocketChannels.
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public interface AcceptorCallback {
    /**
     * This method will called when new connection accepted.
     */
    public void accepted(SocketChannel socketChannel);
}
