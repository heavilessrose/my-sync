/*
 * Fire (Flexible Interface Rendering Engine) is a set of graphics widgets for creating GUIs for j2me applications. 
 * Copyright (C) 2006-2008 Bluevibe (www.bluevibe.net)
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */
package gr.fire.ui;

import gr.fire.core.Animation;
import gr.fire.core.Component;

import java.util.Vector;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

/**
 * @author padeler
 *
 */
public class Marquee extends Animation
{
	
	public static final long MILISECONDS_PER_FRAME=100;
	
	public static final int SCROLL_PERCENT = 5; // percent of the components width to scroll in each frame
	public static final int SPONSOR_SCROLL_PERCENT = 50;// percent of the sponsor's image width to scroll in and out of the frame
	
	
	public static final int LOGO_RIGHT_OFFSET = 40;
	public static final long LOGO_PAUSE = 3000;
	
	public static final int sponsorYOffset = 1;
	
	
	private long lastFrame;
	private long pause;
	
	private int sponsorId=0;
	
	private int scrollStep=0;
	
	private int logoXOffset=0;
	
	private Image logo,tile;
	private Vector sponsors;

	private boolean running=true;

	public boolean isRunning()
	{
		return running;
	}

	public void paint(Graphics g)
	{
		/* scroll mode */
		// first paint the logo.
		g.drawImage(logo,logoXOffset,0,Graphics.TOP|Graphics.LEFT);
		// then paint the tile image on the right of the logo until the end of the screen.
		if(tile!=null)
		{
			int tileWidth = tile.getWidth();
			for(int offX = logoXOffset+logo.getWidth();offX<width;offX+=tileWidth)
				g.drawImage(tile,offX,0,Graphics.TOP|Graphics.LEFT);
		}
		
		if(sponsorId>=0 && sponsorId<sponsors.size())
		{
			Image sp = (Image)sponsors.elementAt(sponsorId);
			int sponsorXOffset = 5;
			g.drawImage(sp,logoXOffset+width+sponsorXOffset,sponsorYOffset,Graphics.TOP|Graphics.LEFT);
		}
	}

	public Marquee(Component owner,Image logo, Image tile, Vector sponsors)
	{
		super(owner);
		this.logo = logo;
		this.tile = tile;
		this.sponsors = sponsors;
		width = owner.getPrefSize()[0];
		height = owner.getPrefSize()[1];
		
		scrollStep = -(width * SCROLL_PERCENT) / 100; // start by scrolling to the left
		
	}
	public boolean step()
	{
		long now = System.currentTimeMillis();
		
		if(now-lastFrame>=MILISECONDS_PER_FRAME && (now-pause>=LOGO_PAUSE))
		{
			lastFrame = now;
			logoXOffset += scrollStep;
			if(scrollStep<0 && logoXOffset<=-(logo.getWidth()-LOGO_RIGHT_OFFSET)) // left scroll
			{
				logoXOffset = -(logo.getWidth()-LOGO_RIGHT_OFFSET);
				pause = now; // pause for a while
				scrollStep = -scrollStep;
			}
			else if(scrollStep>0 && logoXOffset>=0)
			{
				logoXOffset = 0;
				pause = now; // pause for a while
				scrollStep = -scrollStep;
				sponsorId++;
				if(sponsorId>=sponsors.size()) sponsorId=0;
			}
			return true;
		}
		return false;
	}

	public void stop()
	{
		running=false;
	}

	public int getHeight()
	{
		return height;
	}

	public void setHeight(int height)
	{
		this.height = height;
	}

	public int getWidth()
	{
		return width;
	}

	public void setWidth(int width)
	{
		this.width = width;
	}
}
