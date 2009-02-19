package ora.ch2;

public class KVMProperties {

    private static final String[] properties = {
        "microedition.configuration",
        "microedition.encoding",
        "microedition.platform",
        "microedition.profiles"
    };
    
    public static void main(String[] args) {
        for (int i = 0; i < properties.length; i++) {
            System.out.println(properties[i] + " = " +
                                System.getProperty(properties[i]));
        }
    }
}
