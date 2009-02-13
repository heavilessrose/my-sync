//: bridge:BridgeStructure.java
// A demonstration of the structure and operation
// of the Bridge Pattern.
package bridge;
import junit.framework.*;

class Abstraction {
  private Implementation implementation;
  public Abstraction(Implementation imp) {
    implementation = imp;
  }
  // Abstraction used by the various front-end 
  // objects in order to implement their 
  // different interfaces.
  public void service1() {
    // Implement this feature using some 
    // combination of back-end implementation:
    implementation.facility1();
    implementation.facility2();
  }
  public void service2() {
    // Implement this feature using some other
    // combination of back-end implementation:
    implementation.facility2();
    implementation.facility3();
  }
  public void service3() {
    // Implement this feature using some other
    // combination of back-end implementation:
    implementation.facility1();
    implementation.facility2();
    implementation.facility4();
  }
  // For use by subclasses:
  protected Implementation getImplementation() { 
    return implementation; 
  }
}

class ClientService1 extends Abstraction {
  public ClientService1(Implementation imp) { super(imp); }
  public void serviceA() {
    service1();
    service2();
  }
  public void serviceB() {
    service3();
  }
}

class ClientService2 extends Abstraction {
  public ClientService2(Implementation imp) { super(imp); }
  public void serviceC() {
    service2();
    service3();
  }
  public void serviceD() {
    service1();
    service3();
  }
  public void serviceE() {
    getImplementation().facility3();
  }
}

interface Implementation {
  // The common implementation provided by the 
  // back-end objects, each in their own way.
  void facility1();
  void facility2();
  void facility3();
  void facility4();
}

class Library1 {
  public void method1() {
    System.out.println("Library1.method1()");
  }
  public void method2() {
    System.out.println("Library1.method2()");
  }
}

class Library2 {
  public void operation1() {
    System.out.println("Library2.operation1()");
  }
  public void operation2() {
    System.out.println("Library2.operation2()");
  }
  public void operation3() {
    System.out.println("Library2.operation3()");
  }
}

class Implementation1 implements Implementation {
  // Each facility delegates to a different library
  // in order to fulfill the obligations.
  private Library1 delegate = new Library1();
  public void facility1() {
    System.out.println("Implementation1.facility1");
    delegate.method1();
  }
  public void facility2() {
    System.out.println("Implementation1.facility2");
    delegate.method2();
  }
  public void facility3() {
    System.out.println("Implementation1.facility3");
    delegate.method2();
    delegate.method1();
  }
  public void facility4() {
    System.out.println("Implementation1.facility4");
    delegate.method1();
  }
}
  
class Implementation2 implements Implementation {
  private Library2 delegate = new Library2();
  public void facility1() {
    System.out.println("Implementation2.facility1");
    delegate.operation1();
  }
  public void facility2() {
    System.out.println("Implementation2.facility2");
    delegate.operation2();
  }
  public void facility3() {
    System.out.println("Implementation2.facility3");
    delegate.operation3();
  }
  public void facility4() {
    System.out.println("Implementation2.facility4");
    delegate.operation1();
  }
}

public class BridgeStructure extends TestCase  {
  public void test1() {
    // Here, the implementation is determined by 
    // the client at creation time:
    ClientService1 cs1 = 
      new ClientService1(new Implementation1());
    cs1.serviceA();
    cs1.serviceB();
  }
  public void test2() {
    ClientService1 cs1 = 
      new ClientService1(new Implementation2());
    cs1.serviceA();
    cs1.serviceB();
  }
  public void test3() {
    ClientService2 cs2 = 
      new ClientService2(new Implementation1());
    cs2.serviceC();
    cs2.serviceD();
    cs2.serviceE();
  }
  public void test4() {
    ClientService2 cs2 = 
      new ClientService2(new Implementation2());
    cs2.serviceC();
    cs2.serviceD();
    cs2.serviceE();
  }
  public static void main(String[] args) {
    junit.textui.TestRunner.run(BridgeStructure.class);
  }  
} ///:~
