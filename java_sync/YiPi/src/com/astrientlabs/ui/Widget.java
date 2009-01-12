/*
 * Copyright (C) 2006 Astrient Labs, LLC
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.ui;

import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Graphics;

public abstract class Widget
{
    public static final int POSITION_ABSOLUTE = 0;
    public static final int POSITION_RELATIVE = 1;
    
    protected int xyFloat =  POSITION_RELATIVE;
    
    protected int startY;
    protected int startX;
    protected int widgetHeight;
    protected int widgetWidth;
    
    protected int textColor;
    protected int bgColor;
    
    protected String name = "";
    protected Window parent = null;
    
    
    
    
    public Widget(Window parent)
    {
        this.parent = parent;
    }
    
    public boolean supportsFocus()
    {
        return true;
    }
    
    public int getWidth()
    {
        return parent.getWindowWidth();
    }

    public void keyPressed(int keyCode)
    {        
        switch ( parent.getGameAction(keyCode) )
        {
            case Canvas.FIRE : parent.widgetSelected(this,this);break;
            case Canvas.UP : parent.moveFocus(false); break;
            case Canvas.DOWN : parent.moveFocus(true); break;
            case Canvas.LEFT : parent.moveFocus(false); break;
            case Canvas.RIGHT : parent.moveFocus(true); break;
            default : parent.handleKeyPressed(keyCode);
        }   
    }

    public void keyRepeated(int keyCode)
    { 
    }
    
    public void keyReleased(int keyCode)
    {
    }
    
    public int getWidgetWidth()
    {
        return this.widgetWidth;
    }
    
    public int getWidgetHeight()
    {
        return this.widgetHeight;
    }
    
    public abstract void paint(Graphics g, int x, int y, int maxX, int maxY, boolean hasFocus);
    
    public void onFocusEnter()
    {
    }
    
    public void onFocusExit()
    {
    }
}
