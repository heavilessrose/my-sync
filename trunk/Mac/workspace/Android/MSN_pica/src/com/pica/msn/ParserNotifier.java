package com.pica.msn;

import java.util.Vector;

public interface ParserNotifier
{   
    /**设置cache状态 暂时没用*/
    public void setCacheStatus(boolean isCacheEmpty);
    
    /**收到该次客户端登录的session id http联网需要用 socket联网不需要*/
    public void receiveSessionId(String sid);
    
    /**如果 errorCode为空 收到jerror 需要返回到登录页面<br>
     * 	1401 登录passport失败<br>
		1402 客户端没有为请求的操作做好同步准备<br>
		1403 增量同步操作失败。客户端应该执行一次完全同步<br>
		1600 被其他客户端踢出<br>
		1601 Pica Server长时间没有收到客户端的心跳包
     * */
    public void receiveLoginJerror(String errorMsg,String errorCode);
    
	//#ifdef support_save_rms
    /**收到addition==0  同步错误 需要提示用户然后注销退出*/
    public void receiveSynchronizeError();
    //#endif
    
    /** 收到iq error包 返回进入操作等待前的页面 如果jid为群的jid 则表示登入群过程中报错*/
    public void receiveOtherJerror(String jid,String errorMsg);

	
	/**客户端收到此包后 发送上线状态  <presence type=’available'/>*/
	public void receivePicaIqRoster();
	
	/**收到个人资料*/
	public void receiveVcardTemp(String nickname,String impresa,String portrait_data,String resourceId,String url,Vector nodeVec);
		
	
	/**对广告包进行解析处理*/
	public void receiveAD(String type,String id,String action,String target,String param0,String dbid,String sale,String text,String src);
	
	/**广告收到后要保存RMS 保存广告id的作用*/
	public void receiveADComplete();

	
	/**收到聊天室 并刷新页面
	 * @param type 聊天室的类型 type=’fixed固定|custom定制 zwp类wap频道
	 * */
	public void receiveRoom(String jid,String name,String type);
	

	/***
	 * 收到验证请求 生成验证请求的好友的对象 将其保存到verifyFriendVector向量里面 然后显示验证请求页面
	 * @param imid 验证联系人的imid
	 * @param action =subscriptionanswer
	 * @param nickname 验证联系人的昵称
	 * @param realname 验证联系人的真实姓名
	 * @param sex 验证联系人的性别
	 * @param age 验证联系人的年龄
	 * @param province 验证联系人的省份
	 * @param desc 验证联系人的描述
	 */
	public void receiveVerify(String imid,String action,String nickname,String realname,String sex,String age,String province,String desc);
	
	
	/**收到退出注销某聊天室的包 退出等待页面 删掉一个聊天室 然后刷新页面*/
	public void receiveExitCluster(String jid);
	
	/**客户端收到新的聊天室列表后，退出等待页面 增加一个聊天室 更新列表显示。
	 * 如果当前的视图为类WAP视图，弹出提示信息“热点话题列表已更新，是否现在查看？”。
		如果用户点击“确定”，则切换视图到联系人视图，同时展开“热点话题”组。*/
	public void receiveJoinCluster(String jid,String name,String type);
	
	/**收到被踢出聊天室或者被加入黑名单的包 给用户提示*/
	public void receiveKickoffOrBlacklistChatroom(String role,String reason);
	
    /***
     * 收到单聊消息
     * @param jid 该消息联系人的jid
     * @param imid 该消息联系人(陌生人)的imid
     * @param sid 会话sid
     * @param body 消息内容
     * @param stamp 消息时间
     * @param nudge 消息是否震动
     * @param status ="fail"失败
     * @param reason 消息失败原因
     * @param transfer_ftID 文件发送失败 根据这个值来查找文件传送消息
     */
    public void receiveMessage(String jid,String imid,String sid, String body, String stamp,boolean nudge,String status,String reason,String transfer_ftID);

	//#ifdef support_groupmessage
    /**收到临时群聊消息 有可能是陌生人发送的 用imid区分*/
    public void receiveGroupMessage(String from,String jid,String imid,String sid,  String body,String stamp,boolean nudge,String status,String reason);
	//#endif
    
    /**收到群里面相关信息*/
    public void receiveClusterMessage(String from, String to,String name, String body, String stamp);
    

	
	/**收到 表示用户验证通过 接着发送获取PICA好友列表*/
	public void receiveIqAuth();
	
