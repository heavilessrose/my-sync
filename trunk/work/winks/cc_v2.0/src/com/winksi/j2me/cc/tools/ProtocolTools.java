package com.winksi.j2me.cc.tools;

import java.util.Hashtable;
import java.util.Vector;

/**
 * 协议处理
 * @author jerry
 */
public class ProtocolTools {

	public static final int HEADER_LENGTH = 34;
	public static final int PROPERTY_LENGTH = 2;
	/** 总的数据长度(根据协议规定固定为4字节表示) */
	public static final int LENGTH = 4;
	
	/////////////////////////////////////////////server version
	public static final int PROPERTY_VERSION = 0x1;
	public static final int POSITION_VERSION_VALUE = 10;
	public static final byte VALUE_VERSION = 0x1;
	
	////////////////////////////////////////////way
	public static final int PROPERTY_WAY = 0x2;
	public static final int POSITION_WAY_VALUE = 17;
	public static final byte VALUE_WAY_HTTP = 0x1;
	
	///////////////////////////////////////////message type 
	public static final int PROPERTY_MESSAGETYPE = 0x3;
	/** 协议中messageTyep属性的value,开始的偏移 */
	public static final int POSITION_MESSAGETYPE_VALUE = 25;
	public static final int VALUE_MESSAGETYPE_SURE_PAY = 0x7ff2;
	public static final int VALUE_MESSAGETYPE_SURE_PAY_BACK = 0xfff2;
	public static final int VALUE_MESSAGETYPE_MODIFY_GREETING = 0x7ff3;
	public static final int VALUE_MESSAGETYPE_MODIFY_GREETING_BACK = 0xfff3;
	public static final int VALUE_MESSAGETYPE_NORMALCONNECTION = 0x7ff5;
	public static final int VALUE_MESSAGETYPE_NORMALCONNECTION_BACK = 0xfff5;
	/** 客户端到配置服务器:请求通讯服务器和短信网关地址消息 */
	public static final int VALUE_MESSAGETYPE_CONFIGSERVER = 0x7ff9;
	/** 配置服务器到客户端: 返回通信服务器和短信网关地址消息 */
	public static final int VALUE_MESSAGETYPE_CONFIGSERVER_BACK = 0xfff9;
	public static final int VALUE_MESSAGETYPE_UPLOADPIM_UPDATA = 0x7ffb;
	public static final int VALUE_MESSAGETYPE_UPLOADPIM_UPDATA_BACK = 0xfffb;
	public static final int VALUE_MESSAGETYPE_UPLOADPIM = 0x7ffc;
	public static final int VALUE_MESSAGETYPE_UPLOADPIM_BACK = 0xfffc;
	public static final int VALUE_MESSAGETYPE_DOWNLOAD_CC = 0x7ffa;
	public static final int VALUE_MESSAGETYPE_DOWNLOAD_CC_BACK = 0xfffa;
	public static final int VALUE_MESSAGETYPE_DIY = 0x7ff6;
	public static final int VALUE_MESSAGETYPE_DIY_BACK = 0xfff6;
	/** 出错时服务器的返回中使用的错误属性 */
	public static final int VALUE_MESSAGETYPE_ERROR = 0xffff;
	
	///////////////////////////////////sequence number
	public static final int PROPERTY_SEQUENCENUM = 0x4;
	public static final int POSITION_SEQUENCENUM_VALUE = 33;
	public static final short VALUE_SEQUENCENUM = 0x1;
	
