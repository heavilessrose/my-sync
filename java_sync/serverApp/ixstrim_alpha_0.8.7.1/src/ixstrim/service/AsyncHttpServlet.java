package ixstrim.service;

import javax.servlet.http.HttpServlet;
import java.nio.ByteBuffer;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 19, 2003
 */
public class AsyncHttpServlet extends HttpServlet{
    public void service(AsyncHttpServletRequest request,AsyncHttpServletResponse response){
        response.setContent(ByteBuffer.wrap(new byte[0]));
    }
}
