//: simplifying:CollectingParameterDemo.java
package simplifying;
import java.util.*;
import junit.framework.*;

class CollectingParameter extends ArrayList {}

class Filler {
  public void f(CollectingParameter cp) {
    cp.add("accumulating");
  }
  public void g(CollectingParameter cp) {
    cp.add("items");
  }
  public void h(CollectingParameter cp) {
    cp.add("as we go");
  }
}

public class CollectingParameterDemo extends TestCase {
  public void test() {
    Filler filler = new Filler();
    CollectingParameter cp = new CollectingParameter();
    filler.f(cp);
    filler.g(cp);
    filler.h(cp);
    String result = "" + cp;
    System.out.println(cp);
    assertEquals(result,"[accumulating, items, as we go]");
  }
  public static void main(String[] args) {
    junit.textui.TestRunner.run(
      CollectingParameterDemo.class);
  }
} ///:~
