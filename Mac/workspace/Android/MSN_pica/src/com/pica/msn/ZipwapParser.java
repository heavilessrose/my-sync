package com.pica.msn;

import java.io.DataInputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;
import android.util.Log;



public class ZipwapParser
{
	public StringBuffer htmlString;
	MSNApplication app;
	String kAlreadyElement = "kAlreadyElement";
	String kNeedProcElement = "kNeedProcElement";
	/**���ڻ��ͼƬ��Դ����ȷ��ַ����ΪbaseUrl��û�и��£���ҳ����سɹ��Ÿ��£�*/
	public String tempBaseUrl;
    public ZipwapParser() 
    {
		app = MSNApplication.getApplication();
    	if (app == null)
    	{
    		return;
    	}
    	htmlString = new StringBuffer();
    	tempBaseUrl = app.getBaseUrl(app.zwp_homePageAddress);
    }
    
	public  void parseElement(DataInputStream stream, int contentLength) throws Exception
	{
		int recvLength = 0; 
		int elementLength = 0;
		   
		try
		{   
		   	while(true) 
		   	{
		   		if(!app.bZwpLoad)
		   		{
		   			htmlString.delete(0, htmlString.length());
		   			return;
		   		}
			    short Header_id = stream.readShort();		    
			    elementLength = stream.readInt();
			    recvLength = recvLength + 6;			    
			    switch(Header_id) 
			    {    
			    	case 0x1000:
			    	    ZWP_PageStyle(stream,elementLength);
			    	    break;
			    	case 0x1001:
			    	    ZWP_FontStyle(stream,elementLength);
			    	    break;
			    	case 0x1002:
			    	    ZWP_ColorStyle(stream,elementLength);
			    	    break;
			    	case 0x1003:
			    	    ZWP_AlignmentStyle(stream,elementLength);
			    	    break;
			    	case 0x1004:
			    	    ZWP_SizeStyle(stream,elementLength);
			    	    break;
			    	case 0x1005:
			    	    ZWP_OfficeStyle(stream,elementLength);
			    	    break;
			    	case 0x2000:
			    	    ZWP_ResData(stream,elementLength);
			    	    break;
			    	case 0x2001:
			    	    ZWP_Column(stream,elementLength);
			    	    break;
			    	case 0x3000:
			    	    ZWP_Title(stream,elementLength);
			    	    break;
			    	case 0x3001:
			    	    ZWP_BR(stream,elementLength);
			    	    break;
			    	case 0x3002:
			    	    ZWP_HR(stream,elementLength);
			    	    break;
			    	case 0x3003:
			    	    ZWP_Marquee(stream,elementLength);
			    	    break;
			    	case 0x3004:
			    	    ZWP_Text(stream,elementLength);
			    	    break;
			    	case 0x3005:
			    	    ZWP_Link(stream,elementLength);
			    	    break;
			    	case 0x3006:
			    	    ZWP_LocalAnchor(stream,elementLength);
			    	    break;
			    	case 0x3007:
			    	    ZWP_Image(stream,elementLength);
			    	    break;
			    	case 0x3008:
			    		ZWP_AutoRefresh(stream,elementLength);
			    		break;
			    	case 0x4000:
			    	    ZWP_TextBox(stream,elementLength);
			    	    break;
			    	case 0x4001:
			    	    ZWP_SingleSelection(stream,elementLength);
			    	    break;
			    	case 0x4002:
			    	    ZWP_MultiSelection(stream,elementLength);
			    	    break;
			    	case 0x4003:
			    	    ZWP_Hidden(stream,elementLength);
			    	    break;
			    	case 0x4004:
			    	    ZWP_FileBrowser(stream,elementLength);
			    	    break;
			    	case 0x4005:
			    	    ZWP_Record(stream,elementLength);
			    	    break;
			    	case 0x4006:
			    	    ZWP_Camera(stream,elementLength);
			    	    break;
			    	case 0x4007:
			    	    ZWP_ValueResetButton(stream,elementLength);
			    	    break;
			    	case 0x4008:
			    	    ZWP_ResetButton(stream,elementLength);
			    	    break;
			    	case 0x4009:
			    	    ZWP_SubmitButton(stream,elementLength);
			    	    break;
			    	case 0x400A://����ѡ��    
			    		ZWP_Checkbox(stream,elementLength);
			    	    break;
			    	case 0x5000:
			    	    ZWP_ExtCommand(stream,elementLength);
			    	    break;
			    	default:
			    		return;
				}  		    
			    recvLength = recvLength + elementLength;		    
			    //���ڴ�С��20kʱ֪ͨ�ڴ����
			    if(Runtime.getRuntime().freeMemory()<20000)
			    {
			    	System.gc();
			    }				
			    if(recvLength >= contentLength)
			    {		    	
			        return;
			    }
		   	}
	   }
	   catch(Exception e)
	   {
		   Log.e("Parse Error", e.getMessage());
	       e.printStackTrace();
	       throw e;
	   }
	}
    
