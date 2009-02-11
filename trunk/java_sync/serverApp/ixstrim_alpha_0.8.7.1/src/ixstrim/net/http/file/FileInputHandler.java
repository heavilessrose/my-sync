package ixstrim.net.http.file;

import ixstrim.context.RequestContext;
import ixstrim.net.http.HttpConstants;
import ixstrim.net.http.InputHandler;
import ixstrim.event.ServerErrorEvent;
import ixstrim.event.EventDispatcher;
import ixstrim.context.ApplicationContext;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.File;
import java.nio.ByteBuffer;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class FileInputHandler implements InputHandler {

    private static ChunkedFileManager fileManager = ChunkedFileManager.getFileManager();

    private EventDispatcher eventDispatcher
            = ApplicationContext.getDefaultApplicationContext().getEventDispatcher();

    private RequestContext requestContext;
    private File localPath;
    private String webPath;
    private int chunkNumber = 0;
    private File filename;
    private byte[] content;
    private boolean isFile = false;
    private boolean closed = false;


    /**
     *
     * @param requestContext RequestContext
     * @param webPath path which must not be included in file name, but exists in URI
     * @param localPath local path where files placed
     */
    public FileInputHandler(RequestContext requestContext, String webPath, String localPath) {
        this.requestContext = requestContext;
        this.webPath = webPath;
        this.localPath = new File(localPath);
        if(!this.localPath.isDirectory())throw new RuntimeException("Is not local directory specified.");
        check();
        requestContext.getCallback().readyForOutput();
    }

    public ByteBuffer read() {
        if(isFile) {
            if(closed) return null;
            byte[] content = fileManager.getChunk(filename, chunkNumber++);
            if(content == null) {
                requestContext.getCallback().close();
                return null;
            }
            if(content.length < fileManager.getChunkSize()) {
                requestContext.getCallback().close();
                closed = true;
            }
            return ByteBuffer.wrap(content);
        } else {
            if(content == null) return null;
            byte[] bb = content;
            content = null;
            return ByteBuffer.wrap(bb);
        }
    }

    private void check() {
        try {
            String uri = requestContext.getRequest().getURI();
            String fn = uri.substring(webPath.length(), uri.length());
            filename = new File(localPath,fn);
            if(filename.getCanonicalPath().startsWith(localPath.getCanonicalPath())){
                if(filename.isDirectory()){
                    if(!fn.endsWith("/")){//then redirect
                        requestContext.getResponse().setHeader("Location", uri+"/");
                        requestContext.getResponse().setResponseCode(HttpConstants.RESPONSE_CODE_302);
                        requestContext.getCallback().close();
                    }else{
                        listDirectory();
                    }
                }else{
                    if(fileManager.getChunk(filename, 0) == null) throw new FileNotFoundException();
                    isFile = true;
                    requestContext.getResponse().setHeader("content-type", FileContentType.getContentType(this.filename.getPath()));
                }
            }else{
                //disable access for parent dirs of specified dir
                throw new FileNotFoundException();
            }
        } catch(FileNotFoundException e) {
            content = ("<h1>"+HttpConstants.RESPONSE_CODE_404+"<h1>").getBytes();
            requestContext.getResponse().setResponseCode(HttpConstants.RESPONSE_CODE_404);
            requestContext.getCallback().close();
        } catch(IOException e) {
            eventDispatcher.errorOccured(new ServerErrorEvent(this,e));
            content = "<h1>Internal error : IOException occured<h1>".getBytes();
            requestContext.getResponse().setResponseCode(HttpConstants.RESPONSE_CODE_500);
            requestContext.getCallback().close();
        }
    }


    private void listDirectory(){
        StringBuffer sb = new StringBuffer(4096);
        File[] files = filename.listFiles();
        sb.append("<table cellpadding='4' border='1' width='100%'>");
        for(int i=0;i<files.length;i++){
            boolean isDir = files[i].isDirectory();
            String fn = files[i].getName() + (isDir ? "/" : "");
            sb.append("<tr><td>");
            sb.append("<a href='").append(fn).append("'>");
            sb.append(fn);
            sb.append("</a>");
            sb.append("</td><td nowrap width='1%'>");
            sb.append(isDir ? "dir" : (String.valueOf(files[i].length()+" bytes")));
            sb.append("</td></tr>");
        }
        sb.append("</table>");
        content = sb.toString().getBytes();
        requestContext.getResponse().setResponseCode(HttpConstants.RESPONSE_CODE_200);
        requestContext.getCallback().close();
    }
}
