package org.enhydra.khttp;

/** A common interface for the socket and proxy based 
    J2ME HTTP server classes */ 

import java.io.IOException;
import javax.microedition.io.Connection;

public interface HttpServer extends Connection {


    public abstract ServerHttpConnection accept () throws IOException;
    public abstract void close () throws IOException;
}
