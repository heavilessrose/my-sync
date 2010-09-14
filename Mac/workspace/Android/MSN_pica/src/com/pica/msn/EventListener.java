package com.pica.msn;
public interface EventListener
{
	/**
	 * 注册失败<br>
	 * 参数类型：String 错误提示文本 （可能为空）<br>
	 */
	public static final byte EVENT_ERGISTER_FAILURE = 0;
	
	/**
	 * 注册成功 给出注册成功相关信息页面<br>
	 * 参数类型：String 提示文本<br>
	 */
	public static final byte EVENT_REGISTER_SUCCESS=1;
	
	/**
	 * 提示文本“网络连接中断,请稍后再试”回到登录页面
	 * 
	 */
	public static final byte EVENT_NETWORK_FAILURE=2;
	
	/**
	 * 显示dispatch返回的Notice<br>
	 * 所有的notice都以String形式 保存在noticeVector
	 */
	public static final byte EVENT_DISPATCH_NOTICE=3;
	
	/**
	 * 显示dispatch返回的url 提示升级<br>
	 * 保存在jabber.updateUrl和jabber.updateVersion
	 */
	public static final byte EVENT_DISPATCH_URL=4;
	
	
	
	/**
	 * 登录完成 显示联系人页面
	 */
	public static final byte EVENT_LOGIN_SUCCESS=5;
	
	
	
	/**
	 * <br>
	 * 事件含义：读取Zipwap页面成功<br>
	 * 参数类型：Object[]{DataInputStream,Integer}<br>
	 * 参数含义：页面内容,长度<br>
	 */
	public static final byte EVENT_ZIPWAP_OK = 6;

	/**
	 * <br>
	 * 事件含义：读取Zipwap页面失败<br>
	 * 参数类型：Object<br>
	 * 参数含义：无(null)<br>
	 */
	public static final byte EVENT_ZIPWAP_ERROR = 7;
	
	/**
	 * 发送点击广告统计包<br>
	 * 参数类型：String advist<br>
	 * */
	public static final byte EVENT_SEND_ADVIST=8;
	
	
	/**
	 * 注销成功 退回到登录页面<br>
	 * 如果是同步联系人操作 则继续登录
	 * 如果是普通注销操作 则提示注销成功
	 * 参数类型：NULL<br>
	 * */
	public static final byte EVENT_RECEIVE_LOGOUT=9;
	
	/**
	 * 退出成功 退出程序<br>
	 * 参数类型：NULL<br>
	 * */
	public static final byte EVENT_RECEIVE_EXIT=10;
	
	/**
	 * 登录中(也可能是登陆成功后)收到错误包 进行相应的处理并提示错误并退回到登录页面（或再自动登录）<br>
	 * 参数类型：{String errorMsg,String errorCode }错误提示和错误代码<br>
	 * */
	public static final byte EVENT_RECEIVE_LOGIN_ERROR=11;
	
	/**
	 * 收到错误包 提示错误<br>
	 * 参数类型：String errorMsg错误提示<br>
	 * */
	public static final byte EVENT_RECEIVE_OTHER_ERROR=12;
	
	
	/**
	 * 刷新联系人页面
	 */
	public static final byte EVENT_UPDATE_CONTACT_UI=13;
	
	/**
	 * 收到presence type=unavailable包 如果没有主动发起退出、注销请求，则表示很长时间没操作 微软认为掉线 <br>
	 * 参数类型：null<br>
	 * 界面跳到登录界面 并且退出其他所有的activity<br>
	 * */
	public static final byte EVENT_RECEIVE_MSN_LOGOUT=14;
	
	/**
	 * 收到presence type=error <error>包 from=selectChatroomJid 聊天室里面报错<br>
	 * 参数类型：String errorMsg错误提示<br>
	 * */
	public static final byte EVENT_RECEIVE_CHATROOM_ERROR=15;
	
	/**
	 * 收到presence 个人的在线状态发生改变包 退出等待页面<br>
	 * 参数类型：null<br>
	 * */
	public static final byte EVENT_CHANGE_MY_STATE=16;
	
	/**
	 * 进入一个正在进入 且进入成功的聊天室 退出进入等待页面 呈现会话页面
	 */
	public static final byte EVENT_ENTER_ROOM_OK=17;
	
	
	/**
	 * 设置 判断是否是msn.cn新用户
	 */
	public static final byte EVENT_SET_NEWMSNCN=18;
	
	/**
	 * 收到好友请求的验证请求，立即显示出来（暂未考虑当前是等待页面）<br>
	 * 所有的验证好友 都装载在app.verifyFriendVector向量里面
	 */
	public static final byte EVENT_RECEIVE_VERIFY_FRIEND=19;
	
	/**
	 * 好友请求的验证请求全部显示完 要退出验证请求界面
	 */
	public static final byte EVENT_OVER_VERIFY_FRIEND=20;
	
