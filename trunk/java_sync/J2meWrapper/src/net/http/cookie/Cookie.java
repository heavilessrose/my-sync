package net.http.cookie;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Date;

import net.http.StringUtil;

/**
 * a cookie stored on the mobile device, cookie is used to maintain the states between client and server
 * @author mingjava
 * @version 0.1 05/06/2006
 */
public class Cookie {
    
    private String path = "";
    private String name = "";
    private String value = "";
    private long expire = SESSION_COOKIE;
    public static  long SESSION_COOKIE = 0;//session cookie,only valid this session
    
    public Cookie() {
    }
    
    public String getPath() {
        return path;
    }
    
    public void setPath(String path) {
        this.path = path;
    }
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public String getValue() {
        return value;
    }
    
    public void setValue(String value) {
        this.value = value;
    }
    
    public void serialize(DataOutputStream dos) throws IOException{
        dos.writeUTF(name);
        dos.writeUTF(value);
        dos.writeUTF(path);
        dos.writeLong(expire);
    }
    
    public static Cookie deserialize(DataInputStream dis) throws IOException{
        Cookie cookie = new Cookie();
        cookie.name = dis.readUTF();
        cookie.value = dis.readUTF();
        cookie.path = dis.readUTF();
        cookie.expire = dis.readLong();
        return cookie;
    }
    
    public long getExpire() {
        return expire;
    }
    
    public void setExpire(long expire) {
        this.expire = expire;
    }
    //for debug
    public String toString(){
        return name+"="+value+";expires="+new Date(expire).toString()+";path="+path;
    }
    
    public boolean isExpired(long now){
        return expire-now<0;
    }
    
    public boolean isExpired(){
        return expire-(new Date().getTime())<0;
    }
    
    public static Cookie parseCookie(String s,String uri){
        Cookie cookie = new Cookie();
        StringUtil su = new StringUtil(s,";");
        while(su.hasMoreTokens()){
            String str = su.nextToken().trim();
            int i = str.indexOf("=");
            if(i == -1){
                //secure do nothing
                continue;
            }else{
                String name = str.substring(0,i);
                String value = str.substring(i+1,str.length());
                if("path".equalsIgnoreCase(name)){
                    cookie.setPath(value);
                }else if("expires".equalsIgnoreCase(name)){
                    cookie.setExpire(StringUtil.getData(value));
                }else if("domain".equalsIgnoreCase(name)){
                    //do nothing
                }else{
                    cookie.setName(name);
                    cookie.setValue(value);
                }
            }
            if(cookie.getPath().equals(""))
                cookie.setPath(uri);
        }
        return cookie;
    }
    
    public boolean equals(Object obj){
        if(obj instanceof Cookie){
            Cookie o = (Cookie)obj;
            if(o.getName().equals(name) && o.getPath().equals(path))
                return true;
        }
        return false;
    }
    
    public int hashCode(){
        int result = 17;
        result = result * 37 + path.hashCode();
        result = result * 37 + name.hashCode();
        return result;
    }
}
