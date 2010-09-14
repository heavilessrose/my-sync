package com.pica.msn;

import java.util.*;

import android.util.Log;




/**
 * 解析器，用于解析服务器响应的XML内容。解析完数据后，产生底层事件。
 */
public class Parser

{

	ParserNotifier parserNotifier;

	
	boolean isDeleteAllRecord=false;
	MSNApplication app=null;
	Parser(ParserNotifier notifier)
	{
		parserNotifier = notifier;
		app=MSNApplication.getApplication();
	}


	public void parseMessage(XmlReader reader)throws Exception
	{
		String from = reader.getAttribute("from");
		String type = reader.getAttribute("type");
		String jid=reader.getAttribute("jid");
		String imid=reader.getAttribute("imid");
		String status = reader.getAttribute("status");//发送失败时候才有
		String sid=reader.getAttribute("sid");
		String to=reader.getAttribute("to");
		String name=reader.getAttribute("name");
		String body = null, reason = null, stamp = null;
		boolean nudge=false;//闪屏震动
		
		String transferID=reader.getAttribute("transferid");//服务器用来标识传输的文件
		String transfer_ftID=reader.getAttribute("ftid");;
		String transfer_action=null;
		String transfer_name=null;
		String transfer_size=null;
		String transfer_seqid=null;
		String transfer_status=null;
		String transfer_bytes=null;
		String transfer_start=null;
		String transfer_end=null;
		String transfer_base64Length=null;
		String transfer_base64Data=null;
		String transfer_url=null;
		String transfer_data_mime=null;
		String transfer_url_mime=null;
		

		
		while (reader.next() == XmlReader.START_TAG)
		{
			if (reader.getName().equals("body"))
			{
				body = parseText(reader);
			} 
			else if(reader.getName().equals("nudge"))
			{
				parseIgnore(reader);
				nudge=true;
			}
			else if (reader.getName().equals("reason"))
			{
				reason = parseText(reader);//失败的原因
			} 
			else if (reader.getName().equals("x"))
			{
				stamp = reader.getAttribute("stamp");
				String xmlns = reader.getAttribute("xmlns");

				if (xmlns == null || xmlns.length() == 0)
				{
					parseIgnore(reader);
				} 
				else if (xmlns.equals("jabber:x:oob"))
				{
					while (reader.next() == XmlReader.START_TAG)
					{
						parseIgnore(reader);		
					}
				} 	
				else
				{
					parseIgnore(reader);
				}

			} 
			else if(reader.getName().equals("control"))
			{
				transfer_ftID=reader.getAttribute("ftid");
				transfer_action=reader.getAttribute("action");
				transfer_status=reader.getAttribute("status");
				transfer_seqid=reader.getAttribute("seqid");
				while (reader.next() == XmlReader.START_TAG)
				{
					String temp = reader.getName();
					if(temp.equals("name"))
						transfer_name=parseText(reader);
					else if(temp.equals("size"))
						transfer_size=parseText(reader);
					else if(temp.equals("data"))
					{
						transfer_bytes=reader.getAttribute("bytes");
						transfer_start=reader.getAttribute("start");
						transfer_end=reader.getAttribute("end");
						transfer_base64Length=reader.getAttribute("length");
						
						transfer_base64Data=parseText(reader);
					}
					else
						parseIgnore(reader);
				}

			}
			else if(reader.getName().equals("data"))
			{
				transfer_data_mime=reader.getAttribute("mime");
				transfer_bytes=reader.getAttribute("bytes");
				transfer_base64Length=reader.getAttribute("length");
				
				transfer_base64Data=parseText(reader);
			}
			else if(reader.getName().equals("url"))
			{
				transfer_url_mime=reader.getAttribute("mime");
				transfer_url=parseText(reader);
			}
			else
			{
				parseIgnore(reader);
			}
		}
		////////
		//android暂时不考虑内存
		//if(isOutOfMemory())
		//	return;
		/////////
		if(from!=null&&from.indexOf(MSNApplication.CLUSTER_DOMAIN)!=-1)
		{
			//关于群聊天室的信息包
			//parserNotifier.receiveClusterMessage(from,to,name,body,stamp);
		}
		else
		{	
			if (type != null&& type.equals("chat"))
				parserNotifier.receiveMessage(jid,imid,sid,body,stamp,nudge,status,reason,transfer_ftID);
			//#ifdef support_groupmessage
			//android暂时不考虑多人对话
			else if (type != null && type.equals("groupchat"))
			{
				parserNotifier.receiveGroupMessage(from,jid,imid,sid,body,stamp,nudge,status,reason);
			}
			//#endif
			else if(type!=null&&type.equals("ft"))
			{
				if(transfer_action!=null)
				{
					if(transfer_action.equals("push"))
						parserNotifier.receiveTransferInviteRequest(from,jid,imid,type,transferID,sid,transfer_ftID,transfer_action,transfer_name,transfer_size);
					else if(transfer_action.equals("accept")||transfer_action.equals("reject"))
						parserNotifier.receiveTransferInviteResponse(from,type,transferID,sid,transfer_ftID,transfer_action);
					else if(transfer_action.equals("ack"))
						parserNotifier.receiveTransferInnerResponse(from,type,transferID,sid,transfer_ftID,transfer_seqid,transfer_action,transfer_status);
					else if(transfer_action.equals("data"))
						parserNotifier.receiveTransferData(from,jid,imid,type,transferID,sid,transfer_ftID,transfer_seqid,transfer_action,transfer_bytes,
								transfer_start,transfer_end,transfer_base64Length,transfer_base64Data);
					else if(transfer_action.equals("complete_ack"))
					{
						if(transfer_url!=null&&transfer_url.length()!=0)
						{
							parserNotifier.receiveTransferOuterReceive(from,jid,imid,type,transferID,
									sid,transfer_ftID,transfer_action,transfer_url);
						}
						else
							parserNotifier.receiveTransferCompleteAck(from,type,transferID,sid,transfer_ftID,transfer_action);
					}
					else if(transfer_action.equals("complete"))
					{
						parserNotifier.receiveTransferComplete(from,type,transferID,sid,transfer_ftID,transfer_action);
					}
					else if(transfer_action.equals("cancel"))
					{
						parserNotifier.receiveTransferCancelResponse(from,type,transferID,sid,transfer_ftID,transfer_action);
					}				
				}
			}
			else if(type!=null&&type.equals("voiceclip"))
			{
				parserNotifier.receiveTransferVoiceclip(from,jid,imid,type,sid,transfer_data_mime,transfer_bytes,transfer_base64Length,transfer_base64Data,transfer_url_mime,transfer_url);
			}
		}
	}
	
