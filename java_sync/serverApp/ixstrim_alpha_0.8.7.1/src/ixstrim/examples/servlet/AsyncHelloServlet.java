package ixstrim.examples.servlet;

import ixstrim.service.AsyncHttpServlet;
import ixstrim.service.AsyncHttpServletRequest;
import ixstrim.service.AsyncHttpServletResponse;

import java.nio.ByteBuffer;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 20, 2003
 */
public class AsyncHelloServlet extends AsyncHttpServlet{

    private static final byte[] content = "<html><body><h1>Hello world from AsyncServlet</h1></body></html>".getBytes();

    public void service(AsyncHttpServletRequest request,AsyncHttpServletResponse response){
        response.setContent(ByteBuffer.wrap(content));
    }
}
