package ixstrim.util;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class HexUtil {

    private static final byte CR = (byte)13;
    private static final byte LF = (byte)10;

    private static final byte[] crlfZerroChunk = {CR, LF, (byte)'0', CR, LF, CR, LF};
    private static final byte[] zerroChunk = {(byte)'0', CR, LF, CR, LF};

    /**
     * Table for fast converting from hex string to byte
     */
    private static final byte[] HEX_VALUES = new byte[128];

    static {
        for(int i = 0; i < HEX_VALUES.length; i++) {
            if(i >= (byte)'0' && i <= (byte)'9') HEX_VALUES[i] = (byte)(i - (byte)'0');
            if(i >= (byte)'a' && i <= (byte)'f') HEX_VALUES[i] = (byte)(i + 10 - (byte)'a');
            if(i >= (byte)'A' && i <= (byte)'F') HEX_VALUES[i] = (byte)(i + 10 - (byte)'A');
        }
    }

    private static final byte[] HEX_SYMBOLS =
            {
                (byte)'0', (byte)'1', (byte)'2', (byte)'3'
                , (byte)'4', (byte)'5', (byte)'6', (byte)'7'
                , (byte)'8', (byte)'9', (byte)'A', (byte)'B'
                , (byte)'C', (byte)'D', (byte)'E', (byte)'F'
            };

    public static final byte parseByteFromHex(byte[] bb, int i) {
        int result = HEX_VALUES[bb[i] & 0x7F] * 16 + HEX_VALUES[bb[i + 1] & 0x7F];
        return (byte)result;
    }

    /**
     * return hex String as bytes wrapped in ByteBuffer
     * Restriction : chunk size can't be more tahn 4Gb
     */
    public static final byte[] chunkSize(int size, boolean firstCRLF) {
        int firstCRLFSize = firstCRLF ? 2 : 0;
        int len = 0;
        int n = size;
        while(n > 0) {
            len++;
            n = n >> 4;
        }
        byte[] result = new byte[len + firstCRLFSize + 2];//8 symbols + 2 CRLF + 2 CRLF
        int pos = 0;
        if(firstCRLF) {
            result[pos++] = CR;
            result[pos++] = LF;
        }
        int numPos = pos;
        for(int i = 7; i >= 0; i--) {
            int hexValue = (size >> (i * 4)) & 0x0F;
            result[pos] = HEX_SYMBOLS[hexValue];
            if(!(hexValue == 0 && pos == numPos)) pos++;
        }
        result[pos++] = CR;
        result[pos++] = LF;
        return result;
    }

    public static final byte[] getLastChunkSize(boolean firstCRLF) {
        if(firstCRLF) {
            return crlfZerroChunk;
        } else {
            return zerroChunk;
        }
    }

}
