//: refactor:doubledispatch:DDGlass.java
// Glass for double dispatching.
package refactor.doubledispatch;
import refactor.trash.*;

public class DDGlass extends Glass 
    implements TypedBinMember {
  public DDGlass(double wt) { super(wt); }
  public boolean addToBin(TypedBin[] tb) {
    for(int i = 0; i < tb.length; i++)
      if(tb[i].add(this))
        return true;
    return false;
  }
} ///:~
