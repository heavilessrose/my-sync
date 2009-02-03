package luke.java.practice.net.socket.TCPserver.pooledServer;

import java.io.*;
import java.net.*;
import java.util.*;

public class PooledConnectionHandler implements Runnable {
	protected Socket connection;
	protected static List pool = new LinkedList();

	public PooledConnectionHandler() {
	}

	// 攫取连接的流，使用它们，并在任务完成之后清除它们
	public void handleConnection() {
		try {
			PrintWriter streamWriter = new PrintWriter(connection
					.getOutputStream());
			BufferedReader streamReader = new BufferedReader(
					new InputStreamReader(connection.getInputStream()));

			String fileToRead = streamReader.readLine();
			BufferedReader fileReader = new BufferedReader(new FileReader(
					fileToRead));

			String line = null;
			while ((line = fileReader.readLine()) != null)
				streamWriter.println(line);

			fileReader.close();
			streamWriter.close();
			streamReader.close();
		} catch (FileNotFoundException e) {
			System.out.println("Could not find requested file on the server.");
		} catch (IOException e) {
			System.out.println("Error handling a client: " + e);
		}
	}

	/**
	 * 把传入请求添加到池中，并告诉其它正在等待的对象该池已经有一些内容
	 * 
	 * @param requestToHandle
	 */
	public static void processRequest(Socket requestToHandle) {
		// 确保没有人能跟我同时修改连接池
		synchronized (pool) {
			pool.add(pool.size(), requestToHandle);
			// 通知其它正在等待该池的 Thread，池现在已经可用
			pool.notifyAll();
		}
	}

	// 在连接池上等待，并且池中一有连接就处理它.
	// 当 run() 拥有池的互斥锁时，processRequest() 如何能够把连接放到池中呢？答案是对池上的 wait() 的调用释放锁，而
	// wait() 接着就在自己返回之前再次攫取该锁。这就使得池对象的其它同步代码可以获取该锁。
	public void run() {
		while (true) {
			synchronized (pool) {
				while (pool.isEmpty()) {
					try {
						pool.wait();
					} catch (InterruptedException e) {
						return;
					}
				}
				connection = (Socket) pool.remove(0);
			}
			// 处理程序一旦有一个连接可以使用，就调用 handleConnection() 处理它
			handleConnection();
		}
	}
}
