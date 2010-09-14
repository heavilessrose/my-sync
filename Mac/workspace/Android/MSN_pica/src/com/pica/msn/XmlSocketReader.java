package com.pica.msn;
import java.io.*;
import java.util.*;

public class XmlSocketReader extends XmlReader
{

	//InputStreamReader ipsr;
	InputStream ipsr;
	private byte[] data ;
	private int k;
	int cacheLength= 2048;
	byte[] tmpRcvData;
	
	public XmlSocketReader(InputStream in) throws IOException,
			UnsupportedEncodingException
	{
		super(in);
		//ipsr=new InputStreamReader(in,"UTF-8");
		ipsr=in;
		inside_tag = false;
	}
	
	
	public void reset()
	{
		attributes.clear();
		inside_tag=false;
		type = END_TAG;
	}
	
	public int next() throws IOException
	{
		cLast = c;
		c = ipsr.read();
		if (c <= ' ') // omit white space
			while ((c = ipsr.read()) <= ' ' && c != -1)
			{
				//System.out.println("omit");
			}

		if (c == -1)
		{
			type = END_DOCUMENT;
			return type;
		}

		if (c == '<' || (cLast == '<' && c == '/' && !inside_tag))
		{
			inside_tag = true;
			// reset all
			tagName = null;
			text = null;
			attributes.clear();

			if (c == '<')
			{
				c = ipsr.read();
			}
			if (c == '/')
			{
				type = END_TAG;
				c = ipsr.read();
				tagName = readName('>');
			} else if (c == '?' || c == '!')
			{ // ignore xml heading & comments
				while ((c = ipsr.read()) != '>')
				{
					//System.out.println("ignore");
				}

				next();
			} else
			{
				type = START_TAG;
				tagName = readName(' ');

				String attribute = "";
				String value = "";
				while (c == ' ')
				{
					c = ipsr.read();
					attribute = readName('=');
					c = ipsr.read(); // '''
					c = ipsr.read();
					if (c == '\'') // if a null value
						value = "";
					else
						value = readText('\'');

					c = ipsr.read();
					attributes.put(attribute, value);
				}

				if (c != '/')
				{
					inside_tag = false;
				}
			}
		} else if (c == '>' && inside_tag)
		{ // last tag ended
			type = END_TAG;
			inside_tag = false;
		} 
		///////
		else if(c!='/' && cLast == '<') {
            type = START_TAG;
            tagName = readName(' ');

            String attribute = "";
            String value = "";
            while (c == ' ')
            {
                c = ipsr.read();
                attribute = readName('=');
                c = ipsr.read(); // '''
                c = ipsr.read();
                if (c == '\'') // if a null value
                    value = "";
                else
                    value = readText('\'');

                c = ipsr.read();
                attributes.put(attribute, value);
            }

            if (c != '/')
            {
                inside_tag = false;
            }
        }
		/////////
		
		
		else
		{
			tagName = null;
			attributes.clear();

			type = TEXT;
			text = readText('<');
		}

		return type;
	}

	public String getName()
	{
		return tagName;
	}

	public String getAttribute(String name)
	{
		return (String) attributes.get(name);
	}

	public String getText()
	{
		return text;
	}
	
	public String readText(int end)  throws IOException
	{
		data = new byte[cacheLength];
	    k = 0;

	      do
	      {

	            if (c == (int) '&')
	            {
	                c = ipsr.read();
	                switch (c)
	                {
	                case 'l':
	                    add('<');
	                    break;
	                case 'g':
	                    add('>');
	                    break;
	                case 'a':
	                    if(ipsr.read()=='m')
	                        add('&');
	                    else
	                        add('\'');
	                    break;
	                case 'q':
	                    add('"');
	                    break;
	                case 'n':
	                    add(' ');
	                    break;
	                default:
	                    add('?');
	                    break;
	                }

	                while ((c = ipsr.read()) != ';')
	                {
	                }
	            } else if (c == '\\')
	            {
	                c = ipsr.read();
	                if (c == end)
	                {
	                    add('\\');
	                    break;
	                } else if (c == '&')
	                {
	                    add('\\');
	                    continue;
	                }

	                if (c == '<')
	                {
	                    add('\\');
	                    break;
	                } else if (c == '\\'){
	                    add('\\');
	                    add('\\');
	                }else
	                {
	                    add('\\');
	                    add((char)c);
	                }
	            } else{
	                add((char)c);
	            }
	        } while ((c = (c != '&' ? ipsr.read() : '&')) != end);
	            
	        String strText="";
	        

	        strText = new String(data, 0, k, "UTF-8");

	        data = null;
	        k = 0;
	        
	        return strText;
	    }
	    
    private void add(char c)
    {
        data[k] = (byte)(c);    
        if (++k == cacheLength)
        {
        	cacheLength += 2048;
        	tmpRcvData = new byte[cacheLength];
        	System.arraycopy(data, 0, tmpRcvData, 0, k);
        	data = null;
        	data = tmpRcvData;
        }
    }

	// modified by Kevin Hua
	public String readName(int end) throws IOException
	{
		StringBuffer output = new StringBuffer("");
		do
		{
			output.append((char) c);
		} while ((c = ipsr.read()) != end && c != '>' && c != '/');

		return output.toString();
	}
	
	public void close()
	{
		try
		{
			ipsr.close();
		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	public String readLengthText(int length)
	{
		try
		{
			//System.out.println("sssssssss");
		StringBuffer output = new StringBuffer("");
		int len=0;
		c = ipsr.read();
		do
		{
			output.append((char) c);
			len++;
		} while ((c = ipsr.read()) != -1 && len!=length);
		
		return output.toString();
		}catch(Exception e)
		{
			e.printStackTrace();
		}
		return "";
//		String response=null;
//		response=xml_string.substring(pos,pos+length);
//		pos=pos+length;
//		return response;
	}
}

