//: refactor:trash:Aluminum.java 
// The Aluminum class with prototyping.
package refactor.trash;

public class Aluminum extends Trash {
  private static double val = 1.67f;
  public Aluminum(double wt) { super(wt); }
  public double getValue() { return val; }
  public static void setValue(double newVal) {
    val = newVal;
  }
} ///:~