	/***
	 * 收到dispacth回包
	 * @param result result=0系统不允许登录 result=1客户端需要强制升级  result=2成功
	 * @param noticeVec 装载系统提示信息的容器
	 * @param force force=1强制升级 force=0建议升级
	 * @param url 下载升级客户端的URL
	 * @param version  新客户端版本号
	 * @param id  Dispatch分配的临时pica帐号  如67890001@pica
	 * @param password 临时pica账号的密码
	 * @param ip 登陆pica服务器的地址和端口 jabber服务器
	 * @param ua 手机UA
	 * @param carrier 运营商信息
	 * @param adurl 广告服务器的URL
	 * @param ad_interval 取广告的时间间隔
	 * @param zwp ZWP网关的IP和端口 如果此项没有，或者值为空，则不支持类WAP
	 * @param fpage 类WAP的首页地址
	 * @param add_friend msn.cn用户添加好友的地址
	 * @param room_name  MSN群分组的名称 如果此项没有或者为空，则不支持MSN群功能
	 * @param room_search 查找MSN群（聊天室）的地址
	 * @param hotmail Hotmail邮箱服务 的地址 如果此项没有或者为空，则不支持邮箱
	 * @param spaces MSN spaces地址 如果此项没有或者为空，则不支持空间
	 * @param search  MSN 搜索地址 如果此项没有或者为空，则不支持搜索
	 * @param roomCreateAddress 创建/管理自建群的地址
	 * @param roomInviteAddress 邀请加入自建群的地址
	 * @param transparent 对客户端无意义，客户端登录MSN时将此节点透传给网关
	 * @param transfer_limit
	 * @param data_url
	 * @param data_method
	 * @param capability 能力
	 */
	public void receiveDispatch(String result,Vector noticeVec,String force,String url,String version,String id,String password,String ip,String ua,String carrier,String adurl,String ad_interval,String zwp,String fpage,
			String add_friend,String room_name,String room_search,String hotmail,String spaces,String search,String roomCreateAddress,String roomInviteAddress,String transparent,String transfer_limit,String data_url,
			String data_method,String capability);
	
	/**
	 * 收到分组列表
	 * @param last last=’0还有后续包|1没有后续包
	 * @param groupName 分组名 
	 * @param groupID 分组ID
	 * @param gbcode 分组名前4个字符的gb2312码
	 * @param sync  new新分组|updated修改的分组|deleted删除的分组
	 * @param version 分组的版本号
	 */
    public void receiveGroup(String last, String groupName, String groupID, String gbcode,String sync, String version);
    
    /**收到分组列表 且没有后续包了
     * */
    public void receiveGroupComplete(String last,String version);
    
    /**
     * 收到联系人列表(登录时收到或者是添加/删除联系人等操作成功)
     * @param action=add添加|delete只删除|rmblk删除并阻止|block阻止|unblock解除阻止
     * @param last  last=’0还有后续包|1没有后续包 
     * @param jid  联系人的jid
     * @param imid 联系人的imid
     * @param nickname 联系人的昵称
     * @param gbcode 联系人的gbcode码
     * @param block  联系人的阻止情况 0正常|1阻止
     * @param sync  new新联系人|updated修改的联系人|deleted删除的联系人
     * @param groupId 联系人的分组id
     * @param impresa 联系人的心情短语
     * @param portrait 联系人的头像标识
     * @param phone 联系人的电话
     * @param gleam 联系人的控件更新情况
     * @param url 联系人的空间地址
     * @param version 联系人的
     * @param nodeVec 储存了所有包括的节点元素
     * @param deviceId 设备id
     * @param addition
     */
    public void receiveRoster(String action,String last,String jid,String imid,String nickname,String  gbcode,String  block, 
    		String sync, String groupId,String impresa,String portrait,String phone, String gleam,
    		String url,String version,Vector nodeVec,String deviceId,String addition);
    
    /**收到联系人列表 没有后续包了<br>
     * 如果action不为空 则表示是添加删除等联系人操作 这时候不再需要保存整个列表到数据库中了
     * 将版本号保存到RMS
     * */
    public void receiveRosterComplete(String  last,String  version,String action);
    
	//#ifdef support_head_image
    /***
     * 收到联系人的头像 转码后 保存到rms里面去 url和imageData不能共存
     */
    public void receiveContactPortrait(String jid,String portraithash,String imageData,String url);
    //#endif
    
	//#ifdef support_groupmessage
    /**用户加入或离开多人会话*/
    public void receiveParticipant(String sid, String jid,String[][]node);
    //#endif
    
    /**用户收到频道列表  要刷新列表*/
    public void receiveChannel(String jid,String name,String groupname,String[][] options,boolean iswap,boolean auth);
    
    /**刷新页面
     * */
    public void receiveUpdateUI();
    /**内存不足*/
    public void receiveOutOfMemory();
    
    /**用户请求登出成功 收到presece type=unavailable包 用户收到msn注销成功包*/
    public void receiveLogoutMsn();
    
    /**收到用户改变自己的状态*/
    public void receiveModifyMyState(String show);
    
    /**	表示客户端发送上线状态  服务器返回成功 接下来就是客户端发起登录MSN*/
    public void receiveLoginPicaSuccess();
    
