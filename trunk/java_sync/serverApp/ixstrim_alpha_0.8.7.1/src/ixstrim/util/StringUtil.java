package ixstrim.util;

/**
 * String's utilites for converting String to bytes
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class StringUtil {

    private static final byte PERCENT = (byte)'%';

    private char[] charResult;
    private CookieUtil cookieUtil = new CookieUtil(this);

    public StringUtil(int initialCapacity) {
        this.charResult = new char[initialCapacity];
    }

    public StringUtil() {
        this(1024);
    }

    /**
     * Convert String to bytes into specified array
     * @param s
     * @param dst
     * @return string length
     */
    public final int stringToBytes(String s, byte[] dst, int off) {
        int len = s.length();
        if(charResult.length < len) charResult = new char[len];
        s.getChars(0, len, charResult, 0);
        for(int i = 0; i < len; i++) dst[off + i] = (byte)charResult[i];
        return len;
    }


    public final String createValue(byte[] bb, int start, int len) {
        if(charResult.length < len) charResult = new char[len];
        int j = 0;
        int end = start + len;
        for(int i = start; i < end; i++) {
            if(bb[i] == PERCENT) {
                charResult[j++] = (char)HexUtil.parseByteFromHex(bb, i + 1);
                i += 2;
            } else {
                charResult[j++] = (char)bb[i];
            }
        }
        return new String(charResult, 0, j);
    }

    /**
     * @param start start position
     * @param len length of bytes
     * @return extracted substring
     */
    public final String extractString(byte[] bb, int start, int len) {
        if(charResult.length < len) charResult = new char[len];
        for(int i = 0; i < len; i++) {
            charResult[i] = (char)bb[i + start];
        }
        return new String(charResult, 0, len);
    }

    /**
     * @param start start position
     * @param len length of bytes
     * @return extracted substring
     */
    public final String extractStringLower(byte[] bb, int start, int len) {
        if(charResult.length < len) charResult = new char[len];
        char c;
        for(int i = 0; i < len; i++) {
            c = (char)bb[i + start];
            //to lower case
            if(c >= 0x41 && c <= 0x5A) c += 0x20;
            charResult[i] = c;
        }
        return new String(charResult, 0, len);
    }

    /**
     * @param start start position
     * @param len length of bytes
     * @return extracted substring
     */
    public final String extractStringUpper(byte[] bb, int start, int len) {
        if(charResult.length < len) charResult = new char[len];
        char c;
        for(int i = 0; i < len; i++) {
            c = (char)bb[i + start];
            //to upper case
            if(c >= 0x61 && c <= 0x7A) c -= 0x20;
            charResult[i] = c;
        }
        return new String(charResult, 0, len);
    }

    public final CookieUtil getCookieUtil() {
        return cookieUtil;
    }

}
