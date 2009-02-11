package ixstrim.service;

import javax.servlet.http.HttpServlet;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 19, 2003
 */
public interface HttpService {

    public void registerServlet(String path, HttpServlet servlet);
    public void unregister(String path);
}
