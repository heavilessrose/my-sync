//: singleton:PoolManager.java
package singleton;
import java.util.*;

public class PoolManager {
  private static class PoolItem {
    boolean inUse = false;
    Object item;
    PoolItem(Object item) { this.item = item; }
  }
  private ArrayList items = new ArrayList();
  public void add(Object item) {
    items.add(new PoolItem(item));
  }
  static class EmptyPoolException extends Exception {}
  public Object get() throws EmptyPoolException {
    for(int i = 0; i < items.size(); i++) {
      PoolItem pitem = (PoolItem)items.get(i);
      if(pitem.inUse == false) {
        pitem.inUse = true;
        return pitem.item;
      }
    }
    // Fail early:
    throw new EmptyPoolException();
    // return null; // Delayed failure
  }
  public void release(Object item) {
    for(int i = 0; i < items.size(); i++) {
      PoolItem pitem = (PoolItem)items.get(i);
      if(item == pitem.item) {
        pitem.inUse = false;
        return;
      }
    }
    throw new RuntimeException(item + " not found");
  }
} ///:~
