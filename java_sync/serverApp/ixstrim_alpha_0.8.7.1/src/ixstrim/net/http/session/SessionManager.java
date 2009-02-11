package ixstrim.net.http.session;

import java.util.Hashtable;
import java.util.Map;
import java.util.Random;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 9, 2003
 */
public class SessionManager {

    private static SessionManager instance = new SessionManager();
    private static final int ID_LEN = 16;
    private static final char[] codes = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                                         'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F',
                                         'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                                         'W', 'X', 'W', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    private Random r = new Random();

    private Map map = new Hashtable();

    private SessionManager() {
    }

    public static SessionManager getSessionManager() {
        return instance;
    }

    /**
     * Bind session
     * @param session
     */
    public void bind(HttpSessionImpl session) {
        String id = generateId();
        while(map.get(id) != null) id = generateId();
        map.put(id, session);
        session.setId(id);
    }

    /**
     * Lookup session
     * @param id session id
     * @return found session
     */
    public HttpSessionImpl lookup(String id) {
        return (HttpSessionImpl)map.get(id);
    }

    /**
     * Unbind session (invalidate)
     * @param id session id
     * @return removed session
     */
    public HttpSessionImpl unbind(String id) {
        return (HttpSessionImpl)map.remove(id);
    }

    private String generateId() {
        char[] cc = new char[ID_LEN];
        for(int i = 0; i < cc.length; i++) {
            cc[i] = codes[r.nextInt(codes.length)];
        }
        return new String(cc);
    }

}
