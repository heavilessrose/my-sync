package luke.java.practice.net.socket.TCP.blockingIO.pooled;

import java.io.*;
import java.net.*;
import java.util.*;

/**
 * �������ӳص�socket������
 * 
 * @author WangYinghua
 * 
 */
public class PooledRemoteFileServer {
	protected int maxConnections;
	protected int listenPort;
	protected ServerSocket serverSocket;

	public PooledRemoteFileServer(int aListenPort, int maxConnections) {
		listenPort = aListenPort;
		this.maxConnections = maxConnections;
	}

	public static void main(String[] args) {
		PooledRemoteFileServer server = new PooledRemoteFileServer(3000, 3);
		server.setUpHandlers();
		server.acceptConnections();
	}

	/** ������ĿΪ maxConnections �� PooledConnectionHandler */
	public void setUpHandlers() {
		for (int i = 0; i < maxConnections; i++) {
			PooledConnectionHandler currentHandler = new PooledConnectionHandler();
			new Thread(currentHandler, "Handler " + i).start();
		}
	}

	/** �� ServerSocket ����������Ŀͻ������� */
	public void acceptConnections() {
		try {
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

	/** �ڿͻ�������һ�����������ʵ�ʴ����� */
	protected void handleConnection(Socket connectionToHandle) {
		PooledConnectionHandler.processRequest(connectionToHandle);
	}
}