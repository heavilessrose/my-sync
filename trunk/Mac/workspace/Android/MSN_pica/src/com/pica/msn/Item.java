package com.pica.msn;

import java.util.Vector;

import android.graphics.Bitmap;

import com.pica.ui.tools.RosterItem;

public class Item
{

	//public RosterItem ri;
	
	//public byte TreeList_TYPE; 
	//**************
	//一级列表中元素的属性 
	//************
	
	/**会话是sid(其中聊天室会话是该聊天室的jid) <br>
	 * 验证请求是imid <br>
	 * 聊天室好友是jid*/
	public String TOPITEM_JID;//jid  会话是sid(其中聊天室会话是该聊天室的jid) 验证请求是imid 聊天室好友是jid
	/**标识一级列表元素类型：会话,聊天室好友,查找好友返回*/
	//public byte TOPITEM_TYPE;//标识一级列表元素类型：会话,聊天室好友,查找好友返回
	
	//**************
	//二级列表中元素的属性 
	//************
	/**标识二级列表元素类型：聊天室,联系人,频道*/
	//public byte SUBITEM_TYPE;//标识二级列表元素类型：聊天室,联系人,频道
	
	/**联系人|频道|聊天室 的jid */
	//public String SUBITEM_JID;//jid 联系人 频道 聊天室 都可以用
	
//	/**联系人的在线状态 列表项排序所依据的权重 频道和聊天室都默认为脱机*/
//	public byte SUBITEM_STATE=MSNApplication.STATE_OFFLINE; //在线状态 列表项排序所依据的权重 频道和聊天室不赋值 所以默认都是为0
//
//
//	
//	//*****************************
//	//分组
//    //*****************************
//	/**联系人分组的组ID*/
//    public String GROUP_id;//  
//    /**组gb码*/
//    public String GROUP_gbcode;// 
//    /**组名*/
//    public String GROUP_name;//  
//    /**组类型 用户自定义分组 聊天室分组 频道分组 广告*/
//    public byte GROUP_flag;//  
//    /**组的展开状态*/
//    public boolean  GROUP_isexpand;
//    /**组内成员的数量*/
//    public short GROUP_itemnum;// 
//    /**组内在线状态的人数*/
//    public String GROUP_maxweight_itemnum;
//    
//	//*****************************
//	//联系人
//    //*****************************
//    /**联系人的邮箱*/
//    public String CONTACT_imid;//邮箱
//    /**联系人的昵称*/
//    public String CONTACT_nickname;// 昵称
//    /**联系人的本地显示名称前4个字符的gb2312码*/
//    public String CONTACT_gbcode;//   本地显示名称前4个字符的gb2312码
//    /**联系人的分组id*/
//    public String CONTACT_groupId;//分组id
//    /**联系人的联系人标志位（正常、陌生人、黑名单）*/
//    public byte CONTACT_contactFlag;// 联系人标志位（正常、陌生人、黑名单）
//    /**联系人的心情短语*/
//    public String CONTACT_impresa;// 心情短语
//    /**联系人的电话号码*/
//    public String CONTACT_phone;//电话号码 如果非空 则增加“拨打电话”
//    /**联系人的当前状态的文字说明 如busy|online等*/
//    public String CONTACT_statusShow;// 当前状态的文字说明 busy online等 不用保存到rms	
//    /**联系人的个人空间更新 0=没有更新 1=有更新*/
//    public String CONTACT_gleam;//0 没有更新 1 有更新
//    /**联系人的空间地址*/
//    public String CONTACT_url;// 空间地址
//    
//    //public String CONTACT_mobile;//   0非移动用户|1移动用户
//    
//    //#ifdef support_head_image
//    public String CONTACT_portraithash;// 联系人的头像标识，根据该值客户端可以判断用户的头像是否发生变化
//	public byte[] CONTACT_imageData;// 头像数据
//	public boolean CONTACT_hasRMSImageData=false;//RMS里面是否保存有头像数据
//    /**当前已发送过获取头像包*/
//	public boolean CONTACT_hasSendGetImage=false;
//	//#endif
	
	//*****************************
	// 消息数据结构
    //******************************
	/**消息类型*/
	public byte MESSAGE_type;//  消息类型
	/**消息所属的会话ID*/
	public String MESSAGE_sid;//  消息所属的会话ID
	/**消息 消息发出人的jid(若是发出的消息则为接收人jid),陌生人消息没有此项 */
	public String MESSAGE_jid;//  陌生人没有此项 消息发出人的jid(若是发出的消息则为接收人jid)
	/**消息 陌生人消息的陌生人imid 登录账号*/
	public String MESSAGE_imid;// 陌生人消息的陌生人imid
	/**消息 消息发出人昵称(若是发出的消息则为接收人昵称)*/
	public String MESSAGE_nickname;//  消息发出人昵称(若是发出的消息则为接收人昵称)
	/**消息内容*/
	public String MESSAGE_body;//   消息内容
	/**消息状态 是否是发送消息*/
	public boolean MESSAGE_IS_SEND;//  
	/**消息附件类型*/
	public String MESSAGE_mime;//  附件类型


