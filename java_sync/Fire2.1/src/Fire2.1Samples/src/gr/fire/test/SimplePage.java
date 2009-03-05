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
package gr.fire.test;

import gr.fire.browser.Browser;
import gr.fire.browser.util.Page;
import gr.fire.core.CommandListener;
import gr.fire.core.Component;
import gr.fire.core.FireScreen;
import gr.fire.core.Panel;
import gr.fire.ui.FireTheme;
import gr.fire.util.Log;

import java.io.IOException;

import javax.microedition.io.HttpConnection;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

/**
 * A simple sample application that Loads an html file stored inside the jar and display's it.
 * Check the comments inside the source code for details on the writing similar applications.
 * 
 * @author padeler
 *
 */
public class SimplePage extends MIDlet implements CommandListener
{
	private Command exit;
	private Browser b;
	
	public SimplePage()
	{
	}

	protected void destroyApp(boolean arg0) throws MIDletStateChangeException
	{
		FireScreen.getScreen().destroy();
	}

	protected void pauseApp()
	{
	}

	protected void startApp() throws MIDletStateChangeException
	{
		// initialize fire screen
		FireScreen screen = FireScreen.getScreen(Display.getDisplay(this));
		screen.setFullScreenMode(true); // on full screen mode
		try
		{
			// load a theme file. 
			FireScreen.setTheme(new FireTheme("file://theme.properties"));
		} catch (IOException e)
		{
			e.printStackTrace();
		}
		// initialize a browser instance
		b = new Browser();
		exit = new Command("Exit",Command.EXIT,1); 
		
		try
		{
			// use the browser to load a page from the jar
			Page p = b.loadPage("file://help.html",HttpConnection.GET,null,null);
			// create a panel to display that page 
			Panel panel = new Panel(p.getPageContainer(),Panel.HORIZONTAL_SCROLLBAR|Panel.VERTICAL_SCROLLBAR,true);
			panel.setCommandListener(this); // listen for events on this panel
			panel.setLeftSoftKeyCommand(exit); // such as an exit softkey
			panel.setDragScroll(true); // This enables the Drag scroll function for this Panel.
			panel.setLabel(p.getPageTitle()); // The html page has a title tag, display it as a label on the panel
			screen.setCurrent(panel); // show the panel on the screen.
		} catch (Exception e)
		{
			// Use the Log class of the fire utility classes to easily log errors.
			// Check the BrowserTest.java application and the javadoc for more info on 
			// the Log class and the Logger interface.
			Log.logError("Failed to load Pane.",e);
		}
	}
	

	public void commandAction(javax.microedition.lcdui.Command c, Component cmp)
	{
		
		if(c==exit)
		{
			notifyDestroyed();
			return;			
		}
	}

	public void commandAction(javax.microedition.lcdui.Command arg0, Displayable arg1)
	{
	}

}
