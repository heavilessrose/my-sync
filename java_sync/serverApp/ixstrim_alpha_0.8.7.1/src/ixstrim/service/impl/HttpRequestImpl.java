package ixstrim.service.impl;

import ixstrim.Version;
import ixstrim.service.AsyncHttpServletRequest;
import ixstrim.net.http.HttpRequestData;
import ixstrim.net.http.bridge.OutputHandlerStreamed;
import ixstrim.context.RequestContext;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpSession;
import javax.servlet.ServletInputStream;
import javax.servlet.RequestDispatcher;
import java.util.Enumeration;
import java.util.Locale;
import java.util.Collections;
import java.util.Map;
import java.security.Principal;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.nio.channels.WritableByteChannel;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 8, 2003
 */
public class HttpRequestImpl implements AsyncHttpServletRequest{

    private RequestContext requestContext;
    private HttpRequestData req;

    public HttpRequestImpl(RequestContext requestContext) {
        this.requestContext = requestContext;
        this.req = requestContext.getRequest();
    }

    public String getAuthType() {
        return null;
    }

    public Cookie[] getCookies() {
        return req.getCookies();
    }

    public long getDateHeader(String s) {
        return 0;
    }

    public String getHeader(String s) {
        return (String)req.getHeaders().get(s);
    }

    public Enumeration getHeaders(String s) {
        return null;
    }

    public Enumeration getHeaderNames() {
        return Collections.enumeration(req.getHeaders().keySet());
    }

    public int getIntHeader(String s) {
        try{
            return Integer.parseInt(getHeader(s));
        }catch(Exception e){
            return 0;
        }
    }

    public String getMethod() {
        return req.getMethod();
    }

    public String getPathInfo() {
        return null;
    }

    public String getPathTranslated() {
        return null;
    }

    public String getContextPath() {
        return "/";
    }

    public String getQueryString() {
        String uri = req.getURI();
        int i = uri.indexOf('?');
        if(i != -1 && i + 1 < uri.length()){
            return uri.substring(i + 1);
        }else{
            return null;
        }
    }

    public String getRemoteUser() {
        return null;
    }

    public boolean isUserInRole(String s) {
        return false;
    }

    public Principal getUserPrincipal() {
        return null;
    }

    public String getRequestedSessionId() {
        return null;
    }

    public String getRequestURI() {
        return req.getURI();
    }

    public String getServletPath() {
        return "";
    }

    public HttpSession getSession(boolean b) {
        return requestContext.getSession(b);
    }

    public HttpSession getSession() {
        return getSession(true);
    }

    public boolean isRequestedSessionIdValid() {
        return true;
    }

    public boolean isRequestedSessionIdFromCookie() {
        return true;
    }

    public boolean isRequestedSessionIdFromURL() {
        return false;
    }

    /**
     * @deprecated
     */
    public boolean isRequestedSessionIdFromUrl() {
        return false;
    }

    public Object getAttribute(String s) {
        return null;
    }

    public Enumeration getAttributeNames() {
        return null;
    }

    public String getCharacterEncoding() {
        return null;
    }

    public int getContentLength() {
        return 0;
    }

    public String getContentType() {
        return null;
    }

    public ServletInputStream getInputStream() throws IOException {
        if(requestContext.isAsynchronousProcessing()){
            throw new IllegalStateException("Can't provide stream in asynchronous mode");
        }
        checkOutputHandler();
        OutputHandlerStreamed handler = new OutputHandlerStreamed(requestContext);
        requestContext.setOutputHandler(handler);
        final InputStream is = handler.getInputStream();
        return new ServletInputStream(){
            public int read() throws IOException {
                return is.read();
            }
        };
    }

    public String getParameter(String s) {
        return null;
    }

    public Enumeration getParameterNames() {
        return Collections.enumeration(req.getParameters().keySet());
    }

    public String[] getParameterValues(String s) {
        return new String[0];
    }

    public String getProtocol() {
        return req.getProtocol();
    }

    public String getScheme() {
        return "http";
    }

    public String getServerName() {
        return Version.getSignature();
    }

    public int getServerPort() {
        return 0;
    }

    public BufferedReader getReader() throws IOException {
        return null;
    }

    public String getRemoteAddr() {
        return req.getRemoteAddress().getHostAddress();
    }

    public String getRemoteHost() {
        return req.getRemoteAddress().getHostName();
    }

    public void setAttribute(String s, Object o) {
    }

    public void removeAttribute(String s) {
    }

    public Locale getLocale() {
        return null;
    }

    public Enumeration getLocales() {
        return null;
    }

    public boolean isSecure() {
        return false;
    }

    public RequestDispatcher getRequestDispatcher(String s) {
        return null;
    }

    /**
     * @deprecated
     */
    public String getRealPath(String s) {
        return null;
    }


    public void setChannel(WritableByteChannel channel) {
        if(requestContext.isAsynchronousProcessing()){
            throw new RuntimeException("Method not supported yet");
        }else{
            throw new IllegalStateException("Can't set channel in not asynchronous mode.");
        }
    }

    public StringBuffer getRequestURL() {
        return null;
    }

    public void setCharacterEncoding(String s) throws UnsupportedEncodingException {
    }

    public Map getParameterMap() {
        return null;
    }

    private void checkOutputHandler(){
        if(requestContext.getOutputHandler() != null){
            throw new RuntimeException("OutputHandler already registered.");
        }
    }

}
