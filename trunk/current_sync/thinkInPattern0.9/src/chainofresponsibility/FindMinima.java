//: chainofresponsibility:FindMinima.java
package chainofresponsibility;
import com.bruceeckel.util.*; // Arrays2.toString()
import junit.framework.*;

// Carries the result data and 
// whether the strategy was successful:
class LineData {
  public double[] data;
  public LineData(double[] data) { this.data = data; }
  private boolean succeeded;
  public boolean isSuccessful() { return succeeded; }
  public void setSuccessful(boolean b) { succeeded = b; }
}

interface Strategy {
  LineData strategy(LineData m);
}

class LeastSquares implements Strategy {
  public LineData strategy(LineData m) {
    System.out.println("Trying LeastSquares algorithm");
    LineData ld = (LineData)m;
    // [ Actual test/calculation here ]
    LineData r = new LineData(
      new double[] { 1.1, 2.2 }); // Dummy data
    r.setSuccessful(false);
    return r;
  }
}

class NewtonsMethod implements Strategy {
  public LineData strategy(LineData m) {
    System.out.println("Trying NewtonsMethod algorithm");
    LineData ld = (LineData)m;
    // [ Actual test/calculation here ]
    LineData r = new LineData(
      new double[] { 3.3, 4.4 }); // Dummy data
    r.setSuccessful(false);
    return r;
  }
}

class Bisection implements Strategy {
  public LineData strategy(LineData m) {
    System.out.println("Trying Bisection algorithm");
    LineData ld = (LineData)m;
    // [ Actual test/calculation here ]
    LineData r = new LineData(
      new double[] { 5.5, 6.6 }); // Dummy data
    r.setSuccessful(true);
    return r;
  }
}

class ConjugateGradient implements Strategy {
  public LineData strategy(LineData m) {
    System.out.println(
      "Trying ConjugateGradient algorithm");
    LineData ld = (LineData)m;
    // [ Actual test/calculation here ]
    LineData r = new LineData(
      new double[] { 5.5, 6.6 }); // Dummy data
    r.setSuccessful(true);
    return r;
  }
}

class MinimaFinder {
  private static Strategy[] solutions = {
    new LeastSquares(),
    new NewtonsMethod(),
    new Bisection(),
    new ConjugateGradient(),
  };
  public static LineData solve(LineData line) {
    LineData r = line;
    for(int i = 0; i < solutions.length; i++) {
      r = solutions[i].strategy(r);
      if(r.isSuccessful())
        return r;
    }
    throw new RuntimeException("unsolved: " + line);
  }
}
  
public class FindMinima extends TestCase  {
  LineData line = new LineData(new double[]{ 
    1.0, 2.0, 1.0, 2.0, -1.0, 3.0, 4.0, 5.0, 4.0
  });
  public void test() {
    System.out.println(Arrays2.toString(
      ((LineData)MinimaFinder.solve(line)).data));
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(FindMinima.class);
  }
} ///:~