	//����
	public static void ZWP_Title(DataInputStream stream,int elementLength) throws IOException
	{
	    String title = stream.readUTF();
	    elementLength=elementLength-(title.getBytes("UTF-8").length+2);
	    if(elementLength>0)
	    {
	    	stream.skip(elementLength);
	    }
	    	
	}
	//ͼ��
	public void ZWP_Image(DataInputStream stream,int elementLength) throws IOException
	{
        String text = stream.readUTF();
        int id = stream.readInt();
        short width = stream.readShort();
        short height = stream.readShort();
        String href_orifinal_img = stream.readUTF();
        href_orifinal_img = app.getAbsoluteUrl(href_orifinal_img, tempBaseUrl);
        String href = stream.readUTF();
        
        byte target_type = stream.readByte();    
        String advist = stream.readUTF();
        elementLength=elementLength-(9+text.getBytes("UTF-8").length+2+href_orifinal_img.getBytes("UTF-8").length+2+href.getBytes("UTF-8").length+2+advist.getBytes("UTF-8").length+2);
	    if(elementLength>0)
	    {
	    	stream.skip(elementLength);
	    }
		if (null == text) 
		{
			text = "";
		}

		if (0 == width ||0 == height)
		{
			if (advist != null && advist.length() > 0)
			{
				if( (href.equals("")) || (href.length() == 0) )
				{
					htmlString.append("<a \"link***" + Integer.toString(target_type) + "***" + advist
										+"***" +"\">" + "<img src='" + href_orifinal_img +"' alt='" + text + "'></a>");
				}
				else
				{
					htmlString.append("<a href=\"link***" + Integer.toString(target_type) + "***" + advist +"***"+ href
										+"\">" + "<img src='" + href_orifinal_img +"' alt='" + text + "'></a>");
				}
				
			}
			else
			{
				if( (href.equals("")) || (href.length() == 0) )
				{
					htmlString.append("<a \"link***" + Integer.toString(target_type) +"***" +"\">"
										+ "<img src='" + href_orifinal_img +"' alt='" + text + "'></a>");
//					htmlString.append("<img src='" + href_orifinal_img +"' alt='" + text + "'></a>");
				}
				else
				{
					htmlString.append("<a href=\"link***" + Integer.toString(target_type) +"***"+ href
									+"\">" + "<img src='" + href_orifinal_img +"' alt='" + text + "'></a>");
//					htmlString.append("<a href='" + href + "'>" + "<img src='" + href_orifinal_img +"' alt='" + text
//										+ "'></a>");
				}
				
			}
		}
		else 
		{
			if (advist != null && advist.length() > 0)
			{
				if( (href.equals("")) || (href.length() == 0) )
				{
					htmlString.append("<a \"link***" + Integer.toString(target_type) + "***" + advist +"***" 
										+"\">" + "<img src='" + href_orifinal_img +"' alt='" + text + "' width='" 
										+ Integer.toString(width) + "' height='" + Integer.toString(height) +"'></a>");
				}
				else
				{
					htmlString.append("<a href=\"link***" + Integer.toString(target_type) + "***" + advist + "***" + href
										+"\">" + "<img src='" + href_orifinal_img +"' alt='" + text + "' width='" 
										+ Integer.toString(width) + "' height='" + Integer.toString(height) +"'></a>");
				}
				
			}
			else
			{
				if( (href.equals("")) || (href.length() == 0) )
				{
					htmlString.append("<a \"link***" + Integer.toString(target_type) +"***" 
							+"\">" + "<img src='" + href_orifinal_img +"' alt='" + text + "' width='" 
							+ Integer.toString(width) + "' height='" + Integer.toString(height) +"'></a>");
//					htmlString.append("<a href='" + "'>" + "<img src='" + href_orifinal_img +"' alt='" + text
//							+ "' width='" + Integer.toString(width) + "' height='" + Integer.toString(height) +"'></a>");
				}
				else
				{
					htmlString.append("<a href=\"link***" + Integer.toString(target_type) + "***" + href
							+"\">" + "<img src='" + href_orifinal_img +"' alt='" + text + "' width='" 
							+ Integer.toString(width) + "' height='" + Integer.toString(height) +"'></a>");
//					htmlString.append("<a href='" + href + "'>" + "<img src='" + href_orifinal_img +"' alt='" + text
//							+ "' width='" + Integer.toString(width) + "' height='" + Integer.toString(height) +"'></a>");
				}
			}
		}
	}
	
