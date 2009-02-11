package ixstrim.net.http;

import ixstrim.context.ConnectionContext;
import ixstrim.util.StringUtil;

import javax.servlet.http.Cookie;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class HttpResponseData{

    private static final byte CR = (byte)13;
    private static final byte LF = (byte)10;
    private static final byte SP = (byte)' ';
    private static final byte COLON = (byte)':';


    /**
     * Default protocol HTTP/1.1
     */
    private String responseProtocol = HttpConstants.PROTOCOL_HTTP11;
    /**
     * Default response code = 200 Ok
     */
    private String responseCode = HttpConstants.RESPONSE_CODE_200;
    private Map headers = new HashMap();
    private StringUtil stringUtil;
    private ArrayList cookiesList;

    public HttpResponseData(ConnectionContext connectionContext) {
        this.stringUtil = connectionContext.getStringUtil();
        //set default content-type = text/html
        headers.put(HttpConstants.HEADER_CONTENT_TYPE, HttpConstants.HEADER_VALUE_TEXT_HTML);
        headers.put(HttpConstants.HEADER_SERVER, HttpConstants.HEADER_VALUE_SERVER_IXSTRIM);
    }

    public void setHeader(String key, String value) {
        headers.put(key, value);
    }

    public String getHeader(String key) {
        return (String)headers.get(key);
    }

    public void setResponseCode(String s) {
        this.responseCode = s;
    }

    public void setProtocol(String s) {
        this.responseProtocol = s;
    }

    /**
     * Add cookie
     * @param cookie
     */
    public void addCookie(Cookie cookie) {
        if(cookiesList == null) cookiesList = new ArrayList();
        cookiesList.add(cookie);
    }

    public void sendRedirect(String location) {
        setResponseCode(HttpConstants.RESPONSE_CODE_302);
        setHeader(HttpConstants.HEADER_LOCATION, location);
    }

    public int toByteArray(byte[] bb) {
        int off = 0;
        off += stringUtil.stringToBytes(responseProtocol, bb, off);
        bb[off++] = SP;
        off += stringUtil.stringToBytes(responseCode, bb, off);
        bb[off++] = CR;
        bb[off++] = LF;
        Iterator i = headers.entrySet().iterator();
        while(i.hasNext()) {
            Map.Entry entry = (Map.Entry)i.next();
            off += stringUtil.stringToBytes((String)entry.getKey(), bb, off);
            bb[off++] = COLON;
            off += stringUtil.stringToBytes((String)entry.getValue(), bb, off);
            bb[off++] = CR;
            bb[off++] = LF;
        }
        if(cookiesList != null) {
            for(int k = 0; k < cookiesList.size(); k++) {
                Cookie cookie = (Cookie)cookiesList.get(k);
                off += stringUtil.getCookieUtil().writeTo(cookie,bb, off);
            }
        }
        bb[off++] = CR;
        bb[off++] = LF;
        return off;
    }


}