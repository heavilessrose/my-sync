package com.pica.msn;

import java.io.UnsupportedEncodingException;
import java.util.Hashtable;

public class XmlHttpReader extends XmlReader
{

	public final static int START_DOCUMENT=0;
	public final static int END_DOCUMENT=1;
	public final static int START_TAG=2;
	public final static int END_TAG=3;
	public final static int TEXT=4;

	private boolean inside_tag;
	private String tagName;
	private String text;
	private Hashtable attributes=new Hashtable();
	private int c, cLast;
	private int type=START_DOCUMENT;

	//public String xml_string;
	private int pos;
	public XmlHttpReader(String xmlStr) throws Exception,
		UnsupportedEncodingException
	{
		super(xmlStr);
		inside_tag=false;
		xml_string=xmlStr;
		pos=0;
	}

	int read()
	{
		if(pos>=xml_string.length())
			return-1;

		return(int)xml_string.charAt(pos++);
	}

	public int next() throws Exception
	{
		cLast=c;
		c=read();
		if(c<=' ') // omit white space
			while((c=read())<=' '&&c!=-1)
				;

		if(c==-1){
			type=END_DOCUMENT;
			return type;
		}

		if(c=='<'||(cLast=='<'&&c=='/'&&!inside_tag)){
			inside_tag=true;
			// reset all
			tagName=null;
			text=null;
			attributes.clear();

			if(c=='<'){
				c=read();
			}
			if(c=='/'){
				type=END_TAG;
				c=read();
				tagName=readName('>');
			} else
				if(c=='?'||c=='!'){ // ignore xml heading & comments
					while((c=read())!='>')
						;

					next();
				} else{
					type=START_TAG;
					tagName=readName(' ');

					String attribute="";
					String value="";
					while(c==' '){
						c=read();
						attribute=readName('=');
						c=read(); // '''
						c=read();
						if(c=='\'') // if a null value
							value="";
						else
							value=readText('\'');

						c=read();
						attributes.put(attribute,value);
					}

					if(c!='/'){
						inside_tag=false;
					}
				}
		} else
			if(c=='>'&&inside_tag){ // last tag ended
				type=END_TAG;
				inside_tag=false;
			} else{
				tagName=null;
				attributes.clear();

				type=TEXT;
				text=readText('<');
			}

		return type;
	}

	public String getName()
	{
		return tagName;
	}

	public String getAttribute(String name)
	{
		return(String)attributes.get(name);
	}

	public String getText()
	{
		return text;
	}

	private String readText(int end) throws Exception
	{
		StringBuffer output=new StringBuffer("");
		do{
			if(c==(int)'&'){
				c=read();
				switch(c){
				case 'l':
					output.append('<');
					break;
				case 'g':
					output.append('>');
					break;
				case 'a':
					if(read()=='m')
						output.append('&');
					else
						output.append('\'');
					break;
				case 'q':
					output.append('"');
					break;
				case 'n':
					output.append(' ');
					break;
				default:
					output.append('?');
				break;
				}

				while((c=read())!=';'){
				}
			} else
				if(c=='\\'){
					c=read();
					if(c==end){
						output.append('\\');
						break;
					} else
						if(c=='&'){
							output.append('\\');
							continue;
						}

					if(c=='<'){
						output.append('\\');
						break;
					} else
						if(c=='\\')
							output.append("\\\\");
						else{
							output.append('\\');
							output.append((char)c);
						}
				} else
					output.append((char)c);
		} while((c=(c!='&'?read():'&'))!=end&&c!=-1);

		String strText="";
		try{
			strText=output.toString();
		} catch(Exception e){
			e.printStackTrace();
		}
		return strText;
	}


	private String readName(int end) throws Exception
	{
		StringBuffer output=new StringBuffer("");
		do{
			output.append((char)c);
		} while((c=read())!=end&&c!='>'&&c!='/'&&c!=-1);

		return output.toString();
	}
	
	public String readLengthText(int length)
	{
		String response=null;
		response=xml_string.substring(pos,pos+length);
		pos=pos+length;
		return response;
	}
	
	public void close()
	{
		
	}

}
