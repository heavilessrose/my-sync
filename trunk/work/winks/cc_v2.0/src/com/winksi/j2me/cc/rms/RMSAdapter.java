package com.winksi.j2me.cc.rms;

/**
 * RMS封装
 */
import java.io.*;
import java.util.Hashtable;
import java.util.Vector;
import javax.microedition.rms.*;

import com.winksi.j2me.cc.common.CPProperty;
import java.util.Enumeration;

public class RMSAdapter {

	private int language = 0;
	private RecordStore rs1 = null;
	private RecordStore rs2 = null;
	/** 号码与图片对应关系recordStore */
	private RecordStore rs3 = null;
	private RecordStore rs4 = null;
	private RecordStore rs5 = null;
	private RecordStore rs6 = null;
	/** 保存激活时提供的本机号码 */
	private RecordStore rs7 = null;
	private RecordStore rs8 = null;
	private RecordStore rs9 = null;
	private RecordStore rs10 = null;
	private RecordStore rs11 = null;

	public RMSAdapter() {

	}

	public void setLanguage(int i) {
		this.language = i;
	}

	/**
	 * 给定的rms记录集是否存在
	 * 
	 * @param rmsNum
	 * @return
	 */
	public boolean isNotExist(int rmsNum) {
		boolean isnotexist = false;
		try {
			RecordStore record = RecordStore.openRecordStore(
					CPProperty.RECORDSTORE_NAME[rmsNum], false);
			record.closeRecordStore();
		} catch (Exception ex) {
			isnotexist = true;
		}
		return isnotexist;
	}

