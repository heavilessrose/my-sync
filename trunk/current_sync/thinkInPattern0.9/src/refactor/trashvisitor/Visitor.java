//: refactor:trashvisitor:Visitor.java
// The base interface for visitors.
package refactor.trashvisitor;
import refactor.trash.*;

interface Visitor {
  void visit(Aluminum a);
  void visit(Paper p);
  void visit(Glass g);
  void visit(Cardboard c);
} ///:~
