//: decorator:alldecorators:CoffeeShop2.java
// Coffee example using decorators
package decorator.alldecorators;
import junit.framework.*;

interface DrinkComponent {
  String getDescription();
  float getTotalCost();
}

class Mug implements DrinkComponent {
  public String getDescription() {
    return "mug";
  }
  public float getTotalCost() {
    return 0;
  }
}

abstract class Decorator implements DrinkComponent
{
  protected DrinkComponent component;
  Decorator(DrinkComponent component) {
    this.component = component;
  }
  public float getTotalCost() {
    return component.getTotalCost();
  }
  public abstract String getDescription();
}

class Espresso extends Decorator {
  private float cost = 0.75f;
  private String description = " espresso";
  public Espresso(DrinkComponent component) {
    super(component);
  }
  public float getTotalCost() {
    return component.getTotalCost() + cost;
  }
  public String getDescription() {
    return component.getDescription() +
      description;
  }
}

class Decaf extends Decorator {
  private String description = " decaf";
  public Decaf(DrinkComponent component) {
    super(component);
  }
  public String getDescription() {
    return component.getDescription() +
      description;
  }
}

class FoamedMilk extends Decorator {
  private float cost = 0.25f;
  private String description = " foamed milk";
  public FoamedMilk(DrinkComponent component) {
    super(component);
  }
  public float getTotalCost() {
    return component.getTotalCost() + cost;
  }
  public String getDescription() {
    return component.getDescription() +
      description;
  }
}

class SteamedMilk extends Decorator {
  private float cost = 0.25f;
  private String description = " steamed milk";
  public SteamedMilk(DrinkComponent component) {
    super(component);
  }
  public float getTotalCost() {
    return component.getTotalCost() + cost;
  }
  public String getDescription() {
    return component.getDescription() +
      description;
  }
}

class Whipped extends Decorator {
  private float cost = 0.25f;
  private String description = " whipped cream";
  public Whipped(DrinkComponent component) {
    super(component);
  }
  public float getTotalCost() {
    return component.getTotalCost() + cost;
  }
  public String getDescription() {
    return component.getDescription() +
      description;
  }
}

class Chocolate extends Decorator {
  private float cost = 0.25f;
  private String description = " chocolate";
  public Chocolate(DrinkComponent component) {
    super(component);
  }
  public float getTotalCost() {
    return component.getTotalCost() + cost;
  }
  public String getDescription() {
    return component.getDescription() +
      description;
  }
}

public class CoffeeShop2 extends TestCase  {
  public void testCappuccino() {
    // This just makes sure it will complete 
    // without throwing an exception.
    // Create a plain cappucino
    DrinkComponent cappuccino = new Espresso(
      new FoamedMilk(new Mug()));
    System.out.println(cappuccino.
      getDescription().trim() + ": $" +
      cappuccino.getTotalCost());
  }
  public void testCafeMocha() {
    // This just makes sure it will complete 
    // without throwing an exception.
    // Create a decaf cafe mocha with whipped
    // cream
    DrinkComponent cafeMocha = new Espresso(
      new SteamedMilk(new Chocolate(new Whipped(
      new Decaf(new Mug())))));
    System.out.println(cafeMocha.getDescription().
      trim() + ": $" + cafeMocha.getTotalCost());
  }
  public static void main(String[] args) {
    junit.textui.TestRunner.run(CoffeeShop2.class);
  }
} ///:~
