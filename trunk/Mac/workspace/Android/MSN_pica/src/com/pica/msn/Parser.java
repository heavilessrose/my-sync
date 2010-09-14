package com.pica.msn;

import java.util.*;

import android.util.Log;




/**
 * �����������ڽ�����������Ӧ��XML���ݡ����������ݺ󣬲����ײ��¼���
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
		String status = reader.getAttribute("status");//����ʧ��ʱ�����
		String sid=reader.getAttribute("sid");
		String to=reader.getAttribute("to");
		String name=reader.getAttribute("name");
		String body = null, reason = null, stamp = null;
		boolean nudge=false;//������
		
		String transferID=reader.getAttribute("transferid");//������������ʶ������ļ�
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
				reason = parseText(reader);//ʧ�ܵ�ԭ��
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
		//android��ʱ�������ڴ�
		//if(isOutOfMemory())
		//	return;
		/////////
		if(from!=null&&from.indexOf(MSNApplication.CLUSTER_DOMAIN)!=-1)
		{
			//����Ⱥ�����ҵ���Ϣ��
			//parserNotifier.receiveClusterMessage(from,to,name,body,stamp);
		}
		else
		{	
			if (type != null&& type.equals("chat"))
				parserNotifier.receiveMessage(jid,imid,sid,body,stamp,nudge,status,reason,transfer_ftID);
			//#ifdef support_groupmessage
			//android��ʱ�����Ƕ��˶Ի�
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
		String phone = null, gleam = "0", url = null;// ����ϵ�˵Ŀռ��ַ
		String temp;
		Vector nodeVec = new Vector();
		while (reader.next() == XmlReader.START_TAG) 
		{
			temp = reader.getName();
			if (temp.equals("show")) {
				show = parseText(reader);// ״ֵ̬
				if (from.indexOf(MSNApplication.CLUSTER_DOMAIN) == -1)
					nodeVec.addElement("show");
			} else if (temp.equals("nickname")) {
				nickname = parseText(reader);// �ǳ�
				nodeVec.addElement("nickname");
			} else if (temp.equals("gbcode")) {
				gbcode = parseText(reader);// �ǳ�ǰ4���ַ���gb2312��
				nodeVec.addElement("gbcode");
			} else if (temp.equals("impresa")) {
				impresa = parseText(reader);// ������Ϣ��������
				nodeVec.addElement("impresa");
			} else if (temp.equals("portraithash")) {
				portraithash = parseText(reader);// ͷ���ʶ
				if (portraithash.length() == 0)
					portraithash = null;
				nodeVec.addElement("portraithash");
			}
			// else if(temp.equals("mobile"))
			// {
			// mobile=parseText(reader);//0���ƶ��û�|1�ƶ��û�
			// nodeVec.addElement("mobile");
			// }
			else if (temp.equals("spaces")) {
				gleam = reader.getAttribute("gleam");// 0û�и��£�Ĭ��|1�и���
				nodeVec.addElement("gleam");
				url = parseText(reader);// ����ϵ�˵Ŀռ��ַ
				nodeVec.addElement("spaces");
			} else if (temp.equals("phone")) {
				phone = parseText(reader);
				nodeVec.addElement("phone");
			} else if (temp.equals("name")) {
				name = parseText(reader);
			} else if (temp.equals("role"))// �����ҹ���Ա
			{
				role = parseText(reader);
			} else if (temp.equals("speak"))// �Ƿ���Ȩ���������з���
			{
				speak = parseText(reader);
			} else if (temp.equals("prvtmsg"))// �������Ƿ��������Ļ�
			{
				prvtmsg = parseText(reader);
			} else if (temp.equals("imid")) {
				imid = parseText(reader);
			} else if (temp.equals("error")) {
				errorText = parseText(reader);
			} else
				parseIgnore(reader);
		}
		if (from.indexOf(MSNApplication.CLUSTER_DOMAIN) != -1)// �й������ҵĲ���
			;
			//parserNotifier.parseRoomPresence(type, from, to, name, role, speak,prvtmsg, imid, errorText);
		else
		{
			if (from != null && from.equals(MSNApplication.MSN_DOMAIN))
			{
				if (type != null && type.equals("unavailable"))
				{
					//receiveLogoutMsn();// �û�����ǳ� �ɹ�
					parserNotifier.receiveLogoutMsn();
				}
				else
				{
					parserNotifier.receiveModifyMyState(show);
					// �û��ı��Լ���״̬
				}
			} 
			else
			{
				//<presence type=��available' from=�� '78276741@pica/pica��/>
				if (type != null && type.equals("available") && from != null
						&& from.equals(MSNApplication.getApplication().GetPicaID() + "/pica"))
				{
					
					//��ʾ�ͻ��˷�������״̬  ���������سɹ�
					//���������ǿͻ��˷����¼MSN
					parserNotifier.receiveLoginPicaSuccess();
				} 
				else
				//��ϵ�˵�״̬�仯
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
//					{ // ����/ע�������ҳɹ�
//						parseJoinOrExitCluster(reader);
//					} 
//					else if (xmlns.equals("pica:cluster:role"))
//					{ // ���߳����߼��������
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
					deviceId = reader.getAttribute("deviceid");//�����deviceid�ֶΣ����Ǳ��ر����
					if (xmlns.equals("vcard-temp"))//��ȡ�û���������
					{
						parseVcardTemp(reader,resourceId);
					}
//					else if (xmlns.equals("pica:cluster:roomlist"))// �����������б�
//						parseChatroom(reader);
					else if (xmlns.equals("jabber:iq:roster"))
					{
						if(from.equals(MSNApplication.MSN_DOMAIN))//"msn.pica"
						{
							if(action!=null&&action.equals("getportrait"))
							{
								//#ifdef support_head_image
								//ȡͷ��
								parseContactPortrait(reader);
								//#else
								//# parseIgnore(reader);
								//#endif
							}
							else//ȡ��ϵ���б�
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
						if(addition!=null&&addition.equals("0"))//�滻
						{						
							if(!isDeleteAllRecord)//ɾ�����ر������ϵ�˷����б�
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
	
	/**��ȡ��ϵ���б�*/
	private void parseIqRoster(XmlReader reader,String action,String last, 
			String version,String deviceId,String addition)throws Exception
	{
		while (reader.next() == XmlReader.START_TAG)
		 {
		    String jid=null, imid=null,nickname=null, gbcode=null, block=null, 
	        sync=null, group=null, impresa=null, portraithash=null,
	        phone=null,gleam="0",url=null;//����ϵ�˵Ŀռ��ַ
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
					//����汾 gbcode������ô��
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
					if (temp.equals("group"))//����id,����id,
					{		
						//ÿ����ϵ�˿������ڶ�����飬�޷���id��Ĭ��Ϊ0
						group = parseText(reader);
						nodeVec.addElement("group");
					} 
					else if(temp.equals("impresa"))//������Ϣ��������
					{
						impresa=parseText(reader);
						nodeVec.addElement("impresa");
					}
					else if(temp.equals("portraithash"))
					{//ͷ���ʶ
						portraithash=parseText(reader);
						if(portraithash.length()==0)
							portraithash=null;
						nodeVec.addElement("portraithash");
					}
					else if(temp.equals("spaces"))
					{
						gleam = reader.getAttribute("gleam");//0û�и��£�Ĭ��|1�и���
						nodeVec.addElement("gleam");
						url=parseText(reader);//����ϵ�˵Ŀռ��ַ
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
			isDeleteAllRecord=false;//ֻ�����յ������ ����Ϊ��ϵ�˷������ϵ���б�ȫ��������
		}

	}

	/** �յ������Ϊ���ѵ���֤���� */
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
					//����Ϊ��չ��Ϣ ֻ������Ϊmsn.cn�û���Я��
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
	
	/**��ȡ�����б�*/
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
	
	
	/** ��ȡ�û��������� */
	private void parseVcardTemp(XmlReader reader,String resourceId)throws Exception
	{
		//���ĳ���ڵ㲻���ڣ���ʾ������û�и���
		//����Ҫ��һ������ ��ʾ��Щ�ڵ����
		String nickname =null,impresa=null;
		String portrait_data=null;
		String temp=null;
		Vector nodeVec=new Vector();
		String dataLength=null;//data����
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
	
	/** ��dispatch�ذ����н��� */
	public void parseDispatchResponse(String response, String urlPage,String content) throws Exception 
	{
		if (response == null || response.length() == 0|| !response.startsWith("<sso"))// �й��ƶ�����ҳ��
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
			else if (element.equals("sso"))// ����dispatch�ذ�
			{
				result = reader.getAttribute("result");
				parseDispatchSSOXml(reader, result);
			}
			//System.gc();
		}
	}
	
	/**�������*/
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
	
	
	/** ����dispatch�ذ� */
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
		String ua=null;//�ֻ�UA
		String carrier=null;//��Ӫ����Ϣ
		String adurl=null;//����������URL
		String ad_interval=null;//ȡ����ʱ����
		String zwp=null;//ZWP���ص�IP�Ͷ˿� �������û�У�����ֵΪ�գ���֧����WAP �����֧����WAP�������¸��ǿ��Ϊ��֧��
		String fpage=null;//��WAP����ҳ��ַ
		String add_friend=null;//msn.cn�û������ϵ�˵ĵ�ַ
		String room_name=null;//MSNȺ���������  �������û�л���Ϊ�գ���֧��MSNȺ����
		String room_search=null;//����MSNȺ�������ң��ĵ�ַ  
		String hotmail=null;//Hotmail�������ĵ�ַ   �������û�л���Ϊ�գ���֧������
		String spaces=null;//MSN spaces��ַ  �������û�л���Ϊ�գ���֧�ֿռ�
		String search=null;//MSN ������ַ  �������û�л���Ϊ�գ���֧������
		String roomInviteAddress=null;//��������Խ�Ⱥ�ĵ�ַ
		String roomCreateAddress=null;//�����Խ�Ⱥ�ĵ�ַ
		String transparent=null;

		String force=reader.getAttribute("force");
		
		//
		String transfer_limit=null;//�ļ����͵Ĵ�С���ƣ��ֽ����� ���û�д������ֵΪ�ջ���0�����趨��2M
		String data_url=null;//���⴫�ͷ�ʽ��ý���������ַ
		String data_method=null;//���ݴ���ķ���
		
		String capability=null;//�ͻ���֧������
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
						data_url=parseText(reader);//���⴫�ͷ�ʽ��ý���������ַ
					}
					else if(temp.equals("data_method"))
					{
						data_method=parseText(reader);//���ݴ���ķ���
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
				//v5.5�޸�
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
			for(int i=noticeSystemVec.size()-1;i>=0;i--)//��system��notice������ǰ��
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
	/**�յ��û�������뿪���˻Ự*/
	private void parseDialog(XmlReader reader,
			String sid, String action) throws Exception
	{ //action=������enter|�뿪leave��
		String[][] node=null;
		Vector vc=new Vector();
		while (reader.next() == XmlReader.START_TAG)
		{   
			String jid = null;//������jid
			String imid=null;//İ������imid
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
	/**��ȡ������ϵ�˵�ͷ��*/
	private void parseContactPortrait(XmlReader reader) throws Exception
	{
		String jid=null;
		String imageData = null;// Base64����ͷ���ļ�����
		String portraithash=null;//ͷ���ʶ
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