	/**
	 * 还有好友请求的验证没有处理完 要显示出来
	 */
	public static final byte EVENT_MORE_VERIFY_FRIEND=21;
	
	
	/**
	 * 收到加入聊天室包 判断页面情况 如果客户端收到新的聊天室列表后，更新列表显示。如果
	当前的视图为类WAP视图，弹出提示信息“热点话题列表已更新，是否现在查看？”。
	如果用户点击“确定”，则切换视图到联系人视图，同时展开“热点话题”组。			
	 */
	public static final byte EVENT_UPDATE_JOIN_CLUSTER=22;
	
	/**
	 * 被踢出聊天室 弹出页面
	 */
	public static final byte EVENT_KICKOFF_CHATROOM = 23;
	
	/**
	 * 被聊天室加入黑名单 弹出页面
	 */
	public static final byte EVENT_BLACKLIST_CHATROOM = 24;
	

	/**
	 * 登录完成 保存该用户的索引进入数据库
	 */
	public static final byte EVENT_SAVE_LOGIN_USER=25;
	
	/**
	 * dispatch不允许登录 不用提示 直接退到登录页面即可
	 */
	public static final byte EVENT_DISPATCH_REFUSE_LOGIN=26;
	
	/**
	 * dispatch强制升级 <br>  
	 * 因为已经调用过浏览器 所以让其退到登录页面即可
	 */
	public static final byte EVENT_DISPATCH_MUST_UPGRADE=27;
	
	/**
	 * <br>
	 * 事件含义：收到自己的个人资料 <br>
	 * 退出等待页面 
	 * 参数类型：Object<br>
	 * 参数含义：无(null)<br>
	 */
	public static final byte EVENT_MY_VCARD_RECEIVED = 28;
	
	
	/**会话类型有变更 单人-多人-单人 刷新菜单*/
	public static final byte EVENT_UPDATE_SESSION_FLAG=29;
	
	/**
	 * <br>
	 * 事件含义：收到新消息<br>
	 * 参数类型：Object<br>
	 * 参数含义：无(null)<br>
	 */
	public static final byte EVENT_RECEIVED_MESSAGE  = 30;
	
	/**
	 * <br>
	 * 事件含义：发出消息<br>
	 * 参数类型：Message<br>
	 */
	public static final byte EVENT_SEND_MESSAGE = 31;
	
	/**
	 * 打开对话窗口 
	 *
	 */
	public static final byte EVENT_OPEN_CHAT_WINDOW = 32;
	

	
	//#ifdef support_head_image
	/**
	 * 收到联系人的头像 需要刷新<br>
	 * 参数类型 Contact 该联系人
	 */
	public static final byte EVENT_RECEIVE_CONTACT_PORTRAIT=33;
	//#endif
	
	/***
	 * 添加联系人成功 <br>
	 * 参数类型 null<br>
	 * 更新该联系人的数据库<br>
	 * 添加了一个新联系人 根据Imid 找到陌生人会话里面的是否有该联系人 如有则将新联系人覆盖老联系人
	 */
	public static final byte EVENT_ADD_CONTACT_OK=34;
	
	
	/***
	 * 删除联系人成功 退出等待页面 提示删除成功<br>
	 * 参数类型 null
	 */
	public static final byte EVENT_DELETE_CONTACT_OK=35;
	
	/***
	 * 删除并阻止联系人成功 退出等待页面 提示删除并阻止成功<br>
	 * 参数类型 null
	 */
	public static final byte EVENT_DELETE_BLOCK_CONTACT_OK=36;
	
	/***
	 * 阻止联系人成功 退出等待页面 提示阻止成功<br>
	 * 参数类型 String nickname 该联系人的nickname
	 */
	public static final byte EVENT_BLOCK_CONTACT_OK=37;
	
	/***
	 * 解除阻止联系人成功 退出等待页面 提示解除阻止成功<br>
	 * 参数类型 String nickname 该联系人的nickname
	 */
	public static final byte EVENT_UNBLOCK_CONTACT_OK=38;
	
	/**
	 * 三秒闪屏完毕
	 */
	public static final byte EVENT_FLASH_COMPLETE = 39;
	
	/**
	 * 用户自己离开聊天室成功 结束等待页面 页面要转到联系人页面
	 */
	public static final byte EVENT_CHATROOM_LOGOUT_OK = 40;
	
	/**
	 * 其它联系人离开聊天室 如果当前页面是聊天室好友列表页面 则要刷新
	 */
	public static final byte EVENT_OTHER_FREIEND_LOGOUT_CHATROOM = 41;

	/**
	 * 用户自己退出聊天室成功 结束等待页面 页面要转到联系人页面
	 */
	public static final byte EVENT_CHATROOM_SIGNOFF_OK=42;

