/*
 * Copyright (C) 2006 Astrient Labs, LLC
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.ui;

import com.astrientlabs.log.LogListener;


public class LogWindow extends PopupWidget implements LogListener
{            
    private int maxLength;
    
    public LogWindow(Window parent, int widgetWidth, int widgetHeight, int bgColor, int maxLength)
    {
        super(parent, widgetWidth, widgetHeight, bgColor);
        
        this.maxLength = maxLength;
    }
    
    public void log(String message)
    {
        add(new LogPopupWidget(parent,message,widgetWidth-(4*padding),widgetHeight),0);
    
        if ( size() > maxLength )
        {
            setSize(maxLength);
        }
    }
}
