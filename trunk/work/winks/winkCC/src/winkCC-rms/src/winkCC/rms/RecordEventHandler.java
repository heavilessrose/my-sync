/**
 * 
 */
package winkCC.rms;

import javax.microedition.rms.RecordListener;
import javax.microedition.rms.RecordStore;

/**
 * recordStore事件处理器, 监听recordStore内容的修改事件
 * 
 * @author WangYinghua
 * 
 */
public class RecordEventHandler implements RecordListener {

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * javax.microedition.rms.RecordListener#recordAdded(javax.microedition.
	 * rms.RecordStore, int)
	 */
	public void recordAdded(RecordStore rs, int id) {

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * javax.microedition.rms.RecordListener#recordChanged(javax.microedition
	 * .rms.RecordStore, int)
	 */
	public void recordChanged(RecordStore rs, int id) {

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * javax.microedition.rms.RecordListener#recordDeleted(javax.microedition
	 * .rms.RecordStore, int)
	 */
	public void recordDeleted(RecordStore rs, int id) {

	}

}
