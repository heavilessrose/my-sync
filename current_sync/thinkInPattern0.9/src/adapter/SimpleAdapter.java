//: adapter:SimpleAdapter.java
// "Object Adapter" from GoF diagram
package adapter;
import junit.framework.*;

class Target {
  public void request() {}
}

class Adaptee {
  public void specificRequest() {
    System.out.println("Adaptee: SpecificRequest");
  }
}

class Adapter extends Target {
  private Adaptee adaptee;
  public Adapter(Adaptee a) {
    adaptee = a;
  }
  public void request() {
    adaptee.specificRequest();
  }
}

public class SimpleAdapter extends TestCase  {
  Adaptee a = new Adaptee();
  Target t = new Adapter(a);
  public void test() {
    t.request();
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(SimpleAdapter.class);
  }
} ///:~
