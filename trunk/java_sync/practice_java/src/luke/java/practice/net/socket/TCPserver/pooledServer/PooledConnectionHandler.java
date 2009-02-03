package luke.java.practice.net.socket.TCPserver.pooledServer;

import java.io.*;
import java.net.*;
import java.util.*;

public class PooledConnectionHandler implements Runnable {
	protected Socket connection;
	protected static List pool = new LinkedList();

	public PooledConnectionHandler() {
	}

	// ��ȡ���ӵ�����ʹ�����ǣ������������֮���������
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
	 * �Ѵ���������ӵ����У��������������ڵȴ��Ķ���ó��Ѿ���һЩ����
	 * 
	 * @param requestToHandle
	 */
	public static void processRequest(Socket requestToHandle) {
		// ȷ��û�����ܸ���ͬʱ�޸����ӳ�
		synchronized (pool) {
			pool.add(pool.size(), requestToHandle);
			// ֪ͨ�������ڵȴ��óص� Thread���������Ѿ�����
			pool.notifyAll();
		}
	}

	// �����ӳ��ϵȴ������ҳ���һ�����Ӿʹ�����.
	// �� run() ӵ�гصĻ�����ʱ��processRequest() ����ܹ������ӷŵ������أ����ǶԳ��ϵ� wait() �ĵ����ͷ�������
	// wait() ���ž����Լ�����֮ǰ�ٴξ�ȡ���������ʹ�óض��������ͬ��������Ի�ȡ������
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
			// �������һ����һ�����ӿ���ʹ�ã��͵��� handleConnection() ������
			handleConnection();
		}
	}
}