	/**消息时间戳*/
	public String MESSAGE_stamp;//  时间戳
	/**消息 震动消息 true=震动 false=不震动*/
	public boolean MESSAGE_nudge=false;//true则震动
	/**消息失败的原因*/
	public String MESSAGE_reason;//发送失败的原因
	/**消息 status=’fail’ */
	public String MESSAGE_status;//status=’fail’ 
	/**消息 群消息 group或者chat 悄悄话 msn好像没用*/
	public String MESSAGE_CLUSTER_TYPE;//group或者chat 群消息 悄悄话 msn好像没用
	/**消息附件文件名*/
	public String MESSAGE_FILE_VOICE_NAME;
	/**消息附件大小*/
	public String MESSAGE_FILE_SIZE;
	/**消息附件的 服务器用来标识传输的文件 */
	public String MESSAGE_FILE_transferID;
	/**消息附件的 ftid=’some-id’*/
	public String MESSAGE_FILE_ftID;
	/**消息附件的 数据base64格式*/
	public byte[] MESSAGE_FILE_DATA;
	
	/**重传次数*/
	public String MESSAGE_FILE_VOICE_TRANSFER_COUNT;
	
	/**带内发送时的序号*/
	public String MESSAGE_FILE_SEQID;
	
	public String MESSAGE_FILE_START;
	
	public String MESSAGE_FILE_END;
	
	/**消息附件的状态 
	 * <br>发送：等待发送等待发送中 被对方拒绝<br>，正在发送<br>发送完成<br>发送中取消(主动或被动)<br>发送中异常<br>
	 * <br>接收：等待接收<br>等待中自己拒绝<br>正在接收<br>接收完毕<br>接收中取消<br>接收中异常*/
	public byte MESSAGE_FILE_STATUS_TYPE;//
	
	/**消息附件下载地址*/
	public String MESSAGE_FILE_URL;//  文件下载地址
	
	/**消息语音的状态 <br>
	 * 录音中
	 * <br>录音完毕 带内发送完毕
	 * <br>带外发送完毕
	 * <br>接收到一个语音剪辑 带内<br>接收完毕 带外
	 * <br>录音完毕 带外正在发送<br>带外 正在接收<br>带外 发送取消<br>带外 接收取消。
	 * */
	public byte MESSAGE_VOICE_STATUS_TYPE;//
	
	/**声音附件的 数据 base64格式*/
	public byte[] MESSAGE_VOICE_DATA;
	
	/**声音附件的mime */
	public String MESSAGE_VOICE_MIME;
	
	/**声音附件的大小 */
	public String MESSAGE_VOICE_SIZE;
	
	/**语音剪辑下载地址*/
	public String MESSAGE_VOICE_URL;//  
	
	/**已传输的百分比 0-100*/
	public String MESSAGE_GAUGE_INDEX;
	
	/**语音剪辑在本地保存的文件路径*/
	public String MESSAGE_VOICE_PATH;
	
	/**和该文件/语音剪辑 相关的带外传输对象*/
	public FileOuterTransfer MESSAGE_FILEOUTTRANSFER;
	
	//*****************************
	// 会话数据结构
    //******************************
	 /**会话类型  SESSION_TYPE_CHAT  SESSION_TYPE_GROUP_CHAT  SESSION_TYPE_CLUSTER_CHAT*/
	 public byte SESSION_sessionType;// 会话类型  SESSION_TYPE_CHAT  SESSION_TYPE_GROUP_CHAT  SESSION_TYPE_CLUSTER_CHAT
	 /**会话开始时间*/
	 public String SESSION_time;//  会话开始时间
	 /**会话成员容器 装载联系人*/
	 public Vector SESSION_chaters;// 会话成员
	 /**会话 未读新消息数量*/
	 public String SESSION_numOfNewMsg;// 未读新消息数量
	 /**会话 装载消息容器*/
	 public Vector SESSION_contents;// 会话消息内容
	 /**会话 聊天室会话 屏蔽的用户清单*/
	 public Vector SESSION_BLOCK_LIST;//聊天室会话 屏蔽的用户清单
	 
