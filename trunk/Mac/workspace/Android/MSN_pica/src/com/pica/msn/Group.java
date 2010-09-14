package com.pica.msn;
import java.util.*;
public class Group {
	
	/**联系人|频道|聊天室 的jid */
	public String SUBITEM_JID;//jid 联系人 频道 聊天室 都可以用
	
	/**联系人分组的组ID*/
    public String GROUP_id;//  
    /**组gb码*/
    public String GROUP_gbcode;// 
    /**组名*/
    public String GROUP_name;//  
    /**组类型 用户自定义分组 聊天室分组 频道分组 广告*/
    public byte GROUP_flag;//  
    /**组的展开状态*/
    public boolean  GROUP_isexpand;
    /**组内成员的数量*/
    public short GROUP_itemnum;// 
    /**组内在线状态的人数*/
    public String GROUP_maxweight_itemnum;
    
    public Vector<Contact> contactVector;
    
    public Group()
    {
    	contactVector=new Vector<Contact>();
    }
}
