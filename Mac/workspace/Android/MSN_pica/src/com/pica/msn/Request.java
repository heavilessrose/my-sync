package com.pica.msn;

import java.io.ByteArrayOutputStream;

public class Request
{
    
	public String command="";
	public String urlPage="";
	public String content="";
	public ByteArrayOutputStream bytearrayoutputstream=null;
	public byte type;
	
	String id="",action="",target="",param0="",src="",dbid="",sale;
	public String imid;
	
	/**��ͨjabber��*/
	public Request(String command,String urlPage,String content)
	{
		this.command=command;
		this.urlPage=urlPage;
		this.content=content;
		this.type = MSNApplication.JABBER_REQUEST;
	}
	
	/**��wap*/
	public Request(String urlPage, ByteArrayOutputStream stream)
	{
	    this.urlPage=urlPage;
	    this.bytearrayoutputstream = stream;
	    this.type = MSNApplication.ZIPWAP_REQUEST; 
	}
	
	/**��¼ҳ����*/
	public Request(String urlPage)
	{
	    this.urlPage=urlPage;
	    this.type = MSNApplication.AD_LOGIN_REQUEST;
	}
	
	/**�����ͼƬ��ȡ*/
	public Request(String imid,String url,byte type)
	{
		this.imid=imid;
		this.urlPage=url;
		this.type=type;
	}
	
	/**�����ͳ�ư� |������ذ�*/
	public Request(String content,byte type)
	{
		this.urlPage="/advjava";
		this.content=content;
		this.type=type;
	}
	
	/**��ϵ��ҳ��ҳ�Ź��*/
	public Request(byte type,String urlPage,String id,String action,String target
			,String param0,String src,String dbid,String sale)
	{
	    this.urlPage=urlPage;
		this.id=id;
		this.action=action;
		this.target=target;
		this.param0=param0;
		this.src=src;
		this.dbid=dbid;
		this.sale=sale;
		this.type=type;
	}

	
}

