package ora.ch2;

import java.util.Calendar;
import java.util.Date;

public class KVMCalendar {

    public static void main(String[] args) {
        
        // Get today's day and month
        Calendar cal = Calendar.getInstance();
        Date date = new Date();
        cal.setTime(date);
        int month = cal.get(Calendar.MONTH);
        int day = cal.get(Calendar.DAY_OF_MONTH);    
        System.out.println("Day is " + day + ", month is " + month);
        
        // Find the date 20 days hence
        final long MILLIS_PER_DAY = 24 * 60 * 60 * 1000L;
        long offset = date.getTime();
        offset += 20 * MILLIS_PER_DAY;
        date.setTime(offset);
        cal.setTime(date);
        
        // Now get the adjusted date back
        month = cal.get(Calendar.MONTH);
        day = cal.get(Calendar.DAY_OF_MONTH); 
        System.out.println("In 20 days time, day will " + day + ", month will be " + month);
        System.out.println(cal);        
    }
}