	public void parsePresence(XmlReader reader)throws Exception
	{
		String from = reader.getAttribute("from"), to = reader
				.getAttribute("to"), type = reader.getAttribute("type"), version = reader
				.getAttribute("version");
		String show = null, nickname = null, gbcode = null, impresa = null, portraithash = null;

		String name = null, role = null, speak = null, prvtmsg = null, imid = null;
		String errorText = null;
		String phone = null, gleam = "0", url = null;// 该联系人的空间地址
		String temp;
		Vector nodeVec = new Vector();
		while (reader.next() == XmlReader.START_TAG) 
		{
			temp = reader.getName();
			if (temp.equals("show")) {
				show = parseText(reader);// 状态值
				if (from.indexOf(MSNApplication.CLUSTER_DOMAIN) == -1)
					nodeVec.addElement("show");
			} else if (temp.equals("nickname")) {
				nickname = parseText(reader);// 昵称
				nodeVec.addElement("nickname");
			} else if (temp.equals("gbcode")) {
				gbcode = parseText(reader);// 昵称前4个字符的gb2312码
				nodeVec.addElement("gbcode");
			} else if (temp.equals("impresa")) {
				impresa = parseText(reader);// 个人信息（心情短语）
				nodeVec.addElement("impresa");
			} else if (temp.equals("portraithash")) {
				portraithash = parseText(reader);// 头像标识
				if (portraithash.length() == 0)
					portraithash = null;
				nodeVec.addElement("portraithash");
			}
			// else if(temp.equals("mobile"))
			// {
			// mobile=parseText(reader);//0非移动用户|1移动用户
			// nodeVec.addElement("mobile");
			// }
			else if (temp.equals("spaces")) {
				gleam = reader.getAttribute("gleam");// 0没有更新，默认|1有更新
				nodeVec.addElement("gleam");
				url = parseText(reader);// 该联系人的空间地址
				nodeVec.addElement("spaces");
			} else if (temp.equals("phone")) {
				phone = parseText(reader);
				nodeVec.addElement("phone");
			} else if (temp.equals("name")) {
				name = parseText(reader);
			} else if (temp.equals("role"))// 聊天室管理员
			{
				role = parseText(reader);
			} else if (temp.equals("speak"))// 是否有权在聊天室中发言
			{
				speak = parseText(reader);
			} else if (temp.equals("prvtmsg"))// 聊天室是否允许发悄悄话
			{
				prvtmsg = parseText(reader);
			} else if (temp.equals("imid")) {
				imid = parseText(reader);
			} else if (temp.equals("error")) {
				errorText = parseText(reader);
			} else
				parseIgnore(reader);
		}
		if (from.indexOf(MSNApplication.CLUSTER_DOMAIN) != -1)// 有关聊天室的操作
			;
			//parserNotifier.parseRoomPresence(type, from, to, name, role, speak,prvtmsg, imid, errorText);
		else
		{
			if (from != null && from.equals(MSNApplication.MSN_DOMAIN))
			{
				if (type != null && type.equals("unavailable"))
				{
					//receiveLogoutMsn();// 用户请求登出 成功
					parserNotifier.receiveLogoutMsn();
				}
				else
				{
					parserNotifier.receiveModifyMyState(show);
					// 用户改变自己的状态
				}
			} 
			else
			{
				//<presence type=’available' from=’ '78276741@pica/pica’/>
				if (type != null && type.equals("available") && from != null
						&& from.equals(MSNApplication.getApplication().GetPicaID() + "/pica"))
				{
					
					//表示客户端发送上线状态  服务器返回成功
					//接下来就是客户端发起登录MSN
					parserNotifier.receiveLoginPicaSuccess();
				} 
				else
				//联系人的状态变化
				{
					parserNotifier.receiveContactChangeState(type,from,version, show, nickname,
							gbcode, impresa, portraithash,gleam,url,phone,nodeVec);
					////////
					//if(isOutOfMemory())
					//	return;
					/////////
				}
			}
		}
	}
	