	/**
	 * <br>
	 * 事件含义：收到聊天室的新消息<br>
	 * 参数类型：Object<br>
	 * 参数含义：无(null)<br>
	 */
	public static final byte EVENT_CHATROOM_MESSAGE_RECEIVED = 43;
	
	/**
	 * 收到首页页脚广告 在屏幕上绘制出来<br>
	 * 参数类型：Item 广告条<br>
	 */
	public static final byte EVENT_RECEIVE_MAIN_FOOTER_AD_ITEM=44;
	
	/**
	 * 收到联系人页脚广告 在屏幕上绘制出来<br>
	 * 参数类型：Item 广告条<br>
	 */
	public static final byte EVENT_RECEIVE_ROSTER_FOOTER_AD_ITEM=45;
	
	/**
	 * 收到会话列表页脚广告 在屏幕上绘制出来<br>
	 * 参数类型：Item 广告条<br>
	 */
	public static final byte EVENT_RECEIVE_SESSION_FOOTER_AD_ITEM=46;
	
	
	/**
	 * 其它联系人进入聊天室 如果当前页面是聊天室好友列表页面 则要刷新
	 */
	public static final byte EVENT_OTHER_FREIEND_ENTER_CHATROOM = 47;
	
	/**
	 * 登录超时 提示用户登录超时3秒钟后 退回到登录页面
	 * */
	public static final byte EVENT_WAITING_LOGIN_TIMEOUT=48;
	
	/***
	 * 其它等待操作 超时 给用户提示3秒后 退出到上级页面
	 */
	public static final byte EVENT_WAITING_OTHER_TIMEOUT=49;

	
	//#ifdef support_save_rms
	/**
	 * 收到addition==0  同步错误 需要提示用户然后注销退出
	 * */
	public static final byte EVENT_RECEIVE_SYNCHRONIZE_ERROR=50;
	//#endif
	
	/**进行判断 然后刷新会话窗口页面的菜单和顶栏<br>
	 * @param obj contact 当前改变的联系人对象
	 * */
	public static final byte EVENT_UPDATE_CHAT_UI=51;
	
	/***
	 * 提示 内存不够了
	 */
	public static final byte EVENT_OUT_OF_MEMORY=52;
	
	/**来了新消息 放新消息声音*/
	public static final byte EVENT_SOUND_NEW_MESSENGE=53;
	
	/**登录成功 开始发送心跳包 60秒发送一个空格*/
	public static final byte EVENT_SEND_HAERT=54;
	
	/**dispatch连接成功 登录等待页面由查找服务器->连接服务器*/
	public static final byte EVENT_RECEIVE_DISPATCH_OK=55;
	
	/**登录Pica成功 开始登录msn 登录等待页面由连接服务器->获取个人资料等*/
	public static final byte EVENT_RECEIVE_LOGIN_PICA_OK=56;
	
	/**来了新联系人 放联系人上线声音*/
	public static final byte EVENT_SOUND_NEW_ONLINE=57;
	
	/**来了新震动消息 放震动消息声音 并产生震动效果*/
	public static final byte EVENT_SOUND_NEW_NUDGE=58;
	
	/**新增加一个会话 添加到列表中去*/
	public static final byte EVENT_ADD_NEW_SESSION=59;
	
	/**刷新心情短语重绘*/
	public static final byte EVENT_UPDATE_INPRESEA_UI=60;
	
	/**关闭当前的activity*/
	public static final byte EVENT_FINISH_ACTIVITY=61;
	
	/**关闭当前的activity 开启login activity*/
	public static final byte EVENT_CLOSE_TO_LOGIN=62;
	
	/**显示信息 Object<br>
	 * 参数类型：String text 显示文本<br>
	 * */
	public static final byte EVENT_SHOW_INFO=63;
	
//	/**显示信息 Object[]<br>
//	 * 参数类型：String title 标题文本<br>
//	 * 参数类型：String text 显示文本<br>
//	 * 参数类型：String leftCommand 左软键文本 <br>
//	 * 参数类型：String rightCommand 左软键文本 <br>
//	 * */
//	public static final byte EVNET_SHOW_DIALOG=64;
	
	
	/**
	 * 登录中收到错误包 进行相应的处理并提示错误并退回到登录页面（或再自动登录）<br>
	 * 参数类型：{String errorMsg,String errorCode }错误提示和错误代码<br>
	 * */
	public static final byte EVENT_RECEIVE_LOGIN_ERROR_LOGINMONITOR=65;
	
	/**
	 *收到错误包 进行相应的处理并提示错误并退回到登录页面（或再自动登录）<br>
	 * 参数类型：{String errorMsg,String errorCode }错误提示和错误代码<br>
	 * */
	public static final byte EVENT_RECEIVE_LOGIN_ERROR_MAIN=66;
	
