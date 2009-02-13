//: refactor:trash:Glass.java 
// The Glass class with prototyping.
package refactor.trash;

public class Glass extends Trash {
  private static double val = 0.23f;
  public Glass(double wt) { super(wt); }
  public double getValue() { return val; }
  public static void setValue(double newVal) {
    val = newVal;
  }
} ///:~
