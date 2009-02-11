package ixstrim.util;

import javax.servlet.http.Cookie;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 18, 2003
 */
public class CookieUtil {


    private static final byte[] SET_COOKIE = "Set-Cookie: ".getBytes();
    private static final byte[] DOMAIN = "; Domain=".getBytes();
    private static final byte[] PATH = "; Path=".getBytes();
    //private static final byte[] EXPIRES = "; Expires=".getBytes();//date formatted as Wdy, DD-Mon-YYYY HH:MM:SS GMT
    //private static final byte[] SECURE = "; Secure".getBytes();
    private static final byte EQU = (byte)'=';
    private static final byte CR = (byte)13;
    private static final byte LF = (byte)10;

    private static final String DEFAULT_PATH = "/";

    private StringUtil stringUtil;

    public CookieUtil(StringUtil stringUtil) {
        this.stringUtil = stringUtil;
    }

    public int writeTo(Cookie cookie,byte[] bb, int off) {
        int pos = off;
        System.arraycopy(SET_COOKIE, 0, bb, pos, SET_COOKIE.length);
        pos += SET_COOKIE.length;

        pos += stringUtil.stringToBytes(cookie.getName(), bb, pos);

        bb[pos] = EQU;
        pos++;

        pos += stringUtil.stringToBytes(cookie.getValue(), bb, pos);

        if(cookie.getDomain() != null) {
            System.arraycopy(DOMAIN, 0, bb, pos, DOMAIN.length);
            pos += DOMAIN.length;
            pos += stringUtil.stringToBytes(cookie.getDomain(), bb, pos);
        }

        String path = cookie.getPath();
        if(path == null) path = DEFAULT_PATH;
        System.arraycopy(PATH, 0, bb, pos, PATH.length);
        pos += PATH.length;

        pos += stringUtil.stringToBytes(path, bb, pos);

        bb[pos] = CR;
        pos++;
        bb[pos] = LF;
        pos++;

        return pos - off;
    }

}
