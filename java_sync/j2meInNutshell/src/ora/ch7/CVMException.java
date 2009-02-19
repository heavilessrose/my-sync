package ora.ch7;


public class CVMException {
    
    public static void main(String[] args) {
        System.out.println("Sleep for 10 seconds...");
        try {
            Thread.sleep(10000);
        } catch (InterruptedException ex) {
        }
        System.out.println("...done");
        Object o = null;
        int hashcode = o.hashCode();    // Deliberate NullPointerException
    }
}
