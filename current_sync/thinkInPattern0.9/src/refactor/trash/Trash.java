//: refactor:trash:Trash.java
// Base class for Trash recycling examples.
package refactor.trash;
import java.util.*;
import java.lang.reflect.*;

public abstract class Trash {
  private double weight;
  public Trash(double wt) { weight = wt; }
  public Trash() {}
  public abstract double getValue();
  public double getWeight() { return weight; }
  // Sums the value of Trash given an
  // Iterator to any container of Trash:
  public static void sumValue(Iterator it) {
    double val = 0.0f;
    while(it.hasNext()) {
      // One kind of RTTI: A dynamically-checked cast
      Trash t = (Trash)it.next();
      val += t.getWeight() * t.getValue();
      System.out.println(
        "weight of " +
        // Using RTTI to get type
        // information about the class:
        t.getClass().getName() + " = " + t.getWeight());
    }
    System.out.println("Total value = " + val);
  }
  public static class Messenger {
    public String id;
    public double data;
    public Messenger(String name, double val) {
      id = name;
      data = val;
    }
  }
  // Remainder of class provides 
  // support for prototyping:
  private static List trashTypes = new ArrayList();
  public static Trash factory(Messenger info) {
    Iterator it = trashTypes.iterator();
    while(it.hasNext()) {
      // Somehow determine the new type
      // to create, and create one:
      Class tc = (Class)it.next();
      if (tc.getName().indexOf(info.id) != -1) {
        try {
          // Get the dynamic constructor method
          // that takes a double argument:
          Constructor ctor = tc.getConstructor(
              new Class[]{ double.class });
          // Call the constructor  
          // to create a new object:
          return (Trash)ctor.newInstance(
            new Object[]{new Double(info.data)});
        } catch(Exception e) {
          throw new RuntimeException(
            "Cannot Create Trash", e);
        }
      }
    }
    // Class was not in the list. Try to load it,
    // but it must be in your class path!
    try {
      System.out.println("Loading " + info.id);
      trashTypes.add(Class.forName(info.id));
    } catch(Exception e) {
      throw new RuntimeException("Prototype not found", e);
    }
    // Loaded successfully. 
    // Recursive call should work:
    return factory(info);
  }
} ///:~
