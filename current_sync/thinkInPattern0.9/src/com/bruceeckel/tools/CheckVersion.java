//: com:bruceeckel:tools:CheckVersion.java
// {RunByHand}
package com.bruceeckel.tools;

public class CheckVersion {
  public static void main(String[] args) {
    String version = System.getProperty("java.version");
    char minor = version.charAt(2);
    char point = version.charAt(4);
    if(minor < '3' || point < '0')
      throw new RuntimeException("JDK 1.3.0 or higher " +
        "is required to run the examples in this book.");
    System.out.println("JDK version "+ version + " found");
  }
} ///:~
