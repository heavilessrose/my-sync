//: state:KissingPrincess2.java
package state;
import junit.framework.*;

class Creature {
  private interface State {
    String response();
  }
  private class Frog implements State {
    public String response() { return "Ribbet!"; }
  }
  private class Prince implements State {
    public String response() { return "Darling!"; }
  }
  private State state = new Frog();
  public void greet() { 
    System.out.println(state.response());
  }
  public void kiss() { state = new Prince(); }
}

public class KissingPrincess2 extends TestCase  {
  Creature creature = new Creature();
  public void test() {
    creature.greet();
    creature.kiss();
    creature.greet();
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(KissingPrincess2.class);
  }
} ///:~
