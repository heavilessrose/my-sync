package com.pica.msn;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Timer;
import java.util.Vector;
import android.app.Activity;
import android.app.Application;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.res.Resources;
import android.database.Cursor;
import android.graphics.drawable.Drawable;
import android.media.MediaPlayer;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Handler;
import android.os.Message;
import android.os.Process;
import android.os.Vibrator;
import android.provider.Settings;
import android.util.DisplayMetrics;
import android.util.Log;

import com.pica.ui.tools.Animation;
import com.pica.ui.tools.AnimationListener;
import com.pica.ui.tools.EmotionTextView;


public class MSNApplication extends Application implements ParserNotifier,EventListener, AnimationListener{
	private static MSNApplication app;
	public DatabaseHelper DB;
	public static final String DB_NAME = "MsnDB.db";
	public static final int DB_VERSION = 1;
	/**用户账号*/
	private String strLiveID;
	/**用户密码*/
	private String strPassWord;
	
	private String strEditName;
	private String strDomain;
	private String strIMSI = "";
	/**picaid 包括@pica*/
	private String strPicaID;
	private String strPicaPW;
	
	public Handler LMHandler = null;
	public Handler MainHandler= null;
	public Handler chatroomHandler = null;
	public Handler loginHandler = null;
	public Handler searchRosterHander = null;
	
	private String msn_serverIP = null;
	private String msn_serverPORT = null;
	
	public int mCurActivityIdx = 0;
	public int Splash_Activity = 0;
	public int Login_Activity = 1;
	public int LoginM_Activity = 2;
	public int Main_Activity = 3;
	public int Chat_Activity = 4;
	public int Register_Activity = 5;
	public int Screen_Off_Timeout = -1;
	public String strText;
	public InputStream input;
	public Socket socket;
	public String message;

	public int iPackAge = -1;
	public String strElement = null;

	public String error = null;
	private MediaPlayer mMediaPlayer;
	public ScreenManager screenManager = null;
	public NotificationManager mNotificationManager;
	public static Activity mActivity;
	public short[] agent = new short[6];//类wap头用上
	public boolean isPause = false;
	public Vector jids = new Vector();
	public ConnectivityManager conMgr = null;
	public boolean bRefoot = false;
	public boolean bChatAdClick = false;
	public byte[] CameraByte = null;
	public Drawable logo;
	public Drawable offline_block;
	public Drawable online_block;
	public Drawable online;
	public Drawable busy;
	public Drawable away;
	public Drawable offline;
	//public byte[] rcv_bytes = null;
	public int groupIdx_item = -1;
	public int childIdx_item = -1;

	public boolean bZwpDialog = false;
	public boolean bZwpLoad = true;
	
	final public static String SCHEME_MAIL = "mailto:";
	final public static String SCHEME_TEL = "tel:";
	final public static String SCHEME_SMS = "sms:";
	final public static String SCHEME_ADDBUDDY = "addbuddy:";

	final public static String STR_http="http://";
	final public static int STR_http_length=7;//http://的长度
	final public static String STR_httpS="https://";
	final public static int STR_https_length=8;//https://的长度
	
	public Vector baseUrls;
	public Vector htmlStrings;
	public Vector urls;
	//保存表单提交按钮中的url，method对（在ZwpView中的shouldOverrideUrlLoading使用）
	public Map<String, String> urlAndMethod;
	

	/**解决进程被杀死的bug(已经重新登录，在其他的Activity的oncreate中就不要再次处理)*/
    public boolean killedToRelogin = false;
    
    /**标志程序是否启动（避免重新启动一个程序，在splashActivity中使用）*/
	public boolean isStart = false;
	public boolean isStartLogin = false;

	public float screenScale;
	public byte Screen_type = 0x02;
	public static final byte SCREEN_Q = 0x01; 
	public static final byte SCREEN_H = 0x02; 
	public static final byte SCREEN_W = 0x03; 
	
	/**保存在rms里面的识别串*/
	private String registerIdentifier=null;
	/**注册用户名 密码*/
	public String registerUsername=null,registerPassword=null;
	public static String REGISTER_SERVER="reg1.msn.pica.com:8082";//运营
//	public static String REGISTER_SERVER = "221.99.191.239:8020";//测试
	
    public void onCreate() {
    	super.onCreate();

    	//groupData = new ArrayList<Map<String, Group>>();
    	groupData = new Vector<Group>();
    	//childData = new ArrayList<List<Map<String, Contact>>>();
    	
    	
        //groupDataNormalOnlyOnline= new ArrayList<Map<String, Group>>();
    	groupDataNormalOnlyOnline= new Vector<Group>();
    	//childDataNormalOnlyOnline= new ArrayList<List<Map<String, Contact>>>();
    	sessionData= new ArrayList<Map<String, Item>>();
    	//////////////////增加按状态排序功能
    	groupDataNormalOnlyOnline = new Vector<Group>();
    	//childDataNormalOnlyOnline= new ArrayList<List<Map<String, Contact>>>();
    	
    	//////////////////增加按状态排序功能
    	groupDataByStatus = new Vector<Group>();    	
    	groupDataStatusOnlyOnline = new Vector<Group>();
    	//////////////////    	
    	baseUrls = new Vector();
    	htmlStrings = new Vector();
    	urls = new Vector();
    	//保存表单提交按钮中的url，method对（在ZwpView中的shouldOverrideUrlLoading使用）
    	urlAndMethod = new HashMap<String, String>();

    	eventListener=this;
    	screenManager = new ScreenManager();
    }
    
    public void pushMain(Activity ac)
    {
        app.screenManager.setMain(ac);
    }
    public void pushChat(Activity ac)
    {
        app.screenManager.setChat(ac);
    }
    
    public void popMain()
    {
        app.screenManager.popMain();
    }
    public void popChat()
    {
        app.screenManager.popChat();
    }

    public void onTerminate() {
    }
    
    private MSNApplication()
    {
        //////////
        EmotionTextView.initImage();
        //////////
    }
    
    public static MSNApplication getApplication(Activity activity) {
        // TODO should this be synchronized?
    	if (app == null) 
        {
            initialize(activity);
        }
        mActivity = activity;
        return app;
    }
    
    public static MSNApplication getApplication() {
    	if (app == null) 
    	{
    		app=new MSNApplication();
    	}
        return app;
    }
    
    private static void initialize(Activity activity)
    {
    	app = new MSNApplication();
        app.onCreate();
        app.screenScale = activity.getResources().getDisplayMetrics().density;
        if(app.screenScale == 1.5)
        {
            app.Screen_type = SCREEN_W;
        }
        else if(app.screenScale == 1)
        {
            app.Screen_type = SCREEN_H;
        }
        else if(app.screenScale == 0.75)
        {
            app.Screen_type = SCREEN_Q;
        }  
    }
    
    public void initBitmap()
    {
        mNotificationManager = (NotificationManager)mActivity.getSystemService(NOTIFICATION_SERVICE);        
		Resources r = mActivity.getResources();      
        logo = r.getDrawable(R.drawable.icon);
        offline_block = r.getDrawable(R.drawable.offline_block);
        online_block = r.getDrawable(R.drawable.online_block);
        online = r.getDrawable(R.drawable.online);
        busy = r.getDrawable(R.drawable.busy);
        away = r.getDrawable(R.drawable.away);
        offline = r.getDrawable(R.drawable.offline);
    }

    public void SetCurActivity(Activity activity)
    {
    	mActivity = activity;
    }

    /**用户自己的  包括后缀的email帐号 密码*/
    public void SetLiveId(String id, String pw)
    {
    	strLiveID = id;
    	strPassWord = pw;
    }
    
    public void SetEditAccount(String name, String domain)
    {
    	strEditName = name;
    	strDomain = domain;
    }
    
    public void setDoMain(String domain){
    	strDomain = domain;
    }
    
    public String GetEditName()
    {
    	return strEditName;
    }
    
    public String GetDomain()
    {
    	return strDomain;
    }
    
    public void SetPicaId(String id)
    {
    	strPicaID = id;
    }
    public void SetPicaPW(String pw)
    {
    	strPicaPW = pw;
    }
    public void SetLoginStatus(String idx)
    {
    	iLoginStatus = idx;
    	if(idx.equals("0"))
    	{
    		sStatus = "online";
    	}
    	else if(idx.equals("1"))
    	{
    		sStatus = "busy";
    	}
    	else if(idx.equals("2"))
    	{
    		sStatus = "berightback";
    	}
    	else if(idx.equals("3"))
    	{
    		sStatus = "away";
    	}
    	else if(idx.equals("4"))
    	{
    		sStatus = "onthephone";
    	}
    	else if(idx.equals("5"))
    	{
    		sStatus = "outtolunch";
    	}
    	else if(idx.equals("6"))
    	{
    		sStatus = "offline";
    	}
    }
    
    public void SetIMSI(String id)
    {
    	strIMSI = id;
    }
    public void SetLMHandler(Handler h)
    {
    	LMHandler = h;
    }
    public void SetMainHandler(Handler h)
    {
    	MainHandler = h;
    }    
    
    public void setLoginHandler(Handler h)
    {
    	loginHandler=h;
    }
    
    public void setChatroomHandler(Handler h)
    {
    	chatroomHandler=h;
    }
    
    public void setSearchRosterHander(Handler h)
    {
    	searchRosterHander = h;
    }
    
    public void SetMSN_SERVER(String ip, String port)
    {
    	msn_serverIP = ip;
    	msn_serverPORT = port;
    }
    
    public void setConMgr(ConnectivityManager mGr)
    {
        this.conMgr = mGr;
    }
    
    public String getVersion()
    {
    	return VERSION;
    }
    
    public String GetIMSI()
    {
    	return strIMSI;
    }  
    
    /**用户自己的  包括后缀的email帐号 相当于老接口的allUsername*/
    public String GetLiveID()
    {
    	return strLiveID;
    }   
    
    /**用户自己的  email密码*/
    public String GetPassWord()
    {
    	return strPassWord;
    }
    
	/**picaid 包括@pica*/
    public String GetPicaID()
    {
    	return strPicaID;
    }   
    public String GetPicaPW()
    {
    	return strPicaPW;
    }
    public Handler GetMSNHandler()
    {
    	return MSNHandler;
    }
    public String GetMSN_IP()
    {
    	return msn_serverIP;
    }   
    public String GetMSN_PORT()
    {
    	return msn_serverPORT;
    }
    
    public void InitWapAgent(int winHeigth, int winWidth, int wapHeight, int wapWidth)
    {
    	app.agent[0] = (short)winWidth;
    	app.agent[1] = (short)(winHeigth - 100);
    	app.agent[2] = (short)wapWidth;
    	app.agent[3] = (short)wapHeight;
    	app.agent[4] = (short)10;
    	app.agent[5] = (short)18;
    }
    

    
    //////////////////////////////////////////////// yangmin 新增
    /**按分组排序 显示所有联系人 分组列表 String="GROUP" Item装载的是group的哈希*/
	//public List<Map<String, Group>> groupData;
    public Vector<Group> groupData;
	/**按分组排序 显示所有联系人 等同分组的联系人列表  索引同分组列表*/
	//public List<List<Map<String, Contact>>> childData;
	
    /**按分组排序 仅显示在线联系人 分组列表 String="GROUP" Item装载的是group的哈希*/
	//public List<Map<String, Group>> groupDataNormalOnlyOnline;
	public Vector<Group> groupDataNormalOnlyOnline;
	/**按分组排序 仅显示在线联系人 等同分组的联系人列表  索引同分组列表*/
	//public List<List<Map<String, Contact>>> childDataNormalOnlyOnline;
	
	//////////////////增加按状态排序功能
	/**按状态排序 显示在线联系人和脱机联系人 分组列表 String="GROUP" Item装载的是group的哈希*/	
	public Vector<Group> groupDataByStatus;
	/**按状态排序 只显示在线联系人 分组列表 String="GROUP" Item装载的是group的哈希*/	
	public Vector<Group> groupDataStatusOnlyOnline;
	///////////
	
	/**会话列表 String="SESSION" Item装载的是session的哈希*/
	public List<Map<String,Item>> sessionData;
    
    public static String MSN_DOMAIN = "msn.pica";
    public static String CLUSTER_DOMAIN = "@cluster.pica";
	public static String MSN_CHINA_DOMAIN="@msn.cn";
	/**
	 * SUBITEM_STATE 状态：联机
	 */
	public static final byte STATE_ONLINE = 0x00;
	/**
	 * SUBITEM_STATE 状态：忙碌
	 */
	public static final byte STATE_BUSY = 0x01;
	/**
	 * SUBITEM_STATE 状态：离开
	 */
	public static final byte STATE_AWAY = 0x02;
	/**
	 * SUBITEM_STATE 状态：马上回来
	 */
	public static final byte STATE_BeRightBack=0x03;
	/**
	 * SUBITEM_STATE 状态：通话中
	 */
	public static final byte STATE_InACall=0x04;
	/**
	 * SUBITEM_STATE 状态：外出就餐
	 */
	public static final byte STATE_OutToLunch=0x05;
	/**
	 * SUBITEM_STATE 状态：离线
	 */
	public static final byte STATE_OFFLINE = 0x06;


	/**与状态协议相关的字符串*/
	public static final String[] STATE_FLAG_All={"online","busy","away","berightback","onthephone","outtolunch","offline"};

	/**发包操作时候 删除联系人*/
	public static final String CONTACT_DELETE="delete";
	/**发包操作时候 删除并阻止联系人*/
	public static final String CONTACT_DELETE_BLOCK="rmblk";
	/**发包操作时候 阻止联系人*/
	public static final String CONTACT_BLOCK="block";
	/**发包操作时候 解除阻止联系人*/
	public static final String CONTACT_UNBLOCK="unblock";
    
    /**客户端能力*/
	public static final String CLIENT_CAPABILITY = "0000003F";//支持多人对话  支持语音和文件传输 支持头像 支持nudge和本地分组

	/**客户端dispatch服务器地址*/


	public static String dispServer = "disp1.msn.pica.com";//运营服务
//	/**客户端dispatch服务器地址 端口号*/
	public static String dispServerPort = "22246";//运营服务器

//	/**客户端dispatch服务器地址*/
//	public static String dispServer = "211.99.191.239";//测试服务器
//	/**客户端dispatch服务器地址 端口号*/
//	public static String dispServerPort = "32246";//测试服务器

	/**客户端版本号 发送给服务器的*/
//	public static String VERSION="5.004.1053.cn";//TCL内置
	public static String VERSION="5.004.1088.cn";//推广渠道“酷站收录”的版本
//	public static String VERSION="5.004.1021.cn";//索爱内置
//	public static String VERSION="5.004.1018.cn";//大陆线上下载版PC官网
//	public static String VERSION="5.004.1058.cn";//大陆地面推广版WAP官网
//	public static String VERSION="5.004.1048.cn";//三星内置版本 
	/**发给服务器的客户端来源*/
	public static final String SOURCE = "msn";
	
	public static final String VERSION_ABOUT="5.0.0 Beta";
	public static final String EMAIL_ABOUT="messenger@pica.com";
	
	/**用户的pica号 临时登录用的 群成员判断自己用*/
	public String picaNumber=null;//picaAllID删掉@后缀得到的Pica号码
	/**自己头像显示的像素大小-整数*/
	public static final String PROFILE_PORTAITSIZE="64";
	/**联系人头像显示的像素大小-整数*/
	public static final String CONTACT_PORTRAITSIZE="64";

	//public static String text_group_default="其他聯絡人";
	/**
	 * 默认分组名、ID与gbcode
	 */
	public static  String DEFAULT_GROUP_ID = "0";
	public static  String DEFAULT_GROUP_GBC = "e585b6e4";
	public static  String DEFAULT_GROUP_NAME ="";
	
	/**
	 * 联系人类型标志位：普通，阻止，陌生人
	 * item.CONTACT_contactFlag
	 */
	public static final byte CONTACT_NORMAL_FLAG= 0x00, //普通联系人
			CONTACT_BLOCK_FLAG = 0x01, //阻止的联系人
			CONTACT_STRANGER_FLAG = 0x02; //陌生人
	
	/**
	 * 组标志位  
	 * item.GROUP_flag; 用户自定义分组
	 */
	public static final byte GROUP_USER_FLAG = 0x02; 
	/**
	 * 组标志位  
	 * item.GROUP_flag; 聊天室分组
	 */
	public static final byte GROUP_CHATROOM_FLAG=0x03;
	/**
	 * 组标志位  
	 * item.GROUP_flag; 频道分组
	 */
	public static final byte GROUP_CHANNEL_FLAG=0x04;
	/**
	 * 组标志位  
	 * item.GROUP_flag; 联系人页脚广告
	 */
	public static final byte GROUP_ROSTER_AD_FLAG=0x05;
	

	/**
	 * 消息类型 单聊消息
	 */
	public static final byte MESSAGE_TYPE_CHAT = 0x01; 
	
	//#ifdef support_groupmessage
	/**
	 * 消息类型 群聊消息
	 */
	public static final byte MESSAGE_TYPE_GROUP_CHAT = 0x02; 
	//#endif
	
	/**
	 * 消息类型 系统消息
	 */
	public static final byte MESSAGE_TYPE_SYSTEM = 0x03; 
	/**
	 * 消息类型 群消息
	 */
	public static final byte MESSAGE_TYPE_CLUSTER = 0x04; 
	/**
	 * 文件传输的消息
	 */
	public static final byte MESSAGE_TYPE_FILE_TRANSFER=0X05;
	/**
	 * 语音剪辑的消息
	 */
	public static final byte MESSAGE_TYPE_VOICECLIP=0x06;
	
	
	///////////////
	//传送消息的类型
	/**消息附件的文件状态 发送：等待发送，等待中拒绝，正在发送，发送完成，发送中取消，发送中异常<br>
	 * 接收：等待接收，等待中拒绝，正在接收，接收完毕，接收中取消，接收中异常*/
	
	/**等待发送*/
	public static final byte MESSAGE_FILE_TYPE_SEND_WAITING=0x01;
	/**等待发送中 被对方拒绝*/
	public static final byte MESSAGE_FILE_TYPE_SEND_WAITING_REFUSED=0x02;
	
	/**正在发送*/
	public static final byte MESSAGE_FILE_TYPE_SEND_SENDING=0x03;
	/**发送完成*/
	public static final byte MESSAGE_FILE_TYPE_SEND_COMPLETE=0x04;
	/**发送中取消(主动或被动)*/
	public static final byte MESSAGE_FILE_TYPE_SEND_CANCEL=0x05;
	/**发送中异常*/
	public static final byte MESSAGE_FILE_TYPE_SEND_EXCEPTION=0x06;
	
	/**等待接收*/
	public static final byte MESSAGE_FILE_TYPE_RECEIVE_WAITING=0x07;
	
	/**等待接收中 被对方取消*/
	//public static final byte MESSAGE_FILE_TYPE_RECEIVE_WAITING_CANCELED=0x08;
	
	/**等待中自己拒绝*/
	public static final byte MESSAGE_FILE_TYPE_RECEIVE_WAITING_REFUSE=0x09;
	/**正在接收*/
	public static final byte MESSAGE_FILE_TYPE_RECEIVE_SENDING=0x10;
	/**接收完毕*/
	public static final byte MESSAGE_FILE_TYPE_RECEIVE_COMPLETE=0x11;
	/**接收中取消*/
	public static final byte MESSAGE_FILE_TYPE_RECEIVE_CANCEL=0x12;
	/**接收中异常*/
	public static final byte MESSAGE_FILE_TYPE_RECEIVE_EXCEPTION=0x13;
	
	/////////////////
	
	//传送声音的类型
	/**正在录音中*/
	public static final byte MESSAGE_VOICE_TYPE_RECORDING=0x00;
	
	/**录音完毕 带内发送完毕*/
	public static final byte MESSAGE_VOICE_TYPE_INNER_SEND_COMPLETE=0x01;
	
	/**带外发送完毕*/
	public static final byte MESSAGE_VOICE_TYPE_OUTER_SEND_COMPLETE=0x02;
	
	/**接收到一个语音剪辑 带内*/
	public static final byte MESSAGE_VOICE_TYPE_RECEIVE_OVER=0x03;
	
	/**接收完毕 带外*/
	public static final byte MESSAGE_VOICE_TYPE_OUTER_RECEIVE_COMPLETE=0x04;
	
	/**录音完毕 带外正在发送*/
	public static final byte MESSAGE_VOICE_TYPE_OUTER_SEND_SENDING=0x05;
	
	/**带外 接收等待 喇叭下面仅有“接收”按钮*/
	public static final byte MESSAGE_VOICE_TYPE_OUTER_RECEIVE_WAITING=0x06;
	
	/**带外 正在接收*/
	public static final byte MESSAGE_VOICE_TYPE_OUTER_RECEIVE_SENDING=0x07;
	
	/**带外 发送取消*/
	public static final byte MESSAGE_VOICE_TYPE_SEND_CANCEL=0x08;
	/**带外 接收取消*/
	public static final byte MESSAGE_VOICE_TYPE_RECEIVE_CANCEL=0x09;
	/**带外 接收 异常*/
	public static final byte MESSAGE_VOICE_TYPE_OUTER_RECEIVE_EXCEPTION=0x10;
	/**带外 发送 异常*/
	public static final byte MESSAGE_VOICE_TYPE_OUTER_SEND_EXCEPTION=0x11;
	
	/////////////
	/**带外 发送头像 取消*/
	public static final byte MESSAGE_HEAD_TYPE_SEND_CANCEL=0x20;
	
	/**带外 发送头像 异常*/
	public static final byte MESSAGE_HEAD_TYPE_OUTER_SEND_EXCEPTION=0x21;
	
	/**带外 发送头像 完毕*/
	public static final byte MESSAGE_HEAD_TYPE_OUTER_SEND_COMPLETE=0x22;
	
	// 个人资料
	//****************************** 
	/**个人资料 昵称*/
	public String myVCARD_myNickname;
	/**个人资料 心情短语*/
	public String myVCARD_myImpresa;
	/**个人资料 个人资料resourceid*/
	public String myVCARD_resourceId;
	
	 //#ifdef support_head_image
	/**个人头像的字节长度*/
	 public int myVCARD_headImageDataLength;//个人图片字节数长度
	/**个人头像图片数据*/
	 public byte[] myVCARD_headImageData;//图片数据
	 //#endif
	
	/**用户是否选择退出程序*/
	public boolean isExit=false;
	/**用户是否选择注销*/
	public boolean isLogout=false;
	
	/**是否同步 如果同步了 不再添加数据到记录里面*/
	public boolean isSynchronize=false;
	
	/**用户本人是否登录成功 主要用于收到上线状态包的判断 <br>
	 * 另外 可用于收到错误时候的处理 如果是登录等待情况下 就要退到登录页面*/
	public boolean isLoginSuccess=false;
	
	/**jabber对象*/
	public Jabber jabber;
	
	/**自己的个人资料版本*/
    //public String myVcardVersion;
    /**联系人以及分组版本*/
    public String myContactGroupVersion; //  //自己的联系人以及分组版本  |||
    /**初始化从RMS里面得到的版本号   */
    public String myFirstContactGroupVersion;//初始化从RMS里面得到的版本号
    
    /**联系人上线时候 是否有声音*/
	public boolean isOpenOnlineSound=true;
	/**收到新消息时候 是否有声音*/
	public boolean isOpenNewMessageSound=true;
	

	/**获取联系人图片*/
	//public boolean isGetRosterIcon=true;
	/**事件监听器对象*/
	private EventListener eventListener;
	/**解析器对象*/
	public Parser parser;
	
	/**装载请求验证的好友的向量*/
	public Vector verifyFriendVector=new Vector();
	
	/**用户本人所处的状态*/
	private byte myState;
	
	/**当前会话id 客户端生成 每次递增1*/
	private int maxSid = 0;
	
	/**设备id*/
	public String devicedId;
	
	
	/**当前打开的对话*/
	public Item currentSession;
	
	/**notify 对话*/
	public Item notifySession = null;
	
	/**是否自动登录*/
	public boolean isAutoLogin=false;
	/**是否保存密码*/
	public boolean isSavePassword=false;
	
	/**用户登陆状态*/
	public String iLoginStatus="0";
	public String sStatus="online";
	/**正在登录页面*/
	public static final byte MONITOR_LOGIN_FLAG=1;
	/**正在注销页面*/
	public static final byte MONITOR_LOGOUT_FLAG=2;
	/**正在退出页面*/
	public static final byte MONITOR_EXIT_FLAG=3;
	/**3秒时间 3000*/
	//public static final String showInfoShortTime="3000";
	/**5秒时间 5000*/
	//public static final String showInfoLongTime="5000";
	
	/**注销成功 回到登录页面*/
	public static final byte LOGOUT_SUCCESS_FLAG=1;
	/**未知的原因 微软掉线 提示网络错误*/
	public static final byte LOGOUT_UNAVALIABLE_FLAG=2;
	/**账号在其他地方登录*/
	public static final byte LOGOUT_ABALIABLE_FLAG=3;
	
	//#ifdef support_groupmessage
	/**临时群聊会话*/
	public static final byte SESSION_TYPE_GROUP_CHAT = 0x01; //临时多人聊会话
	//#endif
	
	/**单聊会话*/
	public static final byte SESSION_TYPE_CHAT = 0x02; //单聊会话     
	/**群聊天室会话*/
	public static final byte SESSION_TYPE_CLUSTER_CHAT = 0x03; //群聊天室会话
	 //#ifdef support_save_rms
	/**历史会话*/
	public static final byte SESSION_TYPE_RECORD=0x04;//历史会话
	//#endif

	
	/**有联系人上线 需要播放声音*/
	public static boolean hasContactOnline=false;
	
	////////
	/**文件大小限制*/
	public int transfer_limit=2048000;
	/**带内分包大小*/
	public int per_file_size=2048;
	/**带外上传分包大小*/
	public int per_file_outer_upload_size=2*1024;
	/**带外下载分包大小*/
	public int per_file_outer_download_size=2*1024;
	/**媒体服务器地址*/
	public String data_url=null;
	public String data_method=null;
	
	/**是否 带内 发送头像*/
	public boolean isInnerSendHead=true;
	/**是否 带内 接收头像*/
	public boolean isInnerReceiveHead=true;
	/**是否 带内 发文件*/
	public boolean isInnerSendFile=true;
	/**是否 带内 收文件*/
	public boolean isInnerReceiveFile=true;
	/**是否 带内 发语音*/
	public boolean isInnerSendVoice=true;
	/**是否 带内 收语音*/
	public boolean isInnerReceiveVoice=true;
	////////
	/**最大重连次数*/
	public static int MAX_TRY_COUNT=3;//
	
    public int windowHeight,windowWidth;
	///////////////////////////////类wap部分
	/**是否支持类wap*/
	public String zwp_homePageAddress;
	public String currentUrl;
	public String baseUrl;
	/** 类wap页面是否缓存 */
	public boolean tempSave = true;
	/** 用于设置currentUrl */
	public String tempUrl;
	////////////////////////////////
	//广告部分
	public String getAdInterval="300";//取广告时间间隔的参数 单位：秒
	public boolean isSupportChannel,//是否支持频道 如果支持广告 则支持频道
	isSupportZipWap,//是否支持类wap
	isSupportAd,//是否支持广告
	isSupportRoom,//是否支持聊天室？
	isSupportHotmail,//是否支持hotmail服务
	isSupportSpaces,//是否支持spaces服务
	isSupportSearch,//是否支持搜索服务
	isSupportSearchRoom,//是否支持查找MSN群（聊天室）
	isSupportAddFriend,//是否支持msn.cn用户添加联系人
	isSupportRoomCreate,//是否支持管理自建群
	isSupportRoomInvite;//是否支持邀请加入自建群的地址
	public static byte JABBER_REQUEST = 0x01;//普通jabber包
    public static byte ZIPWAP_REQUEST = 0x02;//类wap
    public static byte AD_LOGIN_REQUEST=0x03;//登录过程广告
    public static byte AD_ROSTER_REQUEST=0x04;//联系人页面页脚广告
    public static byte AD_CLICK_REQUEST=0x05;//广告点击统计包
    public static byte AD_MAIN_REQUEST=0x06;//主页面页脚广告
    public static byte AD_SESSION_REQUEST=0x07;//会话列表页脚广告
    public static byte AD_GET_REQUEST=0x09;//连接广告网关
    
    /**自己的头像 带外*/
    public static byte HEAD_VCARD_REQUEST=0x10;
    /**联系人的头像 带外*/
    public static byte HEAD_CONTACT_REQUEST=0x11;
    
	/**信息页面的文本广告*/
	public Vector ad_chat_Vector=null;
	/**信息页面广告索引*/
	public int ad_chat_idx = 0;
	/**联系人页面图片广告*/
	public Item adRosterItem;
	/**广告ID号 */
	public String AD_ID_chat_header="0",
		AD_ID_login_prog="0",
		AD_ID_roster_footer="0",
		AD_ID_main_footer="0",
		AD_ID_session_footer="0";
	
	/**
	 * 页脚广告条
	 * */
	public static final byte AD_IVR_FLAG = 0x00;//IVR广告
	public static final byte AD_WAP_FLAG = 0x01;//wap广告
	public static final byte AD_ZWP_FLAG = 0x02;//类wap广告
	public static final byte AD_SMS_FLAG = 0x03;//短信广告
	
	/**正常分组 有脱机人*/
	public static final byte ROSTER_NORMAL_HAS_OFFLINE=0x01;
	//#ifdef support_exchange_contact_group
	/**正常分组 无脱机人*/
	public static final byte ROSTER_NORMAL_NO_OFFLINE=0x02;
//////////////////添加按状态排序联系人
	/**状态分组 有脱机人*/
	public static final byte ROSTER_STATE_HAS_OFFLINE=0x03;
	/**状态分组 无脱机人*/
	public static final byte ROSTER_STATE_NO_OFFLINE=0x04;
	
	 /**广告组 分组ID*/
	 public static final String GROUP_AD_ID="groupadid";
	//////////////
		/**在线组 分组ID*/
	 public static final String GROUP_ONLINE_ID="grouponlineid";
	 /**脱机组 分组ID*/
	 public static final String GROUP_OFFLINE_ID="groupofflineid";
	 
	//#ifdef support_head_image
	/**流量设置 是否接收联系人头像变化 需保存到RMS中 前提是能力支持*/
	public boolean isReceiveHeadChange=true;
	//#endif
	
	/**流量设置  是否接收联系人个人信息变化 需保存到RMS中 前提是能力支持*/
	public boolean isReceiveVcardChange=true;

	//#ifdef support_exchange_contact_group
	/**是否只显示在线联系人*/
	public boolean isShowOnline=true;
	//#endif
	
	/**是否震动*/
	public boolean isNudge=true;
	
	//#ifdef support_exchange_contact_group
	private byte groupOrder=ROSTER_NORMAL_NO_OFFLINE; //当前联系人分组方式; 
	//#endif
	
	 /**保存自己头像的路径*/
	 public static final String RMS_VCARD_HEAD="/data/data/com.pica.msn/vcardhead/";
	 /**保存登录联系人头像的路径*/
	 public static final String RMS_CONTACT_HEAD="/data/data/com.pica.msn/contacthead/";
	 /**保存登录联系人数据的路径*/
	 public static final String RMS_CONTACT_DATA="/data/data/com.pica.msn/contactdata/";
	 /**保存广告图片文件*/
	 public static final String RMS_AD_DATA="/data/data/com.pica.msn/";
	/////////////
	 
		//////////// v5.5 根据dispatch返回的capability得出
		/**支持多人会话（临时群组）*/
		public boolean isSupportGroupChatByCapability;
		/**支持头像*/
		public boolean isSupportHeadImageByCapability;
		/**支持文件传送*/
		public boolean isSupportTransferFileByCapability;
		/**支持录音剪辑*/
		public boolean isSupportRecordAudioByCapability;
		/**支持接收nudge*/
		public boolean isSupportNudgeMessageByCapability;	
		/**支持本地保存联系人和分组*/
		public boolean isSupportSaveGroupAndContactByCapability;	
		/**支持接收联系人头像变化（仅指presence）*/
		public boolean isSupportReceiveHeadImageChangeByCapability;	
		/**支持接收联系人个人信息变化（仅指presence）*/
		public boolean isSupportReceiveVcardChangeByCapability;	
		/**支持离线消息*/
		public boolean isSupportOfflineMessageByCapability;	
		/**支持编辑和显示联系人昵称*/
		public boolean isSupportLocalnameByCapability;	
		////////////
	public String	capability=null;//能力

	
	/**在联系人页面 选择图片修改个人头像*/
	public static final byte SEARCHFILE_MAINROSTER_FLAG=1;
	/**在聊天页面 选择文件发送*/
	public static final byte SEARCHFILE_CHATROOM_FLAG=2;
	/**在联系人页面 选择拍照修改个人头像*/
	public static final byte CAMERA_MAINROSTER_FLAG=3;
	/**在聊天页面 选择拍照 文件发送*/
	public static final byte CAMERA_CHATROOM_FLAG=4;
	
	/**文件资源管理器 选择文件*/
	public static final byte SELECT_FILE_FROM_EXPLORE = 1;
	
	/**系统多媒体图片管理器 选择文件*/
	public static final byte SELECT_FILE_FROM_IMAGE=2;
	
