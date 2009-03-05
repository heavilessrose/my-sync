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
import gr.fire.core.GridLayout;
import gr.fire.core.KeyListener;
import gr.fire.core.Panel;
import gr.fire.ui.FireTheme;
import gr.fire.ui.InputComponent;
import gr.fire.util.Log;

import java.io.IOException;
import java.util.Enumeration;
import java.util.Stack;

import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Font;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

/**
 * This Midlet is a very simple graphical calculator. It's purpose is to demonstrate tha use of the 
 * GridLayout as well as the KeyListener and CommandListener. 
 * Read inside the source code for comments and a walkthrough on the development of the application.
 *  
 * @author padeler
 *
 */
public class SimpleCalc extends MIDlet implements CommandListener, KeyListener
{
	// Some commands exit, exits the application
	private gr.fire.browser.util.Command exit = new gr.fire.browser.util.Command("Exit",Command.EXIT,1);
	// clear clears the calculator's memory
	private gr.fire.browser.util.Command clear = new gr.fire.browser.util.Command("Clear",Command.CANCEL,1);
	// every other button is going to be associated with the b command
	private gr.fire.browser.util.Command b = new gr.fire.browser.util.Command("");
	
	private Stack stack; // a stack containing the numbers and symbols entered by the user
	private InputComponent numField; // the input field that displays the results etc.
	
	protected void destroyApp(boolean arg0) throws MIDletStateChangeException
	{
		FireScreen.getScreen().destroy(); // this method informs the animation thread to die. 
	}

	protected void pauseApp()
	{

	}

	protected void startApp() throws MIDletStateChangeException
	{
		// initialize the firescreen using the Display 
		FireScreen screen = FireScreen.getScreen(Display.getDisplay(this));
		screen.setFullScreenMode(true); // use the fireScreen in fullscreen mode.	 
		
		try
		{ // load a theme. This is not necessery, but it improves the visual result a lot.
			FireScreen.setTheme(new FireTheme("file://theme.properties"));
		} catch (IOException e)
		{
			Log.logError("Failed to load theme.",e);
		}
		
		stack = new Stack();
		stack.push(new Long(0));
		
		/*
		 * Our user interface will consist of the following
		 * - An InputComponent of type TEXT (a textfield) that will be the "LED screen" of the calculator called numField
		 * - A number of InputComponents of type BUTTON that will be the Buttons of the calculator
		 * - A container for the buttons called numPad 
		 * - A container the numField and the numPad
		 */
		
		// first create the external container that will hold the numPad and the numField 
		// I will use BoxLayout set on Y_AXIS to layout the componens inside it one below of the other.
		Container calc = new Container(new BoxLayout(BoxLayout.Y_AXIS));
		// add a couple of commands assigned to the left and right softkey for the Container
		calc.setLeftSoftKeyCommand(clear); 
		calc.setRightSoftKeyCommand(exit);
		// this class implements the CommandListener interface. I am setting the commandListener of the calc container
		// to be this class.
		calc.setCommandListener(this); 
		
		// Ok now create the numField
		numField = new InputComponent(InputComponent.TEXT);
		numField.setLayout(FireScreen.RIGHT|FireScreen.VCENTER); // this is usually the layout calculator screens display numbers
		Font numFieldFont = Font.getFont(Font.FACE_SYSTEM,Font.STYLE_BOLD,Font.SIZE_LARGE);
		numField.setFont(numFieldFont); // set a nice big font for the screen
		// Set a prefered size. The boxLayout will use this size properly place the components inside the container
		numField.setPrefSize(screen.getWidth(),numFieldFont.getHeight()*2);   
		
		// have all key events inside the numField be handled by this KeyListener 
		// I want that in order to allow the user to be able to use the keypad
		// in order to enter numbers as well as using the interface buttons
		numField.setKeyListener(this);
	
		// add the numField to the container
		calc.add(numField);
		
		// create a Container for all the buttons. 
		// For this container we use GridLayout it is set 
		// to 5 rows and 3 columns. 
		Container numPad = new Container(new GridLayout(5,3,5,5));
		InputComponent button;
		
		Font buttonFont = Font.getFont(Font.FACE_SYSTEM,Font.STYLE_BOLD,Font.SIZE_MEDIUM);
		
		String symbols[] = {"1","2","3","4","5","6","7","8","9", "*","0","=","+","-","/" };
		for(int i =0;i<symbols.length;++i)
		{
			button = new InputComponent(InputComponent.BUTTON); // InputComponent of type BUTTON
			button.setValue(symbols[i]); 
			button.setCommandListener(this); // this CommandListener waits for button presses
			button.setCommand(b); // the button will send the b command.
			if(i==9 || i>10)
			{
				button.setForegroundColor(0xFF0000);
			}
			button.setFont(buttonFont);
			button.setLayout(FireScreen.CENTER|FireScreen.VCENTER);
			numPad.add(button);
		}
		int numPadHeight = screen.getHeight() - numFieldFont.getHeight()*2;
		
		// if you want to have grid like selection of 
		// your components (up/down selects the prev/next component on the Y_AXIS and left/right on the X_AXIS) 
		// you need to have a container with Grid_Layout directly inside a Panel.
		// NOTE: This limitation is propably going to be removed in the feature versions of fire
		Panel numPane = new Panel(numPad,Panel.NO_SCROLLBAR,false);		
		numPane.setPrefSize(screen.getWidth(),numPadHeight);
		// add the numPad to the container
		calc.add(numPane);

		screen.setCurrent(calc);
	}

