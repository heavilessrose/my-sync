package luke.games.engines.gLib;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;

import javax.microedition.lcdui.Image;
import javax.microedition.rms.RecordStore;

/**
 * @author Luke
 * 
 * TODO 工具类,包括加载文件/图片,RMS操作,日志
 */
public class GUtil {
	/** 是否打印日志 */
	static final public boolean DEBUG = true;

	/**
	 * 打印日志
	 * 
	 * @param message
	 */
	static final public void print(String message) {
		if (DEBUG) {
			System.out.println(message);
		}
	}

	final static public void print() {
		System.out.println();
	}

	// ---------------------------------------------------------------------------
	/**
	 * 载入文件
	 * 
	 * @param file
	 * @return
	 */
	public static byte[] loadFile(String file) {
		byte[] buff = null;
		InputStream is = file.getClass().getResourceAsStream(file);
		DataInputStream dis = new DataInputStream(is);
		try {
			buff = new byte[dis.available()];
			dis.read(buff);
		} catch (IOException e) {
			e.printStackTrace();
		}
		if (buff == null) {
			GUtil.print("Can not Load File '" + file);
		} else {
			GUtil.print("Loaded File '" + file + "' " + buff.length + " bytes");
		}
		return buff;
	}

	private static byte resNum; // 资源文件数
	private static byte[][] resData; // 资源文件数据
	private static String[] resName; // 资源文件名

	// 载入资源包,存放至byte[][]resData，文件名存至String[] resName
	public static void openResPak(String pakFile) {
		int[] resOffset = null;
		int[] resSize = null;
		GUtil.print("Load PakFile '" + pakFile);
		try {
			InputStream is = pakFile.getClass().getResourceAsStream(pakFile);
			DataInputStream dis = new DataInputStream(is);
			resNum = dis.readByte();
			resName = new String[resNum];
			resData = new byte[resNum][];
			resOffset = new int[resNum];
			resSize = new int[resNum];
			int namelen = 0;
			byte[] tmp = null;
			for (int i = 0; i < resNum; i++) {
				namelen = dis.readByte();
				tmp = new byte[namelen];
				dis.read(tmp);
				resName[i] = new String(tmp);
				resOffset[i] = dis.readShort();
				resSize[i] = dis.readShort();
			}
			dis.close();
		} catch (Exception ex) {
		}
		InputStream tis = pakFile.getClass().getResourceAsStream(pakFile);
		try {
			tis.skip(resOffset[0]);
			for (int k = 0; k < resNum; k++) {
				resData[k] = new byte[resSize[k]];
				tis.read(resData[k], 0, resSize[k]);
			}
			tis.close();
		} catch (Exception ex) {
		}
	}

	public static byte[] getResFile(String file) {
		int k = 0;
		for (; k < resNum; k++) {
			if (resName[k].equals(file)) {
				break;
			}
		}
		if (k < resNum)
			return resData[k];
		else
			return null;
	}

	public static void closeResPak() {
		GUtil.print("Close PakFile '" + ImgPakName + "' !");
		resNum = 0; // 资源文件数
		resData = null; // 资源文件数据
		resName = null; // 资源文件名
		ImgPakName = null;
	}

	// ---------------------------------------------------------------------------
	private static String ImgPakName;

	/**
	 * 载入打包的图片
	 * 
	 * @param pakName
	 * @param imageName
	 * @return
	 */
	final static public Image loadImage(String pakName, String imageName) {

		byte data[];

		if (ImgPakName == null
				|| (ImgPakName != null && !ImgPakName.equals(pakName))) {
			closeResPak();
			ImgPakName = pakName;
			openResPak(ImgPakName);
			data = getResFile(imageName);
		} else {
			data = getResFile(imageName);
		}
		if (data != null) {
			GUtil.print("Load Image '" + imageName + "' at '" + pakName);
		} else {
			GUtil
					.print("Can not Load Image '" + imageName + "' at '"
							+ pakName);
		}
		Image ret = Image.createImage(data, 0, data.length);
		if (ret == null) {
			GUtil.print("Can not Create Image '" + imageName + "' at '"
					+ pakName);
		}
		return ret;
	}

