//: statemachine2:StateMachine.java
// A table-driven state machine
package statemachine2;
import java.util.*;

public class StateMachine {
  private State state;
  private Map map = new HashMap();
  public StateMachine(State initial) {
    state = initial;
  }
  public void buildTable(Object[][][] table) {
    for(int i = 0; i < table.length; i++) {
      Object[][] row = table[i];
      Object currentState = row[0][0];
      List transitions = new ArrayList();
      for(int j = 1; j < row.length; j++)
        transitions.add(row[j]);
      map.put(currentState, transitions);
    }
  }
  public void nextState(Input input) {
    Iterator it=((List)map.get(state)).iterator();
    while(it.hasNext()) {
      Object[] tran = (Object[])it.next();
      if(input == tran[0] || 
         input.getClass() == tran[0]) {
        if(tran[1] != null) {
          Condition c = (Condition)tran[1];
          if(!c.condition(input))
            continue; // Failed test
        }
        if(tran[2] != null)
          ((Transition)tran[2]).transition(input);
        state = (State)tran[3];
        return;
      }
    }
    throw new RuntimeException(
      "Input not supported for current state");
  }
} ///:~
