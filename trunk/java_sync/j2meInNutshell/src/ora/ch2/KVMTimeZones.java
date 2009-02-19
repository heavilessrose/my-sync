package ora.ch2;

import java.util.TimeZone;

public class KVMTimeZones {
    
    public static void main(String[] args) {
        TimeZone defaultTZ = TimeZone.getDefault();
        String[] ids = TimeZone.getAvailableIDs();
        
        System.out.println("Available time zone ids: ");
        for (int i = 0; i < ids.length; i++) {
            System.out.print(ids[i] + " ");
        }
        System.out.println("\nDefault timezone is " + defaultTZ.getID() + 
                        ", GMT offset = " + defaultTZ.getRawOffset());
    }
}