	public void commandAction(Command cmd, Component c)
	{
		if(cmd==b)
		{ // numPad button was pressed.
			String val = ((InputComponent)c).getValue();
			Log.logInfo("Button Pressed: "+val);
			int digit=-1;
			try{
				digit = Integer.parseInt(val);
				handleDigit(digit);
			}catch(NumberFormatException e) // not a digit, its a symbol
			{
				handleSymbol(val);
			}
			return;
		}
		
		if(cmd==clear)
		{ // softkey command clear send by container 
			stack.removeAllElements();
			stack.push(new Long(0));
			updateScreen();
			return;
		}
		if(cmd==exit)
		{ // softkey command exit send by container
			notifyDestroyed();
			return;
		}
	}
	
	private void handleDigit(int d)
	{
		Object o = stack.peek();
		if(o instanceof Long)
		{
			Long n = (Long)stack.pop();
			n = new Long(n.longValue()*10 + d);
			stack.push(n);
			
		}
		else 
		{
			stack.push(new Long(d));
		}
		
		updateScreen();
	}
	
	private void handleSymbol(String symbol)
	{
		Object o = stack.peek();
		if(o instanceof Long)
		{
			if(symbol.equals("="))
			{
				calculate();
			}
			else 
			{
				calculate();
				stack.push(symbol);
			}
			updateScreen();
		}
	}
	
	private void calculate()
	{
		if(stack.size()>=3) // we need 2 numbers and 1 symbol 
		{
			if(stack.peek() instanceof Long)
			{
				Long n2 = (Long)stack.pop(); // pop first number
				String symbol = (String)stack.pop(); // pop symbol
				Long n1 = (Long)stack.pop(); // pop second number
				Long res=null;
				if(symbol.equals("+"))
				{
					res = new Long(n1.longValue()+n2.longValue());
					stack.push(res);
				}
				else if(symbol.equals("-"))
				{
					res = new Long(n1.longValue()-n2.longValue());
					stack.push(res);
				}
				else if(symbol.equals("/"))
				{
					res = new Long(n1.longValue()/n2.longValue());
					stack.push(res);
				}
				else if(symbol.equals("*"))
				{
					res = new Long(n1.longValue()*n2.longValue());
					stack.push(res);
				}
				Log.logInfo(n1+" "+symbol+" "+n2+" = " + res);
				
				updateScreen();
			}
		}
	}
	
	private void updateScreen()
	{
		StringBuffer numFieldValue = new StringBuffer();
		Enumeration e = stack.elements();
		while(e.hasMoreElements())
		{
			numFieldValue.append(e.nextElement());
		}
		// setting the value to the InputComponent will cause the component 
		// to request a repaint from the FireScreen.
		numField.setValue(numFieldValue.toString());
	}

	public void commandAction(Command arg0, Displayable arg1)
	{
	}

	public void keyPressed(int code, Component src)
	{	
	}

	public void keyReleased(int code, Component src)
	{
		// when a key is released and the focus is on the numField InputComponent, 
		// the event will be send to the associated KeyListener. 
		if(src==numField) // only listen key events when the selected component if numField
		{// if the selected component is the numfield the user can type directly a number using the keypad of hid phone.
			if(code==Canvas.KEY_NUM0) handleDigit(0);
			else if(code==Canvas.KEY_NUM1) handleDigit(1);
			else if(code==Canvas.KEY_NUM2) handleDigit(2);
			else if(code==Canvas.KEY_NUM3) handleDigit(3);
			else if(code==Canvas.KEY_NUM4) handleDigit(4);
			else if(code==Canvas.KEY_NUM5) handleDigit(5);
			else if(code==Canvas.KEY_NUM6) handleDigit(6);
			else if(code==Canvas.KEY_NUM7) handleDigit(7);
			else if(code==Canvas.KEY_NUM8) handleDigit(8);
			else if(code==Canvas.KEY_NUM9) handleDigit(9);
		}
	}

	public void keyRepeated(int code, Component src)
	{
	}
}
