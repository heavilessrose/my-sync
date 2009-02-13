//: multipledispatch:PaperScissorsRock.java
// Demonstration of multiple dispatching.
package multipledispatch;
import java.util.*;
import junit.framework.*;

// An enumeration type:
class Outcome {
  private String name;
  private Outcome(String name) { this.name = name; }
  public final static Outcome
    WIN = new Outcome("wins"), 
    LOSE = new Outcome("loses"), 
    DRAW = new Outcome("draws");
  public String toString() { return name; }
}

interface Item {
  Outcome compete(Item it);
  Outcome eval(Paper p);
  Outcome eval(Scissors s);
  Outcome eval(Rock r);
}

class Paper implements Item {
  public Outcome compete(Item it) { return it.eval(this); }
  public Outcome eval(Paper p) { return Outcome.DRAW; }
  public Outcome eval(Scissors s) { return Outcome.WIN; }
  public Outcome eval(Rock r) { return Outcome.LOSE; }
  public String toString() { return "Paper"; }
}

class Scissors implements Item {
  public Outcome compete(Item it) { return it.eval(this); }
  public Outcome eval(Paper p) { return Outcome.LOSE; }
  public Outcome eval(Scissors s) { return Outcome.DRAW; }
  public Outcome eval(Rock r) { return Outcome.WIN; }
  public String toString() { return "Scissors"; }
}

class Rock implements Item {
  public Outcome compete(Item it) { return it.eval(this); }
  public Outcome eval(Paper p) { return Outcome.WIN; }
  public Outcome eval(Scissors s) { return Outcome.LOSE; }
  public Outcome eval(Rock r) { return Outcome.DRAW; }
  public String toString() { return "Rock"; }
}

class ItemGenerator {
  private static Random rand = new Random();
  public static Item newItem() {
    switch(rand.nextInt(3)) {
      default:
      case 0: return new Scissors();
      case 1: return new Paper();
      case 2: return new Rock();
    }
  }
}

class Compete {
  public static void match(Item a, Item b) {
    System.out.println(
      a + " " + a.compete(b) + " vs. " + b);
  }
}

public class PaperScissorsRock extends TestCase  {
  static int SIZE = 20;
  public void test() {
    for(int i = 0; i < SIZE; i++)
      Compete.match(ItemGenerator.newItem(),
        ItemGenerator.newItem());
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(PaperScissorsRock.class);
  }
} ///:~
