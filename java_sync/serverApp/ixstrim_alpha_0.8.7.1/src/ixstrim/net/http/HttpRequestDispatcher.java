package ixstrim.net.http;

import ixstrim.context.RequestContext;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public interface HttpRequestDispatcher {
    /**
     */
    public void dispatch(RequestContext requestContext);
}