//: singleton:ConnectionPoolDemo.java
package singleton;
import junit.framework.*;

interface Connection {
  Object get();
  void set(Object x);
}

class ConnectionImplementation implements Connection {
  public Object get() { return null; }
  public void set(Object s) {}
}

class ConnectionPool { // A singleton
  private static PoolManager pool = new PoolManager();
  public static void addConnections(int number) {
    for(int i = 0; i < number; i++)
      pool.add(new ConnectionImplementation());
  }
  public static Connection getConnection() 
  throws PoolManager.EmptyPoolException {
    return (Connection)pool.get();
  }
  public static void releaseConnection(Connection c) {
    pool.release(c);
  }
}

public class ConnectionPoolDemo extends TestCase  {
  static {
    ConnectionPool.addConnections(5);
  }
  public void test() {
    Connection c = null;
    try {
      c = ConnectionPool.getConnection();
    } catch (PoolManager.EmptyPoolException e) {
      throw new RuntimeException(e);
    }
    c.set(new Object());
    c.get();
    ConnectionPool.releaseConnection(c);
  }
  public void test2() {
    Connection c = null;
    try {
      c = ConnectionPool.getConnection();
    } catch (PoolManager.EmptyPoolException e) {
      throw new RuntimeException(e);
    }
    c.set(new Object());
    c.get();
    ConnectionPool.releaseConnection(c);
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(ConnectionPoolDemo.class);
  }
} ///:~
