package ixstrim.net.http;

import java.nio.ByteBuffer;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Dec 7, 2003
 */
public interface OutputHandler {
    /**
     * When you override this method, take care for get data from buffer
     * and return from method immediately.
     * Attention! : You can't use this buffer - this is system buffer.
     * If you will read data length 0, then posible container will not write
     * new data in next time, in this case you can call method IOCallback.readyForInput
     * for signal to container that you can accept data again.
     * If you want do blocking operations for request processing do this in separated Thread.
     */
    public void write(ByteBuffer bb);
}
