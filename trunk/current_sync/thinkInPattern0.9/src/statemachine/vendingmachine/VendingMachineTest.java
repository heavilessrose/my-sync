//: statemachine:vendingmachine:VendingMachineTest.java
// Demonstrates use of StateMachine.java
package statemachine.vendingmachine;
import statemachine2.*;
import junit.framework.*;

public class VendingMachineTest extends TestCase {
  VendingMachine vm = new VendingMachine();
  Input[] inputs = {
    Money.quarter,
    Money.quarter,
    Money.dollar,
    FirstDigit.A,
    SecondDigit.two,
    FirstDigit.A,
    SecondDigit.two,
    FirstDigit.C,
    SecondDigit.three,
    FirstDigit.D,
    SecondDigit.one,
    Quit.quit,
  };
  public void test() {
    for(int i = 0; i < inputs.length; i++)
      vm.nextState(inputs[i]);
  }
  public static void main(String[] args) {
    junit.textui.TestRunner.run(VendingMachineTest.class);
  }
} ///:~
