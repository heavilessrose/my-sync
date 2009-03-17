/**
*  Provides the base interface for blackboard event-driven framework
*
*  @author  Serguei Eremenko
*  @version $Revision: 1.2 $ , $Date: 2002/11/26 12:43:02 $
*/
public interface Blackboard {
   /**
   *  Waits (blocking) for a predicate to be true otherwise times out
   *  @param   predicate to be fulfilled
   *  @param   timeout (ms) for how long it waits for a predicate to happens
   *  @exception  InterruptedException when this thread is interrupted,
   *              TimeoutException when timeout occurs,
   *              Exception when any other exceptio happens
   */
   public void waitPredicate(Predicate predicate,long timeout)
      throws InterruptedException,TimeoutException,Exception;
   /**
   *  Adds predicate - action pairs. One predicate can have more than one
   *  actions.
   *  @param   predicate
   *  @param   action
   */
   public void addPredicate(Predicate predicate,BlackboardAction action);
   /**
   *  Removes predicate from the blackboard
   *  @param   predicate to be removed
   *  @return  true if predicate removed
   */
   public boolean removePredicate(Predicate predicate);
   /**
   *  Notifies the blackboard when an event occurs
   *  @param   event that occurred and to be processed by blackboard
   */
   public void putEvent(BlackboardEvent event);
   /**
   *  Removes event from the blackboard
   *  @param   event to be removed
   *  @return  true if event removed
   */
   public boolean removeEvent(BlackboardEvent event);
   /**
   *  Closes the blackboard and releases all resources associated with it
   */
   public void close();

}