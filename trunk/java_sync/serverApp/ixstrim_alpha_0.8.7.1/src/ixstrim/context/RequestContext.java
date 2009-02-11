package ixstrim.context;

import ixstrim.net.IOCallback;
import ixstrim.net.http.session.HttpSessionImpl;
import ixstrim.net.http.session.SessionManager;
import ixstrim.net.http.*;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpSession;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class RequestContext {

    private static final String COOKIE_SESSION_ID = "IX_SESSION_ID";

    private HttpRequestData httpRequest;
    private HttpResponseData httpResponse;
    private IOCallback callback;
    private HttpSessionImpl session;
    private InputHandler inputHandler;
    private OutputHandler outputHandler;
    private ConnectionContext connectionContext;
    private boolean asynchronousProcessing = false;

    public RequestContext(ConnectionContext connectionContext,HttpRequestData httpRequest, HttpResponseData httpResponse, IOCallback callback) {
        this.connectionContext = connectionContext;
        this.httpRequest = httpRequest;
        this.httpResponse = httpResponse;
        this.callback = callback;
    }

    public HttpRequestData getRequest() {
        return httpRequest;
    }

    public HttpResponseData getResponse() {
        return httpResponse;
    }

    public IOCallback getCallback() {
        return callback;
    }

    public HttpSession getSession() {
        return getSession(true);
    }

    public HttpSession getSession(boolean createIfNotFound) {
        checkCookie();
        if(session == null && createIfNotFound) {
            session = new HttpSessionImpl();
            SessionManager.getSessionManager().bind(session);
            httpResponse.addCookie(new Cookie(COOKIE_SESSION_ID, session.getId()));
        }
        return session;
    }

    private void checkCookie() {
        Cookie[] cooks = httpRequest.getCookies();
        if(cooks == null || cooks.length == 0) return;
        for(int i = 0; i < cooks.length; i++) {
            if(COOKIE_SESSION_ID.equals(cooks[i].getName())) {
                String id = cooks[i].getValue();
                session = SessionManager.getSessionManager().lookup(id);
                if(session != null)session.setIsNew(false);
                return;
            }
        }
    }

    public InputHandler getInputHandler() {
        return inputHandler;
    }

    public void setInputHandler(InputHandler inputHandler) {
        this.inputHandler = inputHandler;
    }

    public OutputHandler getOutputHandler() {
        return outputHandler;
    }

    public void setOutputHandler(OutputHandler outputHandler) {
        this.outputHandler = outputHandler;
    }

    public ConnectionContext getConnectionContext() {
        return connectionContext;
    }

    public boolean isAsynchronousProcessing() {
        return asynchronousProcessing;
    }

    public void setAsynchronousProcessing(boolean asynchronousProcessing) {
        this.asynchronousProcessing = asynchronousProcessing;
    }
}
