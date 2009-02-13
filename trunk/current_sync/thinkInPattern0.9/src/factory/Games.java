//: factory:Games.java
// An example of the Abstract Factory pattern.
package factory;
import junit.framework.*;

interface Obstacle {
  void action();
}

interface Player {
  void interactWith(Obstacle o);
}

class Kitty implements Player {
  public void interactWith(Obstacle ob) {
    System.out.print("Kitty has encountered a ");
    ob.action();
  }
}

class KungFuGuy implements Player {
  public void interactWith(Obstacle ob) {
    System.out.print("KungFuGuy now battles a ");
    ob.action();
  }
}

class Puzzle implements Obstacle {
  public void action() { 
    System.out.println("Puzzle"); 
  }
}

class NastyWeapon implements Obstacle {
  public void action() { 
    System.out.println("NastyWeapon"); 
  }
}

// The Abstract Factory:
interface GameElementFactory {
  Player makePlayer();
  Obstacle makeObstacle();
}

// Concrete factories:
class KittiesAndPuzzles 
implements GameElementFactory {
  public Player makePlayer() { 
    return new Kitty();
  }
  public Obstacle makeObstacle() {
    return new Puzzle();
  }
}

class KillAndDismember 
implements GameElementFactory {
  public Player makePlayer() { 
    return new KungFuGuy();
  }
  public Obstacle makeObstacle() {
    return new NastyWeapon();
  }
}

class GameEnvironment {
  private GameElementFactory gef;
  private Player p;
  private Obstacle ob;
  public GameEnvironment(
    GameElementFactory factory) {
    gef = factory;
    p = factory.makePlayer(); 
    ob = factory.makeObstacle();
  }
  public void play() { p.interactWith(ob); }
}

public class Games extends TestCase  {
  GameElementFactory
    kp = new KittiesAndPuzzles(),
    kd = new KillAndDismember();
  GameEnvironment 
    g1 = new GameEnvironment(kp),
    g2 = new GameEnvironment(kd);
  // These just ensure no exceptions are thrown:
  public void test1() { g1.play(); }
  public void test2() { g2.play(); }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(Games.class);
  }
} ///:~
