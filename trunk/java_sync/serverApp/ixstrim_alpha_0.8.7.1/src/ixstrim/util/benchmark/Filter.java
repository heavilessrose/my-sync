package ixstrim.util.benchmark;

import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.io.IOException;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Dec 1, 2003
 */
public class Filter {

    private static final byte[] RESPONSE_SIGNATURE = "</html>".getBytes();


    private int state = 0;
    private byte[] bb = new byte[4096];
    private ByteBuffer buffer = ByteBuffer.wrap(bb);
    private int number;
    private int[] responsesCount;


    public Filter(int number, int[] responsesCount) {
        this.number = number;
        this.responsesCount = responsesCount;
    }

    public boolean filter(SocketChannel ch) throws IOException {
        buffer.clear();
        ch.read(buffer);
        int len = buffer.position();
        if(len == 0) return false;
        for(int i = 0; i < len; i++) {
            if(bb[i] == RESPONSE_SIGNATURE[state]) {
                state++;
                if(state == RESPONSE_SIGNATURE.length) {
                    state = 0;
                    responsesCount[number]++;
                }
            } else {
                state = 0;
            }
        }
        return true;
    }
}
