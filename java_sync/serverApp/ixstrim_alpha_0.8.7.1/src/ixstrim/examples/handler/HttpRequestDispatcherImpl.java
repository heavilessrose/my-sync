package ixstrim.examples.handler;

import ixstrim.examples.handler.*;
import ixstrim.context.RequestContext;
import ixstrim.net.http.HttpRequestDispatcher;
import ixstrim.net.http.file.FileInputHandler;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class HttpRequestDispatcherImpl implements HttpRequestDispatcher {

    public void dispatch(RequestContext requestContext) {
        String uri = requestContext.getRequest().getURI();
        if("/".equals(requestContext.getRequest().getURI())) {
            requestContext.getResponse().sendRedirect("/index.html");
        }
        if(uri.startsWith("/hello"))
            requestContext.setInputHandler(new InputHandlerHello(requestContext));
        else
            requestContext.setInputHandler(new FileInputHandler(requestContext, "/", "../web"));
    }
}
