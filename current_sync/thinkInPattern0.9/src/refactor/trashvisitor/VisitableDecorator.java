//: refactor:trashvisitor:VisitableDecorator.java
// A decorator that adapts the generic Trash
// classes to the visitor pattern.
// [ Use a Dynamic Proxy here?? ]
package refactor.trashvisitor;
import refactor.trash.*;
import java.lang.reflect.*;

public class VisitableDecorator
extends Trash implements Visitable {
  private Trash delegate;
  private Method dispatch;
  public VisitableDecorator(Trash t) {
    delegate = t;
    try {
      dispatch = Visitor.class.getMethod (
        "visit", new Class[] { t.getClass() }
      );
    } catch(Exception e) {
      throw new RuntimeException(e);
    }
  }
  public double getValue() {
    return delegate.getValue();
  }
  public double getWeight() {
    return delegate.getWeight();
  }
  public void accept(Visitor v) {
    try {
      dispatch.invoke(v, new Object[]{delegate});
    } catch(Exception e) {
      throw new RuntimeException(e);
    }
  }
} ///:~
