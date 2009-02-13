//: state:StateDemo.java
// Simple demonstration of the State pattern.
package state;
import junit.framework.*;

interface State {
  void operation1();
  void operation2();
  void operation3();
}

class ServiceProvider {
  private State state;
  public ServiceProvider(State state) {
    this.state = state;
  }
  public void changeState(State newState) {
    state = newState;
  }
  // Pass method calls to the implementation:
  public void service1() {
    // ...
    state.operation1();
    // ...
    state.operation3();
  }
  public void service2() {
    // ...
    state.operation1();
    // ...
    state.operation2();
  }
  public void service3() {
    // ...
    state.operation3();
    // ...
    state.operation2();
  }
}

class Implementation1 implements State {
  public void operation1() {
    System.out.println("Implementation1.operation1()");
  }
  public void operation2() {
    System.out.println("Implementation1.operation2()");
  }
  public void operation3() {
    System.out.println("Implementation1.operation3()");
  }
}

class Implementation2 implements State {
  public void operation1() {
    System.out.println("Implementation2.operation1()");
  }
  public void operation2() {
    System.out.println("Implementation2.operation2()");
  }
  public void operation3() {
    System.out.println("Implementation2.operation3()");
  }
}

public class StateDemo extends TestCase  {
  static void run(ServiceProvider sp) {
    sp.service1();
    sp.service2();
    sp.service3();
  }
  ServiceProvider sp = 
    new ServiceProvider(new Implementation1());
  public void test() {
    run(sp);
    sp.changeState(new Implementation2());
    run(sp);
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(StateDemo.class);
  }
} ///:~
