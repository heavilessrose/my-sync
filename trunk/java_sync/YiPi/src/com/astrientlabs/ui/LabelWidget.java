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


public class LabelWidget extends Widget
{   
    protected int padding = 2;
    protected String text;

    
    public LabelWidget(Window parent, String text)
    {
        super(parent);
        
        this.text = text;
        widgetHeight = (2*padding) + Fonts.TEXT.getHeight();
        widgetWidth = getWidth();
        
        textColor = Colors.TEXT;
        bgColor = Colors.BACKGROUND;
    }
        
    public void paint(Graphics g, int x, int y, int maxX, int maxY, boolean hasFocus)
	{
        startY = y;
        startX = x;
        g.setFont(Fonts.TEXT);
        g.setColor(bgColor);
        //g.fillRect(x,y,widgetWidth,widgetHeight-1);

        g.setColor(textColor);
        g.drawString(text,x+padding,y+padding,Graphics.LEFT|Graphics.TOP);   
        
        if ( hasFocus )
        {
            g.setStrokeStyle(Graphics.DOTTED);
            g.setColor(Colors.HIGHLIGHT_LINE);
            g.drawLine(x+padding,y+padding+Fonts.TEXT.getHeight(),x+padding+Fonts.TEXT.stringWidth(text),y+padding+Fonts.TEXT.getHeight());
            g.setStrokeStyle(Graphics.SOLID);
        }
    }
    
    public boolean supportsFocus()
    {
        return false;
    }
}