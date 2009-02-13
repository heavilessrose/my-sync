//: flyweight:FlyWeightObjects.java

class ExternalizedData {
  static final int size = 5000000;
  static int[] id = new int[size];
  static int[] i = new int[size];
  static float[] f = new float[size];
  static {
    for(int i = 0; i < size; i++)
      id[i] = i;
  }
}

class FlyPoint {
  private FlyPoint() {}
  public static int getI(int obnum) {
    return ExternalizedData.i[obnum];
  }
  public static void setI(int obnum, int i) {
    ExternalizedData.i[obnum] = i;
  }
  public static float getF(int obnum) {
    return ExternalizedData.f[obnum];
  }
  public static void setF(int obnum, float f) {
    ExternalizedData.f[obnum] = f;
  }
  public static String str(int obnum) {
    return "id: " +
      ExternalizedData.id[obnum] +
      ", i = " +
      ExternalizedData.i[obnum] +
      ", f = " +
      ExternalizedData.f[obnum];
  }
}

public class FlyWeightObjects {
  public static void main(String[] args) {
    for(int i = 0; i < ExternalizedData.size; i++) {
      FlyPoint.setI(i, FlyPoint.getI(i) + 1);
      FlyPoint.setF(i, 47.0f);
    }
    System.out.println(
      FlyPoint.str(ExternalizedData.size -1));
  }
} ///:~
