package ixstrim.net.http.bridge;

import ixstrim.context.RequestContext;
import ixstrim.net.http.InputHandler;
import ixstrim.util.BuffersPool;
import ixstrim.util.FixedBuffer;

import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.util.ArrayList;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Mar 30, 2003
 */
public class InputHandlerStreamed implements InputHandler {

    private RequestContext requestContext;
    private ArrayList list = new ArrayList();
    private ArrayList freeList = new ArrayList();
    private FixedBuffer currentBuffer;
    private boolean closed = false;
    private OutputStream os;
    private BuffersPool pool;


    public InputHandlerStreamed(RequestContext requestContext) {
        this.requestContext = requestContext;
        this.pool = requestContext.getConnectionContext().getBuffersPool();
    }


    public final ByteBuffer read() {
        free();
        synchronized(list) {
            if(list.size() == 0) {
                if(closed) requestContext.getCallback().close();
                return null;
            } else {
                FixedBuffer buffer = (FixedBuffer)list.remove(0);
                freeList.add(buffer);
                return ByteBuffer.wrap(buffer.getArray(), 0, buffer.getLength());
            }
        }
    }

    public synchronized OutputStream getOutputStream() {
        if(os == null){
            os = new OutputStreamImpl();
        }
        return os;
    }


    private void put(FixedBuffer buffer) {
        boolean needNotify = false;
        synchronized(list) {
            if(list.size() == 0) needNotify = true;
            list.add(buffer);
        }
        if(needNotify) {
            requestContext.getCallback().readyForOutput();
        }
    }

    private void free() {
        while(freeList.size() > 0) {
            FixedBuffer buffer = (FixedBuffer)freeList.remove(freeList.size() - 1);
            synchronized(pool) {
                pool.free(buffer);
            }
        }
    }

    private void checkCurrentBuffer() {
        if(currentBuffer == null) {
            synchronized(pool) {
                currentBuffer = pool.allocate();
            }
        } else {
            if(currentBuffer.getFree() == 0) {
                put(currentBuffer);
                synchronized(pool) {
                    currentBuffer = pool.allocate();
                }
            }
        }
    }

    private class OutputStreamImpl extends OutputStream {

        public final void write(int b) {
            checkCurrentBuffer();
            //after check we can write 1 byte - is guaranted
            currentBuffer.append((byte)b);
        }

        public final void write(byte b[]) {
            write(b, 0, b.length);
        }

        public final void write(byte b[], int off, int len) {
            while(len > 0) {
                checkCurrentBuffer();
                int mustWrite = Math.min(len, currentBuffer.getFree());
                currentBuffer.append(b, off, mustWrite);
                len -= mustWrite;
                off += mustWrite;
            }
        }

        public final void flush() {
            if(currentBuffer != null) {
                put(currentBuffer);
                currentBuffer = null;
            }
        }

        public final void close() {
            flush();
            closed = true;
            requestContext.getCallback().readyForOutput();
        }
    }
}
