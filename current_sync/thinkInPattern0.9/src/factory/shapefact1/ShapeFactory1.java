//: factory:shapefact1:ShapeFactory1.java
// A simple static factory method.
package factory.shapefact1;
import java.util.*;
import junit.framework.*;

abstract class Shape {
  public abstract void draw();
  public abstract void erase();
  public static Shape factory(String type) {
    if(type.equals("Circle")) return new Circle();
    if(type.equals("Square")) return new Square();
    throw new RuntimeException(
      "Bad shape creation: " + type);
  }
}

class Circle extends Shape {
  Circle() {} // Package-access constructor
  public void draw() { 
    System.out.println("Circle.draw"); 
  }
  public void erase() { 
    System.out.println("Circle.erase"); 
  }
}

class Square extends Shape {
  Square() {} // Package-access constructor
  public void draw() { 
    System.out.println("Square.draw"); 
  }
  public void erase() { 
    System.out.println("Square.erase"); 
  }
}

public class ShapeFactory1 extends TestCase  {
  String shlist[] = { "Circle", "Square", 
    "Square", "Circle", "Circle", "Square" };
  List shapes = new ArrayList();
  public void test() {
    Iterator it = Arrays.asList(shlist).iterator();
    while(it.hasNext())
      shapes.add(Shape.factory((String)it.next()));
    it = shapes.iterator();
    while(it.hasNext()) {
      Shape s = (Shape)it.next();
      s.draw();
      s.erase();
    }
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(ShapeFactory1.class);
  }
} ///:~