	/**拍照 完毕选择文件*/
	public static final byte SELECT_FILE_FROM_TAKE_PHONE=3;

	/**当前带外传输过程中的消息对象 语音剪辑功能需要用上*/
	public Item currentOuterMessage=null;
	/**当前带外传输过程中的联系人对象 上传头像功能需要用上*/
	public Contact currentOuterContact=null;
	
	/**正在文件传送/等待传送中 （已经选好了待发送的文件或者接收对方发来的文件）*/
	//public boolean isTransfering=false;
	
	/**装载处于正在传送中的message 收发文件和语音*/
	public Vector isTransferingMessageVector;
	
	/**正在带外上传头像*/
	public boolean isTransferingOuterSendHead=false;
	/**装载处于正在传送中的联系人 收发头像*/
	public Vector isTransferingOuterSendHeadVector;
	
	/**有联系人状态变更 需要保存联系人列表*/
	//public static boolean hasContactUpdate=false;
	
	public void setWindowHeight(int height)
	{
		this.windowHeight=height;
	}
	
	public void setWindowWidth(int width)
	{
		this.windowWidth=width;
	}
	
	public void setMyState(byte state)
	{
		this.myState=state;
	}
	
	/**取得个人的状态*/
	public byte getMyState()
	{
		return myState;
	}
	
    public void setPicaNumber(String picaNumber)
    {
    	this.picaNumber=picaNumber;
    }
    
    public String getPicaNumber()
    {
    	return this.picaNumber;
    }
	

	
	/**开始启动网络*/
	public void startJabber()
	{
		jabber=null;
		jabber=new Jabber(this,this);
	}
	

	
	
	
	
	public void receiveDispatch(String result, Vector noticeVec, String force, String url, 
			String version, String id, String password, String ip, String ua, String carrier, 
			String adurl, String ad_interval, String zwp, String fpage, 
			String add_friend, String room_name, String room_search, String hotmail, String spaces, 
			String search,String roomCreateAddress,String roomInviteAddress,String transparent,
			String transfer_limit,String data_url,String data_method,String capability)
	{
		
		if(result!=null&&result.length()!=0)
		{
			jabber.result=result;
		}
		if(noticeVec!=null&&noticeVec.size()!=0)
		{
			jabber.noticeVector=noticeVec;
		}
		if(force!=null&&force.length()!=0)
		{
			jabber.force=force;
		}
		if(url!=null&&url.length()!=0)
		{
			jabber.updateUrl=url;
		}
		if(version!=null&&version.length()!=0)
		{
			jabber.updateVersion =version;
		}
		if(id!=null&&id.length()!=0)
		{
			SetPicaId(id);
			if (id.indexOf("@pica") != -1)
			{
				this.setPicaNumber(id.substring(0,id.indexOf("@pica")));
			}
		}
		if(password!=null&&password.length()!=0)
		{
			this.SetPicaPW(password);
		}
		if(ip!=null&&ip.length()!=0)
		{
			if(ip.indexOf(":")!=-1)
			{
				jabber.jabberServer = ip.substring(0,ip.indexOf(":"));
				jabber.jabberServerPort=ip.substring(ip.indexOf(":")+1);
				jabber.jabberServerPort=jabber.jabberServerPort.trim();
			}
			else
			{
				jabber.jabberServer = ip;
				jabber.jabberServerPort="80";
			}
		}
		if(ua!=null&&ua.length()!=0)
		{
			jabber.msn_ua=ua;
			
		}
		if(carrier!=null&&carrier.length()!=0)
		{
			jabber.msn_carrier=carrier;
		}
		if(adurl!=null&&adurl.length()!=0)
		{
			if(adurl.startsWith("http://"))
				adurl=adurl.substring("http://".length());
			if(adurl.indexOf("/")!=-1)
				adurl=adurl.substring(0,adurl.indexOf("/"));
			jabber.adServer = adurl.substring(0,adurl.indexOf(":"));
			jabber.adServerPort=adurl.substring(adurl.indexOf(":")+1);
			jabber.adServerPort=jabber.adServerPort.trim();
			isSupportAd=true;
			isSupportChannel=true;
		}
		if(ad_interval!=null&&ad_interval.length()!=0)
		{
			getAdInterval =ad_interval;
		}
		if(zwp!=null&&zwp.length()!=0)
		{
			//connector.zipWapServer=zwp;
			jabber.zipWapServer=zwp;
			isSupportZipWap=true;
			//model.isSupportZipWap=true;//如果不支持类WAP，则以下各项都强制为不支持
		}
		if(fpage!=null&&fpage.length()!=0)
		{
			zwp_homePageAddress=fpage;
			currentUrl=fpage;
		   
			boolean startWithHttps=false;//是否是以https://开头的
		    startWithHttps=fpage.startsWith("https://");
		    
		    if(startWithHttps)
		    	fpage=fpage.substring("https://".length());
		    else if(fpage.startsWith("http://"))
		    	fpage=fpage.substring("http://".length());//先抛掉http://
		    //初始化baseUrl和oldBaseUrl
	    	if(fpage.indexOf('/')!=-1)
	    	{
	    		if(startWithHttps)
	    		{
	    			baseUrl="https://"+fpage.substring(0,fpage.lastIndexOf('/')+1);
	    		}	    			
	    		else
	    		{
	    			baseUrl="http://"+fpage.substring(0,fpage.lastIndexOf('/')+1);
	    		}	    			
	    	}
	    	else
	    	{
	    		if(startWithHttps)
	    		{
	    			baseUrl="https://"+fpage+"/";
	    		}	    			
	    		else
	    		{
	    			baseUrl="http://"+fpage+"/";
	    		}	    			
	    	}	
		}
		if(add_friend!=null&&add_friend.length()!=0)
		{
			//model.zwp_addFriendUrl=add_friend;
			//model.isSupportAddFriend=true;
		}
		if(room_name!=null&&room_name.length()!=0)
		{
			//model.room_groupName=room_name;
			//model.isSupportRoom=true;
		}
		if(room_search!=null&&room_search.length()!=0)
		{
			//model.zwp_room_SearchPageAddress=room_search;
			//model.isSupportSearchRoom=true;
		}
		if(hotmail!=null&&hotmail.length()!=0)
		{
			//model.zwp_hotmailAddress=hotmail;
			//model.isSupportHotmail=true;
		}
		if(spaces!=null&&spaces.length()!=0)
		{
			//model.zwp_spacesAddress=spaces;
			//model.isSupportSpaces=true;
		}
		if(search!=null&&search.length()!=0)
		{
			//model.zwp_searchAddress=search;
			//model.isSupportSearch=true;
		}
		if(roomCreateAddress!=null&&roomCreateAddress.length()!=0)
		{
			//model.zwp_roomCreateAddress=roomCreateAddress;
			//model.isSupportRoomCreate=true;
		}
		if(roomInviteAddress!=null&&roomInviteAddress.length()!=0)
		{
			//model.zwp_roomInviteAddress=roomInviteAddress;
			//model.isSupportRoomInvite=true;
		}
		if(transparent!=null)
		{
			jabber.transParent=transparent;
		}
		if(capability!=null&&capability.length()!=0)
		{
			this.capability=capability;
			String newStringCapability=Integer.toBinaryString(Integer.parseInt(capability,16));	
			char[] newCharArrayCapability=newStringCapability.toCharArray();
			int count=0;
			char ch;
			for(int i=newCharArrayCapability.length-1;i>=0;i--)
			{
				ch=newCharArrayCapability[i];
				switch(count++)
				{
					case 0:
						if(ch=='0')
							isSupportGroupChatByCapability=false;
						else
							isSupportGroupChatByCapability=true;
						break;
					case 1:
						if(ch=='0')
							isSupportHeadImageByCapability=false;
						else
							isSupportHeadImageByCapability=true;
						break;
					case 2:
						if(ch=='0')
							isSupportTransferFileByCapability=false;
						else
							isSupportTransferFileByCapability=true;
						break;
					case 3:
						if(ch=='0')
							isSupportRecordAudioByCapability=false;
						else
							isSupportRecordAudioByCapability=true;
						break;
					case 4:
						if(ch=='0')
							isSupportNudgeMessageByCapability=false;
						else
							isSupportNudgeMessageByCapability=true;
						break;
					case 5:
						if(ch=='0')
							isSupportSaveGroupAndContactByCapability=false;
						else
							isSupportSaveGroupAndContactByCapability=true;
						break;
					case 6:
						if(ch=='0')
							isSupportReceiveHeadImageChangeByCapability=false;
						else
							isSupportReceiveHeadImageChangeByCapability=true;
						break;
					case 7:
						if(ch=='0')
							isSupportReceiveVcardChangeByCapability=false;
						else
							isSupportReceiveVcardChangeByCapability=true;
						break;
					case 8:
						if(ch=='0')
							isSupportOfflineMessageByCapability=false;
						else
							isSupportOfflineMessageByCapability=true;
						break;
					case 9:
						if(ch=='0')
							isSupportLocalnameByCapability=false;
						else
							isSupportLocalnameByCapability=true;
						break;
					default:
						break;	
					
				}
			}
			
		}
		
//		if(!model.isSupportZipWap)//如果不支持类wap，则聊天室等其他操作都不支持
//		{
//			model.isSupportHotmail=false;
//			model.isSupportRoom=false;
//			model.isSupportSearch=false;
//			model.isSupportSpaces=false;
//			model.isSupportSearchRoom=false;
//			model.isSupportAddFriend=false;
//			model.isSupportRoomCreate=false;
//			model.isSupportRoomInvite=false;
//		}	
		
//		Vector  setPage=new Vector();
//		setPage.addElement(new Object[]{ new Byte((byte)4),
//				Simplified.text_my_setting });
//		setPage.addElement(new Object[]{ new Byte((byte)2),
//				Simplified.text_system_setting });
//		if(model.isSupportHotmail)
//			setPage.addElement(new Object[]{ new Byte((byte)12),
//					"Hotmail"});
//		if(model.isSupportSpaces)
//			setPage.addElement(new Object[]{ new Byte((byte)13),
//					"Space"  });
//		Object[][] set=new Object[setPage.size()][];
//		setPage.copyInto(set);
//		
//		Shell.itemVec.removeAllElements();
//		Shell.itemVec.addElement(new Object[]
//		                            		{new Byte((byte)0),
//		                            		Simplified.text_my_profile,
//		                            		Shell.imgToolbarItems,
//		                            		set});	
//		
//		
//		Vector contactPage=new Vector();
//		contactPage.addElement(new Object[]{ new Byte((byte)0),
//				Simplified.text_add_contact });
//		//#ifdef support_exchange_contact_group
//		contactPage.addElement(new Object[]
//		{new Byte((byte)1),
//				Simplified.text_exchange_group_type });
//		//#endif	
//
//		Object[][] contact=new Object[contactPage.size()][];
//		contactPage.copyInto(contact);
//		
//		Shell.itemVec.addElement(new Object[]
//		                            		{new Byte((byte)1),
//		                            		Simplified.text_contact,
//		                            		Shell.imgToolbarItems,
//		                            		contact});	
//		
//		
//		
//		//#ifdef support_exchange_contact_group
//		Shell.itemVec.addElement(new Object[]
//		                            		{
//		                            		new Byte((byte)8),
//		                            		Simplified.text_chatting,
//		                            		Shell.imgToolbarItems,
//		                            		new Object[][]
//		                            		{						
//		                            		{ new Byte((byte)1),
//		                            		Simplified.text_chat_sort_type }
//		                            		} });
//		//#else
//		//# {
//		//#	Shell.itemVec.addElement(new Object[]
//		//#	                            		{
//		//#	                            		new Byte((byte)8),
//		//#	                            		Simplified.text_chatting,
//		//#	                            		Shell.imgToolbarItems,
//		//#	                            		new Object[][]
//		//#	                            		{						
//		//#	                            						
//		//#	                            		} });
//		//# }
//		//#endif
		
		if(transfer_limit!=null&&transfer_limit.length()!=0)
		{
			this.transfer_limit=Integer.parseInt(transfer_limit);
		}
		if(data_url!=null&&data_url.length()!=0)
		{
			this.data_url=data_url;
		}
		if(data_method!=null&&data_method.length()!=0)
		{
			//data_method="000000";
			this.data_method=data_method;
			char[] method=data_method.toCharArray();
			if(method[0]=='1')
				isInnerSendFile=true;
			else
				isInnerSendFile=false;
			
			if(method[1]=='1')
				isInnerReceiveFile=true;
			else
				isInnerReceiveFile=false;
			if(method[2]=='1')
				isInnerSendVoice=true;
			else
				isInnerSendVoice=false;			
			if(method[3]=='1')
				isInnerReceiveVoice=true;
			else
				isInnerReceiveVoice=false;
			
			if(method[4]=='1')
				isInnerSendHead=true;
			else
				isInnerSendHead=false;
			
			if(method[5]=='1')
				isInnerReceiveHead=true;
			else
				isInnerReceiveHead=false;
		}
		
	}

	

	
	public void receiveLoginJerror(String errorMsg, String errorCode) 
	{
		eventListener.eventAction(EventListener.EVENT_RECEIVE_LOGIN_ERROR,new Object[]{errorMsg,errorCode} );	
	}


	
	public void receiveOtherJerror(String jid, String errorMsg) 
	{
		if(jid!=null&&jid.indexOf(CLUSTER_DOMAIN)!=-1)
			eventListener.eventAction(EventListener.EVENT_RECEIVE_CHATROOM_ERROR, errorMsg);
		else
			eventListener.eventAction(EventListener.EVENT_RECEIVE_OTHER_ERROR,errorMsg);
		
	}



	
	public void receiveAD(String type, String id, String action, String target,
			String param0, String dbid, String sale, String text, String src) 
	{
			
			if(type.equals("chat-header-ad"))//会话窗口的文本广告
			{
				AD_ID_chat_header=id;
				Item adItem=new Item();
				adItem.AD_flag=MsnUtil.getAdType(action);;
				adItem.AD_id=id;
				adItem.AD_target=target;
				adItem.AD_param0=param0;
				adItem.AD_text=text;
				adItem.AD_dbid=dbid;
				adItem.AD_sale=sale;
				ad_chat_Vector.addElement(adItem);//569去掉
				adItem=null;
			}
			else if(type.equals("login-prog-ad"))
			{
				AD_ID_login_prog=id;
				jabber.addLoginADRequest(src);
			}
			else if(type.equals("roster-footer-ad"))
			{
				AD_ID_roster_footer=id;
				adRosterItem=new Item();
				adRosterItem.AD_flag=MsnUtil.getAdType(action);;
				adRosterItem.AD_id=id;
				adRosterItem.AD_target=target;
				adRosterItem.AD_param0=param0;
				adRosterItem.AD_text=text;
				adRosterItem.AD_dbid=dbid;
				adRosterItem.AD_sale=sale;
				jabber.addFootADRequest(AD_ROSTER_REQUEST,src,id,action,target,param0,src,dbid,sale);
			}
			else if(type.equals("main-footer-ad"))
			{
				AD_ID_main_footer=id;
				jabber.addFootADRequest(AD_MAIN_REQUEST,src,id,action,target,param0,src,dbid,sale);
			}
			else if(type.equals("session-footer-ad"))
			{
				AD_ID_session_footer=id;
				jabber.addFootADRequest(AD_SESSION_REQUEST,src,id,action,target,param0,src,dbid,sale);
			}
	}


	
	
	public void receiveADComplete() 
	{
		// TODO Auto-generated method stub
		//作用是将新的广告ID号保存
	}

	
	public void receiveChannel(String jid, String name, String groupname,
			String[][] options, boolean iswap, boolean auth) 
	{
		// TODO Auto-generated method stub
		
	}

	
	public void receiveClusterMessage(String from, String to, String name,
			String body, String stamp) 
	{
		// TODO Auto-generated method stub
		
	}

    //#ifdef support_head_image
	public void receiveContactPortrait(String jid,String portraithash, String imageData,String url)
	{
		Contact contact = getContact(jid);

		if (contact!=null)
		{
		    if(portraithash != null)
			contact.CONTACT_portraithash = portraithash;
			if(url!=null)//带外
			{
				/////// 带外
			    try {
				new FileOuterTransfer(this,FileOuterTransfer.command_downloadFile,null,contact,url);
				app.isTransferingOuterSendHead=true;//
			    }catch(Exception e)
			    {
			        e.printStackTrace();
			    }
			}
			else//带内
			{
				contact.CONTACT_imageData = MsnUtil.decode(imageData);
				 //#ifdef support_save_rms
				//addContactToRMS(contact);//保存hash
				try {
				addContactHeadToRMS(contact);
				}catch(Exception e)
				{
				    e.printStackTrace();
				}
				//#endif
				eventNotify(EventListener.EVENT_RECEIVE_CONTACT_PORTRAIT, contact);
			}
		}
	}
	//#endif


	
	public void receiveExitCluster(String jid) 
	{
		// TODO Auto-generated method stub
		
	}

	
	public void receiveGroup(String last, String groupName, String groupId, String gbcode, String sync, String version)
	{
		
		Object[] obj = new Object[6];
		obj[0] = last;
		obj[1] = groupName;
		obj[2] = groupId;
		obj[3] = gbcode;
		obj[4] = sync;
		obj[5] = version;		
		eventNotify(EventListener.EVENT_RECEIVE_GROUP, obj);
		
//		byte groupflag=GROUP_USER_FLAG;
//		boolean isAboutDefault=true;//是否关于缺省组的操作
//		boolean isChangeDefault=false;//是否修改一个缺省组
//		boolean isNewDefault=false;//是否需要新生成一个缺省组
//		boolean hasAddDefault=false;//在删除操作时候 是否添加了缺省分组
//		Group oldGroup=null;
//        if(groupId == null||groupId.equals(DEFAULT_GROUP_ID))
//        {	
//        	groupId = DEFAULT_GROUP_ID;
//        	oldGroup=getGroup(DEFAULT_GROUP_ID,app.groupData);
//        	if(oldGroup==null)
//        	{
//        		DEFAULT_GROUP_GBC=gbcode;//给变量赋值
//        		DEFAULT_GROUP_NAME=groupName;//给变量赋值
//        		if(DEFAULT_GROUP_NAME==null||DEFAULT_GROUP_NAME.length()==0)
//        		{
//        			DEFAULT_GROUP_NAME=(String)mActivity.getText(R.string.about);
//        		}
//        		isNewDefault=true;//需要新生成一个缺省组
//        	}
//        	else//缺省组 不考虑删除并添加了 可以减少排序等操作
//        	{
//            	if(!oldGroup.GROUP_gbcode.equals(gbcode))
//            	{
//            		DEFAULT_GROUP_GBC=gbcode;//给变量赋值
//            		oldGroup.GROUP_gbcode=gbcode;
//            		isChangeDefault=true;//修改一个缺省组
//            	}
//            	if(!oldGroup.GROUP_name.equals(groupName))
//            	{
//            		DEFAULT_GROUP_NAME=groupName;//给变量赋值
//            		if(DEFAULT_GROUP_NAME==null||DEFAULT_GROUP_NAME.length()==0)
//            		{
//            			DEFAULT_GROUP_NAME=(String)mActivity.getText(R.string.about);
//            		}
//            		oldGroup.GROUP_name=groupName;
//            		isChangeDefault=true;//修改一个缺省组
//            	}
//            	if(isChangeDefault)
//            	{	
//            		//#ifdef support_exchange_contact_group
//            		hasAddDefault=removeGroup(groupId,app.groupData);//删掉老分组
//            		////////////////////////
//            		//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
//            		removeGroup(groupId,app.groupDataNormalOnlyOnline);
//            		/////////////////////////
//            		//#else
//            		//# hasAddDefault=model.getRoster().removeGroup(groupId);//删掉老分组
//            		//#endif
//            	}
//        	}
//        }
//        else
//        	isAboutDefault=false;
//        
//        if(sync.equals("new"))//新分组
//        {
//        	if(oldGroup!=null)//如果已经存在
//        	{    		
//        		//#ifdef support_exchange_contact_group
//        		hasAddDefault=removeGroup(groupId,app.groupData);	//先删除	
//        		////////////////////////
//        		//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
//        		removeGroup(groupId,app.groupDataNormalOnlyOnline);
//        		/////////////////////////
//        		
//        		
//        		//#else
//        		//# hasAddDefault=model.getRoster().removeGroup(groupId);	//
//        		//#endif
//        	}
//        	if(!hasAddDefault)
//        	{
//            	oldGroup=newGroup(groupId, gbcode, groupName, groupflag,false);
//            	addGroupToList(oldGroup,app.groupData);
//            	/////////////
//            	/////////////
//        		//#ifdef support_exchange_contact_group
//            	//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
//            	addGroupToList(newGroup(groupId, gbcode, groupName, groupflag,false),app.groupDataNormalOnlyOnline);
//            	//#endif
//    			/////////////
//            	
//           	 	//#ifdef support_save_rms
//               // addGroupToRMS(oldGroup);
//                //#endif
//        	}
//        }
//        else if(sync.equals("updated"))//修改
//        {
//        	//是否要保存到rms
//        	boolean updateRms=false;
//        	
//        	if(isNewDefault)
//        	{
//        		oldGroup=newGroup(groupId, gbcode, groupName, groupflag,false);
//        		updateRms=true;
//        		addGroupToList(oldGroup,app.groupData);
//        		//////////////////
//        		//#ifdef support_exchange_contact_group
//        		//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
//        		addGroupToList(newGroup(groupId, gbcode, groupName, groupflag,false),app.groupDataNormalOnlyOnline);
//        		//#endif
//    			/////////////////
//        	}
//        	if(isChangeDefault)
//        	{
//        		if(hasAddDefault)//如果添加过了 就不需要再添加
//        			updateRms=false;
//        	}
//        	if(!isAboutDefault)//不是缺省组
//        	{
//        		oldGroup=getGroup(groupId,app.groupData);
//            	if(oldGroup==null)
//            	{
//            		oldGroup=newGroup(groupId, gbcode, groupName, groupflag,false);
//            		updateRms=true;
//            		addGroupToList(oldGroup,app.groupData);
//            		//////////////////
//            		//#ifdef support_exchange_contact_group
//            		//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
//            		addGroupToList(newGroup(groupId, gbcode, groupName, groupflag,false),app.groupDataNormalOnlyOnline);
//            		//#endif
//            		/////////////////
//            	}
//            	else
//            	{
//            		
//            		if(!oldGroup.GROUP_gbcode.equals(gbcode))
//            		{
//            			oldGroup.GROUP_gbcode=gbcode;
//            			updateRms=true;
//            		}
//            		if(!oldGroup.GROUP_name.equals(groupName))
//            		{
//            			oldGroup.GROUP_name=groupName;
//            			updateRms=true;
//            		}
//            		if(updateRms)
//            		{            		
//            			//#ifdef support_exchange_contact_group
//            			removeGroup(groupId,app.groupData);//删掉老分组
//                		////////////////////////
//            			//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
//                		removeGroup(groupId,app.groupDataNormalOnlyOnline);
//                		/////////////////////////			
//            			//#else
//            			//# model.getRoster().removeGroup(groupId);//删掉老分组
//            			//#endif
//            			oldGroup.GROUP_itemnum=0;
//            			oldGroup.GROUP_maxweight_itemnum="0";
//            			addGroupToList(oldGroup,app.groupData);
//                		
//                		//////////////////
//            			//#ifdef support_exchange_contact_group
//            			Group oldGroup1=newGroup(oldGroup.GROUP_id, oldGroup.GROUP_gbcode, oldGroup.GROUP_name, oldGroup.GROUP_flag,false);
//                		//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
//                		addGroupToList(oldGroup1,app.groupDataNormalOnlyOnline);
//                		//#endif
//            			/////////////////
//            		}
//            	}
//        	}
//        	if(updateRms)
//        	{  		
//           	 	//#ifdef support_save_rms
//        		//addGroupToRMS(oldGroup);
//        		//#endif
//        	}
//        }
//        else if(sync.equals("deleted"))//删除
//        {
//        	//#ifdef support_exchange_contact_group
//        	removeGroup(groupId,app.groupData);
//        	
//    		////////////////////////
//        	//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
//        	removeGroup(groupId,app.groupDataNormalOnlyOnline);
//    		/////////////////////////
//        	
//        	//#else
//        	//# model.getRoster().removeGroup(groupId);
//        	//#endif
//        	
//       	 	//#ifdef support_save_rms
//       	 	if(saveDataTimer != null)
//			{
//				saveDataTimer.cancel();
//				saveDataTimer = null;
//			}			
//			saveDataTimer = new Timer();
//        	saveDataTimer.schedule(new Animation(this,"saveData"), 15000);
//       	 	//removeGroupFromRMS(groupId);
//        	//#endif
//        }
//        //eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
		
	}

	
	public void receiveGroupComplete(String last, String version) 
	{
		String[] obj = new String[2];
		obj[0] = last;
		obj[1] = version;
		eventNotify(EventListener.EVENT_RECEIVE_GROUP_COMPLETE, obj);
		
//		if(getGroup(DEFAULT_GROUP_ID,app.groupData)==null)
//		{
//    		if(DEFAULT_GROUP_NAME==null||DEFAULT_GROUP_NAME.length()==0)
//    		{
//    			DEFAULT_GROUP_NAME=(String)mActivity.getText(R.string.about);
//    		}
//    		Group tempGroup=newGroup(DEFAULT_GROUP_ID, DEFAULT_GROUP_GBC,DEFAULT_GROUP_NAME,GROUP_USER_FLAG,false);
//			addGroupToList(tempGroup,app.groupData);
//    		//////////////////
//        	//#ifdef support_exchange_contact_group
//			//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
//			addGroupToList(newGroup(DEFAULT_GROUP_ID, DEFAULT_GROUP_GBC, DEFAULT_GROUP_NAME, GROUP_USER_FLAG,false),app.groupDataNormalOnlyOnline);
//    		//#endif
//			/////////////////
//			eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
//		}		
//
//        if(version != null)
//        {
//        	if(myContactGroupVersion==null||(!myContactGroupVersion.equals(version)))
//        	{
//        		myContactGroupVersion=updateNewVersion(myContactGroupVersion,version);
//   			 	//#ifdef support_save_rms
//        		setContactGroupVerToRMS(myContactGroupVersion);
//        		//#endif
//        	}
//        }
//        //this.saveAllGroupList();
		
	}

	//#ifdef support_groupmessage
	public void receiveGroupMessage(String from,String jid,String imid, String sid, String body,String stamp,boolean nudge,String status,String reason)
	{
		
		String nickname = null;
		Contact c=null;
		if(from!=null&&from.equals(MSN_DOMAIN)&&jid==null&&imid==null)
		{
			//消息发送失败 网关返回
			nickname=(mActivity.getText(R.string.text_system_msg)).toString();//"系统消息";
		}
		else if(jid==null&&imid!=null)//陌生人
		{
			nickname=imid;
		}
		else
		{
			c=getContact(jid);

			if(c!=null)
			{
				nickname=c.CONTACT_nickname;
			}
			else
			{
				return;//微软有bug 某些时候可能陌生人的消息也是仅有jid而无imid 这时候客户端就不处理了
			}
		}
		if (stamp == null)
			stamp = MsnUtil.toString(new Date());
		
		/*当客户端接收到一条type=’groupchat’消息时，需要检查该消息的sid在本地是否存在，
		如存在，代表客户端原有的2人会话被升级为多人会话（sid不变）；如果该sid不存在，则代表客户端被加入一个新的多人会话。*/
        
		Item session = getSessionBySid(sid);
        if(session == null)
        { //本地不存在这个会话，代表被别人加入了一个多人会话
           //扔掉该消息
        	return;
        }
        else
        { 
            //本地存在该sid的2人会话，代表原会话被升级成多人会话 （sid不变）
            if((session.SESSION_sessionType == SESSION_TYPE_CHAT)&& !"fail".equals(status))
            	session.SESSION_sessionType  = SESSION_TYPE_GROUP_CHAT;
        }
        
        Item message = null;
        if(session.SESSION_sessionType == SESSION_TYPE_CHAT)
        {
        	message = addMessage(MESSAGE_TYPE_CHAT,sid, jid,imid,
					nickname, body,  stamp,nudge,status,reason);
        }
        else
        {
        	message = addMessage(MESSAGE_TYPE_GROUP_CHAT,sid, jid,imid,
					nickname, body,  stamp,nudge,status,reason);
        }
		message.MESSAGE_IS_SEND=false;
		eventNotify(EventListener.EVENT_RECEIVED_MESSAGE, message);

	}

    /**
     * 邀请联系人加入多人临时对话
     * @param Item firstparticipant 单人会话时候 第一个联系人 
     * @param Item session 当前会话
     * @param chaterJid 对方的jids
     * @return
     */
    public boolean addSessionParticipants(Contact firstparticipant,Item session, String[] jids)
	{
    	try
		{
			byte type = session.SESSION_sessionType;// 会话类型
			String sid = session.TOPITEM_JID;// 会话id
			String jid = null,imid = null;
			if (type == SESSION_TYPE_CHAT)
			{
				// 单人对话里面的联系人
				jid = firstparticipant.SUBITEM_JID;
				imid = firstparticipant.CONTACT_imid;
			} 
			app.jabber.sendInviteContactAddSession(jid, imid, sid, jids);
		} 
    	catch (Exception e)
		{
			e.printStackTrace();
			return false;
		}
		return true;
	}
    //#endif

	
	public void receiveIqAuth() 
	{
		// TODO Auto-generated method stub
		jabber.getIqRoster();
	}

	
	public void receiveJoinCluster(String jid, String name, String type) 
	{
		// TODO Auto-generated method stub
		
	}

	
	public void receiveKickoffOrBlacklistChatroom(String role, String reason) 
	{
		// TODO Auto-generated method stub
		
	}

	
	public void receiveLogoutMsn() 
	{
	    jabber.receive_logout_unavailable=true;    
	    jabber.sendEnd();    
	    
	    if(!isExit&&!isLogout)
	    	eventListener.eventAction(EventListener.EVENT_RECEIVE_MSN_LOGOUT, null);
		
	}

	
	public void receiveMessage(String jid, String imid, String sid,
			String body, String stamp, boolean nudge, String status,
			String reason,String transfer_ftID) 
	{
		Item session = null;
		Contact contact=null;
		try
		{
		if(jid==null&&imid!=null)
		{	
			contact=newStranger(imid);
			session =getSessionFromContact(imid,sid);
		}
		else//好友
		{
			contact=getContact(jid);
			if(contact!=null)
			{
				session =getSessionFromContact(contact.CONTACT_imid,sid);
			}
			else
			{
				return;//微软有bug 某些时候可能陌生人的消息也是仅有jid而无imid 这时候客户端就不处理了
			}
		}
	
		if (session == null)
		{ 
			session = addNewSession(contact, SESSION_TYPE_CHAT, sid);
			sid = session.TOPITEM_JID;
		} 
		else
		{ // 本地存在和该好友的2人会话,则以本地sid为准
			sid = session.TOPITEM_JID;
		}

		if (stamp == null)
			stamp = MsnUtil.toString(new Date());


		Item  message = addMessage(MESSAGE_TYPE_CHAT, sid,jid,contact.CONTACT_imid,
				contact.CONTACT_nickname, body, stamp,nudge,status,reason);
		message.MESSAGE_IS_SEND=false;
		
		eventNotify(EventListener.EVENT_RECEIVED_MESSAGE, message);
		
		if(transfer_ftID!=null&&transfer_ftID.length()!=0)
		{
			Item transfer_fail_message=this.getOneMessageByFtID(session, transfer_ftID);
			transfer_fail_message.MESSAGE_FILE_STATUS_TYPE=MESSAGE_FILE_TYPE_SEND_EXCEPTION;
			eventNotify(EventListener.EVENT_UPDATE_MESSAGE, transfer_fail_message);
		}
		}
	      catch (Exception e)
	        {
	            e.printStackTrace();
	            MsnUtil.DebugOutPutFile("MSNAPP 1916" + e.getMessage());
	        }
		
	}
	
	/**
	 * 获取与一个联系人的单聊会话
	 * @param imid 联系人imid 
	 * @return
	 */
	public final Item getSessionFromContact(String imid, String sid)
	{
		Item currentSession;
		Contact chater;
		String chaterImid;
		try {
		for (int i = 0; i < app.sessionData.size(); i++)
		{
			currentSession=null;
			Map<String, Item> currentSessionMap = app.sessionData.get(i);
			currentSession=(Item)currentSessionMap.get("SESSION");
			if(currentSession.SESSION_sessionType==SESSION_TYPE_CHAT)//所有单聊对话
			{
				chater = null;
				chater = (Contact) (currentSession.SESSION_chaters).firstElement();
				chaterImid = chater.CONTACT_imid;
				if (imid.equals(chaterImid))
				{
				    if(sid != null)
				    {
				    currentSession.TOPITEM_JID = sid;
				    }
					return currentSession;
			}
		}
		}
	}
    catch (Exception e)
    {
        e.printStackTrace();
        MsnUtil.DebugOutPutFile("MSNAPP 1950" + e.getMessage());
    }
		return null;
	}
	
