//: refactor:recyclea:RecycleA.java 
// Recycling with RTTI.
package refactor.recyclea;
import java.util.*;
import java.io.*;
import junit.framework.*;

abstract class Trash {
  private double weight;
  Trash(double wt) { weight = wt; }
  abstract double getValue();
  double getWeight() { return weight; }
  // Sums the value of Trash in a bin:
  static void sumValue(Iterator it) {
    double val = 0.0f;
    while(it.hasNext()) {
      // One kind of RTTI:
      // A dynamically-checked cast
      Trash t = (Trash)it.next();
      // Polymorphism in action:
      val += t.getWeight() * t.getValue();
      System.out.println(
        "weight of " +
        // Using RTTI to get type
        // information about the class:
        t.getClass().getName() +
        " = " + t.getWeight());
    }
    System.out.println("Total value = " + val);
  }
}

class Aluminum extends Trash {
  static double val  = 1.67f;
  Aluminum(double wt) { super(wt); }
  double getValue() { return val; }
  static void setValue(double newval) {
    val = newval;
  }
}

class Paper extends Trash {
  static double val = 0.10f;
  Paper(double wt) { super(wt); }
  double getValue() { return val; }
  static void setValue(double newval) {
    val = newval;
  }
}

class Glass extends Trash {
  static double val = 0.23f;
  Glass(double wt) { super(wt); }
  double getValue() { return val; }
  static void setValue(double newval) {
    val = newval;
  }
}

public class RecycleA extends TestCase  {
  Collection 
    bin = new ArrayList(),
    glassBin = new ArrayList(),
    paperBin = new ArrayList(),
    alBin = new ArrayList();
  private static Random rand = new Random();
  public RecycleA() {
    // Fill up the Trash bin:
    for(int i = 0; i < 30; i++)
      switch(rand.nextInt(3)) {
        case 0 :
          bin.add(new
            Aluminum(rand.nextDouble() * 100));
          break;
        case 1 :
          bin.add(new
            Paper(rand.nextDouble() * 100));
          break;
        case 2 :
          bin.add(new
            Glass(rand.nextDouble() * 100));
    }
  }
  public void test() {
    Iterator sorter = bin.iterator();
    // Sort the Trash:
    while(sorter.hasNext()) {
      Object t = sorter.next();
      // RTTI to show class membership:
      if(t instanceof Aluminum)
        alBin.add(t);
      if(t instanceof Paper)
        paperBin.add(t);
      if(t instanceof Glass)
        glassBin.add(t);
    }
    Trash.sumValue(alBin.iterator());
    Trash.sumValue(paperBin.iterator());
    Trash.sumValue(glassBin.iterator());
    Trash.sumValue(bin.iterator());
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(RecycleA.class);
  }
} ///:~