	///////////////////////////////////error
	public static final int PROPERTY_ERROR = 0x0005;
	/** 客户端未知错误 */
	public static final int PROPERTY_ERROR_0001 = 0x0001;
	/** 协议解析错误 */
	public static final int PROPERTY_ERROR_1001 = 0x1001;
	/** 配置服务器内部错误 */
	public static final int PROPERTY_ERROR_1002 = 0x1002;
	/** 无有效的通信服务器地址 */
	public static final int PROPERTY_ERROR_1003 = 0x1003;
	/** 无有效的短信特服号码 */
	public static final int PROPERTY_ERROR_1004 = 0x1004;
	/** 通信服务器内部错误 */
	public static final int PROPERTY_ERROR_2001 = 0x2001;
	/** 客户没有定制信息 */
	public static final int PROPERTY_ERROR_4001 = 0x4001;
	/** 客户没有匹配当前时间的定制彩像 */
	public static final int PROPERTY_ERROR_4002 = 0x4002;
	/** 无适合此型号客户端的彩像 */
	public static final int PROPERTY_ERROR_4003 = 0x4003;
	/** 彩像文件未找到 */
	public static final int PROPERTY_ERROR_4004 = 0x4004;
	/** 用户被锁定 */
	public static final int PROPERTY_ERROR_4005 = 0x4005;
	/** 本机号码为空错误 */
	public static final int PROPERTY_ERROR_5001 = 0x5001;
	/** 对方号码为空错误 */
	public static final int PROPERTY_ERROR_5002 = 0x5002;
	/** CRC校验错误 */
	public static final int PROPERTY_ERROR_5003 = 0x5003;
	/** 上传文件错误 */
	public static final int PROPERTY_ERROR_5004 = 0x5004;
	/** 上传文件信息处理错误 */
	public static final int PROPERTY_ERROR_5005 = 0x5005;
	/** 用户未注册 */
	public static final int PROPERTY_ERROR_5006 = 0x5006;
	/** 用户超过最大上传彩像数 */
	public static final int PROPERTY_ERROR_5007 = 0x5007;
	/** 服务器超过有效期停止服务错误 */
	public static final int PROPERTY_ERROR_5008 = 0x5008;
	/** 用户上传问候语修改失败 */
	public static final int PROPERTY_ERROR_5009 = 0x5009;
	/** 超过用户最大定制业务数错误 */
	public static final int PROPERTY_ERROR_500A = 0x500a;
	/** 客户端电话本号码属性空错误 */
	public static final int PROPERTY_ERROR_500B = 0x500b;
	/** 上传属性值错误 */
	public static final int PROPERTY_ERROR_500C = 0x500c;
	/** 无最新版本软件 */
	public static final int PROPERTY_ERROR_500D = 0x500d;
	/** 用户不是收费用户 */
	public static final int PROPERTY_ERROR_500E = 0x500e;
	/** 用户被运营商关闭 */
	public static final int PROPERTY_ERROR_500F = 0x500f;
	
	//time stamp
	//    public static final int PROPERTY_TIMESTAMP = 0x6;
	// 协议中表示本机号码
	public static final int PROPERTY_PHONENUMBER = 0x8;
	//sms number平台短信特服号码
	public static final int PROPERTY_SMSNUMBER = 0x10;
	//sms port
	//    public static final int PROPERTY_SMSNUM = 0x29;
	//comunicate server
	//    public static final int PROPERTY_COMUSERVER = 0x11;
	//download cc stream
	public static final int PROPERTY_DATASTREAM = 0x12;
	//version info
	public static final int PROPERTY_VERSIOIN_INFO = 0x15;
	//software url
	public static final int PROPERTY_UPDATASOFTWARE_URL = 0x16;
	//sms port
	public static final int PROPERTY_SMSPORT = 0x1A;
	//custom
	public static final int PROPERTY_CUSTOM = 0x1D;
	public static final int PROPERTY_CUSTOM_SUPPORT = 0x1;
	public static final int PROPERTY_CUSTOM_UNSUPPORT = 0x2;
	//send sms short CYC
	public static final int PROPERTY_SHORTCYC = 0x18;
	//send sms long CYC
	public static final int PROPERTY_LONGCYC = 0x19;
	//time stamp
	public static final int PROPERTY_TIMESTAMP = 0x20;
	//client language
	public static final int PROPERTY_CLIENT_LANGUAGE = 0x21;
	public static final String[] VOLUE_CLIENT_LANGUAGE = { "ZH_CN", "EN", "ZH_TW", "ZH_SG", "ZH_HK", "JA" };
	//pay for
	public static final int PROPERTY_PAY = 0x23;
	//sure pay
	public static final int PROPERTY_SUREPAY = 0x28;
	//diy server back index
	public static final int PROPERTY_SERVERBACK_INDEX = 0x29;

