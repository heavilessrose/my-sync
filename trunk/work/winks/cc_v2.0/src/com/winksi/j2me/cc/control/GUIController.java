package com.winksi.j2me.cc.control;

import java.util.Hashtable;
import java.util.Vector;
import java.io.*;

import javax.microedition.io.ConnectionNotFoundException;
import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.Displayable;

import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.midlet.CCMIDlet;
import com.winksi.j2me.cc.model.main.DiyDataModel;
import com.winksi.j2me.cc.model.main.FileModel;
import com.winksi.j2me.cc.model.main.InitSystemModel;
import com.winksi.j2me.cc.model.main.PIMModel;
import com.winksi.j2me.cc.rms.RMSAdapter;
import com.winksi.j2me.cc.model.net.GPRSConnection;
import com.winksi.j2me.cc.model.sms.SendSMSThread;
import com.winksi.j2me.cc.tools.*;
import com.winksi.j2me.cc.view.CCCanvas;
import com.winksi.j2me.cc.view.ErrorAlert;
import com.winksi.j2me.cc.view.ExitCanvas;
import com.winksi.j2me.cc.view.InputMyPhoneNumForm;
//import com.winksi.j2me.cc.view.MainEvent;
//import com.winksi.j2me.cc.view.ControlWindow;
import com.winksi.j2me.cc.view.GreetingTextBox;
import com.winksi.j2me.cc.view.HelpCanvas;
import com.winksi.j2me.cc.view.RegisterForm;
import com.winksi.j2me.cc.view.SettingCanvas;
import com.winksi.j2me.cc.view.ShowImgForm;
import com.winksi.j2me.cc.view.SplashCanvas;
import com.winksi.j2me.cc.view.WaitingCanvas;
import java.util.Date;
import java.util.Enumeration;
import java.util.Random;
import java.util.Stack;
import javax.microedition.io.PushRegistry;
import javax.microedition.lcdui.Image;

/** 控制器,几乎包含每个Model的处理 的方法 */
public class GUIController {

	private static Displayable currentDis;//当前显示
	private Stack eventStack;//存放diy过程事件
	private Random ran;
	private SendSMSThread sendSmsThread;
	private RMSAdapter rmsAdapter;
	private GPRSConnection gprsConnection;
	private InitSystemModel initSystemModel;
	private PIMModel pimModel;
	private FileModel fileModel;
	private CCMIDlet midlet;
	private SplashCanvas splashCanvas;
	private SettingCanvas settingCanvas;
	private WaitingCanvas waitingCanvas;
	private CCCanvas ccCanvas;
	//    private GreetingTextBox greetingTextBox;
	private ShowImgForm showImgForm;
	private HelpCanvas helpCanvas;
	private ExitCanvas exitCanvas;
	private ErrorAlert errorAlert;
	//    private MainCanvas mainCanvas;
	private ProtocolTools protocolTools;
	private InputMyPhoneNumForm inputMyPhoneNumForm; // 输入手机号界面
	private RegisterForm registerForm;
	private DiyDataModel diyDataModel;
	private ConstImage constImage;
	private int runtimes = 0;//记录程序运行的步骤
	private int language = 0;//语言0：简体中文，1：英文，2，繁体中文
	private int clientType;
	private String clientVersion;
	//    private String clientos = "java os";
	private String myPhoneNum = "";//手机号
	//    private String password = "";//密码
	private String smsNumber = "";//短信特服号
	private String smsPort = "";//短信端口
	private int shortCyc = 0;//发送SMS短周期
	private int longCyc = 0;//发送SMS长周期
	private int custom = 0;//是否支持手机定制
	private int timing = -100;//定时通讯间隔
	private Date alarm;
	private Hashtable shouldDownOperatorCcIdHt = new Hashtable();//存放待下载的运营商彩像id
	//    private Hashtable ccidht=new Hashtable();
	private Vector shouldDownFriendCcId = new Vector();//存放待下载好友彩像id
	private Vector allShouldDownCcId = new Vector();//存放全部待下载彩像id
	private boolean firstStart = false;
	private String downloadIdStr = "";
	/** 当前号码簿中新添加的号码 */
	private Vector updatedNumbers;
	//	private Vector[] contractVector;
	/** 当前号码簿中电话号码及图片对应关系 */
	private Hashtable numAndImageHt;
	//    private Hashtable uidht;
	/** rms中保存的电话号码及图片对应关系 */
	private Hashtable rmsHt;
	private Vector connectionHandlers;
	//    private Vector filevector;
	private String settingString = "";
	private String declareString = "";
	private String helpString = "";
	private String aboutString = "";
	private String updatasoftwareurl = "";
	private Hashtable errorCodeHt;
	private boolean firstDiy = true;
	private int total_block = 0;
	private int index_block = 0;
	private boolean isPushWakeup = false; //是否为无视图的处理
	private boolean isIniTok = false;
	private boolean isSendSms = false; // 是否在等待短信(激活短信发出后等待服务器返回)
	private int preEventId = -1;
	private boolean pimInited = false;

	public GUIController(CCMIDlet midlet) {
		this.midlet = midlet;
		this.language = this.midlet.getLanguage();
		String clientTypeTmp = this.midlet.getAppProperty("CELLPHONE_TYPE");
		clientType = Integer.parseInt(clientTypeTmp);
		//        System.out.println("type : "+clientType);
		this.clientVersion = this.midlet.getAppProperty("MIDlet-Version");
	}

	/** 遍历当前电话簿 */
	public void loadPim() {
		splashCanvas = new SplashCanvas(this);
		splashCanvas.setInfo(CPProperty.INIT_PIM[language]);
		setCurrentDis(splashCanvas);
		pimModel = new PIMModel(this, splashCanvas);
		pimModel.start();
	}

	/** 初始化model和view */
	public void init() {
		ran = new Random();
		alarm = new Date();
		helpString = readString("/res/help_" + language + ".txt");
		aboutString = readString("/res/about_" + language + ".txt");
		settingString = readString("/res/set_" + language + ".txt");
		declareString = readString("/res/declare_" + language + ".txt");
		String errorcodeString = readString("/res/errorcode_" + language + ".txt");
		errorCodeHt = StringTools.getItemHt(errorcodeString, "\n", ":");
		//        int test=0x1001;
		//        String ts=Integer.toString(test);

		eventStack = new Stack();

		connectionHandlers = new Vector();
		rmsAdapter = new RMSAdapter();
		rmsAdapter.setLanguage(getLanguage());
		getRuntimesFromRMS();

		updatedNumbers = new Vector();
		numAndImageHt = new Hashtable();
		//        uidht = new Hashtable();
		rmsHt = new Hashtable();
		//        filevector = new Vector();

		initSystemModel = new InitSystemModel(this);
		protocolTools = new ProtocolTools();

		diyDataModel = new DiyDataModel();
		constImage = new ConstImage();

		inputMyPhoneNumForm = new InputMyPhoneNumForm(this, CPProperty.COMMAND_REGISTER[language], CPProperty.INPUT_INFO[language]);
		registerForm = new RegisterForm(this, CPProperty.COMMAND_REGISTER[language], CPProperty.INPUT_INFO[language]);
		settingCanvas = new SettingCanvas(this);
		waitingCanvas = new WaitingCanvas(this);
		ccCanvas = new CCCanvas(this);
		//        greetingTextBox = new GreetingTextBox(this, CPProperty.GREETING_TITLE[language], CPProperty.MAX_INPUT);
		showImgForm = new ShowImgForm(this);
		helpCanvas = new HelpCanvas(this);
		exitCanvas = new ExitCanvas(this);
		errorAlert = new ErrorAlert(this);
		firstStart = true;

		/////////////////////////////// debug
//				                runtimes = 3;
		/////////////////////////////// debug

		if (runtimes == 0) { // 第一次运行程序
			initEvent();
		} else {
			loadPim();
		}
	}

	/** 初始化当前事件 */
	public void initEvent() {
		if (!isPushWakeup) {// 用户主动运行程序
			if (runtimes == 0) { // 第一次运行
				handleEvent(EventID.EVENT_SHOW_SETTING, null);
			} else if (runtimes == 1) {
				handleEvent(EventID.EVENT_SHOW_SURESET, null);
			} else if (runtimes == 2) {
				handleEvent(EventID.EVENT_SHOW_DECLARE, null);
			} else if (runtimes == 3) {
				handleEvent(EventID.EVENT_REGISTER, null);
			} else {
				handleEvent(EventID.EVENT_CONFIGSERVER, null);
			}
		} else {// push启动(没有需要用户参与的操作)
			if (runtimes > 3) {// 已激活
				handleEvent(EventID.EVENT_CONFIGSERVER, null);
			} else {// 未激活(不应出现这种情况:push启动只有在已经激活的情况下才会发生)
				runtimes = 3;
				saveRunTimes(); // 使下次启动时去输入手机号激活
				handleEvent(EventID.EVENT_EXIT, null);
			}
		}
	}

	/**
	 * 读取本地文件
	 * @param url 路径
	 * @return String（UTF-8）
	 */
	public String readString(String url) {
		String s = "";
		try {
			InputStream is = getClass().getResourceAsStream(url);
			byte[] b = new byte[is.available()];
			is.read(b);
			s = new String(b, CPProperty.UTF8);
			//            System.out.println("read: "+s);
		} catch (IOException ex) {
			ex.printStackTrace();
		}
		return s;
	}

	public void setAlertDis(Displayable disp) {
		if (!isPushWakeup) {
			midlet.setCurrent(disp);
		}
	}

	public void setCurrentDis(Displayable disp) {
		if (!isPushWakeup) {
			currentDis = disp;
			midlet.setCurrent(disp);
		} else {
			midlet.setCurrent(null);
		}
	}

	public void setCurrentDis(Alert alert, Displayable disp) {
		if (!isPushWakeup) {
			midlet.setCurrent(alert, disp);
		} else {
			midlet.setCurrent(alert, null);
		}
	}

	public void setBacklight(int time) {
		if (!isPushWakeup) {
			this.midlet.setBacklight(time);
		}
	}

	/** 关闭程序 */
	public void exit() {
		midlet.exit();
	}

	public static class EventID {

		private EventID() {
		}