	public void parseIq(XmlReader reader)throws Exception
	{
		String type = reader.getAttribute("type");
		String from = reader.getAttribute("from");
		String last = reader.getAttribute("last");
		//#ifdef support_save_rms
		String addition=reader.getAttribute("addition");
		//#endif
		
		
		if (type.equals("error"))
		{
			String errorCode;
			String errorMsg;
			while (reader.next() == XmlReader.START_TAG)
			{
				if (reader.getName().equals("error"))
				{				
					 errorCode=reader.getAttribute("code");
					 errorMsg = parseText(reader);
					 if(errorCode.equals("1401")||errorCode.equals("1402")||
							 errorCode.equals("1403")||errorCode.equals("1600")
							 ||errorCode.equals("1601"))
					 {

						 parserNotifier.receiveLoginJerror(errorMsg,errorCode);
					 }
					 else 
					 {
						 parserNotifier.receiveOtherJerror(from, errorMsg);
					 }	
				} 
				else
					parseIgnore(reader);
			}
		} 
		else if (type.equals("get"))
		{
			String xmlns;
			String action;
			while (reader.next() == XmlReader.START_TAG)
			{
				if (reader.getName().equals("query"))
				{
					xmlns = reader.getAttribute("xmlns");
					action = reader.getAttribute("action");
					if (xmlns == null || xmlns.length() == 0)
						parseIgnore(reader);
					if (xmlns.equals("jabber:iq:roster"))
					{
						parseVerify(reader,action);
					} 
					else
						parseIgnore(reader);
				} else
					parseIgnore(reader);
			}
		} 
		else if (type.equals("set"))
		{
			String xmlns;
			String sid;
			String action;
			while (reader.next() == XmlReader.START_TAG)
			{
				if (reader.getName().equals("query"))
				{
					xmlns = reader.getAttribute("xmlns");
					//#ifdef support_groupmessage
					sid = reader.getAttribute("sid");	
					action = reader.getAttribute("action");	
					//#endif
					if (xmlns == null || xmlns.length() == 0)
						parseIgnore(reader);
					//#ifdef support_groupmessage
					else if (xmlns.equals("jabber:x:dialog"))
					{
					    parseDialog(reader, sid, action);			    
					} 
					//#endif
//					else if (xmlns.equals("pica:cluster:join"))
//					{ // 加入/注销聊天室成功
//						parseJoinOrExitCluster(reader);
//					} 
//					else if (xmlns.equals("pica:cluster:role"))
//					{ // 被踢出或者加入黑名单
//						parseOutReason(reader);
//					} 
					else
						parseIgnore(reader);
			 }
		  }
		} 
		else if (type.equals("result"))
		{
			String version;
			String resourceId;
			String action;
			String xmlns;
			String deviceId;
			while (reader.next() == XmlReader.START_TAG)
			{
				if (reader.getName().equals("query"))
				{
					version = reader.getAttribute("version");
					resourceId = reader.getAttribute("resourceid");
					action = reader.getAttribute("action");
					xmlns = reader.getAttribute("xmlns");
					deviceId = reader.getAttribute("deviceid");//如果有deviceid字段，覆盖本地保存的
					if (xmlns.equals("vcard-temp"))//获取用户个人资料
					{
						parseVcardTemp(reader,resourceId);
					}
//					else if (xmlns.equals("pica:cluster:roomlist"))// 返回聊天室列表
//						parseChatroom(reader);
					else if (xmlns.equals("jabber:iq:roster"))
					{
						if(from.equals(MSNApplication.MSN_DOMAIN))//"msn.pica"
						{
							if(action!=null&&action.equals("getportrait"))
							{
								//#ifdef support_head_image
								//取头像
								parseContactPortrait(reader);
								//#else
								//# parseIgnore(reader);
								//#endif
							}
							else//取联系人列表
							{					
								parseIqRoster(reader,action,last,version,deviceId,addition);
							}
						}
						else
						{
							reader.next();
							parserNotifier.receivePicaIqRoster();	
						}			
					}
					else if(xmlns.equals("jabber:iq:auth"))
					{
						reader.next();
						parserNotifier.receiveIqAuth();
					}
					else if(xmlns.equals("jabber:iq:group"))
					{
						//#ifdef support_save_rms
						if(addition!=null&&addition.equals("0"))//替换
						{						
							if(!isDeleteAllRecord)//删掉本地保存的联系人分组列表
							{
								
								parserNotifier.receiveAdditonEqualsZero();
								
								//////////////////////
								isDeleteAllRecord=true;
							}
						}
						//#endif
						parseGroup(reader,last,version,addition);
					}
					//else if(xmlns.equals("jabber:iq:picazone"))
					//{
						//parseChannel(reader); 
					//}
					else
						parseIgnore(reader);
				} 
				else
					parseIgnore(reader);
			}

		}
	}
	
