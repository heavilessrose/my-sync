//: statemachine:mouse:MouseAction.java
package statemachine.mouse;
import java.util.*;
import statemachine.*;

public class MouseAction implements Input {
  private String action;
  private static List instances = new ArrayList();
  private MouseAction(String a) { 
    action = a; 
    instances.add(this);
  }
  public String toString() { return action; }
  public int hashCode() { 
    return action.hashCode();
  }
  public boolean equals(Object o) {
    return (o instanceof MouseAction)
      && action.equals(((MouseAction)o).action);
  }
  public static MouseAction forString(String description) {
    Iterator it = instances.iterator();
    while(it.hasNext()) {
      MouseAction ma = (MouseAction)it.next();
      if(ma.action.equals(description))
        return ma;
    }
    throw new RuntimeException("not found: " + description);
  }
  public static MouseAction
    appears = new MouseAction("mouse appears"),
    runsAway = new MouseAction("mouse runs away"),
    enters = new MouseAction("mouse enters trap"),
    escapes = new MouseAction("mouse escapes"),
    trapped = new MouseAction("mouse trapped"),
    removed = new MouseAction("mouse removed");
} ///:~