		/** 离开程序, 并设置按时启动 */
		public static final int EVENT_EXIT = 1;
		public static final int EVENT_SPLASH = 2;
		public static final int EVENT_WAIT = 3;
		public static final int EVENT_BACK = 4;
		public static final int EVENT_HELP = 5;
		public static final int EVENT_ABOUT = 6;
		public static final int EVENT_ERROR = 7;
		public static final int EVENT_ALERT = 17;
		public static final int EVENT_CURRENT = 8;
		//        public static final int EVENT_MAIN = 8;
		public static final int EVENT_SHOW_MAIN = 9;
		/** 第一次运行, 设置提示 runtimes: 0 */
		public static final int EVENT_SHOW_SETTING = 10;
		/** 确认按步骤设置 runtimes: 1 */
		public static final int EVENT_SHOW_SURESET = 11;
		/** 隐私声明 runtimes: 2 */
		public static final int EVENT_SHOW_DECLARE = 12;
		public static final int EVENT_SHOW_WAITING = 13;
		public static final int EVENT_SHOW_GREETING = 14;
		public static final int EVENT_SHOW_REVIEW = 15;
		public static final int EVENT_SETTING_MENU = 50;
		public static final int EVENT_WAITING_MENU = 55;
		public static final int EVENT_MAIN_MENU = 60;
		public static final int EVENT_MAIN_DIY_IMG_MENU = 61;
		public static final int EVENT_MAIN_DIY_GREETING_MENU = 62;
		public static final int EVENT_MAIN_DIY_NAME_MENU = 63;
		public static final int EVENT_MAIN_DIY_NUM_MENU = 64;
		public static final int EVENT_DIY_MENU = 66;
		public static final int EVENT_MAIN_PAY_MENU = 65;
		//		比较电话薄是否有更新
		//        public static final int EVENT_INITPIM = 31;
		/** 上传更新电话号码 */
		public static final int EVENT_UPDATANUMBER = 32;
		//        public static final int EVENT_SMSPUSH = 33;
		/** 激活的短信push */
		public static final int EVENT_REGISTER_BACK = 33;//
		/** 输入手机号(未进入主界面的"激活") runtimes: 3 */
		public static final int EVENT_REGISTER = 34;
		/** 发送注册短信 */
		public static final int EVENT_SENDSMS = 35;
		/** 连接配置服务器 runtimes: 大于3 */
		public static final int EVENT_CONFIGSERVER = 36;
		/** 初始化上传电话本并获得特殊号码，唤醒方式及周期，是否支持定制 */
		public static final int EVENT_INIT_UPLOADPIM = 37;
		public static final int EVENT_INIT_MAIN = 38;
		/** 上传diy */
		public static final int EVENT_INIT_UPLOAD_DIY = 39;
		/** 定时通讯 */
		public static final int EVENT_INIT_TIMING = 40;
		/** 解析从服务器得到的数据 */
		public static final int EVENT_PARSE = 99;
		public static final int EVENT_INIT_SPECIALNUM = 100;
		public static final int EVENT_PUSHHANDLER = 101;
		public static final int EVENT_DIY = 102;
		public static final int EVENT_FILELIST = 104;
		public static final int EVENT_NAMELIST = 105;
		public static final int EVENT_NUMBERLIST = 106;
		/** 存储下载的图片文件? */
		public static final int EVENT_SAVE_CCFILE = 107;
		public static final int EVENT_UPDATASOFTWARE = 108;
		public static final int EVENT_REQUESTFORPAY = 109;
		public static final int EVENT_SHOW_PAY = 110;
		public static final int EVENT_SURE_EXIT = 200;
		/** 返回要求退出前的操作继续执行 */
		public static final int EVENT_EXIT_BACK = 201;
	}

