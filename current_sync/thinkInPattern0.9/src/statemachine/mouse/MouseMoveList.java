//: statemachine:mouse:MouseMoveList.java
// A "transformer" to produce a
// List of MouseAction objects.
package statemachine.mouse;
import java.util.*;
import com.bruceeckel.util.*;

public class MouseMoveList extends ArrayList {
  public MouseMoveList(Iterator it) {
    while(it.hasNext())
      add(MouseAction.forString((String)it.next()));
  }
} ///:~
