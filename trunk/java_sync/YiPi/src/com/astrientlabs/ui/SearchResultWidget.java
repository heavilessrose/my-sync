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
import com.astrientlabs.search.SearchResult;
import com.astrientlabs.text.Strings;
import com.astrientlabs.util.Images;


public class SearchResultWidget extends Widget
{      
    private int padding = 3;  
    private SearchResult searchResult;
    private Image image;
    private int thumbnailWidth = 30;
    private int maxTextLen;
    private int borderSize = 3;
 
    public SearchResultWidget(Window parent, SearchResult searchResult)
    {
        super(parent);
        
        this.searchResult = searchResult;
        widgetWidth = getWidth();
        widgetHeight = (2*padding) + (2*Fonts.TEXT.getHeight()) + (2*padding);
        
        //TODO: large display vs small;
        maxTextLen = ( widgetWidth >= 240 ) ? 35 : 15;
    }
           
    public void paint(Graphics g, int x, int y, int maxX, int maxY, boolean hasFocus)
	{
        startY = y;
        startX = x;
        
        
        g.setFont(Fonts.TEXT);
        if ( hasFocus )
        {
            //draw bg
            g.setColor(Colors.HIGHTLIGHT_BG);
            g.fillRect(x,y,maxX-x,Math.min(widgetHeight,maxY-y));
            
            //draw border
            g.setColor(0xFFFFFF);
            g.drawRect(x,y,maxX-x,Math.min(widgetHeight,maxY-y));
            

            
            //draw text
            textColor = 0x000000;
        }
        else
        {
            g.setColor(Colors.HORIZONTAL_RULE);
            g.drawLine(x,y+Math.min(widgetHeight,maxY-y),maxX-x,y+Math.min(widgetHeight,maxY-y));
            textColor = Colors.TEXT;
        }
        
        //draw image
        if ( image == null )
        {
            g.drawImage(Images.PICTURE, x + padding , y + (widgetHeight>>1), Graphics.LEFT| Graphics.VCENTER);
        }
        else
        {
            g.drawImage(image, x + padding, y + (widgetHeight>>1), Graphics.LEFT | Graphics.VCENTER);
            g.setColor(0xFFFFFF);
            g.drawRect(x + padding, y + (widgetHeight>>1) - (image.getHeight()>>1), image.getWidth() -1, image.getHeight() -1);
        }
        
        
        g.setColor(textColor);
        g.drawString(Strings.getText(searchResult.getTitle(),maxTextLen,"..."),x+padding+thumbnailWidth+padding+1,y+padding+borderSize,Graphics.TOP|Graphics.LEFT);
        g.drawString(Strings.getText(searchResult.getSizeString(),maxTextLen,"..."),maxX-padding,y+padding+borderSize,Graphics.TOP|Graphics.RIGHT);
        g.drawString(Strings.getText(searchResult.getUrl(),maxTextLen,"..."),x+padding+thumbnailWidth+padding+1,y+borderSize+padding+g.getFont().getHeight()+padding,Graphics.TOP|Graphics.LEFT);
    }
    
    public void setImage(Image image)
    {
        this.image = image;
        widgetHeight = (2*padding) + Math.max(image.getHeight(),2*Fonts.TEXT.getHeight()+2*padding);
        if ( parent.isShown() ) parent.repaintChild(this, false);
    }
    
    public SearchResult getSearchResult()
    {
        return searchResult;
    }   
}