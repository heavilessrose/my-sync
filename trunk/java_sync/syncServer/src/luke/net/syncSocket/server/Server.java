package luke.net.syncSocket.server;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

	private static int PORT = 1025;
	private ServerSocket server;
	private Socket socket;
	private BufferedReader in;
	private PrintWriter out;

	public Server() {
		try {
			server = new ServerSocket(PORT);
			while (true) {
				System.out.println("server ready.. waiting connection.. ");
				socket = server.accept();
				System.out.println("got connection");
				// 字节流转换为字符流
				InputStreamReader inputReader = new InputStreamReader(socket
						.getInputStream());
				in = new BufferedReader(inputReader);

				out = new PrintWriter(socket.getOutputStream(), true);

				String line = in.readLine();
				System.out.println(line);
				out.println("your input: " + line);

				out.close();
				in.close();
				socket.close();
			}
		} catch (IOException e) {
			e.printStackTrace();
			try {
				server.close();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		}
	}

	public static void main(String[] args) {
		new Server();
	}

}