	/**
	 * 添加(单聊 群聊)信息到会话向量 要进行数量判断 
	 * @param type MESSAGE_TYPE_SYSTEM系统消息 MESSAGE_TYPE_CHAT单聊 MESSAGE_TYPE_GROUP_CHAT 多人会话聊等
	 * @param sid  会话id
	 * @param jid
	 * @param imid 
	 * @param nickName 昵称
	 * @param body  内容
//	 * @param fileName
//	 * @param mime
//	 * @param size
//	 * @param url
	 * @param stamp
	 * @param nudge 是否震动 此项和body项是互斥唯一的
	 * @param status  fail
	 * @param reason 发送失败原因
	 */	
	synchronized Item addMessage(byte type, String sid, String jid,String imid,
			String nickName, String body, 
			//String fileName,String mime, String size, String url, 
			String stamp,boolean nudge,
			String status,String reason)
	{
		if(imid!=null&&imid.equals(this.GetLiveID()))
		{
			nickName=myVCARD_myNickname;
		}

		Item session = getSessionBySid(sid);
		if (session != null)
		{
			Item message=new Item();
			message.MESSAGE_type=type;
			message.MESSAGE_sid=sid;
			message.MESSAGE_jid=jid;
			message.MESSAGE_imid=imid;
			message.MESSAGE_nickname=nickName;
			message.MESSAGE_body=body;
			message.MESSAGE_stamp=stamp;
			message.MESSAGE_nudge=nudge;
			message.MESSAGE_status=status;
			message.MESSAGE_reason=reason;		
			//if(session.SESSION_contents.size()>=MESSAGE_MAX_COUNT)
			//	session.SESSION_contents.removeElementAt(0);//删掉第一条
			session.SESSION_contents.addElement(message);
			session.SESSION_numOfNewMsg=Integer.toString(Integer.parseInt(session.SESSION_numOfNewMsg)+1);
			return message;
		} 
		else
		{
			return null;
		}

	}
	
	/**
	 * 添加邀请接收文件的信息到会话向量 
	 * @param type 
	 * @param sid  会话id
	 * @param jid
	 * @param imid 
	 * @param nickName 昵称
	 * @param stamp
	 * @param transferID 服务器用来标识传输的文件
	 * @param fileName 文件名称
	 * @param fileSize  文件大小（字节数）
	 * @param messageFileType 当前的状态（接收 拒绝等）
	 * @param fileData 附件数据
	 */	
	synchronized Item addFileInviteMessage(byte type, String sid, String jid,String imid,
			String nickName, String stamp,String transferID,String fileName,String fileSize,
			byte messageFileType,byte[]fileData)
	{
		if(imid!=null&&imid.equals(this.GetLiveID()))
		{
			nickName=myVCARD_myNickname;
		}

		Item session = getSessionBySid(sid);
		if (session != null)
		{
			Item message=new Item();
			message.MESSAGE_type=type;
			message.MESSAGE_sid=sid;
			message.MESSAGE_jid=jid;
			message.MESSAGE_imid=imid;
			message.MESSAGE_nickname=nickName;
			message.MESSAGE_stamp=stamp;
			message.MESSAGE_FILE_VOICE_NAME=fileName;
			message.MESSAGE_FILE_SIZE=fileSize;
			message.MESSAGE_FILE_transferID=transferID;
			message.MESSAGE_FILE_ftID=System.currentTimeMillis()+jid;
			message.MESSAGE_FILE_STATUS_TYPE=messageFileType;
			message.MESSAGE_FILE_DATA=fileData;
			//if(session.SESSION_contents.size()>=MESSAGE_MAX_COUNT)
			//	session.SESSION_contents.removeElementAt(0);//删掉第一条
			session.SESSION_contents.addElement(message);
			session.SESSION_numOfNewMsg=Integer.toString(Integer.parseInt(session.SESSION_numOfNewMsg)+1);
			return message;
		} 
		else
		{
			return null;
		}
	}

	
	
	/**
	 * 获取指定sid的会话
	 * @param sid 会话ID
	 * @return 会话 Item(session)
	 */
	public final Item getSessionBySid(String sid)
	{
		Item currentSession=null;
		String currentSid;
		for (int i = 0; i < app.sessionData.size(); i++)
		{
			
			Map<String, Item> currentSessionMap = app.sessionData.get(i);
			currentSession=(Item)currentSessionMap.get("SESSION");
    		currentSid=currentSession.TOPITEM_JID;
    		if(currentSid.equals(sid))
    			return currentSession;
		}
		return null;
	}
	
	/**
	 * 新建一个会话 （包括聊天室会话 聊天室会话不加入到列表）
	 * @param chater 联系人/如果是聊天室会话 则是聊天室
	 * @param type 会话类型  Constant.SESSION_TYPE_GROUP_CHAT等
	 * @param sid 会话Id
	 */
	public final Item addNewSession(Contact chater, byte type, String sid)
	{
		Vector chaters = new Vector();
		Vector contents = new Vector();
		Vector blockVec=null;
		if(type==SESSION_TYPE_CLUSTER_CHAT)
		{
			blockVec=new Vector();
		}
		if (chater != null)
			chaters.addElement(chater);

		Item session=new Item();
		
		//session.TreeList_TYPE=TreeList.TYPE_TOPITEM;
		session.SESSION_sessionType=type;
		session.SESSION_time=MsnUtil.toString(new Date());
		session.SESSION_chaters=chaters;

		session.SESSION_contents=contents;
//		if(type==Constant.SESSION_TYPE_CLUSTER_CHAT)
//		{
//			session.SESSION_BLOCK_LIST=blockVec;
//			
//			////////////////0320
//			session.SESSION_numOfNewMsg="0";
//			sessionList.insertTopItem(session,0);
//			session.TOPITEM_JID=chater.SUBITEM_JID;
//			////////////////
//		}
//		else
		{
			session.SESSION_numOfNewMsg="0";
			if (sid == null)
			{
				maxSid++;
				sid = "c-" + maxSid;
			}
			session.TOPITEM_JID=sid;
			
			///////////////////直接把会话加进去 加到最后一个
			Map<String, Item>  currentSessionMap = new HashMap<String, Item>();      
            app.sessionData.add(currentSessionMap);
            currentSessionMap.put("SESSION", session);
            ///////////////
			//sessionList.insertTopItem(session, 0);//直接把会话加进去 加到最起始的索引处
		}
		return session;
	}
	
	
	/***
	 * 新生成一个陌生联系人并返回
	 * @param imid 陌生人的imid
	 * @return 陌生联系人对象（Contact的一种）
	 */
    public Contact newStranger(String imid)
    {
    	Contact stranger=new Contact();
//		stranger.TreeList_TYPE=TreeList.TYPE_SUBITEM;
//		stranger.SUBITEM_TYPE=TreeList.SUBITEM_CONTACT;
		stranger.CONTACT_imid=imid;
		stranger.CONTACT_contactFlag=CONTACT_STRANGER_FLAG;
		stranger.CONTACT_nickname=imid;
		return stranger;
    }

	
	public void receiveModifyMyState(String show) 
	{
		// 用户改变自己的状态
		if (show != null)
		{
			if (show == null || show.equals(STATE_FLAG_All[0]))
			{
				this.setMyState(STATE_ONLINE);
			} 
			else if (show.equals(STATE_FLAG_All[1]))
			{
				this.setMyState(STATE_BUSY);
			} 
			else if (show.equals(STATE_FLAG_All[2]))
			{
				this.setMyState(STATE_AWAY);
			} 
			else if (show.equals(STATE_FLAG_All[3]))
			{
				this.setMyState(STATE_BeRightBack);
			} 
			else if (show.equals(STATE_FLAG_All[4]))
			{
				this.setMyState(STATE_InACall);
			} 
			else if (show.equals(STATE_FLAG_All[5]))
			{
				this.setMyState(STATE_OutToLunch);
			} 
			else if (show.equals(STATE_FLAG_All[6]))
			{
				this.setMyState(STATE_OFFLINE);
			}
		}
		if(isLoginSuccess)	
			eventListener.eventAction(EventListener.EVENT_CHANGE_MY_STATE,null);
		
	}

	
	public void receiveOutOfMemory() {
		// TODO Auto-generated method stub
		
	}

	//#ifdef support_groupmessage
    /**
     * 多人会话（临时群组会话）
     * @param sid 会话id
     * @param content 会话内容
     * @return
     */
    public void sendGroupMessage(String sid,String content)
	{
		app.jabber.sendGroupMessageRequest(content, sid);

		Item message =addMessage(MESSAGE_TYPE_GROUP_CHAT,
				sid,null, this.GetLiveID(), myVCARD_myNickname, 
				content, MsnUtil.toString(new Date()),false,null,null);
		message.MESSAGE_IS_SEND=true;
		eventNotify(EventListener.EVENT_SEND_MESSAGE, message);
		return;
	}

    /**临时多人对话里面是否有jid 或者imid这个联系人*/
    public boolean isChaterInThisSession(Item session, String jid,String imid,boolean isStranger)
	{
		Vector chaters = session.SESSION_chaters;
		Contact contact;
		try {
		for (int i = 0; i < chaters.size(); i++)
		{
			contact = null;
			contact = (Contact) chaters.elementAt(i);
			if(isStranger)
			{
				if (contact.CONTACT_imid.equals(imid))
					return true;
			}
			else
			{
				if (contact.SUBITEM_JID!=null&&contact.SUBITEM_JID.equals(jid))
					return true;
			}

		}
		}
		catch(Exception e)
        {
            e.printStackTrace();
        }
		return false;
	}
    

    /**用户加入或离开临时多人会话
     * @param node[] jid imid 
     * */
    public void receiveParticipant(String sid,  String action,String[][] node)
	{
    	Item session =getSessionBySid(sid);
    	int count=node.length;
    	boolean isStranger=false;
		String jid=null,imid=null;
		try {
		if (session != null)//该会话已经存在
		{ 				//对话有新成员加入  暂时认为都是普通好友
			byte oldSessionFlag=session.SESSION_sessionType;
			byte newSessionFlag;
			boolean isRefreshUI=false;
			Contact contact =null;
			String nickname;
			Contact stranger;
			Item message;
			Contact item;
			for(int k=0;k<count;k++)
			{
				jid=node[k][0];
				imid=node[k][1];
		    	if(imid!=null)
		    		isStranger=true;
		    	
				if(!isStranger)//好友
				{
					contact =null;
					contact=getContact(jid);			
					if(contact==null)
						continue;
				}
							
				
				if (action.equals("enter"))
				{
					//该用户已在对话成员列表里 则不添加
					if (isChaterInThisSession(session, jid,imid,isStranger))
						return;
					
					if (contact != null)
					{
						session.SESSION_chaters.addElement(contact);
						nickname = contact.CONTACT_nickname;//==null?contact.CONTACT_imid:contact.CONTACT_nickname;
					} 
					else
					{
						stranger = null;
						stranger = newStranger(imid);
						session.SESSION_chaters.addElement(stranger);
						nickname = imid;
						
					}
					session.SESSION_sessionType=SESSION_TYPE_GROUP_CHAT;
					message = null;
					message = addMessage(MESSAGE_TYPE_SYSTEM,sid, null,null,
							(mActivity.getText(R.string.text_system_msg)).toString(), 
							nickname + " " + mActivity.getText(R.string.text_somebody_add_to_chat), 
							MsnUtil.toString(new Date()),false,null,null);
					message.MESSAGE_IS_SEND=false;
					eventNotify(EventListener.EVENT_RECEIVED_MESSAGE, message);
					eventNotify(EventListener.EVENT_UPDATE_SESSION_TOP_NAME, null);
					isRefreshUI=true;
				} 
				else if (action.equals("leave"))
				{
					if (contact != null)
					{
						session.SESSION_chaters.removeElement(contact);
						nickname = contact.CONTACT_nickname;
					} 
					else//陌生人
					{						
						for (int i = 0; i < session.SESSION_chaters.size(); i++)
						{
							item = null;
							item = (Contact)session.SESSION_chaters.elementAt(i);
							if (item.CONTACT_imid.equals(imid))
								session.SESSION_chaters.removeElement(item);
						}
						nickname =imid;
					}
					if(session.SESSION_chaters.size()==1)//降级为单人对话
					{
						session.SESSION_sessionType=SESSION_TYPE_CHAT;
					}

					message = addMessage(MESSAGE_TYPE_SYSTEM,sid, null,null,
							(mActivity.getText(R.string.text_system_msg)).toString(), 
							nickname + " " + mActivity.getText(R.string.text_somebody_quit_to_chat), 
							MsnUtil.toString(new Date()),false,null,null);
					message.MESSAGE_IS_SEND=false;
					eventNotify(EventListener.EVENT_RECEIVED_MESSAGE, message);
					eventNotify(EventListener.EVENT_UPDATE_SESSION_TOP_NAME, null);
					isRefreshUI=true;
				}
			}
			newSessionFlag=session.SESSION_sessionType;
			if(oldSessionFlag!=newSessionFlag)
			{
				if(!isRefreshUI)
					eventNotify(EventListener.EVENT_UPDATE_SESSION_FLAG, null);
			}
		} 
		else
		{	//被人加入一个多人对话或者单人会话 标准是participant的个数
			if(count==1)
			{
				//单人对话
				jid=node[0][0];
				imid=node[0][1];
				Contact c=getContact(jid);
				
				if (action.equals("enter"))
				{
					String nickname;
					if (c != null)
					{
						session =addNewSession(c,SESSION_TYPE_CHAT, sid);
						nickname = c.CONTACT_nickname;
					} 
					else
					{
						Contact stranger=newStranger(imid);
						nickname = imid;
						session = addNewSession(stranger,SESSION_TYPE_CHAT, sid);
					}	
					
					Item message = addMessage(MESSAGE_TYPE_SYSTEM,sid, null,null,
							(mActivity.getText(R.string.text_system_msg)).toString(),
							nickname + " " + mActivity.getText(R.string.text_somebody_add_to_chat), 
							MsnUtil.toString(new Date()),false,null,null);
					message.MESSAGE_IS_SEND=false;
					eventNotify(EventListener.EVENT_RECEIVED_MESSAGE, message);
					eventNotify(EventListener.EVENT_UPDATE_SESSION_TOP_NAME, null);			

				}
			}
			else
			{
				Contact c;
				Contact stranger;
				Item message;
				String nickname;
				//多人对话
				for(int j=0;j<node.length;j++)
				{
					jid=node[j][0];
					imid=node[j][1];
					c=null;
					c=getContact(jid);
					if (action.equals("enter"))
					{						
						if (c != null)
						{
							session = addNewSession(c,
									SESSION_TYPE_GROUP_CHAT, sid);
							nickname = c.CONTACT_nickname;
						} 
						else
						{	
							stranger = null;
							stranger = newStranger(imid);
							nickname = imid;
							session = addNewSession(stranger,SESSION_TYPE_GROUP_CHAT, sid);
						}	
						message = null;
						message = addMessage(MESSAGE_TYPE_SYSTEM,sid, null,null,
								(mActivity.getText(R.string.text_system_msg)).toString(),
								nickname + " " + mActivity.getText(R.string.text_somebody_add_to_chat),
								MsnUtil.toString(new Date()),false,null,null);
						message.MESSAGE_IS_SEND=false;
						eventNotify(EventListener.EVENT_RECEIVED_MESSAGE, message);
						eventNotify(EventListener.EVENT_UPDATE_SESSION_TOP_NAME, null);
					}
					
				}
			}
		}
	}
	      catch(Exception e)
	        {
	            e.printStackTrace();
	        }
	}
    //#endif

	/**收到pica好友列表包 发送上线状态给pica*/
	public void receivePicaIqRoster()
	{
		jabber.sendPresenceAvailable();
		
	}

	
	public void receiveRoom(String jid, String name, String type) {
		// TODO Auto-generated method stub
		
	}

	
	public void receiveRoster(String action,String last, String jid,String imid,
			String nickname, String gbcode,String block, String sync, String groupId,
			String impresa, String portraithash,String phone,String gleam, 
			String url, String version,Vector nodeVec,String deviceId,String addition)
	{
		Object[] obj = new Object[18];
		obj[0] = action;
		obj[1] = last;
		obj[2] = jid;
		obj[3] = imid;
		obj[4] = nickname;
		obj[5] = gbcode;
		obj[6] = block;
		obj[7] = sync;
		obj[8] = groupId;
		obj[9] = impresa;
		obj[10] = portraithash;
		obj[11] = phone;
		obj[12] = gleam;
		obj[13] = url;
		obj[14] = version;
		obj[15] = nodeVec;
		obj[16] = deviceId;
		obj[17] = addition;
		eventNotify(EventListener.EVENT_RECEIVE_ROSTER, obj);
//		try
//		{
//			 //#ifdef support_save_rms
//			if (deviceId != null)
//			{
//				if ((devicedId == null)
//						|| (!devicedId.equals(deviceId)))
//				{
//					setDeviceIdToRMS(this.GetLiveID(), deviceId);
//					devicedId=deviceId;
//				}
//				
//			}
//			//#endif
//			
//			byte contactFlag=CONTACT_NORMAL_FLAG;
//			boolean isChangeBlockState=false;
//			if(addition!=null&&addition.equals("0"))
//			{
//				if(sync==null||sync.length()==0)
//					sync="updated";
//				if (block != null && block.equals("1"))
//					contactFlag = CONTACT_BLOCK_FLAG;
//				isChangeBlockState=true;
//			}
//			else
//			{
//				if(nodeVec!=null&&nodeVec.contains("block"))
//				{
//					if (block != null && block.equals("1"))
//						contactFlag = CONTACT_BLOCK_FLAG;
//					isChangeBlockState=true;
//				}
//				else
//				{
//					isChangeBlockState=false;
//				}
//			}
//
//			if (sync != null && sync.equals("new"))// 新联系人
//			{
//				Contact contact =getContact(jid);
//				if (contact == null)
//				{
//					if(imid==null)
//						return;//如果imid为空 则把这个包扔掉（防服务器出问题）
//									
//					//addContact(jid, imid, nickname, gbcode, groupId,
//					//		contactFlag, portraithash, impresa, null,phone, gleam, url,
//					//		null, false, true);
//					
//					Contact newContact=newContact(MSNApplication.STATE_OFFLINE,jid, imid, nickname, gbcode, groupId,
//							contactFlag, portraithash, impresa, null,phone, gleam, url,null);
//					//addContact(newContact);
//					
//					addContact(newContact,ROSTER_NORMAL_HAS_OFFLINE,app.groupData);
//					///////////////////
//					//#ifdef support_exchange_contact_group
//					if(getGroupOrder()>ROSTER_NORMAL_HAS_OFFLINE)
//					{
//						addContact(newContact,getGroupOrder(),app.getCurrentGroupData());
//					}
//					//#endif
//					
//					
//					//#ifdef support_save_rms
//					//if(isSaveRMS)
//					{
//						//addContactToRMS(newContact);	
//						addContactHeadToRMS(newContact);
//					}
//					//#endif
//					
//					if (action != null && action.equals("add"))// 表示添加联系人成功
//					{
//						eventNotify(EventListener.EVENT_ADD_CONTACT_OK, newContact);
//						return;
//						
//					}
//					//eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
//				}
//				else//相当于更新update
//				{
//					if(!isChangeBlockState)
//						contactFlag=contact.CONTACT_contactFlag;
//					updateContact(contact,action,jid,imid,contactFlag,
//							nodeVec,block,nickname,gbcode,groupId,
//							impresa,phone,portraithash,gleam,url);
//				}
//			} 
//			else if (sync != null && sync.equals("updated"))// 修改的联系人
//			{
//				Contact contact =getContact(jid);
//				if (contact==null)
//				{			
//					if(imid==null)
//						return;//如果imid为空 则把这个包扔掉（防服务器出问题）
//					
//					// 特殊处理 相当于new
////					addContact(jid, imid, nickname, gbcode, groupId,
////							contactFlag, portraithash, impresa, null,phone, gleam,
////							url,  null,false, true);
//					
//					Contact newContact=newContact(MSNApplication.STATE_OFFLINE,jid, imid, nickname, gbcode, groupId,
//							contactFlag, portraithash, impresa, null,phone, gleam, url,null);
//					//addContact(newContact);
//					addContact(newContact,ROSTER_NORMAL_HAS_OFFLINE,app.groupData);
//					///////////////////
//					//#ifdef support_exchange_contact_group
//					if(getGroupOrder()>ROSTER_NORMAL_HAS_OFFLINE)
//					{
//						addContact(newContact,getGroupOrder(),app.getCurrentGroupData());
//					}
//					//#endif
//					//#ifdef support_save_rms
//					//if(isSaveRMS)
//					{
//						//addContactToRMS(newContact);	
//						addContactHeadToRMS(newContact);
//					}
//					//#endif
//					
//					
//					// 新增加了一个联系人 联系人页面需要刷新
//					//eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
//				} 
//				else
//				// 已有联系人 更新
//				{
//					if(!isChangeBlockState)
//						contactFlag=contact.CONTACT_contactFlag;
//					updateContact(contact,action,jid,imid,contactFlag,
//							nodeVec,block,nickname,gbcode,groupId,
//							impresa,phone,portraithash,gleam,url);
//				}
//			} 
//			else if (sync != null && sync.equals("deleted"))// 删除的联系人
//			{
//				Contact c = (Contact)getContact(jid);
//				byte oldState = STATE_OFFLINE;
//				String deleteImid=imid;
//				if (c != null)
//				{
//					oldState = c.SUBITEM_STATE;
//					deleteImid=c.CONTACT_imid;
//				}
//				//#ifdef support_save_rms
//				//removeContactFromRMS(jid);
//				deleteContactHeadFromFile(deleteImid);
//				//#endif
//				deleteSubItem(jid, oldState,app.groupData);
//				
//				///////////////////
//				//#ifdef support_exchange_contact_group
//				if(getGroupOrder()>ROSTER_NORMAL_HAS_OFFLINE)
//				{
//					deleteSubItem(jid,oldState,app.getCurrentGroupData());
//				}
//				//#endif
//				//////////////////////
//				
//				if (action != null)
//				{
//					if (action.equals("delete"))// 表示删除联系人成功
//						eventNotify(EventListener.EVENT_DELETE_CONTACT_OK, jid);
//					else if (action.equals("rmblk"))// 删除并阻止
//						eventNotify(EventListener.EVENT_DELETE_BLOCK_CONTACT_OK,jid);
//
//					//eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
//				} 
//				//else
//				//{
//					// 新删减了一个联系人 联系人页面需要刷新
//					//eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
//				//}
//
//			}
//		} 
//		catch (Exception e)
//		{
//			e.printStackTrace();
//		}
	}

	
	public void receiveRosterComplete(String last, String version,String action) 
	{
		// TODO Auto-generated method stub
	    try {
        if(version !=null&&!version.equals(this.myContactGroupVersion))// 联系人更新完毕
        {
			myContactGroupVersion=updateNewVersion(this.myContactGroupVersion,version);
			 //#ifdef support_save_rms
			setContactGroupVerToRMS(this.myContactGroupVersion);
			//#endif
        }
        if(action==null||action.length()==0)
        {
        	if(saveDataTimer != null)
			{
				saveDataTimer.cancel();
				saveDataTimer = null;
			}			
			saveDataTimer = new Timer();        	
        	saveDataTimer.schedule(new Animation(this,"saveData"), 15000);
        }        	
	}
	       catch(Exception e)
	        {
	            e.printStackTrace();
	            MsnUtil.DebugOutPutFile("MSNAPP 2676" + e.getMessage());
	        }
	}

	
	public void receiveSessionId(String sid) 
	{
		// TODO Auto-generated method stub
		jabber.setSessionId(sid);
	}

	
	public void receiveSynchronizeError() 
	{
		// TODO Auto-generated method stub
		eventListener.eventAction(EventListener.EVENT_RECEIVE_SYNCHRONIZE_ERROR, null);
	}

	
	public void receiveUpdateUI() 
	{
		// TODO Auto-generated method stub
		eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
	}

	
	public void receiveVcardTemp(String nickname, String impresa,
			String portrait_data, String resourceId,String url,
			Vector nodeVec) 
	{
		setVcard(nickname, impresa, portrait_data,resourceId,url, nodeVec);
		if(isLoginSuccess)
		{
			eventNotify(EventListener.EVENT_MY_VCARD_RECEIVED, null);
		}
		else
		{
			isLoginSuccess=true;
			if(saveDataTimer != null)
			{
				saveDataTimer.cancel();
				saveDataTimer = null;
			}			
			saveDataTimer = new Timer();        	
        	saveDataTimer.schedule(new Animation(this,"saveData"), 15000);
			eventNotify(EventListener.EVENT_LOGIN_SUCCESS, null);
			eventNotify(EventListener.EVENT_SAVE_LOGIN_USER,null);
	        //Message msgSaveContactList=new Message();
	        //msgSaveContactList.what=EventListener.EVENT_SAVE_CONTACT_LIST ; 
	        //GetMSNHandler().sendMessageDelayed(msgSaveContactList, 5000);
		}
		
	}

	
	public void receiveVerify(String imid, String action, String nickname,
			String realname, String sex, String age, String province,
			String desc) 
	{
		// TODO Auto-generated method stub
		Contact object=getContact(imid,false);
		if(object!=null)//极端情况：如果联系人列表中已经有这个人 则不添加验证请求了
			return;
	    Item friend=new Item();
	    if(imid.indexOf(MSN_CHINA_DOMAIN)!=-1)
	    	friend.VERIFY_isHasProfile=true;
	    friend.VERIFY_age=age;
	    friend.VERIFY_desc=desc;
	    friend.VERIFY_imid=imid;
	    friend.VERIFY_nickname=nickname;
	    friend.VERIFY_realname=realname;
	    friend.VERIFY_province=province;
	    friend.VERIFY_sex=sex;
	    verifyFriendVector.addElement(friend);
	    
		eventNotify(EventListener.EVENT_RECEIVE_VERIFY_FRIEND,null);
	}

	
	public void setCacheStatus(boolean isCacheEmpty) 
	{
		// TODO Auto-generated method stub
		jabber.setCacheStatus(isCacheEmpty);
	}

	
	public void receiveLoginPicaSuccess() 
	{
		try {
		eventNotify(EventListener.EVENT_RECEIVE_LOGIN_PICA_OK,null);
		////////
		//this.data_method="000000";
		////////
		jabber.sendLoginMSN(
				this.GetLiveID(),
				this.GetPassWord(),
				this.sStatus,
				"WVGateway-MMIMGatewaySE.K700c_0100-0.800.360.cn",
				VERSION,
				SOURCE,
				"all",
				jabber.msn_carrier,
				CLIENT_CAPABILITY,
				PROFILE_PORTAITSIZE,
				CONTACT_PORTRAITSIZE,
				devicedId,
				//this.myVcardVersion,
				this.myContactGroupVersion,
				this.myVCARD_resourceId,
				jabber.transParent,
				null,
				"audio/amr",
				this.data_method);
		
		}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}

	
	public void receiveContactChangeState(String type, String from,
			String version, String show, String nickname, String gbcode,
			String impresa, String portraithash, String gleam, String url,
			String phone, Vector nodeVec) {
		// TODO Auto-generated method stub
		//Log.v("start=:", ""+System.currentTimeMillis());
		Object[] obj = new Object[12];
		obj[0] = type;
		obj[1] = from;
		obj[2] = version;
		obj[3] = show;
		obj[4] = nickname;
		obj[5] = gbcode;
		obj[6] = impresa;
		obj[7] = portraithash;
		obj[8] = gleam;
		obj[9] = url;
		obj[10] = phone;
		obj[11] = nodeVec;
		eventNotify(EventListener.EVENT_RECEIVE_CONTACT_CHANGE_STATE, obj);
//		Contact contact = getContact(from);
//		if (contact != null)
//		{	
//			//某时候，服务器可能返回的在线人 没有type=available 所以这里认为凡是type为空 则认为它为在线
//			if(type==null)
//				type="available";
//			
//			updateContactPresence(contact,type, from, version, show, nickname,gbcode, impresa, portraithash,gleam,url,phone,nodeVec);
//
//			eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI,null);
//			
//			eventNotify(EventListener.EVENT_UPDATE_CHAT_UI,contact);
//		} 
		//Log.v("end=:", ""+System.currentTimeMillis());
	}

	/**删掉本地保存的
	 * 删掉内存里面的联系人分组和联系人*/
	public void receiveAdditonEqualsZero() 
	{
		try
		{
			deleteAllRosterData(app.GetLiveID());
			removeContactGroupVersionFromRMS();
		}
		catch (Exception e)
		{
		    try
		    {
			removeContactGroupVersionFromRMS();
		    }
		    catch (Exception ee)
		    {
		        e.printStackTrace();
		    }
			e.printStackTrace();
		}
		//删掉内存里面的联系人分组和联系人
		eventNotify(EventListener.EVENT_RECEIVE_ADDITION_EQUALS_ZERO, null);
	}
	

	
	/**更改个人资料
	 * @param portrait_data
	 * */
	synchronized void setVcard(String nickname, String impresa,String portrait_data,
			String resourceId,String url,Vector nodeVec)
	{
   	 	//#ifdef support_save_rms
		boolean updateRms=false;
		boolean updateHeadRms=false;
		//#endif
		try {
		if(nodeVec.contains("nickname"))
		{
			if(myVCARD_myNickname==null||!myVCARD_myNickname.equals(nickname))
			{
				myVCARD_myNickname=nickname;
	       	 	//#ifdef support_save_rms
				updateRms=true;
				//#endif
			}
		}
		if(nodeVec.contains("impresa"))
		{
			if(myVCARD_myImpresa==null||!myVCARD_myImpresa.equals(impresa))
			{
				myVCARD_myImpresa=impresa;
	       	 	//#ifdef support_save_rms
				updateRms=true;
				//#endif
			}
		}
		if (nodeVec.contains("data"))// 说明有节点 头像  带内
		{	
			 //#ifdef support_head_image
			byte[] newData=null;
			int newLength=0;
			if (portrait_data != null && portrait_data.length() > 0)
			{
				newData=MsnUtil.decode(portrait_data);
				newLength=newData.length;
	       	 	//#ifdef support_save_rms
				if(myVCARD_headImageDataLength!=newLength)
				{
					updateHeadRms=true;
				}
				//#endif
				setHeadImage(newData);
			} 
			else//如果为空 说明自己的头像被删除了
			{	
				//#ifdef support_save_rms
				if(myVCARD_headImageDataLength!=myVCARD_headImageDataLength)
					updateHeadRms=true;
				//#endif
				setHeadImage(null);		
			}
			//#endif
		}
		if(nodeVec.contains("url")) //带外 
		{
			 //#ifdef support_head_image
			//byte[] newData=null;
			//int newLength=0;
			if (url != null && url.length() > 0)
			{
				/////// 带外
				Contact contact=new Contact();
				contact.CONTACT_OUTER_HEAD_ONESELF_IMID=this.GetLiveID();
				new FileOuterTransfer(this,FileOuterTransfer.command_downloadFile,null,contact,url);
				app.isTransferingOuterSendHead=true;//
			} 
			else//如果为空 说明自己的头像被删除了
			{	
				//#ifdef support_save_rms
					updateHeadRms=true;
				//#endif
				setHeadImage(null);		
			}
			//#endif
		}
		
		if(resourceId!=null)
		{
			boolean isChange=compareTwoValueIsChange(myVCARD_resourceId,resourceId);
			if(isChange)
			{
				myVCARD_resourceId=resourceId;
				 //#ifdef support_save_rms
				setVcardResourceIDToRMS(this.GetLiveID(),myVCARD_resourceId);
				//#endif
			}
		}

		//#ifdef support_save_rms
		if(updateRms)
		{
			addVcardToRMS(myVCARD_myNickname,myVCARD_myImpresa);
		}
		if(updateHeadRms)
		{
			deleteVcardHead();
			addVcardHeadToRMS();
		}
		//#endif
		nodeVec.removeAllElements();
		nodeVec=null;
	}	
	      catch(Exception e)
	        {
	            e.printStackTrace();
	        }
	}	
	
	/**比较两个字符串 如果有更改 则返回true 否则返回false
	 * @param oldValue 老值
	 * @param newValue 新值
	 * */
	public static boolean compareTwoValueIsChange(String oldValue,String newValue)
	{
		if(oldValue==null)
		{
			if(newValue!=null)
				return true;
			else 
				return false;
		}
		else
		{
		    if(newValue == null)
		    {
		        return false;
		    }
			if(oldValue.equals(newValue))
				return false;
			else
				return true;		
		}
	}
	
	public String updateNewVersion(String currentVer,String newVer)
	{
		if(currentVer==null)
			return newVer;
		else
		{   
			//客户端在收到服务器返回的同步版本时，只更新收到的部分，没有收到的部分不做更新。
			String[] currentMyVcardVer=parseVersion(currentVer);
			String[] newMyVcardVer=parseVersion(newVer);
			for(int i=0;i<newMyVcardVer.length;i++)
			{
				if(newMyVcardVer[i]!=null&&newMyVcardVer[i].length()>0)
					currentMyVcardVer[i]=newMyVcardVer[i];
			}
			return updateVersion(currentMyVcardVer);
		}
	}
	
	/**解析版本号*/
	public String[] parseVersion(String version)
	{
		Vector temp=new Vector();
		StringBuffer sb=new StringBuffer();
		for(int i=0;i<version.length();i++)
		{
			if(version.charAt(i)!='|')
				sb.append(version.charAt(i));
			else
			{
				temp.addElement(sb.toString());
				sb.delete(0,sb.toString().length());
			}
		}
		temp.addElement(sb.toString());
		String[] myVcardVersion=new String[temp.size()];
		temp.copyInto(myVcardVersion);
		return myVcardVersion;
	}
	
