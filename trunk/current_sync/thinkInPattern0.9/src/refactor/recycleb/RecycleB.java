//: refactor:recycleb:RecycleB.java
// Containers that grab objects of interest.
package refactor.recycleb;
import refactor.trash.*;
import java.util.*;
import junit.framework.*;

// A container that admits only the right type
// of Trash (established in the constructor):
class Tbin {
  private List list = new ArrayList();
  private Class type;
  public Tbin(Class binType) { type = binType; }
  public boolean grab(Trash t) {
    // Comparing class types:
    if(t.getClass().equals(type)) {
      list.add(t);
      return true; // Object grabbed
    }
    return false; // Object not grabbed
  }
  public Iterator iterator() {
    return list.iterator();
  }
}

class TbinList extends ArrayList {
  void sortTrashItem(Trash t) {
    Iterator e = iterator(); // Iterate over self
    while(e.hasNext())
      if(((Tbin)e.next()).grab(t)) return;
    // Need a new Tbin for this type:
    add(new Tbin(t.getClass()));
    sortTrashItem(t); // Recursive call
  }
}

public class RecycleB extends TestCase  {
  Collection bin = new ArrayList();
  TbinList trashBins = new TbinList();
  public RecycleB() {
    ParseTrash.fillBin("../trash/Trash.dat",bin);
  }
  public void test() {
    Iterator it = bin.iterator();
    while(it.hasNext())
      trashBins.sortTrashItem((Trash)it.next());
    Iterator e = trashBins.iterator();
    while(e.hasNext())
      Trash.sumValue(((Tbin)e.next()).iterator());
    Trash.sumValue(bin.iterator());
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(RecycleB.class);
  }
} ///:~
