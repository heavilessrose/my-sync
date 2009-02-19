package ora.ch2;

public class BacktraceExample {

    public static void main(String[] args) {
        try {
            throw new Exception();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}
