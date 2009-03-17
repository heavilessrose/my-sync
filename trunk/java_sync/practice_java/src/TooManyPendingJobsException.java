/**
*  @author  Serguei Eremenko
*  @version $Revision: 1.2 $, $Date: 2002/11/05 09:11:13 $
*/

public class TooManyPendingJobsException extends ExecutorException {

   public TooManyPendingJobsException(){ super();}

   public TooManyPendingJobsException(String s){ super(s);}

}