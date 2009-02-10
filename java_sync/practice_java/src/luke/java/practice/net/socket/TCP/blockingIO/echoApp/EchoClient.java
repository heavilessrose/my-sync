package luke.java.practice.net.socket.TCP.blockingIO.echoApp;

/*************************************************************************
 *  Compilation:  javac EchoClient.java In.java Out.java
 *  Execution:    java EchoClient name host
 *
 *  Connects to host server on port 4444, sends text, and prints out
 *  whatever the server sends back.
 *
 *  
 *  % java EchoClient wayne localhost
 *  Connected to localhost on port 4444
 *  this is a test
 *  [wayne]: this is a test
 *  it works
 *  [wayne]: it works
 *  <Ctrl-d>                 
 *  Closing connection to localhost
 *
 *  Windows users: replace <Ctrl-d> with <Ctrl-z>
 *  
 *************************************************************************/

import java.net.Socket;

public class EchoClient {

	public static void main(String[] args) throws Exception {
		String screenName = "luke";
		String host = "127.0.0.1";
		int port = 4444;

		// connect to server and open up IO streams
		Socket socket = new Socket(host, port);
		In stdin = new In();
		In in = new In(socket);
		Out out = new Out(socket);
		System.err.println("Connected to " + host + " on port " + port);

		// read in a line from stdin, send to server, and print back reply
		String ss;
		while ((ss = stdin.readLine()) != null) {

			// read line of client
			String s = ss;

			// send over socket to server
			out.println("[" + screenName + "]: " + s);

			// get reply from server and print it out
			System.out.println(in.readLine());
		}

		// close IO streams, then socket
		System.err.println("Closing connection to " + host);
		out.close();
		in.close();
		socket.close();
	}
}