	/**�Զ�ˢ��*/
	public  void ZWP_AutoRefresh(DataInputStream stream,int elementLength)throws IOException
	{
		String hrel;
		hrel=stream.readUTF();
		short timeout=stream.readShort();
		elementLength=elementLength-(hrel.getBytes("UTF-8").length+2+2);
	    if(elementLength>0)
	    	stream.skip(elementLength);
	}
		
	//����
	public void ZWP_BR(DataInputStream stream,int elementLength) throws IOException
	{
		htmlString.append("<br/>");
	}
	
	//���� URL
	public void ZWP_Link(DataInputStream stream,int elementLength) throws IOException
	{
        String text = stream.readUTF();
        byte target_type = stream.readByte();
        String url = stream.readUTF();
        String advist = stream.readUTF();
	    elementLength=elementLength-(1+text.getBytes("UTF-8").length+2+url.getBytes("UTF-8").length+2+advist.getBytes("UTF-8").length+2);
	    if(elementLength>0)
	    {
	    	stream.skip(elementLength);
	    }
	    if(text.length()==0)
	    {
	    	return;
	    }
		if (url == null)
		{
			if (text != null)
			{
				htmlString.append("<a>" + text +"</a>");
			}	
		}
		else
		{
			if (text == null)
			{
				if (advist != null && advist.length() > 0)
				{
					htmlString.append(" <a href=\"link***" + Integer.toString(target_type) + "***" + advist +"***"+ url +"\"/> ");
				}
					
				else
				{
					htmlString.append(" <a href=\"link***" + Integer.toString(target_type) +"***"+ url +"\"/> ");
//					htmlString.append(" <a href='" + url + "'/> ");
				}	
			}
			else
			{
				if (advist != null && advist.length() > 0)
				{
					htmlString.append(" <a href=\"link***" + Integer.toString(target_type) + "***" + advist +"***"+ url +"\">" + text + "</a> ");
				}	
				else
				{
					htmlString.append(" <a href=\"link***" + Integer.toString(target_type) +"***"+ url +"\">" + text + "</a> ");
//					htmlString.append(" <a href='" + url + "'>" + text + "</a> ");
				}	
			}
		}
	}
	
	//�ָ���
	public static void ZWP_HR(DataInputStream stream,int elementLength) throws IOException
	{
	    stream.readByte();
	    stream.readByte();
	    elementLength=elementLength-2;
	    if(elementLength>0)
	    	stream.skip(elementLength);
	}
	//���ʾ
	public static void ZWP_Marquee(DataInputStream stream,int elementLength) throws IOException
	{
	    String text=stream.readUTF();
	    String href=stream.readUTF();
	    stream.readByte();
	    stream.readByte();
	    stream.readShort();
	    elementLength=elementLength-(4+text.getBytes("UTF-8").length+2+href.getBytes("UTF-8").length+2);
	    if(elementLength>0)
	    	stream.skip(elementLength);
	}
	//��ͨ�ı�
	public void ZWP_Text(DataInputStream stream,int elementLength) throws IOException
	{
        String text = stream.readUTF();
		elementLength=elementLength-(text.getBytes("UTF-8").length+2);
		if(elementLength>0)
		   stream.skip(elementLength);
		if(text.length()==0)
			return;
		
		htmlString.append(text);		
	}
	
