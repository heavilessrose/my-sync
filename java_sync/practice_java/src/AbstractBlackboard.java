import java.util.*;

/**
*  Basic implementation of the Blackboard
*
*  @author  Serguei Eremenko
*  @version $Revision: 1.2 $ , $Date: 2002/11/26 12:43:02 $
*/
public abstract class AbstractBlackboard implements Blackboard {

   public AbstractBlackboard(){
      predicates           = new HashMap();
      events               = new ArrayList();
      executor             = new ExecutorPool(11);
      maxEventSize         = 10000;
      maxEventCheckTime    = 10000;
      isRunning            = true;
      errorHandler         = new DefaultErrorHandler(System.err);
      executor.execute(new EventChecker(),Thread.NORM_PRIORITY,"EVENT-CHECK");
   }
   /**
   *  Waits until this predicate is fulfilled or timed out
   *
   *  @param   predicate on which it waits
   *  @param   timeout the time to wait for the predicate to fulfill
   */
   public void waitPredicate(Predicate predicate,long timeout)
      throws InterruptedException,TimeoutException,Exception {
      BlackboardTimedWait lock = null;
      synchronized(predicates){
         if (predicates.get(predicate) != null)
            throw new BlackboardException("Predicate is used by other lock");
         lock = new BlackboardTimedWait();
         add(predicate,lock);
      }
      synchronized(events){ events.notify();}
      try{
         lock.timedWait(timeout);
      }catch (Exception e){
         synchronized(predicates){ predicates.remove(predicate);}
         throw e;
      }
   }

   public void addPredicate(Predicate predicate,BlackboardAction action){
      add(predicate,action);
   }

   private void add(Predicate predicate,Object action){
      synchronized(predicates){
         Object prev = predicates.get(predicate);
         if (prev == null){
            predicates.put(predicate,action);
         }else if (prev instanceof ArrayList){
            ArrayList l = (ArrayList) prev;
            l.add(action);
         }else{
            ArrayList l = new ArrayList();
            l.add(prev);
            l.add(action);
            predicates.put(predicate,l);
         }
      }
   }

   public boolean removePredicate(Predicate predicate){
      synchronized (predicates){
         return (predicates.remove(predicate) != null);
      }
   }
   /**
   *  Clears all predicates
   */
   public void clearPredicate(){
      synchronized (predicates){
         predicates.clear();
      }
   }
   /**
   *  @return number of predicates  
   */
   public int predicateSize(){
      synchronized (predicates){ return predicates.size();}
   }
   /**
   *  @param   event the source of the BlackboardEvent
   */
   public void putEvent(Object event){
      putEvent(new BlackboardEvent(event));
   }
   /**
   *  Puts the event for processing
   *  @param   event to be checked againts predicates
   */
   public void putEvent(BlackboardEvent event){
      synchronized (events){
         events.add(event);
         events.notify();
      }
   }

   public boolean removeEvent(Object event){
      return removeEvent(new BlackboardEvent(event));
   }

   public boolean removeEvent(BlackboardEvent event){
      synchronized (events){
         return events.remove(event);
      }
   }
   /**
   *  Clears all events
   */
   public void clearEvent(){
      synchronized (events){
         events.clear();
      }
   }
   /**
   *  @param   the size of event pool
   */
   public void setMaxEventSize(int size){
      if (size <= 0) throw new IllegalArgumentException ("Size can't be "+size);
      maxEventSize = size;
   }
   /**
   *  @return  number of events
   */
   public int eventSize(){
      synchronized (events){ return events.size();}
   }

   public synchronized void close(){
      isRunning = false;
      synchronized (events){ events.notify();}
      executor.halt();            
      clearPredicate();
      clearEvent();
   }
   /**
   *  @param   errorHandler the new error handler
   *  @return  the old error handler
   */
   public ErrorHandler setErrorHandler(ErrorHandler errorHandler){
      ErrorHandler old = this.errorHandler;
      this.errorHandler = errorHandler;
      return old;
   }

   private void ensureCapacity(List l){
      while (l.size() > maxEventSize) l.remove(0);
   }

   private boolean ensureTTL(List l){
      boolean done = false;
      for (ListIterator li=l.listIterator();li.hasNext();){
         BlackboardEvent evt = (BlackboardEvent) li.next();
         if (evt.isExpired()){
            li.remove();
            done = true;
         }
      }
      return done; 
   }

   private void processEvent(List l){
      for (ListIterator li=l.listIterator();li.hasNext();){
         BlackboardEvent evt = (BlackboardEvent) li.next();
         boolean processed = false;
         synchronized (predicates){
            for (Iterator it=predicates.keySet().iterator();it.hasNext();){
               Predicate pred = (Predicate) it.next();
               if (!pred.test(evt)) continue;
               Object o = predicates.get(pred);
               processed = true;
               if (o instanceof BlackboardAction){
                  processAction((BlackboardAction) o,evt);
               }else if (o instanceof List){
                  List al = (List) o;
                  for (int i=0;i<al.size();i++){ 
                     Object o2 = al.get(i);
                     if (o2 instanceof BlackboardAction){
                        processAction((BlackboardAction) o2,evt);
                     }
                     if (o2 instanceof BlackboardTimedWait){
                        it.remove();
                        BlackboardTimedWait w = (BlackboardTimedWait) o;
                        w.setCondition(true);
                        w.timedNotify();
                     }
                  }
               }else if (o instanceof BlackboardTimedWait){
                  it.remove();
                  BlackboardTimedWait w = (BlackboardTimedWait) o;
                  w.setCondition(true);
                  w.timedNotify();
               }
            }
         }// sync
         if (processed || evt.isExpired()) li.remove();
      }// iterator
   }

   private void processAction(BlackboardAction action,BlackboardEvent event) {
      action.setEvent(event);
      try{
         executor.execute(action,action.getPriority(),action.getName());
      }catch (Exception e){
         if (errorHandler != null) errorHandler.onError("process action",e);
      }
   }

   protected Map           predicates;
   protected List          events;
   protected ExecutorPool  executor;
   protected ErrorHandler  errorHandler;
   protected int           maxEventSize;
   protected long          maxEventCheckTime;
   protected boolean       isRunning;

   private class EventChecker implements Runnable {
      public void run(){
         try{
            mainLoop();
         }finally{

         }
      }
      private final void mainLoop(){
         while(isRunning){
            try{
               synchronized (events){
                  events.wait(maxEventCheckTime);
                  if (events.isEmpty()) continue;
                  ensureCapacity(events);
                  ensureTTL(events);
                  processEvent(events);
               }
            }catch (InterruptedException ie){

            }catch (Exception e){
               if (errorHandler != null) errorHandler.onError("main loop",e);
            }
         }
      }
   }

   private class BlackboardTimedWait extends TimedWait {

      public boolean isCondition(){ return cond;}

      public void setCondition(boolean cond){ this.cond = cond;}

      private boolean cond = false;
   }
}