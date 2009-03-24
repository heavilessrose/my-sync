package org.enhydra.khttp;

import java.io.*;
import java.util.*;
import javax.microedition.io.*;


/** The server side view of a HTTP connection. The access methods are
    similar to the javax.microedition.io.HttpConnection interface,
    except that set methods are available for the response properties
    instead of the request properties. */


public abstract class ServerHttpConnection implements StreamConnection {

    Hashtable reqProp = new Hashtable ();
    Hashtable rspProp = new Hashtable ();
    int rspCode = 200;
    String rspMsg = "Ok";
    String method;
    String url;

    InputStream in;
    ByteArrayOutputStream buf = new ByteArrayOutputStream ();


    ServerHttpConnection (InputStream in) throws IOException {
	this.in = in;

	// read request line

	String req = readLine (in);

	// parse request line

	int sp1 = req.indexOf (' ');
	method = req.substring (0, sp1);
	int sp2 = req.indexOf (' ', sp1+1);
	url = req.substring (sp1+1, sp2);

	// fill request header

	while (true) {
	    String l = readLine (in);
	    System.out.println ("header: "+l);
	    if (l == null || l.length () == 0) 
		break;

	    int c = l.indexOf (':');
	    reqProp.put (l.substring (0, c).trim ().toLowerCase (), 
			 l.substring (c+1).trim ());
	}
    }


    public abstract void close () throws IOException;


    public String getRequestProperty (String name) {
	return (String) reqProp.get (name.toLowerCase ());
    }

    public String getURL () {
	return url;
    }


    public DataInputStream openDataInputStream () {
	return new DataInputStream (in);
    }


    public DataOutputStream openDataOutputStream () {
	return new DataOutputStream (buf);
    }


    public InputStream openInputStream () {
	return in;
    }


    public OutputStream openOutputStream () {
	return buf;
    }

    public void setResponseCode (int code, String msg) {
	rspCode = code;
	rspMsg = msg;
    }    


    public void setResponseProperty (String name, String prop) {
	rspProp.put (name, prop);
    }


    protected void write (OutputStream out) throws IOException {
	out.write (("HTTP/1.1 " + rspCode + " " + rspMsg + "\r\n").getBytes ());

	System.out.println ("Sending: "+buf);

	byte [] body = buf.toByteArray ();
	rspProp.put ("Content-Length", ""+body.length);
	rspProp.put ("Connection", "close");

	for (Enumeration e = rspProp.keys (); e.hasMoreElements (); ) {
	    String key = (String) e.nextElement ();
	    out.write ((key + ": "+ rspProp.get (key) + "\r\n").getBytes ());
	}

	System.out.println ("rspProps:"+rspProp);

	out.write ('\r');
	out.write ('\n');

	out.write (body);
	out.flush ();
    }
    
    
    public static String readLine (InputStream in) throws IOException {

	StringBuffer buf = new StringBuffer ();

	while (true) {
	    int i = in.read ();
	    if (i == -1 && buf.length()==0) return null;
	    if (i == -1 || i == '\n') return buf.toString ();
	    if (i != '\r') 
		buf.append ((char) i);
	}
    }
}
