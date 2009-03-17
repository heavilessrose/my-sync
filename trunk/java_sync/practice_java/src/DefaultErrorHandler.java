import java.io.PrintStream;

/**
*  @author  Serguei Eremenko
*  @version $Revision: 1.3 $ , $Date: 2002/11/18 14:38:17 $
*/
public class DefaultErrorHandler implements ErrorHandler {

   public DefaultErrorHandler(PrintStream out){
      this.out = out;
   }

   public void onError(Object msg,Throwable err) {
      if (out == null) {
         System.out.println("$$$ "+msg+" "+err);
         return;
      }
      out.println(msg+", "+err);
      out.flush();
   }

   private PrintStream out;
}