	/**
	 * 实时处理包，从中解出需要的文件
	 * 
	 * @param File
	 * @param SubFile
	 * @return byte[]
	 */
	final static public byte[] unpackFile(String File, String SubFile) {
		byte[] data = null;
		byte fileNum = 0;
		String[] fileName = null;
		int[] fileOff = null;
		int[] fileSize = null;

		try {
			InputStream is = File.getClass().getResourceAsStream(File);
			DataInputStream dis = new DataInputStream(is);
			fileNum = dis.readByte();
			fileName = new String[fileNum];
			fileOff = new int[fileNum];
			fileSize = new int[fileNum];
			int namelen = 0;
			byte[] tmp = null;
			for (int i = 0; i < fileNum; i++) {
				namelen = (int) (dis.readByte() & 0x0000ffff);
				tmp = new byte[namelen];
				dis.read(tmp);
				fileName[i] = new String(tmp);
				fileOff[i] = (int) (dis.readShort() & 0x0000ffff);
				fileSize[i] = (int) (dis.readShort() & 0x0000ffff);
			}
			dis.close();
		} catch (Exception ex) {
			GUtil.print("Error Load '" + SubFile + "' at '" + File);
			ex.printStackTrace();
		}

		for (int k = 0; k < fileNum; k++) {
			if (fileName[k].equals(SubFile)) {
				try {
					InputStream tis = File.getClass().getResourceAsStream(File);
					data = new byte[fileSize[k]];
					tis.skip(fileOff[k]);
					tis.read(data, 0, fileSize[k]);
					tis.close();
				} catch (Exception ex) {
					GUtil.print("Error Load '" + SubFile + "' at '" + File
							+ "' -_-!");
					ex.printStackTrace();
				}
				break;
			}
		}
		if (data == null) {
			GUtil.print("Can not Load File '" + SubFile + "' at '" + File
					+ "' -_-!");
		} else {
			GUtil.print("Loaded File '" + SubFile + "' " + data.length
					+ " bytes ^_^!");
		}
		return data;
	}

	final static public String[] getResFileNameList(String File) {
		byte[] data = null;
		byte fileNum = 0;
		String[] fileName = null;
		int[] fileOff = null;
		int[] fileSize = null;

		try {
			InputStream is = File.getClass().getResourceAsStream(File);
			DataInputStream dis = new DataInputStream(is);
			fileNum = dis.readByte();
			fileName = new String[fileNum];
			fileOff = new int[fileNum];
			fileSize = new int[fileNum];
			int namelen = 0;
			byte[] tmp = null;
			for (int i = 0; i < fileNum; i++) {
				namelen = (int) (dis.readByte() & 0x0000ffff);
				tmp = new byte[namelen];
				dis.read(tmp);
				fileName[i] = new String(tmp);
				fileOff[i] = (int) (dis.readShort() & 0x0000ffff);
				fileSize[i] = (int) (dis.readShort() & 0x0000ffff);
			}
			dis.close();
		} catch (Exception ex) {
			GUtil.print("Error Load  '" + File);
			ex.printStackTrace();
		}
		if (fileName == null) {
			GUtil.print("Can not get FileList at '" + File);
		} else {
			GUtil.print("FileList :");
			for (int i = 0; i < fileName.length; i++) {
				GUtil.print(" FileName:" + fileName[i] + " Offset:"
						+ fileOff[i] + " Size:" + fileSize[i]);
			}
		}
		return fileName;
	}

	final static public Image loadImage(String FilePath) {
		GUtil.print("Load Image '" + FilePath);
		Image Imagetemp = null;
		try {
			Imagetemp = Image.createImage(FilePath);
		} catch (java.io.IOException Err) {
			Imagetemp = null;
		}
		if (Imagetemp == null) {
			GUtil.print("Can not Load Image : " + FilePath);
		}
		return Imagetemp;
	}

