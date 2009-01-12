package com.astrientlabs.util;

import javax.microedition.io.HttpConnection;

public class TimeoutConnection
{
    private HttpConnection connection;
    private long timeout;
    private boolean timedout;
    
    public TimeoutConnection(HttpConnection connection, long timeout)
    {
        this.connection = connection;
        this.timeout = timeout;
    }

    public HttpConnection getConnection()
    {
        return connection;
    }

    public long getTimeout()
    {
        return timeout;
    }
    
    public boolean isTimedout()
    {
        return timedout;
    }

    public void close()
    {
        try
        {
            connection.close();
        }
        catch (Throwable t)
        {
            t.printStackTrace();
        }
        
        timedout = true;
    }
}
