//: refactor:recycleap:RecycleAP.java 
// Recycling with RTTI and Prototypes.
package refactor.recycleap;
import refactor.trash.*;
import java.util.*;
import junit.framework.*;

public class RecycleAP extends TestCase  {
  Collection
    bin = new ArrayList(), 
    glassBin = new ArrayList(),
    paperBin = new ArrayList(),
    alBin = new ArrayList();
  public RecycleAP() {
    // Fill up the Trash bin:
    ParseTrash.fillBin("../trash/Trash.dat", bin);
  }
  public void test() {
    Iterator sorter = bin.iterator();
    // Sort the Trash:
    while(sorter.hasNext()) {
      Object t = sorter.next();
      // RTTI to show class membership:
      if(t instanceof Aluminum)
        alBin.add(t);
      else if(t instanceof Paper)
        paperBin.add(t);
      else if(t instanceof Glass)
        glassBin.add(t);
      else
        System.err.println("Unknown type " + t);
    }
    Trash.sumValue(alBin.iterator());
    Trash.sumValue(paperBin.iterator());
    Trash.sumValue(glassBin.iterator());
    Trash.sumValue(bin.iterator());
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(RecycleAP.class);
  }
} ///:~
