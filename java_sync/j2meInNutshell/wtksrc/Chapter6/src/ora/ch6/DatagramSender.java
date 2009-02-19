package ora.ch6;

import java.io.IOException;
import javax.microedition.io.Connector;
import javax.microedition.io.Datagram;
import javax.microedition.io.DatagramConnection;

public class DatagramSender {
    public static void main(String[] args) {
        if (args.length != 2) {
            System.out.println("Usage: DatagramSender port length");
            System.exit(1);
        }
        
        try {
            DatagramConnection sender = 
                    (DatagramConnection)Connector.open("datagram://localhost:" + args[0]);
            int length = Integer.parseInt(args[1]);
            byte[] buffer = new byte[length];
            for (int i = 0; i < length; i++) {
                buffer[i] = (byte)('0' + (i % 10));
            }
            Datagram dgram = sender.newDatagram(buffer, buffer.length);
            sender.send(dgram);
            
            // Wait for the packet to be returned
            for (int i = 0; i < length; i++) {
                buffer[i] = (byte)0;
            }
            sender.receive(dgram);
            length = dgram.getLength();
            System.out.println("Received return packet, length is " + length);
            
            // Show the content of the datagram.
            for (int i = 0; i < length; i++) {
                System.out.print(buffer[i] + " ");
            }
            System.out.println();
        } catch (IOException ex) {
            System.out.println("IOException: " + ex);
        }
    }
}