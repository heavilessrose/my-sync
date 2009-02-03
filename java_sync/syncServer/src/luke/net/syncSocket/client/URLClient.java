package luke.net.syncSocket.client;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

/**
 * URLConnection 是一个秘密的套接字(没有显示的使用Socket)
 * 
 * @author WangYinghua
 * 
 */
public class URLClient {
	protected URLConnection connection;

	public static void main(String[] args) {
		URLClient client = new URLClient();
		// String google = client
		// .getDocumentAt("http://www.google.com/intl/en_ALL/images/logo.gif");
		// System.out.println(google);
		client
				.getBinaryFile("http://www.google.com/intl/en_ALL/images/logo.gif");
	}

	public String getDocumentAt(String urlString) {
		StringBuffer document = new StringBuffer();

		try {
			URL url = new URL(urlString);
			URLConnection conn = url.openConnection();
			BufferedReader reader = new BufferedReader(new InputStreamReader(
					conn.getInputStream()));

			String line = null;

			// readLine() 的调用将直至碰到一个从 InputStream
			// 传入的行终止符（例如换行符）时才阻塞。如果没碰到，它将继续等待。只有当连接被关闭时，它才会返回 null
			while ((line = reader.readLine()) != null)
				document.append(line + "\n");
			reader.close();
		} catch (MalformedURLException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return document.toString();
	}

	public void getBinaryFile(String urlString) {
		try {
			URL url = new URL(urlString);
			URLConnection conn = url.openConnection();
			BufferedInputStream in = new BufferedInputStream(conn
					.getInputStream());
			byte[] buff = new byte[1024];

			FileOutputStream downFile = new FileOutputStream("c:/google1.gif");
			int size = 0;
			int count = 0;

			System.out.println("start download.. ");
			long start = System.currentTimeMillis();
			while ((size = in.read(buff)) != -1) {
				downFile.write(buff, 0, size);
				count++;
			}
			System.out.println("download finished: " + count + ".. "
					+ (System.currentTimeMillis() - start));
			in.close();
		} catch (MalformedURLException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void uploadBinaryFile(String fileName) {

	}

}
