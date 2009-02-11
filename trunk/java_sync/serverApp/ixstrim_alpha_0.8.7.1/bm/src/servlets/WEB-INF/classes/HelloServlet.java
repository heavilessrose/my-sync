import java.io.*;
import java.util.*;

import javax.servlet.http.*;
import javax.servlet.*;

public class HelloServlet extends HttpServlet {
  private static final byte[] content = "<html><body><h1>Hello world</h1></body></html>".getBytes();

  public HelloServlet() {}

  public void doGet (HttpServletRequest  req,HttpServletResponse res) throws ServletException, IOException{
    res.setContentType("text/html");
    OutputStream os = res.getOutputStream();
    os.write(content);
    os.close();
  }

}
