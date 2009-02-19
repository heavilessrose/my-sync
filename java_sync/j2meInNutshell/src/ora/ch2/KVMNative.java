package ora.ch2;

public class KVMNative {

    public native void printMessage(String message);
    
    public static void main(String[] args) {
        String msg = args.length > 0 ? args[0] : "";
        for (int i = 1; i < args.length; i++) {
            msg += " " + args[i];
        }        
        new KVMNative().printMessage(msg);
    }
}
