package winkCC.net.http2;

import java.io.DataInputStream;
import java.io.IOException;

/**
 * ���������ݺ���Ҫ�����������,��ȡ,�������ݵ���Ӧʵ�ִ˽ӿ�.<br>
 * 
 * @author WangYinghua
 * 
 */
public interface DataHandler {
	void handleData(DataInputStream input) throws IOException;
}