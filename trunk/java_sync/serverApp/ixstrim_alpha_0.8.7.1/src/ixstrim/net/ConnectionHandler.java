package ixstrim.net;

import java.net.InetAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public abstract class ConnectionHandler implements IOCallback {

    private SelectorHandler selectorHandler;
    private SelectionKey key;
    private ByteBuffer readBuffer;
    private ByteBuffer writeBuffer;
    private boolean needClose = false;

    /**
     * Set context of this Connection.
     * SelectionHandler and connection key
     * This context need for callback
     */
    void setContext(SelectorHandler selectorHandler, SelectionKey key) {
        this.selectorHandler = selectorHandler;
        this.key = key;
    }


    /**
     * Return buffered buffer returned from underlying handler
     */
    ByteBuffer getReadBuffer() {
        if(!hasData()) {
            readBuffer = read();
        }
        return readBuffer;
    }

    boolean hasData() {
        return readBuffer == null ? false : readBuffer.remaining() > 0;
    }

    /**
     * Method for writing incoming data.
     * This method automatically will buffering data when data will not writed
     * to underlying handler.
     * Return true if data writed fully, and false otherwise
     */
    boolean putWriteBuffer(ByteBuffer byteBuffer) {
        if(writeBuffer != null)
            throw new RuntimeException("Assertion error. Buffered data not wited by flush.");
        write(byteBuffer);
        int remaining = byteBuffer.remaining();
        if(remaining == 0) {
            return true;
        } else {
            byte[] bb = new byte[remaining];
            byteBuffer.get(bb);
            this.writeBuffer = ByteBuffer.wrap(bb);
            return false;
        }
    }

    /**
     * Flush buffered writeBuffer
     */
    boolean flushWriteBuffer() {
        if(writeBuffer != null) {
            write(writeBuffer);
            if(writeBuffer.remaining() == 0) {
                writeBuffer = null;
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    }

    boolean needClose() {
        return needClose;
    }

    /**
     * Call this method when you want to close connection.
     * Connection will closed after read() method returned null.
     */
    public void close() {
        this.needClose = true;
        readyForOutput();
    }

    /**
     * Call this method when data ready for read from you handler.
     */
    public void readyForOutput() {
        selectorHandler.markWritable(this.key);
    }

    /**
     * Call this method when you handler ready for processing incoming data.
     */
    public void readyForInput() {
        selectorHandler.markReadable(this.key);
    }

    /**
     * When you override this method, take care for get data from buffer
     * and return from method immediately.
     * Attention! : You can't use this buffer - this is system buffer.
     */
    public abstract void write(ByteBuffer bb);

    /**
     * This method will called by container for read data.
     */
    public abstract ByteBuffer read();

    /**
     * Will called before destroying connection
     */
    public void destroy() {
    }

    /**
     * @return remote address which connected this socket
     */
    public InetAddress getRemoteAddress() {
        return ((java.nio.channels.SocketChannel)key.channel()).socket().getInetAddress();
    }
}