package net.http;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;
import javax.microedition.lcdui.Form;

import net.http.cookie.Cookie;
import net.http.cookie.CookieManager;

/**
 * HttpUtil is the core of the httpme framework,It provides three methods to send data to web server.
 *
 * @author mingjava
 * @version 0.1 05/06/2006
 * @since httpme 0.1
 */
public abstract class HttpUtil{
    
    private String host = "";
    private CookieManager cm = new CookieManager();
    public static final String BOUNDARY = "----------------dfjksdfjkdjf88982kjdf";
    public static final String APPLICATION_X_WWW_FORM_URLENCODED = "application/x-www-form-urlencoded";
    public static final String MULTIPART_FORM_DATA = "multipart/form-data";
    public static final String SET_COOKIE = "set-cookie";
    public static final String COOKIE = "cookie";
    
    
    /**
     *Initializes a HttpUtil instance so that you can use it to submit the form
     *@param host a String such as http://www.j2medev.com
     */
    public HttpUtil(String host) {
        this.host = host;
    }
    
    /**
     * send the data to the web server,This will use GET method.and the data will be encoded in the url.
     * for example /webapp/post?hello=world
     * @param uri the uri of the request
     * @param params a String array ,the parameter's name
     * @param values a String array ,the parameter's value
     */
    public void get(final String uri,final String[] params,final String[] values){
        new Thread(){
            public void run(){
                HttpConnection conn = null;
                try {
                    StringBuffer parameter = new StringBuffer();
                    for(int i = 0;i<params.length;i++){
                        parameter.append(params[i]+"="+URLEncoder.encode(values[i],"utf-8")+"&");
                    }
                    conn = (HttpConnection)Connector.open(host+uri);
                    conn.setRequestMethod(HttpConnection.GET);
                    conn.setRequestProperty("content-length",parameter.toString().length()+"");
                    String cookie = collectCookie(uri);
                    if(cookie != null)
                        conn.setRequestProperty(COOKIE,cookie);
                    int code = conn.getResponseCode();
                    if(code != HttpConnection.HTTP_OK){
                        //fix me here
                        onFailed(null,code);
                    }
                    String sCookie = conn.getHeaderField(SET_COOKIE);
                    if(sCookie != null){
                        saveCookie(sCookie,uri);
                    }
                    DataInputStream dis = conn.openDataInputStream();
                    onSuccess(conn,dis);
                    
                } catch (IOException ex) {
                    ex.printStackTrace();
                    onFailed(ex,-1);
                }
            }
        }.start();
    }
    
    /**
     * post the data contained in the form to the web server.the content type is set to application/x-www-form-urlencoded or multipart/form-data according parameter type.
     * you should note that if the length of data is more than 2106 bytes,the mobile device may be transfer the data
     * block by block.so the header content-length will be discarded.The web server should be able to handle this situation.
     * @param uri the uri of request
     * @param form the form which contained data to be submited
     * @param type the content-type either application/x-www-form-urlencoded or multipart/form-data
     */
    public void post(final String uri,final Form form,final String type) throws IllegalArgumentException{
        new Thread(){
            public void run(){
                byte[] data = null;
                HttpConnection conn = null;
                try {
                    conn = (HttpConnection)Connector.open(host+uri);
                    conn.setRequestMethod(HttpConnection.POST);
                    conn.setRequestProperty("Connection","Close");
                    if(APPLICATION_X_WWW_FORM_URLENCODED.equals(type)){
                        data = FormUtil.collect(form);
                        conn.setRequestProperty("Content-type",APPLICATION_X_WWW_FORM_URLENCODED);
                    }else if(MULTIPART_FORM_DATA.equals(type)){
                        data = FormUtil.collectMultipart(form);
                        conn.setRequestProperty("Content-Type",MULTIPART_FORM_DATA+";boundary="+BOUNDARY);
                    }else{
                        throw new IllegalArgumentException("content-type header is invalid");
                    }
                    conn.setRequestProperty("Content-Length",String.valueOf(data.length));
                    String _cookie = collectCookie(uri);
                    if(_cookie != null)
                        conn.setRequestProperty(COOKIE,_cookie);
                    DataOutputStream dos = conn.openDataOutputStream();
                    dos.write(data);
                    dos.close();
                    int code = conn.getResponseCode();
                    if(code != HttpConnection.HTTP_OK){
                       onFailed(null,code);
                    }
                    String sCookie = conn.getHeaderField(SET_COOKIE);
                    if(sCookie != null){
                        saveCookie(sCookie,uri);
                    }
                    DataInputStream dis = conn.openDataInputStream();
                    onSuccess(conn,dis);
                    
                } catch (IOException ex) {
                    ex.printStackTrace();
                    onFailed(ex,-1);
                }
            }
        }.start();
    }
    
    private void saveCookie(String sCookie,String uri){
        System.out.println("receive string:"+sCookie);
        Cookie cookie = Cookie.parseCookie(sCookie,uri);
        System.out.println("receive cookie :"+cookie.toString());
        cm.addCookie(cookie);
    }
    
    private String collectCookie(String uri){
        Cookie[] cookies = cm.getCookie(uri);
        if(cookies != null){
            StringBuffer sb = new StringBuffer();
            for(int i = 0;i<cookies.length;i++){
                sb.append(cookies[i].getName()+"="+cookies[i].getValue()+";");
            }
            String c = sb.toString();
            //delete the last ;
            return c.substring(0,c.length()-1);
        }
        return null;
    }
    /**
     * the subclass of HttpUtil should implement this method when the response is returned successflly
     * @param conn the HttpConnection instance
     * @param dis the DataInputStream returned from HttpConnection.openDataInputStream();
     */
    public abstract void onSuccess(HttpConnection conn,DataInputStream dis);
    
    /**
     * the subclass of HttpUtil should implement this method when error happend
     * @param code the response code
     */
    public abstract void onFailed(Exception ex,int code);
}
