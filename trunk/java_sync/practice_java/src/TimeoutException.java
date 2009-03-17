public class TimeoutException extends RuntimeException {

   public TimeoutException(){ this("");}

   public TimeoutException(String s){ super(s);}

}