	////////////////////////////////////////config of client
	public static final int PROPERTY_CLIENT_CONFIG = 0x8000;
	public static final int PROPERTY_CLIENT_CONFIG_TYPE = 0x1;
	//    public static final int PROPERTY_CLIENT_CONFIG_OS = 0x2;
	public static final int PROPERTY_CLIENT_CONFIG_SOFTWARETYPE = 0x2;
	public static final int PROPERTY_CLIENT_CONFIG_SOFTWARETYPE_LENGTH = 1;
	public static final byte[] PROPERTY_CLIENT_CONFIG_SOFTWARETYPE_JAVA = { 0x1 };
	public static final int PROPERTY_CLIENT_CONFIG_SCREENHEIGHT = 0x3;
	public static final int PROPERTY_CLIENT_CONFIG_SCREENWIDTH = 0x4;
	
	////////////////////////////////////////property of download cc
	public static final int PROPERTY_DOWNLOAD_CC = 0x8001;
	public static final int PROPERTY_DOWNLOAD_CC_ID = 0x1;
	public static final int PROPERTY_DOWNLOAD_CC_TYPEID = 0x2;
	public static final String[] PROPERTY_DOWNLOAD_CC_TYPEID_STRING = { "jpg", "png", "tiff", "spl", "swf", "gif" };
	public static final int PROPERTY_DOWNLOAD_CC_TYPE = 0x4;
	public static final int PROPERTY_DOWNLOAD_CC_TYPE_USER = 0x1;
	public static final int PROPERTY_DOWNLOAD_CC_TYPE_CMCC = 0x3;
	
	////////////////////////////////////////CC 
	public static final int PROPERTY_CC = 0x8002;
	public static final int PROPERTY_CC_FIEL = 0x1;
	public static final int PROPERTY_CC_TYPE = 0x2;
	public static final byte PROPERTY_CC_TYPE_FCC = 0x1;
	public static final byte PROPERTY_CC_TYPE_CMCCCC = 0x3;
	public static final int PROPERTY_CC_TOTAL = 0x3;
	public static final int PROPERTY_CC_COUNT = 0x4;
	
	////////////////////////////////////////the method of wake up APP
	public static final int PROPERTY_WAKEUP = 0x8005;
	public static final int PROPERTY_WAKEUP_SMS = 0x4;
	public static final int PROPERTY_WAKEUP_SMS_SUPPORT = 0x1;
	public static final int PROPERTY_WAKEUP_SMS_UNSUPPORT = 0x2;
	public static final int PROPERTY_WAKEUP_PUSH = 0x5;
	public static final int PROPERTY_WAKEUP_PUSH_SUPPORT = 0x1;
	public static final int PROPERTY_WAKEUP_PUSH_UNSUPPORT = 0x2;
	public static final int PROPERTY_WAKEUP_DEFALUT = 0x6;
	public static final int PROPERTY_WAKEUP_DEFALUT_SMS = 0x1;
	public static final int PROPERTY_WAKEUP_DEFALUT_PUSH = 0x2;
	public static final int PROPERTY_WAKEUP_TIME = 0xA;
	
	////////////////////////////////////////GREETING
	public static final int PROPERTY_GREETING = 0x8004;
	public static final int PROPERTY_GREETING_NUMBER = 0x1;
	public static final int PROPERTY_GREETING_MSG = 0x2;
	
	////////////////////////////////////////DIY
	public static final int PROPERTY_DIY = 0x8006;
	public static final int PROPERTY_DIY_NUMBER = 0x1;
	public static final int PROPERTY_DIY_MESSAGE = 0x2;
	public static final int PROPERTY_DIY_FILENAME = 0x3;
	public static final int PROPERTY_DIY_FILESIZE = 0x4;
	public static final int PROPERTY_DIY_FILE_TOTALBLOCK = 0x5;
	public static final int PROPERTY_DIY_FILE_INDEXBLOCK = 0x6;
	public static final int PROPERTY_DIY_FILE_BLOCKSIZE = 0x7;
	public static final int PROPERTY_DIY_FILETYPE = 0x8;
	public static final int PROPERTY_DIY_TOTAL_FILE = 0x9;
	public static final int PROPERTY_DIY_INDEX_FILE = 0xa;
	/** 用户个人状态属性:客户端向通信服务器请求修改主叫个人状态属性 */
	public static final int PROPERTY_DIY_FILEID = 0xb;
	//    public static final int PROPERTY_DIY_FILESTREAM=0xc;
	
