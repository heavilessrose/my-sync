//: refactor:trash:ParseTrash.java 
// Parse file contents into Trash objects,
// placing each into a Fillable holder.
package refactor.trash;
import java.util.*;
import java.io.*;
import com.bruceeckel.util.StringList;

public class ParseTrash {
  public static void 
  fillBin(String filePath, Fillable bin) {
    Iterator it = new StringList(filePath).iterator();
    while(it.hasNext()) {
      String line = (String)it.next();
      String type = line.substring(0, 
        line.indexOf(':')).trim();
      double weight = Double.valueOf(
        line.substring(line.indexOf(':') + 1)
          .trim()).doubleValue();
      bin.addTrash(
        Trash.factory(new Trash.Messenger(type, weight)));
    }
  }
  // Special case to handle Collection:
  public static void 
  fillBin(String filePath, Collection bin) {
    fillBin(filePath, new FillableCollection(bin));
  }
} ///:~
