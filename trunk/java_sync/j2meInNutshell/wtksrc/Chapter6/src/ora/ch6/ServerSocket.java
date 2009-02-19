package ora.ch6;

import java.io.IOException;
import java.io.OutputStream;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.microedition.io.StreamConnectionNotifier;

public class ServerSocket {
    public static void main(String[] args) {
        try {
            StreamConnectionNotifier serverSocket =
                (StreamConnectionNotifier)Connector.open("socket://:8000");
            for (;;) {
                // Get the next connection
                final StreamConnection socket =
                    (StreamConnection)serverSocket.acceptAndOpen();

                // Handle the connection in a new thread
                Thread t = new Thread() {
                    public void run() {
                        OutputStream os = null;
                        try {
                                os = socket.openOutputStream();
                                // Communicate with client here....

                        } catch (IOException ex) {
                            System.out.println(ex);
                        } finally {
                            if (os != null) {
                                try {
                                    os.close();
                                    os = null;
                                } catch (IOException ex) {
                                }
                            }
                            try {
                                socket.close();
                            } catch (IOException ex) {
                            }
                        }
                    }
                };
                t.start();
            }
        } catch (IOException e) {
            System.out.println(e);
        }
    }
}