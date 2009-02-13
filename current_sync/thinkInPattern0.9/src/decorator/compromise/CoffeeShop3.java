//: decorator:compromise:CoffeeShop3.java
// Coffee example with a compromise of basic
// combinations and decorators
package decorator.compromise;
import junit.framework.*;

interface DrinkComponent {
  float getTotalCost();
  String getDescription();
}

class Espresso implements DrinkComponent {
  private String description = "Espresso";
  private float cost = 0.75f;
  public float getTotalCost() {
    return cost;
  }
  public String getDescription() {
    return description;
  }
}

class EspressoConPanna implements DrinkComponent {
  private String description = "EspressoConPare";
  private float cost = 1;
  public float getTotalCost() {
    return cost;
  }
  public String getDescription() {
    return description;
  }
}

class Cappuccino implements DrinkComponent {
  private float cost = 1;
  private String description = "Cappuccino";
  public float getTotalCost() {
    return cost;
  }
  public String getDescription() {
    return description;
  }
}

class CafeLatte implements DrinkComponent {
  private float cost = 1;
  private String description = "Cafe Late";
  public float getTotalCost() {
    return cost;
  }
  public String getDescription() {
    return description;
  }
}

class CafeMocha implements DrinkComponent {
  private float cost = 1.25f;
  private String description = "Cafe Mocha";
  public float getTotalCost() {
    return cost;
  }
  public String getDescription() {
    return description;
  }
}

abstract class Decorator implements DrinkComponent {
  protected DrinkComponent component;
  public Decorator(DrinkComponent component) {
    this.component = component;
  }
  public float getTotalCost() {
    return component.getTotalCost();
  }
  public String getDescription() {
    return component.getDescription();
  }
}

class ExtraEspresso extends Decorator {
  private float cost = 0.75f;
  public ExtraEspresso(DrinkComponent component) {
    super(component);
  }
  public String getDescription() {
    return component.getDescription() +
      " extra espresso";
  }
  public float getTotalCost() {
    return cost + component.getTotalCost();
  }
}

class Whipped extends Decorator {
  private float cost = 0.50f;
  public Whipped(DrinkComponent component) {
    super(component);
  }
  public float getTotalCost() {
    return cost + component.getTotalCost();
  }
  public String getDescription() {
    return component.getDescription() +
      " whipped cream";
  }
}

class Decaf extends Decorator{
  public Decaf(DrinkComponent component) {
    super(component);
  }
  public String getDescription() {
    return component.getDescription() + " decaf";
  }
}

class Dry extends Decorator {
  public Dry(DrinkComponent component) {
    super(component);
  }
  public String getDescription() {
    return component.getDescription() +
      " extra foamed milk";
  }
}

class Wet extends Decorator {
  public Wet(DrinkComponent component) {
    super(component);
  }
  public String getDescription() {
    return component.getDescription() +
      " extra steamed milk";
  }
}

public class CoffeeShop3 extends TestCase  {
  public void testCappuccino() {
    // This just makes sure it will complete 
    // without throwing an exception.
    // Create a plain cappucino
    DrinkComponent cappuccino = new Cappuccino();
    System.out.println(cappuccino.getDescription()
      + ": $" + cappuccino.getTotalCost());
  }
  public void testCafeMocha() {
    // This just makes sure it will complete 
    // without throwing an exception.
    // Create a decaf cafe mocha with whipped
    // cream
    DrinkComponent cafeMocha = new Whipped(
      new Decaf(new CafeMocha()));
    System.out.println(cafeMocha.getDescription()
      + ": $" + cafeMocha.getTotalCost());
  }
  public static void main(String[] args) {
    junit.textui.TestRunner.run(CoffeeShop3.class);
  }
} ///:~
