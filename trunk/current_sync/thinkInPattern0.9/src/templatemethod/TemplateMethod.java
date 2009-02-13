//: templatemethod:TemplateMethod.java
// Simple demonstration of Template Method.
package templatemethod;
import junit.framework.*;

abstract class ApplicationFramework {
  public ApplicationFramework() {
    templateMethod(); // Dangerous!
  }
  abstract void customize1();
  abstract void customize2();
  final void templateMethod() {
    for(int i = 0; i < 5; i++) {
      customize1();
      customize2();
    }
  }
}

// Create a new "application":
class MyApp extends ApplicationFramework {
  void customize1() { 
    System.out.print("Hello ");
  }
  void customize2() { 
    System.out.println("World!");
  }
}

public class TemplateMethod extends TestCase  {
  MyApp app = new MyApp();
  public void test() {
    // The MyApp constructor does all the work.
    // This just makes sure it will complete 
    // without throwing an exception.
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(TemplateMethod.class);
  }
} ///:~
