package com.winksi.j2me.cc.model.main;

import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.control.GUIController;
import com.winksi.j2me.cc.tools.ProtocolTools;
import com.winksi.j2me.cc.tools.StringTools;
import java.io.UnsupportedEncodingException;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Random;
import java.util.Vector;

/**
 * 用从服务器解析得到的数据进行初始化
 * @author jerry
 */
public class InitSystemModel {

	private GUIController controller;
	private int messageType = 0;
	private Vector commUrl = new Vector();//通讯服务器地址
	private String smsUrl = "";//短信特服号
	private String smsPort = "5001";//短信端口
	private int shortCyc = 0;//发送短信短周期
	private int longCyc = 0;//发送短信长周期
	private String timestamp = "";//时间戳
	//    private byte[] connectionmethoddata;//运营商提供连接方式（2层协议0x8005）
	private int pushMethod = 0;//push唤醒 true：1；false：2
	private int alertMethod = 0;//定时唤醒 true：1；false：2
	private int defaultMethod = 0;//默认方式
	private int connectCyc = -100;//定时通讯间隔时间 
	private int custom = 1;//是否支持手机定制0x001D
	private String payTip = "";//收费提示语
	private String updateUrl = "";// 软件更新的地址
	private byte[] fileData;//下传彩像文件数据流
	private String fileId;// 下传彩像文件id
	private String fileTypeId;//下传彩像文件类型id
	private int fileType;//下传彩像文件类型
	//    private byte[] specialnum;//特殊号码0xFFFE(3层协议)
	//    private Vector specialvector=new Vector();
	private Hashtable specialHt_add = new Hashtable();
	private Hashtable specialHt_del = new Hashtable();
	//    private byte[] ccid;//好友彩像id 0xFFFC(3层协议)
	private Vector ccIdVector = new Vector();
	private Hashtable ccIdHt_add = new Hashtable();
	private Hashtable ccIdHt_del = new Hashtable();
	//    private byte[] cmccccid;//运营商彩像id 0xFFFA(3层协议)
	private Hashtable cmccHt = new Hashtable();

	private int index_block = 0;

	private int errCode = 0;

	public InitSystemModel(GUIController controller) {
		this.controller = controller;
	}

