import java.util.*;

/**
*  The root event class for the all Blackboard' events 
*
*  @author  Serguei Eremenko
*  @version $Revision: 1.2 $ , $Date: 2002/11/26 12:43:02 $
*/

public class BlackboardEvent extends EventObject {
   
   public BlackboardEvent(Object src){
      super(src);
      this.ttl  = Integer.MAX_VALUE;
      this.time = System.currentTimeMillis();
   }
   /**
   *  @return  ttl time to live in ms
   */
   public long getTTL(){ return ttl;}
   /**
   *  @param  time to live in ms 
   */
   public void setTTL(long ttl){ this.ttl = ttl;}
   /**
   *  @return <tt>true</tt> if this event lived longer than specified by TTL
   */
   public boolean isExpired(){
      return System.currentTimeMillis() - time >= ttl;
   }
   /**
   *  @return true if event' sources are equal
   */
   public boolean equals(Object o){
      return o instanceof BlackboardEvent &&
         ((BlackboardEvent)o).getSource().equals(o);      
   }

   public String toString(){
      return " [ "+super.toString()+",TTL "+ttl+", create time "+time+" ] ";
   }

   protected long ttl,time;
}
