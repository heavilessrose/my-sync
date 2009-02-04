package luke.java.practice.net.socket.TCPClilent;

import java.io.*;
import java.net.*;

public class RemoteFileClient {
	protected String hostIp;
	protected int hostPort;
	protected BufferedReader socketReader;
	protected PrintWriter socketWriter;

	public RemoteFileClient(String aHostIp, int aHostPort) {
		hostIp = aHostIp;
		hostPort = aHostPort;
	}

	public static void main(String[] args) {
		RemoteFileClient remoteFileClient = new RemoteFileClient("127.0.0.1",
				3000);
		remoteFileClient.setUpConnection();
		String fileContents = remoteFileClient.getFile("c:/ss.txt");
		remoteFileClient.tearDownConnection();

		// 打印从服务器拿到的文件内容
		System.out.println(fileContents);
	}

	public void setUpConnection() {
		try {
			Socket client = new Socket(hostIp, hostPort);

			socketReader = new BufferedReader(new InputStreamReader(client
					.getInputStream()));
			socketWriter = new PrintWriter(client.getOutputStream());

		} catch (UnknownHostException e) {
			System.out
					.println("Error setting up socket connection: unknown host at "
							+ hostIp + ":" + hostPort);
		} catch (IOException e) {
			System.out.println("Error setting up socket connection: " + e);
		}
	}

	public String getFile(String fileNameToGet) {
		StringBuffer fileLines = new StringBuffer();

		try {
			socketWriter.println(fileNameToGet);
			socketWriter.flush();

			String line = null;
			while ((line = socketReader.readLine()) != null)
				fileLines.append(line + "\n");
		} catch (IOException e) {
			System.out.println("Error reading from file: " + fileNameToGet);
		}

		return fileLines.toString();
	}

	public void tearDownConnection() {
		try {
			socketWriter.close();
			socketReader.close();
		} catch (IOException e) {
			System.out.println("Error tearing down socket connection: " + e);
		}
	}
}
