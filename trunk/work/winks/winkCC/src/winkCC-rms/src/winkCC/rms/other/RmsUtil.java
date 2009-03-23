/**
 * 
 */
package winkCC.rms.other;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Hashtable;

import javax.microedition.rms.InvalidRecordIDException;
import javax.microedition.rms.RecordEnumeration;
import javax.microedition.rms.RecordListener;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import javax.microedition.rms.RecordStoreFullException;
import javax.microedition.rms.RecordStoreNotFoundException;
import javax.microedition.rms.RecordStoreNotOpenException;

import winkCC.log.ILog;
import winkCC.log.LogFactory;

/**
 * @author WangYinghua
 * 
 */
public class RmsUtil {
	private static ILog log = LogFactory.getLog("RmsUtil");

	// private static RmsUtil instance = null;

	RmsUtil(RecordListener recordListener) {

	}

	//	public static RmsUtil getInstance() {
	//		if (instance == null) {
	//			instance = new RmsUtil();
	//		}
	//		return instance;
	//	}

	/**
	 * 打开一个RecordStore, 如果不存在自动创建
	 * 
	 * @param rs
	 * @param RsName
	 * @return 是否成功打开
	 */
	public RecordStore openRs(String RsName) {
		RecordStore rs = null;
		try {
			rs = RecordStore.openRecordStore(RsName, true);
		} catch (RecordStoreException e) {
			// 打开或创建失败
		}
		return rs;
	}

	/**
	 * 关闭一个RecordStore
	 * 
	 * @param rs
	 * @return 是否成功
	 */
	public boolean closeRs(RecordStore rs) {
		boolean success = false;
		try {
			rs.closeRecordStore();
			success = true;
		} catch (RecordStoreNotOpenException e) {
			success = false;
			e.printStackTrace();
		} catch (RecordStoreException e) {
			success = false;
			e.printStackTrace();
		}
		return success;
	}

	/**
	 * 删除一个recordStore
	 * 
	 * @param rsName
	 * @return
	 */
	public static boolean deleteRs(String rsName) {
		boolean success = false;
		try {
			RecordStore.deleteRecordStore(rsName);
			success = true;
		} catch (RecordStoreNotFoundException e) {
			success = false;
			e.printStackTrace();
		} catch (RecordStoreException e) {
			success = false;
			e.printStackTrace();
		}
		return success;
	}

	/**
	 * 得到Midlet套件中的所有的recordStore
	 * 
	 * @return
	 */
	public String[] getRsList() {

		return RecordStore.listRecordStores();
	}

	/**
	 * 准备要存储的记录
	 * 
	 * @param record
	 * @return
	 */
	private byte[] prepareRecordData(Record record) {
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		DataOutputStream dos = new DataOutputStream(baos);

		try {
			dos.writeUTF(((Record) record).getName());
			dos.write(((Record) record).getData());
			dos.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return baos.toByteArray();
	}

	/**
	 * 添加记录
	 * 
	 * @param rs
	 * @param name
	 * @param data
	 * @return recordId
	 */
	public int addRecord(RecordStore rs, Record record) {

		byte[] recordData = prepareRecordData(record);
		int recordId = 0;
		try {
			recordId = rs.addRecord(recordData, 0, recordData.length);
		} catch (RecordStoreNotOpenException e) {
			e.printStackTrace();
		} catch (RecordStoreFullException e) {
			e.printStackTrace();
		} catch (RecordStoreException e) {
			e.printStackTrace();
		}
		return recordId;
	}

	/**
	 * 根据record ID 取记录
	 * 
	 * @param rs
	 * @param recordId
	 * @return
	 */
	public Record getRecord(RecordStore rs, int recordId) {
		byte[] recordData = null;
		try {
			recordData = rs.getRecord(recordId);
		} catch (RecordStoreNotOpenException e) {
			e.printStackTrace();
		} catch (InvalidRecordIDException e) {
			e.printStackTrace();
		} catch (RecordStoreException e) {
			e.printStackTrace();
		}

		DataInputStream is = new DataInputStream(new ByteArrayInputStream(
				recordData));
		Record record = new Record();
		try {
			String name = is.readUTF();
			record.setName(name);
			byte[] data;
			try {
				data = new byte[rs.getRecordSize(recordId) - name.getBytes().length /*- 2*/];
				is.read(data);
				record.setData(data);
			} catch (RecordStoreNotOpenException e) {
				e.printStackTrace();
			} catch (InvalidRecordIDException e) {
				e.printStackTrace();
			} catch (RecordStoreException e) {
				e.printStackTrace();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

		return record;
	}

	/**
	 * 修改已有记录
	 * 
	 * @param rs
	 * @param recordId
	 * @param data
	 */
	public void modify(RecordStore rs, int recordId, byte[] data) {
		Record record = getRecord(rs, recordId);
		record.setData(data);

		byte[] recordData = prepareRecordData(record);
		try {
			rs.setRecord(recordId, recordData, 0, recordData.length);
		} catch (RecordStoreNotOpenException e) {
			log.debug("rs not opened", e);
		} catch (InvalidRecordIDException e) {
			log.debug("Invalid RecordID", e);
		} catch (RecordStoreFullException e) {
			log.debug("RecordStore Full", e);
		} catch (RecordStoreException e) {
			log.debug(e);
		}
	}

	/**
	 * 删除一条记录
	 * 
	 * @param rs
	 * @param recordId
	 */
	public void deleteRecord(RecordStore rs, int recordId) {
		try {
			rs.deleteRecord(recordId);
		} catch (RecordStoreNotOpenException e) {
			e.printStackTrace();
		} catch (InvalidRecordIDException e) {
			e.printStackTrace();
		} catch (RecordStoreException e) {
			e.printStackTrace();
		}
	}

	public void traverseRs(RecordStore rs) {
		try {
			// recordStore中的所有记录
			RecordEnumeration enum = rs.enumerateRecords(null, null, false);
			// 向前遍历
			while (enum.hasPreviousElement()) {
				int recordId = enum.previousRecordId();
				// rs.getRecord(id);
				Record record = getRecord(rs, recordId);
				// TODO 对此记录做工作
			}
			// 向后遍历
			while (enum.hasNextElement()) {
				int recordId = enum.nextRecordId();
				Record record = getRecord(rs, recordId);
				// TODO 对此记录做工作
			}
		} catch (RecordStoreNotOpenException e) {
			e.printStackTrace();
		} catch (InvalidRecordIDException e) {
			e.printStackTrace();
		} finally {

		}
	}

}
