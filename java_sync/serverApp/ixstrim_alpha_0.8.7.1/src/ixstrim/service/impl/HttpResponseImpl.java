package ixstrim.service.impl;

import ixstrim.net.http.HttpConstants;
import ixstrim.net.http.HttpResponseData;
import ixstrim.net.http.bridge.InputHandlerStreamed;
import ixstrim.net.http.bridge.InputHandlerContent;
import ixstrim.net.http.bridge.InputHandlerChanneled;
import ixstrim.context.RequestContext;
import ixstrim.service.AsyncHttpServletResponse;

import javax.servlet.http.Cookie;
import javax.servlet.ServletOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.OutputStream;
import java.util.Locale;
import java.nio.channels.ReadableByteChannel;
import java.nio.channels.SelectableChannel;
import java.nio.ByteBuffer;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 8, 2003
 */
public class HttpResponseImpl implements AsyncHttpServletResponse{

    private RequestContext requestContext;
    private HttpResponseData resp;
    private UserSelectorHandler selectorHandler;

    public HttpResponseImpl(RequestContext requestContext) {
        this.requestContext = requestContext;
        this.resp = requestContext.getResponse();
    }

    public void addCookie(Cookie cookie) {
        resp.addCookie(cookie);
    }

    public boolean containsHeader(String s) {
        return false;
    }

    public String encodeURL(String s) {
        return s;
    }

    public String encodeRedirectURL(String s) {
        return s;
    }

    /**
     * @deprecated
     */
    public String encodeUrl(String s) {
        return s;
    }

    /**
     * @deprecated
     */
    public String encodeRedirectUrl(String s) {
        return s;
    }

    public void sendError(int i, String s) throws IOException {
    }

    public void sendError(int i) throws IOException {
    }

    public void sendRedirect(String s) throws IOException {
        resp.sendRedirect(s);
    }

    public void setDateHeader(String s, long l) {
    }

    public void addDateHeader(String s, long l) {
    }

    public void setHeader(String s, String s1) {
    }

    public void addHeader(String s, String s1) {
    }

    public void setIntHeader(String s, int i) {
    }

    public void addIntHeader(String s, int i) {
    }

    public void setStatus(int i) {
    }

    /**
     * @deprecated
     */
    public void setStatus(int i, String s) {
    }

    public String getCharacterEncoding() {
        return null;
    }

    public ServletOutputStream getOutputStream() throws IOException {
        if(requestContext.isAsynchronousProcessing()){
            throw new IllegalStateException("Can't provide stream in asynchronous mode");
        }
        checkInputHandler();
        InputHandlerStreamed handler = new InputHandlerStreamed(requestContext);
        requestContext.setInputHandler(handler);
        final OutputStream os = handler.getOutputStream();
        return new ServletOutputStream(){
            public void write(int b) throws IOException {
                os.write(b);
            }
            public void write(byte b[]) throws IOException {
                os.write(b);
            }
            public void write(byte b[], int off, int len) throws IOException {
                os.write(b, off, len);
            }
            public void flush() throws IOException {
                os.flush();
            }
            public void close() throws IOException {
                os.close();
            }
        };
    }

    public PrintWriter getWriter() throws IOException {
        return new PrintWriter(getOutputStream());
    }

    public void setContentLength(int i) {
        resp.setHeader(HttpConstants.HEADER_CONTENT_LENGTH,String.valueOf(i));
    }

    public void setContentType(String s) {
        resp.setHeader(HttpConstants.HEADER_CONTENT_TYPE,s);
    }

    public void setBufferSize(int i) {
    }

    public int getBufferSize() {
        return 0;
    }

    public void flushBuffer() throws IOException {
    }

    public boolean isCommitted() {
        return true;
    }

    public void reset() {
    }

    public void setLocale(Locale locale) {
    }

    public Locale getLocale() {
        return null;
    }

    public void setChannel(ReadableByteChannel channel) {
        if(requestContext.isAsynchronousProcessing()){
            if(!(channel instanceof SelectableChannel)){
                throw new IllegalArgumentException("ReadableByteChannel argument must be SelectableChannel");
            }
            SelectableChannel selectableChannel = (SelectableChannel)channel;
            try{
                selectableChannel.configureBlocking(false);
            }catch(IOException e){
                throw new IllegalArgumentException("Can't configure channel to non-blocking mode");
            }
            checkInputHandler();
            InputHandlerChanneled handler = new InputHandlerChanneled(requestContext,channel,selectorHandler);
            requestContext.setInputHandler(handler);
        }else{
            throw new IllegalStateException("Can't set channel in not asynchronous mode.");
        }
    }

    public void setContent(ByteBuffer buffer) {
        checkInputHandler();
        requestContext.setInputHandler(new InputHandlerContent(requestContext,buffer));
        setContentLength(buffer.remaining());
    }

    public void setSelectorHandler(UserSelectorHandler selectorHandler) {
        this.selectorHandler = selectorHandler;
    }

    public void resetBuffer() {
    }

    private void checkInputHandler(){
        if(requestContext.getInputHandler() != null){
            throw new RuntimeException("InputHandler already registered.");
        }
    }
}
