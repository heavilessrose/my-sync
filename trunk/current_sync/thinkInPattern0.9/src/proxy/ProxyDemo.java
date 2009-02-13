//: proxy:ProxyDemo.java
// Simple demonstration of the Proxy pattern.
package proxy;
import junit.framework.*;

interface ProxyBase {
  void f();
  void g();
  void h();
}

class Proxy implements ProxyBase {
  private ProxyBase implementation;
  public Proxy() { 
    implementation = new Implementation(); 
  }
  // Pass method calls to the implementation:
  public void f() { implementation.f(); }
  public void g() { implementation.g(); }
  public void h() { implementation.h(); }
}

class Implementation implements ProxyBase {
  public void f() { 
    System.out.println("Implementation.f()"); 
  }
  public void g() { 
    System.out.println("Implementation.g()"); 
  }
  public void h() { 
    System.out.println("Implementation.h()"); 
  }
}

public class ProxyDemo extends TestCase  {
  Proxy p = new Proxy();
  public void test() {
    // This just makes sure it will complete 
    // without throwing an exception.
    p.f();
    p.g();
    p.h();
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(ProxyDemo.class);
  }
} ///:~
