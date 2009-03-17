/**
*  Signals that an Blackboard exception of some sort has occurred
*
*  @author  Serguei Eremenko
*  @version $Revision: 1.1 $ , $Date: 2002/11/26 09:08:05 $
*/

public class BlackboardException extends Exception {

   public BlackboardException(){ super();}
   
   public BlackboardException(String s){ super(s);}
}