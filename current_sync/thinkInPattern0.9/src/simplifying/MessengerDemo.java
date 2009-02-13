//: simplifying:MessengerDemo.java
package simplifying;
import junit.framework.*;

class Point { // A messenger
  public int x, y, z; // Since it's just a carrier
  public Point(int x, int y, int z) {
    this.x = x;
    this.y = y;
    this.z = z;
  }
  public Point(Point p) { // Copy-constructor
    this.x = p.x;
    this.y = p.y;
    this.z = p.z;
  }
  public String toString() {
    return "x: " + x + " y: " + y + " z: " + z;
  }
}

class Vector { 
  public int magnitude, direction;
  public Vector(int magnitude, int direction) {
    this.magnitude = magnitude;
    this.direction = direction;
  }
}

class Space {
  public static Point translate(Point p, Vector v) {
    p = new Point(p); // Don't modify the original
    // Perform calculation using v. Dummy calculation:
    p.x = p.x + 1;
    p.y = p.y + 1;
    p.z = p.z + 1;
    return p;
  }
}

public class MessengerDemo extends TestCase {
  public void test() {
    Point p1 = new Point(1, 2, 3);
    Point p2 = Space.translate(p1, new Vector(11, 47));
    String result = "p1: " + p1 + " p2: " + p2;
    System.out.println(result);
    assertEquals(result, 
      "p1: x: 1 y: 2 z: 3 p2: x: 2 y: 3 z: 4");
  }   
  public static void main(String[] args) {
    junit.textui.TestRunner.run(MessengerDemo.class);
  }
} ///:~
