/**
*  Basic implementation of the Predicate
*
*  @author  Serguei Eremenko
*  @version $Revision: 1.2 $ , $Date: 2002/11/26 12:43:02 $
*/

public abstract class AbstractPredicate implements Predicate {

   public AbstractPredicate(){}
   /**
   *  @param   the event
   *  @return  true iff this event satisfies this predicate
   */
   public abstract boolean test(BlackboardEvent event);

}
