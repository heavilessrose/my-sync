package ixstrim.util;

import java.util.ArrayList;

/**
 * This util for pooling reusable resources (FixedBuffers)
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class BuffersPool {

    private ArrayList list = new ArrayList();
    private int buffSize = 0;
    private int limit;

    /**
     * Create BuffersPool with specified allocation unit size, and pool limit.
     * @param buffSize size of buffers which will allocated by this pool
     * @param limit max count of pooled buffers
     */
    public BuffersPool(int buffSize, int limit) {
        this.buffSize = buffSize;
        this.limit = limit;
    }

    /**
     * Allocate buffer.
     * @return FixedBuffer allocated, or from pool.
     */
    public final FixedBuffer allocate() {
        FixedBuffer result = null;
        int listSize = list.size();
        if(listSize != 0) {
            result = (FixedBuffer)list.remove(listSize - 1);
        }
        if(result == null)
            result = new FixedBuffer(buffSize);
        else
            result.reset();
        return result;
    }

    /**
     * Put unused buffer into pool.
     * @param buff unused buffer which will placed into pool.
     */
    public final void free(FixedBuffer buff) {
        if(buff.getArray().length != this.buffSize)
            throw new RuntimeException("Can't accept buffer with size " + buff.getArray().length
                    + " size must be " + this.buffSize);
        if(list.size() < limit) list.add(buff);
    }
}
