//: projects:Rat.java
package projects;

public class Rat {
  static int ratCount = 0;
  private Maze prison;
  private int vertDir = 0; 
  private int horizDir = 0;
  private int x,y;
  private int myRatNo = 0;
  public Rat(Maze maze, int xStart, int yStart) {
    myRatNo = ratCount++;
    System.out.println("Rat no." + myRatNo + 
      " ready to scurry.");
    prison = maze;
    x = xStart;
    y = yStart;
    prison.setXY(x,y, (byte)'R');
    new Thread() {
      public void run(){ scurry(); }
    }.start();
  }
  public void scurry() {
    // Try and maintain direction if possible.
    // Horizontal backward
    boolean ratCanMove = true;
    while(ratCanMove) {
      ratCanMove = false;
      // South 
      if (prison.isEmptyXY(x, y + 1)) {
        vertDir = 1; horizDir = 0;         
        ratCanMove = true;
      }
      // North
      if (prison.isEmptyXY(x, y - 1))
        if (ratCanMove)
          new Rat(prison, x, y-1);
          // Rat can move already, so give 
          // this choice to the next rat.
        else {
          vertDir = -1; horizDir = 0;         
          ratCanMove = true;
        }
      // West
      if (prison.isEmptyXY(x-1, y))
        if (ratCanMove)
          new Rat(prison, x-1, y);   
          // Rat can move already, so give 
          // this choice to the next rat.
        else {
          vertDir = 0; horizDir = -1;         
          ratCanMove = true;
        }
      // East
      if (prison.isEmptyXY(x+1, y))
        if (ratCanMove)
          new Rat(prison, x+1, y);   
          // Rat can move already, so give 
          // this choice to the next rat.
        else {
          vertDir = 0; horizDir = 1;         
          ratCanMove = true;
        }
      if (ratCanMove) { // Move original rat.
        x += horizDir;
        y += vertDir;
        prison.setXY(x,y,(byte)'R');
      }  // If not then the rat will die.
      try {
        Thread.sleep(2000);   
      } catch(InterruptedException e) {
        throw new RuntimeException(e);
      }
    }
    System.out.println("Rat no." + myRatNo + 
      " can't move..dying..aarrgggh.");
  }
} ///:~
