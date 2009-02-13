//: refactor:trashvisitor:Visitable.java
// An interface to add visitor functionality 
// to the Trash hierarchy without 
// modifying the base class.
package refactor.trashvisitor;
import refactor.trash.*;

interface Visitable {
  // The new method:
  void accept(Visitor v);
} ///:~
