//: statemachine:mousetrap2:MouseTrap2Test.java
// A better mousetrap using tables
package statemachine.mousetrap2;
import statemachine.mouse.*;
import statemachine.*;
import java.util.*;
import java.io.*;
import com.bruceeckel.util.*;
import junit.framework.*;

abstract class StateT implements State {
  protected Map transitions = null;
  protected void init(Object[][] states) {
    transitions = new HashMap();
    for(int i = 0; i < states.length; i++)
      transitions.put(states[i][0], states[i][1]);
  }
  public abstract void run();
  public State next(Input i) {
    if(transitions.containsKey(i))
      return (State)transitions.get(i);
    else
      throw new RuntimeException(
        "Input not supported for current state");
  }
}

class MouseTrap extends StateMachine {
  public static State
    waiting = new Waiting(),
    luring = new Luring(),
    trapping = new Trapping(),
    holding = new Holding();
  public MouseTrap() {
    super(waiting); // Initial state
  }
}

class Waiting extends StateT {
  public void run() { 
    System.out.println(
      "Waiting: Broadcasting cheese smell");
  }
  public State next(Input i) {
    // Delayed initialization:
    if(transitions == null)
      init(new Object[][] { 
        { MouseAction.appears, MouseTrap.luring },
      });
    return super.next(i);
  }
}

class Luring extends StateT {
  public void run() {
    System.out.println(
      "Luring: Presenting Cheese, door open");
  }
  public State next(Input i) {
    if(transitions == null)
      init(new Object[][] { 
        { MouseAction.enters, MouseTrap.trapping },
        { MouseAction.runsAway, MouseTrap.waiting },
      });
    return super.next(i);
  }
}

class Trapping extends StateT {
  public void run() {
    System.out.println("Trapping: Closing door");
  }
  public State next(Input i) {
    if(transitions == null)
      init(new Object[][] { 
        { MouseAction.escapes, MouseTrap.waiting },
        { MouseAction.trapped, MouseTrap.holding },
      });
    return super.next(i);
  }
}

class Holding extends StateT {
  public void run() {
    System.out.println("Holding: Mouse caught");
  }
  public State next(Input i) {
    if(transitions == null)
      init(new Object[][] { 
        { MouseAction.removed, MouseTrap.waiting },
      });
    return super.next(i);
  }
}

public class MouseTrap2Test extends TestCase  {
  MouseTrap trap = new MouseTrap();
  MouseMoveList moves = 
    new MouseMoveList(
      new StringList("../mouse/MouseMoves.txt")
        .iterator());
  public void test() {
    trap.runAll(moves.iterator());
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(MouseTrap2Test.class);
  }
} ///:~