	/***
	 * 给loginmonitor 重置文本和时间
	 * 参数类型:String text
	 */
	public static final byte EVENT_LOGIN_MONITOR_SETTEXT=67;
	
	/**关闭动画页面*/
	public static final byte EVENT_STOP_MONITOR=68;
	/**刷新main activity*/
	public static final int EVENT_ROSTER  =  69;
	public static final int EVENT_SESSION  =  70;	
	
	/**
	 * 收到传送文件的请求 弹出选择页面
	 */
	public static final byte EVENT_RECEIVE_FILE_INVITE=71;
	
	
	/**
	 * <br>
	 * 事件含义：升级一个消息的相关状态<br>
	 * 参数类型：Object<br>
	 * 参数含义：无(null)<br>
	 */
	public static final byte EVENT_UPDATE_MESSAGE  = 72;
	
	/**
	 * <br>
	 * 事件含义：升级一个消息的传送指示条<br>
	 * 参数类型：Object<br>
	 * 参数含义：无(null)<br>
	 */
	public static final byte EVENT_UPDATE_GAUGE_MESSAGE  = 73;
	
	public static final int EVENT_RECORD_COMPLETE  =  74;
	
	/**刷新msn小人动画*/
	public static final byte EVENT_UPDATE_MONITOR_UI=75;


	/**发送图片文件作为头像*/
	public static final byte EVENT_SEND_VCARDHEAD_I=76;
	
	/**发送相片作为头像*/
	public static final byte EVENT_SEND_VCARDHEAD_P=77;	

	/**更新带外上传头像的滚动条的值*/
	public static final byte EVENT_UPDATE_OUTER_HEAD_GAUGE=78;
	/**更新带外上传头像的滚动条的状态 界面上去掉滚动条 结束 取消 有异常之类的*/
	public static final byte EVENT_UPDATE_OUTER_HEAD_STATUS=79;
	
	/**会话参与者有变更 刷新顶栏*/
	public static final byte EVENT_UPDATE_SESSION_TOP_NAME=80;
	
	/**对某一个联系人进行数据库操作 增加/更新*/
	//public static final byte EVENT_ADD_ONE_CONTACT_TO_DATABASE=81;
	
	/**对某一个联系人进行数据库操作 删除*/
	//public static final byte EVENT_DELETE_ONE_CONTACT_TO_DATABASE=82;

	/**每5秒钟判断一次是否需要保存联系人列表*/
	//public static final byte EVENT_SAVE_CONTACT_LIST=81;
	
	/**保存分组、联系人数据到文件中*/
	public static final byte EVENT_SAVE_CONTACT_DATA=84;
	
	/**收到联系人状态改变包*/
	public static final byte EVENT_RECEIVE_CONTACT_CHANGE_STATE=85;	
	
	/**收到联系人包*/
	public static final byte EVENT_RECEIVE_ROSTER=86;
	
	/**收到分组包*/
	public static final byte EVENT_RECEIVE_GROUP=87;
	
	/**收到分组结束包*/
	public static final byte EVENT_RECEIVE_GROUP_COMPLETE=88;
	
	/**清空内存里面的联系人分组和联系人的事件*/
	public static final byte EVENT_RECEIVE_ADDITION_EQUALS_ZERO=89;
	
	/**设置显示在线联系人的事件*/
	public static final byte EVENT_ROSTER_GROUP_BY_NORMAL=90;
	/**刷新main activity*/
	public static final byte EVENT_ZWP  =  91;
	/**Zwp 加载界面*/
	public static final byte EVENT_ZWP_LOAD  =  92;
	/**Zwp 取消加载界面*/
	public static final byte EVENT_ZWP_CANCEL  =  93;
	/**更新 广告页面*/
	public static final byte EVENT_AD_UPDATA  =  94;
	
	public static final byte OPEN_CHAT_FROM_SEARCH_ROSTER = 95;

	//////////////////增加按状态排序功能
	/**按状态排序*/
	public static final byte EVENT_ROSTER_GROUP_BY_STATE = 96;
	/**设置是否显示离线联系人*/
	public static final byte EVENT_SET_IS_SHOW_ONLINE = 97;
	
	/**MyItem 触发启动logo更换界面*/
	public static final byte EVENT_MY_ITEM_LOGO = 98;
	
	/**MyItem 触发启动info更换界面*/
	public static final byte EVENT_MY_ITEM_INFO = 99;
	
	/**更新取出头像*/
	public static final byte EVENT_UPDATE_PORTRAIT_HASH = 100;
	
	/**主界面广告点击事件*/
	public static final byte EVENT_MAIN_AD_CLICK = 101;
	/**
	 * 事件发生时被回调的方法<br>
	 * @param eventType 事件类型
	 * @param obj 事件相关参数，数据类型和事件类型相关
	 */
	public void eventAction(byte eventType,Object obj);
	
	
}