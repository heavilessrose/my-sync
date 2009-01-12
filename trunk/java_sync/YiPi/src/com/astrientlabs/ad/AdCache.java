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
package com.astrientlabs.ad;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;
import javax.microedition.rms.InvalidRecordIDException;
import javax.microedition.rms.RecordEnumeration;
import javax.microedition.rms.RecordStore;

import com.astrientlabs.midlet.Yipi;
import com.astrientlabs.util.Images;



public class AdCache implements Runnable
{
    public static AdCache instance;

    private static final int OK = 0;
    private String URL = "http://www.hostj2me.com/adserv";
	private int id = 0;
	public static final String STORE_NAME = "adcache";
	private boolean running = false;
	private int cacheSize;
    private Ad defaultAd;
    private RecordStore store;
    private RecordEnumeration renum = null;
    public int adHeight;
    public int adWidth;

    private int count;
    private int inc = 5;

    public AdCache()
    {
        cacheSize = Yipi.instance.systemPrefs.getInt("ad.cache.sz", 5);
        defaultAd = new Ad();
        defaultAd.setName("");
        defaultAd.setDisplayTime(5000);
        defaultAd.setInfoURL("");
        defaultAd.setImage(Images.FAKE_AD); 
	}
    
    
    public void open()
    {
            try
            {
                store = RecordStore.openRecordStore(AdCache.STORE_NAME, true);            
                renum = store.enumerateRecords(null, null, false);            
                //renum.keepUpdated(true);
            }
            catch (Exception e)
            {
                e.printStackTrace();
            }
    }

    public Ad next(int width, int height)
    {
        try
        {
            if ( !renum.hasNextElement() )
            {
                // renum.reset();
                this.adHeight = height;
                this.adWidth = width;
                
                renum.rebuild();
            }
            
            if ( renum.hasNextElement() )
            {
                Ad ad = new Ad();
                ad.fromPersistentFormat(renum.nextRecord());
                defaultAd = ad;
                return ad;
            }
        }
        catch (InvalidRecordIDException e)
        {
            renum.rebuild();
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
        
        return defaultAd;
    }


    public void close()
    {
        try
        {
            if (store != null)
                store.closeRecordStore();
            
            if (renum != null)
                renum.destroy();
        }
        catch (Exception e)
        {
        }
    }
    
    
	public int add(Ad ad) throws IOException
	{
		return add(ad.toPersistentFormat());
	}

	public int add(byte[] data) throws IOException
	{
		try
		{
			return store.addRecord(data, 0, data.length);
		}
        catch (Exception e)
        {
            e.printStackTrace();
        }

		return -1;
	}

	
	public void deleteOldAd()
	{
	    try
	    {
			if ( store.getNumRecords() > cacheSize || ((store.getSizeAvailable() - store.getSize()) < 1024 ) )
			{
			    
				RecordEnumeration renum = store.enumerateRecords(null, null, false);
				if (renum.hasNextElement())
				{
					try
					{					
						store.deleteRecord(renum.nextRecordId());
					}
					catch (Exception e)
					{
						e.printStackTrace();
					}
				}
				renum.destroy();			    
			}
            
            
	    }
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}
    
    
    public void run()
    {
        //TODO: move adds to network actions,i.e after folder listing etc
        if ( count++ % inc == 0 )
        {
            if ( !running )
            { 
                running = true;
                try
                {
                    deleteOldAd();
                    getNewAd();
                }
                finally
                {
                    running = false;
                }
            }   
        }
    }
    

    public void getNewAd()
    {
        String appid = Yipi.instance.systemPrefs.get("ad.appid", "slides");
        String keyword = Yipi.instance.systemPrefs.get("ad.keyword", "*");

        HttpConnection c = null;
        DataInputStream dis = null;
        DataOutputStream dos = null;
        
        try
        {           
            c = (HttpConnection) Connector.open(URL,Connector.READ_WRITE);
            c.setRequestMethod(HttpConnection.POST);

            dos = c.openDataOutputStream();
            dos.writeInt(0);
            dos.writeUTF(appid);
            dos.writeUTF(keyword);
            dos.writeInt(adHeight);
            dos.writeInt(adWidth);
            dos.writeInt(id++);
            dos.flush();
           
            int rc = c.getResponseCode();
            
            
            if (  rc == HttpConnection.HTTP_OK )
            {    
                dis = c.openDataInputStream();
                rc = dis.readInt();
                if ( rc == OK )
                {
                    byte[] buffer = new byte[1024];
                    ByteArrayOutputStream baos = new ByteArrayOutputStream(1024);
                    
                    int i;
                    int t=0;
                    while ( ( i = dis.read(buffer)) != -1 )
                    {
                        t+=i;
                        baos.write(buffer,0,i);
                    }
                    
                    buffer = null;
                    
                    Ad temp = new Ad();
                    temp.fromPersistentFormat(baos.toByteArray());
                    add(temp);
                    
                    baos = null;
                }
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
        finally
        {       
            if (dis != null)
            {
                try
                {
                    dis.close();
                }
                catch (Exception e)
                {
                }
            }
            if (dos != null)
            {
                try
                {
                    dos.close();
                }
                catch (Exception e)
                {
                }
            }
            if (c != null)
            {
                try
                {
                    c.close();
                }
                catch (Exception e)
                {
                }
            }
        }
	}
}
