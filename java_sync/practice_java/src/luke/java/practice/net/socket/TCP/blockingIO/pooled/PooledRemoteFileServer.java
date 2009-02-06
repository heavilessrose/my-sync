package luke.java.practice.net.socket.TCP.blockingIO.pooled;

import java.io.*;
import java.net.*;
import java.util.*;

/**
 * 带有连接池的socket服务器
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

	/** 创建数目为 maxConnections 的 PooledConnectionHandler */
	public void setUpHandlers() {
		for (int i = 0; i < maxConnections; i++) {
			PooledConnectionHandler currentHandler = new PooledConnectionHandler();
			new Thread(currentHandler, "Handler " + i).start();
		}
	}

	/** 在 ServerSocket 上侦听传入的客户机连接 */
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

	/** 在客户机连接一旦被建立后的实际处理方法 */
	protected void handleConnection(Socket connectionToHandle) {
		PooledConnectionHandler.processRequest(connectionToHandle);
	}
}