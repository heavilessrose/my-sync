package ixstrim.util;

import java.nio.ByteBuffer;

/**
 * Buffer with fixed size.
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class FixedBuffer {
    private byte[] array;
    private int len;

    /**
     * Create buffer with specified capacity
     * @param capacity max size of data in buffer
     */
    public FixedBuffer(int capacity) {
        if(capacity <= 0) throw new RuntimeException("Capacity must be > 0");
        this.array = new byte[capacity];
        reset();
    }

    /**
     * Clear this buffer
     */
    public final void reset() {
        this.len = 0;
    }

    /**
     * Append all data from byteBuffer to this buffer
     * @param byteBuffer
     */
    public final void append(ByteBuffer byteBuffer) {
        append(byteBuffer, byteBuffer.remaining());
    }

    /**
     * Append posible data to end of this buffer
     * Attention: may appended not fully if buffer is full
     * @param byteBuffer
     */
    public final void appendPosible(ByteBuffer byteBuffer) {
        append(byteBuffer, Math.min(byteBuffer.remaining(), getFree()));
    }


    /**
     * Append data from byteBuffer to this buffer
     * @param byteBuffer source ByteBuffer
     * @param buffLen data length
     */
    public final void append(ByteBuffer byteBuffer, int buffLen) {
        int free = array.length - len;
        if(buffLen > free) throw new RuntimeException("Can't append more than free space");
        byteBuffer.get(array, len, buffLen);
        len += buffLen;
    }


    public final void append(byte[] bb) {
        int free = array.length - len;
        if(bb.length > free) throw new RuntimeException("Can't append more than free space");
        System.arraycopy(bb, 0, array, len, bb.length);
        len += bb.length;
    }

    public final void append(byte[] bb, int start, int length) {
        int free = array.length - len;
        if(length > free) throw new RuntimeException("Can't append more than free space");
        System.arraycopy(bb, start, array, len, length);
        len += length;
    }

    public final void append(byte b) {
        array[len++] = b;
    }


    /**
     * @return actually length of this buffer
     */
    public final int getLength() {
        return len;
    }

    /**
     * Set new length of this buffer
     * @param len new length of this buffer
     */
    public final void setLength(int len) {
        this.len = len;
    }

    /**
     * @return free size of this buffer
     */
    public final int getFree() {
        return array.length - len;
    }

    /**
     * @return underlyed array of this buffer
     */
    public final byte[] getArray() {
        return array;
    }

}