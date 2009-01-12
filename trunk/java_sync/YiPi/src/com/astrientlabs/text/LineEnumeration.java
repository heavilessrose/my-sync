/*
 * Copyright (C) 2006 Astrient Labs, LLC Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance 
 * with the License. You may obtain a copy of the License at 
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the 
 * License for the specific language governing permissions and limitations under
 * the License.
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.text;

import java.util.Enumeration;
import java.util.NoSuchElementException;

import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;


public class LineEnumeration implements Enumeration
{
	private Font font;
	private String text;
	
	private int width;
	private int position;
	private int length;
	private int start = 0;

	public LineEnumeration(Font font, String text, int width)
	{
	    this.font = font;
		this.text = text;
		this.width = width;
		this.length = text.length();
	}
	
	
	public boolean hasMoreElements()
	{
	    return (position < (length-1));
	}
	
	public Object nextElement() throws NoSuchElementException
	{
	    try
	    {
	        int next = next();
	        
	        String s = text.substring(start,next);        
	        start = next;
	        
	        if ( text.length()-1 > start && ( text.charAt(start) == ' ' || text.charAt(start) == '\n' ) )
	        {
	            position++;
	            start++;
	        }
	                
	        return s;
	    }
	    catch ( IndexOutOfBoundsException e )
	    {
	        throw new NoSuchElementException(e.getMessage());
	    }
	    catch ( Exception e )
	    {
	        throw new NoSuchElementException(e.getMessage());
	    }
	}
	

	private int next()
	{	    
		int i = position;	
		int lastBreak = -1;
		
		for ( ;i < length && font.stringWidth(text.substring(position,i)) <= width; i++ )
		{
		    if ( text.charAt(i) == ' ' )
		    {
		        lastBreak = i;
		    }
		    else if ( text.charAt(i) == '\n' )
		    {
		        lastBreak = i;
		        break;
		    }  
		}
		
		
		if ( i == length )
		{
		    position = i;
		}
		else if ( lastBreak == position )
		{
		    position++;
		}
		else if ( lastBreak < position )
		{
		    position = i;
		}
		else
		{
		    position = lastBreak;
		}
				
		return position;
	}
	
	
	public int writeTo(Graphics g, int startx, int starty, int maxY, Font font)
	{
	    int fontHeight = font.getHeight()+1;
	    
		while ( hasMoreElements() && starty < maxY ) 
		{	
			g.drawString(String.valueOf(nextElement()).trim(), startx, starty, Graphics.TOP | Graphics.LEFT);
			starty += fontHeight;
		}
		
	    return starty;
	}
	
	public void reset()
	{
	    start = 0;
	    position = 0;
	}
}