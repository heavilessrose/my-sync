package winkCC.net.http;

import java.io.DataInputStream;
import java.io.IOException;

/**
 * 下载完数据后需要获得联网数据,读取,处理数据的类应实现此接口.<br>
 * 
 * @author WangYinghua
 * 
 */
public interface HttpDataListener {
	void handleData(DataInputStream input) throws IOException;
}