	////////////////////////////////////////communicate server
	public static final int PROPERTY_COMMSERVER = 0xFFF8;
	public static final int PROPERTY_COMMSERVER_URL = 0x1;
	public static final int PROPERTY_COMMSERVER_PORT = 0x2;

	////////////////////////////////////////property of cmcc cc id
	public static final int PROPERTY_CMCC_CCID = 0xFFFA;
	public static final int PROPERTY_CMCC_CCID_NUMBER = 0x1;
	public static final int PROPERTY_CMCC_CCID_TYPE = 0x3;
	public static final int PROPERTY_CMCC_CCID_TYPE_ADD = 0x1;
	public static final int PROPERTY_CMCC_CCID_TYPE_DEL = 0x2;
	
	////////////////////////////////////////property of friend`s cc id
	public static final int PROPERTY_CCID = 0xFFFC;
	public static final int PROPERTY_CCID_NUMBER = 0x1;
	public static final int PROPERTY_CCID_TYPE = 0x2;
	public static final int PROPERTY_CCID_TYEP_ADD = 0x1;
	public static final int PROPERTY_CCID_TYEP_DEL = 0x2;
	public static final int PROPERTY_CCID_ID = 0x4;
	
	////////////////////////////////////////property of PIM
	public static final int PROPERTY_PIM = 0xfffd;
	public static final int PROPERTY_PIM_NUMBER = 0x1;
	public static final int PROPERTY_PIM_CONTROLTYPE = 0x2;
	public static final byte[] PROPERTY_PIM_CONTROLTYPE_ADD = { 0x1 };
	public static final byte[] PROPERTY_PIM_CONTROLTYPE_DEL = { 0x2 };
	
	////////////////////////////////////////property of special number
	public static final int PROPERTY_SPECIALNUMBER = 0xFFFE;
	public static final int PROPERTY_SPECIALNUMBER_ID = 0x1;
	public static final int PROPERTY_SPECIALNUMBER_TYPE = 0x2;
	public static final int PROPERTY_SPECIALNUMBER_VALUE = 0x3;

	/** 协议头 */
	private byte[] header = {//
	//
			0, 0, 0, 34,// 总length
			0, 1, 0, 0, 0, 1, 1,// version
			0, 2, 0, 0, 0, 1, 1,// way
			0, 3, 0, 0, 0, 2, 0, 0,// messageType
			0, 4, 0, 0, 0, 2, 0, 1 // sequenceNumber 
	};
	/** 从服务器得到的返回数据, 即要解析的数据 */
	private byte[] result = null;
	/** 要解析的数据的长度 */
	private int dataLen = 0;
	/** 解析后的数据 */
	private Hashtable resultHt;

	public ProtocolTools() {

	}

	/** 重置要发送的数据,为按协议组织 发送给服务器的数据做准备 */
	public void reset() {
		header = new byte[] { 0, 0, 0, 34, 0, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 0, 1, 1, 0, 3, 0, 0, 0, 2, 0, 0, 0, 4, 0,
				0, 0, 2, 0, 1 };
	}

	public void setHeaderLength(int length) {
		combine(header, StringTools.int2byte(length), 0);
	}

	public void setVersionValue(byte b) {
		header[POSITION_VERSION_VALUE] = b;
	}

	public void setWayValue(byte b) {
		header[POSITION_WAY_VALUE] = b;
	}

	public void setMessageTypeValue(short mt) {
		combine(header, StringTools.short2byte(mt), POSITION_MESSAGETYPE_VALUE - 1);
	}

	public void setSequenceNumberValue(short sn) {
		combine(header, StringTools.short2byte(sn), POSITION_SEQUENCENUM_VALUE - 1);
	}

	/**
	 * 　数据加到尾部
	 * 
	 * @param b　要添加的数据
	 */
	public void add(byte[] b) {
		byte[] bb = new byte[header.length + b.length];
		setHeaderLength(bb.length);
		bb = combine(bb, header, 0);
		bb = combine(bb, b, header.length);
		header = bb;
	}

