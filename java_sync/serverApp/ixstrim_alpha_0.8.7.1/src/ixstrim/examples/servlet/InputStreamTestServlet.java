package ixstrim.examples.servlet;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.ServletException;
import java.io.IOException;
import java.io.InputStream;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Dec 18, 2003
 */
public class InputStreamTestServlet  extends HttpServlet{
    protected void doGet(HttpServletRequest req, HttpServletResponse res) throws ServletException, IOException {
        InputStream is = req.getInputStream();
        try{
            while(true){
                int b = is.read();
                if(b == 1)break;
                System.err.write(b);
                System.err.flush();
            }
        }finally{
            res.getOutputStream().write("<html><body>ok</body></html>".getBytes());
            res.getOutputStream().close();
        }
    }
    protected void doPost(HttpServletRequest req, HttpServletResponse res) throws ServletException, IOException {
        doGet(req,res);
    }
}
