package winkCC.net.http;

import java.io.DataInputStream;
import java.io.IOException;

/**
 * ���������ݺ���Ҫ�����������,��ȡ,�������ݵ���Ӧʵ�ִ˽ӿ�.<br>
 * 
 * @author WangYinghua
 * 
 */
public interface HttpDataListener {
	void handleData(DataInputStream input) throws IOException;
}