	/**
	 * 根据事件类型启动对应动作
	 * @param eventID 事件
	 * @param args 传递对象
	 */
	public void handleEvent(int eventID, Object[] args) {
		switch (eventID) {
		case EventID.EVENT_EXIT: {
			try {
				try {
					if (timing > -1) {
						if (timing == 0) {
							long time = 60 * 60 * 1000 * 24;
							//                                System.out.println("save : "+time);
							//                                saveAlermTime(time);
							midlet.scheduleMIDlet(time); //注册按时启动
						} else {
							//                                long time = System.currentTimeMillis();
							timing = timing * 1000;
							//                                saveAlermTime(timing,time);
							midlet.scheduleMIDlet(timing); //注册按时启动
							//                                System.out.println("save : " +
							//                                        "delay " + timing +" time : "+time);
						}
					}
					//                        else{
					//                            long[] savetime=getAlermTime();
					//                            if(savetime[0]>0){
					//                                long time =  System.currentTimeMillis();
					//                                saveAlermTime(savetime[0],time);
					//                                System.out.println("register : "+time+savetime[0]);
					//                                midlet.scheduleMIDlet(savetime[0]+time); //注册按时启动
					//                            }
					//                        }ClassNotFoundException,            

				} catch (ClassNotFoundException cnfe) {
				} catch (ConnectionNotFoundException cnfe) {

				} catch (SecurityException se) {

				}
				System.gc();
				//                    Thread.yield();
				exit();
			} catch (Exception exc) {
				exc.printStackTrace();
			}
			break;
		}
		case EventID.EVENT_SURE_EXIT:
			if (preEventId > -1) {// 有前一事件存在,则要求用户确认是否关闭软件
				setCurrentDis(exitCanvas);
			} else { // 设置定时唤醒后,直接退出midlet
				handleEvent(EventID.EVENT_EXIT, null);
			}
			//                midlet.setCurrent(exitCanvas);
			break;
		case EventID.EVENT_EXIT_BACK: // 返回继续处理前一事件
			handleEvent(this.preEventId, null);
			break;
		case EventID.EVENT_BACK:
			popPreEvent();
			break;
		case EventID.EVENT_WAIT:
			break;
		case EventID.EVENT_ALERT:
			if (args[0] != null) {
				errorAlert.setString((String) args[0]);
				errorAlert.setTimeout(10000);
				setAlertDis(errorAlert);
			}
			break;
		case EventID.EVENT_ERROR:
			diyDataModel.init();
			total_block = diyDataModel.getTotalBlock();
			index_block = diyDataModel.getIndexBlock();
			int errorcode = initSystemModel.getErrorCode();
			System.out.println("GUIController:EVENT_ERROR: " + errorcode);
			String temp = Integer.toString(errorcode);
			String errorinfo = (String) errorCodeHt.get(temp);
			//                System.out.println("errorinfo : "+errorinfo);
			if (errorinfo == null || errorinfo.length() < 1) {
				errorinfo = (String) errorCodeHt.get(Integer.toString(0x1001));
				//                    System.out.println("errorinfo : "+errorinfo);
			}
			errorAlert.setString(errorinfo);

			if (errorcode == ProtocolTools.PROPERTY_ERROR_500F) {
				errorAlert.setTimeout(Alert.FOREVER);
				setAlertDis(errorAlert);
			} else {
				errorAlert.setTimeout(10000);
				setAlertDis(errorAlert);
			}

			if (runtimes < 4) {
				handleEvent(EventID.EVENT_REGISTER, null);
			}

			if (errorcode == ProtocolTools.PROPERTY_ERROR_4004) {
				if (!downloadIdStr.equals("")) {
					//                                            System.out.println("remove : " + downloadidString);
					allShouldDownCcId.removeElement(downloadIdStr);
				}

				removeAllDownloadID();

				if (allShouldDownCcId != null && !allShouldDownCcId.isEmpty() && allShouldDownCcId.size() > 0) {
					for (int i = 0; i < allShouldDownCcId.size(); i++) {
						//                                                System.out.println("save id: "+(String) downloadid.elementAt(i));
						saveDownloadId((String) allShouldDownCcId.elementAt(i));
					}
				}
				handleEvent(GUIController.EventID.EVENT_SHOW_MAIN, null);
			}
			break;
		case EventID.EVENT_CURRENT:
			//                handleEvent(this.preeventid,null);
			//                if(current instanceof CCCanvas){
			//                    eventStack.removeAllElements();
			//                }
			midlet.setCurrent(currentDis);
			//                setCurrent(current);
			break;
		case EventID.EVENT_HELP:
			helpCanvas.setString(CPProperty.COMMAND_HELP[language], helpString);
			setCurrentDis(helpCanvas);
			break;
		case EventID.EVENT_ABOUT:
			helpCanvas.setString(CPProperty.COMMAND_ABOUT[language], aboutString);
			setCurrentDis(helpCanvas);
			break;
		case EventID.EVENT_SPLASH:
			break;
		case EventID.EVENT_FILELIST:
			FileModel fm1 = new FileModel(this);
			//                fm1.setCurrDirName(CPProperty.DIYPATH_C);
			//                fm1.start();
			fm1.traverseDirectory(CPProperty.DIYPATH_C);
			//                fileModel.traverseDirectory(CPProperty.DIYPATH_C);
			Vector filevectorc = fm1.getList();

			FileModel fm2 = new FileModel(this);
			//                fm2.setCurrDirName(CPProperty.DIYPATH_E);
			//                fm2.start();
			fm2.traverseDirectory(CPProperty.DIYPATH_E);
			//                fileModel1.traverseDirectory(CPProperty.DIYPATH_E);
			Vector filevectore = fm2.getList();

			int size = filevectorc.size();
			for (int i = 0; i < size; i++) {
				filevectore.addElement(filevectorc.elementAt(i));
			}

			diyDataModel.init();

			ccCanvas.setFileList(filevectore);
			ccCanvas.setEvent(CCCanvas.DIY_IMG);
			setCurrentDis(ccCanvas);
			break;
		case EventID.EVENT_NAMELIST:
			ccCanvas.setNameList(pimModel.getFullHashtable());
			ccCanvas.setEvent(CCCanvas.DIY_NAME);
			setCurrentDis(ccCanvas);
			break;
		case EventID.EVENT_NUMBERLIST:
			ccCanvas.setNumberList();
			ccCanvas.setEvent(CCCanvas.DIY_NUM);
			setCurrentDis(ccCanvas);
			break;
		case EventID.EVENT_PUSHHANDLER:
			connectionHandlers.addElement(args[0]);
			break;
		case EventID.EVENT_PARSE: // 解析
			byte[] data = (byte[]) args[0];
			protocolTools.setResult(data);
			protocolTools.parse();
			//                    int len = httpTools.getDataLen();
			Hashtable resultParsedHt = protocolTools.getResultHt();
			// 用解析得到的信息初始化
			System.out.println("GUIController:handleEvent(EVENT_PARSE): use the parsed data initComuServer");
			initSystemModel.initComuServer(resultParsedHt);
			break;
		case EventID.EVENT_CONFIGSERVER:
			if (waitingCanvas == null) {
				waitingCanvas = new WaitingCanvas(this);
			}
			if (runtimes < 4) {// 未完成激活,即第一次运行激活.
				// 从输入框传回用户手动输入的本机电话号码
				myPhoneNum = (String) args[0];
			} else {// 已完成激活
				if (args == null) {// 不是激活界面,从RMS取本机电话号码
					rmsAdapter.openRms(CPProperty.RECORDID_MyPhoneNum);
					myPhoneNum = rmsAdapter.getMyPhoneNum();

					rmsAdapter.closeRms(CPProperty.RECORDID_MyPhoneNum);
				} else {// 
					myPhoneNum = (String) args[0];
				}
			}
			System.out.println("EVENT_CONFIGSERVER: phonenum : " + myPhoneNum);
			// 组织协议, 与配置服务器通讯
			combineCfgBody(myPhoneNum);
			// 联网等待界面
			waitingCanvas.setInfo(CPProperty.WAIT_INFO[language]);
			setCurrentDis(waitingCanvas);
			break;
		case EventID.EVENT_REGISTER:
			//                this.preeventid=eventID;
			if (runtimes < 3) {// 如果还没有激活
				runtimes++;
				saveRunTimes();
			} else {// 步骤保证在 输入手机号即3
				runtimes = 3;
				saveRunTimes();
			}

			if (isSendSms) {
				isSendSms = false;
				System.out.println("unregisterPush : " + CPProperty.SMSURL + smsPort);
				// 注销短信Push监控
				UnRegisterPush(CPProperty.SMSURL + smsPort);
				//                    System.out.println("exit sms : ");
				waitingCanvas.exitSMS();
			}
			setCurrentDis(inputMyPhoneNumForm); // 切换到输入手机号界面
			break;
		case EventID.EVENT_REGISTER_BACK:// 主菜单中
			this.preEventId = eventID;
			if (runtimes > 3) {
				if (isSendSms) {
					isSendSms = false;
				}
				runtimes = 3;
				saveRunTimes();
			}
			setCurrentDis(registerForm);
			break;
		case EventID.EVENT_SENDSMS:
			// 发送注册短信
			//                this.preeventid=eventID;
			shortCyc = initSystemModel.getShortCyc();

			longCyc = initSystemModel.getLongCyc();
			//                ccCanvas.start();
			if (runtimes < 4) { // 未激活
				if (!isSendSms) {
					StringBuffer sb = new StringBuffer();
					sb.append("PHONE:");
					sb.append(myPhoneNum);
					sb.append(";");
					sb.append("CLIENTTYPE:0");

					waitingCanvas.setInfo(CPProperty.SENDSMS_INFO[language]);
					smsNumber = initSystemModel.getSmsUrl();
					smsPort = initSystemModel.getSmsPort();

					//                    System.out.println("smsNumber : "+smsNumber);
					// 发送激活短信
					sendSms(sb.toString());

					// 等待服务器的短信返回
					waitingCanvas.setMonitorSMSPort(smsPort);
					registerPush(CPProperty.SMSURL + smsPort);
					//                        long times = System.currentTimeMillis();
					//                        waitingCanvas.setOldTime(times);
					isSendSms = true;
				}
				waitingCanvas.hideMainMenu();
				setCurrentDis(waitingCanvas);
				//                    handleEvent(GUIController.EventID.EVENT_INIT_UPLOADPIM, null);//debug
			} else {// 已激活
				// 注册短信Push
				registerPush(CPProperty.SMSURL + smsPort);
				//                    getTiming();
				//                    ccCanvas.start();
				combinNormalConn();
			}
			break;
		case EventID.EVENT_INIT_UPLOADPIM:
			//                this.preeventid=eventID;
			//                System.out.println("runtimes 1: "+runtimes);
			if (runtimes < 4) {// 激活流程
				rmsAdapter.openRms(CPProperty.RECORDID_MyPhoneNum);
				rmsAdapter.addPhoneNumber(myPhoneNum);
				rmsAdapter.closeRms(CPProperty.RECORDID_MyPhoneNum);
			}
			//                if (args[0] != null /*&& ((String) args[0]).equals(phonenum)*/) {//debug
			combineInitBody();
			waitingCanvas.hideMainMenu();
			waitingCanvas.setInfo(CPProperty.UPLOADPIM_IMFO[language]);
			//                }
			setCurrentDis(waitingCanvas);
			//                handleEvent(GUIController.EventID.EVENT_SHOW_MAIN, null);//debug
			break;
		case EventID.EVENT_INIT_MAIN:
			//                System.out.println("init main");
			initMain();
			//                System.out.println("ok!");
			break;
		case EventID.EVENT_SAVE_CCFILE:
			new Thread(new Runnable() {

				public void run() {
					synchronized (fileModel) {
						try {
							//                                        System.out.println("create file : " + CPProperty.SAVEPATH + "/" + initSystemModel.getDownloadCCID());
							fileModel.createFile(CPProperty.SAVEPATH + "/" + initSystemModel.getDownloadCCID(),
									initSystemModel.getDownloadCCData());
							if (!downloadIdStr.equals("")) {
								//                                            System.out.println("remove : " + downloadidString);
								allShouldDownCcId.removeElement(downloadIdStr);
							}

							removeAllDownloadID();

							if (allShouldDownCcId != null && !allShouldDownCcId.isEmpty()
									&& allShouldDownCcId.size() > 0) {
								for (int i = 0; i < allShouldDownCcId.size(); i++) {
									//                                                System.out.println("save id: "+(String) downloadid.elementAt(i));
									saveDownloadId((String) allShouldDownCcId.elementAt(i));
								}
							}
						} catch (Exception e) {
							e.printStackTrace();
						}
						handleEvent(GUIController.EventID.EVENT_SHOW_MAIN, null);
					}
				}
			}).start();
			break;
		case EventID.EVENT_SHOW_MAIN:
			if (waitingCanvas != null) {
				waitingCanvas = null;
			}
			if (!isSendSms) {
				isSendSms = true;
			}
			this.preEventId = eventID;
			if (runtimes < 4) {
				runtimes++;
				saveRunTimes();
			}
			//                diyDataModel.init();                
			if (firstStart) {
				setDownloadInfo();
			}

			if (args != null) {
				ccCanvas.showAlert(true);
				ccCanvas.setAlertInfo((String) args[0]);
			}

			//                ccCanvas.start();
			ccCanvas.setEvent(CCCanvas.MAIN);
			ccCanvas.hideMainMenu();
			//                System.out.println("main");
			if (!firstStart) {
				updatasoftwareurl = initSystemModel.getSoftwareUrl();
				if (!updatasoftwareurl.equals("")) {
					Alert alert1 = new Alert("");
					alert1.setString(CPProperty.INFO_UPDATASOFTWARE[language]);
					alert1.setTimeout(1000);
					setCurrentDis(alert1, ccCanvas);
				} else {
					setCurrentDis(ccCanvas);
				}
			}
			break;
		case EventID.EVENT_DIY:
			//                System.out.println("event diy");
			//                this.preeventid=eventID;
			custom = initSystemModel.getCustom();
			//                System.out.println("custom : "+custom);
			//                if (initSystemModel.getCustom() == HttpTools.PROPERTY_CUSTOM_SUPPORT) {
			ccCanvas.setEvent(CCCanvas.DIY);
			//                }else{
			//                    ccCanvas.setAlertInfo("");
			//                    ccCanvas.setAlert(true);
			//                }
			ccCanvas.hideMainMenu();
			setCurrentDis(ccCanvas);
			//                waitingCanvas=null;
			break;
		case EventID.EVENT_INIT_UPLOAD_DIY:
			if (!firstDiy && total_block > 0) {
				index_block = initSystemModel.getIndexBlock() + 1;
			}
			//                System.out.println("ib : "+index_block+" tb : "+total_block);
			if (index_block <= total_block) {
				if (total_block == 0 && diyDataModel.getMsgData() == null) {
					diyDataModel.init();
					index_block = 0;
					total_block = 0;
					ccCanvas.setAlertInfo(CPProperty.UPLOADIMG_FAULT_INFO[language]);
					ccCanvas.showAlert(true);
				} else if (total_block == 0 && diyDataModel.getMsgData().length > 0) {
					index_block = total_block + 1;
					ccCanvas.setAlertInfo(CPProperty.UPLOADGREETING_INFO[language]);
					combineGreetingMsgBody();
				} else {
					//                        System.out.println("file size : "+diyDataModel.getFileData().length);

					byte[] block = StringTools.getBytesFromBytes(DiyDataModel._singleBlockLength, total_block,
							index_block, diyDataModel.getFileData());
					//                        System.out.println("get block size : "+block.length);
					ccCanvas.setAlertInfo(CPProperty.UPLOADIMG_INFO[language] + index_block + "/" + total_block);
					combineDiyBody(total_block, index_block, block);
				}
			} else {
				diyDataModel.init();
				eventStack.removeAllElements();
				ccCanvas.setCurFileName("");
				//                    ccCanvas.setGprs(false);
				if (index_block > total_block/*&&total_block>0*/) {
					index_block = 0;
					total_block = 0;
					ccCanvas.setAlertInfo(CPProperty.UPLOADIMG_SUCCEED_INFO[language]);
					ccCanvas.showAlert(true);
				}
			}
			handleEvent(EventID.EVENT_SHOW_MAIN, null);
			//                ccCanvas.setEvent(CCCanvas.MAIN);
			//                setCurrent(ccCanvas);
			break;
		case EventID.EVENT_SHOW_REVIEW:
			try {
				String url = ccCanvas.getCurFilename();
				InputStream is = FileModel.getFileInputStream(url);

				Image view = ImageTools.createImage(is);

				showImgForm.setImage(view);
				setCurrentDis(showImgForm);
			} catch (Exception e) {
				Object[] obj = { CPProperty.ERROR_NO_FILE1[language] };
				handleEvent(EventID.EVENT_ALERT, obj);
				//                    System.out.println(e.toString());
			}

			break;
		case EventID.EVENT_SHOW_SETTING:
			//                this.preeventid=eventID;
			if (runtimes < 1) {// 确保是第一次运行
				runtimes++;
				saveRunTimes();
			}
			//                settingCanvas.setInfo(CPProperty.SET_GUIDE[language]);
			settingCanvas.setInfo(CPProperty.SET_GUIDE[language], settingString);
			settingCanvas.removeMenuItem();
			settingCanvas.initMenuItemSetting();
			setCurrentDis(settingCanvas);
			break;
		case EventID.EVENT_SHOW_SURESET:
			//                this.preeventid=eventID;
			settingCanvas.setInfo(CPProperty.SET_GUIDE_SURE[language], CPProperty.SURE_SETTING[language]);
			settingCanvas.removeMenuItem();
			settingCanvas.initMenuItemSureSet();
			setCurrentDis(settingCanvas);
			splashCanvas.stop();
			splashCanvas = null;
			break;
		case EventID.EVENT_SHOW_DECLARE:
			//                this.preeventid=eventID;
			if (runtimes < 2) {
				runtimes++;
				saveRunTimes();
			}
			settingCanvas.setInfo(CPProperty.DECLARE[language], declareString);
			settingCanvas.removeMenuItem();
			settingCanvas.initMenuItemDeclare();
			setCurrentDis(settingCanvas);
			break;
		case EventID.EVENT_SETTING_MENU:
			settingCanvas.showMainMenu();
			setCurrentDis(settingCanvas);
			break;
		case EventID.EVENT_SHOW_WAITING:
			setCurrentDis(waitingCanvas);
			break;
		case EventID.EVENT_WAITING_MENU:
			waitingCanvas.removeMenuItem();
			waitingCanvas.initMenuItem();
			waitingCanvas.showMainMenu();
			break;
		case EventID.EVENT_SHOW_GREETING:
			GreetingTextBox greetingTextBox = new GreetingTextBox(this, CPProperty.GREETING_TITLE[language],
					CPProperty.MAX_INPUT);
			setCurrentDis(greetingTextBox);
			break;
		case EventID.EVENT_MAIN_MENU:
			ccCanvas.removeMenuItem();
			ccCanvas.initMainmenuItem();
			ccCanvas.showMainMenu();
			break;
		case EventID.EVENT_DIY_MENU:
			ccCanvas.removeMenuItem();
			ccCanvas.initDiy_MenuItem();
			ccCanvas.showMainMenu();
			break;
		case EventID.EVENT_MAIN_DIY_IMG_MENU:
			ccCanvas.removeMenuItem();
			ccCanvas.initDiy_ImgMenuItem();
			ccCanvas.showMainMenu();
			break;
		case EventID.EVENT_MAIN_DIY_GREETING_MENU:
			break;
		case EventID.EVENT_MAIN_DIY_NAME_MENU:
			ccCanvas.removeMenuItem();
			ccCanvas.initDiy_selectUserMenuItem();
			ccCanvas.showMainMenu();
			break;
		case EventID.EVENT_MAIN_DIY_NUM_MENU:
			ccCanvas.removeMenuItem();
			ccCanvas.initDiy_selectUserNumberMenuItem();
			ccCanvas.showMainMenu();
			break;
		case EventID.EVENT_MAIN_PAY_MENU: // ?没用? 
			ccCanvas.removeMenuItem();
			ccCanvas.initMenuItem5();
			ccCanvas.showMainMenu();
			break;
		//            case EventID.EVENT_INITPIM:
		//                settingCanvas.setInfo("",CPProperty.INIT_PIM[language]);
		//                settingCanvas.removeMenuItem();
		//                settingCanvas.initMenuItemSetting();
		//                setCurrent(settingCanvas);
		//                break;
		case EventID.EVENT_UPDATANUMBER:
			//                System.out.println("updata num size others: "+updatanumber.size());
			if (!updatedNumbers.isEmpty() && updatedNumbers.size() > 0) {
				//                    System.out.println("save new list");
				//                    saveRmsPim(contractVector);
				saveRmsContactsList(numAndImageHt);
				//发送新增电话号码到服务器
				//                    System.out.println("updata num");
				combineUpdataPimBody(updatedNumbers);
				ccCanvas.setAlertInfo(CPProperty.UPLOADPIM_IMFO[language]);
				setCurrentDis(ccCanvas);
			}
			break;

		case EventID.EVENT_INIT_SPECIALNUM:
			firstStart = true;
			setIsInitOk(true);
			System.out.println("EVENT_INIT_SPECIALNUM: save special");
			saveSpecialNum();
			System.out.println("EVENT_INIT_SPECIALNUM: save ccid");
			saveDownloadCCid();
			System.out.println("EVENT_INIT_SPECIALNUM: save cmccid");
			saveDownloadCmccid();

			if (initSystemModel.getTimeStamp().length() > 0) {
				String stringtemp = initSystemModel.getTimeStamp();
				StringBuffer sb = new StringBuffer();
				for (int i = 0; i < 3; i++) {
					sb.append(stringtemp);
					sb.append("|");
				}
				sb.append(stringtemp);
				System.out.println("EVENT_INIT_SPECIALNUM: save timestamp : " + stringtemp);
				setTimeStamp(sb.toString());
			}

			if (initSystemModel.getConnectionTime() != -100) {
				System.out.println("EVENT_INIT_SPECIALNUM: set timing : " + initSystemModel.getConnectionTime());
				setTiming(initSystemModel.getConnectionTime());
			}

			getTiming();
			System.out.println("EVENT_INIT_SPECIALNUM: get timing : " + timing);

			if (runtimes < 4) {
				runtimes++;
				saveRunTimes();
			}
			handleEvent(GUIController.EventID.EVENT_INIT_MAIN, null);//debug
			break;
		case EventID.EVENT_UPDATASOFTWARE:
			if (!updatasoftwareurl.equals("")) {
				try {
					midlet.platformRequest(updatasoftwareurl);
				} catch (ConnectionNotFoundException ex) {
					ex.printStackTrace();
				}
			} else {
				Alert alert1 = new Alert("");
				alert1.setString(CPProperty.ERROR_UPDATASOFTWARE[language]);
				alert1.setTimeout(1000);
				setCurrentDis(alert1, currentDis);
			}
			break;
		case EventID.EVENT_REQUESTFORPAY:
			combinSurePay(myPhoneNum, 1);
			break;
		case EventID.EVENT_SHOW_PAY:
			firstDiy = true;
			eventStack.removeAllElements();
			pushPreEvent(CCCanvas.MAIN);

			ccCanvas.setEvent(CCCanvas.PAY);
			ccCanvas.setAlertInfo(initSystemModel.getPayInfo());
			setCurrentDis(ccCanvas);
			break;

		default:
			break;
		}
	}

