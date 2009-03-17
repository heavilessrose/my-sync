/**
*  The base class for Blackboard' actions to be run
*
*  @author  Serguei Eremenko
*  @version $Revision: 1.2 $ , $Date: 2002/11/26 12:43:02 $
*/

public abstract class BlackboardAction implements Runnable {
   /**
   *  @param   lock the lock for this action
   *  @param   name the Thread's name
   *  @param   priority the Thread's priority
   */
   public BlackboardAction(Object lock,String name,int priority){
      this.lock      = lock;
      this.name      = name;
      this.priority  = priority;
   }

   public BlackboardAction(Object lock,String name){
      this(lock,name,Thread.NORM_PRIORITY);
   }
   /**
   *  @param   name of this action
   */
   public BlackboardAction(String name){ this(new Object(),name);}
   /**
   *  Runs: setUp(), then execute(), and finally tearDown() method
   */
   public void run(){
      try{
         setUp();
         execute(event);
      }catch(Throwable t){
         error = t;         
      }finally{
         tearDown();
      }
   }
   /**
   *  @return  name of this action
   */
   public String getName(){ return name;}
   /**
   *  @param   priority Thread's priority
   */
   public void setPriority(int priority){ this.priority = priority;}
   /**
   *  @return  Thread's priority
   */
   public int getPriority(){ return priority;}
   /**
   *  @return  error that happened diring run() execution or null
   */
   public Throwable getError(){ return error;}
   /**
   *  @param   event against which this action is run and has to be 
   *           set prior to executing run() method
   */
   public void setEvent(BlackboardEvent event){ this.event = event;}
   /**
   *  @return event against which this action is run
   */
   public Object getEvent(){ return event;}
   /**
   *  @return Blackboard action's status
   */
   public BlackboardActionStatus getStatus(){ return status;}

   public String toString(){
      return " [ action name "+name+" ] ";
   }
   /**
   *  Processes this Blackboard's event
   *  @param      Blackboard's event to be processed
   *  @exception  any exception
   */
   protected abstract void execute(BlackboardEvent evt) throws Exception;
   /**
   *  Prepares this action
   */
   protected void setUp(){
      error  = null;
   }
   /**
   *  Cleans up this action
   */
   protected void tearDown(){
      event = null;
   }

   protected String                 name;
   protected int                    priority;
   protected final Object           lock;
   protected Throwable              error;
   protected BlackboardEvent        event;
   protected BlackboardActionStatus status;
}