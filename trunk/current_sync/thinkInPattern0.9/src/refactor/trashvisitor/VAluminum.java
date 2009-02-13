//: refactor:trashvisitor:VAluminum.java
// Taking the previous approach of creating a
// specialized Aluminum for the visitor pattern.
package refactor.trashvisitor;
import refactor.trash.*;

public class VAluminum extends Aluminum 
    implements Visitable {
  public VAluminum(double wt) { super(wt); }
  public void accept(Visitor v) {
    v.visit(this);
  }
} ///:~