    //��Դ����
	public static void ZWP_ResData(DataInputStream stream,int elementLength) throws IOException
	{
        int id = stream.readInt();
        String name = stream.readUTF();
        String format = stream.readUTF();
        int data_len = stream.readInt(); 
        byte[] Imgtmp = new byte[data_len]; //����ͼ�����ݳ���
        elementLength=elementLength-(8+name.getBytes("UTF-8").length+2+format.getBytes("UTF-8").length+2);
        for (int i = 0; i < data_len; i++) 
        {
            Imgtmp[i] = stream.readByte();
            elementLength=elementLength-1;
        }
	    if(elementLength>0)
	    	stream.skip(elementLength);
	    
	    if (name != null && format != null && Imgtmp != null)
		{
			if (format.equals("png") || format.equals("bmp")
				|| format.equals("gif") || format.equals("jpg"))
			{
				//NSString *filePath = [NSTemporaryDirectory() stringByAppendingPathComponent:name];
				//[data writeToFile:filePath atomically:YES];
//				MYLog(@"sData>>>%@",filePath);
			}
		}
	    
	    
	}
	
	//����ê��
	public static void ZWP_LocalAnchor(DataInputStream stream,int elementLength) throws IOException
	{
	    String name=stream.readUTF();
	    elementLength=elementLength-(name.getBytes("UTF-8").length+2);
	    if(elementLength>0)
	    	stream.skip(elementLength);
	}
	
	//���ı������
	public void ZWP_TextBox(DataInputStream stream,int elementLength) throws IOException
	{
        byte id = stream.readByte();
        String name = stream.readUTF();
        String value = stream.readUTF();
        byte password_mask = stream.readByte();
        byte lines = stream.readByte();
        int size = stream.readInt();
	    elementLength=elementLength-(7+name.getBytes("UTF-8").length+2+value.getBytes("UTF-8").length+2);
	    if(elementLength>0)
	    	stream.skip(elementLength);
	    
		String iType = null;
		if( 35 == password_mask || 42 == password_mask)
		{
			iType = "password";
		}else {
			iType = "text";
		}

		StringBuffer textBoxString = new StringBuffer();
		if (lines>1)
		{
			textBoxString.append("<textarea type='"+ iType +"' name='"+ name +"' value='"+ value +"' size='"+ Integer.toString(size) +"' rows='"+ lines +"' />");			
		}else {
			textBoxString.append("<input type='"+ iType +"' name='"+ name +"' value='"+ value +"' />");			
		}

		addElementIntoForm(id, textBoxString.toString(),htmlString);
	    
	}
	
	/**
	 * ���ܣ�  �ѿؼ������Ӧ�ı���
	 * @param id   �ؼ������ı�id
	 * @param elementString   �ؼ���html�ı�
	 * @param htmlString
	 */
	private void addElementIntoForm(byte id, String elementString, StringBuffer htmlString)
	{
		//�����һ��<form>��ǩ
		int formBeginIndex = htmlString.toString().toLowerCase().lastIndexOf(("<form id=\"").toLowerCase());
		//���û��<form>��ǩ�������<form id="xxx">��ǩ
		if(-1 == formBeginIndex)
		{
			htmlString.append("<form id=\"" + id + "\">" + elementString);
		}
		else
		{
			int idEndIndex = htmlString.toString().toLowerCase().indexOf('"', formBeginIndex + 10);
			//����form��idֵ ��+10�Ǽ���<form id="�ĳ���
			String formId = htmlString.toString().substring(formBeginIndex + 10, idEndIndex);
			//�Ƚ��ҵ���form��Id�Ƿ�Ϳؼ���formId��ͬ����ͬ�������У���ͬ������<form>��װ�ؼ�
			if(formId.equals(Byte.toString(id)))
			{
				htmlString.append(elementString);
			}
			else
			{
				htmlString.append("<form id=\"" + id + "\">" + elementString);
			}
		}
	}
	
