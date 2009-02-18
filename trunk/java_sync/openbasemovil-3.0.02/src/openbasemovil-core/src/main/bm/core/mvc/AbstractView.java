package bm.core.mvc;

import bm.core.Application;
import bm.core.ResourceManager;
import bm.core.math.FixedPoint;
import bm.core.tools.DeviceInfo;

import javax.microedition.lcdui.*;
import java.util.Hashtable;

/*
 * File Information
 *
 * Created on       : 04-mar-2006 9:32:31
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * Abstract View implementation.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class AbstractView implements View, CommandListener, ItemCommandListener {
	private static long lastAccess = System.currentTimeMillis();

	protected static boolean delay;
	static {
		delay = DeviceInfo.isNokia();
		// final ApplicationManager manager = Application.getManager();
		// if( manager != null )
		// {
		// final String property = System.getProperty( "microedition.platform"
		// );
		// if( property != null )
		// {
		// delay = property.indexOf( "6670" ) > -1;
		// }
		// }
	}

	protected static View current;

	/** View id. */
	protected int id;
	/** Controller assigned to this view. */
	protected Controller controller;
	/** View that called this one. */
	protected View caller;
	/** Displayable element to show. */
	protected Displayable displayable;
	/**
	 * If the displayable is a form, the focusedItem is selected on a call to
	 * show.
	 */
	protected Item focusedItem;
	/** Attached object map. */
	protected Hashtable attachments = new Hashtable(10);
	/** Actions */
	protected Hashtable actions = new Hashtable(10);
	protected Hashtable activeActions = new Hashtable(10);

	/**
	 * Get last time a view was displayed.
	 * 
	 * @return last time a view was displayed
	 */
	public static long getLastAccess() {
		return lastAccess;
	}

	/**
	 * Set the last time a view was displayed.
	 * 
	 * @param lastAccess
	 *            last access time
	 */
	public static synchronized void setLastAccess(final long lastAccess) {
		AbstractView.lastAccess = lastAccess;
	}

	/** Update the last access time to the current time. */
	public static synchronized void touchLastAccess() {
		AbstractView.lastAccess = System.currentTimeMillis();
	}

	/**
	 * Get the time the application has been idle in milliseconds. Take note
	 * that the idle time is really the time since the last view was displayed
	 * in many cases, so the application might have not really been idle for
	 * such a time.
	 * 
	 * @return idle time in milliseconds
	 */
	public static long idle() {
		return System.currentTimeMillis() - lastAccess;
	}

	/**
	 * Get currently displpayed view.
	 * 
	 * @return view or null if no current view yet
	 */
	public static View getCurrent() {
		return current;
	}

	/**
	 * Set the current view.
	 * 
	 * @param view
	 *            view
	 */
	public static synchronized void setCurrent(final View view) {
		current = view;
	}

	/**
	 * Create a new view.
	 * 
	 * @param controller
	 *            view controller
	 */
	public AbstractView(final Controller controller) {
		this.controller = controller;
	}

	/**
	 * Get view caller.
	 * 
	 * @return view caller
	 */
	public View getCaller() {
		return caller;
	}

	/**
	 * Set view caller.
	 * 
	 * @param caller
	 *            caller view
	 */
	public synchronized void setCaller(final View caller) {
		this.caller = caller;
	}

	/**
	 * Set view controller.
	 * 
	 * @param controller
	 *            controller
	 */
	public synchronized void setController(final Controller controller) {
		this.controller = controller;
	}

	/**
	 * Get view controller.
	 * 
	 * @return controller
	 */
	public Controller getController() {
		return controller;
	}

	/**
	 * Each view must have an id that identifies it.
	 * 
	 * @return view id
	 */
	public int getId() {
		return id;
	}

	/**
	 * Get the displayable object of this view. This method is intended for
	 * those clases that use an AbstractView directly as a Delgate to achieve
	 * inheritance.
	 * 
	 * @return displayable
	 */
	public Displayable getDisplayable() {
		return displayable;
	}

	/**
	 * Set the displayable object of this view. This method is intended for
	 * those clases that use an AbstractView directly as a Delgate to achieve
	 * inheritance.
	 * 
	 * @param displayable
	 *            displayable
	 */
	public void setDisplayable(final Displayable displayable) {
		this.displayable = displayable;
	}

	/**
	 * Default implementation does nothing.
	 * 
	 * @param command
	 *            command
	 */
	public synchronized void removeCommand(Command command) {
		if (displayable != null && command != null) {
			displayable.removeCommand(command);
		}
	}

	/**
	 * Add a command to the view.
	 * 
	 * @param command
	 *            command to add
	 */
	public synchronized void addCommand(Command command) {
		if (displayable != null && command != null) {
			displayable.addCommand(command);
		}
	}

	/**
	 * Fill in view data from a repository. Useful when you need to persist
	 * application/view state. This implementation does nothing.
	 */
	public void feed() {
	}

	/**
	 * If the view supports restore. feed should not be called if the view can
	 * not be restored. This implementation returns false.
	 * 
	 * @return false
	 */
	public boolean canBeRestored() {
		return false;
	}

	/**
	 * Default implementation of commandAction, calls
	 * AbstractController.commandAction methods choosing the right one depending
	 * on the displayable type.<br/>
	 * If a subclass overrides this method it should take care of calling
	 * handleAction or view actions will not get processed.
	 * 
	 * @param command
	 *            command to execute
	 * @param displayable
	 *            displayable item
	 */
	public void commandAction(final Command command,
			final Displayable displayable) {
		if (!handleAction(command)) {
			if (displayable instanceof Choice) {
				AbstractController.commandAction(this, command,
						(Choice) displayable);
			} else {
				AbstractController.commandAction(this, command);
			}
		}
	}

	/**
	 * Default implementation of commandAction, simply calls
	 * AbstractController.commandAction( view, command ).<br/>
	 * If a subclass overrides this method it should take care of calling
	 * handleAction or view actions will not get processed.
	 * 
	 * @param command
	 *            command
	 * @param item
	 *            item
	 */
	public void commandAction(final Command command, final Item item) {
		if (!handleAction(command)) {
			AbstractController.commandAction(this, command);
		}
	}

	/**
	 * Handle view actions.
	 * 
	 * @param command
	 *            command
	 * @return true if an action has been executed
	 */
	public boolean handleAction(final Command command) {
		if (activeActions.containsKey(command)) {
			final Action action = (Action) activeActions.get(command);
			final ControllerEvent event = new ControllerEvent(action.getCode(),
					this);
			controller.handle(event);
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Utility method that goes to a view setting it's caller to this view.<br/>
	 * Improves code clarity.
	 * 
	 * @param view
	 *            view
	 */
	public void go(final View view) {
		view.setCaller(this);
		view.show();
	}

	/**
	 * Utility method that adds the command to the item, sets it as the default
	 * command optionally, and sets this view as the item's command listener.
	 * 
	 * @param item
	 *            item
	 * @param command
	 *            command
	 * @param defaultCommand
	 *            if command should be the item default command
	 */
	protected void configure(final Item item, final Command command,
			final boolean defaultCommand) {
		item.addCommand(command);
		if (defaultCommand) {
			item.setDefaultCommand(command);
		}
		item.setItemCommandListener(this);
	}

	/**
	 * Create a StringItem with button appearance and associate a command to it.
	 * 
	 * @param resource
	 *            resource identifier for label, if resource not found, the text
	 *            is used as label
	 * @param command
	 *            command
	 * @return a StringItem button
	 */
	protected StringItem createButton(final String resource,
			final Command command) {
		String label = ResourceManager.getResource(resource);
		if (label == null) {
			label = resource;
		}
		final StringItem item = new StringItem(null, label, Item.BUTTON);
		configure(item, command, true);
		return item;
	}

	/**
	 * Default implementation, calls controller's onRelease and clears attached
	 * objects.
	 */
	public void release() {
		controller.onRelease(this);
		detachAll();
	}

	/** Make the displayable object the current at the display. */
	public void show() {
		touchLastAccess();
		try {
			doShow();
		} catch (Throwable e) {
			try {
				doShow();
			} catch (Throwable e1) {
				try {
					doShow();
				} catch (Throwable e2) {
					try {
						doShow();
					} catch (Throwable e3) {
						doShow();
					}
				}
			}
		}
	}

	private void doShow() {
		// This delays minimze the chance that a Nokia 6670 or Nokia S60 3rd
		// edition device (might apply to other nokias) freez when showing a
		// view. The total delay of 600ms, with this distribution seems to
		// make it stable on E70 and E61. The delay is only applied to Nokia
		// phones at this moment
		if (displayable != null) {
			final Display display = Application.getManager().getDisplay();
			delay();
			delay();
			display.setCurrent(displayable);
			if (!DeviceInfo.isNokia() && focusedItem != null) {
				delay();
				delay();
				display.setCurrentItem(focusedItem);
			}
			setCurrent(this);
			delay();
			delay();
		}
	}

	private void delay() {
		if (delay) {
			// Under some strange circumstances the Nokia 6670 crashes whitout
			// this delay
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
			}
		}
	}

	/**
	 * Set the view title.
	 * 
	 * @param title
	 *            title
	 */
	public synchronized void setTitle(final String title) {
		try {
			if (title != null) {
				displayable.setTitle(ResourceManager.getResource(title));
			}
		} catch (Exception e) {
		}
	}

	/**
	 * Get the view title.
	 * 
	 * @return title or null if no displayable set
	 */
	public String getTitle() {
		return displayable != null ? displayable.getTitle() : null;
	}

	/**
	 * Check if the view is showing.
	 * 
	 * @return true if it is
	 */
	public boolean isShown() {
		return Application.getManager().getDisplay().getCurrent() == displayable;
	}

	/**
	 * Add an action to the view.<br/>
	 * An action is a command associated to the view and to a concrete action
	 * code, so the view can directly create and dispatch a controller event.<br/>
	 * Actions can be enabled or disabled, which actually means that the command
	 * is added or removed from the underlying displayable object.
	 * 
	 * @param action
	 *            the action
	 * @param active
	 *            if initially active
	 */
	public void addAction(final Action action, final boolean active) {
		if (!actions.containsKey(action.getName())) {
			actions.put(action.getName(), action);
			if (active) {
				activateAction(action);
			}
		}
	}

	private void activateAction(final Action action) {
		final Command command = action.getCommand();
		activeActions.put(command, action);
		final Item item = action.getItem();
		if (item == null) {
			addCommand(command);
		} else {
			item.addCommand(command);
			if (action.isDefaultAction()) {
				item.setDefaultCommand(command);
			}
		}
	}

	/**
	 * Remove an action from the view.
	 * 
	 * @param action
	 *            action
	 */
	public void removeAction(Action action) {
		if (actions.containsKey(action.getName())) {
			if (activeActions.containsKey(action.getCommand())) {
				deactivateAction(action);
			}
			actions.remove(action.getName());
		}
	}

	private void deactivateAction(final Action action) {
		activeActions.remove(action.getCommand());
		if (action.getItem() == null) {
			removeCommand(action.getCommand());
		} else {
			action.getItem().removeCommand(action.getCommand());
		}
	}

	/**
	 * Get an action.
	 * 
	 * @param name
	 *            action name
	 * @return action or null if not found
	 */
	public Action getAction(String name) {
		return (Action) actions.get(name);
	}

	/**
	 * Check if the action is currently active.
	 * 
	 * @param action
	 *            action
	 * @return true if so
	 */
	public boolean isActive(Action action) {
		return activeActions.containsKey(action.getCommand());
	}

	/**
	 * Activate an action.
	 * 
	 * @param action
	 *            the action
	 */
	public void activate(Action action) {
		if (actions.containsKey(action.getName())
				&& !activeActions.containsKey(action.getCommand())) {
			activateAction(action);
		}
	}

	/**
	 * Activate an action.
	 * 
	 * @param name
	 *            action name
	 */
	public void activate(String name) {
		final Action action = (Action) actions.get(name);
		if (action != null && !activeActions.containsKey(action.getCommand())) {
			activateAction(action);
		}
	}

	/**
	 * Deactivate an action.
	 * 
	 * @param action
	 *            action
	 */
	public void deactivate(Action action) {
		if (actions.containsKey(action.getName())
				&& activeActions.containsKey(action.getCommand())) {
			deactivateAction(action);
		}
	}

	/**
	 * Deactivate an action.
	 * 
	 * @param name
	 *            action name
	 */
	public void deactivate(String name) {
		final Action action = (Action) actions.get(name);
		if (action != null && activeActions.containsKey(action.getCommand())) {
			deactivateAction(action);
		}
	}

	/**
	 * Attach an obeject to the view. Useful for state management. If the value
	 * is null, this is equivalent to detach( key ).
	 * 
	 * @param key
	 *            object key
	 * @param value
	 *            object
	 */
	public void attach(final Object key, final Object value) {
		if (value != null) {
			attachments.put(key, value);
		} else {
			attachments.remove(key);
		}
	}

	/**
	 * Detach an object.
	 * 
	 * @param key
	 *            object key
	 */
	public void detach(Object key) {
		attachments.remove(key);
	}

	/**
	 * Detach all attached objects.
	 */
	public void detachAll() {
		attachments.clear();
	}

	/**
	 * Get an attached object.
	 * 
	 * @param key
	 *            object key
	 * @return object or null if not attached
	 */
	public Object getAttachment(Object key) {
		return attachments.get(key);
	}

	/**
	 * Find an attached object, either to this view or to one of the previous
	 * views in the call chain.
	 * 
	 * @param key
	 *            object key
	 * @return object or null if not attached
	 */
	public Object findAttachment(Object key) {
		return attachments.containsKey(key) ? attachments.get(key)
				: caller != null ? caller.findAttachment(key) : null;
	}

	/**
	 * Get an attachment as a boolean value.
	 * 
	 * @param key
	 *            key
	 * @return boolean value, or false if it's null or not a Boolean
	 */
	public boolean getBooleanAttachment(Object key) {
		final Object value = attachments.get(key);
		return value != null && value instanceof Boolean
				&& ((Boolean) value).booleanValue();
	}

	/**
	 * Get an attachment as a String value.
	 * 
	 * @param key
	 *            key
	 * @return String value, or null if not found
	 */
	public String getStringAttachment(Object key) {
		final Object value = attachments.get(key);
		return value != null ? value.toString() : null;
	}

	/**
	 * Get an attachment as a FixedPoint value.
	 * 
	 * @param key
	 *            key
	 * @return FixedPoint value, or null if not found
	 */
	public FixedPoint getFixedPointAttachment(Object key) {
		final Object value = attachments.get(key);
		return value != null && value instanceof FixedPoint ? (FixedPoint) value
				: null;
	}

	/**
	 * Get an attachment as an int value.
	 * 
	 * @param key
	 *            key
	 * @return int value, or -1 if it's null or not an Integer, Long, Short or
	 *         Byte
	 */
	public int getIntAttachment(Object key) {
		return (int) getNumber(key);
	}

	private long getNumber(final Object key) {
		final Object value = attachments.get(key);
		if (value != null) {
			if (value instanceof Integer) {
				return ((Integer) value).intValue();
			} else if (value instanceof Long) {
				return ((Long) value).longValue();
			} else if (value instanceof Short) {
				return ((Short) value).shortValue();
			} else if (value instanceof Byte) {
				return ((Byte) value).byteValue();
			} else {
				return -1;
			}
		} else {
			return -1;
		}
	}

	/**
	 * Get an attachment as a long value.
	 * 
	 * @param key
	 *            key
	 * @return long value, or -1 if it's null or not a Long, Integer, Short or
	 *         Byte
	 */
	public long getLongAttachment(Object key) {
		return getNumber(key);
	}
}
