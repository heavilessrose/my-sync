//: projects:Maze.java
package projects;
import java.util.*;
import java.io.*;
import java.awt.*;

public class Maze extends Canvas {
  private Vector lines; // a line is a char array
  private int width = -1;
  private int height = -1;
  public static void main (String [] args) 
  throws IOException {
    if (args.length < 1) {
      System.out.println("Enter filename");
      System.exit(0);
    }
    Maze m = new Maze();
    m.load(args[0]);
    Frame f = new Frame();
    f.setSize(m.width*20, m.height*20);
    f.add(m);     
    Rat r = new Rat(m, 0, 0);
    f.setVisible(true);
  }
  public Maze() {
    lines = new Vector();
    setBackground(Color.lightGray);
  }
  synchronized public boolean 
  isEmptyXY(int x, int y) {
    if (x < 0) x += width;
    if (y < 0) y += height; 
    // Use mod arithmetic to bring rat in line:
    byte[] by = 
      (byte[])(lines.elementAt(y%height));  
    return by[x%width]==' ';
  }
  synchronized public void 
  setXY(int x, int y, byte newByte) {
    if (x < 0) x += width;
    if (y < 0) y += height; 
    byte[] by = 
      (byte[])(lines.elementAt(y%height));
    by[x%width] = newByte;
    repaint();
  }
  public void 
  load(String filename) throws IOException {
    String currentLine = null;
    BufferedReader br = new BufferedReader(
      new FileReader(filename));
    for(currentLine = br.readLine(); 
        currentLine != null;
        currentLine = br.readLine())  {
      lines.addElement(currentLine.getBytes());       
      if(width < 0 || 
         currentLine.getBytes().length > width)
        width = currentLine.getBytes().length;
    }
    height = lines.size();
    br.close();
  }       
  public void update(Graphics g) { paint(g); }
  public void paint (Graphics g) {
    int canvasHeight = this.getBounds().height;
    int canvasWidth  = this.getBounds().width;
    if (height < 1 || width < 1) 
      return; // nothing to do 
    int width = 
      ((byte[])(lines.elementAt(0))).length;
    for (int y = 0; y < lines.size(); y++) {
      byte[] b;
      b = (byte[])(lines.elementAt(y));
      for (int x = 0; x < width; x++) {
        switch(b[x]) {
          case ' ': // empty part of maze
            g.setColor(Color.lightGray);
            g.fillRect(
              x*(canvasWidth/width),
              y*(canvasHeight/height),
              canvasWidth/width,
              canvasHeight/height);
            break;
          case '*':     // a wall 
            g.setColor(Color.darkGray);
            g.fillRect(
              x*(canvasWidth/width),
              y*(canvasHeight/height),
              (canvasWidth/width)-1,
              (canvasHeight/height)-1);
            break;
          default:      // must be rat
            g.setColor(Color.red);
            g.fillOval(x*(canvasWidth/width),
            y*(canvasHeight/height),
            canvasWidth/width,
            canvasHeight/height);
            break;              
        }      
      }     
    }
  }
} ///:~
