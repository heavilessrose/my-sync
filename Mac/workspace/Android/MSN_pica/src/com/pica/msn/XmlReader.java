package com.pica.msn;


import java.io.*;
import java.util.*;

public class XmlReader{
	public final static int START_DOCUMENT=0;
	public final static int END_DOCUMENT=1;
	public final static int START_TAG=2;
	public final static int END_TAG=3;
	public final static int TEXT=4;

	public boolean inside_tag;
	public String tagName;
	public String text;
	public Hashtable attributes=new Hashtable();
	public int c, cLast;
	public int type=START_DOCUMENT;

	public String xml_string;
	public int pos;
	public XmlReader(Object in) throws IOException,
	UnsupportedEncodingException
	{

		inside_tag=false;
	}

	int read()
	{
		return 0;
	}

	public int next() throws Exception
	{
		return 0;
	}

	public String getName()
	{
		return "";
	}

	public String getAttribute(String name)
	{
		return "";
	}

	public String getText()
	{
		return "";
	}

	private String readText(int end) throws Exception
	{

		return "";
	}


	private String readName(int end) throws Exception
	{
		return "";
	}

	
	public void close()
	{
		
	}
}
