/*
 * Copyright (C) 2006 Astrient Labs, LLC
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.ui;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

import com.astrientlabs.cache.Cache;
import com.astrientlabs.i18n.TextMap;
import com.astrientlabs.jobs.DownloadImageJob;
import com.astrientlabs.log.Logger;
import com.astrientlabs.net.Cancellable;
import com.astrientlabs.net.Network;
import com.astrientlabs.search.SearchQuery;
import com.astrientlabs.search.SearchResult;
import com.astrientlabs.threads.JobRunner;
import com.astrientlabs.util.ProgressListener;


public class ResultDisplayWindow extends Window implements Runnable, ProgressListener, Cancellable
{        
    public static ResultDisplayWindow instance;
    private ResultDisplayWidget resultDisplayWidget;

    private SearchQuery searchQuery;
    private SearchResult searchResult;
    private boolean running;
    private Cache imageCache;
    
    public ResultDisplayWindow()
    {
        super(TextMap.instance.get("text.slideshow"));
        
        headerHeight = 0;
        imageCache = new Cache(2);
        
        resultDisplayWidget = new ResultDisplayWidget(this,getHeight());
        children.addElement(resultDisplayWidget);
        focus = resultDisplayWidget;
    }
    
    //TODO: fix for nokia full canvas bug
    protected void sizeChanged(int w, int h)
    {
        super.sizeChanged(w, h);
        if ( resultDisplayWidget != null )
        {
            resultDisplayWidget.widgetHeight = getHeight()-footerHeight-headerHeight;
        }
    }
    
    public void setSearchResult(SearchResult searchResult)
    {
        this.searchResult = searchResult;
        this.searchQuery = searchResult.getSearchQuery();
    }
    
    protected void drawHeader(Graphics g)
    {
    }


    public void widgetSelected(Widget who, Object arg)
    {
        navigateTo(SearchWindow.instance);
    }
    
    protected void moveItem(boolean forward)
    {
        if ( !running )
        {
            int index = Math.max(0, this.searchQuery.getResults().indexOf(searchResult)) + ((forward)?1:-1);
            if ( index > -1 && index < this.searchQuery.getResults().size() )
            {
                searchResult = (SearchResult) searchQuery.getResults().elementAt(index);
            }
            else
            {
                searchResult = null;
            }
            
            
            JobRunner.instance.run(this);         
        }
    }
    
    protected void handleKeyPressed(int keyCode)
    {
        int action = getGameAction(keyCode);
        if ( action == LEFT )
        {
            moveItem(false);
        }
        else if ( action == RIGHT )
        {
            moveItem(true);
        }
        else if ( action == DOWN )
        {
            if ( searchResult != null )
            {
                JobRunner.instance.run(new DownloadImageJob(this,searchResult));
            }
        }
        else if ( action == UP )
        {
            resultDisplayWidget.setImage(null,"");
            navigateTo(SearchWindow.instance);
        }
        else
        {
            super.handleKeyPressed(keyCode);
        }
    }
    
    
    public void run()
    {
        if ( !running )
        {
            running = true;
            
            try
            {
                if ( searchResult == null )
                {
                    AdWindow.instance.setMessage(searchQuery.getTerm());
                    navigateTo(AdWindow.instance);
                    AdWindow.instance.setMessage("searching...");
                    searchQuery.execute();
                    
                    if ( !searchQuery.getResults().isEmpty() )
                    {
                        searchResult = (SearchResult)searchQuery.getResults().firstElement();
                    }
                }
                
                if ( searchResult != null && searchResult.getUrl().startsWith("http") )
                {
                    
                    Image image = (Image)imageCache.get(searchResult.getUrl());
                     
                    if ( image == null )
                    {
                        AdWindow.instance.setMessage("fetching " + searchResult.getTitle());
                        navigateTo(AdWindow.instance);
                        image = searchResult.getImage(resultDisplayWidget.widgetWidth,resultDisplayWidget.widgetHeight);
                        imageCache.put(searchResult.getUrl(), image);
                        Network.postOp();
                    }
                    
                    if ( running )
                    {
                        resultDisplayWidget.setImage(image,searchResult.getTitle());
                        repaintChild(resultDisplayWidget, false);     
                    }
                }
            }
            catch (Throwable e)
            {
                setError(e);
                Logger.instance.log("slideshowWidget.run()",e);
            }
            finally
            {
                if ( running ) navigateTo(this);
                running = false;
            }
        }
    }
    
    

    public void cancel()
    {
        if ( searchResult != null ) searchResult.cancel();
        running = false;
    }

    public boolean shouldCancel()
    {
        return false;
    }

    public void completed(Object who)
    {
        // TODO Auto-generated method stub
        
    }

    public void update(Object who, long position)
    {
        // TODO Auto-generated method stub   
    }
}
