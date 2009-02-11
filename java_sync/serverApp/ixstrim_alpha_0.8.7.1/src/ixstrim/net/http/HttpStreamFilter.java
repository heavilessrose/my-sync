package ixstrim.net.http;

import ixstrim.util.FixedBuffer;
import ixstrim.util.StringUtil;
import ixstrim.event.ServerErrorEvent;
import ixstrim.event.EventDispatcher;
import ixstrim.context.ApplicationContext;

import javax.servlet.http.Cookie;
import java.nio.ByteBuffer;
import java.util.ArrayList;

/**
 * Util class for filtering HTTP request headers, and create HttpRequest object
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
class HttpStreamFilter{

    private EventDispatcher eventDispatcher
            = ApplicationContext.getDefaultApplicationContext().getEventDispatcher();

    private static final int STATE_0 = 0;
    private static final int STATE_CR = 1;
    private static final int STATE_CRLF = 2;
    private static final int STATE_CRLFCR = 3;
    private static final int STATE_DATA = 4;

    private static final byte CR = (byte)13;
    private static final byte LF = (byte)10;
    private static final byte SP = (byte)' ';
    private static final byte COLON = (byte)':';
    private static final byte SEMICOLON = (byte)';';
    private static final byte QUESTION = (byte)'?';
    private static final byte EQU = (byte)'=';
    private static final byte AMP = (byte)'&';

    private HttpConnectionHandler httpConnectionHandler;

    /**
     * special buffer for buffering input data.
     */
    private FixedBuffer buffer;

    /**
     * current state of finite automaton
     */
    private int state;

    /**
     * end of previous line
     */
    private int prevLineEnd;

    /**
     * end of last line
     */
    private int lineEnd;

    /**
     * first index of ':' in line
     */
    private int colon;

    /**
     * last len requested from buffer
     */
    private int oldLen;

    /**
     * Count bytes must by written to underlying handler
     */
    private int mustWrite;

    /**
     * keys is header names, values - headers values
     */

    private StringUtil stringUtil;

    private HttpRequestData requestData;


    HttpStreamFilter(HttpConnectionHandler httpConnectionHandler, FixedBuffer buffer) {
        this.buffer = buffer;
        this.httpConnectionHandler = httpConnectionHandler;
        this.stringUtil = httpConnectionHandler.getContext().getStringUtil();
        this.requestData = new HttpRequestData(httpConnectionHandler);
        mustWrite = 0;
        reset();
    }


    /**
     * reset all data and states (may be used for keep-alive connection)
     */
    final void reset() {
        buffer.reset();
        prevLineEnd = -1;
        lineEnd = -1;
        colon = -1;
        oldLen = 0;
        state = STATE_0;
        requestData.requestMethod = null;
        requestData.requestURI = null;
        requestData.requestProtocol = null;
        requestData.headers.clear();
        requestData.parameters.clear();
        mustWrite = 0;
    }

    final int mustWrite() {
        return mustWrite;
    }

    final void doFilter(ByteBuffer byteBuffer) {
        if(mustWrite == 0 && httpConnectionHandler.isClosed()) {
            httpConnectionHandler.reset();
            reset();
        }
        if(state != STATE_DATA) {
            parseHeader(byteBuffer);
            if(byteBuffer.remaining() > 0) {
                filterData(byteBuffer);
            }
        } else {
            filterData(byteBuffer);
        }
    }


    /**
     * do parsing on collected data
     */
    private void parseHeader(ByteBuffer byteBuffer) {
        int appendSize = Math.min(buffer.getFree(), byteBuffer.remaining());
        buffer.append(byteBuffer, appendSize);
        int newLen = buffer.getLength();
        byte[] bb = buffer.getArray();
        boolean needSendRequest = false;
        for(int i = oldLen; i < newLen; i++) {
            byte b = bb[i];
            switch(b) {
                case COLON:
                    if(colon < 0) colon = i;
                    break;
                case CR:
                    if(state == STATE_0)
                        state = STATE_CR;
                    else if(state == STATE_CRLF)
                        state = STATE_CRLFCR;
                    else
                        state = STATE_0;
                    break;
                case LF:
                    if(state == STATE_CR) {
                        state = STATE_CRLF;
                        prevLineEnd = lineEnd;
                        lineEnd = i;
                        if(prevLineEnd == -1) {
                            parseRequestLine(0, lineEnd - 1);
                        } else {
                            if(colon != -1) {
                                try {
                                    String key = stringUtil.extractStringLower(bb, prevLineEnd + 1, colon - (prevLineEnd + 1)).trim();
                                    if(HttpConstants.HEADER_COOKIE.equals(key)) {
                                        parseCookies(bb, colon + 1, lineEnd - (colon + 1) - 1);
                                    } else {
                                        String value = stringUtil.extractString(bb, colon + 1, lineEnd - (colon + 1) - 1).trim();
                                        requestData.headers.put(key, value);
                                    }
                                } catch(Exception e) {
                                    eventDispatcher.errorOccured(new ServerErrorEvent(this,e));
                                } finally {
                                    colon = -1;
                                }
                            }
                        }
                    } else if(state == STATE_CRLFCR) {
                        state = STATE_DATA;
                        String contentLengthValue = (String)requestData.headers.get(HttpConstants.HEADER_CONTENT_LENGTH);
                        if(contentLengthValue != null) {
                            try {
                                mustWrite = Integer.parseInt(contentLengthValue);
                            } catch(Exception e) {
                                //do nothig mustWrite = 0
                            }
                        }
                        //init request
                        int restoredSize = newLen - i - 1;
                        byteBuffer.position(byteBuffer.position() - restoredSize);
                        needSendRequest = true;
                        break;
                    } else {
                        state = STATE_0;
                    }
                    break;
                default :
                    state = STATE_0;
                    break;
            }
            if(state == STATE_DATA) break;
        }
        oldLen = buffer.getLength();
        if(buffer.getLength() == buffer.getArray().length && !needSendRequest) throw new RuntimeException("Can't process HTTP header. Size > " + buffer.getArray().length);
        if(needSendRequest) httpConnectionHandler.request(requestData);
    }


    private void filterData(ByteBuffer byteBuffer) {
        int canWrite = Math.min(mustWrite, byteBuffer.remaining());
        if(canWrite > 0) {
            if(httpConnectionHandler.isClosed()) {
                //if closed then invalidate incoming data
                byteBuffer.position(byteBuffer.position() + canWrite);
                mustWrite -= canWrite;
                if(mustWrite == 0) {
                    httpConnectionHandler.reset();
                    reset();
                }
                if(byteBuffer.remaining() > 0) httpConnectionHandler.readyForInput();
            } else {
                int oldLimit = byteBuffer.limit();
                byteBuffer.limit(byteBuffer.position() + canWrite);
                if(byteBuffer.remaining() > 0) {
                    int writed = byteBuffer.remaining();
                    httpConnectionHandler.writeHttp(byteBuffer);
                    writed -= byteBuffer.remaining();
                    mustWrite -= writed;
                }
                byteBuffer.limit(oldLimit);
            }
        }
        if(mustWrite < 0) throw new RuntimeException("Assertion error. MustWrite < 0. MustWrite = " + mustWrite);
    }

    private void parseRequestLine(int start, int len) {
        byte[] bb = buffer.getArray();
        //parsing METHOD
        int i = 0;
        while(i < len && bb[i + start] != SP) {
            i++;
        }
        requestData.requestMethod = stringUtil.extractStringUpper(bb, start, i);

        //parsing URI and parameters
        i++;
        start += i;
        len -= i;
        i = 0;
        int equIdx = -1;
        int ampIdx = -1;
        int firstQuestion = -1;
        while(i++ < len) {
            byte b = bb[i + start];
            switch(b) {
                case QUESTION:
                    if(firstQuestion == -1) firstQuestion = i;
                case AMP:
                    if(ampIdx != -1 && equIdx != -1) {
                        String key = stringUtil.createValue(bb, start + ampIdx + 1, equIdx - (ampIdx + 1));
                        String value = stringUtil.createValue(bb, start + equIdx + 1, i - (equIdx + 1));
                        addParam(key, value);
                    }
                    ampIdx = i;
                    equIdx = -1;
                    break;
                case EQU:
                    equIdx = i;
                    break;
            }
            if(b == SP) break;
        }
        //parse last parameter
        if(ampIdx != -1 && equIdx != -1) {
            String key = stringUtil.createValue(bb, start + ampIdx + 1, equIdx - (ampIdx + 1));
            String value = stringUtil.createValue(bb, start + equIdx + 1, i - (equIdx + 1));
            addParam(key, value);
        }

        int uriLen = firstQuestion == -1 ? i : firstQuestion;
        requestData.requestURI = stringUtil.createValue(bb, start, uriLen);

        //parsing PROTOCOL
        i++;
        start += i;
        len -= i;
        if(len > 0) {
            requestData.requestProtocol = stringUtil.extractStringUpper(bb, start, len);
        } else {
            throw new RuntimeException("Can't parse protocol from Request Line.");
        }
    }


    private void parseCookies(byte[] bb, int start, int len) {
        ArrayList cookiesList = new ArrayList(10);
        int equIdx = -1;
        int delimIdx = start;
        int end = start + len;
        int i = start;
        for(; i < end; i++) {
            byte b = bb[i];
            switch(b) {
                case SEMICOLON:
                    if(equIdx != -1) {
                        String key = stringUtil.createValue(bb, delimIdx + 1, equIdx - (delimIdx + 1)).trim();
                        String value = stringUtil.createValue(bb, equIdx + 1, i - (equIdx + 1)).trim();
                        cookiesList.add(new Cookie(key, value));
                    }
                    delimIdx = i;
                    equIdx = -1;
                    break;
                case EQU:
                    equIdx = i;
                    break;
            }
        }
        if(equIdx != -1) {
            String key = stringUtil.createValue(bb, delimIdx + 1, equIdx - (delimIdx + 1)).trim();
            String value = stringUtil.createValue(bb, equIdx + 1, i - (equIdx + 1)).trim();
            cookiesList.add(new Cookie(key, value));
        }
        requestData.cookies = (Cookie[])cookiesList.toArray(new Cookie[cookiesList.size()]);
    }

    /**
     * Put new paramenter into parameters map, if old value already exists
     * then put old values + new value as String[]
     * @param key
     * @param value
     */
    private void addParam(String key, String value) {
        Object oldValue = requestData.parameters.put(key, value);
        if(oldValue != null) {
            if(oldValue instanceof String) {
                requestData.parameters.put(key, new String[]{(String)oldValue, value});
            } else if(oldValue instanceof String[]) {
                String[] s1 = (String[])oldValue;
                String[] s2 = new String[s1.length + 1];
                System.arraycopy(s1, 0, s2, 0, s1.length);
                s2[s2.length - 1] = value;
                requestData.parameters.put(key, s2);
            }
        }
    }

}