	//����ѡ/�б��
	public void ZWP_SingleSelection(DataInputStream stream,int elementLength) throws IOException
	{	
		byte id = stream.readByte();
		String name = stream.readUTF();
		String value = stream.readUTF();
		stream.readByte();
		stream.readByte();
		byte items = stream.readByte();
		String text[] = new String[items];
		String itemValue[] = new String[items];
		
		elementLength=elementLength-(4+name.getBytes("UTF-8").length+2+value.getBytes("UTF-8").length+2);
		for(int i=0;i<items;i++)
		{
			text[i] = stream.readUTF();
			itemValue[i] = stream.readUTF();
			elementLength=elementLength-(text[i].getBytes("UTF-8").length+2+itemValue[i].getBytes("UTF-8").length+2);
		}
		if(elementLength>0)
			stream.skip(elementLength);
		if (value.length() == 0)
			value = itemValue[0];
		
		StringBuffer selectString = new StringBuffer();
		{
			selectString.append("<select name='"+ name +"' single>");
	
			for(int i = 0; i < text.length; i++)
			{
				if(text[i] == value)
				{
					selectString.append("<option value='"+ itemValue[i] +"' selected>"+ text[i] +"</option>");
				}
				else
				{
					selectString.append("<option value='"+ itemValue[i] +"'>"+ text[i] +"</option>");				
				}
			}
			selectString.append("</select>");		
		}
		addElementIntoForm(id, selectString.toString(), htmlString);
		    	    
	}
	//����ѡ��
	public static void ZWP_MultiSelection(DataInputStream stream,int elementLength) throws IOException
	{
		stream.readByte();
		String name=stream.readUTF();
		String value=stream.readUTF();
		stream.readByte();
		stream.readByte();
		String text=stream.readUTF();
		String value_text=stream.readUTF();
		elementLength=elementLength-(3+name.getBytes("UTF-8").length+2+value.getBytes("UTF-8").length+2+text.getBytes("UTF-8").length+2+value_text.getBytes("UTF-8").length+2);
		if(elementLength>0)
		{
			stream.skip(elementLength);
		}   	
	}
	
	//������Ԫ��
	public void ZWP_Hidden(DataInputStream stream,int elementLength) throws IOException
	{
		byte form_id=stream.readByte();
		String name=stream.readUTF();
		String value=stream.readUTF();
		elementLength=elementLength-(1+name.getBytes("UTF-8").length+2+value.getBytes("UTF-8").length+2);
		if(elementLength>0)
		{
			stream.skip(elementLength);
		}
		String hiddenString = "<input type='hidden' name='"+ name +"' value='"+ value +"'/>";	
		addElementIntoForm(form_id, hiddenString, htmlString);
	}
	
	//���ϴ���������ؼ�
	public static void ZWP_FileBrowser(DataInputStream stream,int elementLength) throws IOException
	{
		stream.readByte();
		String name=stream.readUTF();
		String value=stream.readUTF();
		String caption=stream.readUTF();
		elementLength=elementLength-(1+name.getBytes("UTF-8").length+2+value.getBytes("UTF-8").length+2+caption.getBytes("UTF-8").length+2);
		if(elementLength>0)
		{
			stream.skip(elementLength);
		}	
	}
	
	//��¼����ť
	public static void ZWP_Record(DataInputStream stream,int elementLength) throws IOException
	{
		stream.readByte();
		String name=stream.readUTF();
		String value=stream.readUTF();
		String caption=stream.readUTF();
		elementLength=elementLength-(1+name.getBytes("UTF-8").length+2+value.getBytes("UTF-8").length+2+caption.getBytes("UTF-8").length+2);
		if(elementLength>0)
		{
			stream.skip(elementLength);
		}	
	}
	
	//�����հ�ť
	public static void ZWP_Camera(DataInputStream stream,int elementLength) throws IOException
	{
		stream.readByte();
		String name=stream.readUTF();
		String value=stream.readUTF();
		String caption=stream.readUTF();
		elementLength=elementLength-(1+name.getBytes("UTF-8").length+2+value.getBytes("UTF-8").length+2+caption.getBytes("UTF-8").length+2);
		if(elementLength>0)
		{
			stream.skip(elementLength);
		}
	}
	
	//��Ԫ��ֵ�ظ���ť
	public static void ZWP_ValueResetButton(DataInputStream stream,int elementLength) throws IOException
	{
		stream.readByte();
		String caption=stream.readUTF();
		String name_to_reset=stream.readUTF();
		elementLength=elementLength-(1+caption.getBytes("UTF-8").length+2+name_to_reset.getBytes("UTF-8").length+2);
		if(elementLength>0)
		{
			stream.skip(elementLength);
		}	
	}
	
