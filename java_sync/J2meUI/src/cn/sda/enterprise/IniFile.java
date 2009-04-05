package cn.sda.enterprise;

import java.util.Enumeration;
import java.util.Hashtable;
import javax.microedition.rms.RecordEnumeration;
import javax.microedition.rms.RecordStore;

/**
 *用来记录配置信息，通过RecordStore来处理
 * 
 * @author Administrator
 */
public class IniFile {

	private RecordStore rs;
	private Hashtable IniConfigList;

	public IniFile() {
		internalSDAIniFile();
	}

	private void internalSDAIniFile() {
		IniConfigList = new Hashtable();
		IniConfigList.clear();
	}

	// RS打开关闭操作

	public static RecordStore OpenRS(String RsName) {
		return internalOpenRS(RsName);
	}

	private static RecordStore internalOpenRS(String RsName) {
		RecordStore rs = null;
		if (RsName.length() > 32) {
			return null;
		}
		try {
			rs = RecordStore.openRecordStore(RsName, true);
			return rs;
		} catch (Exception e) {
			return null;
		}
	}

	public boolean DeleteRS(String RsName) {
		return internalDeleteRS(RsName);
	}

	private boolean internalDeleteRS(String RsName) {
		if (RsName.length() > 32) {
			return false;
		}
		try {
			RecordStore.deleteRecordStore(RsName);
			return true;
		} catch (Exception e) {
			return false;
		}
	}

	// 打开文件

	public void CreateIniFile(String IniFileName) {
		internalCreateIniFile(IniFileName);
	}

	private void internalCreateIniFile(String IniFileName) {
		// 打开
		rs = OpenRS(IniFileName);
		ReadFromStream();
	}

	// 关闭文件

	public void CloseIniFile() {
		internalCloseIniFile();
	}

	private void internalCloseIniFile() {
		try {
			rs.closeRecordStore();
		} catch (Exception e) {
		}
	}

	// 文件配置信息读入Hashtable

	private void ReadFromStream() {
		try {
			RecordEnumeration rem = rs.enumerateRecords(null, null, false);
			for (int i = 0; i < rem.numRecords(); i++) {
				byte[] AllByte;
				AllByte = rem.nextRecord();
				String AllStr = new String(AllByte);
				int ipos = AllStr.indexOf("=");
				if (ipos > -1) {
					IniConfigList.put(AllStr.substring(0, ipos), AllStr
							.substring(ipos + 1));
				}
			}
		} catch (Exception e) {
			System.out.println(e.getMessage());
		}
	}

	// 写入配置项目
	public void WriteItem(String ItemName, String ItemValue) {
		internalWriteItem(ItemName, ItemValue);
	}

	private void internalWriteItem(String ItemName, String ItemValue) {
		if (IniConfigList.containsKey(ItemName)) {
			// 存在就修改
			IniConfigList.remove(ItemName);
			IniConfigList.put(ItemName, ItemValue);
		} else {
			IniConfigList.put(ItemName, ItemValue);
		}
	}

	// 删除配置

	public void DeleteItem(String ItemName) {
		internalDeleteItem(ItemName);
	}

	private void internalDeleteItem(String ItemName) {
		while (IniConfigList.containsKey(ItemName)) {
			IniConfigList.remove(ItemName);
		}
	}

	// 读取配置项目

	public String ReadItem(String ItemName) {
		return internalReadItem(ItemName);
	}

	private String internalReadItem(String ItemName) {
		if (IniConfigList.containsKey(ItemName)) {
			return (String) IniConfigList.get(ItemName);
		} else {
			return null;
		}
	}

	// 写入文件

	public void Flush() {
		internalFlush();
	}

	private void internalFlush() {
		// 先删除所有的，再保存
		int Recno = 0;
		RecordEnumeration rem = null;
		try {
			rem = rs.enumerateRecords(null, null, false);
			Recno = rem.numRecords();
		} catch (Exception e) {
		}
		if (rem != null) {
			for (int i = 0; i < Recno; i++) {
				try {
					int ID = rem.nextRecordId();
					rs.deleteRecord(ID);
				} catch (Exception e) {
				}
			}
		}
		// 重新保存
		String Key;
		String Value;
		Enumeration wenum = IniConfigList.keys();
		while (wenum.hasMoreElements()) {
			Key = (String) wenum.nextElement();
			Value = (String) IniConfigList.get(Key);
			String AllStr = Key + "=" + Value;
			byte[] itembyte = AllStr.getBytes();
			try {
				rs.addRecord(itembyte, 0, itembyte.length);
			} catch (Exception e) {
			}
		}
	}
}