	// ---------------------------------------------------------------------------
	/**
	 * 从数据库中得到数据
	 * 
	 * @return byte[]
	 */
	final static public byte[] getRMSSave() {
		RecordStore rs;
		byte[] SaveData = null;
		try {
			rs = RecordStore.openRecordStore("GameSave", true);
			try {
				SaveData = rs.getRecord(1);
			} catch (Exception Err) {
			}
			rs.closeRecordStore();
		} catch (Exception Err) {
			GUtil.print("Exception: " + Err.getMessage());
		}
		return SaveData;
	}

	/**
	 * 写入到数据库中
	 * 
	 * @param SaveData
	 */
	final static public void putRMSSave(byte[] SaveData) {
		RecordStore rs;
		try {
			rs = RecordStore.openRecordStore("GameSave", true);
			if (rs.getNumRecords() <= 0) {
				rs.addRecord(SaveData, 0, SaveData.length);
			}
			rs.setRecord(1, SaveData, 0, SaveData.length);
			rs.closeRecordStore();
		} catch (Exception Err) {
			GUtil.print("Exception: " + Err.getMessage());
		}
	}

	/**
	 * @param Bytes
	 * @param Start
	 * @return short
	 */
	final static public short readShort(byte[] Bytes, int Start) {
		short l, h;
		l = (short) ((Bytes[Start + 0] << 0) & 0x00ff);
		h = (short) ((Bytes[Start + 1] << 8) & 0xff00);
		return (short) (h | l);// java 高位在前
	}

	/**
	 * @param Bytes
	 * @param Start
	 * @param Val
	 */
	final static public void writeShort(byte[] Bytes, int Start, short Val) {
		byte h = (byte) (0x00ff & (Val >> 8));
		byte l = (byte) (0x00ff & (Val >> 0));
		Bytes[Start + 0] = l;
		Bytes[Start + 1] = h;
	}

	/**
	 * 得到RGB值
	 * 
	 * @param R
	 * @param G
	 * @param B
	 * @return color
	 */
	final static public int toColor(int R, int G, int B) {
		return (R + (G << 8) + (G << 16));
	}

	/**
	 * 得到unsigned字节
	 * 
	 * @param TheByte
	 * @return int
	 */
	final static public int toUnSignByte(int TheByte) {
		int NumBuffer = TheByte;
		if (NumBuffer < 0) {
			NumBuffer += 256;
		}
		return NumBuffer;
	}

	final static public byte[] getSave() {
		RecordStore rs;
		byte[] SaveData = null;
		try {
			rs = RecordStore.openRecordStore("GameSave", true);
			try {
				SaveData = rs.getRecord(1);
			} catch (Exception Err) {
			}
			rs.closeRecordStore();
		} catch (Exception Err) {
			System.out.println("Exception: " + Err.getMessage());
		}
		return SaveData;
	}

	final static public void putSave(byte[] SaveData) {
		RecordStore rs;
		try {
			rs = RecordStore.openRecordStore("GameSave", true);
			if (rs.getNumRecords() <= 0) {
				rs.addRecord(SaveData, 0, SaveData.length);
			}
			rs.setRecord(1, SaveData, 0, SaveData.length);
			rs.closeRecordStore();
		} catch (Exception Err) {
			System.out.println("Exception: " + Err.getMessage());
		}
	}

	/**
	 * 从byte数组中读取一个short
	 * 
	 * @param Bytes
	 * @param Start
	 * @return
	 */
	final static public short ReadShort(byte[] Bytes, int Start) {
		short b1, b2;
		b1 = Bytes[Start];
		b2 = Bytes[Start + 1];
		if (b1 < 0) {
			b1 += 256;
		}
		if (b2 < 0) {
			b2 += 256;
		}
		return (short) (b1 | (b2 << 8));
	}
}