	/** 压栈前一事件 */
	public void pushPreEvent(int event) {
		eventStack.push(new Integer(event));
	}

	/** 弹出前一事件 */
	public void popPreEvent() {
		if (!eventStack.isEmpty()) {
			int event = ((Integer) eventStack.pop()).intValue();
			//            if(event==GUIController.EventID.EVENT_FILELIST){
			//                diyDataModel.init();
			//            }
			handleEvent(event, null);
		}
	}

	/** 初始化主界面 */
	public void initMain() {
		getDownloadOperatorId();
		getDownloadCCid();
		saveDownloadId();
		handleEvent(GUIController.EventID.EVENT_SHOW_MAIN, null);
	}

	//    public void initDiy(){
	//        diyDataModel.init();
	//    }

	/** 发送激活短信 */
	public void sendSms(String text) {
		System.out.println("GUIController:sendSms(): " + smsNumber + " : " + smsPort + " Content: " + text);
		sendSmsThread = new SendSMSThread(this);
		sendSmsThread.setDestNumber(smsNumber);
		//        sendSMSThread.setSMSPort("4001");
		//        sendSMSThread.setDestMobileNumber("13426476145");
		sendSmsThread.setSmsPort(smsPort);
		sendSmsThread.setMessageText(text);
		sendSmsThread.notifySend(true);
		System.out.println("GUIController:sendSms(): start new thread to send sms");
		sendSmsThread.start();
	}

	public int getLanguage() {
		return this.language;
	}

	/** 写入运行步骤 */
	public void saveRunTimes() {
		rmsAdapter.openRms(CPProperty.RECORDID_RUNTIMES);
		rmsAdapter.addRuntimes(runtimes);
		rmsAdapter.closeRms(CPProperty.RECORDID_RUNTIMES);
	}

	/** 得到运行步骤 */
	public void getRuntimesFromRMS() {
		rmsAdapter.openRms(CPProperty.RECORDID_RUNTIMES);
		runtimes = rmsAdapter.getRunTimes();
		rmsAdapter.closeRms(CPProperty.RECORDID_RUNTIMES);
	}

	/** 得到运行步骤 */
	public int getRuntimes() {
		return runtimes;
	}

	public void saveSpecialNum() {
		Hashtable ht = new Hashtable();
		Hashtable ht_del = new Hashtable();
		if (!rmsAdapter.isNotExist(CPProperty.RECORDID_SPECIALNUM)) {
			ht = getSpecialNum();
			//            rmsAdapter.delRecordStore(CPProperty.RECORDID_SPECIALNUM);
		}

		ht_del = initSystemModel.getSpecialNumDel();

		Enumeration ee = ht.keys();
		while (ee.hasMoreElements()) {
			String id = (String) ee.nextElement();
			if (ht_del.containsKey(id)) {
				ht.remove(id);
				System.out.println("del special num : " + id);
			}
		}

		rmsAdapter.openRms(CPProperty.RECORDID_SPECIALNUM);

		Hashtable specialht = new Hashtable();
		specialht = initSystemModel.getSpecialNum();
		Enumeration e = specialht.keys();
		while (e.hasMoreElements()) {
			String id = (String) e.nextElement();
			String number = (String) specialht.get(id);
			ht.put(id, number);
			System.out.println("add special num : " + id + " : " + number);
		}

		Enumeration eee = ht.keys();
		while (eee.hasMoreElements()) {
			String id = (String) eee.nextElement();
			String number = (String) ht.get(id);
			System.out.println("special num : " + id + " : " + number);
			rmsAdapter.addSpecialnum(id, number);
		}

		rmsAdapter.closeRms(CPProperty.RECORDID_SPECIALNUM);
	}

	public Hashtable getSpecialNum() {
		Hashtable specialht = new Hashtable();
		rmsAdapter.openRms(CPProperty.RECORDID_SPECIALNUM);
		specialht = rmsAdapter.getSpecialnum();
		rmsAdapter.closeRms(CPProperty.RECORDID_SPECIALNUM);
		return specialht;
	}

	public void saveDownloadId() {
		rmsAdapter.openRms(CPProperty.RECORDID_DOWNLOAD_IDS);
		FileModel fm = new FileModel(this);
		fm.traverseDirectory(CPProperty.SAVEPATH);
		Vector filelist = fm.getList();

		if (!this.shouldDownOperatorCcIdHt.isEmpty()) {
			Enumeration e = shouldDownOperatorCcIdHt.keys();
			while (e.hasMoreElements()) {
				String id = ((String) e.nextElement()).toUpperCase();
				boolean isexist = false;

				for (int i = 0; i < filelist.size(); i++) {
					String temp = (String) filelist.elementAt(i);
					temp = temp.substring(temp.lastIndexOf('/') + 1);
					String imgid = temp.substring(0, temp.lastIndexOf('.')).toUpperCase();
					//                    System.out.println("imgid : "+imgid+" id : "+id);
					if (imgid.equals(id)) {
						isexist = true;
						break;
					}
				}
				int type = Integer.parseInt((String) shouldDownOperatorCcIdHt.get(id));
				if (type == ProtocolTools.PROPERTY_CMCC_CCID_TYPE_ADD && !isexist) {
					rmsAdapter.addDownloadId(id);
				}
			}
		}
		if (!shouldDownFriendCcId.isEmpty()) {
			for (int i = 1; i < shouldDownFriendCcId.size(); i += 3) {
				String id = ((String) shouldDownFriendCcId.elementAt(i)).toUpperCase();
				boolean isexist = false;
				//                FileModel fm=new FileModel(this);
				//                fm.traverseDirectory(CPProperty.SAVEPATH);
				//                Vector filelist = fm.getList();
				for (int j = 0; j < filelist.size(); j++) {
					String temp = (String) filelist.elementAt(j);
					temp = temp.substring(temp.lastIndexOf('/') + 1);
					String imgid = temp.substring(0, temp.lastIndexOf('.')).toUpperCase();
					if (imgid.equals(id)) {
						isexist = true;
						break;
					}
				}
				if (!isexist) {
					rmsAdapter.addDownloadId((String) shouldDownFriendCcId.elementAt(i));
				}
			}
		}

		rmsAdapter.closeRms(CPProperty.RECORDID_DOWNLOAD_IDS);
	}

	public void saveDownloadId(String id) {
		rmsAdapter.openRms(CPProperty.RECORDID_DOWNLOAD_IDS);
		rmsAdapter.addDownloadId(id);
		rmsAdapter.closeRms(CPProperty.RECORDID_DOWNLOAD_IDS);
	}

	public void getDownloadId() {
		if (!rmsAdapter.isNotExist(CPProperty.RECORDID_DOWNLOAD_IDS)) {
			rmsAdapter.openRms(CPProperty.RECORDID_DOWNLOAD_IDS);
			allShouldDownCcId = rmsAdapter.getDownloadIds();
			rmsAdapter.closeRms(CPProperty.RECORDID_DOWNLOAD_IDS);
		}
	}

	public void removeAllDownloadID() {
		if (!rmsAdapter.isNotExist(CPProperty.RECORDID_DOWNLOAD_IDS)) {
			rmsAdapter.delRecordStore(CPProperty.RECORDID_DOWNLOAD_IDS);
		}
	}

	public void saveDownloadCCid() {
		rmsAdapter.openRms(CPProperty.RECORDID_DOWNLOAD_CCID);
		Vector tempv = rmsAdapter.getDownloadCCid();
		rmsAdapter.closeRms(CPProperty.RECORDID_DOWNLOAD_CCID);

		rmsAdapter.delRecordStore(CPProperty.RECORDID_DOWNLOAD_CCID);

		rmsAdapter.openRms(CPProperty.RECORDID_DOWNLOAD_CCID);
		Vector v = initSystemModel.getCCIdVector();
		if (!v.isEmpty()) {
			for (int i = 0; i < v.size(); i += 3) {
				String number = (String) v.elementAt(i);
				String id = (String) v.elementAt(i + 1);
				int type = Integer.parseInt((String) v.elementAt(i + 2));
				//                ccidht.put(number, id);
				if (tempv.contains(number)) {
					for (int j = 0; j < tempv.size(); j += 3) {
						String tempnum = (String) tempv.elementAt(j);
						if (tempnum.equals(number)) {
							//                            System.out.println("del : "+tempnum+" : "+(String)tempv.elementAt(j+1));
							tempv.removeElementAt(j + 2);
							tempv.removeElementAt(j + 1);
							tempv.removeElementAt(j);
							j -= 3;
						}
					}
				}

				//                System.out.println("save : " + number + " : " + id + " : " + type);
				rmsAdapter.addDownloadCCid(number, id, type);
			}
		}

		if (tempv.size() > 0) {
			for (int i = 0; i < tempv.size(); i += 3) {
				String number = (String) tempv.elementAt(i);
				String id = (String) tempv.elementAt(i + 1);
				int type = Integer.parseInt((String) tempv.elementAt(i + 2));
				//                System.out.println("save1 : "+number+" : "+id+" : "+type);
				rmsAdapter.addDownloadCCid(number, id, type);
			}
		}
		rmsAdapter.closeRms(CPProperty.RECORDID_DOWNLOAD_CCID);
	}