	 //#ifdef support_save_rms
	 /**历史会话 保存的用户自设会话名*/
	 public String SESSION_Record_saveName;//历史会话 保存的用户自设会话名
	 /**历史会话 会话消息内容*/
	 public Vector SESSION_Record_contents;//会话消息内容
	 /**历史会话 会话消息的条数*/
	 public String SESSION_Record_contents_count;//会话消息的条数
	 //#endif
	 
	//*****************************
	// 验证请求的好友数据结构
	//******************************
	 /**验证请求的好友 是否有扩展信息 只有在请求方为@msn.cn用户时才携带扩展信息*/
	 public boolean VERIFY_isHasProfile;//是否有扩展信息  只有在请求方为msn.cn用户时才携带扩展信息
	 /**验证请求的好友 imid 登录账号*/
	 public String VERIFY_imid;
	 /**验证请求的好友 昵称*/
	 public String VERIFY_nickname;//昵称
	 /**验证请求的好友 真实姓名*/
	 public String VERIFY_realname;//真实姓名
	 /**验证请求的好友 性别*/
	 public String VERIFY_sex;//性别
	 /**验证请求的好友 年龄*/
	 public String VERIFY_age;//年龄
	 /**验证请求的好友 省份*/
	 public String VERIFY_province;//省份
	 /**验证请求的好友 个人描述*/
	 public String VERIFY_desc;//个人描述

	//*****************************
	// 聊天室ROOM数据结构
	//******************************
	// public String CHATROOM_name;//聊天室名称
	// public byte CHATROOM_chatroomFlag;//标志 NORMAL_FLAG=0x00,//未进入的聊天室状态  ENTER_FLAG=0x01;//已进入的聊天室状态
	// public String CHATROOM_type;//type=’fixed固定|custom定制‘ zwp：只有类wap的操作（类似于频道）
	 
	//*****************************
	// 频道数据结构 String jid, String name, String groupname, String[][] options, boolean iswap, boolean auth
	//******************************
	// public String CHANNEL_groupname;
	// public String[][] CHANNEL_options;
	// public boolean CHANNEL_iswap;
	// public boolean CHANNEL_auth;
	// public String CHANNEL_nickname;
	 
	//*****************************
	// 聊天室好友数据结构
	//****************************** 
	// public String CHATROOM_FRIEND_nickname;//昵称
	// public String CHATROOM_FRIEND_imid;
	// public boolean CHATROOM_FRIEND_Block=false;//屏蔽该聊天室好友
	//*****************************

	 
	//*****************************
	// 个人登录资料
	//****************************** 
	 //public String MYLOGIN_PICAALLID;//pica号码 形如9035@pica
	 //public byte MYLOGIN_selectEmailDomain;//选择的帐号
	 //public boolean MYLOGIN_isUsingCmccProxy;//是否使用代理服务器
	 //public boolean MYLOGIN_isSavePassword;//是否保存密码
	 //public boolean MYLOGIN_isSoundContactOnline;//是否播放联系人上线声音
	 //public boolean MYLOGIN_isSoundMessageReceive;//是否播放来新消息声音
	 //public boolean MYLOGIN_isAutoLogin;//是否自动登录

	 
	 //public String MYLOGIN_username;//编辑框输入的用户名
	 //public String MYLOGIN_allUsername;//带域名的全称用户名
	 //public String MYLOGIN_password;//密码
	 
	
	 
	 ///////////// android暂不考虑
	 
	 //#ifdef support_head_image
	 //public boolean MYLOGIN_isReceiveContactHeadImage;//是否接收头像
	 //#endif
	 
	 //#ifdef support_exchange_contact_group
	// public boolean MYLOGIN_isOnlyShowOnlineContact=true;//是否仅仅显示在线联系人
	 //#endif

	 //public String MYLOGIN_chat_header_AD_ID;//广告id
	 //public String MYLOGIN_login_prog_AD_ID;//广告id
	 //public String MYLOGIN_roster_footer_AD_ID;//广告id
	 //public String MYLOGIN_main_footer_AD_ID;//广告id
	 //public String MYLOGIN_session_footer_AD_ID;//广告id
	 
	 
	 //*********************
	 //页脚广告 发包时候使用
	 //***********************
	public byte AD_type;//联系人页脚、主页面页脚、会话列表页脚 
	public byte AD_flag; //AD_IVR_FLAG = 0x00;//IVR广告 AD_WAP_FLAG = 0x01;//wap广告  AD_ZWP_FLAG = 0x02;//类wap广告  AD_SMS_FLAG = 0x03;//短信广告
	public String AD_id;
	public String AD_target;
	public String AD_param0;
	public String AD_src;
	public String AD_text;
	public String AD_dbid;
	public String AD_sale;
	public Bitmap AD_image;
	

}