	//�����ð�ť
	public static void ZWP_ResetButton(DataInputStream stream,int elementLength) throws IOException
	{
		stream.readByte();
		String caption = stream.readUTF();
		String name = stream.readUTF();
		String value = stream.readUTF();
	      
		elementLength = elementLength-(1+caption.getBytes("UTF-8").length + 2
	    		  			+ name.getBytes("UTF-8").length + 2	+ value.getBytes("UTF-8").length + 2);
		if(elementLength > 0)
		{
			stream.skip(elementLength);
		}	
	}
	
	//���ύ��ť
	public void ZWP_SubmitButton(DataInputStream stream,int elementLength) throws IOException
	{
		Map<String, String> submitDict;
		Map<String, String> needProcDict;
		byte formId = stream.readByte();
		String caption = stream.readUTF();
		String href = stream.readUTF();
		byte method = stream.readByte();
		elementLength = elementLength - (2 + caption.getBytes("UTF-8").length+ 2+href.getBytes("UTF-8").length+2);
		if(0 == elementLength)//Ϊ������Ӧ������
		{
			return;
		}
		String name = stream.readUTF();//���������û���ð�ťʱ��Ҫ��FORM����һ���ṩ
		String value = stream.readUTF();//�ύ����ʱ��name��Ӧ��ֵ
		byte type = stream.readByte();//�ύ��������
				//  0: ������ͨ������form��
				//  1: ����wap1.0/1.1���棬ͨ��Archor��ʵ���ύ�ķ�ʽ
		String submit_data = stream.readUTF();//��Ҫ�ύ������ ֻ���type=1ʱ��Ч
		
		elementLength = elementLength - (1 + name.getBytes("UTF-8").length + 2
						+ value.getBytes("UTF-8").length + 2 + submit_data.getBytes("UTF-8").length + 2);
		if(elementLength>0)
		{
			stream.skip(elementLength);
		}
		
		StringBuffer formElementFromSubmitData = new StringBuffer();		
		Vector temp = new Vector();
		
		if(type == 0)//��ʾ����ͨ�ı�
		{			        	
        	String[] s = {name, value};
            temp.addElement(s);
		}
		else
		{
			if (submit_data != null && submit_data.length() != 0)
			{
				Map<String, Map<String, String>> originalDict = convertSubmitdataToDict(submit_data);
				submitDict = originalDict.get(kAlreadyElement);
				needProcDict = originalDict.get(kNeedProcElement);

				if ( (submitDict != null) && (submitDict.size()>0) )
				{
					String[] submitDictArray = new String[submitDict.size()];
					submitDict.keySet().toArray(submitDictArray);
					for(int n = 0; n < submitDict.size(); n++)
					{
						String key = submitDictArray[n];
						formElementFromSubmitData.append(" <input name='"+ key +"' type='hidden' value='"+ submitDict.get(key) +"' />");
					}
				}

				if ( (needProcDict != null) && (needProcDict.size()>0))
				{
					String[] needProcDictArray = new String[needProcDict.size()];
					needProcDict.keySet().toArray(needProcDictArray);
					for (int m = 0; m < needProcDict.size(); m++)
					{
						String keyString = needProcDictArray[m];
						int theRange = htmlString.lastIndexOf(needProcDict.get(keyString));
						if (theRange != -1)
						{
							htmlString.delete(theRange, theRange + needProcDict.get(keyString).length());
							htmlString.insert(theRange, keyString);
						}
					}
				}
			}
		}


		String iMethod = null;
		href = app.getAbsoluteUrl(href, tempBaseUrl);
		StringBuffer hrefBuffer = new StringBuffer(href);
		switch (method)
		{
			case 0:
			default:
				iMethod = "get";
				// Parsing href and fill up input tags.
				int indexRange = href.indexOf("?");
				if (indexRange != -1)
				{
					String tagsString = href.substring(indexRange + 1);
					hrefBuffer.delete(indexRange, hrefBuffer.length());
					Map<String, Map<String, String>> testDict = convertSubmitdataToDict(tagsString);
					Map<String, String> alreadyDict  = testDict.get(kAlreadyElement);
					if( (alreadyDict != null) && (alreadyDict.size() > 0) )
					{
						String[] alreadyDictArray = new String[alreadyDict.size()];
						alreadyDict.keySet().toArray(alreadyDictArray);
						for (int l = 0; l < alreadyDict.size(); l++)
						{
							String key = alreadyDictArray[l];
							formElementFromSubmitData.append(" <input name='"+ key +"' type='hidden' value='"+ alreadyDict.get(key) +"' />");
						}	
					}
							
				}
				break;
			case 1:
				iMethod = "post";
				int index = href.indexOf("?");
				if (index != -1)
				{
					String tagsString = href.substring(index + 1);
					hrefBuffer.delete(index, hrefBuffer.length());
					Map<String, Map<String, String>> testDict = convertSubmitdataToDict(tagsString);
					Map<String, String> alreadyDict  = testDict.get(kAlreadyElement);
					if( (alreadyDict != null) && (alreadyDict.size() > 0) )
					{
						String[] alreadyDictArray = new String[alreadyDict.size()];
						alreadyDict.keySet().toArray(alreadyDictArray);
						for (int l = 0; l < alreadyDict.size(); l++)
						{
							String key = alreadyDictArray[l];
							formElementFromSubmitData.append(" <input name='"+ key +"' type='hidden' value='"+ alreadyDict.get(key) +"' />");
						}
					}
					
				}
				break;
			case 2:
				iMethod = "put";
				int i = href.indexOf("?");
				if (i != -1)
				{
					String tagsString = href.substring(i + 1);
					hrefBuffer.delete(i, hrefBuffer.length());
					Map<String, Map<String, String>> testDict = convertSubmitdataToDict(tagsString);
					Map<String, String> alreadyDict  = testDict.get(kAlreadyElement);
					if( (alreadyDict != null) && (alreadyDict.size() > 0) )
					{
						String[] alreadyDictArray = new String[alreadyDict.size()];
						alreadyDict.keySet().toArray(alreadyDictArray);
						for (int l = 0; l < alreadyDict.size(); l++)
						{
							String key = alreadyDictArray[l];
							formElementFromSubmitData.append(" <input name='"+ key +"' type='hidden' value='"+ alreadyDict.get(key) +"' />");
						}
					}
					
				}
				break;
		}
		app.urlAndMethod.put(hrefBuffer.toString(), iMethod);
		
		formElementFromSubmitData.append(" <input type='submit' value='"+caption+"' /></form>");
		addElementIntoForm(formId, formElementFromSubmitData.toString(), htmlString);
				
		int formBeginIndex = htmlString.toString().toLowerCase().lastIndexOf(("<form id=\"").toLowerCase());
		int idEndIndex = htmlString.toString().toLowerCase().indexOf('"', formBeginIndex + 10);
		//����form������
		htmlString.insert(idEndIndex + 1, " action='"+ hrefBuffer.toString() +"'" /*+"' method='"+ iMethod +"'"*/);		
	}
	