	/** 
	 * 两个字节数组合并
	 * 
	 * @param sources 合并到的数组
	 * @param item 要合并的数组
	 * @param offset 偏移
	 * @return 合并后的数组
	 */
	public static byte[] combine(byte[] sources, byte[] item, int offset) {
		if (sources.length - offset < item.length) {// 如果大于source的长度
			return item;
		}
		System.arraycopy(item, 0, sources, offset, item.length);
		return sources;
	}

	public byte[] getHeader() {
		return header;
	}

	/** 设置要解析的从服务器得到的数据 */
	public void setResult(byte[] result) {
		this.result = result;
	}

	/** 解析从服务器得到的数据 */
	public void parse() {
		try {
			resultHt = new Hashtable();
			byte[] data = result;
			if (data == null) {
				return;
			}

			int currentlen = 0;

			// 小于4个字节, 数据不完整?
			if (data.length < 4) {
				return;
			}

			byte[] len = new byte[4];
			// 服务器返回数据前4个字节表示总数据长度
			for (int i = 0; i < ProtocolTools.LENGTH; i++) {
				len[i] = data[i];
			}
			// 前4个字节转为其表示的长度(int)
			dataLen = StringTools.bytes2int(len);

			// 数据是否完整
			if (dataLen != data.length) {
				return;
			}
			// 去掉表示总长度的4个字节
			data = StringTools.removeDataPart(data, len.length);

			do {
				if (data.length < 6) {// 数据不完整??
					break;
				}
				//                Hashtable ht = new Hashtable();
				// 解析出messageType属性
				byte[] mTmp = new byte[2];
				mTmp[0] = data[0];
				mTmp[1] = data[1];
				int messageType = StringTools.byteArr2Int(mTmp);
				
				// 去掉表示messageType属性的所有字节
				data = StringTools.removeDataPart(data, mTmp.length);
				
				// 解析出length属性
				byte[] vLen = new byte[4];
				vLen[0] = data[0];
				vLen[1] = data[1];
				vLen[2] = data[2];
				vLen[3] = data[3];
				int valueLen = StringTools.bytes2int(vLen); // 得到属性的值长度
				
				// 去掉表示length属性的所有字节
				data = StringTools.removeDataPart(data, vLen.length);
				
				// 值
				byte[] value = new byte[valueLen];
				for (int i = 0; i < valueLen; i++) {
					value[i] = data[i];
				}
				// 写入解析后的数据
				resultHt.put(new Integer(messageType), value);
				// 去掉表示协议属性值value的所有字节
				data = StringTools.removeDataPart(data, valueLen);
				
				// 读出一个属性之后所剩的数据长度
				currentlen = data.length;
			} while (currentlen > 0);
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	public static Hashtable parse(byte[] data) {
		Hashtable ht = new Hashtable();
		try {
			if (data == null) {
				return null;
			}

			int currentlen = 0;

			do {
				if (data.length < 6) {
					break;
				}
				//                Hashtable ht = new Hashtable();
				byte[] mt = new byte[2];
				mt[0] = data[0];
				mt[1] = data[1];
				int messagetype = StringTools.byteArr2Int(mt);
				data = StringTools.removeDataPart(data, mt.length);
				byte[] vlen = new byte[4];
				vlen[0] = data[0];
				vlen[1] = data[1];
				vlen[2] = data[2];
				vlen[3] = data[3];
				int voluelen = StringTools.bytes2int(vlen);
				data = StringTools.removeDataPart(data, vlen.length);
				byte[] volue = new byte[voluelen];
				for (int i = 0; i < voluelen; i++) {
					volue[i] = data[i];
				}
				ht.put(new Integer(messagetype), volue);
				data = StringTools.removeDataPart(data, voluelen);
				currentlen = data.length;
			} while (currentlen > 0);
		} catch (Exception ex) {
			ex.printStackTrace();
		}
		return ht;
	}

	public int getDataLen() {
		return this.dataLen;
	}

	public Hashtable getResultHt() {
		return this.resultHt;
	}
}
