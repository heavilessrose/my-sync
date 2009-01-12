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
import javax.microedition.lcdui.Canvas;

import com.astrientlabs.colors.Colors;
import com.astrientlabs.fonts.Fonts;
import com.astrientlabs.text.LineEnumeration;


public class PopupWidget extends ListWidget
{            
    protected boolean linger = false;
    
    public PopupWidget(Window parent, int widgetWidth, int widgetHeight, int bgColor)
    {
        super(parent,widgetHeight,bgColor);
        
        this.widgetWidth = widgetWidth;
        this.widgetHeight = widgetHeight;
        this.scrollbarWidth = 4;
        this.padding = 4;
        modal = false;
    }
    
    
    public void keyPressed(int keyCode)
    {
        int action = parent.getGameAction(keyCode);
        
        switch (action)
        {
            case Canvas.FIRE : parent.clearPopup();
                     break;
            case Canvas.UP : previous(); 
                      break;
            case Canvas.DOWN : next();
                        break;
            case Canvas.LEFT : previous(); 
                        break;
            case Canvas.RIGHT : next();
                        break;                        
            default :widgetEvent(keyCode);
        }
    }
        
    public void paint(Graphics g, int x, int y, int maxX, int maxY, boolean hasFocus)
    {
        g.setColor(bgColor);
        g.fillRect(x,y,widgetWidth,widgetHeight);
        
        super.paint(g,x,y,maxX,maxY,hasFocus);
        g.setColor(Colors.POPUP_BORDER);
        g.drawRect(x,y,widgetWidth,widgetHeight);
    }
    
    public void setText(String text)
    {
        clear();
        LineEnumeration le = new LineEnumeration(Fonts.TEXT,text,widgetWidth-padding-scrollbarWidth-padding);
        LabelWidget widget;
        for ( ; le.hasMoreElements(); )
        {
            widget = new LabelWidget(parent,le.nextElement().toString().trim());
            widget.bgColor = this.bgColor;
            add(widget);
        }
    }
}
