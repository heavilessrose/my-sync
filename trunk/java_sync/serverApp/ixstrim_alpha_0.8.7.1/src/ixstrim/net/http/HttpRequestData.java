package ixstrim.net.http;

import ixstrim.net.ConnectionHandler;

import javax.servlet.http.Cookie;
import java.util.HashMap;
import java.util.Map;
import java.net.InetAddress;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 20, 2003
 */
public class HttpRequestData {

    /**
     * Only for getting remote address just in time
     */
    private ConnectionHandler connectionHandler;

    HashMap headers = new HashMap();
    HashMap parameters = new HashMap();
    String requestMethod;
    String requestURI;
    String requestProtocol;
    Cookie[] cookies;

    public HttpRequestData(ConnectionHandler connectionHandler) {
        this.connectionHandler = connectionHandler;
    }

    public Map getHeaders() {
        return headers;
    }

    public Map getParameters() {
        return parameters;
    }

    public String getMethod() {
        return requestMethod;
    }

    public String getURI() {
        return requestURI;
    }

    public String getProtocol() {
        return requestProtocol;
    }

    public Cookie[] getCookies() {
        return cookies;
    }

    public InetAddress getRemoteAddress() {
        return connectionHandler.getRemoteAddress();
    }

}
