//: refactor:dynatrash:DynaTrash.java 
// Using a Map of Lists and RTTI to automatically sort 
// trash into ArrayLists. This solution, despite the
// use of RTTI, is extensible.
package refactor.dynatrash;
import refactor.trash.*;
import java.util.*;
import junit.framework.*;

// Generic TypeMap works in any situation:
class TypeMap {
  private Map t = new HashMap();
  public void add(Object o) {
    Class type = o.getClass();
    if(t.containsKey(type))
      ((List)t.get(type)).add(o);
    else {
      List v = new ArrayList();
      v.add(o);
      t.put(type,v);
    }
  }
  public List get(Class type) {
    return (List)t.get(type);
  }
  public Iterator keys() { 
    return t.keySet().iterator(); 
  }
}

// Adapter class to allow callbacks
// from ParseTrash.fillBin():
class TypeMapAdapter implements Fillable {
  TypeMap map;
  public TypeMapAdapter(TypeMap tm) { map = tm; }
  public void addTrash(Trash t) { map.add(t); }
}

public class DynaTrash extends TestCase  {
  TypeMap bin = new TypeMap();
  public DynaTrash() {
    ParseTrash.fillBin("../trash/Trash.dat", 
      new TypeMapAdapter(bin));
  }
  public void test() {
    Iterator keys = bin.keys();
    while(keys.hasNext())
      Trash.sumValue(
        bin.get((Class)keys.next()).iterator());
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(DynaTrash.class);
  }
} ///:~