    /**联系人状态等的改变包
	 * @param type    available联机及其他|unavailable脱机
	 * @param from    联系人的jid
	 * @param version  联系人同步版本
	 * @param show   联系人状态值
	 * @param nickname  联系人昵称
	 * @param gbcode  联系人昵称前4个字符的gb2312码
	 * @param impresa  联系人个人信息（心情短语）
	 * @param portrait  联系人头像标识
	 * @param mobile  联系人 0非移动用户|1移动用户
	 * @param gleam 该联系人的空间更新情况 0=没有更新|1=有更新  
	 * @param url 该联系人的空间地址
	 * @param phone 该联系人的电话
	 * @param nodeVec 储存了所有包括的节点元素
     * */
    public void receiveContactChangeState(String type,String from,String version,String  show,String  nickname,String gbcode,String  impresa,String  portraithash,String gleam,String url,String phone,Vector nodeVec);

    /**收到additon=0的包 客户端需要删掉本地保存的联系人分组列表，联系人列表，和版本号*/
    public void receiveAdditonEqualsZero();
    
    
    ///////////////////////////////////
    //文件及语音功能的处理
    //////////////////////////////////
    
    /**
     *  接收方收到的传送文件请求
     * @param from ’msn.pica’
     * @param jid
     * @param imid
     * @param type type=’ft’
     * @param transferID 服务器用来标识传输的文件
     * @param sid 会话sid
     * @param ftID
     * @param action
     * @param transfer_name 文件名称
     * @param transfer_size 文件大小（字节数）
     */
    public void receiveTransferInviteRequest(String from,String jid,String imid,String type,String transferID,String sid,String ftID,
    		String action,String transfer_name,String transfer_size);
    
    /**
     *传送文件请求的处理 发送方收到的回应 如果接收方拒绝了请求，该传送过程终止
     * @param from
     * @param type
     * @param transferID
     * @param sid
     * @param ftID
     * @param action  accept 接受 | reject 拒绝
     */
    public void receiveTransferInviteResponse(String from,String type,String transferID,String sid,String transfer_ftID,String transfer_action);
    
    /**
     * 传送文件 带内方式 发送方收到的响应 如果发送成功，则继续发送剩余的数据直至文件全部发送完成；否则发送过程终止
     * @param from
     * @param type
     * @param transferID
     * @param sid
     * @param transfer_ftID
     * @param transfer_seqid
     * @param transfer_action  ack
     * @param transfer_status status=’ok成功|fail失败|canceld取消’
     */
    public void receiveTransferInnerResponse(String from,String type,String transferID,String sid,String transfer_ftID,String transfer_seqid,
    		String transfer_action,String transfer_status);
    
    /**
     * transfer接收方接收到的数据
     * @param from
     * @param jid
     * @param imid
     * @param type
     * @param transferID
     * @param sid
     * @param transfer_ftID
     * @param transfer_seqid
     * @param transfer_action
     * @param transfer_bytes
     * @param transfer_start 数据起始位置
     * @param transfer_end 数据结束位置
     * @param transfer_base64Length Base64编码长度
     * @param transfer_base64Data Base64数据块
     */
    public void receiveTransferData(String from,String jid,String imid,String type,String transferID,String sid,
    		String transfer_ftID,String transfer_seqid,String transfer_action,String transfer_bytes,
    		String transfer_start,String transfer_end,String transfer_base64Length,String transfer_base64Data);
    
    
    /**
     * transfer 发送方发送文件结束后发送方收到的响应 认为发送完全成功了
     * @param from
     * @param type
     * @param transferID
     * @param sid
     * @param transfer_ftID
     * @param transfer_action
     */
    public void receiveTransferCompleteAck(String from,String type,String transferID,String sid,String transfer_ftID,String transfer_action);
    
    
    /**接收方收到complete 认为接收完毕 需要再发一个complete_ack给服务器*/
    public void receiveTransferComplete(String from,String type,String transferID,String sid,String transfer_ftID,String transfer_action);

    /**
     * 传送文件（带外方式）  transfer接收方接收到的数据
     * @param from
     * @param jid
     * @param imid
     * @param type
     * @param transferID
     * @param sid
     * @param transfer_ftID
     * @param transfer_action
     * @param transfer_url 获取该文件的地址
     */
    public void receiveTransferOuterReceive(String from,String jid,String imid,String type,String transferID,String sid,String transfer_ftID,
    		String transfer_action,String transfer_url);
    
    /**
     * 在文件传送的过程中，取消文件的传送
     * @param from
     * @param type
     * @param transferID
     * @param sid
     * @param transfer_ftID
     * @param transfer_action cancel 取消
     */
    public void receiveTransferCancelResponse(String from,String type,String transferID,String sid,String transfer_ftID,String transfer_action);
    
    /**
     * 接收语音剪辑
     * @param from
     * @param jid
     * @param imid
     * @param type
     * @param sid
     * @param transfer_data_mime
     * @param transfer_bytes 文件总长度
     * @param transfer_base64Length Base64编码长度
     * @param transfer_base64Data Base64编码头像文件内容
     * @param transfer_url_mime
     * @param transfer_url 下载文件的url
     */
    public void receiveTransferVoiceclip(String from,String jid,String imid,String type,String sid,String transfer_data_mime,String transfer_bytes,String transfer_base64Length,String transfer_base64Data,String transfer_url_mime,String transfer_url);
}

