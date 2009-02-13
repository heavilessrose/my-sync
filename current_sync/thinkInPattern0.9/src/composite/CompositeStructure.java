//: composite:CompositeStructure.java
package composite;
import java.util.*;
import junit.framework.*;

interface Component {
  void operation();
}

class Leaf implements Component {
  private String name;
  public Leaf(String name) { this.name = name; }
  public String toString() { return name; }
  public void operation() {
    System.out.println(this);
  }
}

class Node extends ArrayList implements Component {
  private String name;
  public Node(String name) { this.name = name; }
  public String toString() { return name; }
  public void operation() {
    System.out.println(this);
    for(Iterator it = iterator(); it.hasNext(); )
      ((Component)it.next()).operation();
  }
}

public class CompositeStructure extends TestCase {
  public void test() {
    Node root = new Node("root");
    root.add(new Leaf("Leaf1"));
    Node c2 = new Node("Node1");
    c2.add(new Leaf("Leaf2"));
    c2.add(new Leaf("Leaf3"));
    root.add(c2);
    c2 = new Node("Node2");
    c2.add(new Leaf("Leaf4"));
    c2.add(new Leaf("Leaf5"));
    root.add(c2);
    root.operation();
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(CompositeStructure.class);
  }
} ///:~
