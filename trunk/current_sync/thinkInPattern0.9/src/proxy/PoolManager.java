//: proxy:PoolManager.java
package proxy;
import java.util.*;

public class PoolManager {
  private static class PoolItem {
    boolean inUse = false;
    Object item;
    PoolItem(Object item) { this.item = item; }
  }
  public class ReleasableReference {  // Used to build the proxy
    private PoolItem reference;
    private boolean released = false;
    public ReleasableReference(PoolItem reference) {
      this.reference = reference;
    }
    public Object getReference() {
      if(released) 
        throw new RuntimeException(
          "Tried to use reference after it was released");
      return reference.item;
    }
    public void release() { 
      released = true;
      reference.inUse = false;
    }
  }
  private ArrayList items = new ArrayList();
  public void add(Object item) {
    items.add(new PoolItem(item));
  }
  // Different (better?) approach to running out of items:
  public static class EmptyPoolItem {}
  public ReleasableReference get() {
    for(int i = 0; i < items.size(); i++) {
      PoolItem pitem = (PoolItem)items.get(i);
      if(pitem.inUse == false) {
        pitem.inUse = true;
        return new ReleasableReference(pitem);
      }
    }
    // Fail as soon as you try to cast it:
    // return new EmptyPoolItem();
    return null; // temporary
  }
} ///:~