	public static void ZWP_Checkbox(DataInputStream stream,int elementLength)throws IOException
	{
		/*����ԭҳ����û�д�value��checkbox��������Ӧ����valueֵ��Ϊ��on����
		ͬʱΪ�˱�֤�ϴ����ݵĿɿ��ԣ�
		�ͻ����жϣ�����˸�ѡ���valueֵΪ�գ�Ҳ������Ϊ��on��*/
		byte form_id=stream.readByte();
		String name=stream.readUTF();
		String value=stream.readUTF();
		if(value==null||value.length()==0)
		{
			value="on";
		}	
		byte select=stream.readByte();
		elementLength=elementLength-(2+name.getBytes("UTF-8").length+2+value.getBytes("UTF-8").length+2);
		if(elementLength>0)
		{
			 stream.skip(elementLength);
		}  
	}
	
	//ҳ����
	public static void ZWP_PageStyle(DataInputStream stream,int elementLength) throws IOException
	{	
	    stream.readByte();
	    stream.readInt();
	    stream.readInt();
	    stream.readInt();
	    stream.readByte();
	    stream.readByte();
	    stream.readByte();
		
	    elementLength=elementLength-16;
	    if(elementLength>0)
	    {
	    	stream.skip(elementLength);
	    }	
	}
	
	//������
	public static void ZWP_FontStyle(DataInputStream stream,int elementLength) throws IOException
	{
	    stream.readByte();
	    stream.readByte();
		
	    elementLength=elementLength-2;
	    if(elementLength>0)
	    {
	    	stream.skip(elementLength);
	    }	
	}
	
