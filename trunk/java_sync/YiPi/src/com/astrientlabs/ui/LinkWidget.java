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

import com.astrientlabs.colors.Colors;
import com.astrientlabs.fonts.Fonts;


public class LinkWidget extends Widget
{   
    private int padding = 2;  
    private Image image;
    private int textX;
    private Object value;
    
    public LinkWidget(Window parent, String name, Image image, Object value)
    {
        super(parent);
        
        this.image = image;
        this.name  = name;
        this.value = value;
        
        if ( image == null ) 
        {
            textX = padding;
            widgetHeight = (2*padding) + Fonts.TEXT.getHeight();
        }
        else
        {
            textX = padding+image.getWidth()+padding;
            widgetHeight = (2*padding) + Math.max(image.getHeight(),Fonts.TEXT.getHeight());
        }
        
        widgetWidth = getWidth();
    }
    
        
    public void paint(Graphics g, int x, int y, int maxX, int maxY, boolean hasFocus)
    {
        startY = y;
        startX = x;
        
        if ( hasFocus )
        {
            g.setFont(Fonts.ROLLOVER_TEXT);
        }
        else
        {
            g.setFont(Fonts.TEXT);
        }
        
        g.setColor(Colors.HIGHLIGHT_TEXT);
        
        
        if ( image != null ) g.drawImage(image,x+padding,y+(widgetHeight>>1),Graphics.VCENTER| Graphics.LEFT);
        g.drawString(name,x+textX,y+padding,Graphics.TOP|Graphics.LEFT);
    }


    public Object getValue()
    {
        return value;
    }
}
