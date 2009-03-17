/**
*  Provides basic interface for testing logical condition
*
*  @author  Serguei Eremenko
*  @version $Revision: 1.2 $ , $Date: 2002/11/26 12:43:02 $
*/

public interface Predicate extends java.io.Serializable {
   /**
   *  @param   the vent to be analyzed
   *  @return  <tt>true</tt> if this event satisfies this predicate
   */
   public boolean test(BlackboardEvent event);

}
