package luke.java.practice.net.socket.TCP.blockingIO.echoApp;

/*************************************************************************
 *  Compilation:  javac EchoServer.java
 *  Execution:    java EchoServer port
 *  Dependencies: In.java [VERSION IN THIS DIRECTORY ONLY] Out.java
 *  
 *  Runs an echo server which listents for connections on port 4444,
 *  and echoes back whatever is sent to it.
 *
 *
 *  % java EchoServer 4444
 *
 *
 *  Limitations
 *  -----------
 *  The server is not multi-threaded, so at most one client can connect
 *  at a time.
 *
 *************************************************************************/

import java.io.IOException;
import java.net.Socket;
import java.net.ServerSocket;

public class EchoServer {

	public static void main(String[] args) throws Exception {

		// create socket
		int port = 8088;
		ServerSocket serverSocket = new ServerSocket(port);
		System.err.println("Started server on port " + port);

		// repeatedly wait for connections, and process
		while (true) {
			// a "blocking" call which waits until a connection is requested
			final Socket clientSocket = serverSocket.accept();
			System.err.println("Accepted connection from client");

			new Thread(){
				public void run(){
					// open up IO streams
					In in = new In(clientSocket);
					Out out = new Out(clientSocket);

					// waits for data and reads it in until connection dies
					// readLine() blocks until the server receives a new line from
					// client
					String s;
					while ((s = in.readLine()) != null) {
						out.println(s);
					}

					// close IO streams, then socket
					System.err.println("Closing connection with client");
					out.close();
					in.close();
					try {
						clientSocket.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}.start();
			
		}
	}
}
