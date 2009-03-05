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

import gr.fire.core.BoxLayout;
import gr.fire.core.CommandListener;
import gr.fire.core.Component;
import gr.fire.core.Container;
import gr.fire.core.FireScreen;
import gr.fire.core.Panel;
import gr.fire.ui.Alert;
import gr.fire.ui.FireTheme;
import gr.fire.ui.Row;
import gr.fire.util.Log;

import java.io.IOException;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Image;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;


/**
 * This is a sample application Demonstrating the use of the Row sample component as well as the Alert component. 
 * Check the RowTest.java file and the comments inside the code for more info in developing similar classes and 
 * applications.
 * 
 * 
 * @see Alert
 * @see Row
 * 
 * @author padeler
 *
 */
public class RowTest extends MIDlet implements CommandListener
{
	/* (non-Javadoc)
	 * @see javax.microedition.midlet.MIDlet#destroyApp(boolean)
	 */
	protected void destroyApp(boolean arg0) throws MIDletStateChangeException
	{
		FireScreen.getScreen().destroy();
	}

	/* (non-Javadoc)
	 * @see javax.microedition.midlet.MIDlet#pauseApp()
	 */
	protected void pauseApp()
	{
	}
		
	private Command exit = new Command("Exit",Command.OK,1);
	private Command alertCmd = new Command("Alert",Command.OK,1);
	private Command alertCallback = new Command("This is send by the alert component.",Command.OK,1);

	
	protected void startApp() throws MIDletStateChangeException
	{
			FireScreen screen = FireScreen.getScreen(Display.getDisplay(this));
			screen.setFullScreenMode(true);	

			try
			{
				FireScreen.setTheme(new FireTheme("file://theme.properties"));
			} catch (IOException e)
			{
				e.printStackTrace();
			}
			
			/*
			 * We will create a simple Container and add some Rows in it, to demonstrate similar 
			 * functionality as the one provided by the Row class in the Fire v1.2.
			 * 
			 */
			try{
				Container cnt = new Container(new BoxLayout(BoxLayout.Y_AXIS));
				cnt.add(new Row("This is a text only row."));
				cnt.add(new Row(Image.createImage("/fire.jpg")));
				cnt.add(new Row("The above is a image only row and this is an [text,image] row.",Image.createImage("/fire.jpg")));
				cnt.add(new Row(Image.createImage("/fire.jpg"),"Check the Row class for an example of compining the Fire2.0 UI primitives to create more complex components"));
				Row r = new Row(Image.createImage("/water.jpg"),"This row has a command action. It is also center alligned both horizontally and vertivally",false,screen.getWidth(),FireScreen.CENTER|FireScreen.VCENTER);;
				r.setCommand(alertCmd);
				r.setCommandListener(this);
				cnt.add(r);
				
				cnt.add(new Row("The above row when clicked will exit the application."));
				
				Panel panel = new Panel(cnt,Panel.VERTICAL_SCROLLBAR,true);
				panel.setRightSoftKeyCommand(exit);
				panel.setCommandListener(this);
				screen.setCurrent(panel);
			}catch(IOException e){
				Log.logError("Failed to load images.",e);
			}
	}
	
	public void commandAction(Command cmd, Component c)
	{
		Log.logInfo("Command "+cmd.getLabel());
		if(cmd==exit)
		{
			notifyDestroyed();
			return;
		}
		if(cmd==alertCmd)
		{
			FireScreen.getScreen().showAlert("This is an alert message of type yes no cancel. " +
					"An alert has a command associated with it and a CommandListener. " +
					"When a user presses a button on the Alert it will call the commandAction() method of the listener " +
					"using the associated command and itself as the parameter. " +
					"The developer can then find out about the userSelection using the alert.getUserSelection() method.",
					Alert.TYPE_YESNOCANCEL,Alert.USER_SELECTED_YES,alertCallback,this);
			return;
		}
		if(cmd==alertCallback)
		{
			Alert alert = ((Alert)c);
			byte sel = alert.getUserSelection();
			String answer=null;
			switch (sel){
			case Alert.USER_SELECTED_YES:
				answer = "YES";
				break;
			case Alert.USER_SELECTED_NO:
				answer = "NO";
				break;
			case Alert.USER_SELECTED_CANCEL:
				answer = "CANCEL";
				break;
			}
			Log.logInfo("User Selected: "+answer+" ("+alert.getUserSelection()+")");
			// ok now display the answer in another popup.
			// NOTE: You can display popups without the FireScreen.showAlert()
			// The showAlert method is only a utility method to provide you with 
			// a fast way to show a popup. Check the code in the fuction FireScreen.showAlert() 
			// to see how to create and display a popup manually. 
			FireScreen.getScreen().showAlert("You selected: "+answer,Alert.TYPE_INFO,Alert.USER_SELECTED_OK,null,null);
			return;
		}
	}

	public void commandAction(Command arg0, Displayable arg1)
	{
	}
	
}
