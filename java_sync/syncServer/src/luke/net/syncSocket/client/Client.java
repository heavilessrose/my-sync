package luke.net.syncSocket.client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class Client {
	private static int PORT = 1025;
	Socket socket;
	BufferedReader in;
	PrintWriter out;

	public Client() {
		try {
			socket = new Socket("127.0.0.1", PORT);

			BufferedReader line = new BufferedReader(new InputStreamReader(
					System.in));

			out = new PrintWriter(socket.getOutputStream(), true);
			out.println(line.readLine());

			in = new BufferedReader(new InputStreamReader(socket
					.getInputStream()));
			String serverReply = in.readLine();
			System.out.println(serverReply);

			line.close();
			out.close();
			in.close();
			socket.close();
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		new Client();
	}

}
