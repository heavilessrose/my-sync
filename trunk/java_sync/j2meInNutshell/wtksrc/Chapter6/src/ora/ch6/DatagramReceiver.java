package ora.ch6;

import java.io.IOException;
import javax.microedition.io.Connector;
import javax.microedition.io.Datagram;
import javax.microedition.io.DatagramConnection;

public class DatagramReceiver {
    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("Usage: DatagramReceiver port");
            System.exit(1);
        }
        
        try {
            DatagramConnection receiver = 
                    (DatagramConnection)Connector.open("datagram://:" + args[0]);
            byte[] buffer = new byte[256];
            Datagram dgram = receiver.newDatagram(buffer, buffer.length);
            for (;;) {
                dgram.setLength(buffer.length);
                receiver.receive(dgram);
                int length = dgram.getLength();
                System.out.println("Datagram received. Length is " + length);

                // Show the content of the datagram.
                for (int i = 0; i < length; i++) {
                    System.out.print(buffer[i] + " ");
                }
                System.out.println();
                
                // Send it back...
                receiver.send(dgram);
            }
        } catch (IOException ex) {
            System.out.println("IOException: " + ex);
        }
    }
}