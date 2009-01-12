/*
 * Copyright (C) 2006 Astrient Labs, LLC
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.ui;

import com.astrientlabs.i18n.TextMap;
import com.astrientlabs.net.Network;
import com.astrientlabs.threads.Cron;


public class AdWindow extends Window
{        
    public static final AdWindow instance = new AdWindow();
    
    public AdWidget adWidget;
    private String initialMessage = TextMap.instance.get("text.processing");
    
    public AdWindow()
    {
        super("Advertisement");
        
        adWidget = new AdWidget(this,getHeight()-footerHeight-headerHeight,initialMessage);
        children.addElement(adWidget);
        focus = adWidget;
    }
    
    
    //TODO: fix for nokia full canvas bug
    protected void sizeChanged(int w, int h)
    {
        super.sizeChanged(w, h);
        if ( adWidget != null )
        {
            adWidget.widgetHeight = getHeight()-footerHeight-headerHeight;
        }
    }

    
    public void setMessage(String message)
    {
        adWidget.setMessage(message);
    }

    protected void hideNotify()
    {
        super.hideNotify();
        //adWidget.setMessage(initialMessage);
        Cron.instance.unregister(adWidget);
    }

    protected void showNotify()
    {
        super.showNotify();
        //adWidget.setMessage(initialMessage);
        //setMessage(initialMessage);
        Cron.instance.register(adWidget);
    }
    
    protected void keyPressed(int keyCode)
    {
        int action = getGameAction(keyCode);
        if ( action == UP )
        {
            Network.instance.cancelAll();
            navigateTo(SearchWindow.instance);
        }
        else
        {
            super.keyPressed(keyCode);
        }
    }

    public void widgetSelected(Widget who, Object arg)
    {
        //navigateTo(SeachWindow.instance);
    }
}