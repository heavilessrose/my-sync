package com.pica.msn;

public class Contact {

	/**联系人|频道|聊天室 的jid */
	public String SUBITEM_JID;//jid 联系人 频道 聊天室 都可以用
	
	/**联系人的在线状态 列表项排序所依据的权重 频道和聊天室都默认为脱机*/
	public byte SUBITEM_STATE=MSNApplication.STATE_OFFLINE; //在线状态 列表项排序所依据的权重 频道和聊天室不赋值 所以默认都是为0
	
	//*****************************
	//联系人
    //*****************************
    /**联系人的邮箱*/
    public String CONTACT_imid;//邮箱
    /**联系人的昵称*/
    public String CONTACT_nickname;// 昵称
    /**联系人的本地显示名称前4个字符的gb2312码*/
    public String CONTACT_gbcode;//   本地显示名称前4个字符的gb2312码
    /**联系人的分组id*/
    public String CONTACT_groupId;//分组id
    /**联系人的联系人标志位（正常、陌生人、黑名单）*/
    public byte CONTACT_contactFlag;// 联系人标志位（正常、陌生人、黑名单）
    /**联系人的心情短语*/
    public String CONTACT_impresa;// 心情短语
    /**联系人的电话号码*/
    public String CONTACT_phone;//电话号码 如果非空 则增加“拨打电话”
    /**联系人的当前状态的文字说明 如busy|online等*/
    public String CONTACT_statusShow;// 当前状态的文字说明 busy online等 不用保存到rms	
    /**联系人的个人空间更新 0=没有更新 1=有更新*/
    public String CONTACT_gleam;//0 没有更新 1 有更新
    /**联系人的空间地址*/
    public String CONTACT_url;// 空间地址
    
    //public String CONTACT_mobile;//   0非移动用户|1移动用户
    
    //#ifdef support_head_image
    public String CONTACT_portraithash;// 联系人的头像标识，根据该值客户端可以判断用户的头像是否发生变化
	public byte[] CONTACT_imageData;// 头像数据
	public boolean CONTACT_hasRMSImageData=false;//RMS里面是否保存有头像数据
    /**当前已发送过获取头像包*/
	public boolean CONTACT_hasSendGetImage=false;
	//#endif
	
	/**带外下载头像 用来判断是否是自己的个人头像 如果等于自己的imid 则认为是自己的个人头像*/
	public String CONTACT_OUTER_HEAD_ONESELF_IMID;
	/**带外 头像 状态*/
	public byte CONTACT_OUTER_HEAD_STATUS;
	
	
	/**重传次数*/
	public String CONTACT_FILE_VOICE_TRANSFER_COUNT;
	
	/**已传输的百分比 0-100*/
	public String CONTACT_GAUGE_INDEX;
	
	/**消息附件下载地址*/
	public String CONTACT_FILE_URL;//  文件下载地址
	
	/**消息附件的 数据base64格式*/
	public byte[] CONTACT_FILE_DATA;
	
	/**消息附件文件名*/
	public String CONTACT_FILE_VOICE_NAME;
	
	/**和该文件/语音剪辑 相关的带外传输对象*/
	public FileOuterTransfer CONTACT_FILEOUTTRANSFER;
	
	/**消息附件的状态 
	 * <br>发送：等待发送等待发送中 被对方拒绝<br>，正在发送<br>发送完成<br>发送中取消(主动或被动)<br>发送中异常<br>
	 * <br>接收：等待接收<br>等待中自己拒绝<br>正在接收<br>接收完毕<br>接收中取消<br>接收中异常*/
	public byte CONTACT_FILE_STATUS_TYPE;//
}
