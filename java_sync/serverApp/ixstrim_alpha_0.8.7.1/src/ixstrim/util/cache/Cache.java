package ixstrim.util.cache;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 11, 2003
 */
public interface Cache {

    /**
     * Put element into cache, or replace old element if exists
     * @param key
     * @param value
     */
    public void put(Object key, Object value);

    /**
     * Return cached element or null
     * @param key
     * @return cached value
     */
    public Object get(Object key);

    /**
     * Delete element from cache
     * @param key
     */
    public void delete(Object key);

    /**
     * Removes all elements from this cache.
     */
    public void clear();

}