	/**获取联系人列表*/
	private void parseIqRoster(XmlReader reader,String action,String last, 
			String version,String deviceId,String addition)throws Exception
	{
		while (reader.next() == XmlReader.START_TAG)
		 {
		    String jid=null, imid=null,nickname=null, gbcode=null, block=null, 
	        sync=null, group=null, impresa=null, portraithash=null,
	        phone=null,gleam="0",url=null;//该联系人的空间地址
		    Vector nodeVec=new Vector();
			if (reader.getName().equals("item"))
			{
				jid = reader.getAttribute("jid");
				imid=reader.getAttribute("imid");
				if(imid!=null)
					nodeVec.addElement("imid");
				nickname = reader.getAttribute("nickname");
				if(nickname!=null)
					nodeVec.addElement("nickname");
				gbcode = reader.getAttribute("gbcode");

				
				if(gbcode!=null)
				{
					//精简版本 gbcode不用那么长
					//#ifndef support_enough_memory
					if(gbcode.length()>8)
						gbcode=gbcode.substring(0,8);
					//#endif
					nodeVec.addElement("gbcode");
				}
					
				block = reader.getAttribute("block");
				if(block!=null)
					nodeVec.addElement("block");
				sync = reader.getAttribute("sync");
				
				
				String temp;
				while (reader.next() == XmlReader.START_TAG)
				{
					temp=reader.getName();
					if (temp.equals("group"))//分组id,分组id,
					{		
						//每个联系人可以属于多个分组，无分组id则默认为0
						group = parseText(reader);
						nodeVec.addElement("group");
					} 
					else if(temp.equals("impresa"))//个人信息（心情短语）
					{
						impresa=parseText(reader);
						nodeVec.addElement("impresa");
					}
					else if(temp.equals("portraithash"))
					{//头像标识
						portraithash=parseText(reader);
						if(portraithash.length()==0)
							portraithash=null;
						nodeVec.addElement("portraithash");
					}
					else if(temp.equals("spaces"))
					{
						gleam = reader.getAttribute("gleam");//0没有更新，默认|1有更新
						nodeVec.addElement("gleam");
						url=parseText(reader);//该联系人的空间地址
						nodeVec.addElement("spaces");
					}
					else if(temp.equals("phone"))
					{
						phone=parseText(reader);
						nodeVec.addElement("phone");
					}
					else
						parseIgnore(reader);
				}
				////////
				//if(isOutOfMemory())
				//	return;
				/////////
				parserNotifier.receiveRoster(action,last, jid,imid,nickname, gbcode, block, 
						sync, group,impresa, portraithash,phone,gleam,url,version,nodeVec,deviceId,addition);
			} 
			else
			{
				parseIgnore(reader);
			}
		}
	     app.eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);