	public void getDownloadCCid() {
		rmsAdapter.openRms(CPProperty.RECORDID_DOWNLOAD_CCID);
		shouldDownFriendCcId = rmsAdapter.getDownloadCCid();
		rmsAdapter.closeRms(CPProperty.RECORDID_DOWNLOAD_CCID);
	}

	public void delDownloadCCid() {
		if (!rmsAdapter.isNotExist(CPProperty.RECORDID_DOWNLOAD_CCID)) {
			rmsAdapter.delRecordStore(CPProperty.RECORDID_DOWNLOAD_CCID);
		}
	}

	public void saveDownloadCmccid() {
		rmsAdapter.openRms(CPProperty.RECORDID_DOWNLOAD_CMCCCCID);
		Hashtable ht = initSystemModel.getCMCCht();
		if (ht.size() > 0) {
			Enumeration e = ht.keys();
			while (e.hasMoreElements()) {
				String key = (String) e.nextElement();
				int type = ((Integer) ht.get(key)).intValue();
				rmsAdapter.addDownloadCmccid(key, type);
			}
		}
		rmsAdapter.closeRms(CPProperty.RECORDID_DOWNLOAD_CMCCCCID);
	}

	public void getDownloadOperatorId() {
		rmsAdapter.openRms(CPProperty.RECORDID_DOWNLOAD_CMCCCCID);
		shouldDownOperatorCcIdHt = rmsAdapter.getDownloadCmccid();
		rmsAdapter.closeRms(CPProperty.RECORDID_DOWNLOAD_CMCCCCID);
	}

	public void delDownloadOperatorId() {
		if (!rmsAdapter.isNotExist(CPProperty.RECORDID_DOWNLOAD_CMCCCCID)) {
			rmsAdapter.delRecordStore(CPProperty.RECORDID_DOWNLOAD_CMCCCCID);
		}
	}

	public void setTimeStamp(String timestamp) {
		rmsAdapter.openRms(CPProperty.RECORDID_TIMESTAMP);
		rmsAdapter.addTimeStamp(timestamp);
		rmsAdapter.closeRms(CPProperty.RECORDID_TIMESTAMP);
	}

	public String getTimeStamp() {
		String timestamp = "";
		rmsAdapter.openRms(CPProperty.RECORDID_TIMESTAMP);
		timestamp = rmsAdapter.getTimeStamp();
		rmsAdapter.closeRms(CPProperty.RECORDID_TIMESTAMP);
		return timestamp;
	}

	/** 设置Push启动时间间隔 */
	public void setTiming(int time) {
		rmsAdapter.openRms(CPProperty.RECORDID_TIME_CONNECTION);
		rmsAdapter.addTiming(time);
		rmsAdapter.closeRms(CPProperty.RECORDID_TIME_CONNECTION);
	}

	/** 得到Push启动时间间隔 */
	public void getTiming() {
		rmsAdapter.openRms(CPProperty.RECORDID_TIME_CONNECTION);
		timing = rmsAdapter.getTiming();
		//        System.out.println("timeing : "+timing);
		rmsAdapter.closeRms(CPProperty.RECORDID_TIME_CONNECTION);
	}

	/** 保存号码与图片对应关系到RMS */
	public void saveRmsContactsList(Hashtable numAndImageHt) {
		if (!rmsAdapter.isNotExist(CPProperty.RECORDID_NumAndImage)) {
			rmsAdapter.delRecordStore(CPProperty.RECORDID_NumAndImage);
		}
		rmsAdapter.openRms(CPProperty.RECORDID_NumAndImage);
		rmsAdapter.saveContacts(numAndImageHt);
		rmsAdapter.closeRms(CPProperty.RECORDID_NumAndImage);
	}

	//    public void saveRmsPim(Vector[] v) {
	//        if (!rmsAdapter.isExist(CPProperty.RECORDID_CONTRACT)) {
	//            rmsAdapter.delRecordStore(CPProperty.RECORDID_CONTRACT);
	//        }
	//        rmsAdapter.openRms(CPProperty.RECORDID_CONTRACT);
	//        rmsAdapter.addContract(v);
	//        rmsAdapter.closeRms(CPProperty.RECORDID_CONTRACT);
	//    }

	/** 得到保存在RMS中的号码图片对应关系hashtable */
	public void getRmsNumAndImage() {
		rmsAdapter.openRms(CPProperty.RECORDID_NumAndImage);
		rmsHt = rmsAdapter.getNumAndImgHashtable();
		rmsAdapter.closeRms(CPProperty.RECORDID_NumAndImage);
	}

	/** 保存联系人与图片的对应关系 ? */
	public void saveNumAndPic(Hashtable ht) {
		if (!rmsAdapter.isNotExist(CPProperty.RECORDID_NUMANDPIC)) {
			rmsAdapter.delRecordStore(CPProperty.RECORDID_NUMANDPIC);
		}
		rmsAdapter.openRms(CPProperty.RECORDID_NUMANDPIC);
		rmsAdapter.addNumAndPic(ht);
		rmsAdapter.closeRms(CPProperty.RECORDID_NUMANDPIC);
	}

	public Hashtable getNumAndPic() {
		Hashtable ht = new Hashtable();
		rmsAdapter.openRms(CPProperty.RECORDID_NUMANDPIC);
		ht = rmsAdapter.getNumAndPic();
		rmsAdapter.closeRms(CPProperty.RECORDID_NUMANDPIC);
		return ht;
	}

	public void saveAlermTime(long delay, long time) {
		rmsAdapter.openRms(CPProperty.RECORDID_ALERMTIME);
		rmsAdapter.addAlermTimes(delay, time);
		rmsAdapter.closeRms(CPProperty.RECORDID_ALERMTIME);
	}

	public long[] getAlermTime() {
		long time[] = new long[2];
		rmsAdapter.openRms(CPProperty.RECORDID_ALERMTIME);
		time = rmsAdapter.getAlermTimes();
		rmsAdapter.closeRms(CPProperty.RECORDID_ALERMTIME);
		return time;
	}

	public String getSmsPort() {
		return this.smsPort;
	}

	public int getShortCyc() {
		return shortCyc;
	}

	public int getLongCyc() {
		return longCyc;
	}

	/** 是否支持手机定制 */
	public int getCustom() {
		//        System.out.println("get custom : "+custom);
		return custom;
	}

	/** 
	 * 改变电话号码与图片的对应关系 ?
	 * @param filelists
	 */
	private void modifyPimImgUrl(Vector filelists) {
		Vector filelist = new Vector();
		String tempfile = "";
		for (int i = 0; i < filelists.size(); i++) {
			tempfile = filelists.elementAt(i).toString();
			tempfile = tempfile.substring(tempfile.lastIndexOf('/') + 1);
			filelist.addElement(tempfile);
		}
		Hashtable specialnum = getSpecialNum();
		Hashtable napht = getNumAndPic();
		//        Enumeration ccide=ccidht.keys();
		//        while(ccide.hasMoreElements()){
		//            String num=(String)ccide.nextElement();
		//            napht.put(num, ccidht.get(num));
		//        }

		Vector opmlist = new Vector();//运营商彩像文件
		//        System.out.println("file : "+filelist.size());
		for (int j = 0; j < filelist.size(); j++) {
			String filename = ((String) filelist.elementAt(j));
			//            filename=filename.substring(filename.lastIndexOf('/'));
			String temp = filename.substring(0, 3).toUpperCase();
			if (temp.equals(CPProperty.OPM)) {
				System.out.println("opm file : " + filename);
				opmlist.addElement(filename);
			}
		}

		try {
			if (!opmlist.isEmpty() && opmlist.size() > 0) {
				Enumeration ce = numAndImageHt.keys();
				int randomIndex = 0;

				while (ce.hasMoreElements()) {
					randomIndex++;
					int index = randomIndex % opmlist.size();

					String key = (String) ce.nextElement();
					ccCanvas.setAlertInfo(CPProperty.INIT_MODIFY_PIM[language] + " " + key);
					if (specialnum != null && specialnum.size() > 0) {
						if (specialnum.contains(key)) {
							pimModel.changeImage(key, "", 2);
							System.out.println("containt key : " + key);
							break;
						}
					}
					String tempkey = key;
					int keylen = tempkey.length();
					if (keylen > CPProperty.MATCH_MAX) {
						tempkey = tempkey.substring(keylen - CPProperty.MATCH_MAX, keylen);
					}

					if (napht.containsKey(tempkey)) {
						String imgpath = ((String) napht.get(tempkey));
						String temp = "";

						if (imgpath.length() > 3) {
							temp = imgpath.substring(0, 3).toUpperCase();
						}

						if (!temp.equals(CPProperty.IMG)) {
							System.out.println("2 : modify(has special number): " + key + " : "
									+ (String) opmlist.elementAt(index));
							pimModel.changeImage(key, (String) opmlist.elementAt(index), 2);
						} else {//保证用户手动修改后恢复
							if (filelist.contains(imgpath)) {
								System.out.println("resume(has special number): " + key + " : " + imgpath);
								pimModel.changeImage(key, imgpath, 1);
							} else {
								System.out.println("resume opm(has special number): " + key + " : "
										+ (String) opmlist.elementAt(index));
								pimModel.changeImage(key, (String) opmlist.elementAt(index), 2);
							}
						}
					} else {
						System.out.println("1 : modify(has special number): " + key + " : "
								+ (String) opmlist.elementAt(index));
						pimModel.changeImage(key, (String) opmlist.elementAt(index), 2);
					}
				}
			}

			if (!shouldDownFriendCcId.isEmpty() && shouldDownFriendCcId.size() > 0) {
				for (int i = 0; i < shouldDownFriendCcId.size(); i += 3) {
					String number = (String) shouldDownFriendCcId.elementAt(i);
					String id = (String) shouldDownFriendCcId.elementAt(i + 1);
					int type = Integer.parseInt((String) shouldDownFriendCcId.elementAt(i + 2));
					System.out.println("number : " + number + " id : " + id + " type : " + type);
					ccCanvas.setAlertInfo(CPProperty.INIT_MODIFY_PIM[language] + " " + number);
					String filename = "";
					for (int j = 0; j < filelist.size(); j++) {
						filename = (String) filelist.elementAt(j);
						//                        System.out.println("filename : "+filename);
						//                        filename=filename.substring(filename.lastIndexOf('/'));
						//                        System.out.println("filename : "+filename);
						String fileid = filename.substring(0, filename.lastIndexOf('.')).toUpperCase();
						//                        System.out.println("fileid : "+fileid);
						if (fileid.equals(id)) {
							break;
						}
					}
					if (type == 1) {// 添加修改彩像
						System.out.println("add: " + number + " : " + filename);
						if (specialnum != null && specialnum.size() > 0) {
							if (!specialnum.contains(number)) {
								int numlen = number.length();
								if (numlen > CPProperty.MATCH_MAX) {
									number = number.substring(numlen - CPProperty.MATCH_MAX, numlen);
								}
								if (napht.containsKey(number)) {
									napht.remove(number);
								}
								napht.put(number, filename);
							}
						} else {
							int numlen = number.length();
							if (numlen > CPProperty.MATCH_MAX) {
								number = number.substring(numlen - CPProperty.MATCH_MAX, numlen);
							}
							if (napht.containsKey(number)) {
								napht.remove(number);
							}
							napht.put(number, filename);
						}
						pimModel.changeImage(number, filename, 1);
					} else {//删除彩像，有运营商彩像则替换为运营商彩像，没有则删除大头帖
						int numlen = number.length();
						if (numlen > CPProperty.MATCH_MAX) {
							number = number.substring(numlen - CPProperty.MATCH_MAX, numlen);
						}

						if (napht.containsKey(number)) {
							String opmfilename = "";
							int size = opmlist.size();
							int index = 0;
							if (size > 0) {
								Random random1 = new Random();
								index = Math.abs(random1.nextInt()) % size;
								opmfilename = (String) opmlist.elementAt(index);
								System.out.println("del: " + number + " : " + opmfilename);
								pimModel.changeImage(number, opmfilename, 2);
							} else {
								System.out.println("del: " + number);
								pimModel.delImage(number);
							}
						}
					}
				}
			}
			//            opmlist.removeAllElements();
			delDownloadCCid();
			delDownloadOperatorId();

			//            Vector[] cv = pimModel.getContractList();
			//            saveRmsPim(cv);
			saveRmsContactsList(pimModel.getContactHashtable());

		} catch (Exception e) {
			//            System.out.println("exce : "+e.toString());
		}
		saveNumAndPic(napht);
		if (isPushWakeup) {
			//                System.out.println("exit");
			handleEvent(EventID.EVENT_EXIT, null);
		}
	}

