/**
 * 
 */
package winkCC.rms.other;

import java.util.Hashtable;

/**
 * ¼ÇÂ¼
 * 
 * @author WangYinghua
 * 
 */
public class Record/* extends Hashtable*/ {
	private String name;
	private byte[] data;

	//	Record(String name, byte[] data) {
	//		this.name = name;
	//		this.data = data;
	//	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public byte[] getData() {
		return data;
	}

	public void setData(byte[] data) {
		this.data = data;
	}

}
