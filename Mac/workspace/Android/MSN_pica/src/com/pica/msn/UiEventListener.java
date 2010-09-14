package com.pica.msn;

public class UiEventListener {

	public static final int UI_EVENT_BASE =  0x100;
	
	public static final int UI_EVENT_SET_STATUS =  UI_EVENT_BASE + 1;
	public static final int UI_EVENT_SET_VCARD  =  UI_EVENT_BASE + 2;
	public static final int UI_EVENT_DELETE_ROSTER  =  UI_EVENT_BASE + 3;
	public static final int UI_EVENT_BLOCK  =  UI_EVENT_BASE + 4;
	public static final int UI_EVENT_UNBLOCK  =  UI_EVENT_BASE + 5;
	public static final int UI_EVENT_ADD_ROSTER  =  UI_EVENT_BASE + 6;
	public static final int UI_EVENT_SEND_ROSTER  =  UI_EVENT_BASE + 7;	
	public static final int UI_EVENT_EMOTION  =  UI_EVENT_BASE + 8;
	public static final int UI_EVENT_SHORTMESSAGE  =  UI_EVENT_BASE + 9;
	public static final int UI_EVENT_SET_SYSTEM  =  UI_EVENT_BASE + 10;
	public static final int UI_EVENT_DETAIL_INFO  =  UI_EVENT_BASE + 11;
	public static final int UI_EVENT_USER_LIST  =  UI_EVENT_BASE + 12;
	/**输入的密码为空*/
	public static final int UI_EVENT_PASSWORD_NULL_ERROR  =  UI_EVENT_BASE + 13;
	public static final int UI_EVENT_SAVE_PASSWROD  =  UI_EVENT_BASE + 14;
	public static final int UI_EVENT_AUTO_LOGIN  =  UI_EVENT_BASE + 15;
	/**隐私声明*/
	public static final int UI_EVENT_FIRST_TIME  =  UI_EVENT_BASE + 16;
	public static final int UI_EVENT_ERROR_USERNAME  =  UI_EVENT_BASE + 17;
	public static final int UI_EVENT_ERROR_PASSWORD  =  UI_EVENT_BASE + 18;
	public static final int UI_EVENT_ADD_CONFRIM  =  UI_EVENT_BASE + 19;
	public static final int UI_EVENT_ERROR_MESSAGE  =  UI_EVENT_BASE + 20;
	public static final int UI_EVENT_ABOUT  =  UI_EVENT_BASE + 21;
	public static final int UI_EVENT_HELP  =  UI_EVENT_BASE + 22;
	public static final int UI_EVENT_UPDATE  =  UI_EVENT_BASE + 23;
	/**注销*/
	public static final int UI_EVENT_LOGOUT  =  UI_EVENT_BASE + 24;
	public static final int UI_EVENT_DELETE_ONE_SESSION=UI_EVENT_BASE+25;
	/**退出*/
	public static final int UI_EVENT_EXIT  =  UI_EVENT_BASE + 26;
	/**邀请*/
	public static final int UI_EVENT_INVITE = UI_EVENT_BASE + 27;
	/**查看参与者*/
	public static final int UI_EVENT_CHECK_INVITE = UI_EVENT_BASE + 28;
	/**输入的帐户为空*/

	public static final int UI_EVENT_USERNAME_NULL_ERROR  =  UI_EVENT_BASE + 29;
	

	public static final int UI_EVENT_SESSION_HELP= UI_EVENT_BASE + 30;	
	/**保存聊天记录*/
	public static final int UI_EVENT_SAVE_HISTORY= UI_EVENT_BASE + 31;	
	/**聊天记录*/
	public static final int UI_EVENT_CHECK_HISTORY= UI_EVENT_BASE + 32;
	/**流量设置*/
	public static final int UI_EVENT_FLOW_SETTING=UI_EVENT_BASE+33;
	/**带外语音剪辑 发送 接收*/
	public static final int UI_EVENT_OUTER_VOICECLIP=UI_EVENT_BASE+34;
	/**带外头像 上传 */
	public static final int UI_EVENT_OUTER_VCARDHEAD=UI_EVENT_BASE+35;
	/**Zwp 界面加载*/
	public static final int UI_EVENT_ZWAP_LOAD=UI_EVENT_BASE+36;
	/**掉线通知*/
	public static final int UI_EVENT_LOGIN_LOSE=UI_EVENT_BASE+37;
	/**是否删除选中的聊天记录*/
	public static final int UI_EVENT_DELETE_HISTORY = UI_EVENT_BASE+38;
	/**登录的时候输入的密码少于6位*/
	public static final int UI_EVENT_LOGIN_PASSWORD_ERROR = UI_EVENT_BASE+39;
	/**登录的时候输入完带后缀名的邮件地址后有选择后缀*/
	public static final int UI_EVENT_LOGIN_EMAIL_ERROR = UI_EVENT_BASE+40;
	/**账号在其他地方登陆的提示*/
	public static final int UI_EVENT_LOGOUT_AVALIABLE = UI_EVENT_BASE+41;
	
	
	/** 输入的注册用户名应该在3位以上 */
	public static final int UI_EVENT_REGISTER_USERNAME_ERROR = UI_EVENT_BASE + 42;
	/** 输入的注册密码应该在6位以上 */
	public static final int UI_EVENT_REGISTER_PASSWORD_ERROR = UI_EVENT_BASE + 43;
	/** 输入的注册用户名必须以字母开头 */
	public static final int UI_EVENT_REGISTER_USERNAME_HEAD_ERROR = UI_EVENT_BASE + 44;
	/** 注册失败，请稍后再试*/
	public static final int EVENT_REGISTER_FAILURE = UI_EVENT_BASE + 45;
	/** 注册成功 */
	public static final int EVENT_REGISTER_SUCCESS = UI_EVENT_BASE + 46;
	/** 注册失败 */
	public static final int EVENT_REGISTER_ERROR = UI_EVENT_BASE + 47;
}
