package ixstrim.net.http;

import java.nio.ByteBuffer;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Dec 7, 2003
 */
public interface InputHandler {
    /**
     * This method will called by container when content will ready.
     * For notifying container about data ready call method IOCallback.readyForOutput()
     * in corresponding IOCallback..
     */
    public ByteBuffer read();
}