	/** 设置下载的内容 ? */
	public void setDownloadInfo() {
		//        System.out.println("start download");
		getDownloadId();

		boolean resetpim = false;
		FileModel fm = new FileModel(this);
		fm.traverseDirectory(CPProperty.SAVEPATH);
		if (allShouldDownCcId != null && allShouldDownCcId.size() > 0) {

			downloadIdStr = (String) allShouldDownCcId.elementAt(0);
			//            System.out.println("download..."+downloadidString);

			Vector v = fm.getList();
			Vector vv = new Vector();

			for (int i = 0; i < v.size(); i++) {
				String fileid = ((String) v.elementAt(i)).toUpperCase();
				fileid = fileid.substring(fileid.lastIndexOf('/') + 1);
				fileid = fileid.substring(0, fileid.lastIndexOf('.'));
				//                System.out.println("fileid : "+fileid);
				vv.addElement(fileid);
			}

			//            for(int i=0;i<v.size();i++){
			//                System.out.println("file : "+(String)v.elementAt(i));
			//            }
			//            System.out.println("downloadString : "+downloadidString);
			while (vv.contains(downloadIdStr)) {
				allShouldDownCcId.removeElementAt(0);
				if (allShouldDownCcId.size() > 0) {
					downloadIdStr = (String) allShouldDownCcId.elementAt(0);
				} else {
					downloadIdStr = "";
				}
				//                System.out.println("downloadString 2: "+downloadidString);
			}
			//            System.out.println("downloadString 3: "+downloadidString);
			if (!downloadIdStr.equals("")) {
				combineDownloadOperatorIdBody(((String) allShouldDownCcId.elementAt(0)));
				//            System.out.println("download : " + downloadidString);
				ccCanvas.setAlertInfo(CPProperty.DOWNLOAD_CCFILE[language]);
				setCurrentDis(ccCanvas);
			} else {
				resetpim = true;
			}
		} else {
			resetpim = true;
		}

		if (resetpim) {
			//            System.out.println("reset pim");
			ccCanvas.setGprs(true);
			ccCanvas.setAlertInfo(CPProperty.INIT_MODIFY_PIM[language]);
			//            ccCanvas.setAlertInfo(CPProperty.INIT_IMGURL[language]+" ");
			setCurrentDis(ccCanvas);
			//            FileModel fm = new FileModel(this);
			//            fm.start();
			//            synchronized (fm) {

			downloadIdStr = "";
			//                fm.traverseDirectory(CPProperty.SAVEPATH);
			//                fm.traverseDirectory(CPProperty.SAVEPATH);
			Vector filelist = fm.getList();

			modifyPimImgUrl(filelist);
			//            }
			ccCanvas.setGprs(false);
			//            System.out.println("updata num");
			if (updatedNumbers != null && !updatedNumbers.isEmpty() && updatedNumbers.size() > 0) {
				//                    System.out.println("save new list");
				//                    saveRmsPim(contractVector);
				saveRmsContactsList(numAndImageHt);
				//发送新增电话号码到服务器
				//                    System.out.println("updata num");
				combineUpdataPimBody(updatedNumbers);
				ccCanvas.setAlertInfo(CPProperty.UPLOADPIM_IMFO[language]);
				//                    setCurrent(ccCanvas);
			}

			firstStart = false;
			//            System.out.println("exit");
		}
	}

	/**　组合协议,与配置服务器通讯,取得短信网关地址及通信服务器地址 */
	public void combineCfgBody(String phonenum) {
		System.out.println("GUIController:combineCfgBody()");
		protocolTools.reset();
		byte[] number = phonenum.getBytes();
		byte[] numberlength = StringTools.int2byte(number.length);
		byte[] numberproperty = StringTools.int2ByteArr(ProtocolTools.PROPERTY_PHONENUMBER);

		protocolTools.setMessageTypeValue((short) ProtocolTools.VALUE_MESSAGETYPE_CONFIGSERVER);
		protocolTools.add(numberproperty);
		protocolTools.add(numberlength);
		protocolTools.add(number);
		// 得到组织好的协议包字节数组
		byte[] body = protocolTools.getHeader();

		//        StringBuffer sb = new StringBuffer();
		//        String content = null;

		//        try {
		//            int ch = -1;
		//            InputStream is = getClass().getResourceAsStream("/lprogress1.png");///lprogress1.png
		//            while ((ch = is.read()) != -1) {
		//                sb.append(ch);
		//            }
		//            content = sb.toString();
		//        } catch (Exception e) {
		//            e.printStackTrace();
		//        }

		// 开始联网操作
		gprsConnection = new GPRSConnection(waitingCanvas, CPProperty.CONFIGSERVER_URL, body);
		gprsConnection.start();
	}

	public void combinSurePay(String phonenum, int pay) {
		System.out.println("GUIController:combinSurePay()");
		protocolTools.reset();
		protocolTools.setMessageTypeValue((short) ProtocolTools.VALUE_MESSAGETYPE_SURE_PAY);
		byte[] number = (phonenum).getBytes();
		byte[] numberlength = StringTools.int2byte(number.length);
		byte[] numberproperty = StringTools.int2ByteArr(ProtocolTools.PROPERTY_PHONENUMBER);
		number = StringTools.linkbytes(numberproperty, numberlength, number);
		protocolTools.add(number);
		byte[] payproperty = StringTools.int2ByteArr(ProtocolTools.PROPERTY_SUREPAY);
		byte[] paylen = StringTools.int2byte(1);
		byte[] paydata = { (byte) pay };
		paydata = StringTools.linkbytes(payproperty, paylen, paydata);
		protocolTools.add(paydata);

		byte[] body = protocolTools.getHeader();

		gprsConnection = new GPRSConnection(ccCanvas, initSystemModel.getComuUrl(), body);
		gprsConnection.start();
	}

	/** 普通连接 */
	public void combinNormalConn() {
		System.out.println("GUIController:combinNormalConn()");
		protocolTools.reset();
		protocolTools.setMessageTypeValue((short) ProtocolTools.VALUE_MESSAGETYPE_NORMALCONNECTION);

		protocolTools.add(StringTools.int2ByteArr(ProtocolTools.PROPERTY_PHONENUMBER));//0x0008
		protocolTools.add(StringTools.int2byte(myPhoneNum.length()));
		protocolTools.add(myPhoneNum.getBytes());

		protocolTools.add(StringTools.int2ByteArr(ProtocolTools.PROPERTY_VERSIOIN_INFO));//0x0015
		protocolTools.add(StringTools.int2byte(clientVersion.length()));
		protocolTools.add(clientVersion.getBytes());

		String timestamp = getTimeStamp();
		protocolTools.add(StringTools.int2ByteArr(ProtocolTools.PROPERTY_TIMESTAMP));//0x0015
		protocolTools.add(StringTools.int2byte(timestamp.length()));
		protocolTools.add(timestamp.getBytes());

		byte[] body = protocolTools.getHeader();

		//        for (int i = 0; i < body.length; i++) {
		//            System.out.print(" " + body[i]);            
		//        }

		gprsConnection = new GPRSConnection(waitingCanvas, initSystemModel.getComuUrl(), body);
		gprsConnection.start();
	}

	/** 发送新增电话号码到服务器 */
	public void combineUpdataPimBody(Vector newNumbers) {
		System.out.println("GUIController:combineUpdataPimBody()");
		protocolTools.reset();
		protocolTools.setMessageTypeValue((short) ProtocolTools.VALUE_MESSAGETYPE_UPLOADPIM_UPDATA);

		protocolTools.add(StringTools.int2ByteArr(ProtocolTools.PROPERTY_PHONENUMBER));//0x0008
		protocolTools.add(StringTools.int2byte(myPhoneNum.length()));
		protocolTools.add(myPhoneNum.getBytes());

		protocolTools.add(StringTools.int2ByteArr(ProtocolTools.PROPERTY_PIM));
		byte[] temp = {};
		for (int i = 0; i < newNumbers.size(); i++) {
			String number = (String) newNumbers.elementAt(i);
			byte[] pdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_PIM_NUMBER);
			byte[] ldata = StringTools.int2byte(number.length());
			pdata = StringTools.linkbytes(pdata, ldata, number.getBytes());

			byte[] ptdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_PIM_CONTROLTYPE);
			byte[] pldata = StringTools.int2byte(ProtocolTools.PROPERTY_PIM_CONTROLTYPE_ADD.length);
			byte[] pvdata = ProtocolTools.PROPERTY_PIM_CONTROLTYPE_ADD;
			ptdata = StringTools.linkbytes(ptdata, pldata, pvdata);
			pdata = StringTools.linkbytes(pdata, ptdata);

			byte[] pidata = StringTools.int2ByteArr(i + 1);
			byte[] lidata = StringTools.int2byte(pdata.length);
			pdata = StringTools.linkbytes(pidata, lidata, pdata);

