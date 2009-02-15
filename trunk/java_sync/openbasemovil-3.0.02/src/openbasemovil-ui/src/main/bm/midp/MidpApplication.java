/*
 * Copyright (c) 2006 Elondra S.L. All Rights Reserved.
 */
package bm.midp;

/* -----------------------------------------------------------------------------
 OpenBaseMovil User Interface Library
 Copyright (C) 2004-2008 Elondra S.L.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.
 If not, see <a href="http://www.gnu.org/licenses">http://www.gnu.org/licenses</a>.
 ----------------------------------------------------------------------------- */

import bm.core.*;
import bm.core.event.Event;
import bm.core.event.EventListener;
import bm.core.mvc.AbstractController;
import bm.core.mvc.ControllerEvent;
import bm.core.mvc.View;
import bm.core.res.PropertyResourceProvider;
import bm.db.DBException;
import bm.db.Settings;
import bm.mvc.PromptView;

import javax.microedition.lcdui.*;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;
import java.io.IOException;

/*
 * File Information
 *
 * Created on       : 23-may-2005 17:11:11
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2006/01/03 13:17:46 $
 * Revision         : $Revision: 1.2 $
 */

/**
 * Base J2ME Application.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 1.2 $
 */
public abstract class MidpApplication extends MIDlet implements
		ApplicationManager, CommandFactory, EventListener {
	protected static MidpApplication instance;

	protected static Command backCommand;
	protected static Command helpCommand;
	protected static Command exitCommand;
	protected static Command cancelCommand;
	protected static Command okCommand;
	protected static Command stopCommand;
	protected static Command homeCommand;

	protected Display display;
	protected View mainView;
	protected Properties defaultProperties;

	/**
	 * Protected constructor, initializes static instance.
	 * 
	 * @param bigSplashSize
	 *            horizontal resolution that triggers the use of the big splash
	 *            image
	 */
	protected MidpApplication(final int bigSplashSize) {
		instance = this;
		display = Display.getDisplay(this);
		loadProperties();
		showSplash(bigSplashSize);
		registerResourceBoundles();
		Event.register(this, Event.LANGUAGE_CHANGE);
		Application.setCommandFactory(this);
		Application.setManager(this);
		initCommands();
	}

	/**
	 * Load the splash image.<br/>
	 * If screen width is over bigSplashSize pixels, it will try to load the
	 * resource path from the default property &quot;splash.big.image&quot;,
	 * otherwise it will try &quot;splash.small.image&quot;. If the appropiate
	 * image is not defined, it will default to &quot;splash.image&quot;. If
	 * none of them are defined, no splash image is loaded.
	 * 
	 * @param bigSplashSize
	 *            horizontal resolution that triggers the use of the big splash
	 *            image
	 */
	protected void showSplash(final int bigSplashSize) {
		System.out.println("showSplash");
		try {
			System.out.println("showSplash");
			String splash = null;
			System.out.println("showSplash");
			final Form form = new Form("");
			System.out.println("showSplash");
			final int width = form.getWidth();
			if (width > bigSplashSize) {
				splash = defaultProperties.getProperty("splash.big.image");
			} else {
				splash = defaultProperties.getProperty("splash.small.image");
			}
			if (splash == null) {
				splash = defaultProperties.getProperty("splash.image");
			}
			if (splash != null) {
				final Image image = Image.createImage(getClass()
						.getResourceAsStream(splash));
				form.append(new ImageItem("", image, Item.LAYOUT_CENTER
						| Item.LAYOUT_VCENTER, "", Item.PLAIN));
				display.setCurrent(form);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * If the application gets blocked in a progress view there can be a method
	 * for restoring the best view for the application in its current state.<br/>
	 * This default implementation returns the MainView.
	 * 
	 * @return the best view to show in the current application state.
	 */
	public View getBestView() {
		return mainView;
	}

	/**
	 * Register resource bundles during application startup.<br/>
	 * It registers a single resource bundle called &quot;language&quot; which
	 * should include all the resources needed for the application.<br/>
	 * You should pack all the resources in that bundle and not override this
	 * method.
	 */
	protected void registerResourceBoundles() {
		ResourceManager.addProvider(new PropertyResourceProvider("language"));
	}

	protected void initCommands() {
		backCommand = new Command(ResourceManager.getResource("global.Back"),
				Command.BACK, 1);
		helpCommand = new Command(ResourceManager.getResource("global.Help"),
				Command.HELP, 2);
		exitCommand = new Command(ResourceManager.getResource("global.Exit"),
				Command.EXIT, 1);
		okCommand = new Command(ResourceManager.getResource("global.Ok"),
				Command.OK, 1);
		stopCommand = new Command(ResourceManager.getResource("global.Stop"),
				Command.STOP, 1);
		homeCommand = new Command(ResourceManager.getResource("global.Home"),
				Command.SCREEN, 1);
		cancelCommand = new Command(ResourceManager
				.getResource("global.Cancel"), Command.CANCEL, 1);
	}

	/**
	 * Listens to language change events to reload commands and progress view.
	 * 
	 * @param event
	 *            event to handle
	 */
	public void handleEvent(final Event event) {
		initCommands();
	}

	/**
	 * Get application instance.
	 * 
	 * @return application instance
	 */
	public static MidpApplication getInstance() {
		return instance;
	}

	/**
	 * Get the application Display.
	 * 
	 * @return display
	 */
	public Display getDisplay() {
		return display;
	}

	/**
	 * Exit the application.
	 */
	public void exitApplication() {
		try {
			destroyApp(true);
			notifyDestroyed();
		} catch (MIDletStateChangeException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Ask user if he/she want's to exit application and call exitApplication if
	 * he/she answers yes, do nothing if not.
	 */
	public void mildExitApplication() {
		final boolean result = confirm(ResourceManager
				.getResource("alert.exitApplication"), ResourceManager
				.getResource("alert.confirmExit"));
		if (result) {
			exitApplication();
		}
	}

	public View getMainView() {
		return mainView;
	}

	public void setMainView(final View mainView) {
		this.mainView = mainView;
	}

	/**
	 * Utility mehtod that shows a warning alert.
	 * 
	 * @param message
	 *            message to show
	 */
	public void showWarning(final String message) {
		showMessage(ResourceManager.getResource("alert.attention"), message,
				AlertType.WARNING);
	}

	/**
	 * Utility mehtod that shows an information alert.
	 * 
	 * @param message
	 *            message to show
	 */
	public void showInfo(final String message) {
		showMessage(ResourceManager.getResource("alert.info"), message,
				AlertType.INFO);
	}

	/**
	 * Utility mehtod that shows an error alert.
	 * 
	 * @param message
	 *            message to show
	 */
	public void showError(final String message) {
		showMessage(ResourceManager.getResource("alert.error"), message,
				AlertType.ERROR);
		AbstractController.hideProgressView();
	}

	/**
	 * Utility mehtod that shows an error alert.
	 * 
	 * @param message
	 *            message to show
	 * @param e
	 *            exception to show
	 */
	public void showError(final String message, final Exception e) {
		showMessage(ResourceManager.getResource("alert.error"), message + " ["
				+ getExceptionPresentation(e) + "]", AlertType.ERROR);
		AbstractController.hideProgressView();
	}

	/**
	 * Utility mehtod that shows an error alert.
	 * 
	 * @param e
	 *            exception to show
	 */
	public void showError(final Exception e) {
		showMessage(ResourceManager.getResource("alert.error"),
				getExceptionPresentation(e), AlertType.ERROR);
		AbstractController.hideProgressView();
	}

	private String getExceptionPresentation(final Exception e) {
		if (e instanceof J2MEException) {
			return ((J2MEException) e).getErrorNumber() + " ("
					+ ((J2MEException) e).getTrace() + ")";
		} else {
			return e.getClass().getName() + ": " + e.getMessage();
		}
	}

	private void showMessage(final String title, final String message,
			final AlertType alertType) {
		final Dialog dialog = new Dialog(title, message, Dialog.OK, alertType);
		dialog.confirm();
	}

	/**
	 * Utitliy method that displays a confirmation alert.<br/>
	 * It shows a modal alert and waits until the user presses yes o no, then
	 * the method returns.<br/>
	 * In fact, this is a short cut for Dialog class.
	 * 
	 * @param title
	 *            alert title
	 * @param message
	 *            alert message
	 * @return true if the user pressed yes, false if the user pressed no
	 */
	public boolean confirm(final String title, final String message) {
		final Dialog dialog = new Dialog(title, message);
		return dialog.confirm();
	}

	/**
	 * Display a modal dialog that allows the user to select one of the given
	 * options.
	 * 
	 * @param title
	 *            dialog title
	 * @param options
	 *            options array
	 * 
	 * @return index of the option selected
	 */
	public int select(final String title, final String[] options) {
		final SelectionDialog dialog = new SelectionDialog(title, options);
		return dialog.select();
	}

	/**
	 * Display a modal dialog that allows the user to select one of the given
	 * options.
	 * 
	 * @param title
	 *            dialog title
	 * @param options
	 *            options array
	 * @param ticker
	 *            ticker text
	 * 
	 * @return index of the option selected
	 */
	public int select(final String title, final String[] options,
			final String ticker) {
		final SelectionDialog dialog = new SelectionDialog(title, options,
				ticker);
		return dialog.select();
	}

	/**
	 * Get an application property. Properties are taken from several places
	 * with a given order of preference:<br/>
	 * <ol>
	 * <li>Preferences database</li>
	 * <li>Default application bundled properties</li>
	 * <li>JAD defined properties</li>
	 * <li>System properties</li>
	 * </ol>
	 * 
	 * @param key
	 *            property key
	 * @return property value or null if not found
	 */
	public String getProperty(final String key) {
		String value;
		try {
			value = Settings.getProperty(key);
			if (value != null) {
				return value;
			}
		} catch (DBException e) {
			e.printStackTrace();
		}

		value = defaultProperties.getProperty(key);
		if (value != null) {
			return value;
		}

		value = getAppProperty(key);
		if (value != null) {
			return value;
		}

		return System.getProperty(key);
	}

	public String getProperty(final String key, final String defaultValue) {
		final String value = getProperty(key);
		return value != null ? value : defaultValue;
	}

	public Integer getPropertyInt(final String key) {
		final String value = getProperty(key);
		if (value != null) {
			try {
				return new Integer(Integer.parseInt(value));
			} catch (NumberFormatException e) {
				e.printStackTrace();
				return null;
			}
		} else {
			return null;
		}
	}

	public int getPropertyInt(final String key, final int defaultValue) {
		final Integer value = getPropertyInt(key);
		return value != null ? value.intValue() : defaultValue;
	}

	public Long getPropertyLong(final String key) {
		final String value = getProperty(key);
		if (value != null) {
			try {
				return new Long(Long.parseLong(value));
			} catch (NumberFormatException e) {
				e.printStackTrace();
				return null;
			}
		} else {
			return null;
		}
	}

	public long getPropertyLong(final String key, final long defaultValue) {
		final Long value = getPropertyLong(key);
		return value != null ? value.longValue() : defaultValue;
	}

	public Boolean getPropertyBoolean(final String key) {
		String value = getProperty(key);
		if (value != null) {
			value = value.toLowerCase().trim();
			return new Boolean(value.equals("true") || value.equals("1")
					|| value.equals("si") || value.equals("yes"));
		} else {
			return null;
		}
	}

	public boolean getPropertyBoolean(final String key,
			final boolean defaultValue) {
		final Boolean value = getPropertyBoolean(key);
		return value != null ? value.booleanValue() : defaultValue;
	}

	public boolean isMIDP() {
		return true;
	}

	/**
	 * Lookup an object given it's url. This default implementation only answers
	 * to the resource <code>midlet://self</code> wich returns the object
	 * itself, i.e. a Midlet.<br/>
	 * This is useful for system dependent resources, such as printers,etc.<br/>
	 * For example, to retrieve a configured default printer:<br/>
	 * <code>Application.getManager().lookUp( "printer://default" );
	 * 
	 * @return object or null if not found
	 */
	public Object lookUp(final String url) {
		if (url != null) {
			if (url.equals("midlet://self")) {
				return this;
			} else {
				return null;
			}
		} else {
			return null;
		}
	}

	private void loadProperties() {
		try {
			defaultProperties = new Properties(getClass().getResourceAsStream(
					"/default.properties"));
			System.out.println(defaultProperties);
		} catch (IOException e) {
			e.printStackTrace();
			defaultProperties = null;
		}
	}

	/**
	 * Get the command for back.
	 * 
	 * @return back command
	 */
	public Command backCommand() {
		return backCommand;
	}

	/**
	 * Get the command for help.
	 * 
	 * @return help command
	 */
	public Command helpCommand() {
		return helpCommand;
	}

	/**
	 * Get the command for exit.
	 * 
	 * @return exit command
	 */
	public Command exitCommand() {
		return exitCommand;
	}

	public Command cancelCommand() {
		return cancelCommand;
	}

	public Command okCommand() {
		return okCommand;
	}

	public Command stopCommand() {
		return stopCommand;
	}

	public Command homeCommand() {
		return homeCommand;
	}

	/**
	 * Handles default commands: back, cancel, exit, help, ok, stop, home.
	 * 
	 * @param id
	 *            command id
	 * @return command or null if not one of the default commands.
	 */
	public Command getCommand(final int id) {
		switch (id) {
		case Command.BACK:
		case ControllerEvent.BACK:
			return backCommand;

		case Command.CANCEL:
		case ControllerEvent.CANCEL:
			return cancelCommand;

		case Command.EXIT:
		case ControllerEvent.EXIT:
			return exitCommand;

		case Command.HELP:
		case ControllerEvent.HELP:
			return helpCommand;

		case Command.OK:
		case ControllerEvent.ACCEPT:
			return okCommand;

		case Command.STOP:
		case ControllerEvent.STOP:
			return stopCommand;

		case ControllerEvent.HOME:
			return stopCommand;

		default:
			return null;
		}
	}

	public String prompt(String title, int maxSize, int constraints) {
		return new PromptView(title, maxSize, constraints).prompt();
	}

	public String prompt(String title, int maxSize) {
		return prompt(title, maxSize, TextField.ANY);
	}

	public String promptPassword(String title) {
		return promptPassword(title, 32);
	}

	public String promptPassword(String title, final int maxSize) {
		return prompt(title, maxSize, TextField.PASSWORD);
	}

	public String prompt(String title) {
		return prompt(title, 256);
	}
}
