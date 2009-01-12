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

import com.astrientlabs.colors.Colors;
import com.astrientlabs.fonts.Fonts;
import com.astrientlabs.text.StringVector;


public class LogPopupWidget extends Widget
{   
    private String message;    
    private int padding = 2;  
    private StringVector sv;
 
    public LogPopupWidget(Window parent, String message, int widgetWidth, int widgetHeight)
    {
        super(parent);
        
        this.message = message;
        
        this.widgetHeight = widgetHeight;
        this.widgetWidth = widgetWidth;
        
        setText(message);
    }
    
    public void setText(String text)
    {
        sv = new StringVector(Fonts.TEXT,text,widgetWidth);
    }
        
    public void paint(Graphics g, int x, int y, int maxX, int maxY, boolean hasFocus)
	{
        startY = y;
        startX = x;
        
        g.setColor(Colors.TEXT);
        g.setFont(Fonts.TEXT);
        
        sv.writeTo(g,x+padding,y+padding,widgetHeight,Fonts.TEXT);
    }
    
    public String getMessage()
    {
        return message;
    }
}