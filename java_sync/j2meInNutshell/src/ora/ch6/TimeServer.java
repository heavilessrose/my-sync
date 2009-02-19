package ora.ch6;

import java.io.IOException;
import java.util.Calendar;
import java.util.Date;
import javax.microedition.io.Connector;
import javax.microedition.io.Datagram;
import javax.microedition.io.DatagramConnection;

public class TimeServer {
    public static void main(String[] args) {
                
        try {
            Calendar cal = Calendar.getInstance();
            DatagramConnection receiver = 
                    (DatagramConnection)Connector.open("datagram://:13");
            byte[] buffer = new byte[256];
            Datagram dgram = receiver.newDatagram(buffer, buffer.length);
            for (;;) {
                dgram.setLength(buffer.length);
                
                // Wait for somebody to call...
                receiver.receive(dgram);
                
                // Get the time and store it in the buffer
                cal.setTime(new Date());
                String time = cal.toString();
                byte[] dataBytes = time.getBytes();
                System.arraycopy(dataBytes, 0, buffer, 0, dataBytes.length);
                
                // Send back the reply
                dgram.setLength(dataBytes.length);
                receiver.send(dgram);
            }
        } catch (IOException ex) {
            System.out.println("IOException: " + ex);
        }
    }
}