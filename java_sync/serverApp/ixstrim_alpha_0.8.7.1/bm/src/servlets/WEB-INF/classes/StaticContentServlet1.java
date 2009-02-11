import java.io.*;
import java.util.*;

import javax.servlet.http.*;
import javax.servlet.*;

public class StaticContentServlet1 extends HttpServlet {

    private static final byte[] content;
    private static final int CONTENT_LENGTH = 1000;

    static{
        StringBuffer sb = new StringBuffer(CONTENT_LENGTH * 2);
        sb.append("<html><body>");
        String s = "Static content "+CONTENT_LENGTH+"<br>";
        while(sb.length() < CONTENT_LENGTH)sb.append(s);
        sb.append("</body></html>");
        content = sb.toString().getBytes();
    }

    private static String len = ""+content.length;

    public StaticContentServlet1() {}

    public void doGet (HttpServletRequest  req,HttpServletResponse res) throws ServletException, IOException{
      res.setContentType("text/html");
      res.setHeader("content-length",len);
      OutputStream os = res.getOutputStream();
      os.write(content);
      os.close();
    }


}
