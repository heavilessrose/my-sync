//: refactor:trashvisitor:TrashVisitor.java 
// The "visitor" pattern with VisitableDecorators.
package refactor.trashvisitor;
import refactor.trash.*;
import java.util.*;
import junit.framework.*;

// Specific group of algorithms packaged
// in each implementation of Visitor:
class PriceVisitor implements Visitor {
  private double alSum; // Aluminum
  private double pSum; // Paper
  private double gSum; // Glass
  private double cSum; // Cardboard
  public void visit(Aluminum al) {
    double v = al.getWeight() * al.getValue();
    System.out.println(
      "value of Aluminum= " + v);
    alSum += v;
  }
  public void visit(Paper p) {
    double v = p.getWeight() * p.getValue();
    System.out.println(
      "value of Paper= " + v);
    pSum += v;
  }
  public void visit(Glass g) {
    double v = g.getWeight() * g.getValue();
    System.out.println(
      "value of Glass= " + v);
    gSum += v;
  }
  public void visit(Cardboard c) {
    double v = c.getWeight() * c.getValue();
    System.out.println(
      "value of Cardboard = " + v);
    cSum += v;
  }
  void total() {
    System.out.println(
      "Total Aluminum: $" + alSum +
      "\n Total Paper: $" + pSum + 
      "\nTotal Glass: $" + gSum + 
      "\nTotal Cardboard: $" + cSum +
      "\nTotal: $" + 
        (alSum + pSum + gSum + cSum));
  }
}

class WeightVisitor implements Visitor {
  private double alSum; // Aluminum
  private double pSum; // Paper
  private double gSum; // Glass
  private double cSum; // Cardboard
  public void visit(Aluminum al) {
    alSum += al.getWeight();
    System.out.println("weight of Aluminum = "
        + al.getWeight());
  }
  public void visit(Paper p) {
    pSum += p.getWeight();
    System.out.println("weight of Paper = "
        + p.getWeight());
  }
  public void visit(Glass g) {
    gSum += g.getWeight();
    System.out.println("weight of Glass = "
        + g.getWeight());
  }
  public void visit(Cardboard c) {
    cSum += c.getWeight();
    System.out.println("weight of Cardboard = "
        + c.getWeight());
  }
  void total() {
    System.out.println(
      "Total weight Aluminum: " + alSum +
      "\nTotal weight Paper: " + pSum +
      "\nTotal weight Glass: " + gSum +
      "\nTotal weight Cardboard: " + cSum +
      "\nTotal weight: " + (alSum + pSum + gSum + cSum));

  }
}

public class TrashVisitor extends TestCase  {
  Collection bin = new ArrayList();
  PriceVisitor pv = new PriceVisitor();
  WeightVisitor wv = new WeightVisitor();
  public TrashVisitor() {
    ParseTrash.fillBin("../trash/Trash.dat", 
      new FillableVisitor(
        new FillableCollection(bin)));
  }
  public void test() {
    Iterator it = bin.iterator();
    while(it.hasNext()) {
      Visitable v = (Visitable)it.next();
      v.accept(pv);
      v.accept(wv);
    }
    pv.total();
    wv.total();
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(TrashVisitor.class);
  }
} ///:~
