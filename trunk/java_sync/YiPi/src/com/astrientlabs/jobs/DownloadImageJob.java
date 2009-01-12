/*
 * Copyright (C) 2006 Astrient Labs, LLC Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance 
 * with the License. You may obtain a copy of the License at 
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the 
 * License for the specific language governing permissions and limitations under
 * the License.
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.jobs;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Enumeration;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;
import javax.microedition.io.file.FileSystemRegistry;

import com.astrientlabs.i18n.TextMap;
import com.astrientlabs.log.Logger;
import com.astrientlabs.midlet.Yipi;
import com.astrientlabs.net.Network;
import com.astrientlabs.search.SearchResult;
import com.astrientlabs.text.Strings;
import com.astrientlabs.ui.AdWindow;
import com.astrientlabs.ui.Window;
import com.astrientlabs.util.Metric;
import com.astrientlabs.util.ProgressListener;
import com.astrientlabs.util.Stats;

public class DownloadImageJob implements Runnable, ProgressListener
{
    private static int antiFlood = 1024;
    protected Window window;

    protected SearchResult searchResult;
    private StringBuffer buffer = new StringBuffer();
    
    private int lastPos = 0;
    
    public DownloadImageJob(Window window, SearchResult searchResult)
    {
        this.window = window;
        this.searchResult = searchResult;
    }

    public void run()
    {
        lastPos = 0;
        AdWindow.instance.setMessage(searchResult.getTitle());
        window.navigateTo(AdWindow.instance);

        FileConnection fileConnection = null;
        try
        {
            Metric metric = new Metric("download");

            
            String location = getDownloadsDir();
            Logger.instance.log("downloadFileJob.run().1","root: " + location);

            String filename = location + searchResult.getTitle();
            Logger.instance.log("downloadFileJob.run()",filename);

            fileConnection = (FileConnection) Connector.open(filename, Connector.READ_WRITE);
            
            StringBuffer buffer = new StringBuffer("download\n\n").append(filename);
            
            if ( !fileConnection.exists() )
            {
                fileConnection.create();

                OutputStream os = fileConnection.openOutputStream();
                searchResult.writeTo(os,this);
                os.close();
                
                Stats.downloads++;
                
                buffer.append(" saved.\n\n")
                .append( (metric.elapsed()/1000) )
                .append(" seconds");
            }
            else
            {
                buffer.append(TextMap.instance.get("text.fileexists"));
                throw new IOException(buffer.toString());
            }
            
            window.setMessage(buffer.toString());         
        }
        catch (Throwable t)
        {
            Logger.instance.log("downloadFileJob.run().2",t);
            
            window.setError(t);
        }
        finally
        {
            if (fileConnection != null)
            {
                try
                {
                    fileConnection.close();
                }
                catch (Exception e)
                {
                }
            }

            Network.postOp();
            AdWindow.instance.navigateTo(window);
        }
    }
    
    
    public String getDownloadsDir()
    {
        String location = Yipi.instance.systemPrefs.get("dldbin");

        if ( location == null )
        {
            
            FileConnection dirConnection = null;

                String dir;
                for (Enumeration e = FileSystemRegistry.listRoots(); e.hasMoreElements();)
                {
                     dir = "file:///" + e.nextElement() + "yipi/";
                     Logger.instance.log("dl",dir);
                     try
                     {
                         dirConnection = (FileConnection) Connector.open(dir, Connector.READ_WRITE);
                         if (!dirConnection.exists())
                         {
                             dirConnection.mkdir();
                             
                         }
                         
                         if ( dirConnection.canWrite() )
                         {
                             location = dirConnection.getURL();
                             Yipi.instance.systemPrefs.put("dldbin",location);
                             break;
                         }
                     }
                     catch (Throwable ioe)
                     {
                         Logger.instance.log("downloadImageJob",ioe);
                     }
                     finally
                     {
                         if ( dirConnection != null )
                             try {dirConnection.close();} catch(Exception ex){}
                     }
                }
        }
        
        return location;
    }
    
    
    

    public void update(Object who, long position)
    {
        if ( who == searchResult )
        {
            if ( (position - lastPos) > antiFlood )
            {
                buffer.setLength(0);
                buffer.append(searchResult.getTitle())
                .append(" ")
                .append(Strings.getSizeString(position))
                .append("/")
                .append(Strings.getSizeString(searchResult.getSize()));
                
                AdWindow.instance.setMessage( buffer.toString() );   
            }
        }
    }
    
    public void completed(Object who)
    {
        
    }
}