	/** 用解析的数据初始化通讯服务器, 更新彩像等 */
	public void initComuServer(Hashtable ht) {
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_ERROR))) {//错误属性
			byte[] temp = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_ERROR));
			errCode = StringTools.byteArr2Int(temp);
			System.out.println("initComuServer: PROPERTY_ERROR, errCode: " + errCode);
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_TIMESTAMP))) {//时间戳
			try {
				byte[] temp = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_TIMESTAMP));
				timestamp = new String(temp, CPProperty.UTF8);
				//                System.out.println("timestamp : "+timestamp);
			} catch (UnsupportedEncodingException ex) {
				ex.printStackTrace();
			}
			System.out.println("initComuServer: PROPERTY_TIMESTAMP: " + timestamp);
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_PHONENUMBER))) {//本机电话号码
			System.out.println("initComuServer: PROPERTY_PHONENUMBER");

		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_DATASTREAM))) {//下载文件流
			System.out.println("initComuServer: PROPERTY_DATASTREAM");
			fileData = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_DATASTREAM));
			//            int size = filedata.length;
			//            System.out.println();
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_SMSNUMBER))) {//平台短信特服号码信息属性
			System.out.println("initComuServer: PROPERTY_SMSNUMBER");
			try {
				byte[] temp = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_SMSNUMBER));

				smsUrl = new String(temp, CPProperty.UTF8);
				System.out.println("smsUrl : " + smsUrl);
			} catch (UnsupportedEncodingException ex) {
				ex.printStackTrace();
			}
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_COMMSERVER))) {//配置服务器回复通信服务器地址信息属性
			System.out.println("initComuServer: PROPERTY_COMMSERVER");
			try {
				byte[] temp = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_COMMSERVER));
				Hashtable comht = ProtocolTools.parse(temp);
				Enumeration e = comht.elements();
				String url = null;
				String port = null;
				while (e.hasMoreElements()) {
					byte[] temp2 = (byte[]) e.nextElement();
					Hashtable tempht = ProtocolTools.parse(temp2);

					byte[] comurldata = (byte[]) tempht.get(new Integer(ProtocolTools.PROPERTY_COMMSERVER_URL));
					url = new String(comurldata, CPProperty.UTF8);
					byte[] comportdata = (byte[]) tempht.get(new Integer(ProtocolTools.PROPERTY_COMMSERVER_PORT));
					port = new String(comportdata, CPProperty.UTF8);
					commUrl.addElement(url + ":" + port);
				}
				System.out.println("commuUrl : " + url + ":" + port);
			} catch (UnsupportedEncodingException ex) {
				ex.printStackTrace();
			}
		}

		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_SMSPORT))) {//短信端口号
			System.out.println("initComuServer: PROPERTY_SMSPORT");
			try {
				byte[] temp = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_SMSPORT));
				smsPort = new String(temp, CPProperty.UTF8);
				System.out.println("smsPort: " + smsPort);
			} catch (UnsupportedEncodingException ex) {
				ex.printStackTrace();
			}
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_SHORTCYC))) {//SMS 短周期
			System.out.println("initComuServer: PROPERTY_SHORTCYC");
			try {
				byte[] temp = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_SHORTCYC));
				String shortcyc = new String(temp, CPProperty.UTF8);
				shortCyc = Integer.parseInt(shortcyc);
				System.out.println("shortCyc: " + shortCyc);
			} catch (UnsupportedEncodingException ex) {
				ex.printStackTrace();
			}
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_LONGCYC))) {//SMS 长周期
			System.out.println("initComuServer: PROPERTY_LONGCYC");
			try {
				byte[] temp = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_LONGCYC));
				String longcyc = new String(temp, CPProperty.UTF8);
				longCyc = Integer.parseInt(longcyc);
				System.out.println("longCyc: " + longCyc);
			} catch (UnsupportedEncodingException ex) {
				ex.printStackTrace();
			}
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_CUSTOM))) {//针对号码和群组进行定制属性
			System.out.println("initComuServer: PROPERTY_CUSTOM");
			byte[] temp = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_CUSTOM));
			//                String customstring = new String(temp, CPProperty.UTF8);
			custom = (int) temp[0];
			//            System.out.println("custom :"+custom);
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_UPDATASOFTWARE_URL))) {//软件升级地址
			System.out.println("initComuServer: PROPERTY_UPDATASOFTWARE_URL");
			try {
				byte[] temp = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_UPDATASOFTWARE_URL));
				updateUrl = new String(temp, CPProperty.UTF8);
				System.out.println("updateUrl: " + updateUrl);
			} catch (UnsupportedEncodingException ex) {
				ex.printStackTrace();
			}
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_PAY))) {//收费信息
			System.out.println("initComuServer: PROPERTY_PAY");
			try {
				byte[] temp = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_PAY));
				payTip = new String(temp, CPProperty.UTF8);
				System.out.println("payTip: " + payTip);
			} catch (UnsupportedEncodingException ex) {
				ex.printStackTrace();
			}
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_CLIENT_CONFIG))) {//客户端配置信息属性
			System.out.println("initComuServer: PROPERTY_CLIENT_CONFIG");

		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_DOWNLOAD_CC))) {//下传彩像文件基本属性
			System.out.println("initComuServer: PROPERTY_DOWNLOAD_CC");
			try {
				//下传彩像文件基本属性;
				byte[] temp = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_DOWNLOAD_CC));
				Hashtable dccht = ProtocolTools.parse(temp);
				fileId = new String((byte[]) dccht.get(new Integer(ProtocolTools.PROPERTY_DOWNLOAD_CC_ID)), CPProperty.UTF8);
				String filetid = new String((byte[]) dccht.get(new Integer(ProtocolTools.PROPERTY_DOWNLOAD_CC_TYPEID)),
						CPProperty.UTF8);
				fileTypeId = ProtocolTools.PROPERTY_DOWNLOAD_CC_TYPEID_STRING[Integer.parseInt(filetid) - 1];
				StringBuffer sb = new StringBuffer();
				sb.append(fileId);
				sb.append(".");
				sb.append(fileTypeId);
				fileId = sb.toString();
				byte[] filetypedata = (byte[]) dccht.get(new Integer(ProtocolTools.PROPERTY_DOWNLOAD_CC_TYPE));
				fileType = (int) filetypedata[0];
				//                System.out.println();
			} catch (UnsupportedEncodingException ex) {
				ex.printStackTrace();
			}
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_WAKEUP))) {//运营商提供终端连接方式属性
			System.out.println("initComuServer: PROPERTY_WAKEUP");
			try {//运营商提供终端连接方式属性
				byte[] connectionmethoddata = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_WAKEUP));
				Hashtable cmdht = ProtocolTools.parse(connectionmethoddata);
				byte[] smstemp = (byte[]) cmdht.get(new Integer(ProtocolTools.PROPERTY_WAKEUP_SMS));
				alertMethod = (int) smstemp[0];

				byte[] pushtemp = (byte[]) cmdht.get(new Integer(ProtocolTools.PROPERTY_WAKEUP_PUSH));
				pushMethod = (int) pushtemp[0];

				byte[] defaulttemp = (byte[]) cmdht.get(new Integer(ProtocolTools.PROPERTY_WAKEUP_DEFALUT));
				defaultMethod = (int) defaulttemp[0];
				if (cmdht.containsKey(new Integer(ProtocolTools.PROPERTY_WAKEUP_TIME))) {
					byte[] conntime = (byte[]) cmdht.get(new Integer(ProtocolTools.PROPERTY_WAKEUP_TIME));
					connectCyc = Integer.parseInt(new String(conntime, CPProperty.UTF8));
				}
			} catch (UnsupportedEncodingException ex) {
				ex.printStackTrace();
			}
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_CMCC_CCID))) {//运营商彩像id
			System.out.println("initComuServer: PROPERTY_CMCC_CCID");
			byte[] cmccccid = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_CMCC_CCID));
			Hashtable ctempht = ProtocolTools.parse(cmccccid);
			Enumeration e = ctempht.elements();
			while (e.hasMoreElements()) {
				try {
					byte[] temp = (byte[]) e.nextElement();
					Hashtable tempht = ProtocolTools.parse(temp);

					byte[] typetemp = (byte[]) tempht.get(new Integer(ProtocolTools.PROPERTY_CMCC_CCID_TYPE));
					int type = (int) typetemp[0];
					byte[] cmccidtemp = (byte[]) tempht.get(new Integer(ProtocolTools.PROPERTY_CMCC_CCID_NUMBER));
					String id = new String(cmccidtemp, CPProperty.UTF8);
					//                    System.out.println("cmccid : "+id);
					cmccHt.put(id, new Integer(type));
				} catch (UnsupportedEncodingException ex) {
					ex.printStackTrace();
				}
			}
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_CCID))) {//好友彩像id
			System.out.println("initComuServer: PROPERTY_CCID");
			byte[] ccid = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_CCID));
			ccIdVector = new Vector();
			ccIdHt_add = new Hashtable();
			ccIdHt_del = new Hashtable();
			Hashtable ctempht = ProtocolTools.parse(ccid);
			Enumeration e = ctempht.elements();
			while (e.hasMoreElements()) {
				try {
					byte[] temp = (byte[]) e.nextElement();
					Hashtable tempht = ProtocolTools.parse(temp);
					byte[] numtemp = (byte[]) tempht.get(new Integer(ProtocolTools.PROPERTY_CCID_NUMBER));
					String number = new String(numtemp, CPProperty.UTF8);
					ccIdVector.addElement(number);
					byte[] cidtemp = (byte[]) tempht.get(new Integer(ProtocolTools.PROPERTY_CCID_ID));
					String id = new String(cidtemp, CPProperty.UTF8);
					ccIdVector.addElement(id);
					byte[] typetemp = (byte[]) tempht.get(new Integer(ProtocolTools.PROPERTY_CCID_TYPE));
					int type = (int) typetemp[0];
					ccIdVector.addElement(Integer.toString(type));
					//                    System.out.println("friend : "+number+"ccid : "+id+" type : "+type);
					if (type == ProtocolTools.PROPERTY_CCID_TYEP_ADD) {
						ccIdHt_add.put(number, id);
					} else if (type == ProtocolTools.PROPERTY_CCID_TYEP_DEL) {
						ccIdHt_del.put(number, id);
					}
				} catch (UnsupportedEncodingException ex) {
					ex.printStackTrace();
				}
			}
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_PIM))) {//客户端电话本号码属性
			System.out.println("initComuServer: PROPERTY_PIM");

		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_SPECIALNUMBER))) {//紧急号码属性
			System.out.println("initComuServer: PROPERTY_SPECIALNUMBER");
			byte[] specialnum = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_SPECIALNUMBER));

			specialHt_add = new Hashtable();
			specialHt_del = new Hashtable();

			Hashtable snht = ProtocolTools.parse(specialnum);
			Enumeration e = snht.elements();

			while (e.hasMoreElements()) {
				try {
					byte[] temp = (byte[]) e.nextElement();
					Hashtable tempht = ProtocolTools.parse(temp);
					byte[] keytemp = (byte[]) tempht.get(new Integer(ProtocolTools.PROPERTY_SPECIALNUMBER_ID));
					//                    int keystring = StringTools.byte2Arr2Int(keytemp);
					String keystring = new String(keytemp, CPProperty.UTF8);
					byte[] type = (byte[]) tempht.get(new Integer(ProtocolTools.PROPERTY_SPECIALNUMBER_TYPE));
					int specialtype = (int) type[0];
					byte[] valuetemp = (byte[]) tempht.get(new Integer(ProtocolTools.PROPERTY_SPECIALNUMBER_VALUE));
					String valuestring = new String(valuetemp, CPProperty.UTF8);
					//                    System.out.println("key: " + keystring);
					//                    System.out.println("type: " + specialtype);
					//                    System.out.println("value: " + valuestring);
					if (specialtype == 1) {
						specialHt_add.put(keystring, valuestring);
					} else {
						specialHt_del.put(keystring, valuestring);
					}

				} catch (UnsupportedEncodingException ex) {
					ex.printStackTrace();
				}
			}
		}
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_SERVERBACK_INDEX))) {//DIY服务器返回上传成功块的ID
			System.out.println("initComuServer: PROPERTY_SERVERBACK_INDEX");
			try {
				//DIY服务器返回上传成功块的ID
				byte[] temp = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_SERVERBACK_INDEX));
				String index = new String(temp, CPProperty.UTF8);
				index_block = Integer.parseInt(index);
			} catch (UnsupportedEncodingException ex) {
				ex.printStackTrace();
			}
		}

		// 根据messageType进行相应处理
		if (ht.containsKey(new Integer(ProtocolTools.PROPERTY_MESSAGETYPE))) {//MESSAGE TYPE
			System.out.println("initComuServer: PROPERTY_MESSAGETYPE");
			byte[] temp = (byte[]) ht.get(new Integer(ProtocolTools.PROPERTY_MESSAGETYPE));
			//            int len = temp.length;
			messageType = StringTools.byteArr2Int(temp);
			//            System.out.println("messagetype : "+messagetype);
			if (messageType == ProtocolTools.VALUE_MESSAGETYPE_CONFIGSERVER_BACK) {
				// 从配置服务器拿到通信服务器地址及短信网关等相关配置信息
				controller.handleEvent(GUIController.EventID.EVENT_SENDSMS, null);
			} else if (messageType == ProtocolTools.VALUE_MESSAGETYPE_UPLOADPIM_BACK) {
				controller.backupNumAndImgPairs();
				controller.handleEvent(GUIController.EventID.EVENT_INIT_SPECIALNUM, null);
			} else if (messageType == ProtocolTools.VALUE_MESSAGETYPE_DOWNLOAD_CC_BACK) {
				controller.handleEvent(GUIController.EventID.EVENT_SAVE_CCFILE, null);
			} else if (messageType == ProtocolTools.VALUE_MESSAGETYPE_DIY_BACK) {
				controller.handleEvent(GUIController.EventID.EVENT_INIT_UPLOAD_DIY, null);
			} else if (messageType == ProtocolTools.VALUE_MESSAGETYPE_MODIFY_GREETING_BACK) {
				controller.handleEvent(GUIController.EventID.EVENT_INIT_UPLOAD_DIY, null);
			} else if (messageType == ProtocolTools.VALUE_MESSAGETYPE_UPLOADPIM_UPDATA_BACK) {
				controller.initUpdataNumber();
				controller.handleEvent(GUIController.EventID.EVENT_INIT_SPECIALNUM, null);
			} else if (messageType == ProtocolTools.VALUE_MESSAGETYPE_NORMALCONNECTION_BACK) {
				controller.handleEvent(GUIController.EventID.EVENT_INIT_SPECIALNUM, null);
			} else if (messageType == ProtocolTools.VALUE_MESSAGETYPE_SURE_PAY_BACK) {
				controller.handleEvent(GUIController.EventID.EVENT_SHOW_MAIN, null);
			} else if (messageType == ProtocolTools.VALUE_MESSAGETYPE_ERROR) {
				System.out.println("InitSystemModel:initComuServer:VALUE_MESSAGETYPE_ERROR");
				if (!payTip.equals("")) {
					controller.handleEvent(GUIController.EventID.EVENT_SHOW_PAY, null);
				} else {
					controller.handleEvent(GUIController.EventID.EVENT_ERROR, null);
				}
			}

		}
	}

	public String getComuUrl() {
		Random random = new Random();
		int index = Math.abs(random.nextInt()) % commUrl.size();
		index = 1;//debug
		return commUrl.elementAt(index).toString();
	}

	public String getSmsUrl() {
		return this.smsUrl;
	}

	public String getSmsPort() {
		return this.smsPort;
	}

	public int getShortCyc() {
		return this.shortCyc;
	}

	public int getLongCyc() {
		return this.longCyc;
	}

	public int getCustom() {
		return custom;
	}

	public String getTimeStamp() {
		return this.timestamp;
	}

	public int getConnectionTime() {
		return connectCyc;
	}

	public boolean isSupportAlertWakeup() {
		return (this.alertMethod == ProtocolTools.PROPERTY_WAKEUP_SMS_SUPPORT);
	}

	public boolean isSupportPushWakeup() {
		return (this.pushMethod == ProtocolTools.PROPERTY_WAKEUP_PUSH_SUPPORT);
	}

	public int getDefaultWakeup() {
		return this.defaultMethod;
	}

	public Hashtable getSpecialNum() {
		return this.specialHt_add;
	}

	public Hashtable getSpecialNumDel() {
		return this.specialHt_del;
	}

	public Hashtable getFriendNumAdd() {
		return this.ccIdHt_add;
	}

	public Hashtable getFriendNumDel() {
		return this.ccIdHt_del;
	}

	/** 得到彩像ID的vector */
	public Vector getCCIdVector() {
		return this.ccIdVector;
	}

	public Hashtable getCMCCht() {
		return this.cmccHt;
	}

	public String getDownloadCCID() {
		return this.fileId;
	}

	public String getDownloadCCTypeID() {
		return this.fileTypeId;
	}

	public int getDownloadCCType() {
		return this.fileType;
	}

	public byte[] getDownloadCCData() {
		return this.fileData;
	}

	public int getIndexBlock() {
		return this.index_block;
	}

	public int getErrorCode() {
		return this.errCode;
	}

	public String getSoftwareUrl() {
		return this.updateUrl;
	}

	public String getPayInfo() {
		return this.payTip;
	}
}
