//: statemachine:vendingmachine:VendingMachine.java
// Demonstrates use of StateMachine.java
package statemachine.vendingmachine;
import statemachine2.*;

final class VM extends State {
  private VM(String nm) { super(nm); }
  public final static VM
    quiescent = new VM("Quiesecent"),
    collecting = new VM("Collecting"),
    selecting = new VM("Selecting"),
    unavailable = new VM("Unavailable"),
    wantMore = new VM("Want More?"),
    noChange = new VM("Use Exact Change Only"),
    makesChange = new VM("Machine makes change");
}
final class HasChange implements Input {
  private String name;
  private HasChange(String nm) { name = nm; }
  public String toString() { return name; }
  public final static HasChange
    yes = new HasChange("Has change"),
    no = new HasChange("Cannot make change");
}
class ChangeAvailable extends StateMachine {
  public ChangeAvailable() {
    super(VM.makesChange);
    buildTable(new Object[][][]{
      { {VM.makesChange}, // Current state
        // Input, test, transition, next state:
        {HasChange.no, null, null, VM.noChange}},
      { {VM.noChange}, // Current state
        // Input, test, transition, next state:
        {HasChange.yes, null, 
         null, VM.makesChange}},
    });
  }
}
final class Money implements Input {
  private String name;
  private int value;
  private Money(String nm, int val) {
    name = nm;
    value = val;
  }
  public String toString() { return name; }
  public int getValue() { return value; }
  public final static Money
    quarter = new Money("Quarter", 25),
    dollar = new Money("Dollar", 100);
}
final class Quit implements Input {
  private Quit() {}
  public String toString() { return "Quit"; }
  public final static Quit quit = new Quit();
}
final class FirstDigit implements Input {
  private String name;
  private int value;
  private FirstDigit(String nm, int val) {
    name = nm;
    value = val;
  }
  public String toString() { return name; }
  public int getValue() { return value; }
  public final static FirstDigit
    A = new FirstDigit("A", 0),
    B = new FirstDigit("B", 1),
    C = new FirstDigit("C", 2),
    D = new FirstDigit("D", 3);
}
final class SecondDigit implements Input {
  private String name;
  private int value;
  private SecondDigit(String nm, int val) {
    name = nm;
    value = val;
  }
  public String toString() { return name; }
  public int getValue() { return value; }
  public final static SecondDigit
    one = new SecondDigit("one", 0),
    two = new SecondDigit("two", 1),
    three = new SecondDigit("three", 2),
    four = new SecondDigit("four", 3);
}
class ItemSlot {
  int price;
  int quantity;
  static int counter = 0;
  String id = Integer.toString(counter++);
  public ItemSlot(int prc, int quant) {
    price = prc;
    quantity = quant;
  }
  public String toString() { return id; }
  public int getPrice() { return price; }
  public int getQuantity() { return quantity; }
  public void decrQuantity() { quantity--; }
}
public class VendingMachine extends StateMachine{
  StateMachine changeAvailable = 
    new ChangeAvailable();
  int amount = 0;
  FirstDigit first = null;
  ItemSlot[][] items = new ItemSlot[4][4];
  Condition notEnough = new Condition() {
    public boolean condition(Input input) {
      int i1 = first.getValue();
      int i2 = ((SecondDigit)input).getValue();
      return items[i1][i2].getPrice() > amount;
    }
  };
  Condition itemAvailable = new Condition() {
    public boolean condition(Input input) {
      int i1 = first.getValue();
      int i2 = ((SecondDigit)input).getValue();
      return items[i1][i2].getQuantity() > 0;
    }
  };
  Condition itemNotAvailable = new Condition() {
    public boolean condition(Input input) {
      return !itemAvailable.condition(input);
    }
  };
  Transition clearSelection = new Transition() {
    public void transition(Input input) {
      int i1 = first.getValue();
      int i2 = ((SecondDigit)input).getValue();
      ItemSlot is = items[i1][i2];
      System.out.println(
        "Clearing selection: item " + is +
        " costs " + is.getPrice() +
        " and has quantity " + is.getQuantity());
      first = null;
    }
  };
  Transition dispense = new Transition() {
    public void transition(Input input) {
      int i1 = first.getValue();
      int i2 = ((SecondDigit)input).getValue();
      ItemSlot is = items[i1][i2];
      System.out.println("Dispensing item " + 
        is + " costs " + is.getPrice() +
        " and has quantity " + is.getQuantity());
      items[i1][i2].decrQuantity();
      System.out.println("New Quantity " + 
        is.getQuantity());
      amount -= is.getPrice();
      System.out.println("Amount remaining " + 
        amount);
    }
  };
  Transition showTotal = new Transition() {
    public void transition(Input input) {
      amount += ((Money)input).getValue();
      System.out.println("Total amount = " + 
        amount);
    }
  };
  Transition returnChange = new Transition() {
    public void transition(Input input) {
      System.out.println("Returning " + amount);
      amount = 0;
    }
  };
  Transition showDigit = new Transition() {
    public void transition(Input input) {
      first = (FirstDigit)input;
      System.out.println("First Digit= "+ first);
    }
  };
  public VendingMachine() {
    super(VM.quiescent); // Initial state
    for(int i = 0; i < items.length; i++)
      for(int j = 0; j < items[i].length; j++)
        items[i][j] = new ItemSlot((j+1)*25, 5);
    items[3][0] = new ItemSlot(25, 0);
    buildTable(new Object[][][]{
      { {VM.quiescent}, // Current state
        // Input, test, transition, next state:
        {Money.class, null, 
         showTotal, VM.collecting}},
      { {VM.collecting}, // Current state
        // Input, test, transition, next state:
        {Quit.quit, null, 
         returnChange, VM.quiescent},
        {Money.class, null, 
         showTotal, VM.collecting},
        {FirstDigit.class, null, 
         showDigit, VM.selecting}},
      { {VM.selecting}, // Current state
        // Input, test, transition, next state:
        {Quit.quit, null, 
         returnChange, VM.quiescent},
        {SecondDigit.class, notEnough, 
         clearSelection, VM.collecting},
        {SecondDigit.class, itemNotAvailable, 
         clearSelection, VM.unavailable},
        {SecondDigit.class, itemAvailable, 
         dispense, VM.wantMore}},
      { {VM.unavailable}, // Current state
        // Input, test, transition, next state:
        {Quit.quit, null, 
         returnChange, VM.quiescent},
        {FirstDigit.class, null, 
         showDigit, VM.selecting}},
      { {VM.wantMore}, // Current state
        // Input, test, transition, next state:
        {Quit.quit, null, 
         returnChange, VM.quiescent},
        {FirstDigit.class, null, 
         showDigit, VM.selecting}},
    });
  }
} ///:~