			temp = StringTools.linkbytes(temp, pdata);
		}
		byte[] lendata = StringTools.int2byte(temp.length);
		temp = StringTools.linkbytes(lendata, temp);
		protocolTools.add(temp);

		byte[] pimdata = protocolTools.getHeader();
		//        System.out.print("send updata data : "); 
		//        for (int i = 0; i < pimdata.length; i++) {
		//            System.out.print(" " + pimdata[i]);            
		//        }

		gprsConnection = new GPRSConnection(ccCanvas, initSystemModel.getComuUrl(), pimdata);
		gprsConnection.start();
	}

	/** 上传电话本 */
	public void combineInitBody() {
		System.out.println("GUIController:combineInitBody(): -----combine and upload contacts-----");
		protocolTools.reset();
		protocolTools.setMessageTypeValue((short) ProtocolTools.VALUE_MESSAGETYPE_UPLOADPIM);

		protocolTools.add(StringTools.int2ByteArr(ProtocolTools.PROPERTY_PHONENUMBER));//0x0008
		protocolTools.add(StringTools.int2byte(myPhoneNum.length()));
		protocolTools.add(myPhoneNum.getBytes());

		// 客户端机型类型
		byte[] deviceTypeData = StringTools.linkbytes(StringTools
				.int2ByteArr(ProtocolTools.PROPERTY_CLIENT_CONFIG_TYPE), StringTools.int2byte(4), StringTools
				.int2byte(clientType));
		//        byte[] osdata = StringTools.linkbytes(StringTools.int2ByteArr2(HttpTools.PROPERTY_CLIENT_CONFIG_OS), StringTools.int2byte(clientos.length()), clientos.getBytes());
		// 软件类型
		byte[] softwareTypeData = StringTools.linkbytes(StringTools
				.int2ByteArr(ProtocolTools.PROPERTY_CLIENT_CONFIG_SOFTWARETYPE), StringTools
				.int2byte(ProtocolTools.PROPERTY_CLIENT_CONFIG_SOFTWARETYPE_LENGTH),
				ProtocolTools.PROPERTY_CLIENT_CONFIG_SOFTWARETYPE_JAVA);
		// 屏幕类型
		byte[] screenData_h = StringTools.linkbytes(StringTools
				.int2ByteArr(ProtocolTools.PROPERTY_CLIENT_CONFIG_SCREENHEIGHT), StringTools.int2byte(((Integer
				.toString(CPProperty.SCREENHEIGHT)).getBytes()).length), (Integer.toString(CPProperty.SCREENHEIGHT))
				.getBytes());
		byte[] screenData_w = StringTools.linkbytes(StringTools
				.int2ByteArr(ProtocolTools.PROPERTY_CLIENT_CONFIG_SCREENWIDTH), StringTools.int2byte(((Integer
				.toString(CPProperty.SCREENWIDTH)).getBytes()).length), (Integer.toString(CPProperty.SCREENWIDTH))
				.getBytes());
		//        byte[] temp = StringTools.linkbytes(typedata, osdata, swtypedata);
		byte[] temp = StringTools.linkbytes(deviceTypeData, softwareTypeData);
		temp = StringTools.linkbytes(temp, screenData_h, screenData_w);

		// 添加客户端类型数据
		protocolTools.add(StringTools.int2ByteArr(ProtocolTools.PROPERTY_CLIENT_CONFIG));
		protocolTools.add(StringTools.int2byte(temp.length));
		protocolTools.add(temp);

		if (!numAndImageHt.isEmpty()) {
			System.out.println("GUIController:combineInitBody(): contact is not empty: start combine contacts");
			protocolTools.add(StringTools.int2ByteArr(ProtocolTools.PROPERTY_PIM));
			Enumeration e = numAndImageHt.keys();
			int i = 0;
			int len = 0;

			/** 要上传的电话本数据 */
			byte[] phdata = StringTools.int2byte(len);

			while (e.hasMoreElements()) {
				i++;
				String s = e.nextElement().toString();
				phdata = StringTools.linkbytes(phdata, StringTools.int2ByteArr(i));
				phdata = StringTools.linkbytes(phdata, StringTools.int2byte((s.length() + ProtocolTools.LENGTH * 2
						+ ProtocolTools.PROPERTY_LENGTH * 2 + ProtocolTools.PROPERTY_PIM_CONTROLTYPE_ADD.length)));
				phdata = StringTools.linkbytes(phdata, StringTools.int2ByteArr(ProtocolTools.PROPERTY_PIM_NUMBER));
				phdata = StringTools.linkbytes(phdata, StringTools.int2byte(s.length()));
				phdata = StringTools.linkbytes(phdata, s.getBytes());
				phdata = StringTools.linkbytes(phdata, StringTools.int2ByteArr(ProtocolTools.PROPERTY_PIM_CONTROLTYPE));
				phdata = StringTools.linkbytes(phdata, StringTools
						.int2byte(ProtocolTools.PROPERTY_PIM_CONTROLTYPE_ADD.length));
				phdata = StringTools.linkbytes(phdata, ProtocolTools.PROPERTY_PIM_CONTROLTYPE_ADD);
			}

			phdata = StringTools.replacebytes(phdata, StringTools.int2byte(phdata.length - 4), 0);

			protocolTools.add(phdata);
		}
		byte[] pimdata = protocolTools.getHeader();

		//        for(int ii=0;ii<pimdata.length;ii++){
		//            System.out.print(pimdata[ii]+" ");
		//        }

		gprsConnection = new GPRSConnection(waitingCanvas, initSystemModel.getComuUrl(), pimdata);
		System.out.println("GUIController:combineInitBody(): start upload contacts");
		gprsConnection.start();
	}

	public void combineDownloadOperatorIdBody(String ccid) {
		System.out.println("GUIController:combineDownloadOperatorIdBody()");
		//        System.out.println("ccid : "+ccid);
		protocolTools.reset();
		byte[] number = (myPhoneNum).getBytes();
		byte[] numberlength = StringTools.int2byte(number.length);
		byte[] numberproperty = StringTools.int2ByteArr(ProtocolTools.PROPERTY_PHONENUMBER);

		protocolTools.setMessageTypeValue((short) ProtocolTools.VALUE_MESSAGETYPE_DOWNLOAD_CC);
		protocolTools.add(numberproperty);
		protocolTools.add(numberlength);
		protocolTools.add(number);

		byte[] temp;
		byte[] iddata = ccid.getBytes();
		byte[] piddata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_CC_FIEL);
		temp = StringTools.linkbytes(piddata, StringTools.int2byte(iddata.length), iddata);

		byte tvdata = ProtocolTools.PROPERTY_CC_TYPE_CMCCCC;
		if (ccid.startsWith(CPProperty.IMG)) {
			tvdata = ProtocolTools.PROPERTY_CC_TYPE_FCC;
		} else if (ccid.startsWith(CPProperty.OPM)) {
			tvdata = ProtocolTools.PROPERTY_CC_TYPE_CMCCCC;
		}
		byte[] typedata = { tvdata };
		typedata = StringTools.linkbytes(StringTools.int2byte(typedata.length), typedata);
		byte[] ptypedata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_CC_TYPE);
		temp = StringTools.linkbytes(temp, ptypedata, typedata);

		byte[] totaldata = "1".getBytes();
		totaldata = StringTools.linkbytes(StringTools.int2byte(totaldata.length), totaldata);
		byte[] ptotaldata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_CC_TOTAL);
		temp = StringTools.linkbytes(temp, ptotaldata, totaldata);

		byte[] countdata = "1".getBytes();
		countdata = StringTools.linkbytes(StringTools.int2byte(countdata.length), countdata);
		byte[] pcountdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_CC_COUNT);
		temp = StringTools.linkbytes(temp, pcountdata, countdata);

		byte[] pccdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_CC);
		temp = StringTools.linkbytes(pccdata, StringTools.int2byte(temp.length), temp);

		protocolTools.add(temp);

		byte[] body = protocolTools.getHeader();
		//        for(int i=0;i<body.length;i++){
		//            System.out.print(body[i]+" ");
		//        }

		gprsConnection = new GPRSConnection(ccCanvas, initSystemModel.getComuUrl(), body);
		gprsConnection.start();
	}

	public boolean hasDiyDataFile() {
		//		if (diyDataModel.getFileData().length > 0) {
		//			return true;
		//		}
		//		return false;
		return diyDataModel.getFileData().length > 0;
	}

	public boolean isEventFileUpload() {
		//		boolean isfile = false;
		//		if (ccCanvas.getEvent() == CCCanvas.DIY_IMG) {
		//			isfile = true;
		//		}
		//		return isfile;
		return ccCanvas.getEvent() == CCCanvas.DIY_IMG;
	}

	/** 设置DIY图片 */
	public boolean setDiyImg(String filename) {
		//        System.out.println("set name : "+filename);
		if (filename.equals("")) {
			return false;
		}
		try {
			firstDiy = true;
			diyDataModel.init();
			//            StringBuffer sb=new StringBuffer();
			//            String content = null;

			// 本地DIY图片输入流
			InputStream is = FileModel.getFileInputStream(filename);

			int len = is.available();
			if (len > 1024 * 500) { // 图片大于500k提示用户无法上传
				Object[] obj = { CPProperty.ERROR_IMG_NULL[language] };
				handleEvent(EventID.EVENT_ALERT, obj);
				return false;
			} else if (len < 1) { // 提示用户图片不存在
				Object[] obj = { CPProperty.ERROR_NO_FILE1[language] };
				handleEvent(EventID.EVENT_ALERT, obj);
				return false;
			} else {

				byte[] temp = new byte[len];

				int ch = -1;
				int i = 0;
				while ((ch = is.read()) != -1) {
					temp[i] = (byte) ch;
					i++;
				}

				diyDataModel.setFileData(temp);

				total_block = diyDataModel.getTotalBlock();
				index_block = diyDataModel.getIndexBlock();
				// 图片文件名
				String diy_filename = filename.substring(filename.lastIndexOf('/') + 1);

				diyDataModel.setFullFileName(diy_filename);
			}
		} catch (Exception ex) {
			//            System.out.println("ee"+ex.getMessage());
			Object[] obj = { CPProperty.ERROR_NO_FILE1[language] };
			handleEvent(EventID.EVENT_ALERT, obj);
			return false;
		}
		return true;
	}

	/** 初始化DIY模型 */
	public void initDiyModel() {
		diyDataModel.init();
		index_block = 0;
		total_block = 0;
	}

	/** 设置DIY号码 ? */
	public void setDiyNum(byte[] b) {
		diyDataModel.setNumberData(b);
		//        this.diy_num=b;
	}

	/** 设置问候语 */
	public void setDiyMsg(byte[] b) {
		firstDiy = true;
		diyDataModel.setMsgData(b);
		//        this.diy_msg=b;
	}

	/** 组织问候语的协议包 */
	public void combineGreetingMsgBody() {
		System.out.println("GUIController:combineGreetingMsgBody()");
		firstDiy = false;
		protocolTools.reset();
		protocolTools.setMessageTypeValue((short) ProtocolTools.VALUE_MESSAGETYPE_MODIFY_GREETING);

		protocolTools.add(StringTools.int2ByteArr(ProtocolTools.PROPERTY_PHONENUMBER));//0x0008
		protocolTools.add(StringTools.int2byte(myPhoneNum.length()));
		protocolTools.add(myPhoneNum.getBytes());

		protocolTools.add(StringTools.int2ByteArr(ProtocolTools.PROPERTY_GREETING));//0x8004

		//        byte[] diy_img = diyDataModel.getFileData();
		byte[] diy_num = diyDataModel.getNumberData();
		byte[] diy_msg = diyDataModel.getMsgData();

		//        System.out.println("file id 2: "+diyDataModel.getFileID());

		int datalen = 0;
		byte[] temp = StringTools.int2byte(datalen);

		if (diy_num != null && diy_num.length > 0) {
			byte[] pnumdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_GREETING_NUMBER);
			byte[] numberlen = StringTools.int2byte(diy_num.length);
			diy_num = StringTools.linkbytes(pnumdata, numberlen, diy_num);

			temp = StringTools.linkbytes(temp, diy_num);
		}
		if (diy_msg != null) {
			byte[] pmsgdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_GREETING_MSG);
			byte[] msglen = StringTools.int2byte(diy_msg.length);
			diy_msg = StringTools.linkbytes(pmsgdata, msglen, diy_msg);

			temp = StringTools.linkbytes(temp, diy_msg);
		}

		datalen = temp.length - 4;

		temp = StringTools.replacebytes(temp, StringTools.int2byte(datalen), 0);

		protocolTools.add(temp);

		byte[] body = protocolTools.getHeader();
		//        for(int i=0;i<body.length;i++){
		//            System.out.print(body[i]+",");
		//        }

		gprsConnection = new GPRSConnection(ccCanvas, initSystemModel.getComuUrl(), body);
		gprsConnection.start();
	}

	/** 组织DIY的协议包 */
	public void combineDiyBody(int total_block, int block_id, byte[] block) {
		System.out.println("GUIController:combineDiyBody()");
		firstDiy = false;

		protocolTools.reset();
		protocolTools.setMessageTypeValue((short) ProtocolTools.VALUE_MESSAGETYPE_DIY);

		protocolTools.add(StringTools.int2ByteArr(ProtocolTools.PROPERTY_PHONENUMBER));//0x0008
		protocolTools.add(StringTools.int2byte(myPhoneNum.length()));
		protocolTools.add(myPhoneNum.getBytes());

		protocolTools.add(StringTools.int2ByteArr(ProtocolTools.PROPERTY_CLIENT_LANGUAGE));//0x0021
		protocolTools.add(StringTools.int2byte(((ProtocolTools.VOLUE_CLIENT_LANGUAGE[language]).getBytes()).length));
		protocolTools.add((ProtocolTools.VOLUE_CLIENT_LANGUAGE[language]).getBytes());

		protocolTools.add(StringTools.int2ByteArr(ProtocolTools.PROPERTY_DIY));//0x8006

		String name = diyDataModel.getFileName();
		String type = diyDataModel.getFileType();
		int totalLen = diyDataModel.getTotalLength();
		//        byte[] diy_img = diyDataModel.getFileData();
		byte[] diy_num = diyDataModel.getNumberData();
		byte[] diy_msg = diyDataModel.getMsgData();
		String diy_fileId = diyDataModel.getFileID();

		//        System.out.println("file type : "+type);

		if (diy_fileId == null) {
			StringBuffer sb = new StringBuffer();
			sb.append(myPhoneNum);
			sb.append(Math.abs(ran.nextInt()) % 1000);
			diyDataModel.setFileID(sb.toString());
			diy_fileId = diyDataModel.getFileID();
		}
		//        System.out.println("file id 2: "+diyDataModel.getFileID());

		int datalen = 0;
		byte[] temp = StringTools.int2byte(datalen);

		//        System.out.println("set num ");

		if (diy_num != null && diy_num.length > 0) {
			//            System.out.println("set num ");
			//			 加入diy_num
			byte[] pnumdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_DIY_NUMBER);
			byte[] numberlen = StringTools.int2byte(diy_num.length);
			diy_num = StringTools.linkbytes(pnumdata, numberlen, diy_num);

			temp = StringTools.linkbytes(temp, diy_num);
		}
		//        System.out.println("set msg ");
		if (diy_msg != null && diy_msg.length > 0) {
			// 加入问候语
			byte[] pmsgdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_DIY_MESSAGE);
			byte[] msglen = StringTools.int2byte(diy_msg.length);
			diy_msg = StringTools.linkbytes(pmsgdata, msglen, diy_msg);

			temp = StringTools.linkbytes(temp, diy_msg);
		}
		//        System.out.println("set block ");
		if (totalLen > 0) {
			// 加入文件数据长度
			byte[] pdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_DIY_FILESIZE);
			byte[] len = StringTools.int2byte((Integer.toString(totalLen)).length());
			byte[] fiddata = StringTools.linkbytes(pdata, len, (Integer.toString(totalLen)).getBytes());

			temp = StringTools.linkbytes(temp, fiddata);
		}

		//            diy_fileid = Integer.parseInt(sb.toString());
		if (diy_fileId.length() > 0) {
			// 加入fileID
			byte[] pdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_DIY_FILEID);
			byte[] len = StringTools.int2byte((diyDataModel.getFileID()).length());
			byte[] fiddata = StringTools.linkbytes(pdata, len, (diyDataModel.getFileID()).getBytes());

			temp = StringTools.linkbytes(temp, fiddata);
		}

		if (total_block > 0) {
			// 加入块长度
			byte[] pdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_DIY_FILE_TOTALBLOCK);
			byte[] len = StringTools.int2byte(((Integer.toString(total_block)).getBytes()).length);
			byte[] tbdata = StringTools.linkbytes(pdata, len, (Integer.toString(total_block)).getBytes());

			temp = StringTools.linkbytes(temp, tbdata);
		}

		if (block_id > 0) {
			byte[] pdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_DIY_FILE_INDEXBLOCK);
			byte[] len = StringTools.int2byte(((Integer.toString(block_id)).getBytes()).length);
			byte[] bidata = StringTools.linkbytes(pdata, len, (Integer.toString(block_id)).getBytes());

			temp = StringTools.linkbytes(temp, bidata);
		}
		if (name != null) {
			try {
				byte[] namebyte = name.getBytes(CPProperty.UTF8);
				byte[] pdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_DIY_FILENAME);
				byte[] len = StringTools.int2byte(namebyte.length);
				byte[] namedata = StringTools.linkbytes(pdata, len, namebyte);

				temp = StringTools.linkbytes(temp, namedata);
			} catch (Exception e) {
			}
		}
		if (type != null) {
			byte[] pdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_DIY_FILETYPE);
			byte[] len = StringTools.int2byte(type.length());
			byte[] typedata = StringTools.linkbytes(pdata, len, type.getBytes());

			temp = StringTools.linkbytes(temp, typedata);
		}

		if (block != null) {
			byte[] pimgdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_DIY_FILE_BLOCKSIZE);
			byte[] imglen = StringTools.int2byte(((Integer.toString(block.length)).getBytes()).length);
			byte[] sizedata = StringTools.linkbytes(pimgdata, imglen, (Integer.toString(block.length)).getBytes());

			temp = StringTools.linkbytes(temp, sizedata);

			byte[] ftpdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_DIY_TOTAL_FILE);
			byte[] ftlen = StringTools.int2byte(1);
			byte[] ftdata = StringTools.linkbytes(ftpdata, ftlen, "1".getBytes());

			temp = StringTools.linkbytes(temp, ftdata);

			byte[] fipdata = StringTools.int2ByteArr(ProtocolTools.PROPERTY_DIY_INDEX_FILE);
			byte[] filen = StringTools.int2byte(1);
			byte[] fidata = StringTools.linkbytes(fipdata, filen, "1".getBytes());

			temp = StringTools.linkbytes(temp, fidata);

			//            byte[] pdata = StringTools.int2ByteArr2(HttpTools.PROPERTY_DATASTREAM);
			//            byte[] len = StringTools.int2byte(block.length);
			//            block = StringTools.linkbytes(pdata, len, block);

			//            temp = StringTools.linkbytes(temp, block);
		}

		datalen = temp.length - 4;

		temp = StringTools.replacebytes(temp, StringTools.int2byte(datalen), 0);

		protocolTools.add(temp);
		if (block != null) {
			//             System.out.println("block : "+block.length);
			protocolTools.add(StringTools.int2ByteArr(ProtocolTools.PROPERTY_DATASTREAM));//0x0021
			protocolTools.add(StringTools.int2byte(block.length));
			protocolTools.add(block);
		}

		byte[] body = protocolTools.getHeader();
		//        for(int i=0;i<body.length;i++){
		//            System.out.print(body[i]+",");
		//        }

		gprsConnection = new GPRSConnection(ccCanvas, initSystemModel.getComuUrl(), body);
		gprsConnection.start();
	}

	//    public void cancelGprs(){
	//        gprsConnection.cancel();
	//        eventStack.removeAllElements();        
	//    }

	/** 注销一个动态连接(短信监控) */
	public void UnRegisterPush(final String connURL) {
		Thread registerThread = new Thread() {

			private String m_sRegClassName = "";

			public void setClassName(String className) {
				m_sRegClassName = className;
			}

			public void run() {
				try {
					String MIDletStr = "com.winksi.j2me.cc.midlet.CCMIDlet";
					String FilterStr = "*";
					PushRegistry.unregisterConnection(connURL);
				} catch (IllegalArgumentException iae) {

				} catch (SecurityException se) {
					handleEvent(GUIController.EventID.EVENT_EXIT, null);
				}
			}
		};
		registerThread.start();
	}

	/** 注册一个动态连接(等待服务器返回激活短信(判断是否为本机号)) */
	public void registerPush(final String connURL) {
		System.out.println("GUIController:RegisterPush(): " + connURL);
		Thread registerThread = new Thread() {

			private String m_sRegClassName = "";

			public void setClassName(String className) {
				m_sRegClassName = className;
			}

			public void run() {
				try {
					String MIDletStr = "com.winksi.j2me.cc.midlet.CCMIDlet";
					String FilterStr = "*";
					PushRegistry.registerConnection(connURL, MIDletStr, FilterStr);
				} catch (IllegalArgumentException iae) {

				} catch (ClassNotFoundException cnf) {

				} catch (IOException ioe) {

				} catch (SecurityException se) {
					handleEvent(GUIController.EventID.EVENT_EXIT, null);
				}
				System.out.println("GUIController:RegisterPush: register push success");
			}
		};
		System.out.println("GUIController:RegisterPush: start a new thread to register push");
		registerThread.start();
	}

	/** 比较当前电话簿与rms中保存的号码图片对应关系, 得到新添加的号码 */
	public void comparePhonebookAndRms() {
		if (!rmsAdapter.isNotExist(CPProperty.RECORDID_NumAndImage)) {// 如果rms中存在 号码图片对应关系
			getRmsNumAndImage();
			updatedNumbers = PimCompareTools.compareHashtable(numAndImageHt, rmsHt);
		}
	}

	/** 程序第一次启动保存一份号码图片对应关系 */
	public void backupNumAndImgPairs() {
		if (rmsAdapter.isNotExist(CPProperty.RECORDID_NumAndImage)) {
//			System.out.println("back up contact");
			saveRmsContactsList(numAndImageHt);
			getRmsNumAndImage();

			Hashtable tempht = new Hashtable();
			Enumeration e = numAndImageHt.keys();
			while (e.hasMoreElements()) {
				String number = (String) e.nextElement();
				tempht.put(number, "");
			}
		}
	}

	/** 初始化电话簿数据 */
	public void initPim() {
		numAndImageHt = pimModel.getContactHashtable();

		comparePhonebookAndRms();

		pimInited = true;
		fileModel = new FileModel(this);
		//        fileModel.setCurrDirName(CPProperty.DIYPATH_C);
		//        fileModel.start();

		//        fileModel1 = new FileModel(this);
		//        fileModel1.setCurrDirName(CPProperty.DIYPATH_E);
		//        fileModel1.start();

		// 
		initEvent();
	}

	public boolean isInitPim() {
		return this.pimInited;
	}

	/** 设置是否为无视图的处理(非用户主动打开软件) */
	public void setIsPushWakeUp(boolean b) {
		this.isPushWakeup = b;
	}

	public boolean getIsPushWakeup() {
		return isPushWakeup;
	}

	public void setIsInitOk(boolean b) {
		this.isIniTok = b;
	}

	public void setCurrentFileName(String name) {
		this.ccCanvas.setCurFileName(name);
	}

	/** 到问候语设置 */
	public void go2Greeting() {
		this.ccCanvas.go2Greeting();
	}

	/** 到选择号码 */
	public void go2Number() {
		this.ccCanvas.go2Number();
	}

	public String getCurFileName() {
		return ccCanvas.getCurFilename();
	}

	public boolean getIsInitOk() {
		return this.isIniTok;
	}

	//    public void restartGprs(){
	//        if (gprsConnection != null&&gprsConnection.isRunning()) {
	//            gprsConnection.start();
	//        }
	//    }

	public void initUpdataNumber() {
		updatedNumbers.removeAllElements();
		updatedNumbers = null;
	}
}
