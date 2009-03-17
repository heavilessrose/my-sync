import java.util.*;
import junit.framework.*;

/**
*  Blackboard framework test class
*
*  @author  Serguei Eremenko
*  @version $Revision: 1.2 $ , $Date: 2002/11/26 12:43:02 $
*/
public class BlackboardTest extends TestCase{

   public BlackboardTest(String name){ super(name);} 

   public static void main(String[] v){
      junit.textui.TestRunner.run(suite());      
   }

   public void setUp(){
      blackboard = new DefaultBlackboard();
   }

   public void tearDown(){
      
   }

   public static Test suite(){
      TestSuite suite = new TestSuite();
      suite.addTest(new BlackboardTest("testAddPredicate"){
         protected void runTest(){ testAddPredicate();}
      });
      suite.addTest(new BlackboardTest("testWaitPredicateOne"){
         protected void runTest(){ testWaitPredicateOne();}
      });
      suite.addTest(new BlackboardTest("testWaitPredicateDouble"){
         protected void runTest(){ testWaitPredicateDouble();}
      });
      suite.addTest(new BlackboardTest("testAddAndWaitPredicate"){
         protected void runTest(){ testAddAndWaitPredicate();}
      });
      suite.addTest(new BlackboardTest("testClose"){
         protected void runTest(){ testClose();}
      });
      
      return suite;
   }

   public void testAddPredicate(){
      try{
         blackboard.addPredicate(new OnePredicate(20),new OneAction("ACTION-1"));
         Thread.sleep(500);
         blackboard.putEvent(new Msg(20));
         Thread.sleep(500);
         Assert.assertTrue("Should be no events",blackboard.eventSize()==0);
      }catch (Exception e){
         e.printStackTrace();
      }
   }

   public void testWaitPredicateOne(){
      try{
         new Thread(new Runnable(){
            public void run(){
               try{
                  Thread.sleep(3000);
                  blackboard.putEvent(new Msg(20));
               }catch (Exception e){
                  e.printStackTrace();
               }
            }
         }).start();
         blackboard.waitPredicate(new OnePredicate(20),20000);
         Assert.assertEquals("Should be no events",blackboard.eventSize(),0);
      }catch (Exception e1){
         e1.printStackTrace();
      }
   }

   public void testWaitPredicateDouble(){
      try{
         new Thread(new Runnable(){
            public void run(){
               try{
                  Thread.sleep(2000);
                  blackboard.putEvent(new Msg(1));
                  Thread.sleep(2000);
                  blackboard.putEvent(new Msg(2));
               }catch (Exception e){
                  e.printStackTrace();
               }
            }
         }).start();
         DoublePredicate pred = new DoublePredicate(1,2);
         blackboard.waitPredicate(pred,20000);
         Assert.assertEquals("Should be no events",blackboard.eventSize(),0);
         BlackboardEvent[] evt = pred.getEvent();
         Assert.assertTrue(evt.length == 2 && evt[0] != null && evt[1] != null);         
      }catch (Exception e1){
         e1.printStackTrace();
      }
   }

   public void testAddAndWaitPredicate(){
      try{
         blackboard.addPredicate(new OnePredicate(20),new OneAction("ACTION-1"));
         new Thread(new Runnable(){
            public void run(){
               try{
                  Thread.sleep(3000);
                  blackboard.putEvent(new Msg(20));
               }catch (Exception e){
                  e.printStackTrace();
               }
            }
         }).start();
         blackboard.waitPredicate(new OnePredicate(20),20000);
         Assert.assertEquals("Should be no events",blackboard.eventSize(),0);
      }catch (Exception e1){
         e1.printStackTrace();
      }
   }

   public void testClose(){
      blackboard.close();
   }

   private DefaultBlackboard blackboard;

   private class OnePredicate extends AbstractPredicate {
      
      public OnePredicate(int id){ this.id=id;}
      
      public boolean test(BlackboardEvent o){
         return o.getSource() instanceof Msg && ((Msg)o.getSource()).id == id;
      }

      private int id;
   }

   private class DoublePredicate extends AbstractPredicate {
      
      public DoublePredicate(int id1,int id2){ 
         this.id1=id1;
         this.id2=id2;
         evt = new BlackboardEvent[2];
      }
      
      public boolean test(BlackboardEvent o){
         if (!(o.getSource() instanceof Msg)) return false;
         if (((Msg)o.getSource()).id == id1) {
            // marks as to discarded this event
            o.setTTL(0);
            wasMsg1 = true;
            // stores the event
            evt[0] = o;
         }
         if (wasMsg1 && ((Msg)o.getSource()).id == id2){
            o.setTTL(0);
            evt[1] = o;
            return true;
         }
         return false;
      }

      public BlackboardEvent[] getEvent(){ return evt;}

      private int id1,id2;
      private boolean wasMsg1 = false;
      private BlackboardEvent[] evt;
   }

   private class Msg {
      
      public Msg(int id){this.id=id;}

      public String toString(){ return "message "+id;}

      public int id;
   }

   private class OneAction extends BlackboardAction {
      public OneAction(String name){ super(name);}
      protected void execute(BlackboardEvent o) throws Exception {
         System.out.println("*** "+this+" is done with "+((Msg)o.getSource()).id);   
      }
   }
}