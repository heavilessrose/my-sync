package ixstrim.examples.servlet;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.ServletException;
import java.io.IOException;
import java.io.OutputStream;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 8, 2003
 */
public class HelloServlet extends HttpServlet{

    protected void doGet(HttpServletRequest req, HttpServletResponse res) throws ServletException, IOException {
        OutputStream os = res.getOutputStream();
        os.write("<html><body>".getBytes());
        os.write("Hello world, from real Servlet".getBytes());
        os.write("</body></html>".getBytes());
        os.flush();
        os.close();
    }
}
