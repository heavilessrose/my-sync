//: decorator:nodecorators:CoffeeShop.java
// Coffee example with no decorators
package decorator.nodecorators;
import junit.framework.*;

class Espresso {}
class DoubleEspresso {}
class EspressoConPanna {}

class Cappuccino {
  private float cost = 1;
  private String description = "Cappucino";
  public float getCost() {
    return cost;
  }
  public String getDescription() {
    return description;
  }
}
class CappuccinoDecaf {}
class CappuccinoDecafWhipped {}
class CappuccinoDry {}
class CappuccinoDryWhipped {}
class CappuccinoExtraEspresso {}
class CappuccinoExtraEspressoWhipped {}
class CappuccinoWhipped {}

class CafeMocha {}
class CafeMochaDecaf {}
class CafeMochaDecafWhipped {
  private float cost = 1.25f;
  private String description =
    "Cafe Mocha decaf whipped cream";
  public float getCost() {
    return cost;
  }
  public String getDescription() {
    return description;
  }
}
class CafeMochaExtraEspresso {}
class CafeMochaExtraEspressoWhipped {}
class CafeMochaWet {}
class CafeMochaWetWhipped {}
class CafeMochaWhipped {}

class CafeLatte {}
class CafeLatteDecaf {}
class CafeLatteDecafWhipped {}
class CafeLatteExtraEspresso {}
class CafeLatteExtraEspressoWhipped {}
class CafeLatteWet {}
class CafeLatteWetWhipped {}
class CafeLatteWhipped {}

public class CoffeeShop extends TestCase  {
  public void testCappuccino() {
    // This just makes sure it will complete 
    // without throwing an exception.
    // Create a plain cappuccino
    Cappuccino cappuccino = new Cappuccino();
    System.out.println(cappuccino.getDescription()
      + ": $" + cappuccino.getCost());
  }
  public void testCafeMocha() {
    // This just makes sure it will complete 
    // without throwing an exception.
    // Create a decaf cafe mocha with whipped
    // cream
    CafeMochaDecafWhipped cafeMocha =
      new CafeMochaDecafWhipped();
    System.out.println(cafeMocha.getDescription()
      + ": $" + cafeMocha.getCost());
  }
  public static void main(String[] args) {
    junit.textui.TestRunner.run(CoffeeShop.class);
  }
} ///:~