	/**
	 * 
	 * @param rmsNum
	 */
	public void openRms(int rmsNum) {
		try {
			switch (rmsNum) {
			case CPProperty.RECORDID_RUNTIMES:
				rs1 = RecordStore.openRecordStore(
						CPProperty.RECORDSTORE_NAME[rmsNum], true);
				break;
			case CPProperty.RECORDID_SPECIALNUM:
				rs2 = RecordStore.openRecordStore(
						CPProperty.RECORDSTORE_NAME[rmsNum], true);
				break;
			case CPProperty.RECORDID_NumAndImage:
				rs3 = RecordStore.openRecordStore(
						CPProperty.RECORDSTORE_NAME[rmsNum], true);
				break;
			case CPProperty.RECORDID_DOWNLOAD_CCID:
				rs4 = RecordStore.openRecordStore(
						CPProperty.RECORDSTORE_NAME[rmsNum], true);
				break;
			case CPProperty.RECORDID_DOWNLOAD_CMCCCCID:
				rs5 = RecordStore.openRecordStore(
						CPProperty.RECORDSTORE_NAME[rmsNum], true);
				break;
			case CPProperty.RECORDID_DOWNLOAD_IDS:
				rs6 = RecordStore.openRecordStore(
						CPProperty.RECORDSTORE_NAME[rmsNum], true);
				break;
			case CPProperty.RECORDID_MyPhoneNum:
				rs7 = RecordStore.openRecordStore(
						CPProperty.RECORDSTORE_NAME[rmsNum], true);
				break;
			case CPProperty.RECORDID_TIME_CONNECTION:
				rs8 = RecordStore.openRecordStore(
						CPProperty.RECORDSTORE_NAME[rmsNum], true);
				break;
			case CPProperty.RECORDID_NUMANDPIC:
				rs9 = RecordStore.openRecordStore(
						CPProperty.RECORDSTORE_NAME[rmsNum], true);
				break;
			case CPProperty.RECORDID_TIMESTAMP:
				rs10 = RecordStore.openRecordStore(
						CPProperty.RECORDSTORE_NAME[rmsNum], true);
				break;
			case CPProperty.RECORDID_ALERMTIME:
				rs11 = RecordStore.openRecordStore(
						CPProperty.RECORDSTORE_NAME[rmsNum], true);
				break;
			}

		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	/**
	 * 
	 * @param rmsNum
	 */
	public void closeRms(int rmsNum) {
		try {
			switch (rmsNum) {
			case CPProperty.RECORDID_RUNTIMES:
				rs1.closeRecordStore();
				break;
			case CPProperty.RECORDID_SPECIALNUM:
				rs2.closeRecordStore();
				break;
			case CPProperty.RECORDID_NumAndImage:
				rs3.closeRecordStore();
				break;
			case CPProperty.RECORDID_DOWNLOAD_CCID:
				rs4.closeRecordStore();
				break;
			case CPProperty.RECORDID_DOWNLOAD_CMCCCCID:
				rs5.closeRecordStore();
				break;
			case CPProperty.RECORDID_DOWNLOAD_IDS:
				rs6.closeRecordStore();
				break;
			case CPProperty.RECORDID_MyPhoneNum:
				rs7.closeRecordStore();
				break;
			case CPProperty.RECORDID_TIME_CONNECTION:
				rs8.closeRecordStore();
				break;
			case CPProperty.RECORDID_NUMANDPIC:
				rs9.closeRecordStore();
				break;
			case CPProperty.RECORDID_TIMESTAMP:
				rs10.closeRecordStore();
				break;
			case CPProperty.RECORDID_ALERMTIME:
				rs11.closeRecordStore();
				break;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void delRecordStore(int rmsNum) {
		try {
			RecordStore.deleteRecordStore(CPProperty.RECORDSTORE_NAME[rmsNum]);
			// System.out.println("delete ok");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void addRuntimes(int times) {
		try {
			ByteArrayOutputStream baos = null;
			DataOutputStream dos;
			baos = new ByteArrayOutputStream();
			dos = new DataOutputStream(baos);
			dos.writeInt(times);

			byte[] data = baos.toByteArray();
			if (data.length > 0) {
				if (rs1.getNumRecords() > 0) {
					rs1.setRecord(1, data, 0, data.length);
					// System.out.println("set is ok!");
				} else {
					rs1.addRecord(data, 0, data.length);
					// System.out.println("adds is ok!");
				}
			}
			// /System.out.println("adds is ok!");
			if (dos != null) {
				dos.close();
			}
			if (baos != null) {
				baos.close();
			}
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}

	public int getRunTimes() {
		int times = 0;
		try {
			byte[] data;
			ByteArrayInputStream bais = null;
			DataInputStream dis = null;
			if (rs1.getNumRecords() > 0) {
				data = rs1.getRecord(1);
				bais = new ByteArrayInputStream(data);
				dis = new DataInputStream(bais);
				times = dis.readInt();
			}

			if (dis != null) {
				dis.close();
			}
			if (bais != null) {
				bais.close();
			}
		} catch (IOException ex) {
			ex.printStackTrace();
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (InvalidRecordIDException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		}
		return times;
	}

	public void addSpecialnum(String id, String number) {
		try {
			ByteArrayOutputStream baos = null;
			DataOutputStream dos;
			baos = new ByteArrayOutputStream();
			dos = new DataOutputStream(baos);
			dos.writeUTF(id);
			dos.writeUTF(number);

			byte[] data = baos.toByteArray();
			if (data.length > 0) {
				rs2.addRecord(data, 0, data.length);
				// System.out.println("adds is ok!");
			}
			// /System.out.println("adds is ok!");
			if (dos != null) {
				dos.close();
			}
			if (baos != null) {
				baos.close();
			}
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}

	public Hashtable getSpecialnum() {
		Hashtable ht = new Hashtable();
		try {
			byte[] data;
			ByteArrayInputStream bais = null;
			DataInputStream dis = null;
			RecordEnumeration e = rs2.enumerateRecords(null, null, false);
			while (e.hasNextElement()) {
				data = e.nextRecord();
				bais = new ByteArrayInputStream(data);
				dis = new DataInputStream(bais);
				String id = dis.readUTF();
				String number = dis.readUTF();
				ht.put(id, number);
				System.out.println("get special num: " + id + " : " + number);
			}

			if (dis != null) {
				dis.close();
			}
			if (bais != null) {
				bais.close();
			}
		} catch (IOException ex) {
			ex.printStackTrace();
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (InvalidRecordIDException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		}
		return ht;
	}

	/** 添加一个联系人图片对应关系 ? */
	public void addNumAndPic(Hashtable ht) {
		try {
			ByteArrayOutputStream baos = null;
			DataOutputStream dos;
			Enumeration e = ht.keys();
			while (e.hasMoreElements()) {
				String number = (String) e.nextElement();
				String pic = (String) ht.get(number);
				baos = new ByteArrayOutputStream();
				dos = new DataOutputStream(baos);
				dos.writeUTF(number);
				dos.writeUTF(pic);

				byte[] data = baos.toByteArray();
				if (data.length > 0) {
					rs9.addRecord(data, 0, data.length);
					// System.out.println("adds is ok!");
				}
				if (dos != null) {
					dos.close();
				}
				if (baos != null) {
					baos.close();
				}
			}
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}

	public Hashtable getNumAndPic() {
		Hashtable ht = new Hashtable();
		try {
			byte[] data;
			ByteArrayInputStream bais = null;
			DataInputStream dis = null;
			RecordEnumeration e = rs9.enumerateRecords(null, null, false);
			while (e.hasNextElement()) {
				data = e.nextRecord();
				bais = new ByteArrayInputStream(data);
				dis = new DataInputStream(bais);
				String number = dis.readUTF();
				String pic = dis.readUTF();
				ht.put(number, pic);
			}

			if (dis != null) {
				dis.close();
			}
			if (bais != null) {
				bais.close();
			}
		} catch (IOException ex) {
			ex.printStackTrace();
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (InvalidRecordIDException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		}
		return ht;
	}

	// public void addContract(Vector[] vector) {
	// ByteArrayOutputStream baos = null;
	// DataOutputStream dos;
	// try {
	// for (int i = 0; i < vector.length; i++) {
	// Vector tempv = vector[i];
	// boolean hasnumber = false;
	// for (int t = 0; t < tempv.size(); t++) {
	// Vector[] v = (Vector[]) vector[i].elementAt(t);
	// baos = new ByteArrayOutputStream();
	// dos = new DataOutputStream(baos);
	// for (int j = 0; j < v.length; j++) {
	// Vector vv = (Vector) v[j];
	// int size = vv.size();
	//                        
	// String temp = vv.elementAt(0).toString();
	//
	// if (temp.equals(CPProperty.CONTRACT_TAG[language][CPProperty.NUMBER])) {
	// if (size > 1) {
	// hasnumber = true;
	// dos.writeInt(size + 1);
	// for (int k = 1; k < size; k++) {
	// String number = vv.elementAt(k).toString();
	// dos.writeUTF(number);
	// }
	// }
	// }
	//
	// if (hasnumber) {
	// if (temp.equals(CPProperty.CONTRACT_TAG[language][CPProperty.PICTURE])) {
	// String pic = "";
	// if (size > 1) {
	// pic = vv.elementAt(1).toString();
	// }
	// dos.writeUTF(pic);
	// hasnumber = false;
	// }
	// }
	// }
	//
	// byte[] data = baos.toByteArray();
	// if (data.length > 0) {
	// rs3.addRecord(data, 0, data.length);
	// }
	// // /System.out.println("adds is ok!");
	// if (dos != null) {
	// dos.close();
	// }
	// if (baos != null) {
	// baos.close();
	// }
	// }
	// }
	// } catch (Exception e) {
	// // e.printStackTrace();
	// }
	// }

	/** 保存联系人到RMS ? */
	public void saveContacts(Hashtable ht) {
		ByteArrayOutputStream baos = null;
		DataOutputStream dos;
		try {
			Enumeration e = ht.keys();
			while (e.hasMoreElements()) {
				baos = new ByteArrayOutputStream();
				dos = new DataOutputStream(baos);
				String number = (String) e.nextElement();
				dos.writeUTF(number);

				String pic = (String) ht.get(number);
				dos.writeUTF(pic);

				byte[] data = baos.toByteArray();
				if (data.length > 0) {
					rs3.addRecord(data, 0, data.length);
				}
				// /System.out.println("adds is ok!");
				if (dos != null) {
					dos.close();
				}
				if (baos != null) {
					baos.close();
				}
			}

		} catch (Exception e) {
			// e.printStackTrace();
		}
	}

	/** 得到已保存的号码与图片对应关系hashtable */
	public Hashtable getNumAndImgHashtable() {
		Hashtable ht = new Hashtable();
		byte[] data;
		ByteArrayInputStream bais = null;
		DataInputStream dis = null;

		try {
			RecordEnumeration e = rs3.enumerateRecords(null, null, false);
			while (e.hasNextElement()) {
				data = e.nextRecord();
				bais = new ByteArrayInputStream(data);
				dis = new DataInputStream(bais);
				String number = dis.readUTF();
				String pic = dis.readUTF();
				ht.put(number, pic);
			}

			if (dis != null) {
				dis.close();
			}
			if (bais != null) {
				bais.close();
			}
		} catch (Exception e) {
			// e.printStackTrace();
		}
		return ht;
	}

	// public Hashtable getContract() {
	// Hashtable ht = new Hashtable();
	// byte[] data;
	// ByteArrayInputStream bais = null;
	// DataInputStream dis = null;
	//
	// try {
	// int len = rs3.getNumRecords();
	// for (int i = 0; i < len; i++) {
	// Vector vector = new Vector();
	// String pic = "";
	// data = rs3.getRecord(i + 1);
	// bais = new ByteArrayInputStream(data);
	// dis = new DataInputStream(bais);
	// String key;
	// int lens = dis.readInt();
	// for (int j = 0; j < lens - 2; j++) {
	// key = dis.readUTF();
	// vector.addElement(key);
	// }
	// pic = dis.readUTF();
	//
	// int vlen = vector.size();
	//
	// if (vlen > 0) {
	// for (int vi = 0; vi < vlen; vi++) {
	// key = vector.elementAt(vi).toString();
	// ht.put(key, pic);
	// }
	// }
	// }
	//
	// if (dis != null) {
	// dis.close();
	// }
	// if (bais != null) {
	// bais.close();
	// }
	// } catch (Exception e) {
	// //e.printStackTrace();
	// }
	// return ht;
	// }

	public void addDownloadCCid(String number, String ccid, int type) {
		try {
			ByteArrayOutputStream baos = null;
			DataOutputStream dos;

			baos = new ByteArrayOutputStream();
			dos = new DataOutputStream(baos);

			dos.writeUTF(number);
			dos.writeUTF(ccid);
			dos.writeInt(type);

			byte[] data = baos.toByteArray();
			if (data.length > 0) {
				rs4.addRecord(data, 0, data.length);
				// System.out.println("adds is ok!");
			}
			// /System.out.println("adds is ok!");
			if (dos != null) {
				dos.close();
			}
			if (baos != null) {
				baos.close();
			}
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}

	public Vector getDownloadCCid() {
		Vector v = new Vector();
		try {
			byte[] data;
			ByteArrayInputStream bais = null;
			DataInputStream dis = null;
			RecordEnumeration e = rs4.enumerateRecords(null, null, false);
			while (e.hasNextElement()) {
				data = e.nextRecord();
				bais = new ByteArrayInputStream(data);
				dis = new DataInputStream(bais);
				String number = dis.readUTF();
				String id = dis.readUTF();
				int type = dis.readInt();

				v.addElement(number);
				v.addElement(id);
				v.addElement(Integer.toString(type));
			}

			if (dis != null) {
				dis.close();
			}
			if (bais != null) {
				bais.close();
			}
		} catch (IOException ex) {
			ex.printStackTrace();
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (InvalidRecordIDException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		}
		return v;
	}

	public void addDownloadCmccid(String id, int type) {
		try {
			ByteArrayOutputStream baos = null;
			DataOutputStream dos;

			baos = new ByteArrayOutputStream();
			dos = new DataOutputStream(baos);

			dos.writeUTF(id);
			dos.writeInt(type);

			byte[] data = baos.toByteArray();
			if (data.length > 0) {
				rs5.addRecord(data, 0, data.length);
				// System.out.println("adds is ok!");
			}
			// /System.out.println("adds is ok!");
			if (dos != null) {
				dos.close();
			}
			if (baos != null) {
				baos.close();
			}
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}

	public Hashtable getDownloadCmccid() {
		Hashtable ht = new Hashtable();
		try {
			byte[] data;
			ByteArrayInputStream bais = null;
			DataInputStream dis = null;
			RecordEnumeration e = rs5.enumerateRecords(null, null, false);
			while (e.hasNextElement()) {
				data = e.nextRecord();
				bais = new ByteArrayInputStream(data);
				dis = new DataInputStream(bais);
				String number = dis.readUTF();
				int type = dis.readInt();
				ht.put(number, Integer.toString(type));
			}

			if (dis != null) {
				dis.close();
			}
			if (bais != null) {
				bais.close();
			}
		} catch (IOException ex) {
			ex.printStackTrace();
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (InvalidRecordIDException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		}
		return ht;
	}

	public void addDownloadId(String id) {
		try {
			ByteArrayOutputStream baos = null;
			DataOutputStream dos;

			baos = new ByteArrayOutputStream();
			dos = new DataOutputStream(baos);

			dos.writeUTF(id);

			byte[] data = baos.toByteArray();
			if (data.length > 0) {
				rs6.addRecord(data, 0, data.length);
				// System.out.println("adds is ok!");
			}
			// /System.out.println("adds is ok!");
			if (dos != null) {
				dos.close();
			}
			if (baos != null) {
				baos.close();
			}
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}

	public Vector getDownloadIds() {
		Vector v = new Vector();
		try {
			byte[] data;
			ByteArrayInputStream bais = null;
			DataInputStream dis = null;
			RecordEnumeration e = rs6.enumerateRecords(null, null, false);
			while (e.hasNextElement()) {
				data = e.nextRecord();
				bais = new ByteArrayInputStream(data);
				dis = new DataInputStream(bais);
				String id = dis.readUTF();
				v.addElement(id);
			}

			if (dis != null) {
				dis.close();
			}
			if (bais != null) {
				bais.close();
			}
		} catch (IOException ex) {
			ex.printStackTrace();
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (InvalidRecordIDException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		}
		return v;
	}

	public void addPhoneNumber(String number) {
		try {
			ByteArrayOutputStream baos = null;
			DataOutputStream dos;

			baos = new ByteArrayOutputStream();
			dos = new DataOutputStream(baos);

			dos.writeUTF(number);

			byte[] data = baos.toByteArray();
			if (data.length > 0) {
				if (rs7.getNumRecords() > 0) {
					rs7.setRecord(1, data, 0, data.length);
				} else {
					rs7.addRecord(data, 0, data.length);
				}
			}
			if (dos != null) {
				dos.close();
			}
			if (baos != null) {
				baos.close();
			}
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}


	/** 从rms中得到激活时提供的本机号码 */
	public String getMyPhoneNum() {
		String number = "";
		try {
			byte[] data;
			ByteArrayInputStream bais = null;
			DataInputStream dis = null;
			if (rs7.getNumRecords() > 0) {
				data = rs7.getRecord(1);
				bais = new ByteArrayInputStream(data);
				dis = new DataInputStream(bais);
				number = dis.readUTF();
			}
			if (dis != null) {
				dis.close();
			}
			if (bais != null) {
				bais.close();
			}
		} catch (IOException ex) {
			ex.printStackTrace();
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (InvalidRecordIDException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		}
		return number;
	}

	public void addTiming(int time) {
		try {
			ByteArrayOutputStream baos = null;
			DataOutputStream dos;

			baos = new ByteArrayOutputStream();
			dos = new DataOutputStream(baos);

			dos.writeInt(time);

			byte[] data = baos.toByteArray();
			if (data.length > 0) {
				if (rs8.getNumRecords() > 0) {
					rs8.setRecord(1, data, 0, data.length);
				} else {
					rs8.addRecord(data, 0, data.length);
				}
			}
			if (dos != null) {
				dos.close();
			}
			if (baos != null) {
				baos.close();
			}
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}

	public int getTiming() {
		int time = -100;
		try {
			byte[] data;
			ByteArrayInputStream bais = null;
			DataInputStream dis = null;
			if (rs8.getNumRecords() > 0) {
				data = rs8.getRecord(1);
				bais = new ByteArrayInputStream(data);
				dis = new DataInputStream(bais);
				time = dis.readInt();
			}
			if (dis != null) {
				dis.close();
			}
			if (bais != null) {
				bais.close();
			}
		} catch (IOException ex) {
			ex.printStackTrace();
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (InvalidRecordIDException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		}
		return time;
	}

	public void addTimeStamp(String timestamp) {
		try {
			ByteArrayOutputStream baos = null;
			DataOutputStream dos;

			baos = new ByteArrayOutputStream();
			dos = new DataOutputStream(baos);

			dos.writeUTF(timestamp);

			byte[] data = baos.toByteArray();
			if (data.length > 0) {
				if (rs10.getNumRecords() > 0) {
					rs10.setRecord(1, data, 0, data.length);
				} else {
					rs10.addRecord(data, 0, data.length);
				}
			}
			if (dos != null) {
				dos.close();
			}
			if (baos != null) {
				baos.close();
			}
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}

	public String getTimeStamp() {
		String timestamp = "";
		try {
			byte[] data;
			ByteArrayInputStream bais = null;
			DataInputStream dis = null;
			if (rs10.getNumRecords() > 0) {
				data = rs10.getRecord(1);
				bais = new ByteArrayInputStream(data);
				dis = new DataInputStream(bais);
				timestamp = dis.readUTF();
			}
			if (dis != null) {
				dis.close();
			}
			if (bais != null) {
				bais.close();
			}
		} catch (IOException ex) {
			ex.printStackTrace();
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (InvalidRecordIDException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		}
		return timestamp;
	}

	public void addAlermTimes(long delay, long times) {
		try {
			ByteArrayOutputStream baos = null;
			DataOutputStream dos;
			baos = new ByteArrayOutputStream();
			dos = new DataOutputStream(baos);
			dos.writeLong(delay);
			dos.writeLong(times);

			byte[] data = baos.toByteArray();
			if (data.length > 0) {
				if (rs11.getNumRecords() > 0) {
					rs11.setRecord(1, data, 0, data.length);
					// System.out.println("set is ok!");
				} else {
					rs11.addRecord(data, 0, data.length);
					// System.out.println("adds is ok!");
				}
			}
			// /System.out.println("adds is ok!");
			if (dos != null) {
				dos.close();
			}
			if (baos != null) {
				baos.close();
			}
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}

	public long[] getAlermTimes() {
		long[] times = new long[2];
		times[0] = 0;
		times[1] = 0;
		try {
			byte[] data;
			ByteArrayInputStream bais = null;
			DataInputStream dis = null;
			if (rs11.getNumRecords() > 0) {
				data = rs11.getRecord(1);
				bais = new ByteArrayInputStream(data);
				dis = new DataInputStream(bais);
				times[0] = dis.readLong();
				times[1] = dis.readLong();
			}

			if (dis != null) {
				dis.close();
			}
			if (bais != null) {
				bais.close();
			}
		} catch (IOException ex) {
			ex.printStackTrace();
		} catch (RecordStoreNotOpenException ex) {
			ex.printStackTrace();
		} catch (InvalidRecordIDException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		}
		// System.out.println("return "+times);
		return times;
	}
}
