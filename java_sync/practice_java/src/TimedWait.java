/**
*  @author  Serguei Eremenko
*  @version $Revision: 1.3 $, $Date: 2002/11/28 10:12:58 $
*/

public abstract class TimedWait {
     
   public TimedWait () { lock = new Object(); }

   public TimedWait (Object obj){
      lock = obj;
   }

   public abstract boolean isCondition ();

   public final void timedWait (long msecTimeout) 
      throws InterruptedException,TimeoutException {
      if (!isCondition ()) {
         long start = System.currentTimeMillis ();
         long wait_time = msecTimeout;

         for (;;) {
            synchronized (lock){ lock.wait (wait_time);}
            if (!isCondition ()) {
               long now = System.currentTimeMillis ();
               long timeSoFar = now - start;
               if (timeSoFar >= msecTimeout){ 
                  throw new TimeoutException ("Timed out");
               }else{
                  wait_time = msecTimeout - timeSoFar; 
               }
            }else{
               break;
            }
         }
      }
   }

   /** Notify all threads waiting on the lock. */
   public final void timedNotify () {
      synchronized (lock){ lock.notifyAll ();}
   }
   
   protected Object lock;
}