	//��ɫ���
	public static void ZWP_ColorStyle(DataInputStream stream,int elementLength) throws IOException
	{
	    stream.readInt();
	    stream.readInt();
		
	    elementLength=elementLength-8;
	    if(elementLength>0)
	    {
	    	stream.skip(elementLength);
	    }	
	}
	
	//������
	public static void ZWP_AlignmentStyle(DataInputStream stream,int elementLength) throws IOException
	{
	    stream.readByte();
	    stream.readByte();
	    stream.readByte();
		
	    elementLength=elementLength-3;
	    if(elementLength>0)
	    {
	    	stream.skip(elementLength);
	    }	
	}
	
	//�ߴ���
	public static void ZWP_SizeStyle(DataInputStream stream,int elementLength) throws IOException
	{
	    stream.readShort();
	    stream.readShort();
		
	    elementLength=elementLength-4;
	    if(elementLength>0)
	    {
	    	stream.skip(elementLength);
	    }	
	}
	
	//ƫ�Ʒ��
	public static void ZWP_OfficeStyle(DataInputStream stream,int elementLength) throws IOException
	{
		stream.readShort();
		
	    elementLength=elementLength-2;
	    if(elementLength>0)
	    {
	    	stream.skip(elementLength);
	    }	
	}
	
	//��չ����Ԫ��
	public static void ZWP_ExtCommand(DataInputStream stream,int elementLength) throws IOException
	{
	    stream.readByte();
	    stream.readByte();
	    String caption=stream.readUTF();
	    String value=stream.readUTF();
	    elementLength=elementLength-(2+caption.getBytes("UTF-8").length+2+value.getBytes("UTF-8").length+2);
	    if(elementLength>0)
	    {
	    	stream.skip(elementLength);
	    }	
	}
	
	//��ҳ����ĿԪ��
	public static void ZWP_Column(DataInputStream stream,int elementLength) throws IOException
	{
	  String icon_format=stream.readUTF();  
	  stream.readByte();
	  stream.readByte();
	  stream.readInt();
	  stream.readByte();
	  
	  String title=stream.readUTF();
	  String description=stream.readUTF();
	  String uri=stream.readUTF();
	  elementLength=elementLength-(icon_format.getBytes("UTF-8").length+2+7+title.getBytes("UTF-8").length+2+description.getBytes("UTF-8").length+2+uri.getBytes("UTF-8").length+2);
	  if(elementLength>0)
	  {
		  stream.skip(elementLength);
	  }	
	}
	
	private Map<String, Map<String, String>> convertSubmitdataToDict(String submitString)
	{
		Map<String, Map<String, String>> restDict = new HashMap<String, Map<String, String>>(2);
		Map<String, String> alreadyDict =  new HashMap<String, String>();
		Map<String, String> needProcDict = new HashMap<String, String>();

		try
		{
			String[]keyValues = MsnUtil.split(submitString, '&');
			String keyValueString = null;
			for (int i = 0; i < keyValues.length; i++)
			{
				keyValueString = keyValues[i];
				//��ֵ��
				String[] keyValue = MsnUtil.split(keyValueString, '=');
				if (-1 == keyValueString.indexOf("$"))
				{
					if(1 == keyValue.length)
					{
						alreadyDict.put(keyValue[0], "");
					}
					else
					{
						alreadyDict.put(keyValue[0], keyValue[1]);
					}
					
				}
				else
				{
					String tmpObj = keyValue[1];
					tmpObj = checkCharacter(tmpObj);
					//tmpObj = [tmpObj stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"$()"]];
					needProcDict.put(keyValue[0], tmpObj);
				}
			}
			restDict.put(kAlreadyElement, alreadyDict);
			restDict.put(kNeedProcElement, needProcDict);
		}
		catch (Exception e)
		{
			;
		}	
		return restDict;
	}
	/**
	 * ���ܣ� ɾ���������
	 * @param str
	 * @return
	 */
	private String checkCharacter(String str)
	{
		String[] arrayS;
		arrayS = new String[3];
		arrayS[0] = "$";
		arrayS[1] = "(";
		arrayS[2] = ")";
		
		StringBuffer strBuffer = new StringBuffer(str);
		for(int i = 0; i < arrayS.length; i++)
		{
			String tmp = arrayS[i];
			strBuffer.delete(strBuffer.indexOf(tmp), strBuffer.indexOf(tmp) + 1);
		}
		return strBuffer.toString();
	}
}