		if(last!=null&&last.equals("1"))
		{
			parserNotifier.receiveRosterComplete(last, version,action);
			isDeleteAllRecord=false;//只有在收到这个包 才认为联系人分组和联系人列表全部收完了
		}

	}

	/** 收到被添加为好友的验证请求 */
	private void parseVerify(XmlReader reader,String action) throws Exception
	{
        String imid=null,nickname=null,realname=null,
               sex=null,age=null,province=null,desc=null;
        String temp;
		while (reader.next() == XmlReader.START_TAG) 
		{
			if(reader.getName().equals("item"))
			{
				imid=reader.getAttribute("imid");		        
				while (reader.next() == XmlReader.START_TAG)
				{
					temp=reader.getName();
					if (temp.equals("nickname"))
						nickname=parseText(reader);
					//以下为扩展信息 只有请求方为msn.cn用户才携带
					else if(temp.equals("realname")) 					
						realname=parseText(reader);
					else if(temp.equals("sex"))
						sex=parseText(reader);
					else if(temp.equals("age"))
						age=parseText(reader);
					else if(temp.equals("province"))
						province=parseText(reader);
					else if(temp.equals("desc"))
						desc=parseText(reader);
					else 
						parseIgnore(reader);
				} 
			 }
		     else
		     {
		         parseIgnore(reader);
		     }                  		
		}		
		
		parserNotifier.receiveVerify(imid,action,nickname,realname,sex,age,province,desc);
	}
	
	/**获取分组列表*/
	private void parseGroup(XmlReader reader, String last, String version, String addition)
			throws Exception
	{
		String groupName = null, groupId = null, gbcode = null, sync = null;
		while (reader.next() == XmlReader.START_TAG)
		{
			if (reader.getName().equals("group"))
			{
				groupName = reader.getAttribute("name");
				groupId = reader.getAttribute("groupid");
				gbcode = reader.getAttribute("gbcode");
				sync = reader.getAttribute("sync");
				
				if(addition!=null&&addition.equals("0"))
				{
					if(sync==null||sync.length()==0)
						sync="updated";
				}
				parserNotifier.receiveGroup(last, groupName, groupId,gbcode, sync, version);
				reader.next();
			} 
			else
				parseIgnore(reader);
		}
		app.eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);

		if (last != null && last.equals("1"))
			parserNotifier.receiveGroupComplete(last, version);

	}
	
	
	/** 获取用户个人资料 */
	private void parseVcardTemp(XmlReader reader,String resourceId)throws Exception
	{
		//如果某个节点不存在，表示该内容没有更新
		//所以要加一个向量 表示哪些节点存在
		String nickname =null,impresa=null;
		String portrait_data=null;
		String temp=null;
		Vector nodeVec=new Vector();
		String dataLength=null;//data长度
		String url=null;
		while (reader.next() == XmlReader.START_TAG)
		{
			temp= reader.getName();
			if (temp.equals("nickname"))
			{	
				nickname = parseText(reader);
				nodeVec.addElement("nickname");
			}
			else if(temp.equals("impresa"))
			{
				impresa = parseText(reader);
				nodeVec.addElement("impresa");
			}
			else if(temp.equals("portrait"))
			{
				while (reader.next() == XmlReader.START_TAG) 
				{
					String tag=reader.getName();
					
				    if (tag.equals("data"))
				    {
				      	portrait_data = parseText(reader);
				    	nodeVec.addElement("data");
					}
				    else if(tag.equals("url"))
				    {
				      	url = parseText(reader);
				    	nodeVec.addElement("url");
				    }
				    else
					    parseIgnore(reader);
				} 
			}
			else
				parseIgnore(reader);
		}
		parserNotifier.receiveVcardTemp(nickname,impresa,portrait_data,resourceId,url,nodeVec);
	}
	
	
	public String parseStream(XmlReader reader)throws Exception 
	{
		String id = null;
		id = reader.getAttribute("id");
		System.gc();
		if (id != null)
		{
			app.jabber.setSessionId(id);
			
		}
		return id;
	}
	
	/** 对dispatch回包进行解析 */
	public void parseDispatchResponse(String response, String urlPage,String content) throws Exception 
	{
		if (response == null || response.length() == 0|| !response.startsWith("<sso"))// 中国移动拦截页面
			response =MsnUtil.httpRequest(urlPage, content,app.jabber.jabberServer,app.jabber.jabberServerPort);

		if (response.startsWith("<sso")) 
		{
			parse(new XmlHttpReader(response), "dispatch");
		}
	}
	
