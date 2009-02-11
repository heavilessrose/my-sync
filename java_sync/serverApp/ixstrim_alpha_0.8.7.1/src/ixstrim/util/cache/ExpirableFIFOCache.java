package ixstrim.util.cache;

import java.util.ArrayList;
import java.util.HashMap;

/**
 * Cache implementation with FIFO startegy and elements expiring.
 * If expiration time <= 0 then elements will not expiring
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Aug 17, 2003
 */
public class ExpirableFIFOCache implements Cache {

    private HashMap map = new HashMap();
    private ArrayList list = new ArrayList();
    private long expirationTime;
    private int limit;

    /**
     *
     * @param limit Max count elements in cache
     * @param expirationTime Expiration time in milliseconds. If expirationTime <=0 then elements will not expiring.
     */
    public ExpirableFIFOCache(int limit, long expirationTime) {
        this.limit = limit;
        this.expirationTime = expirationTime;
    }


    public synchronized void put(Object key, Object value) {
        if (value == null) {
            throw new NullPointerException("Value can't be null.");
        }
        while(map.size() > limit && list.size() > 0) {
            Object deletedKey = list.remove(0);
            map.remove(deletedKey);
        }
        Object prev = map.put(key, new CachedElement(value));
        if(prev == null) list.add(key);
    }

    public synchronized Object get(Object key) {
        CachedElement element = (CachedElement)map.get(key);
        if(element == null) return null;
        if(expirationTime > 0
                && element.getCreationTime() + expirationTime < System.currentTimeMillis()) {
            return null;
        } else {
            return element.getValue();
        }
    }

    public synchronized void delete(Object key) {
        map.remove(key);
        list.remove(key);
    }

    public synchronized void clear() {
        map.clear();
        list.clear();
    }

    private class CachedElement {
        private long creationTime = System.currentTimeMillis();
        private Object value;

        public CachedElement(Object value) {
            this.value = value;
        }

        public long getCreationTime() {
            return creationTime;
        }

        public Object getValue() {
            return value;
        }
    }

}