	public String updateVersion(String[] ver)
	{
		StringBuffer sb=new StringBuffer();
		for(int i=0;i<ver.length;i++)
		{
			sb.append(ver[i]);
			if(i<ver.length-1)
				sb.append('|');//减去最后一个'|'符号
		}
		return sb.toString();
	}
	
	/**
	 * 保存该用户的deviceID到RMS
	 */
	public void setDeviceIdToRMS(String username,String deviceId)
	{
	    try {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		app.DB.setDeviceId_Data(strLiveID, deviceId);
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}
	
	/**保存该用户个人资料的ID号到RMS*/
	public void setVcardResourceIDToRMS(String username,String id)
	{
	    try {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		app.DB.setVCARD_ResourceId_Data(strLiveID, id);
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}
	
	/**
	 * 保存该用户个人资料的版本号到RMS
	 */
//	public void setVcardVerToRMS(String username,String ver)
//	{
//		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
//		app.DB.SetVCARD_Version_Data(strLiveID, ver);
//	}
	
	/**新增一个 个人资料保存到rms*/
	public void addVcardToRMS(String myVCARD_myNickname,String myVCARD_myImpresa)
	{
	    try {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		app.DB.setVCARD_Nickname_Impresa_Data(strLiveID, myVCARD_myNickname,myVCARD_myImpresa);
	    }
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}

	/**将分组添加进列表*/
//    public void addGroupToList(Item group)
//    {
//    	Map<String, Item> CurGroup = new HashMap<String, Item>();      
//        app.groupData.add(CurGroup);
//        CurGroup.put("GROUP", group);
//        List<Map<String, Item>> childData = new ArrayList<Map<String, Item>>();   
//        app.childData.add(childData);       
//    }
	
	/**添加一个分组 仅仅是为了匹配原有接口而保留 实际执行 addGroupToList(Item group)操作*/
//	private void addGroup(Item group)
//	{
//		addGroupToList(group,app.groupData,app.childData);
//		if(this.getGroupOrder()>ROSTER_NORMAL_HAS_OFFLINE)
//			addGroupToList(group,getCurrentGroupData(),getCurrentChildData());
//	}
    
    /**按照gbcode排序添加分组
     * */
    public void addGroupToList(Group group,Vector<Group> appgroupData)
    {
        try
        {
    	//Group curGroup=null;
    	//List<Map<String, Contact>> childData=null;
    	if(appgroupData.size()==0)
    	{
        	//curGroup = new HashMap<String, Group>();      
        	//appgroupData.add(curGroup);
            //curGroup.put("GROUP", group);
    		appgroupData.add(group);
            //childData = new ArrayList<Map<String, Contact>>();   
            //appchildData.add(childData); 
    	}
    	else
    	{
    		Group currentGroup;
			for (int i = 0; i < appgroupData.size(); i++)
			{
//				Map<String, Group> currentGroupMap = appgroupData.get(i);
//				Group currentGroup=(Group)currentGroupMap.get("GROUP");
				currentGroup = null;
				currentGroup=appgroupData.elementAt(i);
				if(currentGroup==group)//有相同的 不再添加
					return;
				if (compareGroup(currentGroup, group))
				{
					if (i == appgroupData.size()-1)
					{
			        	//curGroup = new HashMap<String, Group>();      
			        	//appgroupData.add(i,curGroup);
			        	//appgroupData.add(appgroupData.size(),curGroup);
			            //curGroup.put("GROUP", group);
						//appgroupData.add(appgroupData.size(), curGroup);
						appgroupData.add( group);
						
			            //childData = new ArrayList<Map<String, Contact>>();   
			            //appchildData.add(i,childData); 
			            //appchildData.add(appgroupData.size()-1,childData); 
			            break;
					}				
					continue;
				} 
				else
				{
//		        	curGroup = new HashMap<String, Group>();      
//		        	appgroupData.add(i,curGroup);
//		            curGroup.put("GROUP", group);
					appgroupData.add(i,group);
					//appgroupData.insertElementAt(group,i);
		            //childData = new ArrayList<Map<String, Contact>>();   
		            //appchildData.add(i,childData); 
		            break;
				}		
			}
    	}
    }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }
    
    /**
     * @param group1			要比较的分组1
     * @param group2			要比较的分组2
     * @return					group1是否应该排于group2之前
     */
    private boolean compareGroup(Group group1,Group group2)
    {
    	//相同类型的分组 如果是联系人分组 则按照gbcode 否则按顺序添加
    	if(group1.GROUP_flag==group2.GROUP_flag)
    	{
    		String gbcode1 = group1.GROUP_gbcode;
			String gbcode2 = group2.GROUP_gbcode;

			if (gbcode1 == null)//频道 聊天室 广告 在线/脱机分组 没有gbcode
			{
				gbcode1 = "1";
			}
			if (gbcode2 == null)
			{
				gbcode2 = "2";
			}
			return gbcode1.compareTo(gbcode2) < 0;
    	}
    	else
    	{
    		return group1.GROUP_flag<group2.GROUP_flag;
    	}

    }
    
    
	/**将联系人 按照分组ID添加进对应的分组中*/
    public void addContactToList(Group curGroup,Contact contact,String contactTempGroupID)
    {  
        try {
    	sortAddContact(curGroup,contact);
    }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }
    
    /**按照gbcode和状态排序添加联系人
     * */
    private void sortAddContact(Group curGroup,Contact subItem)
    {
        try {
    	int itemMaxNum = 0;
    	//Group group=(Group)curGroup.get("GROUP");
    	Group group=curGroup;
    	itemMaxNum = group.GROUP_itemnum; // 该组下列表项的数量
    	//Map<String, Contact> curChild = new HashMap<String, Contact>(); 
    	
		if (itemMaxNum == 0)
		{ // 所要加入的组是空组,则直接添加
			//childData.add(curChild);
		    //curChild.put("CONTACT", subItem);
			group.contactVector.addElement(subItem);
		} 
		else
		{
			//for(int s=0;s<childData.size();s++)
			Contact item;
			for(int s=0;s<group.contactVector.size();s++)
    		{
//    			Map<String, Contact> CurChild = new HashMap<String, Contact>(); 
//    			CurChild=childData.get(s);
//    			Contact item=CurChild.get("CONTACT");
				item = null;
    			item=group.contactVector.elementAt(s);
    			
				if (subItem.CONTACT_imid.equals(item.CONTACT_imid))//有相同的 不加入了
				{
					return;
				}		
				if (compareContact(item, subItem))
				{
					//if (s == childData.size()-1)
					if(s==group.contactVector.size()-1)
					{
						//childData.add(s, curChild);
						//childData.add(childData.size(), curChild);
						//curChild.put("CONTACT", subItem);
						group.contactVector.add(subItem);
						break;
					}
					//continue;
				} 
				else
				{
					//childData.add(s, curChild);
					//curChild.put("CONTACT", subItem);
					group.contactVector.add(s,subItem);
					break;
				}
    		}
		}
    }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }
    
    /**
     * @param con1				要比较的联系人1
     * @param con2				要比较的联系人2
     * @return					con1是否应该排于con2之前
     */
    private boolean compareContact(Contact con1, Contact con2)
    { 
      	byte weight1 = con1.SUBITEM_STATE;
    	byte weight2 = con2.SUBITEM_STATE;
    	
    	
    	//频道和聊天室 都是脱机的
//    	if(con1.SUBITEM_TYPE==TreeList.SUBITEM_CONTACT)
//    	{
//    		if(con1.SUBITEM_STATE!=Constant.STATE_OFFLINE)
//    		{
//    			weight1=Constant.STATE_ONLINE;//所有在线状态（含打电话等）都认为在线	
//    		}
//    	}
//    	if(con2.SUBITEM_TYPE==TreeList.SUBITEM_CONTACT)
    	{
    		if(con1.SUBITEM_STATE!=MSNApplication.STATE_OFFLINE)
    		{
    			weight1=MSNApplication.STATE_ONLINE;//所有在线状态（含打电话等）都认为在线	
    		}
    		if(con2.SUBITEM_STATE!=MSNApplication.STATE_OFFLINE)
    		{
    			weight2=MSNApplication.STATE_ONLINE;//所有在线状态（含打电话等）都认为在线	
    		}
    	}
  
    	//先比较在线状态相同的
    	if(weight1 == weight2)
    	{
				String gbcode1 = con1.CONTACT_gbcode;
				String gbcode2 = con2.CONTACT_gbcode;

				if (gbcode1 == null)//频道和聊天室没有gbcode
				{
					gbcode1 = "1";
				}

				if (gbcode2 == null)
				{
					gbcode2 = "2";
				}

				return gbcode1.compareTo(gbcode2) < 0;
		}
		else
		{
			return weight1 < weight2;
		}
    }
    
    /**根据组id 得到一个分组对象*/
    public Group getGroup(String groupId,Vector<Group> appgroupData)
    {
    	Group currentGroup=null;
    	String currentID;
    	for(int i = 0; i < appgroupData.size(); i++)
    	{
//    		Map<String, Group> CurGroup = appgroupData.get(i);
//    		currentGroup=(Group)CurGroup.get("GROUP");
    		currentGroup=appgroupData.elementAt(i);
    		currentID = null;
    		currentID=currentGroup.GROUP_id;
    		if(currentID.equals(groupId))
    		{
    			return currentGroup;
    		}
    	} 
    	return null;
    }
    
    
	
	/**删除分组 并将该组下所有联系人都转移到缺省分组里
	 * 不用考虑删除缺省分组的情况(group id='0' sync='deleted')
	 * @param groupId 分组id
	 * @return true 添加了缺省分组 false 没有添加
	 * */
    public boolean removeGroup(String groupId,Vector<Group> appgroupData)
	{
		try
		{
			Group group = getGroup(groupId,appgroupData);
			if(group==null)
				return false;
			else
			{
				//将该组下的所有联系人都转移到缺省分组里 并删除该分组
				return deleteGroupContactAndAddToDefaultGroup(groupId,appgroupData);		
			}
		} 
		catch (Exception e)
		{	
			//e.printStackTrace();
			return false;
		}
	}
    
    
    

    /**删除一个分组下面的所有联系人 并且转移到缺省组
     * 如果添加了缺省组 该函数会保存到rms
     * @return true 添加了一个缺省组
     * */
    public boolean deleteGroupContactAndAddToDefaultGroup(String groupId,Vector<Group> appgroupData)throws Exception
	{
    	Group currentGroup;
		boolean isAddDefault=false;//是否需要添加新的缺省组
		Contact currentContact;
		String[] temp;
		StringBuffer newContactGroupId;
		Contact addContact;
		Group defaultGroup;
		Vector addVc;
		for (int i = 0; i < appgroupData.size(); i++)
		{	
			//Map<String, Group> CurGroupMap = appgroupData.get(i);
    		//currentGroup=(Group)CurGroupMap.get("GROUP");
			currentGroup = null;
			currentGroup=appgroupData.elementAt(i);
			if (currentGroup.GROUP_id!=null&&currentGroup.GROUP_id.equals(groupId))
			{
					// 组id相同才认为是相同的分组
				addVc = new Vector();//vc用来装需要添加的所有联系人
				//List<Map<String, Contact>> childData  = appchildData.get(i);
				//appgroupData.remove(CurGroupMap);
				appgroupData.remove(currentGroup);
				if(groupId==DEFAULT_GROUP_ID)
				{
					isAddDefault=true;
				}
				
				for(int k=0;k<currentGroup.contactVector.size();k++)
				{
//					Map<String, Contact> CurChild = new HashMap<String, Contact>(); 
//					CurChild= childData.get(k);
//					Contact currentContact=CurChild.get("CONTACT");
//					childData.remove(CurChild);
					currentContact = null;
					currentContact = currentGroup.contactVector.elementAt(k);
					
					//k--;
					if(currentContact.CONTACT_groupId.indexOf(",")!=-1)//有多个分组存在
					{
							//不转移到缺省组
							//对该联系人的groupid重新赋值
							temp = MsnUtil.split(currentContact.CONTACT_groupId,',');
							newContactGroupId=new StringBuffer();
							for(int j=0;j<temp.length;j++)
							{
								if(!temp[j].equals(groupId))
								{
									newContactGroupId.append(temp[j]);	
									newContactGroupId.append(",");
								}
							}
							currentContact.CONTACT_groupId=newContactGroupId.toString();
							//去掉最后一个','符号
							currentContact.CONTACT_groupId=currentContact.CONTACT_groupId.substring(0,currentContact.CONTACT_groupId.length()-1);
			           	 	//#ifdef support_save_rms
							//addContactToRMS(currentContact);
							//#endif
					}
					else 
					{  
							//先判断当前有没有缺省组，如果没有，就先添加一个缺省分组
						if(getGroup(DEFAULT_GROUP_ID,appgroupData)==null)
						{
							isAddDefault=true;
						}
						currentContact.CONTACT_groupId=DEFAULT_GROUP_ID;//赋值组id为0				
						//
						addVc.addElement(currentContact);
					}	
				}
				currentGroup.contactVector.removeAllElements();
				if(isAddDefault)
				{
	        		if(DEFAULT_GROUP_NAME==null||DEFAULT_GROUP_NAME.length()==0)
	        		{
	        			DEFAULT_GROUP_NAME=(String)mActivity.getText(R.string.about);
	        		}
	        		defaultGroup = null;
	        		defaultGroup=newGroup(DEFAULT_GROUP_ID, DEFAULT_GROUP_GBC, DEFAULT_GROUP_NAME, GROUP_USER_FLAG,false);
						
						
						
						addGroupToList(defaultGroup,appgroupData);
		    			/////////////////
		           	 	//#ifdef support_save_rms
						//只有正常列表 才保存到rms 其他派生列表 不再保存
						//if(this.getGroupOrder()==ROSTER_NORMAL_HAS_OFFLINE)
						//	addGroupToRMS(defaultGroup);
						//#endif
				 }
					
				 for(int m=0;m<addVc.size();m++)
				 {
					 addContact = null;
					 addContact=(Contact)addVc.elementAt(m);
					 addSubItem(addContact,false,appgroupData);
		           	 //#ifdef support_save_rms
					 //if(this.getGroupOrder()==ROSTER_NORMAL_HAS_OFFLINE)
				   		//	addContactToRMS(addContact);
					 //#endif
				 }
				 addVc.removeAllElements();
				 //appchildData.remove(childData);  
				 return isAddDefault;
			}
		}
		return isAddDefault;
	}
    
	//#ifdef support_save_rms
	/**删除所有联系人及联系人分组*/
	public synchronized void deleteAllUserGroupAndContact(Vector<Group> appgroupData)
	{
		Group currentGroup;
		try {
		for (int i = 0; i < appgroupData.size(); i++)
		{	
//			Map<String, Group> CurGroupMap = appgroupData.get(i);
//    		currentGroup=(Group)CurGroupMap.get("GROUP");
			currentGroup = null;
			currentGroup=appgroupData.elementAt(i);
			if(currentGroup==null)
				return;

			//List<Map<String, Contact>> childData  = appchildData.get(i);
			//appgroupData.remove(CurGroupMap);
			currentGroup.contactVector.removeAllElements();
			appgroupData.remove(currentGroup);
//			for(int k=0;k<childData.size();k++)
//			{
//				Map<String, Contact> CurChild = new HashMap<String, Contact>(); 
//				CurChild= childData.get(k);
//				//Item currentContact=CurChild.get("CONTACT");
//				childData.remove(CurChild);
//				k--;
//			}
		}
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}
	//#endif
    
    /**
     * 	生成一个分组并返回
     * @param groupId 组id
     * @param gbcode  组gbcode
     * @param groupname 组名称
     * @param groupflag 组类型（现阶段只有用户分组）
     * @param isExpand 是否展开
     * @return 新生成的分组对象（Group）
     */
	public Group newGroup(String groupId, String gbcode, String groupname, byte groupflag,boolean isExpand)
	{
		Group group=new Group();
	    //group.TreeList_TYPE=TreeList.TYPE_GROUP;
	    group.GROUP_id=groupId;
	    group.GROUP_gbcode=gbcode;
	    group.GROUP_name=groupname;
	    group.GROUP_flag=groupflag;
	    group.GROUP_isexpand=isExpand;
	    group.GROUP_itemnum=0;
	    group.GROUP_maxweight_itemnum="0";
	    return group;
	} 
	
	
	/**
	 * 生成一个联系人对象
	 * @param state 在线状态
	 *@param jid 联系人id
	* @param imid 邮箱
	* @param nickname 昵称
	* @param gbcode gbcode码
	* @param groupId 所属分组id,类似: 1,2,3
	* @param contactFlag 联系人标志位 阻止或者普通
	* @param impresa 心情短语
	* @param portraithash 头像hash
	* @param statusShow 状态说明
	* @param phone 电话号码
	* @param gleam 个人空间是否更新
	* @param url  空间地址
	* @param imageData 头像数据
	 * @return 该联系人对象
	 */
	public Contact newContact(byte state,String jid, String imid, String nickname,
			String gbcode, String groupId, byte contactFlag,
			String portraithash,String impresa,String statusShow ,String phone,
			String gleam, String url,byte[] imageData)
	{
		Contact newContact=new Contact();
		
		//newContact.SUBITEM_TYPE=TreeList.SUBITEM_CONTACT;
		newContact.SUBITEM_STATE=state;;
		//newContact.TreeList_TYPE=TreeList.TYPE_SUBITEM;
		newContact.SUBITEM_JID=jid;
		newContact.CONTACT_imid=imid;
		newContact.CONTACT_nickname=nickname;
		if(newContact.CONTACT_nickname==null||newContact.CONTACT_nickname.length()==0)
			newContact.CONTACT_nickname=newContact.CONTACT_imid;
		newContact.CONTACT_gbcode=gbcode;
		newContact.CONTACT_groupId=groupId;
		newContact.CONTACT_contactFlag=contactFlag;
		newContact.CONTACT_impresa=impresa;
		newContact.CONTACT_statusShow=statusShow;
		newContact.CONTACT_phone=phone;
		if(gleam==null)
			newContact.CONTACT_gleam="0";
		else
			newContact.CONTACT_gleam=gleam;
		
		newContact.CONTACT_url=url;
		//#ifdef support_head_image
		if(portraithash != null)
		newContact.CONTACT_portraithash=portraithash;
		newContact.CONTACT_imageData=imageData;
		//#endif
		
		return newContact;
	}
	
	
	/**
	 * 获取指定jid的联系人信息
	 * @param jid
	 */
	public synchronized final Contact getContact(String jid)
	{
	    return getContact(jid,true);
	}
    
	/**根据参数得到联系人
	 * @param jidOrImid jid或者imid
	 * @param isJid true为jid false为imid
	 * */
	public Contact getContact(String jidOrImid,boolean isJid)
	{
		Contact contact = null;
		//List<Map<String, Contact>> childData = null;
		Group group;
		for (int i = 0; i < app.groupData.size(); i++)
		{
			group = null;
			group=app.groupData.elementAt(i);
			
			
//			childData =app.childData.get(i);
//	    	Map<String, Contact> CurChild = new HashMap<String, Contact>(); 
//	    	for(int k=0;k<childData.size();k++)
			for(int k=0;k<group.contactVector.size();k++)
	    	{
//	    		CurChild=childData.get(k);
//	    		contact=(Contact)CurChild.get("CONTACT");
				contact = null;
				contact=group.contactVector.elementAt(k);
				if(isJid)
				{
					if(contact.SUBITEM_JID.equals(jidOrImid))
					{						
						return contact;
					}
				}
				else
				{
					
					if(contact.CONTACT_imid.toLowerCase().equals(jidOrImid.toLowerCase()))
						return contact;
				}
	    	}
		}
		return null;
	}
	
    /**
	 * 增加(修改)一个联系人数据到记录库中.
	 */
	public void addContactToRMS(Contact contact)
	{
	    try {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		app.DB.insertRosterData(strLiveID, contact.SUBITEM_JID, contact.CONTACT_imid, contact.CONTACT_nickname, contact.CONTACT_gbcode, contact.CONTACT_groupId, contact.CONTACT_portraithash, contact.CONTACT_impresa, contact.CONTACT_gleam, contact.CONTACT_url);		
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}

    
	/**
	 * 增加一个分组到自己记录库中
	 */
//	public void addGroupToRMS(Item group)
//	{
//		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
//		app.DB.InsertGroupData(strLiveID, group.GROUP_id, group.GROUP_gbcode, group.GROUP_name, Byte.toString(group.GROUP_flag));
//	}
    
	/**
	 * 从记录库中删除指定id的分组数据.
	 * @param groupId
	 */
	public void removeGroupFromRMS(String groupId)
	{
	    try {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		app.DB.deleteOneGroupData(strLiveID, groupId);
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}
	
	/**删除一个设备id 置为null*/
	public void removeDeviceIDFromRMS()
	{
	    try
	    {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		app.DB.setDeviceId_Data(strLiveID, null);
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}
	
	/**删除分组的版本号 置为null*/
	public void removeContactGroupVersionFromRMS()
	{
	    try {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		app.DB.setContact_GroupVersion_Data(strLiveID, null);
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}
	
	/**删除数据库里面 当前用户的所有分组数据*/
	public void removeAllGroupFromRMS()
	{
	    try
	    {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		app.DB.deleteGroupData(strLiveID);
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}
	
	/**删除数据库里面 当前用户的所有联系人数据*/
	public void removeAllContactFromRMS()
	{
	    try {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		app.DB.deleteRosterData(strLiveID);
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}
	
	/**
	 *从记录库中删除指定jid的联系人数据.
	 * @param contactJid
	 */
	public void removeContactFromRMS(String contactJid)
	{
		
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		
		app.DB.deleteOneRosterData(strLiveID, contactJid);
	}
	

	
	/**
	 * 设置记录库存储的联系人数据版本号.
	 * @param version
	 */
	public void setContactGroupVerToRMS(String version)
	{
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		app.DB.setContact_GroupVersion_Data(strLiveID, version);
	}
	
	
	
//	/**
//	* 把联系人添加到列表中，可能存在多个分组，要添加全，并添加进RMS数据库
//	* @param jid 联系人id
//	* @param imid 邮箱
//	* @param nickname 昵称
//	* @param gbcode gbcode码
//	* @param groupId 所属分组id,类似: 1,2,3
//	* @param contactFlag 联系人标志位 阻止或者普通
//	* @param impresa 心情短语
//	* @param portrait 头像
//	* @param phone 电话号码
//	* @param gleam 个人空间
//	* @param url  空间地址
//	* @param version 联系人列表版本号
//	* @param hasSendGetImage 是否发送获取头像 如果头像数据有更新 则肯定是false
//	* @param isSaveRMS是否保存到RMS中 低端手机可以考虑不保存
//	*/
//	synchronized void addContact(Item newContact,boolean isSaveRMS)
//	{
//	
////		Item newContact=new Item();
////		
////		//newContact.SUBITEM_TYPE=TreeList.SUBITEM_CONTACT;
////		//newContact.SUBITEM_STATE=Constant.STATE_OFFLINE;
////		//newContact.TreeList_TYPE=TreeList.TYPE_SUBITEM;
////		newContact.SUBITEM_JID=jid;
////		newContact.CONTACT_imid=imid;
////		newContact.CONTACT_nickname=nickname;
////		if(newContact.CONTACT_nickname==null||newContact.CONTACT_nickname.length()==0)
////			newContact.CONTACT_nickname=newContact.CONTACT_imid;
////		newContact.CONTACT_gbcode=gbcode;
////		newContact.CONTACT_groupId=groupId;
////		newContact.CONTACT_contactFlag=contactFlag;
////		newContact.CONTACT_impresa=impresa;
////		newContact.CONTACT_statusShow=statusShow;
////		newContact.CONTACT_phone=phone;
////		if(gleam==null)
////			newContact.CONTACT_gleam="0";
////		else
////			newContact.CONTACT_gleam=gleam;
////		
////		newContact.CONTACT_url=url;
////		//#ifdef support_head_image
////		newContact.CONTACT_portraithash=portraithash;
////		newContact.CONTACT_imageData=imageData;
////		newContact.CONTACT_hasSendGetImage=hasSendGetImage;
////		//#endif
//		try
//		{
//			addContact(newContact);
//			
//			///////////////////
//			//#ifdef support_exchange_contact_group
////			if(getGroupOrder()>Constant.ROSTER_NORMAL_HAS_OFFLINE)
////			{
////				getCurrentRoster().addContact(newContact);
////			}
//			//#endif
//			//////////////////////
//			
//			
//			//#ifdef support_save_rms
//			if(isSaveRMS)
//			{
//				addContactToRMS(newContact);	
//			}
//			//#endif
//		} 
//		catch (Exception e)
//		{
//			//e.printStackTrace();
//		}
//	}
	
	private void updateContact(Contact contact,String action,String jid,String imid,byte contactFlag,
			Vector nodeVec,String block,String nickname,String gbcode,String groupId,
			String impresa,String phone,String portraithash,String gleam,String url)
	{
		try
		{
			Contact c = contact;
			byte oldState = c.SUBITEM_STATE;
			//boolean updateHeadImg=false;
			
			if (action != null)
			{
				deleteSubItem(jid,oldState,app.groupData);
				
				///////////////////
				//#ifdef support_exchange_contact_group
				if(getGroupOrder()>ROSTER_NORMAL_HAS_OFFLINE)
				{
					deleteSubItem(jid,oldState,this.getCurrentGroupData());
				}
				//#endif
				//////////////////////
				
				c.CONTACT_contactFlag = contactFlag;

				addContact(c,ROSTER_NORMAL_HAS_OFFLINE,app.groupData);
				
				///////////////////
				//#ifdef support_exchange_contact_group
				if(getGroupOrder()>ROSTER_NORMAL_HAS_OFFLINE)
				{
					addContact(c,getGroupOrder(),this.getCurrentGroupData());
				}
				//#endif
				//////////////////////
				
				 //#ifdef support_save_rms
				//addContactToRMS(c);
				//#endif
				if (action.equals("block"))// 表示阻止联系人成功
				{
					eventNotify(EventListener.EVENT_BLOCK_CONTACT_OK, c);
					return;
				} 
				else if (action.equals("unblock"))// 解除阻止
				{
					eventNotify(EventListener.EVENT_UNBLOCK_CONTACT_OK, c);
					return;
				}
				//eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
				//因为页面跳转了 所以肯定重绘 不需要考虑会话窗口页面的情况
			} 
			else
			{
				boolean updateRms = false;// 是否要更新RMS
				if(nodeVec.contains("imid"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_imid,imid);
					if(isTempChange)
					{
						c.CONTACT_imid=imid;
						updateRms=true;
					}
				}
				if (nodeVec.contains("block"))
				{
					if (block != null &&block.equals("1"))
					{
						if (c.CONTACT_contactFlag != CONTACT_BLOCK_FLAG)
						{
							c.CONTACT_contactFlag = CONTACT_BLOCK_FLAG;
							updateRms = true;
						}
					} 
					else if (block != null &&block.equals("0"))
					{
						if (c.CONTACT_contactFlag != CONTACT_NORMAL_FLAG)
						{
							c.CONTACT_contactFlag = CONTACT_NORMAL_FLAG;
							updateRms = true;
						}
					}
				}
				if (nodeVec.contains("nickname"))// 需要利用到旧的变量
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_nickname,nickname);
					if(isTempChange)
					{
						c.CONTACT_nickname=nickname;
						updateRms = true;
					}
					if(c.CONTACT_nickname==null||c.CONTACT_nickname.length()==0)
						c.CONTACT_nickname=c.CONTACT_imid;
				}
				if (nodeVec.contains("gbcode"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_gbcode,gbcode);
					if(isTempChange)
					{
						c.CONTACT_gbcode=gbcode;
						updateRms = true;
					}
				}
				if (nodeVec.contains("group"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_groupId,groupId);
					if(isTempChange)
					{
						c.CONTACT_groupId=groupId;
						updateRms = true;
					}
				}
				if (nodeVec.contains("impresa"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_impresa,impresa);
					if(isTempChange)
					{
						c.CONTACT_impresa=impresa;
						//updateRms = true;
					}
				}
				if(nodeVec.contains("phone"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_phone,phone);
					if(isTempChange)
					{
						c.CONTACT_phone=phone;
						updateRms = true;
					}
				}
				//#ifdef support_head_image
				if (nodeVec.contains("portraithash"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_portraithash,portraithash);
					if(isTempChange)
					{
						c.CONTACT_imageData = null;// 把头像数据删掉
						c.CONTACT_hasSendGetImage = false;
						if(portraithash != null)
						c.CONTACT_portraithash=portraithash;
						updateRms = true;
						//updateHeadImg=true;
					}
					else
					{
					    c.CONTACT_hasSendGetImage = true;
					}
				}
				//#endif
				if (nodeVec.contains("gleam"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_gleam,gleam);
					if(isTempChange)
					{
						c.CONTACT_gleam=gleam;
						updateRms = true;
					}
				}
				if (nodeVec.contains("spaces"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_url,url);
					if(isTempChange)
					{
						c.CONTACT_url=url;
						updateRms = true;
					}
				}
				if (updateRms)// 只在有变化的时候进行操作
				{
					deleteSubItem(jid,oldState,app.groupData);
					///////////////////
					//#ifdef support_exchange_contact_group
					if(getGroupOrder()>ROSTER_NORMAL_HAS_OFFLINE)
					{
						deleteSubItem(jid,oldState,this.getCurrentGroupData());
					}
					//#endif
					//////////////////////
					addContact(c,ROSTER_NORMAL_HAS_OFFLINE,app.groupData);
					///////////////////
					//#ifdef support_exchange_contact_group
					if(getGroupOrder()>ROSTER_NORMAL_HAS_OFFLINE)
					{
						addContact(c,getGroupOrder(),this.getCurrentGroupData());
					}
					//#endif
					//////////////////////
					
					 //#ifdef support_save_rms
					//addContactToRMS(c);
					//#endif
					//this.eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI,null);
				}
			}
		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
    /**
     * 删除联系人
     * @param jid 指定所要删除列表项的jid 因为可能有某联系人存在于多个分组 所以要删除完全
     * @param delEmptGrp 若删除列表项后所在的分组为空了，是否删除该空组。 true-删除空组 false-保留空组
     * @param oldState 删除前的老状态 
     * @throws Exception 
     */
    public void deleteSubItem(String jid, byte oldState,Vector<Group> appgroupData)throws Exception
	{
    	Group group;
    	Contact contact;
    	int oldItemnum;
		int newItemnum;
		try {
    	for(int i=0;i<appgroupData.size();i++)
    	{
    		group = null;
    		group  = appgroupData.elementAt(i);
    		//for(int k=0;k<childData.size();k++)
    		for(int k=0;k<group.contactVector.size();k++)
    		{
//    			Map<String, Contact> CurChild = new HashMap<String, Contact>(); 
//    			CurChild=childData.get(k);
//    			Contact contact=CurChild.get("CONTACT");
    			contact = null;
    			contact = group.contactVector.elementAt(k);
    			
    			if(contact.SUBITEM_JID!=null&&contact.SUBITEM_JID.equals(jid))
    			{
    				group.contactVector.remove(contact);
//    				Map<String, Group> CurGroup = appgroupData.get(i);
//    				Group group=(Group)CurGroup.get("GROUP");
    				//Group group=appgroupData.elementAt(i);
    	    		group.GROUP_itemnum-=1; 
    	    		if (oldState!= STATE_OFFLINE)
					{						
						oldItemnum=Integer.parseInt(group.GROUP_maxweight_itemnum);
						newItemnum=oldItemnum-1;
						group.GROUP_maxweight_itemnum = Integer.toString(newItemnum);				
					}
					if((contact.CONTACT_groupId!=null&&contact.CONTACT_groupId.indexOf(',')!=-1))
					{
						//跳到下一个分组去
						break;
					}
					else//如果不是在多分组存在的 那么删除一次就可以退出
						return;				
    			}
    		}

    	}
		}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }

	}
	
	

	
	/**删除会话列表的所有的会话*/
	public synchronized void deleteAllSessionItem()
	{
		app.sessionData.clear();
	}

	public void setSound(boolean isOpenOnlineSound,boolean isOpenNewMessageSound)
	{
		this.isOpenOnlineSound=isOpenOnlineSound;
		this.isOpenNewMessageSound=isOpenNewMessageSound;
	}
	
	/**是否播放联系人上线音*/
	public boolean  getIsOpenOnlineSound()
	{
		return isOpenOnlineSound;
	}
	
	/**是否能播放新信息提示音*/
	public boolean getIsOpenNewMessageSound()
	{
		return isOpenNewMessageSound;
	}
	
	public boolean getIsNudge()
	{
	    return isNudge;
	}
	
	
	/** 
	 * 更新联系人的状态变化 
	 * */	
	synchronized void updateContactPresence(Contact c,String type,String  from,
			String  version, String show,String  nickname,
			String gbcode, String impresa, String  portraithash,
			String gleam,String url,String phone,Vector nodeVec)
	{
		try
		{
			/**是否需要更新RMS*/
			boolean isUpdateRms = false;
			/**是否改变状态*/
			boolean isChangeState=false;
			
			/**是否需要更新头像的RMS*/
			boolean isUpdateHeadRms = false;
			
			if (c != null)
			{
				byte oldState=c.SUBITEM_STATE;
				if (type!=null&&type.equals("available"))
				{
					if (nodeVec.contains("show"))
					{
						if(show == null)//包含节点 不一定有值 这种情况下默认是"online"
							show=STATE_FLAG_All[0];
						if(c.CONTACT_statusShow==null||!c.CONTACT_statusShow.equals(show))
						{
							if (show.equals(STATE_FLAG_All[0]))	
								c.SUBITEM_STATE=STATE_ONLINE;
							else if (show.equals(STATE_FLAG_All[1]))
								c.SUBITEM_STATE=STATE_BUSY;
							else if (show.equals(STATE_FLAG_All[2]))
								c.SUBITEM_STATE=STATE_AWAY;	
							else if (show.equals("iv"))
								c.SUBITEM_STATE=STATE_OFFLINE;
							else if(show.equals(STATE_FLAG_All[3]))
								c.SUBITEM_STATE=STATE_BeRightBack;
							else if(show.equals(STATE_FLAG_All[4]))
								c.SUBITEM_STATE=STATE_InACall;
							else if(show.equals(STATE_FLAG_All[5]))
								c.SUBITEM_STATE=STATE_OutToLunch;
							c.CONTACT_statusShow=show;
							isChangeState=true;
							
							//#ifndef has_no_sound
							if (this.getIsOpenOnlineSound())
							{
								if(oldState== STATE_OFFLINE&&c.SUBITEM_STATE!=STATE_OFFLINE)
								{
									//eventNotify(EventListener.EVENT_SOUND_NEW_ONLINE,null);	
									hasContactOnline=true;
								}
							}		
							//#endif
						}				
					}
				} 
				else if (type.equals("unavailable"))//此人下线了 头像清掉 所以也要更新rms
				{
					isChangeState=true;
					c.SUBITEM_STATE=STATE_OFFLINE;
					if (nodeVec.contains("show"))
					{
						c.CONTACT_statusShow=show;
					}
					
				}
				if(nodeVec.contains("nickname"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_nickname,nickname);
					if(isTempChange)
					{
						c.CONTACT_nickname=nickname;
						isUpdateRms=true;
					}
					if(c.CONTACT_nickname==null||c.CONTACT_nickname.length()==0)
						c.CONTACT_nickname=c.CONTACT_imid;
				}
				if (nodeVec.contains("gbcode"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_gbcode,gbcode);
					if(isTempChange)
					{
						c.CONTACT_gbcode=gbcode;
						isUpdateRms=true;
					}
				}
				if (nodeVec.contains("impresa"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_impresa,impresa);
					if(isTempChange)
					{
						c.CONTACT_impresa=impresa;
						//isUpdateRms=true;
					}
				}
				//#ifdef support_head_image
				if (nodeVec.contains("portraithash"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_portraithash,portraithash);
					if(isTempChange)
					{
					    if(portraithash != null)
						c.CONTACT_portraithash=portraithash;
						if(c.CONTACT_hasRMSImageData==false&&c.CONTACT_hasSendGetImage==false)
						{
							c.CONTACT_imageData=null;
						}
						else
						{
							c.CONTACT_imageData=null;
							c.CONTACT_hasSendGetImage=false;
							isUpdateHeadRms=true;
						}
						isUpdateRms=true;
					}
					else
					{
					    c.CONTACT_hasSendGetImage=true;
					}
				}
				//#endif
				if(nodeVec.contains("phone"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_phone,phone);
					if(isTempChange)
					{
						c.CONTACT_phone=phone;
						isUpdateRms=true;
					}
				}
				if(nodeVec.contains("gleam"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_gleam,gleam);
					if(isTempChange)
					{
						c.CONTACT_gleam=gleam;
						isUpdateRms=true;
					}
				}
				if (nodeVec.contains("spaces"))
				{
					boolean isTempChange=compareTwoValueIsChange(c.CONTACT_url,url);
					if(isTempChange)
					{
						c.CONTACT_url=url;
						isUpdateRms=true;
					}
				}
				if(isChangeState||isUpdateRms)
				{
					deleteSubItem(from,oldState,app.groupData);
					///////////////////
					//#ifdef support_exchange_contact_group
					if(getGroupOrder()>ROSTER_NORMAL_HAS_OFFLINE)
					{
						deleteSubItem(from,oldState,app.getCurrentGroupData());
					}
					//#endif
					//////////////////////
					if (version != null&&!version.equals(myContactGroupVersion))
					{
						myContactGroupVersion=updateNewVersion(myContactGroupVersion,version);
						//#ifdef support_save_rms
						setContactGroupVerToRMS(myContactGroupVersion);
						//#endif
					}
					addContact(c,ROSTER_NORMAL_HAS_OFFLINE,app.groupData);
					
					///////////////////
					//#ifdef support_exchange_contact_group
					if(getGroupOrder()>ROSTER_NORMAL_HAS_OFFLINE)
					{
						addContact(c,getGroupOrder(),app.getCurrentGroupData());
					}
					//#endif
					//////////////////////
					
					 //#ifdef support_save_rms
					if(isUpdateRms)
					{
						if(saveDataTimer != null)
						{
							saveDataTimer.cancel();
							saveDataTimer = null;
						}
						saveDataTimer = new Timer();
						saveDataTimer.schedule(new Animation(this,"saveData"), 15000);
						//hasContactUpdate=true;
					}
					if(isUpdateHeadRms)
						addContactHeadToRMS(c);
					//#endif

				}
			} 
		} 
		catch (Exception e)
		{
			e.printStackTrace();
		}

	}	
	
    //重写
	public void addContact(Contact contact,byte subType,Vector<Group> appgroupData)
	{
		try
		{
			String groupID = contact.CONTACT_groupId;			
			String[] gid = MsnUtil.split(groupID, ',');
			if (gid == null || gid.length == 0)//默认在缺省组
			{
				contact.CONTACT_groupId = DEFAULT_GROUP_ID;	
			}
			if(subType==ROSTER_NORMAL_HAS_OFFLINE)
			{

				addSubItem(contact,false,app.groupData);
			}
			//#ifdef support_exchange_contact_group
			else
			{
				////////////////////  添加按状态排序联系人
				if(subType == ROSTER_NORMAL_NO_OFFLINE)
				{
					if (contact.SUBITEM_STATE!=STATE_OFFLINE)
					{
						addSubItem(contact,false,appgroupData);
				}
			}
				else if(subType == ROSTER_STATE_HAS_OFFLINE)
				{
					addSubItem(contact, true, appgroupData);
				}
				else if(subType == ROSTER_STATE_NO_OFFLINE)
				{
					if (contact.SUBITEM_STATE != STATE_OFFLINE)
					{
						addSubItem(contact, true, appgroupData);
					}
				}
			}
			//#endif
			
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}

	}
	
	/**
	 * 如果是普通联系人分组用groupid
	 * 如果是频道则用groupname
	 * 如果是聊天室 不需要考虑其它分组 因为只有一个组
	 * @param isToTempGroup 是否加入到临时分组 在线组 脱机组
	 * @param 分组排序类型 （正常排序 按状态排序 ）按状态排序 则只要考虑gbcode排序情况
	 * */
    public void addSubItem(Contact subItem,boolean useTempGroup,Vector<Group> appgroupData)throws Exception
	{
    	try
    	{
			int itemMaxNum = 0;
			
			// 先找到所要加入列表项的分组
			//Item group = null;
			String subItemGroupId=subItem.CONTACT_groupId;
			if(useTempGroup)//因为要加入到临时分组中 所以用临时的groupid
			{
				if (subItem.SUBITEM_STATE==STATE_OFFLINE)
				{
					subItemGroupId=GROUP_OFFLINE_ID;
				}
				else
				{
					subItemGroupId=GROUP_ONLINE_ID;
				}
			}
			String[] gid = MsnUtil.split(subItemGroupId, ',');
			Group currentGroup;
			String currentGroupID;
			int oldItemnum;
			int newItemnum;
			for(int h=0;h<gid.length;h++)
			{
				subItemGroupId=gid[h];
		    	for(int i = 0; i < appgroupData.size(); i++)
		    	{
		    		currentGroup=null;
//		    		Map<String, Group> CurGroup = appgroupData.get(i);
//		    		currentGroup=(Group)CurGroup.get("GROUP");
		    		currentGroup=appgroupData.elementAt(i);

		    		currentGroupID=currentGroup.GROUP_id;
		    		if(currentGroupID.equals(subItemGroupId))
		    		{
		    		    boolean isSame = false;
		    		    for(int m = 0; m < currentGroup.contactVector.size(); m++)
		    		    {
		    		        Contact tmp = currentGroup.contactVector.get(m);
		    		        if(tmp == null)
		    		        {
		    		            break;
		    		        }
		    		        if(tmp.CONTACT_imid.equals(subItem.CONTACT_imid))
		    		        {
		                        isSame = true;
		                        break;
		    		        }		            
		    		    }
		    		    
		    		    if(!isSame)
		    		    {
			    		itemMaxNum = currentGroup.GROUP_itemnum; // 该组下列表项的数量
		    			addContactToList(currentGroup,subItem,subItemGroupId);
		    			currentGroup.GROUP_itemnum = (short)(itemMaxNum + 1);
						if (subItem.SUBITEM_STATE != STATE_OFFLINE)
						{
							oldItemnum=Integer.parseInt(currentGroup.GROUP_maxweight_itemnum);
							newItemnum=oldItemnum+1;
							currentGroup.GROUP_maxweight_itemnum = Integer.toString(newItemnum);
						}
		    		}
		    	} 
		    	} 
			  
			}
		}
    	catch(Exception e)
    	{
    		e.printStackTrace();
    	}
	}
	
	public void eventNotify(byte eventType, Object obj)
	{
		eventListener.eventAction(eventType, obj);
	}
	
	public void eventAction(byte eventType, Object obj) 
	{
        Message msg = new Message(); 
        msg.what = eventType; 
        msg.obj=obj;
        app.GetMSNHandler().sendMessage(msg);
	}
	
	
	Message m;
    private Handler MSNHandler = new Handler() 
    { 
    	
         
        public void handleMessage(Message msg) 
        { 
        	 //Object object=msg.obj;
             switch (msg.what) 
             { 
             ////////////////////
             case EVENT_MAIN_AD_CLICK:
            	 if(MainHandler!=null)
            	 {
				     m = new Message(); 
				     m.what = EVENT_MAIN_AD_CLICK; 
				     MainHandler.sendMessage(m);    
            	 }      		
            	 break;
             case EVENT_ERGISTER_FAILURE:
            	 break;
             case EVENT_REGISTER_SUCCESS:
            	 break;
             case EVENT_NETWORK_FAILURE:
			     //////////
            	 if(LMHandler!=null)
            	 {
                	 m = new Message(); 
    			     m.what = EVENT_NETWORK_FAILURE; 
    			     LMHandler.sendMessage(m);
            	 }

			     ////////
            	 break;
             case EVENT_DISPATCH_NOTICE:
            	 if(LMHandler!=null)
            	 {
            	 m = new Message(); 
			     m.what = EVENT_DISPATCH_NOTICE; 
			     LMHandler.sendMessage(m);
            	 }
            	 break;
             case EVENT_DISPATCH_URL:
            	 if(LMHandler!=null)
            	 {
            	 m = new Message(); 
			     m.what = EVENT_DISPATCH_URL; 
			     LMHandler.sendMessage(m);
			     
            	 }
            	 break;       
             case EVENT_LOGIN_SUCCESS:
            	 if(LMHandler!=null)
            	 {
			     m = new Message(); 
			     m.what = EVENT_LOGIN_SUCCESS; 
			     LMHandler.sendMessage(m);
            	 }
            	 break;
             case EVENT_ZIPWAP_OK:
            	 if(MainHandler!=null)
            	 {
				     m = new Message(); 
				     m.obj=msg.obj;
				     m.what = EVENT_ZIPWAP_OK; 
				     MainHandler.sendMessage(m);    
            	 }
            	 break;
             case EVENT_ZIPWAP_ERROR:
            	 if(MainHandler!=null)
            	 {
				     m = new Message(); 
				     m.what = EVENT_ZIPWAP_ERROR; 
				     MainHandler.sendMessage(m);    
            	 }
            	 break;
             case EVENT_AD_UPDATA:
            	 if(MainHandler!=null)
            	 {
				     m = new Message(); 
				     m.what = EVENT_AD_UPDATA; 
				     MainHandler.sendMessage(m);    
            	 }
            	 break;
             case EVENT_SEND_ADVIST:
            	 break;
//             case EVENT_RECEIVE_LOGOUT:
//            	app.jabber.sendEnd();
//		    	 if(chatroomHandler!=null)
//				 {
//				    m = new Message(); 
//				    m.what = EVENT_FINISH_ACTIVITY; 
//				    chatroomHandler.sendMessage(m);
//				 }
//            	 if(MainHandler!=null)
//            	 {
//				     m = new Message(); 
//				     m.what = EVENT_FINISH_ACTIVITY; 
//				     MainHandler.sendMessage(m);    
//            	 }
//            	 break;
             case EVENT_RECEIVE_EXIT:

		    	 if(chatroomHandler!=null)
				 {
				    Message mm = new Message(); 
				    mm.what = EVENT_FINISH_ACTIVITY; 
				    chatroomHandler.sendMessage(mm);
				 }
            	 if(MainHandler!=null)
            	 {
				     m = new Message(); 
				     m.what = EVENT_FINISH_ACTIVITY; 
				     MainHandler.sendMessage(m);    
            	 }
            	 if(LMHandler!=null)
            	 {
                	 m = new Message(); 
    			     m.what = EVENT_FINISH_ACTIVITY; 
    			     m.obj=msg.obj;
    			     LMHandler.sendMessage(m);
            	 }
            	 if(loginHandler!=null)
            	 {
                	 m = new Message(); 
    			     m.what = EVENT_FINISH_ACTIVITY; 
    			     m.obj=msg.obj;
    			     loginHandler.sendMessage(m);
            	 }
            	 break;
             case EVENT_RECEIVE_LOGIN_ERROR:
			     //////////

            	 if(LMHandler!=null)
            	 {
	            	 m = new Message(); 
				     m.what = EVENT_RECEIVE_LOGIN_ERROR_LOGINMONITOR;
				     m.obj=msg.obj;
				     LMHandler.sendMessage(m);
            	 }
            	 else//mainactivity存在
            	 {
    		    	 if(chatroomHandler!=null)
    				 {
    				    Message mm = new Message(); 
    				    mm.what = EVENT_FINISH_ACTIVITY; 
    				    chatroomHandler.sendMessage(mm);
    				 }
                	 if(MainHandler!=null)
                	 {
    				     m = new Message(); 
    				     m.what = EVENT_RECEIVE_LOGIN_ERROR_MAIN; 
    				     m.obj=msg.obj;
    				     MainHandler.sendMessage(m);    
                	 }
            	 }
			     ////////
            	 break;
             case EVENT_RECEIVE_OTHER_ERROR:
            	 if(!isLoginSuccess)//还出于登录等待状态 给完提示后 需要退回到登录页面
            	 {
            		 if(LMHandler!=null)
                	 {
	                	 m = new Message(); 
	    			     m.what = EVENT_RECEIVE_OTHER_ERROR; 
	    			     m.obj=msg.obj;
	    			     LMHandler.sendMessage(m);
                	 }
            	 }
            	 else//再别的页面 仅仅给个提示就可以了
            	 {
                	 if(MainHandler!=null)
                	 {
	                	 m = new Message(); 
	    			     m.what = EVENT_RECEIVE_OTHER_ERROR; 
	    			     m.obj=msg.obj;
	    			     MainHandler.sendMessage(m);
                	 }
            	 }
            	 break;
             case EVENT_UPDATE_CONTACT_UI:
            	 if(MainHandler!=null)
            	 {
            		 //Log.i("EVENT_UPDATE_CONTACT_UI", "EVENT_UPDATE_CONTACT_UI");
				     m = new Message(); 
				     m.what = EVENT_UPDATE_CONTACT_UI; 
				     MainHandler.sendMessage(m);    
            	 }
            	 
            	 if(searchRosterHander!=null)
            	 {
            		 //Log.i("EVENT_UPDATE_CONTACT_UI", "EVENT_UPDATE_CONTACT_UI");
				     m = new Message();
				     m.what = EVENT_UPDATE_CONTACT_UI; 
				     searchRosterHander.sendMessage(m);    
            	 }
            	 break;
             case EVENT_RECEIVE_MSN_LOGOUT:
            	 //显示到Login界面 ?
            	 
                 if(hasMessages(EventListener.EVENT_SEND_HAERT))
                removeMessages(EventListener.EVENT_SEND_HAERT); 
			    if(chatroomHandler!=null)
			    {
			        //Log.v("EVENT_RECEIVE_MSN_LOGOUT", "chatroom");
					 Message mm = new Message(); 
					 mm.what = EVENT_FINISH_ACTIVITY; 
					 chatroomHandler.sendMessage(mm);
				}
	            if(MainHandler!=null)
	            {
	                //Log.v("EVENT_RECEIVE_MSN_LOGOUT", "mainhandler");
					  m = new Message(); 
					  m.what = EVENT_CLOSE_TO_LOGIN; 
					  MainHandler.sendMessage(m);    
	            }
	            if(LMHandler!=null)
	            {
	                //Log.v("EVENT_RECEIVE_MSN_LOGOUT", "lmhandler");
	            	m = new Message(); 
	    			m.what = EVENT_FINISH_ACTIVITY; 
	    			//m.obj=msg.obj;
	    			 LMHandler.sendMessage(m);
	            }
			    
	            
            	 break;
             case EVENT_RECEIVE_CHATROOM_ERROR:
            	 break;
             case EVENT_CHANGE_MY_STATE:
 	            if(MainHandler!=null)
	            {
					  m = new Message(); 
					  m.what = EVENT_CHANGE_MY_STATE; 
					  MainHandler.sendMessage(m);    
	            }
            	 break;
             case EVENT_ENTER_ROOM_OK:
            	 break;
             case EVENT_SET_NEWMSNCN:
            	 break;
             case EVENT_RECEIVE_VERIFY_FRIEND:	    
            	 if(MainHandler!=null)
            	 {
		    	 m = new Message(); 
		    	 m.what = EVENT_RECEIVE_VERIFY_FRIEND; 
		    	 MainHandler.sendMessage(m);	
            	 }
		    	 if(chatroomHandler!=null)
				    {
				    	Message mm = new Message(); 
				    	 mm.what = EVENT_RECEIVE_VERIFY_FRIEND; 
				    	 mm.obj=msg.obj;
				    	 chatroomHandler.sendMessage(mm);
				    }
            	 break;
             case EVENT_OVER_VERIFY_FRIEND:
            	 if(MainHandler!=null)
            	 {
			    	 m = new Message(); 
			    	 m.what = EVENT_OVER_VERIFY_FRIEND; 
			    	 MainHandler.sendMessage(m);
            	 }
		    	 if(chatroomHandler!=null)
				 {
				    	Message mm = new Message(); 
				    	 mm.what = EVENT_OVER_VERIFY_FRIEND; 
				    	 mm.obj=msg.obj;
				    	 chatroomHandler.sendMessage(mm);
				 }
            	 break;
             case EVENT_MORE_VERIFY_FRIEND:
            	 if(MainHandler!=null)
            	 {
	            	 m = new Message(); 
			    	 m.what = EVENT_RECEIVE_VERIFY_FRIEND; 
			    	 MainHandler.sendMessage(m);
            	 }
            	 if(chatroomHandler!=null)
 			    {
 			    	Message mm = new Message(); 
 			    	 mm.what = EVENT_RECEIVE_VERIFY_FRIEND; 
 			    	 mm.obj=msg.obj;
 			    	 chatroomHandler.sendMessage(mm);
 			    }
            	 break;
             case EVENT_UPDATE_JOIN_CLUSTER:
            	 break;
             case EVENT_KICKOFF_CHATROOM:
            	 break;
             case EVENT_BLACKLIST_CHATROOM:
            	 break;
             case EVENT_SAVE_LOGIN_USER:
            	 break;
             case EVENT_DISPATCH_REFUSE_LOGIN:
			     //////////
            	 if(LMHandler!=null)
            	 {
	            	 m = new Message(); 
				     m.what = EVENT_DISPATCH_REFUSE_LOGIN; 
				     LMHandler.sendMessage(m);
            	 }
			     ////////
            	 break;
             case EVENT_DISPATCH_MUST_UPGRADE:
			     //////////
            	 if(LMHandler!=null)
            	 {
	            	 m = new Message(); 
				     m.what = EVENT_DISPATCH_MUST_UPGRADE; 
				     LMHandler.sendMessage(m);
            	 }
			     ////////
            	 break;
             case EVENT_MY_VCARD_RECEIVED:
  	            if(MainHandler!=null)
	            {
					  m = new Message(); 
					  m.what = EVENT_MY_VCARD_RECEIVED; 
					  MainHandler.sendMessage(m);    
	            }
             	break;
             case EVENT_UPDATE_SESSION_FLAG:
 				if(chatroomHandler!=null)
				{
				   Message mm = new Message(); 
				   mm.what = EVENT_UPDATE_SESSION_FLAG; 
				   mm.obj=msg.obj;
				   chatroomHandler.sendMessage(mm);
				}
              	break;
             case EVENT_UPDATE_SESSION_TOP_NAME:
	             if(chatroomHandler!=null)
	             {
					   Message mm = new Message(); 
					   mm.what = EVENT_UPDATE_SESSION_TOP_NAME; 
					   chatroomHandler.sendMessage(mm);
	             }
	             break;
             case EVENT_ADD_NEW_SESSION:	
            	 if(MainHandler!=null)
            	 {
	            	m = new Message(); 
				    m.what = EVENT_ADD_NEW_SESSION; 
				    MainHandler.sendMessage(m);    
            	 }
              	break;
             case EVENT_UPDATE_MESSAGE:
             {
            	if(chatroomHandler!=null)
 			    {
 			    	Message mm = new Message(); 
 			    	 mm.what = EVENT_UPDATE_MESSAGE; 
 			    	 mm.obj=msg.obj;
 			    	 chatroomHandler.sendMessage(mm);
 			    }
            	 break;
             }
             case EVENT_UPDATE_OUTER_HEAD_STATUS:
             {
            	 if(MainHandler!=null)
            	 {
  			    	Message mm = new Message(); 
			    	 mm.what = EVENT_UPDATE_OUTER_HEAD_STATUS; 
			    	 mm.obj=msg.obj;
			    	 MainHandler.sendMessage(mm);
            	 }
            	 break;
             }
             case EVENT_UPDATE_GAUGE_MESSAGE:
             {
            	if(chatroomHandler!=null)
  			    {
  			    	Message mm = new Message(); 
  			    	 mm.what = EVENT_UPDATE_GAUGE_MESSAGE; 
  			    	 mm.obj=msg.obj;
  			    	 chatroomHandler.sendMessage(mm);
  			    }
            	 break;
             }
             case EVENT_UPDATE_OUTER_HEAD_GAUGE:
             {
            	 if(MainHandler!=null)
            	 {
   			    	Message mm = new Message(); 
 			    	 mm.what = EVENT_UPDATE_OUTER_HEAD_GAUGE; 
 			    	 mm.obj=msg.obj;
 			    	MainHandler.sendMessage(mm);
            	 }
            	 break;
             }
             case EVENT_RECEIVED_MESSAGE:
            	Item message = (Item)msg.obj;
	           	 if(MainHandler!=null)
	        	 {
	            	m = new Message(); 
				    m.what = EVENT_RECEIVED_MESSAGE; 
				    m.obj = msg.obj;
				    MainHandler.sendMessage(m);    
					if (getIsOpenNewMessageSound())
					{
							if(message.MESSAGE_nudge)//播放震动消息音
							{
								eventNotify(EventListener.EVENT_SOUND_NEW_NUDGE,null);
							}
							else
							{
								eventNotify(EventListener.EVENT_SOUND_NEW_MESSENGE,null);
							}
							
					}	
					else
					{
                        if(message.MESSAGE_nudge)//播放震动消息音
                        {
                            eventNotify(EventListener.EVENT_SOUND_NEW_NUDGE,null);
                        }
					}
					
	        	 }
	           	 
			    if(chatroomHandler!=null)
			    {
			    	Message mm = new Message(); 
			    	 mm.what = EVENT_RECEIVED_MESSAGE; 
			    	 mm.obj=msg.obj;
			    	 chatroomHandler.sendMessage(mm);
			    }
//			    else
//			    {
//	            	//如果当前页面不是正在会话页面  则需要根据情况给声音提示
//					if (getIsOpenNewMessageSound())
//					{
//							if(message.MESSAGE_nudge)//播放震动消息音
//							{
//								eventNotify(EventListener.EVENT_SOUND_NEW_NUDGE,null);
//							}
//							else//播放信息音
//							{
//								eventNotify(EventListener.EVENT_SOUND_NEW_MESSENGE,null);
//							}
//							
//					}	   
//			    } 
              	break;
             case EVENT_SEND_MESSAGE:
            	 
            	 if(chatroomHandler!=null)
 			    {
            	     Item item = (Item)msg.obj;
            	     if(item.MESSAGE_type == MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)
            	     {
            	         return;
            	     }
            		 m= new Message(); 
 			    	 m.what = EVENT_SEND_MESSAGE; 
 			    	 m.obj=msg.obj;
 			    	 chatroomHandler.sendMessage(m);
 			    }
            	 
              	break;
             case EVENT_OPEN_CHAT_WINDOW:
              	break;
             case EVENT_RECEIVE_CONTACT_PORTRAIT:
            	 if(MainHandler!=null)
            	 {
					  m = new Message(); 
					  m.what = EVENT_RECEIVE_CONTACT_PORTRAIT; 
					  m.obj=msg.obj;
					  MainHandler.sendMessage(m); 
            	 }   
              	break;
             case EVENT_ADD_CONTACT_OK:
             	if(saveDataTimer != null)
				{
					saveDataTimer.cancel();
					saveDataTimer = null;
				}			
				saveDataTimer = new Timer();        	
        		saveDataTimer.schedule(new Animation(MSNApplication.this,"saveData"), 15000);
            	 //app.addContactToRMS((Contact)object);
            	 if(chatroomHandler!=null)
            	 {
            		 Message mm = new Message(); 
				     mm.what = EVENT_ADD_CONTACT_OK;
				     MainHandler.sendMessage(mm);   
            	 }
              	break;
             case EVENT_DELETE_CONTACT_OK:
             	if(saveDataTimer != null)
				{
					saveDataTimer.cancel();
					saveDataTimer = null;
				}			
				saveDataTimer = new Timer();        	
        		saveDataTimer.schedule(new Animation(MSNApplication.this,"saveData"), 15000);
				//app.removeContactFromRMS((String)object);
            	 if(chatroomHandler!=null)
            	 {
            		 Message mm = new Message(); 
				     mm.what = EVENT_DELETE_CONTACT_OK; 
				     MainHandler.sendMessage(mm);   
            	 }
              	break;
             case EVENT_DELETE_BLOCK_CONTACT_OK:
             	if(saveDataTimer != null)
				{
					saveDataTimer.cancel();
					saveDataTimer = null;
				}			
				saveDataTimer = new Timer();        	
        		saveDataTimer.schedule(new Animation(MSNApplication.this,"saveData"), 15000);
            	 //app.removeContactFromRMS((String)object);
            	 if(chatroomHandler!=null)
            	 {
            		 Message mm = new Message(); 
				     mm.what = EVENT_DELETE_BLOCK_CONTACT_OK; 
				     MainHandler.sendMessage(mm);   
            	 }
              	break;
             case EVENT_BLOCK_CONTACT_OK:
             	if(saveDataTimer != null)
				{
					saveDataTimer.cancel();
					saveDataTimer = null;
				}			
				saveDataTimer = new Timer();        	
        		saveDataTimer.schedule(new Animation(MSNApplication.this,"saveData"), 15000);
            	 //app.addContactToRMS((Contact)object);
				 if(chatroomHandler!=null)
				 {
				    Message mm = new Message(); 
				    mm.what = EVENT_BLOCK_CONTACT_OK; 
				    mm.obj=msg.obj;
				    chatroomHandler.sendMessage(mm);
				 }
              	break;
             case EVENT_UNBLOCK_CONTACT_OK:
             	if(saveDataTimer != null)
				{
					saveDataTimer.cancel();
					saveDataTimer = null;
				}			
				saveDataTimer = new Timer();        	
        		saveDataTimer.schedule(new Animation(MSNApplication.this,"saveData"), 15000);
            	 //app.addContactToRMS((Contact)object);
				 if(chatroomHandler!=null)
				 {
				    Message mm = new Message(); 
				    mm.what = EVENT_UNBLOCK_CONTACT_OK; 
				    mm.obj=msg.obj;
				    chatroomHandler.sendMessage(mm);
				 }
              	break;
             case EVENT_FLASH_COMPLETE:
              	break;
             case EVENT_CHATROOM_LOGOUT_OK:
              	break;
             case EVENT_OTHER_FREIEND_LOGOUT_CHATROOM:
              	break;
             case EVENT_CHATROOM_SIGNOFF_OK:
              	break;
             case EVENT_CHATROOM_MESSAGE_RECEIVED:
              	break;
             case EVENT_RECEIVE_MAIN_FOOTER_AD_ITEM:
               	break;
             case EVENT_RECEIVE_ROSTER_FOOTER_AD_ITEM:
            	 
            	 if(MainHandler!=null)
            	 {
                	 m = new Message(); 
                	 m.obj=msg.obj;
    			     m.what = EVENT_RECEIVE_ROSTER_FOOTER_AD_ITEM; 
    			     MainHandler.sendMessage(m); 
            	 }
               	break;   

             case EVENT_RECEIVE_SESSION_FOOTER_AD_ITEM:
               	break;
             case EVENT_OTHER_FREIEND_ENTER_CHATROOM:
               	break;
             case EVENT_WAITING_LOGIN_TIMEOUT:
               	break;
             case EVENT_WAITING_OTHER_TIMEOUT:
               	break;
             case EVENT_RECEIVE_SYNCHRONIZE_ERROR:
            	 
            	 if(LMHandler!=null)
            	 {
	            	 m = new Message(); 
				     m.what = EVENT_RECEIVE_SYNCHRONIZE_ERROR; 
				     m.obj=msg.obj;
				     LMHandler.sendMessage(m);
            	 }
            	 else
            	 {
	            	 m = new Message(); 
				     m.what = EVENT_RECEIVE_SYNCHRONIZE_ERROR; 
				     m.obj=msg.obj;
				     MainHandler.sendMessage(m);
            	 }
               	break;
             case EVENT_UPDATE_CHAT_UI:
				if(chatroomHandler!=null)
				{
				   Message mm = new Message(); 
				   mm.what = EVENT_UPDATE_CHAT_UI; 
				   mm.obj=msg.obj;
				   chatroomHandler.sendMessage(mm);
				}
               	break;
             case EVENT_OUT_OF_MEMORY:
               	break;
             case EVENT_SOUND_NEW_MESSENGE:
            	 if(MainHandler!=null)
            	 {
                	 m = new Message(); 
    			     m.what = EVENT_SOUND_NEW_MESSENGE; 
    			     MainHandler.sendMessage(m); 
            	 }
               	break;            	
             case EVENT_SOUND_NEW_NUDGE:
            	 if(MainHandler!=null)
            	 {
	            	 m = new Message(); 
				     m.what = EVENT_SOUND_NEW_NUDGE; 
				     MainHandler.sendMessage(m); 
            	 }
            	 break;
             case EVENT_SOUND_NEW_ONLINE:
            	 if(MainHandler!=null)
            	 {
	            	 m = new Message(); 
				     m.what = EVENT_SOUND_NEW_ONLINE; 
				     MainHandler.sendMessage(m); 
            	 }
			     break;
             case EVENT_SEND_HAERT:
            	 jabber.sendSpace();
            	 break;
//             case EVENT_SAVE_CONTACT_LIST:
//            	 
//           	 if(hasContactUpdate)
//            	 {
//            		 hasContactUpdate=false;
//            		 saveAllContactList();
//            	 }
//            	 break;
             case EVENT_RECEIVE_DISPATCH_OK:
			     //////////
            	 if(LMHandler!=null)
            	 {
	            	 m = new Message(); 
				     m.what = EVENT_RECEIVE_DISPATCH_OK; 
				     m.obj=msg.obj;
				     LMHandler.sendMessage(m);
            	 }
			     ////////
            	 break;
             case EVENT_RECEIVE_LOGIN_PICA_OK:
			     //////////
            	 if(LMHandler!=null)
            	 {
	            	 m = new Message(); 
				     m.what = EVENT_RECEIVE_LOGIN_PICA_OK; 
				     m.obj=msg.obj;
				     LMHandler.sendMessage(m);
            	 }
			     ////////
            	 break;
            	 
             case EVENT_UPDATE_INPRESEA_UI:
            	 if(MainHandler!=null)
            	 {
                	 m = new Message(); 
                	 m.obj=msg.obj;
    			     m.what = EVENT_UPDATE_INPRESEA_UI; 
    			     MainHandler.sendMessage(m); 
            	 }
			     break;
			     
             case EVENT_UPDATE_MONITOR_UI:
                 if(LMHandler!=null)
                 {
	            	 m = new Message(); 
				     m.what = EVENT_UPDATE_MONITOR_UI; 
				     m.obj=msg.obj;
				     LMHandler.sendMessage(m);
                 }
			     break;
			     
             case EVENT_SAVE_CONTACT_DATA:
                 try {
            	 removeContactGroupVersionFromRMS();
            	 saveAllContactListToFile(app.GetLiveID());
            	 setContactGroupVerToRMS(myContactGroupVersion);
                 }
                 catch (Exception e)
                 {
                     e.printStackTrace();
                 }
			     break;
			     
             case EVENT_RECEIVE_CONTACT_CHANGE_STATE:
            	 String type = (String) ((Object[]) msg.obj)[0];
            	 String from = (String) ((Object[]) msg.obj)[1];
         		 String version = (String) ((Object[]) msg.obj)[2];
         		 String show = (String) ((Object[]) msg.obj)[3];
         		 String nickname = (String) ((Object[]) msg.obj)[4];
         		 String gbcode = (String) ((Object[]) msg.obj)[5];
         		 String impresa = (String) ((Object[]) msg.obj)[6];
         		 String portraithash = (String) ((Object[]) msg.obj)[7];
         		 String gleam = (String) ((Object[]) msg.obj)[8];
         		 String url = (String) ((Object[]) msg.obj)[9];
         		 String phone = (String) ((Object[]) msg.obj)[10];
         		 Vector nodeVec = (Vector) ((Object[]) msg.obj)[11];
         		
         		Contact contact = getContact(from);
        		if (contact != null)
        		{	
        			//某时候，服务器可能返回的在线人 没有type=available 所以这里认为凡是type为空 则认为它为在线
        			if(type==null)
        				type="available";
        			
        			updateContactPresence(contact,type, from, version, show, nickname,gbcode, impresa, portraithash,gleam,url,phone,nodeVec);

        			eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI,null);
        			//eventNotify(EventListener.EVENT_UPDATE_PORTRAIT_HASH,contact);
        			eventNotify(EventListener.EVENT_UPDATE_CHAT_UI,contact);
        		} 
            	       	
			    break;
			    
             case EVENT_RECEIVE_ROSTER:
            	String action = (String) ((Object[]) msg.obj)[0];
            	//String last = (String) ((Object[]) msg.obj)[1];
         		String jid = (String) ((Object[]) msg.obj)[2];
         		String imid = (String) ((Object[]) msg.obj)[3];
         		String nickname1 = (String) ((Object[]) msg.obj)[4];
         		String gbcode1 = (String) ((Object[]) msg.obj)[5];
         		String block = (String) ((Object[]) msg.obj)[6];
         		String sync = (String) ((Object[]) msg.obj)[7];
         		String groupId = (String) ((Object[]) msg.obj)[8];
         		String impresa1 = (String) ((Object[]) msg.obj)[9];
         		String portraithash1 = (String) ((Object[]) msg.obj)[10];
         		String phone1 = (String) ((Object[]) msg.obj)[11];
         		String gleam1 = (String) ((Object[]) msg.obj)[12];
         		String url1 = (String) ((Object[]) msg.obj)[13];
         		//String version1 = (String) ((Object[]) msg.obj)[14];
         		Vector nodeVec1 = (Vector) ((Object[]) msg.obj)[15];
         		String deviceId = (String) ((Object[]) msg.obj)[16];
         		String addition = (String) ((Object[]) msg.obj)[17];
            	 
         		try
        		{
        			 //#ifdef support_save_rms
        			if (deviceId != null)
        			{
        				if ((devicedId == null)
        						|| (!devicedId.equals(deviceId)))
        				{
        					setDeviceIdToRMS(GetLiveID(), deviceId);
        					devicedId=deviceId;
        				}
        			}
        			//#endif
        			
        			byte contactFlag=CONTACT_NORMAL_FLAG;
        			boolean isChangeBlockState=false;
        			if(addition!=null&&addition.equals("0"))
        			{
        				if(sync==null||sync.length()==0)
        					sync="updated";
        				if (block != null && block.equals("1"))
        					contactFlag = CONTACT_BLOCK_FLAG;
        				isChangeBlockState=true;
        			}
        			else
        			{
        				if(nodeVec1!=null&&nodeVec1.contains("block"))
        				{
        					if (block != null && block.equals("1"))
        						contactFlag = CONTACT_BLOCK_FLAG;
        					isChangeBlockState=true;
        				}
        				else
        				{
        					isChangeBlockState=false;
        				}
        			}

        			if (sync != null && sync.equals("new"))// 新联系人
        			{
        				Contact contact1 =getContact(jid);
        				if (contact1 == null)
        				{
        					if(imid==null)
        						return;//如果imid为空 则把这个包扔掉（防服务器出问题）
        									
        					//addContact(jid, imid, nickname, gbcode, groupId,
        					//		contactFlag, portraithash, impresa, null,phone, gleam, url,
        					//		null, false, true);
        					
        					Contact newContact=newContact(MSNApplication.STATE_OFFLINE,jid, imid, nickname1, gbcode1, groupId,
        							contactFlag, portraithash1, impresa1, null,phone1, gleam1, url1,null);
        					//addContact(newContact);
        					
        					addContact(newContact,ROSTER_NORMAL_HAS_OFFLINE,app.groupData);
        					///////////////////
        					//#ifdef support_exchange_contact_group
        					if(getGroupOrder()>ROSTER_NORMAL_HAS_OFFLINE)
        					{
        						addContact(newContact,getGroupOrder(),app.getCurrentGroupData());
        					}
        					//#endif
        					
        					
        					//#ifdef support_save_rms
        					//if(isSaveRMS)
        					{
        						//addContactToRMS(newContact);	
        						addContactHeadToRMS(newContact);
        					}
        					//#endif
        					
        					if (action != null && action.equals("add"))// 表示添加联系人成功
        					{
        						eventNotify(EventListener.EVENT_ADD_CONTACT_OK, newContact);
        						return;
        						
        					}
        					//eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
        				}
        				else//相当于更新update
        				{
        					if(!isChangeBlockState)
        						contactFlag=contact1.CONTACT_contactFlag;
        					updateContact(contact1,action,jid,imid,contactFlag,
        							nodeVec1,block,nickname1,gbcode1,groupId,
        							impresa1,phone1,portraithash1,gleam1,url1);
        					//eventNotify(EventListener.EVENT_UPDATE_PORTRAIT_HASH,contact1);
        				}
        			} 
        			else if (sync != null && sync.equals("updated"))// 修改的联系人
        			{
        				Contact contact1 =getContact(jid);
        				if (contact1==null)
        				{			
        					if(imid==null)
        						return;//如果imid为空 则把这个包扔掉（防服务器出问题）
        					
        					// 特殊处理 相当于new
//        					addContact(jid, imid, nickname, gbcode, groupId,
//        							contactFlag, portraithash, impresa, null,phone, gleam,
//        							url,  null,false, true);
        					
        					Contact newContact=newContact(MSNApplication.STATE_OFFLINE,jid, imid, nickname1, gbcode1, groupId,
        							contactFlag, portraithash1, impresa1, null,phone1, gleam1, url1,null);
        					//addContact(newContact);
        					addContact(newContact,ROSTER_NORMAL_HAS_OFFLINE,app.groupData);
        					///////////////////
        					//#ifdef support_exchange_contact_group
        					if(getGroupOrder()>ROSTER_NORMAL_HAS_OFFLINE)
        					{
        						addContact(newContact,getGroupOrder(),app.getCurrentGroupData());
        					}
        					//#endif
        					//#ifdef support_save_rms
        					//if(isSaveRMS)
        					{
        						//addContactToRMS(newContact);	
        						addContactHeadToRMS(newContact);
        					}
        					//#endif
        					
        					
        					// 新增加了一个联系人 联系人页面需要刷新
        					//eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
        				} 
        				else
        				// 已有联系人 更新
        				{
        					if(!isChangeBlockState)
        						contactFlag=contact1.CONTACT_contactFlag;
        					updateContact(contact1,action,jid,imid,contactFlag,
        							nodeVec1,block,nickname1,gbcode1,groupId,
        							impresa1,phone1,portraithash1,gleam1,url1);
        					//eventNotify(EventListener.EVENT_UPDATE_PORTRAIT_HASH,contact1);
        				}
        			} 
        			else if (sync != null && sync.equals("deleted"))// 删除的联系人
        			{
        				Contact c = (Contact)getContact(jid);
        				byte oldState = STATE_OFFLINE;
        				String deleteImid=imid;
        				if (c != null)
        				{
        					oldState = c.SUBITEM_STATE;
        					deleteImid=c.CONTACT_imid;
        				}
        				//#ifdef support_save_rms
        				//removeContactFromRMS(jid);
        				deleteContactHeadFromFile(deleteImid);
        				//#endif
        				deleteSubItem(jid, oldState,app.groupData);
        				
        				///////////////////
        				//#ifdef support_exchange_contact_group
        				if(getGroupOrder()>ROSTER_NORMAL_HAS_OFFLINE)
        				{
        					deleteSubItem(jid,oldState,app.getCurrentGroupData());
        				}
        				//#endif
        				//////////////////////
        				
        				if (action != null)
        				{
        					if (action.equals("delete"))// 表示删除联系人成功
        						eventNotify(EventListener.EVENT_DELETE_CONTACT_OK, jid);
        					else if (action.equals("rmblk"))// 删除并阻止
        						eventNotify(EventListener.EVENT_DELETE_BLOCK_CONTACT_OK,jid);

        					//eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
        				} 
        				//else
        				//{
        					// 新删减了一个联系人 联系人页面需要刷新
        					//eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
        				//}

        			}
        		} 
        		catch (Exception e)
        		{
        			e.printStackTrace();
        		}
	         	            	
			    break;
             case EVENT_RECEIVE_GROUP:            	
            	//String last1 = (String) ((Object[]) msg.obj)[0];
            	String groupName = (String) ((Object[]) msg.obj)[1];
            	String groupId1 = (String) ((Object[]) msg.obj)[2];
            	String gbcode11 = (String) ((Object[]) msg.obj)[3];
            	String sync1 = (String) ((Object[]) msg.obj)[4];
            	//String version11 = (String) ((Object[]) msg.obj)[5];            	 
            	 
            	byte groupflag=GROUP_USER_FLAG;
         		boolean isAboutDefault=true;//是否关于缺省组的操作
         		boolean isChangeDefault=false;//是否修改一个缺省组
         		boolean isNewDefault=false;//是否需要新生成一个缺省组
         		boolean hasAddDefault=false;//在删除操作时候 是否添加了缺省分组
         		Group oldGroup=null;
         		try {
                 if(groupId1 == null||groupId1.equals(DEFAULT_GROUP_ID))
                 {	
                 	groupId1 = DEFAULT_GROUP_ID;
                 	oldGroup=getGroup(DEFAULT_GROUP_ID,app.groupData);
                 	if(oldGroup==null)
                 	{
                 		DEFAULT_GROUP_GBC=gbcode11;//给变量赋值
                 		DEFAULT_GROUP_NAME=groupName;//给变量赋值
                 		if(DEFAULT_GROUP_NAME==null||DEFAULT_GROUP_NAME.length()==0)
                 		{
                 			DEFAULT_GROUP_NAME=(String)mActivity.getText(R.string.about);
                 		}
                 		isNewDefault=true;//需要新生成一个缺省组
                 	}
                 	else//缺省组 不考虑删除并添加了 可以减少排序等操作
                 	{
                     	if(!oldGroup.GROUP_gbcode.equals(gbcode11))
                     	{
                     		DEFAULT_GROUP_GBC=gbcode11;//给变量赋值
                     		oldGroup.GROUP_gbcode=gbcode11;
                     		isChangeDefault=true;//修改一个缺省组
                     	}
                     	if(!oldGroup.GROUP_name.equals(groupName))
                     	{
                     		DEFAULT_GROUP_NAME=groupName;//给变量赋值
                     		if(DEFAULT_GROUP_NAME==null||DEFAULT_GROUP_NAME.length()==0)
                     		{
                     			DEFAULT_GROUP_NAME=(String)mActivity.getText(R.string.about);
                     		}
                     		oldGroup.GROUP_name=groupName;
                     		isChangeDefault=true;//修改一个缺省组
                     	}
                     	if(isChangeDefault)
                     	{	
                     		//#ifdef support_exchange_contact_group
                     		hasAddDefault=removeGroup(groupId1,app.groupData);//删掉老分组
                     		////////////////////////
                     		//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
                     		removeGroup(groupId1,app.groupDataNormalOnlyOnline);
                     		/////////////////////////
                     		//#else
                     		//# hasAddDefault=model.getRoster().removeGroup(groupId);//删掉老分组
                     		//#endif
                     	}
                 	}
                 }
                 else
                 	isAboutDefault=false;
                 
                 if(sync1.equals("new"))//新分组
                 {
                 	if(oldGroup!=null)//如果已经存在
                 	{    		
                 		//#ifdef support_exchange_contact_group
                 		hasAddDefault=removeGroup(groupId1,app.groupData);	//先删除	
                 		////////////////////////
                 		//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
                 		removeGroup(groupId1,app.groupDataNormalOnlyOnline);
                 		/////////////////////////
                 		
                 		
                 		//#else
                 		//# hasAddDefault=model.getRoster().removeGroup(groupId);	//
                 		//#endif
                 	}
                 	if(!hasAddDefault)
                 	{
                     	oldGroup=newGroup(groupId1, gbcode11, groupName, groupflag,false);
                     	addGroupToList(oldGroup,app.groupData);
                     	/////////////
                     	/////////////
                 		//#ifdef support_exchange_contact_group
                     	//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
                     	addGroupToList(newGroup(groupId1, gbcode11, groupName, groupflag,false),app.groupDataNormalOnlyOnline);
                     	//#endif
             			/////////////
                     	
                    	 	//#ifdef support_save_rms
                        // addGroupToRMS(oldGroup);
                         //#endif
                 	}
                 }
                 else if(sync1.equals("updated"))//修改
                 {
                 	//是否要保存到rms
                 	boolean updateRms=false;
                 	
                 	if(isNewDefault)
                 	{
                 		oldGroup=newGroup(groupId1, gbcode11, groupName, groupflag,false);
                 		updateRms=true;
                 		addGroupToList(oldGroup,app.groupData);
                 		//////////////////
                 		//#ifdef support_exchange_contact_group
                 		//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
                 		addGroupToList(newGroup(groupId1, gbcode11, groupName, groupflag,false),app.groupDataNormalOnlyOnline);
                 		//#endif
             			/////////////////
                 	}
                 	if(isChangeDefault)
                 	{
                 		if(hasAddDefault)//如果添加过了 就不需要再添加
                 			updateRms=false;
                 	}
                 	if(!isAboutDefault)//不是缺省组
                 	{
                 		oldGroup=getGroup(groupId1,app.groupData);
                     	if(oldGroup==null)
                     	{
                     		oldGroup=newGroup(groupId1, gbcode11, groupName, groupflag,false);
                     		updateRms=true;
                     		addGroupToList(oldGroup,app.groupData);
                     		//////////////////
                     		//#ifdef support_exchange_contact_group
                     		//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
                     		addGroupToList(newGroup(groupId1, gbcode11, groupName, groupflag,false),app.groupDataNormalOnlyOnline);
                     		//#endif
                     		/////////////////
                     	}
                     	else
                     	{
                     		
                     		if(!oldGroup.GROUP_gbcode.equals(gbcode11))
                     		{
                     			oldGroup.GROUP_gbcode=gbcode11;
                     			updateRms=true;
                     		}
                     		if(!oldGroup.GROUP_name.equals(groupName))
                     		{
                     			oldGroup.GROUP_name=groupName;
                     			updateRms=true;
                     		}
                     		if(updateRms)
                     		{            		
                     			//#ifdef support_exchange_contact_group
                     			removeGroup(groupId1,app.groupData);//删掉老分组
                         		////////////////////////
                     			//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
                         		removeGroup(groupId1,app.groupDataNormalOnlyOnline);
                         		/////////////////////////			
                     			//#else
                     			//# model.getRoster().removeGroup(groupId);//删掉老分组
                     			//#endif
                     			oldGroup.GROUP_itemnum=0;
                     			oldGroup.GROUP_maxweight_itemnum="0";
                     			addGroupToList(oldGroup,app.groupData);
                         		
                         		//////////////////
                     			//#ifdef support_exchange_contact_group
                     			Group oldGroup1=newGroup(oldGroup.GROUP_id, oldGroup.GROUP_gbcode, oldGroup.GROUP_name, oldGroup.GROUP_flag,false);
                         		//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
                         		addGroupToList(oldGroup1,app.groupDataNormalOnlyOnline);
                         		//#endif
                     			/////////////////
                     		}
                     	}
                 	}
                 	if(updateRms)
                 	{  		
                    	 	//#ifdef support_save_rms
                 		//addGroupToRMS(oldGroup);
                 		//#endif
                 	}
                 }
                 else if(sync1.equals("deleted"))//删除
                 {
                 	//#ifdef support_exchange_contact_group
                 	removeGroup(groupId1,app.groupData);
                 	
             		////////////////////////
                 	//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
                 	removeGroup(groupId1,app.groupDataNormalOnlyOnline);
             		/////////////////////////
                 	
                 	//#else
                 	//# model.getRoster().removeGroup(groupId);
                 	//#endif
                 	
                	 	//#ifdef support_save_rms
                	 	if(saveDataTimer != null)
         			{
         				saveDataTimer.cancel();
         				saveDataTimer = null;
         			}			
         			saveDataTimer = new Timer();
                 	saveDataTimer.schedule(new Animation(app,"saveData"), 15000);
                	 	//removeGroupFromRMS(groupId);
                 	//#endif
                 }
                 //eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
         		}
         	    catch (Exception e)
         	    {
         	        e.printStackTrace();
         	    }
			    break;
			    
			    
             case EVENT_RECEIVE_GROUP_COMPLETE:
            	 //String last1 = (String) ((Object[]) msg.obj)[0];
                 try {
            	 String version11 = (String) ((Object[]) msg.obj)[1];
            	 if(getGroup(DEFAULT_GROUP_ID,app.groupData)==null)
         		{
             		if(DEFAULT_GROUP_NAME==null||DEFAULT_GROUP_NAME.length()==0)
             		{
             			DEFAULT_GROUP_NAME=(String)mActivity.getText(R.string.about);
             		}
             		Group tempGroup=newGroup(DEFAULT_GROUP_ID, DEFAULT_GROUP_GBC,DEFAULT_GROUP_NAME,GROUP_USER_FLAG,false);
         			addGroupToList(tempGroup,app.groupData);
             		//////////////////
                 	//#ifdef support_exchange_contact_group
         			//if(this.getGroupOrder()==ROSTER_NORMAL_NO_OFFLINE)
         			addGroupToList(newGroup(DEFAULT_GROUP_ID, DEFAULT_GROUP_GBC, DEFAULT_GROUP_NAME, GROUP_USER_FLAG,false),app.groupDataNormalOnlyOnline);
             		//#endif
         			/////////////////
         			eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
         		}		

                 if(version11 != null)
                 {
                 	if(myContactGroupVersion==null||(!myContactGroupVersion.equals(version11)))
                 	{
                 		myContactGroupVersion=updateNewVersion(myContactGroupVersion,version11);
            			 	//#ifdef support_save_rms
                 		setContactGroupVerToRMS(myContactGroupVersion);
                 		//#endif
                 	}
                 }
                 //this.saveAllGroupList();
                 }
                 catch (Exception e)
                 {
                     e.printStackTrace();
                 }
			    break;
			 
             case EVENT_RECEIVE_ADDITION_EQUALS_ZERO:
            	//删掉内存里面的联系人分组和联系人
                 try {
            	deleteAllUserGroupAndContact(app.groupData);         		
//          		if(getGroupOrder()>ROSTER_NORMAL_HAS_OFFLINE)
//          		{
//          			deleteAllUserGroupAndContact(app.getCurrentGroupData());
//          		}
             	deleteAllUserGroupAndContact(app.groupDataNormalOnlyOnline);
             	deleteAllUserGroupAndContact(app.groupDataByStatus);
             	deleteAllUserGroupAndContact(app.groupDataStatusOnlyOnline);  
                groupDataByStatus.removeAllElements();
                groupDataStatusOnlyOnline.removeAllElements();
             	addGroupToList(newGroup(MSNApplication.GROUP_ONLINE_ID, null,
             			mActivity.getString(R.string.text_group_online), GROUP_USER_FLAG, false), groupDataByStatus);
             	addGroupToList(newGroup(MSNApplication.GROUP_OFFLINE_ID, null,
             			mActivity.getString(R.string.text_group_offline), GROUP_USER_FLAG, false), groupDataByStatus);
             	addGroupToList(newGroup(MSNApplication.GROUP_ONLINE_ID, null,
             			mActivity.getString(R.string.text_group_online), GROUP_USER_FLAG, false), groupDataStatusOnlyOnline);
             	addGroupToList(newGroup(MSNApplication.GROUP_OFFLINE_ID, null,
             			mActivity.getString(R.string.text_group_offline), GROUP_USER_FLAG, false), groupDataStatusOnlyOnline);
                 }
                 catch (Exception e)
         		{
                     e.printStackTrace();
         		}
			    break;
			    
             case EVENT_ROSTER_GROUP_BY_NORMAL:
            	 boolean onlyShowOnlineContact = Boolean.parseBoolean(((String) msg.obj));
            	 try
         		{
         			if(!onlyShowOnlineContact)//还显示脱机联系人
         			{

         			}
         			else//只显示在线的联系人 无脱机人
         			{
         				app.groupDataNormalOnlyOnline.clear();
         				//app.childDataNormalOnlyOnline.clear();
         				Group currentGroup;
         				//boolean isAddDefault=false;//是否需要添加新的缺省组
         				Vector addVc=new Vector();//vc用来装需要添加的所有联系人
         				for (int i = 0; i < app.groupData.size(); i++)
         				{	
//         					Map<String, Group> CurGroupMap = app.groupData.get(i);
//         		    		currentGroup=(Group)CurGroupMap.get("GROUP");	   
         					currentGroup=app.groupData.elementAt(i);
         		    		app.addGroupToList(app.newGroup(currentGroup.GROUP_id,currentGroup.GROUP_gbcode,currentGroup.GROUP_name,currentGroup.GROUP_flag,false),app.groupDataNormalOnlyOnline);
         					//List<Map<String, Contact>> childData  = app.childData.get(i);
         		    		Group group=app.groupData.elementAt(i);
         		    		Contact currentContact;
         		    		Contact addContact;
         					for(int k=0;k<group.contactVector.size();k++)
         					{
         						//Map<String, Contact> CurChild = new HashMap<String, Contact>(); 
         						//CurChild= childData.get(k);
         						//Contact currentContact=CurChild.get("CONTACT");
         						currentContact = null;
         						currentContact=group.contactVector.elementAt(k);
         						if (currentContact.SUBITEM_STATE!=MSNApplication.STATE_OFFLINE)
         							addVc.addElement(currentContact);	
         					}
         					if(addVc!=null&&addVc.size()!=0)
         					{
         						 for(int m=0;m<addVc.size();m++)
         						 {
         							 addContact = null;
         							 addContact=(Contact)addVc.elementAt(m);
         						    
         						    app.addSubItem(addContact,false,app.groupDataNormalOnlyOnline);
         						 }
         					}
         					addVc.removeAllElements();
         				}
         			}
         		}
         		catch(Exception e)
         		{
         			e.printStackTrace();
            	    }
            	//System.gc();
         		//app.eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
			    break;
			 
			    
             case EVENT_ROSTER_GROUP_BY_STATE:
            	 boolean isShowOnlineContact = Boolean.parseBoolean(((String) msg.obj));
            	 try
         		{
         			if(!isShowOnlineContact)//还显示脱机联系人
         			{
         				app.groupDataByStatus.removeAllElements();
         				addGroupToList(newGroup(MSNApplication.GROUP_ONLINE_ID, null,
         				       (String)mActivity.getText(R.string.text_group_online), GROUP_USER_FLAG, false),groupDataByStatus);
         		    	addGroupToList(newGroup(MSNApplication.GROUP_OFFLINE_ID, null,
         		    	       (String)mActivity.getText(R.string.text_group_offline), GROUP_USER_FLAG, false),groupDataByStatus);
         				Group currentGroup;
         				Vector onlineRoster = new Vector();//vc用来装需要添加的所有联系人
         				Vector offlineRoster = new Vector();
                        Contact currentContact;
                        Contact addContact; 
                        boolean bTrue = true;
         				for (int i = 0; i < app.groupData.size(); i++)
         				{
         					currentGroup=app.groupData.elementAt(i);

         					for(int k=0; k<currentGroup.contactVector.size(); k++)
         					{
         					    bTrue = true;
         						currentContact = null;
         						currentContact = currentGroup.contactVector.elementAt(k);
         						if (currentContact.SUBITEM_STATE == MSNApplication.STATE_OFFLINE)
         						{
         						    if(offlineRoster.size() == 0)
         						    {
         						       offlineRoster.addElement(currentContact);
         						    }
                                    for(int q = 0; q < offlineRoster.size(); q++)
                                    {
                                       Contact tmp = (Contact)offlineRoster.elementAt(q);
                                       if(currentContact.CONTACT_imid.equals(tmp.CONTACT_imid))
                                       {
                                           bTrue = false;
                                           break;
                                       }
                                       else
                                       {
                                           bTrue = true;
                                           continue;
                                       }                                  
                                    }
                                    if(bTrue == true)
                                    {
                                        offlineRoster.addElement(currentContact);
                                    }
         							//offlineRoster.addElement(currentContact);
         						}
         						else
         						{
                                    if(onlineRoster.size() == 0)
                                    {
                                        onlineRoster.addElement(currentContact);
                                    }
                                    for(int q = 0; q < onlineRoster.size(); q++)
                                    {
                                       Contact tmp = (Contact)onlineRoster.elementAt(q);
                                       if(currentContact.CONTACT_imid.equals(tmp.CONTACT_imid))
                                       {
                                           bTrue = false;
                                           break;
                                       }
                                       else
                                       {
                                           bTrue = true;
                                           continue;
                                       }
                                    }
                                    if(bTrue == true)
                                    {
                                        onlineRoster.addElement(currentContact);
                                    }
         							//onlineRoster.addElement(currentContact);
         						}
         							
         					}

         				}
                        if(onlineRoster != null && onlineRoster.size()!=0)
                        {
                             for(int m=0; m<onlineRoster.size(); m++)
                             {
                                 addContact = null;
                                 addContact=(Contact)onlineRoster.elementAt(m);
                                
                                app.addSubItem(addContact, true, app.groupDataByStatus);
                             }
                        }
                        if(offlineRoster != null && offlineRoster.size()!=0)
                        {
                             for(int m=0; m<offlineRoster.size(); m++)
                             {
                                 addContact = null;
                                 addContact=(Contact)offlineRoster.elementAt(m);
                                
                                app.addSubItem(addContact, true, app.groupDataByStatus);
                             }
                        }
                        onlineRoster.removeAllElements();
                        offlineRoster.removeAllElements();
         			}
         			else//只显示在线的联系人 无脱机人
         			{
         				app.groupDataStatusOnlyOnline.removeAllElements();
         				addGroupToList(newGroup(MSNApplication.GROUP_ONLINE_ID, null,
         						mActivity.getString(R.string.text_group_online), GROUP_USER_FLAG, false),groupDataStatusOnlyOnline);
         				addGroupToList(newGroup(MSNApplication.GROUP_OFFLINE_ID, null,
         						mActivity.getString(R.string.text_group_offline), GROUP_USER_FLAG, false), groupDataStatusOnlyOnline);
         				Group currentGroup;
         				Vector addVc=new Vector();//vc用来装需要添加的所有联系人
         				for (int i = 0; i < app.groupData.size(); i++)
         				{
         					currentGroup = app.groupData.elementAt(i);
         		    		Contact currentContact;
         		    		Contact addContact;
         		    		boolean bTrue = true;
         					for(int k=0; k<currentGroup.contactVector.size(); k++)
         					{
         					    bTrue = true;
         						currentContact = null;
         						currentContact=currentGroup.contactVector.elementAt(k);
         						if (currentContact.SUBITEM_STATE != MSNApplication.STATE_OFFLINE)
         						{
                                    if(addVc.size() == 0)
                                    {
                                        addVc.addElement(currentContact);
                                    }
         						    for(int q = 0; q < addVc.size(); q++)
         						    {
         						       Contact tmp = (Contact)addVc.elementAt(q);
         						       if(currentContact.CONTACT_imid.equals(tmp.CONTACT_imid))
         						       {
         						          bTrue = false;
         						          break;
         						       }
         						       else
         						       {
         						           bTrue = true;
                                           continue;
         						       }
         						    }
                                    if(bTrue == true)
                                    {
                                        addVc.addElement(currentContact);
                                    }
         						}

         		}
         					if(addVc!=null&&addVc.size()!=0)
         					{
         						 for(int m=0;m<addVc.size();m++)
         						 {
         							 addContact = null;
         							 addContact=(Contact)addVc.elementAt(m);
         						    
         						    app.addSubItem(addContact, true, app.groupDataStatusOnlyOnline);
         						 }
         					}
         					addVc.removeAllElements();
         				}
         			}
         		}
            	    catch (Exception e)
            	    {
            	        e.printStackTrace();
            	        MsnUtil.DebugOutPutFile("MSNAPP 5968" + e.getMessage());
            	    }
            	    //System.gc();
         		//app.eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
			    break;
			    
			    
             case EVENT_SET_IS_SHOW_ONLINE:
            	if(app.isShowOnline)
			    {
				    if(1 == app.getGroupOrder() || 3 == app.getGroupOrder())
				    {
				    	app.setGroupOrder((byte)(app.getGroupOrder() + 1));
				    }
			    }				
				else
				{
					if(2 == app.getGroupOrder() || 4 == app.getGroupOrder())
				    {
				    	app.setGroupOrder((byte)(app.getGroupOrder() - 1));
				    }
				}
            	switch (getGroupOrder())
				{
					case ROSTER_NORMAL_HAS_OFFLINE:
						app.eventNotify(EventListener.EVENT_ROSTER_GROUP_BY_NORMAL, Boolean.toString(app.isShowOnline));
						break;
					case ROSTER_NORMAL_NO_OFFLINE:
						app.eventNotify(EventListener.EVENT_ROSTER_GROUP_BY_NORMAL, Boolean.toString(app.isShowOnline));
						break;
					case ROSTER_STATE_HAS_OFFLINE:
						app.eventNotify(EventListener.EVENT_ROSTER_GROUP_BY_STATE, Boolean.toString(app.isShowOnline));
						break;
					case ROSTER_STATE_NO_OFFLINE:
						app.eventNotify(EventListener.EVENT_ROSTER_GROUP_BY_STATE, Boolean.toString(app.isShowOnline));
						break;
				}
            	
            	if(MainHandler != null)
            	{
        			Message m = new Message();
        			m.what = EventListener.EVENT_UPDATE_CONTACT_UI;
        			MainHandler.sendMessage(m);		
            	}
 			    break;
             case EVENT_MY_ITEM_LOGO:
                 if(MainHandler != null)
                 {
                     m = new Message(); 
                     m.what = EVENT_MY_ITEM_LOGO; 
                     MainHandler.sendMessage(m);
                 }
                 break;
             case EVENT_MY_ITEM_INFO:
                 if(MainHandler != null)
                 {
                     m = new Message(); 
                     m.what = EVENT_MY_ITEM_INFO; 
                     MainHandler.sendMessage(m);
                 }
                 break;
             case EVENT_UPDATE_PORTRAIT_HASH:
                 if(MainHandler != null)
                 {
                     m = new Message(); 
                     m.what = EVENT_UPDATE_PORTRAIT_HASH;
                     m.obj = msg.obj;
                     MainHandler.sendMessage(m);
                 }
                 break;
             } 
             
             super.handleMessage(msg); 
             
        } 
    };
    
   
    private static Timer saveDataTimer;
    
	public void TimerNotify(Object tag)
	{
		// TODO Auto-generated method stub
    	if(tag.equals("saveData"))
    	{
    		eventNotify(EventListener.EVENT_SAVE_CONTACT_DATA,null);
//    		setContactGroupVerToRMS("");
//    		saveAllContactListToFile(app.GetLiveID());
//    		setContactGroupVerToRMS(myContactGroupVersion);
    	}		
	}  
    
    
    ///////////////////
    
    public void playAudio(int type) {
        try {
        	if(type == 0)
        	{
        		mMediaPlayer = MediaPlayer.create(mActivity, R.raw.online);
        	}
        	else if(type == 1)
        	{
        		mMediaPlayer = MediaPlayer.create(mActivity, R.raw.type);
        	}
        	else if(type == 2)
        	{
        		mMediaPlayer = MediaPlayer.create(mActivity, R.raw.nudge);
        	}
            mMediaPlayer.start();
        } catch (Exception e) {
            //Log.e("Media", "error: " + e.getMessage(), e);
        }

    }
    
    public void playVibrat()
    {
        Vibrator vibrator = (Vibrator)getSystemService(mActivity.VIBRATOR_SERVICE);
        vibrator.vibrate(1000);
    }
    
    
    
    ////////////// yangmin
	
     /**历史会话数据库前缀名称*/
//	 public static final String RMS_RECORD_SESSION="RECORD_RMS_";
//	 /**联系人、分组的版本号数据库前缀名称*/
//	 public static final String RMS_CONACT_VER="CONTACT_VER_RMS_";
//	 /**分组话数据库前缀名称*/
//	 public static final String RMS_GROUP="GROUP_RMS_";
//	 /**联系人数据库前缀名称*/
//	 public static final String RMS_CONTACT="CONTACT_RMS_";
//	 /**个人资料数据库前缀名称*/
//	 public static final String RMS_VCARD="VCARD_RMS_";
//	 /**设备ID数据库前缀名称*/
//	 public static final String RMS_DEVICE_ID="DEVICE_ID_RMS_";
	
	/**读取记录库中指定用户（自己）的个人资料相关数据<br>
	 * 昵称 <br>
	 * 心情短语 <br>
	 * 个人资料的ID号 <br>
	 * 个人资料的版本<br>
	 * @param primaryKey 主键名称
	 * */
	public void initVcardFromRMS(String primaryKey)
	{
	    try {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		Cursor cur = app.DB.getLoginUserData(primaryKey);
		if(cur == null)
		{
			return;
		}
		//int I = cur.getCount();
		if(cur.getCount() == 0)
		{
			return;
		}
		myVCARD_myNickname = MsnUtil.getUnvalidFormRs(cur.getString(10));
		myVCARD_myImpresa = MsnUtil.getUnvalidFormRs(cur.getString(11));
		myVCARD_resourceId = MsnUtil.getUnvalidFormRs(cur.getString(12));
		//myVcardVersion = app.DB.getUnvalidFormRs(cur.getString(13));
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}
	
	
	/**
	 * 从记录库中获取分组及联系人列表的版本号
	 * @param primaryKey 主键名称
	 */
	public void getContactGroupVerFromRMS(String primaryKey)
	{
	    try {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		Cursor cur = app.DB.getLoginUserData(primaryKey);
		if(cur == null)
		{
			return;
		}
		if(cur.getCount() == 0)
		{
			return;
		}		
		myContactGroupVersion = MsnUtil.getUnvalidFormRs(cur.getString(13));
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}
	
	/**
	 * 从记录库中读取指定用户(自己)的分组数据并初始化分组
	 * @param primaryKey 主键名称
	 */
	public void initGroupsFromRMS(String primaryKey)
	{
	    try {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		Cursor cur = app.DB.getGroupData(primaryKey);
		if(cur == null)
		{
			return;
		}
		if(cur.getCount() == 0)
		{
			return;
		}
		Group group;
		for(int i = 0; i < cur.getCount(); i++)
		{
			group = null;
			group=new Group();
			group.GROUP_id = MsnUtil.getUnvalidFormRs(cur.getString(1));
			group.GROUP_gbcode = MsnUtil.getUnvalidFormRs(cur.getString(2));
			group.GROUP_name = MsnUtil.getUnvalidFormRs(cur.getString(3));
			group.GROUP_flag = Byte.parseByte(cur.getString(4));								
			Group newGroup=newGroup(group.GROUP_id,group.GROUP_gbcode,group.GROUP_name,group.GROUP_flag,false);
			addGroupToList(newGroup,app.groupData);
			addGroupToList(newGroup(group.GROUP_id,group.GROUP_gbcode,group.GROUP_name,group.GROUP_flag,false),app.groupDataNormalOnlyOnline);
			cur.moveToNext();
		}
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}
	
	/**
	 * 从记录库中读取数据并初始化联系人
	 *@param primaryKey 主键名称
	 */
	public void initContactsFromRMS(String primaryKey)
	{
	    try {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		Cursor cur = app.DB.getRosterData(primaryKey);
		if(cur == null)
		{
			return;
		}
		if(cur.getCount() == 0)
		{
			return;
		}
		//int d = cur.getCount();
		Contact contact;
		Contact newContact;
		for(int i = 0; i < cur.getCount(); i++)
		{
			contact = null;
			contact = new Contact();
	//		contact.SUBITEM_STATE ;
			contact.SUBITEM_JID = MsnUtil.getUnvalidFormRs(cur.getString(1));
			contact.CONTACT_imid = MsnUtil.getUnvalidFormRs(cur.getString(2));
			contact.CONTACT_nickname = MsnUtil.getUnvalidFormRs(cur.getString(3));
			if (contact.CONTACT_nickname == null|| contact.CONTACT_nickname.length() == 0)
				contact.CONTACT_nickname = contact.CONTACT_imid;
			contact.CONTACT_gbcode = MsnUtil.getUnvalidFormRs(cur.getString(4));
			contact.CONTACT_groupId = MsnUtil.getUnvalidFormRs(cur.getString(5));
	
	//		contact.CONTACT_contactFlag ;
	//
			//contact.CONTACT_portraithash =null;/* = cur.getString(6)*/
			contact.CONTACT_portraithash  = MsnUtil.getUnvalidFormRs(cur.getString(6));
			contact.CONTACT_impresa = MsnUtil.getUnvalidFormRs(cur.getString(7));
			contact.CONTACT_gleam = MsnUtil.getUnvalidFormRs(cur.getString(8));
			contact.CONTACT_url = MsnUtil.getUnvalidFormRs(cur.getString(9));
			newContact = null;
			byte[] imageData = null;
			try {
			imageData = getContactHeadFromRMS(contact.CONTACT_imid);
			}catch(Exception ee)
			{
				ee.printStackTrace();
			}
			newContact=newContact(contact.SUBITEM_STATE,contact.SUBITEM_JID, contact.CONTACT_imid , contact.CONTACT_nickname, 
					contact.CONTACT_gbcode, contact.CONTACT_groupId,
					contact.CONTACT_contactFlag, contact.CONTACT_portraithash, contact.CONTACT_impresa,
					null,null, contact.CONTACT_gleam, contact.CONTACT_url,imageData);
			addContact(newContact,ROSTER_NORMAL_HAS_OFFLINE,app.groupData);
			///////////////////
			//#ifdef support_exchange_contact_group
			
			//////////////////增加按状态排序功能
			if(MSNApplication.ROSTER_NORMAL_NO_OFFLINE == getGroupOrder())
			{
				addContact(newContact,getGroupOrder(), groupDataNormalOnlyOnline);
			}
			//#endif
			cur.moveToNext();
		}
	    }
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }

	}
	
	
	/**从记录库中获取该用户的deviceID*/
	public void getDeviceIdFromRMS(String primaryKey)
	{
	    try {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		Cursor cur = app.DB.getLoginUserData(primaryKey);
		if(cur == null)
		{
			return;
		}
		if(cur.getCount() == 0)
		{
			return;
		}		
		devicedId =MsnUtil.getUnvalidFormRs(cur.getString(14));	
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}

	
    //////////////
	/**
	 * 获取指定会话中的所有消息.
	 * @param sid	会话ID
	 * @return 消息向量
	 */
	public final Vector getSessionMessages(String sid)
	{
		Item session =getSessionBySid(sid);
		if(session!=null)
		    return session.SESSION_contents;
		else
		    return null;
	}
	
	/**根据ftID 得到当前的消息*/
	public Item getOneMessageByFtID(Item session,String ftID)
	{
		if(session.SESSION_contents.size()<=0)
			return null;
		
		Item currentMessage=null;
		for(int i=0;i<session.SESSION_contents.size();i++)
		{
			currentMessage=(Item)session.SESSION_contents.elementAt(i);
			if(currentMessage.MESSAGE_FILE_ftID!=null&&currentMessage.MESSAGE_FILE_ftID.length()!=0)
			{
				if(currentMessage.MESSAGE_FILE_ftID.equals(ftID))
				{
					return currentMessage;
				}
			}
		}
		return null;
	}
	
	public Item getOneMessageByTransferID(Item session,String transferID)
	{
		if(session.SESSION_contents.size()<=0)
			return null;
		
		Item currentMessage=null;
		for(int i=0;i<session.SESSION_contents.size();i++)
		{
			currentMessage=(Item)session.SESSION_contents.elementAt(i);
			if(currentMessage.MESSAGE_FILE_transferID!=null&&currentMessage.MESSAGE_FILE_transferID.length()!=0)
			{
				if(currentMessage.MESSAGE_FILE_transferID.equals(transferID))
				{
					return currentMessage;
				}
			}
		}
		return null;
	}
	
    
    /**
     * * 发信息
	 * @param contact  联系人/群/陌生人
	 * @param sid 会话sid
	 * @param content 内容
	 **/
    public void sendMessage(Contact contact, String sid,String content)
	{
		String imid=null;

		if (sid == null)//没有会话id 需要先查找id 如果没找到 就生成一个会话id
		{	
			Item session=null;
			imid=contact.CONTACT_imid;
			session =getSessionFromContact(imid,sid);
			if (session == null)
			{
				session = addNewSession(contact,SESSION_TYPE_CHAT, null);
				sid = session.TOPITEM_JID;
			} 
			else
			{
				sid = session.TOPITEM_JID;
			}
			eventNotify(EventListener.EVENT_OPEN_CHAT_WINDOW, session);
		}
		Item message = null;
		if(contact.CONTACT_contactFlag==CONTACT_STRANGER_FLAG)//陌生人
		{
			jabber.sendMessageRequest(contact.CONTACT_imid, content,sid,false);
			message=addMessage(MESSAGE_TYPE_CHAT, sid, null,contact.CONTACT_imid, myVCARD_myNickname, 
					 content, MsnUtil.toString(new Date()),false,null,null);
		}
		else
		{
			jabber.sendMessageRequest(contact.SUBITEM_JID,content,sid,true);
			message=addMessage(MESSAGE_TYPE_CHAT, sid, contact.SUBITEM_JID,contact.CONTACT_imid, myVCARD_myNickname, 
					 content, MsnUtil.toString(new Date()),false,null,null);
		}	
		message.MESSAGE_IS_SEND=true;
		eventNotify(EventListener.EVENT_SEND_MESSAGE, message);
	}
    
    public void sendTransferFileMessage(Contact contact,String sid,String fileName,String fileSize,byte[] fileData)
    {
		String imid=null;

		if (sid == null)//没有会话id 需要先查找id 如果没找到 就生成一个会话id
		{	
			Item session=null;
			imid=contact.CONTACT_imid;
			session =getSessionFromContact(imid,sid);
			if (session == null)
			{
				session = addNewSession(contact,SESSION_TYPE_CHAT, null);
				sid = session.TOPITEM_JID;
			} 
			else
			{
				sid = session.TOPITEM_JID;
			}
			eventNotify(EventListener.EVENT_OPEN_CHAT_WINDOW, session);
		}
		Item message = null;
		Item session =getSessionBySid(sid);
		if(session != null)
		{
		    if(session.SESSION_sessionType == SESSION_TYPE_GROUP_CHAT)
		    {
		        return;
		    }
		}

		message=addFileInviteMessage(MESSAGE_TYPE_FILE_TRANSFER, sid,contact.SUBITEM_JID,contact.CONTACT_imid,
					myVCARD_myNickname,MsnUtil.toString(new Date()),null,fileName,fileSize,MESSAGE_FILE_TYPE_SEND_WAITING,fileData);
		jabber.sendTransferInvite(contact.SUBITEM_JID, sid, message.MESSAGE_FILE_ftID, fileName, fileSize);
		eventNotify(EventListener.EVENT_SEND_MESSAGE, message);
		
		if(!app.isTransferingMessageVector.contains(message))
			app.isTransferingMessageVector.addElement(message);
    }
    
    /**
     * 发送语音剪辑
     * @param contact
     * @param sid
     * @param mime
     * @param bytes
     * @param base64Length
     * @param url
     * @param voiceData
     * @param isInnerSend
     */
    public void sendTransferVoiceMessage(Contact contact,String sid,String mime,String bytes,String url,byte[] voiceData,boolean isInnerSend)
    {
    	String imid=null;

		if (sid == null)//没有会话id 需要先查找id 如果没找到 就生成一个会话id
		{	
			Item session=null;
			imid=contact.CONTACT_imid;
			session =getSessionFromContact(imid,sid);
			if (session == null)
			{
				session = addNewSession(contact,SESSION_TYPE_CHAT, null);
				sid = session.TOPITEM_JID;
			} 
			else
			{
				sid = session.TOPITEM_JID;
			}
			eventNotify(EventListener.EVENT_OPEN_CHAT_WINDOW, session);
		}

		String stamp = MsnUtil.toString(new Date());

		Item  msg=null;
    	if(isInnerSend)//带内 直接发送出去了
    	{
    		String sendData=MsnUtil.encode(voiceData);
    		String base64Length=String.valueOf(sendData.length());
            
    		msg = addVoiceMessage(MESSAGE_TYPE_VOICECLIP, sid,contact.SUBITEM_JID,contact.CONTACT_imid,
    				myVCARD_myNickname,stamp,mime,null,
    				MESSAGE_VOICE_TYPE_INNER_SEND_COMPLETE,voiceData, true);
    		eventNotify(EventListener.EVENT_SEND_MESSAGE, msg);
    		jabber.sendVoiceClip(msg.MESSAGE_jid,msg.MESSAGE_sid,"audio/amr",bytes,base64Length,sendData,null,true);
    	}
    	else//带外 
    	{
    		
    		msg = addVoiceMessage(MESSAGE_TYPE_VOICECLIP, sid,contact.SUBITEM_JID,contact.CONTACT_imid,
    				myVCARD_myNickname,stamp,mime,null,
    				MESSAGE_VOICE_TYPE_OUTER_SEND_SENDING,voiceData, false);
    		//eventNotify(EventListener.EVENT_SEND_MESSAGE, msg);
    		msg.MESSAGE_FILEOUTTRANSFER=new FileOuterTransfer(this,FileOuterTransfer.command_select_uploadFile,msg,null,null);
			//app.isTransfering=true;//
			if(!app.isTransferingMessageVector.contains(msg))
				app.isTransferingMessageVector.addElement(msg);
			app.currentOuterMessage=msg;
    	}
		
    }
    
    /**通过联系人生成一个会话 返回一个会话*/
    public Item openChatUI(Contact contact, String sid)
	{
		String imid=null;
		Item session=null;
		if (sid == null)//没有会话id 需要先查找id 如果没找到 就生成一个会话id
		{	
			
			imid=contact.CONTACT_imid;
			session =getSessionFromContact(imid,sid);
			if (session == null)
			{
				session = addNewSession(contact,SESSION_TYPE_CHAT, null);
				sid = session.TOPITEM_JID;
			} 
			else
			{
				sid = session.TOPITEM_JID;
			}
			
			eventNotify(EventListener.EVENT_ADD_NEW_SESSION, session);
		}
		return session;
	}
    
    
	/**
	 * 接受或者拒绝验证请求
	 *@param handleType:拒绝deny,同意accept
	 *@param imid:联系人的imid 
	 */
	public void acceptOrDenyVerifyFriend(String handleType,String imid)
	{
        try 
        {
        	jabber.sendAcceptOrDenyVerifyFriend(handleType, imid);
            verifyFriendVector.removeElementAt(0);
            if (verifyFriendVector.isEmpty()) 
                eventNotify(EventListener.EVENT_OVER_VERIFY_FRIEND,null);     
            else 
            	eventNotify(EventListener.EVENT_MORE_VERIFY_FRIEND,null);
        } 
        catch (Exception e) 
        {
        	e.printStackTrace();
        }
	}
    
    
	/** 每次开始联网 注册/登录 要注意置初始值 */
	public void reset()
	{
		try
		{
			//picaNumber=null;
			isSynchronize=false;
			isLogout=false;
			isExit=false;
			//将个人资料置空了
			//myVCARD_myNickname=null;//昵称
			//myVCARD_myImpresa=null;//心情短语
			//myVCARD_resourceId=null;//个人资料id
			verifyFriendVector.removeAllElements();
			 //#ifdef support_head_image
			//myVCARD_headImageDataLength=0;//个人图片字节数长度
			myVCARD_headImageData=null;//图片数据
			//#endif
			//app.childData.clear();
			
			//////////////////增加按状态排序功能
			if(app.isShowOnline)
			{
			    setGroupOrder(MSNApplication.ROSTER_NORMAL_NO_OFFLINE);
			}
			else
			{
			    setGroupOrder(MSNApplication.ROSTER_NORMAL_HAS_OFFLINE);
			}
			groupData.clear();
			groupDataNormalOnlyOnline.removeAllElements();			
			groupDataByStatus.removeAllElements();
			addGroupToList(newGroup(MSNApplication.GROUP_ONLINE_ID, null,
					mActivity.getString(R.string.text_group_online), GROUP_USER_FLAG, false),groupDataByStatus);
	    	addGroupToList(newGroup(MSNApplication.GROUP_OFFLINE_ID, null,
	    			mActivity.getString(R.string.text_group_offline), GROUP_USER_FLAG, false),groupDataByStatus);
			
	    	groupDataStatusOnlyOnline.removeAllElements();
	    	addGroupToList(newGroup(MSNApplication.GROUP_ONLINE_ID, null,
	    			mActivity.getString(R.string.text_group_online), GROUP_USER_FLAG, false),groupDataStatusOnlyOnline);
	    	addGroupToList(newGroup(MSNApplication.GROUP_OFFLINE_ID, null,
	    			mActivity.getString(R.string.text_group_offline), GROUP_USER_FLAG, false), groupDataStatusOnlyOnline);
	    	//////////////////////////
	    	
			//app.childDataNormalOnlyOnline.clear();
			
			baseUrl = null;
			currentUrl = null;
			tempUrl = null;
			baseUrls.clear();
	    	htmlStrings.clear();
	    	urls.clear();
	    	urlAndMethod.clear();
			
			app.sessionData.clear();
			//isOpenNewMessageSound=true;
			//isOpenOnlineSound=true;
			myFirstContactGroupVersion=null;
			devicedId=null;
			isLoginSuccess=false;
			maxSid = 0;
			//myVcardVersion=null;
		    myContactGroupVersion=null;
		    myState=STATE_ONLINE;
		    currentSession=null;
		    //isAutoLogin=false;
		   // isSavePassword=false;
		    hasContactOnline=false;
		    
			transfer_limit=2048000;
			data_url=null;
			data_method=null;
			isInnerSendFile=true;
			isInnerReceiveFile=true;
			isInnerSendVoice=true;
			isInnerReceiveVoice=true;
			//isShowOnline=true;
	        ad_chat_Vector=null;
	        ad_chat_Vector=new Vector();
			//#ifdef support_exchange_contact_group
	      //  isShowOnline=true;
			//#endif
	    	//#ifdef support_head_image
	    	//isReceiveHeadChange=true;
	    	//#endif
	        
			//#ifdef support_exchange_contact_group
			//#endif
			capability=null;
			if(isTransferingMessageVector==null)
			{
				isTransferingMessageVector=new Vector();
			}
			else
				isTransferingMessageVector.removeAllElements();
			if(isTransferingOuterSendHeadVector==null)
			{
				isTransferingOuterSendHeadVector=new Vector();
			}
			else
				isTransferingOuterSendHeadVector.removeAllElements();
			jids.removeAllElements();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
    
    
	/**
	 * 删除掉同步相关的所有记录库（联系人分组 联系人  联系人版本 deviceID）
	 *@param allUsername 登录用户帐号
	 */
	public void deleteSynchronizedAllRecordStore()
	{
		try 
        {
			isSynchronize=true;
			deleteAllRosterData(app.GetLiveID());
			deleteAllContactHead();
			removeContactGroupVersionFromRMS();
			removeDeviceIDFromRMS();
        }
        catch(Exception e)
        {
        	removeContactGroupVersionFromRMS();
        	e.printStackTrace();
        }
	}
	
	//#ifdef support_groupmessage
    /**
     * 关闭会话  多人对话，在结束对话时发送此命令
     * @param sid
     */
    public final void closeSession(String sid)
    {
        app.jabber.sendCloseSession(sid);
    }
    //#endif

	
	public void receiveTransferCancelResponse(String from, String type,
			String transferID, String sid, String transfer_ftID,
			String transfer_action) 
	{
		Item session = null;
		session=getSessionBySid(sid);
		if(session==null)
			return;
		Item message=null;
		message=getOneMessageByTransferID(session,transferID);
		if(message==null)
			return;
		message.MESSAGE_FILE_DATA=null;
		message.MESSAGE_FILE_STATUS_TYPE=MESSAGE_FILE_TYPE_RECEIVE_CANCEL;	
		eventNotify(EventListener.EVENT_UPDATE_MESSAGE, message);
	}

	
	public void receiveTransferCompleteAck(String from, String type,
			String transferID, String sid, String transfer_ftID,
			String transfer_action) 
	{
		Item session = null;
		session=getSessionBySid(sid);
		if(session==null)
			return;
		Item message=null;
		message=getOneMessageByFtID(session,transfer_ftID);
		if(message==null)
			return;
		message.MESSAGE_FILE_STATUS_TYPE=MESSAGE_FILE_TYPE_SEND_COMPLETE;
		eventNotify(EventListener.EVENT_UPDATE_MESSAGE, message);	
		message.MESSAGE_FILE_DATA=null;
		System.gc();
	}
	
	public void receiveTransferComplete(String from, String type,
			String transferID, String sid, String transfer_ftID,
			String transfer_action) 
	{
		Item session = null;
		session=getSessionBySid(sid);
		if(session==null)
			return;
		Item message=null;
		message=getOneMessageByFtID(session,transfer_ftID);
		if(message==null)
			return;
		message.MESSAGE_FILE_STATUS_TYPE=MESSAGE_FILE_TYPE_RECEIVE_COMPLETE;
		boolean isSaveSuccess=MsnUtil.saveDataToFile("/sdcard/", message.MESSAGE_FILE_VOICE_NAME, message.MESSAGE_FILE_DATA);
		if(isSaveSuccess)
		{
			message.MESSAGE_FILE_DATA=null;
			System.gc();
		}
		eventNotify(EventListener.EVENT_UPDATE_MESSAGE, message);	
		jabber.sendTransferInnerReceiveComplete(transferID, sid, transfer_ftID);
	}
	

	
	public void receiveTransferData(String from, String jid, String imid,
			String type, String transferID, String sid, String transfer_ftID,
			String transfer_seqid, String transfer_action,
			String transfer_bytes, String transfer_start, String transfer_end,
			String transfer_base64Length, String transfer_base64Data) {
		
		Item session = null;
		Contact contact=null;
		if(jid==null&&imid!=null)
		{	
			contact=newStranger(imid);
			session =getSessionFromContact(imid,sid);
		}
		else//好友
		{
			contact=getContact(jid);
			if(contact!=null)
			{
				session =getSessionFromContact(contact.CONTACT_imid,sid);
			}
			else
			{
				return;//微软有bug 某些时候可能陌生人的消息也是仅有jid而无imid 这时候客户端就不处理了
			}
		}
		if(session==null)
			return;
		// 本地存在和该好友的2人会话,则以本地sid为准
		sid = session.TOPITEM_JID;
		Item message=null;
		
		message=getOneMessageByTransferID(session,transferID);
		if(message==null)
			return;
		if(transfer_seqid!=null&&transfer_seqid.length()!=0)
			message.MESSAGE_FILE_SEQID=transfer_seqid;
		
		
		int start=Integer.parseInt(transfer_start);
		int end=Integer.parseInt(transfer_end);
		int totalLength=Integer.parseInt(transfer_bytes);
		int receiveLength=end-start+1;//原数据
		byte[] receiveData=MsnUtil.decode(transfer_base64Data);//base64数据转成原数据
		System.arraycopy(receiveData, 0, message.MESSAGE_FILE_DATA, start, receiveLength);
//		if(end+1>=totalLength)//接收完毕
//		{
//			message.MESSAGE_GAUGE_INDEX="100";
//			eventNotify(EventListener.EVENT_UPDATE_GAUGE_MESSAGE, message);
//			MsnUtil.OutPutFile(message);
//			message.MESSAGE_FILE_STATUS_TYPE=MESSAGE_FILE_TYPE_RECEIVE_COMPLETE;
//			eventNotify(EventListener.EVENT_UPDATE_MESSAGE, message);
//		}
//		else
		{
			int gauge=((end*100)/totalLength);		
			message.MESSAGE_GAUGE_INDEX=Integer.toString(gauge);	
			eventNotify(EventListener.EVENT_UPDATE_GAUGE_MESSAGE, message);
			
			jabber.sendTransferInnerResponse(transferID,sid,message.MESSAGE_FILE_ftID,transfer_seqid);
		}
	}

	
	public void receiveTransferInnerResponse(String from, String type,
			String transferID, String sid, String transfer_ftID,
			String transfer_seqid, String transfer_action,
			String transfer_status) {

		Item session = null;
		session=getSessionBySid(sid);
		if(session==null)
			return;
		Item message=null;
		message=getOneMessageByFtID(session,transfer_ftID);
		if(message==null)
			return;
		
		if(transfer_status.equals("ok")||transfer_status.equals("cancel"))
		{
			int totalLength=Integer.parseInt(message.MESSAGE_FILE_SIZE);
			int start=Integer.parseInt(message.MESSAGE_FILE_START);//上次的起始位置
			int end=Integer.parseInt(message.MESSAGE_FILE_END);//上次的结束位置
			byte[] sendData=null;
			int sendLength=0;
			if(message.MESSAGE_FILE_SEQID.equals(transfer_seqid))
			{
				message.MESSAGE_FILE_START=Integer.toString(end+1);//本次开始位置
				if(end+1==totalLength)//发送完毕
				{
					jabber.sendTransferInnerComplete(transferID,sid,transfer_ftID);
					
					message.MESSAGE_GAUGE_INDEX="100";				
					eventNotify(EventListener.EVENT_UPDATE_GAUGE_MESSAGE, message);
					message.MESSAGE_FILE_STATUS_TYPE=MESSAGE_FILE_TYPE_SEND_COMPLETE;
					eventNotify(EventListener.EVENT_UPDATE_MESSAGE, message);
					return;
				}
				else
				{
					int oldSeqid=Integer.parseInt(message.MESSAGE_FILE_SEQID);
					int newSeqid=oldSeqid+1;
					message.MESSAGE_FILE_SEQID=Integer.toString(newSeqid);
					
					int remainLength=totalLength-(end+1);
					sendLength=0;
					if(remainLength>per_file_size)
					{
						sendLength=per_file_size;
					}
					else
					{
						sendLength=remainLength;
					}
					sendData=new byte[sendLength];
					System.arraycopy(message.MESSAGE_FILE_DATA, end+1, sendData, 0, sendLength);
					String base64Data=MsnUtil.encode(sendData);
					
					end=end+1+sendLength-1;
					
					this.jabber.sendTransferInner(transferID, sid, transfer_ftID, message.MESSAGE_FILE_SEQID, message.MESSAGE_FILE_SIZE, message.MESSAGE_FILE_START,Integer.toString(end), Integer.toString(base64Data.length()), base64Data);
					int gauge=((start*100)/totalLength);
					message.MESSAGE_FILE_END=Integer.toString(end);
					
					message.MESSAGE_GAUGE_INDEX=Integer.toString(gauge);				
					eventNotify(EventListener.EVENT_UPDATE_GAUGE_MESSAGE, message);
				}
			}
			else//不一致 则重新发送请求
			{
				int count=Integer.parseInt(message.MESSAGE_FILE_VOICE_TRANSFER_COUNT);
				message.MESSAGE_FILE_VOICE_TRANSFER_COUNT=String.valueOf(++count);
				if (count <= MAX_TRY_COUNT)
				{
					sendLength=end-start+1;//上次发送的数据长度
					sendData=new byte[sendLength];
					System.arraycopy(message.MESSAGE_FILE_DATA, start, sendData, 0, sendLength);
					String base64Data=MsnUtil.encode(sendData);
					this.jabber.sendTransferInner(transferID, sid, transfer_ftID, message.MESSAGE_FILE_SEQID, message.MESSAGE_FILE_SIZE, message.MESSAGE_FILE_START,Integer.toString(end), Integer.toString(base64Data.length()), base64Data);
				} 
				else
				{
					message.MESSAGE_FILE_STATUS_TYPE=MESSAGE_FILE_TYPE_SEND_EXCEPTION;
					eventNotify(EventListener.EVENT_UPDATE_MESSAGE, message);
				}

			}
		}
		else if(transfer_status.equals("fail"))
		{
			message.MESSAGE_FILE_STATUS_TYPE=MESSAGE_FILE_TYPE_SEND_EXCEPTION;
			eventNotify(EventListener.EVENT_UPDATE_MESSAGE, message);
		}

	}

	
	public void receiveTransferInviteRequest(String from, String jid,
			String imid, String type, String transferID, String sid,
			String ftID, String action, String transfer_name,
			String transfer_size) 
	{
		Item session = null;
		Contact contact=null;
		if(jid==null&&imid!=null)
		{	
			contact=newStranger(imid);
			session =getSessionFromContact(imid,sid);
		}
		else//好友
		{
			contact=getContact(jid);
			if(contact!=null)
			{
				session =getSessionFromContact(contact.CONTACT_imid,sid);
			}
			else
			{
				return;//微软有bug 某些时候可能陌生人的消息也是仅有jid而无imid 这时候客户端就不处理了
			}
		}
	
		if (session == null)
		{ 
			session = addNewSession(contact, SESSION_TYPE_CHAT, sid);
			sid = session.TOPITEM_JID;
		} 
		else
		{ // 本地存在和该好友的2人会话,则以本地sid为准
			sid = session.TOPITEM_JID;
		}
		String stamp = MsnUtil.toString(new Date());


		Item  message = addFileInviteMessage(MESSAGE_TYPE_FILE_TRANSFER, sid,jid,contact.CONTACT_imid,
				contact.CONTACT_nickname,stamp,transferID,transfer_name,transfer_size,MESSAGE_FILE_TYPE_RECEIVE_WAITING,null);
		
		eventNotify(EventListener.EVENT_RECEIVED_MESSAGE, message);
		
	}


	
	public void receiveTransferInviteResponse(String from, String type,
			String transferID, String sid, String transfer_ftID,
			String transfer_action) {
	
		Item session = null;
		session=getSessionBySid(sid);
		if(session==null)
			return;
		Item message=null;
		message=getOneMessageByFtID(session,transfer_ftID);
		if(message==null)
			return;
		message.MESSAGE_FILE_transferID=transferID;
		if(transfer_action.equals("reject"))
		{
			message.MESSAGE_FILE_STATUS_TYPE=MESSAGE_FILE_TYPE_SEND_WAITING_REFUSED;
			eventNotify(EventListener.EVENT_UPDATE_MESSAGE, message);
		}
		else
		{
            if(message.MESSAGE_FILE_STATUS_TYPE==MESSAGE_FILE_TYPE_SEND_CANCEL)
            {
                return;
            }
			if(this.isInnerSendFile)//带内
			{
			    if(message.MESSAGE_FILE_DATA == null)
			    {
			        return;
			    }
				message.MESSAGE_FILE_SEQID="0";
				message.MESSAGE_FILE_START="0";
				message.MESSAGE_FILE_VOICE_TRANSFER_COUNT="0";
				int totalLength=Integer.parseInt(message.MESSAGE_FILE_SIZE);
				int start=Integer.parseInt(message.MESSAGE_FILE_START);
				int sendLength=totalLength;
				if(totalLength>per_file_size)
				{
					sendLength=per_file_size;
				}
				byte[] sendData=new byte[sendLength];
				System.arraycopy(message.MESSAGE_FILE_DATA, start, sendData, 0, sendLength);
				String base64Data=MsnUtil.encode(sendData);
				int end=start+sendLength-1;
				
				this.jabber.sendTransferInner(transferID, sid, transfer_ftID, message.MESSAGE_FILE_SEQID, message.MESSAGE_FILE_SIZE, message.MESSAGE_FILE_START,Integer.toString(end), Integer.toString(base64Data.length()), base64Data);
				
				//message.MESSAGE_FILE_START=Integer.toString(end+1);//本次的起始位置
				message.MESSAGE_FILE_END=Integer.toString(end);//本次的结束位置
				message.MESSAGE_FILE_STATUS_TYPE=MESSAGE_FILE_TYPE_SEND_SENDING;
				message.MESSAGE_GAUGE_INDEX="0";	
				eventNotify(EventListener.EVENT_UPDATE_MESSAGE, message);
			}
			else//带外
			{
				message.MESSAGE_FILE_STATUS_TYPE=MESSAGE_FILE_TYPE_SEND_SENDING;
				message.MESSAGE_GAUGE_INDEX="0";
				eventNotify(EventListener.EVENT_UPDATE_MESSAGE, message);
				message.MESSAGE_FILEOUTTRANSFER=new FileOuterTransfer(this,FileOuterTransfer.command_select_uploadFile,message,null,null);
			
			}
		}
		
	}

	
	public void receiveTransferOuterReceive(String from, String jid,
			String imid, String type, String transferID, String sid,
			String transfer_ftID, String transfer_action, String transfer_url) 
	{
		Item session = null;
		Contact contact=null;
		if(jid==null&&imid!=null)
		{	
			contact=newStranger(imid);
			session =getSessionFromContact(imid,sid);
		}
		else//好友
		{
			contact=getContact(jid);
			if(contact!=null)
			{
				session =getSessionFromContact(contact.CONTACT_imid,sid);
			}
			else
			{
				return;//微软有bug 某些时候可能陌生人的消息也是仅有jid而无imid 这时候客户端就不处理了
			}
		}
		if(session==null)
			return;

		// 本地存在和该好友的2人会话,则以本地sid为准
		sid = session.TOPITEM_JID;
		Item message=null;
		
		message=getOneMessageByTransferID(session,transferID);
		if(message==null)
			return;
		message.MESSAGE_FILE_URL=transfer_url;
		/////// 带外
		message.MESSAGE_FILE_STATUS_TYPE=MESSAGE_FILE_TYPE_RECEIVE_SENDING;
		message.MESSAGE_GAUGE_INDEX="0";
		eventNotify(EventListener.EVENT_UPDATE_MESSAGE, message);
		message.MESSAGE_FILEOUTTRANSFER=new FileOuterTransfer(this,FileOuterTransfer.command_downloadFile,message,null,null);
		
		///////
		
	}

	
	public void receiveTransferVoiceclip(String from, String jid,String imid, String type,
			String sid, String transfer_data_mime, String transfer_bytes,
			String transfer_base64Length, String transfer_base64Data,
			String transfer_url_mime, String transfer_url) 
	{
		Item session = null;
		Contact contact=null;
		if(jid==null&&imid!=null)
		{	
			contact=newStranger(imid);
			session =getSessionFromContact(imid,sid);
		}
		else//好友
		{
			contact=getContact(jid);
			if(contact!=null)
			{
				session =getSessionFromContact(contact.CONTACT_imid,sid);
			}
			else
			{
				return;//微软有bug 某些时候可能陌生人的消息也是仅有jid而无imid 这时候客户端就不处理了
			}
		}
	
		if (session == null)
		{ 
			session = addNewSession(contact, SESSION_TYPE_CHAT, sid);
			sid = session.TOPITEM_JID;
		} 
		else
		{ // 本地存在和该好友的2人会话,则以本地sid为准
			sid = session.TOPITEM_JID;
		}
		String stamp = MsnUtil.toString(new Date());

		if(transfer_url!=null&&transfer_url.length()!=0)//带外
		{
			Item  message = addVoiceMessage(MESSAGE_TYPE_VOICECLIP, sid,jid,contact.CONTACT_imid,
					contact.CONTACT_nickname,stamp,transfer_url_mime,transfer_url,
					MESSAGE_VOICE_TYPE_OUTER_RECEIVE_WAITING,null, true);
			
			eventNotify(EventListener.EVENT_RECEIVED_MESSAGE, message);
			
			
//			/////// 带外
//			message.MESSAGE_GAUGE_INDEX="0";
//			eventNotify(EventListener.EVENT_UPDATE_MESSAGE, message);
//			message.MESSAGE_FILEOUTTRANSFER=new FileOuterTransfer(this,FileOuterTransfer.command_downloadFile,message,null,null);
//			//app.isTransfering=true;//
//			if(!app.isTransferingMessageVector.contains(message))
//				app.isTransferingMessageVector.addElement(message);
		}
		else//带内
		{
			byte[] data=MsnUtil.decode(transfer_base64Data);
			Item  message = addVoiceMessage(MESSAGE_TYPE_VOICECLIP, sid,jid,contact.CONTACT_imid,
					contact.CONTACT_nickname,stamp,transfer_data_mime,null,
					MESSAGE_VOICE_TYPE_RECEIVE_OVER,data, true);
			eventNotify(EventListener.EVENT_RECEIVED_MESSAGE, message);		
		}
	}
   
    
	/**
	 * 添加声音剪辑接收文件的信息到会话向量 要进行数量判断 
	 * @param type 
	 * @param sid  会话id
	 * @param jid
	 * @param imid 
	 * @param nickName 昵称
	 * @param stamp
	 * @param transfer_data_mime audio/wav|audio/amr|audio/qcp
	 * @param url 下载文件的url
	 * @param messageFileType 当前的状态（接收 拒绝等）
	 * @param fileData 附件数据
	 */	
	synchronized Item addVoiceMessage(byte type, String sid, String jid,String imid,
			String nickName, String stamp,String transfer_data_mime,
			String url,
			byte messageVoiceType,byte[]VoiceData, boolean isInnerSend)
	{
		if(imid!=null&&imid.equals(this.GetLiveID()))
		{
			nickName=myVCARD_myNickname;
		}

		Item session = getSessionBySid(sid);
		if (session != null)
		{
			Item message=new Item();
			message.MESSAGE_type=type;
			message.MESSAGE_sid=sid;
			message.MESSAGE_jid=jid;
			message.MESSAGE_imid=imid;
			message.MESSAGE_nickname=nickName;
			message.MESSAGE_stamp=stamp;
			message.MESSAGE_VOICE_URL=url;
			message.MESSAGE_VOICE_MIME=transfer_data_mime;
			message.MESSAGE_VOICE_STATUS_TYPE=messageVoiceType;
			message.MESSAGE_VOICE_DATA=VoiceData;
			//if(session.SESSION_contents.size()>=MESSAGE_MAX_COUNT)
			//	session.SESSION_contents.removeElementAt(0);//删掉第一条
			if(isInnerSend)
			{
				session.SESSION_contents.addElement(message);
				session.SESSION_numOfNewMsg=Integer.toString(Integer.parseInt(session.SESSION_numOfNewMsg)+1);
			}
			
			return message;
		} 
		else
		{
			return null;
		}
	}
	
	
	 //#ifdef support_head_image
	/**重置新的头像*/
	public final void setHeadImage(byte[] image)
	{
		if(image!=null)
		{
			myVCARD_headImageData=image;
			myVCARD_headImageDataLength=image.length;
		}
		else
		{
			myVCARD_headImageData=null;
			myVCARD_headImageDataLength=0;
		}
	}
	//#endif
	
	/**新增一个 自己的头像保存到file*/
	public void addVcardHeadToRMS()
	{
		MsnUtil.saveDataToFile(RMS_VCARD_HEAD,this.GetLiveID(),this.myVCARD_headImageData);
	}
	
	/**删除自己的头像*/
	public void deleteVcardHead()
	{
		MsnUtil.deleteDataFormFile(RMS_VCARD_HEAD,this.GetLiveID());
	}
	
	/**取出自己的头像*/
	public void getVcardHead(String imid)
	{
		byte[] data=MsnUtil.getDataFromFile(RMS_VCARD_HEAD,imid);
		if(data!=null)
		{
			app.myVCARD_headImageData=data;
		}
	}
	
	/**取联系人的头像*/
	public byte[] getContactHeadFromRMS(String imid)
	{
		byte[] data=MsnUtil.getDataFromFile(RMS_CONTACT_HEAD+this.GetLiveID()+"/",imid);
		return data;
	}
	
	/**增加一个联系人的头像数据到记录库中*/
	public void addContactHeadToRMS(Contact contact)
	{
		if(isSynchronize)
			return;//如果在同步 则不对该RMS操作	
		if(isReceiveHeadChange)
		{
			if(contact.CONTACT_imageData==null)
			{
				contact.CONTACT_hasRMSImageData=false;
			}
			else
			{
				MsnUtil.saveDataToFile(RMS_CONTACT_HEAD+this.GetLiveID()+"/",contact.CONTACT_imid,contact.CONTACT_imageData);
				contact.CONTACT_hasRMSImageData=true;
			}
		}
		else
		{
			contact.CONTACT_hasRMSImageData=false;
		}
	
	}
	
	/**
	 * 从记录库中读取并初始化联系人的头像
	 * @param String name 当前用户名（组成数据库名） 
	 * @param Item contact 该联系人对象
	 * @return Item 包含联系人头像的新联系人对象
	 */	
	public void initContactHeadFromRMS(String username)
	{
		Vector vc=MsnUtil.getChildrenFile(RMS_CONTACT_HEAD+this.GetLiveID()+"/");
		if(vc!=null&&vc.size()!=0)
		{
			String imid;
			for(int i=0;i<vc.size();i++)
			{
				imid=(String)vc.elementAt(i);
				Contact contact=this.getContact(imid,false);
				if(contact==null)
					continue;
				if(this.isReceiveHeadChange)
					contact.CONTACT_hasRMSImageData=true;
				else
				{
					contact.CONTACT_hasRMSImageData=false;
				}
			}
		}

	}
	
	/**删除所有联系人的头像数据*/
	public void deleteAllContactHead()
	{
		MsnUtil.deleteDirectory(RMS_CONTACT_HEAD+this.GetLiveID()+"/");
	}
	
	/**删除该联系人的头像数据*/
	public void deleteContactHeadFromFile(String imid)
	{
		MsnUtil.deleteDataFormFile(RMS_CONTACT_HEAD+this.GetLiveID()+"/",imid);
	}
	
	//#ifdef support_exchange_contact_group
	public void setIsOnlyShowOnlineContact(boolean isShow)
	{
		this.isShowOnline=isShow;
	}
	
	/**是否显示脱机联系人*/
	public boolean getIsOnlyShowOnlineContact()
	{
		return this.isShowOnline;
	}
	//#endif
	
	//#ifdef support_exchange_contact_group
	public byte getGroupOrder()
	{
		return groupOrder;
	}
	
	/**设置联系人列表的分类*/
	public void setGroupOrder(byte order)
	{
	 	groupOrder = order;
	}
	//#endif
	
	//////////////////添加按状态排序联系人
	public Vector<Group> getCurrentGroupData()
	{
		if(this.getGroupOrder() == ROSTER_NORMAL_NO_OFFLINE)
		{
			return groupDataNormalOnlyOnline;
		}	
		else if(getGroupOrder() == ROSTER_STATE_HAS_OFFLINE)
		{
			return groupDataByStatus;
		}
		else if(getGroupOrder() == ROSTER_STATE_NO_OFFLINE)//分组显示脱机联系人和在线联系人
		{
			return groupDataStatusOnlyOnline;
		}
		else
		{
			return groupData;
		}
			
	}
	
//	public List<List<Map<String, Contact>>> getCurrentChildData()
//	{
//		if(this.getGroupOrder()==ROSTER_NORMAL_HAS_OFFLINE)
//			return childData;
//		else
//			return childDataNormalOnlyOnline;
//	}
	
	/**保存所有联系人列表*/
	public void saveAllContactList()
	{
	    try {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		Contact contact;
		for(int i = 0; i < app.groupData.size(); i++)
		{
			for(int n = 0; n < app.groupData.elementAt(i).contactVector.size(); n++)
			{
	            //Map<String, Contact> CurChild = (Map<String, Contact>)app.childData.get(i).get(n);
	            //Contact contact=CurChild.get("CONTACT");
				contact = null;
				contact=app.groupData.elementAt(i).contactVector.elementAt(n);
				app.DB.insertAllRosterData(strLiveID, contact.SUBITEM_JID, contact.CONTACT_imid, contact.CONTACT_nickname, contact.CONTACT_gbcode, contact.CONTACT_groupId, contact.CONTACT_portraithash, contact.CONTACT_impresa, contact.CONTACT_gleam, contact.CONTACT_url);
			}
		}
		app.DB.CloseDB();
		System.gc();
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}
	
	/**保存所有分组列表*/
	public void saveAllGroupList()
	{
	    try {
		app.DB = new DatabaseHelper(mActivity, DB_NAME, null, DB_VERSION);
		Group group;
		for(int i = 0; i < app.groupData.size(); i++)
		{
//        	Map<String, Group> map=app.groupData.get(i);            
//        	Group group=(Group)map.get("GROUP");
			group = null;
			group = (Group)app.groupData.elementAt(i);
            app.DB.insertAllGroupData(strLiveID, group.GROUP_id, group.GROUP_gbcode, group.GROUP_name, Byte.toString(group.GROUP_flag));
        }
		app.DB.CloseDB();
		System.gc();
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}
	
	/**保存所有联系人列表到文件中
	 * @param LiveID
	 * */
	public void saveAllContactListToFile(String LiveID)
	{
		ByteArrayOutputStream bytearrayoutputstream = new ByteArrayOutputStream();
		DataOutputStream dataoutputstream = new DataOutputStream(bytearrayoutputstream);

		//写入分组的数量（便于读取，见initGroupsContactsFromFile方法）
		try {
		try
		{
			dataoutputstream.writeInt(app.groupData.size());
		}
		catch (IOException e2)
		{
			// TODO Auto-generated catch block
			e2.printStackTrace();
		}
		Group group;
		Contact contact;
		for(int i = 0; i < app.groupData.size(); i++)
		{
			group = null;
			group = (Group)app.groupData.elementAt(i);
			//写入分组的信息
			try
			{
				dataoutputstream.writeUTF(MsnUtil.toValidRs(group.GROUP_id));
				dataoutputstream.writeUTF(MsnUtil.toValidRs(group.GROUP_gbcode));
				dataoutputstream.writeUTF(MsnUtil.toValidRs(group.GROUP_name));
				dataoutputstream.writeUTF(MsnUtil.toValidRs(Byte.toString(group.GROUP_flag)));
				//写入组中联系人的数量（便于读取，见initGroupsContactsFromFile方法）
				dataoutputstream.writeInt(group.contactVector.size());
			}
			catch (IOException e1)
			{
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			
			for(int n = 0; n < group.contactVector.size(); n++)
			{
	            //Map<String, Contact> CurChild = (Map<String, Contact>)app.childData.get(i).get(n);
	            //Contact contact=CurChild.get("CONTACT");
				contact = null;
				contact=group.contactVector.elementAt(n);
				//写入联系人信息
				try
				{
					dataoutputstream.writeUTF(MsnUtil.toValidRs(contact.SUBITEM_JID));
					dataoutputstream.writeUTF(MsnUtil.toValidRs(contact.CONTACT_imid));
					dataoutputstream.writeUTF(MsnUtil.toValidRs(contact.CONTACT_nickname));
					dataoutputstream.writeUTF(MsnUtil.toValidRs(contact.CONTACT_gbcode));
					dataoutputstream.writeUTF(MsnUtil.toValidRs(contact.CONTACT_groupId));
					dataoutputstream.writeUTF(MsnUtil.toValidRs(contact.CONTACT_portraithash));
					dataoutputstream.writeUTF(MsnUtil.toValidRs(contact.CONTACT_impresa));
					dataoutputstream.writeUTF(MsnUtil.toValidRs(contact.CONTACT_gleam));
					dataoutputstream.writeUTF(MsnUtil.toValidRs(contact.CONTACT_url));
					
				} catch (IOException e)
				{
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		try
		{
		byte rosterData[] = bytearrayoutputstream.toByteArray();
		deleteAllRosterData(LiveID);
		//存入文件中
		MsnUtil.saveDataToFile(RMS_CONTACT_DATA + LiveID + "/","rosterData",rosterData);
		}catch(Exception e)
		{
		    e.printStackTrace();
		}
		}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}	
	
	/**从文件中删除所有联系人列表
	 * @param LiveID
	 */
	public void deleteAllRosterData(String LiveID)
	{
	    try {
		MsnUtil.deleteDataFormFile(RMS_CONTACT_DATA + LiveID, "/rosterData");
	}
	    catch (Exception e)
	    {
	        e.printStackTrace();
	    }
	}
	
	
	/**
	 * 从文件中读取指定用户(自己)的分组数据和联系人数据并初始化
	 * @param LiveID
	 */
	public void initGroupsContactsFromFile(String LiveID)
	{
		File rosterDataFile = new File(RMS_CONTACT_DATA + LiveID +"/rosterData");
		FileInputStream fis = null;
		try
		{
			fis = new FileInputStream(rosterDataFile);
		} 
		catch (FileNotFoundException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
			//没有存则返回
			return;
		}
		
		//没有存则返回
		if(fis == null)
		{
			return;
		}
		//获取数据输出流
		DataInputStream dis = new DataInputStream(fis);
		
		int GroupCount = 0;
		//读取分组的数量
		try
		{
			GroupCount = dis.readInt();
		}
		catch (Exception e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
			//没有数据就返回
			return;
		}
		//分组的数量为零则返回
		if(0 == GroupCount)
		{
			return;
		}
		Group group;
		Group newGroup;
		int contactCount = 0;
		Contact contact;
		Contact newContact;
		for(int i=0; i<GroupCount; i++)
		{
			group = null;
			group=new Group();
			//获得组的信息
			try
			{
				group.GROUP_id = MsnUtil.getUnvalidFormRs(dis.readUTF());
				group.GROUP_gbcode = MsnUtil.getUnvalidFormRs(dis.readUTF());
				group.GROUP_name = MsnUtil.getUnvalidFormRs(dis.readUTF());
				group.GROUP_flag = Byte.parseByte(dis.readUTF());
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			newGroup = null;
			newGroup=newGroup(group.GROUP_id,group.GROUP_gbcode,group.GROUP_name,group.GROUP_flag,false);
			addGroupToList(newGroup,app.groupData);
			addGroupToList(newGroup(group.GROUP_id,group.GROUP_gbcode,group.GROUP_name,group.GROUP_flag,false),app.groupDataNormalOnlyOnline);			
			
			//读取组中联系人的数量
			try
			{
				contactCount = dis.readInt();
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			//组中联系人的数量为零，则读取下一组的信息
			if(0 == contactCount)
				continue;			
			
			for(int j=0; j<contactCount; j++)
			{
				contact = null;
				contact = new Contact();
				//获得联系人信息
				try
				{					
					contact.SUBITEM_JID = MsnUtil.getUnvalidFormRs(dis.readUTF());
					contact.CONTACT_imid = MsnUtil.getUnvalidFormRs(dis.readUTF());
					contact.CONTACT_nickname = MsnUtil.getUnvalidFormRs(dis.readUTF());
					if (contact.CONTACT_nickname == null|| contact.CONTACT_nickname.length() == 0)
						contact.CONTACT_nickname = contact.CONTACT_imid;
					contact.CONTACT_gbcode = MsnUtil.getUnvalidFormRs(dis.readUTF());
					contact.CONTACT_groupId = MsnUtil.getUnvalidFormRs(dis.readUTF());			
					contact.CONTACT_portraithash  = MsnUtil.getUnvalidFormRs(dis.readUTF());
					contact.CONTACT_impresa = MsnUtil.getUnvalidFormRs(dis.readUTF());
					contact.CONTACT_gleam = MsnUtil.getUnvalidFormRs(dis.readUTF());
					contact.CONTACT_url = MsnUtil.getUnvalidFormRs(dis.readUTF());
					
					byte[] imageData = null;
					newContact = null;
					try {
						imageData = getContactHeadFromRMS(contact.CONTACT_imid);
					}catch(Exception ee)
					{
						ee.printStackTrace();
					}

					newContact=newContact(contact.SUBITEM_STATE,contact.SUBITEM_JID, contact.CONTACT_imid , contact.CONTACT_nickname, 
							contact.CONTACT_gbcode, contact.CONTACT_groupId,
							contact.CONTACT_contactFlag, contact.CONTACT_portraithash, contact.CONTACT_impresa,
							null,null, contact.CONTACT_gleam, contact.CONTACT_url,imageData);
					addContact(newContact,ROSTER_NORMAL_HAS_OFFLINE,app.groupData);
					///////////////////
					//#ifdef support_exchange_contact_group
					
					//////////////////增加按状态排序功能
					if(MSNApplication.ROSTER_NORMAL_NO_OFFLINE == getGroupOrder())
					{
						addContact(newContact,getGroupOrder(), groupDataNormalOnlyOnline);
					}									
				}
				catch (IOException e)
				{
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}			
		}		
	}
	
	
	/**
	 * 从文件中读取指定用户(自己)的联系人数据并初始化
	 *@param LiveID 
	 */
	public void initContactsFromFile(String LiveID)
	{
		File rosterDataFile = new File(RMS_CONTACT_DATA + LiveID +"/rosterData");
		FileInputStream fis = null;
		try
		{
			fis = new FileInputStream(rosterDataFile);
		} 
		catch (FileNotFoundException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
			//没有存则返回
			return;
		}
		if(fis == null)
		{
			return;
		}
		//获取数据输出流
		DataInputStream dis = new DataInputStream(fis);
		
		int GroupCount = 0;
		//读取分组的数量
		try
		{
			GroupCount = dis.readInt();
		}
		catch (Exception e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
			//没有数据就返回
			return;
		}
		//分组的数量为零则返回
		if(0 == GroupCount)
		{
			return;
		}		
		
		int contactCount = 0;
		Contact contact;
		Contact newContact;
		for(int i=0; i<GroupCount; i++)
		{
			//忽略组的信息（因为只取联系人的信息）
			try
			{
				dis.readUTF();
				dis.readUTF();
				dis.readUTF();
				dis.readUTF();				
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			
			//读取组中联系人的数量
			try
			{
				contactCount = dis.readInt();
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			//组中联系人的数量为零，则读取下一组的信息
			if(0 == contactCount)
				continue;			
			
			for(int j=0; j<contactCount; j++)
			{
				contact = new Contact();
				//获得联系人信息
				try
				{					
					contact.SUBITEM_JID = MsnUtil.getUnvalidFormRs(dis.readUTF());
					contact.CONTACT_imid = MsnUtil.getUnvalidFormRs(dis.readUTF());
					contact.CONTACT_nickname = MsnUtil.getUnvalidFormRs(dis.readUTF());
					if (contact.CONTACT_nickname == null|| contact.CONTACT_nickname.length() == 0)
						contact.CONTACT_nickname = contact.CONTACT_imid;
					contact.CONTACT_gbcode = MsnUtil.getUnvalidFormRs(dis.readUTF());
					contact.CONTACT_groupId = MsnUtil.getUnvalidFormRs(dis.readUTF());			
					contact.CONTACT_portraithash  = MsnUtil.getUnvalidFormRs(dis.readUTF());
					contact.CONTACT_impresa = MsnUtil.getUnvalidFormRs(dis.readUTF());
					contact.CONTACT_gleam = MsnUtil.getUnvalidFormRs(dis.readUTF());
					contact.CONTACT_url = MsnUtil.getUnvalidFormRs(dis.readUTF());
					
					newContact = null;
					byte[] imageData = null;
					try {
						imageData = getContactHeadFromRMS(contact.CONTACT_imid);
					}catch(Exception ee)
					{
						ee.printStackTrace();
					}
					newContact = newContact(contact.SUBITEM_STATE,contact.SUBITEM_JID, contact.CONTACT_imid , contact.CONTACT_nickname, 
							contact.CONTACT_gbcode, contact.CONTACT_groupId,
							contact.CONTACT_contactFlag, contact.CONTACT_portraithash, contact.CONTACT_impresa,
							null,null, contact.CONTACT_gleam, contact.CONTACT_url,imageData);
					addContact(newContact,ROSTER_NORMAL_HAS_OFFLINE,app.groupData);
					///////////////////
					//#ifdef support_exchange_contact_group
					
					//////////////////增加按状态排序功能
					if(MSNApplication.ROSTER_NORMAL_NO_OFFLINE == getGroupOrder())
					{
						addContact(newContact,getGroupOrder(), groupDataNormalOnlyOnline);
					}									
				}
				catch (IOException e)
				{
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}			
		}	
	}
	
	/**
	 * 从文件中读取指定用户(自己)的分组数据并初始化
	 * @param LiveID
	 */
	public void initGroupsFromFile(String LiveID)
	{
		File rosterDataFile = new File(RMS_CONTACT_DATA + LiveID +"/rosterData");
		FileInputStream fis = null;
		try
		{
			fis = new FileInputStream(rosterDataFile);
		} 
		catch (FileNotFoundException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
			//没有存则返回
			return;
		}
		
		//没有存则返回
		if(fis == null)
		{
			return;
		}
		//获取数据输出流
		DataInputStream dis = new DataInputStream(fis);
		
		int GroupCount = 0;
		//读取分组的数量
		try
		{
			GroupCount = dis.readInt();
		}
		catch (Exception e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
			//没有数据就返回
			return;
		}
		//分组的数量为零则返回
		if(0 == GroupCount)
		{
			return;
		}
		
		Group group;
		Group newGroup;
		int contactCount = 0;
		for(int i=0; i<GroupCount; i++)
		{
			group = null;
			group=new Group();
			//获得分组的信息
			try
			{			
				group.GROUP_id = MsnUtil.getUnvalidFormRs(dis.readUTF());
				group.GROUP_gbcode = MsnUtil.getUnvalidFormRs(dis.readUTF());
				group.GROUP_name = MsnUtil.getUnvalidFormRs(dis.readUTF());
				group.GROUP_flag = Byte.parseByte(dis.readUTF());
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			newGroup = null;
			newGroup=newGroup(group.GROUP_id,group.GROUP_gbcode,group.GROUP_name,group.GROUP_flag,false);
			addGroupToList(newGroup,app.groupData);
			addGroupToList(newGroup(group.GROUP_id,group.GROUP_gbcode,group.GROUP_name,group.GROUP_flag,false),app.groupDataNormalOnlyOnline);			
			
			//读取组中联系人的数量
			try
			{
				contactCount = dis.readInt();
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			//组中联系人的数量为零，则读取下一组的信息
			if(0 == contactCount)
				continue;			
			
			for(int j=0; j<contactCount; j++)
			{
				//忽略联系人信息（因为只取组的信息）
				try
				{					
					dis.readUTF();
					dis.readUTF();
					dis.readUTF();
					dis.readUTF();
					dis.readUTF();					
					dis.readUTF();
					dis.readUTF();
					dis.readUTF();
					dis.readUTF();					
				}
				catch (IOException e)
				{
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}			
		}		
	}
	
	/**
	 * 得到绝对url路径,并根据setBaseUrl设置baseUrl
	 * @param setBaseUrl 是否设置baseUrl
	 */
	public String getAbsoluteUrl(String url,boolean setBaseUrl)
	{	
		if(url.startsWith("mailto:")||url.startsWith("addbuddy:")
				||url.startsWith("sms:")||url.startsWith("tel:"))
		{
			//不用再拼装成绝对路径
		}
		if (url.startsWith(STR_http))
		{
			String temp = url.substring(STR_http_length);
			if(setBaseUrl)
			{
				if (temp.indexOf("/") > 0)
				{
					temp = temp.substring(0, temp.lastIndexOf('/') + 1);
					app.baseUrl = STR_http + temp;
				} 
				else
				{
					app.baseUrl = url + "/";
				}
			}
			
		}
		else if (url.startsWith(STR_httpS))
		{
			String temp = url.substring(STR_https_length);
			if(setBaseUrl)
			{
				if (temp.indexOf("/") > 0)
				{
					temp = temp.substring(0, temp.lastIndexOf('/') + 1);
					app.baseUrl = STR_httpS + temp;
				}
				else
				{
					app.baseUrl = url + "/";
				}
			}
			
		}
		//如果不是全路径，则根据规则获得绝对url路径
		else if (!url.startsWith(STR_http)&&!url.startsWith(STR_httpS))
		{
			if (url.startsWith("./"))
			{
				url = app.baseUrl + url.substring(2);// 抛掉 ./
				if(setBaseUrl)
				{
					app.baseUrl = url.substring(0, url.lastIndexOf('/') + 1);// 保留一个/
				}					
			} 
			else if (url.startsWith("/")) // 根路径
			{
				// 根路径
				boolean startWithHttps = false;// 是否是以https://开头的
				startWithHttps = (app.baseUrl).startsWith(STR_httpS);
				String domainUrl;
				if (startWithHttps)
				{
					domainUrl = (app.baseUrl).substring(STR_https_length);
					domainUrl = domainUrl.substring(0, domainUrl.indexOf('/'));
					url = STR_httpS+ domainUrl + url;
				}
				else
				{
					domainUrl = (app.baseUrl).substring(STR_http_length);
					domainUrl = domainUrl.substring(0, domainUrl.indexOf('/'));
					url = STR_http + domainUrl + url;
				}
				if(setBaseUrl)
				{
					app.baseUrl = url.substring(0, url.lastIndexOf('/') + 1);// 保留一个/
				}					
			} 
			else if (url.startsWith("../"))// 上一级
			{
				String upUrl = app.baseUrl;
				while (url.startsWith("../"))// 可能有多个../
				{
					upUrl = upUrl.substring(0, upUrl.lastIndexOf('/'));
					upUrl = upUrl.substring(0,upUrl.lastIndexOf('/') + 1); // 保留一个/
					url = url.substring(3);
				}
				url = upUrl + url;	
				if(setBaseUrl)
				{
					app.baseUrl = url.substring(0, url.lastIndexOf('/') + 1);
				}					
			} 
			else
			// 什么都没有 等同于./的处理
			{
				url = app.baseUrl + url;
				if(setBaseUrl)
				{
					app.baseUrl = url.substring(0, url.lastIndexOf('/') + 1);// 保留一个/
				}					
			}
		}
		return url;
	}
	
	
	/**
	 * 根据baseUrl得到绝对路径
	 * @param baseUrl 
	 */
	public String getAbsoluteUrl(String url,String baseUrl)
	{	
		if(url.startsWith("mailto:")||url.startsWith("addbuddy:")
				||url.startsWith("sms:")||url.startsWith("tel:"))
		{
			//不用再拼装成绝对路径
			return url;
		}
		
		//如果不是全路径，则根据规则获得绝对url路径
		else if (!url.startsWith(STR_http)&&!url.startsWith(STR_httpS))
		{
			if (url.startsWith("./"))
			{
				url = baseUrl + url.substring(2);// 抛掉 ./
			} 
			else if (url.startsWith("/")) // 根路径
			{
				// 根路径
				boolean startWithHttps = false;// 是否是以https://开头的
				startWithHttps = (baseUrl).startsWith(STR_httpS);
				String domainUrl;
				if (startWithHttps)
				{
					domainUrl = (baseUrl).substring(STR_https_length);
					domainUrl = domainUrl.substring(0, domainUrl.indexOf('/'));
					url = STR_httpS+ domainUrl + url;
				}
				else
				{
					domainUrl = (baseUrl).substring(STR_http_length);
					domainUrl = domainUrl.substring(0, domainUrl.indexOf('/'));
					url = STR_http + domainUrl + url;
				}
			} 
			else if (url.startsWith("../"))// 上一级
			{
				String upUrl = baseUrl;
				while (url.startsWith("../"))// 可能有多个../
				{
					upUrl = upUrl.substring(0, upUrl.lastIndexOf('/'));
					upUrl = upUrl.substring(0,upUrl.lastIndexOf('/') + 1); // 保留一个/
					url = url.substring(3);
				}
				url = upUrl + url;
			} 
			else
			// 什么都没有 等同于./的处理
			{
				url = baseUrl + url;
			}
		}
		return url;
	}
	
	/**
	 * 根据Url得到baseUrl
	 * @param url  
	 */
	public String getBaseUrl(String url)
	{
		if (url.startsWith(STR_http))
		{
			String temp = url.substring(STR_http_length);			
			if (temp.indexOf("/") > 0)
			{
				temp = temp.substring(0, temp.lastIndexOf('/') + 1);
				return (STR_http + temp);
			} 
			else
			{
				return (url + "/");
			}			
		}
		else if (url.startsWith(STR_httpS))
		{
			String temp = url.substring(STR_https_length);			
			if (temp.indexOf("/") > 0)
			{
				temp = temp.substring(0, temp.lastIndexOf('/') + 1);
				return (STR_httpS + temp);
			}
			else
			{
				return (url + "/");
			}
		}		
		return null;
	}

	/**
	 * 用于广告替换USERID.
	 * @return
	 */
 	public String getNewStringReplaceUSERID(String url)
	{
		if(url==null)
			return null;
		int index=url.indexOf("%USERID%");
		if(index!=-1)
		{
			url=url.substring(0,index)+this.GetLiveID()+url.substring(index+"%USERID%".length());
		}
		return url;
	}
    

    
    public void getConnectType(Context context)
    {
    	ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
    	NetworkInfo info = cm.getActiveNetworkInfo();
//    	System.out.println(info);
    	if (info != null && info.getType() != ConnectivityManager.TYPE_WIFI)
    	{
    		String userName = info.getExtraInfo();
			if (userName != null && userName.toLowerCase().contains("cmwap"))
			{
				Jabber.isUseSocket = false;
			}
    	}
    }

    /**
     * 根据IMSI确定版本号，具体规则参见协议文档
     * @param IMSI
     */
    public static void getVersion(String IMSI, MSNApplication app)
    {
        Resources r = mActivity.getResources();
        Locale local = r.getConfiguration().locale;
        String lang = local.getLanguage();

        if(lang.equals("en") || lang.equals("en_CA")
                || lang.equals("en_GB") || lang.equals("en_US"))
        {
        	app.VERSION = app.VERSION.substring(0, app.VERSION.lastIndexOf('.')) + ".en";
        }
    }
	
	/**注册 识别串*/
	public void setRegisterIdentifier(String identifier)
	{
		SharedPreferences pres = mActivity.getSharedPreferences("registerDate", Context.MODE_WORLD_READABLE + Context.MODE_WORLD_WRITEABLE);
		Editor editor = pres.edit();
		editor.putString("identifier", identifier);
		editor.commit();
	}
	
	public String getRegisterIdentifier()
	{
		SharedPreferences pres = mActivity.getSharedPreferences("registerDate", Context.MODE_WORLD_READABLE + Context.MODE_WORLD_WRITEABLE);
		return pres.getString("identifier", "");
	}
}