//	public boolean cache_empty;

	public void parse(XmlReader reader, String cmdType) throws Exception 
	{
		if (cmdType != null && !cmdType.equals("dispatch")
				&& cmdType.equals("cache")) 
		{
//			cache_empty = true;
			parserNotifier.setCacheStatus(true);
		}
		String element;
		String result;
		while ((reader.next()) != XmlReader.END_DOCUMENT) 
		{			
			element = reader.getName();
			if (element == null) {
				return;
			}
			if (element.equals("jabber")) 
			{
				parseJabberXml(reader, cmdType);
			}
			else if(element.equals("ad"))
			{
				if(app.isLogout)
					return;
				parseADXml(reader);
			}
			else if (element.equals("sso"))// 解析dispatch回包
			{
				result = reader.getAttribute("result");
				parseDispatchSSOXml(reader, result);
			}
			//System.gc();
		}
	}
	
	/**解析广告*/
	private void parseADXml(XmlReader reader) throws Exception
	{
		int tag = reader.next();
		String tmp;
		while (tag== XmlReader.START_TAG)
		{
			tmp = reader.getName();
			if (tmp == null)
				return;
			if (tmp.equals("item")) 
			{
				parseADItem(reader);
			} 
			else 
			{
				parseIgnore(reader);
			}
			tag=reader.next();
			if(tag == XmlReader.END_TAG)
			{
				tag=reader.next();
			}	
			tmp = null;
		}
		parserNotifier.receiveADComplete();
		
	}
	
	private void parseADItem(XmlReader reader) throws Exception 
	{
		String type = reader.getAttribute("type");
		String id = reader.getAttribute("id");

		String action = reader.getAttribute("action");
		String target = reader.getAttribute("target");
		String param0 = reader.getAttribute("param0");
		String dbid=reader.getAttribute("dbid");
		String sale=reader.getAttribute("sale");
		
		String text=null,src=null;
		while (reader.next() == XmlReader.START_TAG) 
		{
			if (reader.getName().equals("text")) 
			{
				text=parseText(reader);
			}
			else if(reader.getName().equals("media"))
			{
				src=reader.getAttribute("src");
			}
			else
				parseIgnore(reader);			
		}
		
		parserNotifier.receiveAD(type,id,action,target,param0,dbid,sale,text,src);
	}
	
	
	/** 解析dispatch回包 */
	public void parseDispatchSSOXml(XmlReader reader, String result)
			throws Exception 
	{

		Vector noticeVec=new Vector();
		Vector noticeSystemVec=new Vector();
		String notice=null;
		String version=null;
		String url=null;
		String id=null;
		String password=null;
		String ip=null;
		String ua=null;//手机UA
		String carrier=null;//运营商信息
		String adurl=null;//广告服务器的URL
		String ad_interval=null;//取广告的时间间隔
		String zwp=null;//ZWP网关的IP和端口 如果此项没有，或者值为空，则不支持类WAP 如果不支持类WAP，则以下各项都强制为不支持
		String fpage=null;//类WAP的首页地址
		String add_friend=null;//msn.cn用户添加联系人的地址
		String room_name=null;//MSN群分组的名称  如果此项没有或者为空，则不支持MSN群功能
		String room_search=null;//查找MSN群（聊天室）的地址  
		String hotmail=null;//Hotmail邮箱服务的地址   如果此项没有或者为空，则不支持邮箱
		String spaces=null;//MSN spaces地址  如果此项没有或者为空，则不支持空间
		String search=null;//MSN 搜索地址  如果此项没有或者为空，则不支持搜索
		String roomInviteAddress=null;//邀请加入自建群的地址
		String roomCreateAddress=null;//管理自建群的地址
		String transparent=null;

		String force=reader.getAttribute("force");
		
		//
		String transfer_limit=null;//文件传送的大小限制（字节数） 如果没有此项或者值为空或者0，则设定在2M
		String data_url=null;//带外传送方式的媒体服务器地址
		String data_method=null;//数据传输的方法
		
		String capability=null;//客户端支持能力
		String element;
		while (reader.next() == XmlReader.START_TAG)
		{
			element = reader.getName();
			if (element == null)
			{
				return;
			}
			if (element.equals("system"))
			{
				while (reader.next()== XmlReader.START_TAG)
				{
					if (reader.getName().equals("notice"))
					{
						notice = parseText(reader);
						noticeSystemVec.addElement(notice);
					}
					else
						parseIgnore(reader);
				}
			}
			else if(element.equals("upgrade"))
			{
				while (reader.next()== XmlReader.START_TAG)
				{
					if (reader.getName().equals("notice"))
					{
						notice = parseText(reader);
						noticeVec.addElement(notice);
					}
					else if(reader.getName().equals("version"))
					{
						version = parseText(reader);
					}
					else if(reader.getName().equals("url"))
					{
						url = parseText(reader);
					}
					else
						parseIgnore(reader);
				}
			}
			else if(element.equals("login"))
			{
				while (reader.next()== XmlReader.START_TAG)
				{

					if (reader.getName().equals("id"))
					{
						id = parseText(reader);
					}
					else if(reader.getName().equals("password"))
					{
						password = parseText(reader);
					}
					else if(reader.getName().equals("ip"))
					{
						ip = parseText(reader);
					}
					else
						parseIgnore(reader);
						
				}
			}
			else if(element.equals("config"))
			{
				while (reader.next()== XmlReader.START_TAG)
				{	
					String temp=reader.getName();
					if (temp.equals("ua"))
						ua = parseText(reader);
					else if(temp.equals("carrier"))
						carrier = parseText(reader);
					else if(temp.equals("adurl"))
						adurl = parseText(reader);
					else if(temp.equals("ad_interval"))
						ad_interval = parseText(reader);
					else if(temp.equals("zwp"))
						zwp = parseText(reader);
					else if(temp.equals("fpage"))
						fpage = parseText(reader);
					else if(temp.equals("add_friend"))
						add_friend = parseText(reader);
					else if(temp.equals("room_name"))
						room_name = parseText(reader);
					else if(temp.equals("room_search"))
						room_search = parseText(reader);
					else if(temp.equals("hotmail"))
						hotmail = parseText(reader);
					else if(temp.equals("spaces"))
						spaces = parseText(reader);
					else if(temp.equals("search"))
						search = parseText(reader);
					else if(temp.equals("room_private_create"))
						roomCreateAddress=parseText(reader);
					else if(temp.equals("room_private_invite"))
						roomInviteAddress=parseText(reader);
					///////////
					else if(temp.equals("transfer_limit"))
					{
						transfer_limit=parseText(reader);

					}
					else if(temp.equals("data"))
					{
						data_url=parseText(reader);//带外传送方式的媒体服务器地址
					}
					else if(temp.equals("data_method"))
					{
						data_method=parseText(reader);//数据传输的方法
					}
					else if(temp.equals("capability"))
						capability=parseText(reader);
					////////
					else
						parseIgnore(reader);
				}
			}
			else if(element.equals("transparent"))
			{
				//v5.5修改
				parseIgnore(reader);
				String allString=reader.xml_string;
				int startIndex=allString.indexOf("<transparent>")+"<transparent>".length();
				int endIndex=allString.indexOf("</transparent>");
				transparent=allString.substring(startIndex,endIndex);
				//
			}	
			else 
				parseIgnore(reader);
		}

		if(noticeSystemVec!=null&&noticeSystemVec.size()>0)
		{
			for(int i=noticeSystemVec.size()-1;i>=0;i--)//将system的notice插入在前面
			{
				noticeVec.insertElementAt(noticeSystemVec.elementAt(i),0);
			}
		}
		parserNotifier.receiveDispatch(result,noticeVec,force,url,version,id,password,ip,ua,carrier,adurl,ad_interval,zwp,fpage,
				add_friend,room_name,room_search,hotmail,spaces,search,roomCreateAddress,roomInviteAddress,transparent,transfer_limit,data_url,data_method,capability);
	}
	
	public void parseJabberXml(XmlReader reader, String cmdType)throws Exception 
	{
		while (reader.next() == XmlReader.START_TAG)
		{
			String element = reader.getName();
			if (element == null)
			{
				return;
			}

			if (element.equals("sid"))
			{
				String sid = parseText(reader);
				parserNotifier.receiveSessionId(sid);

			} 
			else if (element.equals("jerror"))
			{
				String errorMsg = parseText(reader);
				parserNotifier.receiveLoginJerror(errorMsg,null);

			} else if (element.equals("message"))
			{

				if (cmdType.equals("cache"))
				{
					parserNotifier.setCacheStatus(false);
				}
				parseMessage(reader);
			} 
			else if (element.equals("presence"))
			{
				if (cmdType.equals("cache"))
				{
					parserNotifier.setCacheStatus(false);
				}
				parsePresence(reader);
			} 
			else if (element.equals("iq"))
			{
				if (cmdType.equals("cache"))
				{
					parserNotifier.setCacheStatus(false);
				}
				parseIq(reader);
			} else
			{
				parseIgnore(reader);
			}
		}
	}
	
	public String parseText(XmlReader reader) throws Exception 
	{
		String endTagName = reader.getName();
		StringBuffer str = new StringBuffer("");
		int t = reader.next();
		while (!endTagName.equals(reader.getName())) 
		{
			if (t == XmlReader.TEXT) {
				str.append(reader.getText());
			}
			t = reader.next();
		}
		return str.toString();
	}
	
	public void parseIgnore(XmlReader reader) throws Exception 
	{
		int x;
		while ((x = reader.next()) != XmlReader.END_TAG) {
			if (x == XmlReader.START_TAG) {
				parseIgnore(reader);
			}
		}
	}
	
	//#ifdef support_groupmessage
	/**收到用户加入或离开多人会话*/
	private void parseDialog(XmlReader reader,
			String sid, String action) throws Exception
	{ //action=’加入enter|离开leave’
		String[][] node=null;
		Vector vc=new Vector();
		while (reader.next() == XmlReader.START_TAG)
		{   
			String jid = null;//好友是jid
			String imid=null;//陌生人是imid
			if (reader.getName().equals("participant"))
			{
				jid = reader.getAttribute("jid");
				imid=reader.getAttribute("imid");
				vc.addElement(new String[]{jid,imid});				
				reader.next();
			} else
			{
				parseIgnore(reader);
			}
		}
		node=new String[vc.size()][2];
		vc.copyInto(node);
		parserNotifier.receiveParticipant(sid,action,node);
	}
	//#endif
	
	//#ifdef support_head_image
	/**获取其它联系人的头像*/
	private void parseContactPortrait(XmlReader reader) throws Exception
	{
		String jid=null;
		String imageData = null;// Base64编码头像文件内容
		String portraithash=null;//头像标识
		String url=null;
		while (reader.next() == XmlReader.START_TAG)
		{
			if(reader.getName().equals("item"))
			{
				jid = reader.getAttribute("jid");
				while (reader.next() == XmlReader.START_TAG)
				{
					if(reader.getName().equals("portrait"))
					{
						while (reader.next() == XmlReader.START_TAG)
						{	
							if(reader.getName().equals("data"))
							{
								//String dataLength=reader.getAttribute("length");
						    	imageData = parseText(reader);
						    	//reader.next();
							}
							else if(reader.getName().equals("portraithash"))
							{
								portraithash=parseText(reader);
								if(portraithash.length()==0)
									portraithash=null;
							}
							else if(reader.getName().equals("url"))
							{
								url=parseText(reader);
								if(url.length()==0)
									url=null;
							}
							else
								parseIgnore(reader);
						}
					}
					else
						parseIgnore(reader);
				}	
			}
			else
				parseIgnore(reader);
		}
		parserNotifier.receiveContactPortrait(jid,portraithash,imageData,url);
	}
	//#endif
	

}
