//: statemachine:StateMachine.java
// Takes an Iterator of Inputs to move from State
// to State using a template method.
package statemachine;
import java.util.*;

public class StateMachine {
  private State currentState;
  public StateMachine(State initialState) {
    currentState = initialState;
    currentState.run();
  }
  // Template method:
  public final void runAll(Iterator inputs) {
    while(inputs.hasNext()) {
      Input i = (Input)inputs.next();
      System.out.println(i);
      currentState = currentState.next(i);
      currentState.run();
    }
  }
}  ///:~
