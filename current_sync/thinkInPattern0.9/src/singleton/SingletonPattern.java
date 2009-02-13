//: singleton:SingletonPattern.java
// The Singleton design pattern: you can
// never instantiate more than one.
package singleton;
import junit.framework.*;

// Since this isn't inherited from a Cloneable
// base class and cloneability isn't added,
// making it final prevents cloneability from
// being added through inheritance:

final class Singleton {
  private static Singleton s = new Singleton(47);
  private int i;
  private Singleton(int x) { i = x; }
  public static Singleton getReference() { 
    return s; 
  }
  public int getValue() { return i; }
  public void setValue(int x) { i = x; }
}

public class SingletonPattern extends TestCase {
  public void test() {
    Singleton s = Singleton.getReference();
    String result = "" + s.getValue();
    System.out.println(result);
    assertEquals(result, "47");
    Singleton s2 = Singleton.getReference();
    s2.setValue(9);
    result = "" + s.getValue();
    System.out.println(result);
    assertEquals(result, "9");
    try {
      // Can't do this: compile-time error.
      // Singleton s3 = (Singleton)s2.clone();
    } catch(Exception e) {
      throw new RuntimeException(e);
    }
  }
  public static void main(String[] args) {
    junit.textui.TestRunner.run(SingletonPattern.class);
  }
} ///:~
