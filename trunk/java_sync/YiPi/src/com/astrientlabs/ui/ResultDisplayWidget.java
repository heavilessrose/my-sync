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


public class ResultDisplayWidget extends Widget
{   
    private int padding = 2;

 
    private Image image;
    private String message;
    
    public ResultDisplayWidget(Window parent, int height)
    {
        super(parent);
       
        widgetHeight = height;
        widgetWidth = getWidth();
    }
    
    public void setImage(Image image, String message)
    {
        this.image = image;
        this.message = message;
    }
    
    public void paint(Graphics g, int x, int y, int maxX, int maxY, boolean hasFocus)
	{
        startY = y;

        g.setColor(0);
        g.fillRect(x,y,getWidth(),widgetHeight);


        if ( image != null )
        {
            g.drawImage(image,getWidth()>>1,y + (widgetHeight>>1),Graphics.VCENTER | Graphics.HCENTER);
        }
        
        if ( message != null ) 
        {
            g.setColor(Colors.TEXT);
            g.drawString(message,x+(widgetWidth>>1),y+padding,Graphics.TOP|Graphics.HCENTER);
        }
    }
    
    
    public void keyPressed(int keyCode)
    {        
        switch ( parent.getGameAction(keyCode) )
        {
            default : parent.handleKeyPressed(keyCode);
        }   
    }
}