package ixstrim.net.http.session;

import javax.servlet.http.HttpSession;
import javax.servlet.http.HttpSessionContext;
import javax.servlet.ServletContext;
import java.util.HashMap;
import java.util.Set;
import java.util.Collections;
import java.util.Enumeration;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class HttpSessionImpl implements HttpSession {

    private HashMap map = new HashMap();
    private String id;
    private long creationTime = System.currentTimeMillis();
    private long lastAccessTime = creationTime;
    private boolean isNew = true;

    public void setAttribute(String attributeName, Object value) {
        map.put(attributeName, value);
    }

    public void removeAttribute(String attributeName) {
        map.remove(attributeName);
    }

    public Object getAttribute(String attributeName) {
        return map.get(attributeName);
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public void invalidate() {
    }

    public long getLastAccessedTime() {
        return lastAccessTime;
    }

    public long getCreationTime() {
        return creationTime;
    }

    public Enumeration getAttributeNames() {
        return Collections.enumeration(map.keySet());
    }

    public void setIsNew(boolean isNew){
        this.isNew = isNew;
    }

    public boolean isNew() {
        return false;
    }

    public void setMaxInactiveInterval(int i) {
    }

    public int getMaxInactiveInterval() {
        return Integer.MAX_VALUE;
    }

    public HttpSessionContext getSessionContext() {
        //depricated
        return null;
    }

    public Object getValue(String s) {
        //depricated
        return null;
    }

    public String[] getValueNames() {
        //depricated
        return new String[0];
    }

    public void putValue(String s, Object o) {
        //depricated
    }

    public void removeValue(String s) {
        //depricated
    }

    public ServletContext getServletContext() {
        return null;
    }
}
