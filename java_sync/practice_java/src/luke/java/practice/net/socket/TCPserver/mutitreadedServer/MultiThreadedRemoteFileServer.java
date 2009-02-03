package luke.java.practice.net.socket.TCPserver.mutitreadedServer;

import java.io.*;
import java.net.*;

public class MultiThreadedRemoteFileServer {
	protected int listenPort = 3000;

	public static void main(String[] args) {
		MultiThreadedRemoteFileServer server = new MultiThreadedRemoteFileServer();
		server.acceptConnections();
	}

	public void acceptConnections() {
		try {
			// ServerSocket server = new ServerSocket(listenPort);

			// 设置backlog, 则服务器在处理第一个连接时, 还可以接受另外5个连接,
			// 这5个连接被放入队列等待处理(即限制了服务器可接受总共6个连接)
			ServerSocket server = new ServerSocket(listenPort, 5);
			Socket incomingConnection = null;
			while (true) {
				incomingConnection = server.accept();
				handleConnection(incomingConnection);
			}
		} catch (BindException e) {
			System.out.println("Unable to bind to port " + listenPort);
		} catch (IOException e) {
			System.out.println("Unable to instantiate a ServerSocket on port: "
					+ listenPort);
		}
	}

	// public void handleConnection(Socket incomingConnection) {
	// try {
	// OutputStream outputToSocket = incomingConnection.getOutputStream();
	// InputStream inputFromSocket = incomingConnection.getInputStream();
	//
	// BufferedReader streamReader = new BufferedReader(
	// new InputStreamReader(inputFromSocket));
	//
	// FileReader fileReader = new FileReader(new File(streamReader
	// .readLine()));
	//
	// BufferedReader bufferedFileReader = new BufferedReader(fileReader);
	// PrintWriter streamWriter = new PrintWriter(outputToSocket);
	// String line = null;
	// while ((line = bufferedFileReader.readLine()) != null) {
	// streamWriter.println(line);
	// }
	//
	// fileReader.close();
	// streamWriter.close();
	// streamReader.close();
	// } catch (Exception e) {
	// System.out.println("Error handling a client: " + e);
	// }
	// }

	public void handleConnection(Socket connectionToHandle) {
		new Thread(new ConnectionHandler(connectionToHandle)).start();
	}

}
