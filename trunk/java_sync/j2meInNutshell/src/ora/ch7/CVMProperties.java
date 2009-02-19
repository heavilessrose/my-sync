package ora.ch7;

import java.util.Enumeration;
import java.util.Properties;

public class CVMProperties {

    
    public static void main(String[] args) {
        Enumeration enum = System.getProperties().propertyNames();
        while (enum.hasMoreElements()) {
            String name = (String)enum.nextElement();
            System.out.println(name + " = " + System.getProperty(name));
        }
    }
}
