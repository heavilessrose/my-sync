package ixstrim.net.http;

import ixstrim.net.ConnectionHandler;
import ixstrim.context.ConnectionContext;
import ixstrim.context.RequestContext;
import ixstrim.util.FixedBuffer;
import ixstrim.util.FormDataBuffer;
import ixstrim.util.HexUtil;

import java.nio.ByteBuffer;
import java.util.ArrayList;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class HttpConnectionHandler extends ConnectionHandler {

    private ConnectionContext connectionContext;
    private FixedBuffer inputBuffer;
    private FixedBuffer outputBuffer;
    private HttpStreamFilter streamFilter;

    private boolean closed = false;//flag used if connection keep-alive
    private boolean keepAlive = false;
    private boolean chunked = false;
    private boolean firstChunkWrited = false;
    private boolean lastChunkWrited = false;
    private boolean responseWrited = false;
    private boolean formUrlencodedProcessing = false;

    private RequestContext requestContext;
    private HttpRequestDispatcher dispatcher;
    private FormDataBuffer formDataBuffer;

    private boolean writeBlocked = false;
    private boolean inRequest = false;
    private ArrayList bufferedOutput = new ArrayList();

    public HttpConnectionHandler(ConnectionContext connectionContext) {
        this.connectionContext = connectionContext;
        this.dispatcher = connectionContext.getRequestDispatcher();
        this.inputBuffer = connectionContext.getBuffersPool().allocate();
        this.outputBuffer = connectionContext.getBuffersPool().allocate();
        this.streamFilter = new HttpStreamFilter(this, this.inputBuffer);
    }

    /**
     * Handling incoming data.
     */
    public void write(ByteBuffer byteBuffer) {
        //we must reject input data if have output data
        if(hasData(bufferedOutput)) {
            writeBlocked = true;
            return;
        }
        //write to buffer while buffer not empty, or data write rejected
        while(byteBuffer.remaining() > 0) {
            int beforeRemaining = byteBuffer.remaining();
            //write to filter, only if GET request not occured
            //writeBlocked will setted to true if request occured in write time
            if(!inRequest) streamFilter.doFilter(byteBuffer);
            if(beforeRemaining == byteBuffer.remaining()) break;//break if data rejected
        }
        inRequest = false;
        //if some data rejected, then connection will blocked for writing by SelectorHandler
        if(byteBuffer.remaining() > 0) writeBlocked = true;
    }


    public ByteBuffer read() {
        boolean responseWrited = this.responseWrited;
        if(!hasData(bufferedOutput)) {
            bufferedOutput.clear();
            readHttp();
        }
        if(responseWrited) outputBuffer.reset();
        ByteBuffer concattedBuffs = concatBuffers(bufferedOutput);
        //we must unblock write if was blocked in write(...) method
        //if all output data returned
        if(writeBlocked && (concattedBuffs == null || !concattedBuffs.hasRemaining())) {
            notifyReadyForInput();
        }
        return concattedBuffs;
    }

    public void close() {
        closed = true;
        if(keepAlive) {
            if(writeBlocked) notifyReadyForInput();
        } else {
            super.close();
        }
    }


    public void writeHttp(ByteBuffer byteBuffer) {
        if(formUrlencodedProcessing) {
            int remaining = byteBuffer.remaining();
            formDataBuffer.append(byteBuffer);
            //if all data parsed then do request with content-length = 0
            if(remaining == streamFilter.mustWrite()) {
                requestContext.getRequest().getHeaders().put(HttpConstants.HEADER_CONTENT_LENGTH, "0");
                formDataBuffer.parametersToMap(requestContext.getRequest().getParameters());
                doDispatch();
            }
        } else {
            OutputHandler outputHandler = requestContext.getOutputHandler();
            if(outputHandler != null){
                requestContext.getOutputHandler().write(byteBuffer);
            }
        }
    }

    private void readHttp() {
        //read first, in first read operation may be setting headers
        ByteBuffer readData = requestContext.getInputHandler().read();

        //write response if need
        if(!responseWrited && requestContext != null) {
            responseWrited = true;
            HttpResponseData response = requestContext.getResponse();
            HttpRequestData request = requestContext.getRequest();
            if(!keepAlive) response.setHeader(HttpConstants.HEADER_CONNECTION, HttpConstants.HEADER_VALUE_CLOSE);
            if(request.getProtocol().compareTo(HttpConstants.PROTOCOL_HTTP11) >= 0) {
                if(response.getHeader(HttpConstants.HEADER_CONTENT_LENGTH) == null
                        && response.getHeader(HttpConstants.HEADER_TRANSFER_ENCODING) == null) {
                    response.setHeader(HttpConstants.HEADER_TRANSFER_ENCODING, HttpConstants.HEADER_VALUE_CHUNKED);
                    chunked = true;
                }
            }
            int len = response.toByteArray(outputBuffer.getArray());
            outputBuffer.setLength(len);
        }

        //write content
        if(chunked) {
            ByteBuffer chunk = readData;
            int arraySize = chunk == null ? 0 : chunk.remaining();
            if(arraySize > 0) {
                int chunkLength = chunk.remaining();
                byte[] chunkSize = HexUtil.chunkSize(chunkLength, firstChunkWrited);
                firstChunkWrited = true;
                bufferedOutput.add(ByteBuffer.wrap(chunkSize));
                bufferedOutput.add(chunk);
            }
        } else {
            ByteBuffer buff = readData;
            int arraySize = buff == null ? 0 : buff.remaining();
            if(arraySize != 0) {
                bufferedOutput.add(buff);
            }
        }

        //write last chunk if need
        if(closed && chunked && !lastChunkWrited) {
            byte[] lastChunk = HexUtil.getLastChunkSize(firstChunkWrited);
            bufferedOutput.add(ByteBuffer.wrap(lastChunk));
            lastChunkWrited = true;
        }
    }

    public void request(HttpRequestData request) {
        //check for keep-alive if not keepAlive already
        //set keep-alive connection only for version >= HTTP/1.1
        if(!keepAlive && HttpConstants.HEADER_VALUE_KEEP_ALIVE
                .equalsIgnoreCase((String)request.getHeaders()
                .get(HttpConstants.HEADER_CONNECTION))
                && HttpConstants.PROTOCOL_HTTP11.compareToIgnoreCase(request.getProtocol()) <= 0) {
            keepAlive = true;
        }
        this.requestContext = new RequestContext(connectionContext,request, new HttpResponseData(this.connectionContext), this);

        //check if method == POST and Content-Type = application/x-www-form-urlencoded
        //then process form data as URI parameters
        //check if method == POST and Content-Type = multipart/form-data
        //then process as multipart
        if(HttpConstants.METHOD_POST.equals(request.getMethod())) {
            String contentType = (String)request.getHeaders().get(HttpConstants.HEADER_CONTENT_TYPE);
            if(HttpConstants.HEADER_VALUE_X_WWW_FORM_URLENCODED
                    .equalsIgnoreCase(contentType) && streamFilter.mustWrite() > 0) {
                formUrlencodedProcessing = true;
                formDataBuffer = new FormDataBuffer(connectionContext, streamFilter.mustWrite());
            } else {
                doDispatch();
            }
        } else {
            doDispatch();
        }
    }

    public void destroy() {
        if(connectionContext != null) {
            //return buffers to pool
            this.connectionContext.getBuffersPool().free(this.inputBuffer);
            this.connectionContext.getBuffersPool().free(this.outputBuffer);
            this.streamFilter = null;
            this.connectionContext = null;
            this.inputBuffer = null;
            this.outputBuffer = null;
        }
    }

    private void notifyReadyForInput() {
        readyForInput();
        writeBlocked = false;
    }

    private void doDispatch() {
        dispatcher.dispatch(requestContext);
        if(streamFilter.mustWrite() <= 0) inRequest = true;
    }

    boolean isClosed() {
        return closed;
    }

    ConnectionContext getContext() {
        return this.connectionContext;
    }

    void reset() {
        closed = false;
        chunked = false;
        firstChunkWrited = false;
        lastChunkWrited = false;
        responseWrited = false;
        formUrlencodedProcessing = false;
        requestContext = null;
        formDataBuffer = null;
    }


    private boolean hasData(ArrayList list) {
        if(list == null) return false;
        for(int i = list.size() - 1; i >= 0; i--) {
            if(((ByteBuffer)list.get(i)).hasRemaining()) return true;
        }
        return false;
    }


    private ByteBuffer concatBuffers(ArrayList list) {
        if(list == null) return null;
        for(int i = 0; i < list.size(); i++) {
            ByteBuffer currentBuffer = (ByteBuffer)list.get(i);
            if(!currentBuffer.hasRemaining()) continue;
            outputBuffer.appendPosible(currentBuffer);
            if(currentBuffer.hasRemaining()) break;
        }
        if(outputBuffer.getLength() > 0)
            return ByteBuffer.wrap(outputBuffer.getArray(), 0, outputBuffer.getLength());
        else
            return null;
    }
}
