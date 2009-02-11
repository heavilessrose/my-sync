package ixstrim.util;

import ixstrim.context.ConnectionContext;

import java.nio.ByteBuffer;
import java.util.Map;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class FormDataBuffer {

    private static final byte EQU = (byte)'=';
    private static final byte AMP = (byte)'&';

    private FixedBuffer buffer;
    private ConnectionContext connectionContext;

    public FormDataBuffer(ConnectionContext connectionContext, int capacity) {
        this.buffer = new FixedBuffer(capacity);
        this.connectionContext = connectionContext;
    }

    public void append(ByteBuffer byteBuffer) {
        buffer.append(byteBuffer);
    }

    public void parametersToMap(Map map) {
        byte[] bb = buffer.getArray();
        int len = buffer.getLength();
        int equIdx = -1;
        int ampIdx = -1;
        int i = 0;
        for(; i < len; i++) {
            byte b = bb[i];
            switch(b) {
                case AMP:
                    if(equIdx != -1) {
                        String key = connectionContext.getStringUtil().createValue(bb, ampIdx + 1, equIdx - (ampIdx + 1));
                        String value = connectionContext.getStringUtil().createValue(bb, equIdx + 1, i - (equIdx + 1));
                        map.put(key, value);
                    }
                    ampIdx = i;
                    equIdx = -1;
                    break;
                case EQU:
                    equIdx = i;
                    break;
                default :
                    break;
            }
        }
        //parse last parameter
        if(equIdx != -1) {
            String key = connectionContext.getStringUtil().createValue(bb, ampIdx + 1, equIdx - (ampIdx + 1));
            String value = connectionContext.getStringUtil().createValue(bb, equIdx + 1, i - (equIdx + 1));
            map.put(key, value);
        }
    }
}
