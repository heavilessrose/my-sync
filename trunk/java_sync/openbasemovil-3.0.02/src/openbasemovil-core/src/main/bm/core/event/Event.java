package bm.core.event;

import java.util.Hashtable;
import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 04-mar-2006 9:32:31
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mi茅, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 * 
 * modified by luke 09.2.17
 */

/**
 * Basic event, provides also dispatching mechanism.
 * 
 * <p>
 * 事件封装类: represents the event itself but also has static methods that control
 * the registration of listeners and event dispatching.
 * </p>
 * There are a number of predefined event types for situations controlled by the
 * core library, and some subclasses of Event that extend the properties of it.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class Event {
	public static final int ET_GLOBAL = 0;
	/**
	 * The event is a progress event, and the object will be an instance of
	 * ProgressEvent.
	 */
	public static final int ET_PROGRESS = 1;
	/** The event signals a language change at the ResourceManager. */
	public static final int ET_LANGUAGE_CHANGE = 2;

	public static final Integer GLOBAL = new Integer(ET_GLOBAL);
	public static final Integer PROGRESS = new Integer(ET_PROGRESS);
	public static final Integer LANGUAGE_CHANGE = new Integer(
			ET_LANGUAGE_CHANGE);

	private static Vector backgroundTasks = new Vector(10);
	private static Hashtable listeners = new Hashtable(10);
	static {
		listeners.put(GLOBAL, new Vector(1));
		listeners.put(PROGRESS, new Vector(1));
		listeners.put(LANGUAGE_CHANGE, new Vector(1));
	}

	protected Integer type;
	protected boolean consumed;
	protected Hashtable attachments;

	/**
	 * Add a background task. A thread registered as background task won't
	 * generate progress events.
	 * 
	 * @param thread
	 *            thread
	 */
	public static void addBackgroundTask(final Thread thread) {
		backgroundTasks.addElement(thread);
	}

	/**
	 * Remove a background task.
	 * 
	 * @param thread
	 *            thread
	 */
	public static void removeBackgroundTask(final Thread thread) {
		backgroundTasks.removeElement(thread);
	}

	/**
	 * Register a global event event listener.
	 * 
	 * @param listener
	 *            listener
	 */
	public static void register(final EventListener listener) {
		register(listener, GLOBAL);
	}

	/**
	 * Register a listener for a certain type of event.
	 * 
	 * @param listener
	 *            listener
	 * @param type
	 *            event type
	 */
	public static synchronized void register(final EventListener listener,
			final Integer type) {
		Vector v = (Vector) listeners.get(type);
		if (v == null) {
			v = new Vector(1);
		}
		if (!v.contains(listener)) {
			v.addElement(listener);
		}
		listeners.put(type, v);
	}

	/**
	 * Unregister a global event event listener.
	 * 
	 * @param listener
	 *            listener
	 */
	public static synchronized void unregister(final EventListener listener) {
		unregister(listener, GLOBAL);
	}

	/**
	 * Unregister a listener from a concrete event type.
	 * 
	 * @param listener
	 *            listener
	 * @param type
	 *            event type
	 */
	public static synchronized void unregister(final EventListener listener,
			final Integer type) {
		Vector v = (Vector) listeners.get(type);
		if (v != null) {
			if (v.contains(listener)) {
				v.removeElement(listener);
			}
			listeners.put(type, v);
		}
	}

	/**
	 * Remove all the listeners for a given event type.
	 * 
	 * @param type
	 *            event type
	 */
	public static void unregisterAll(final Integer type) {
		if (listeners.containsKey(type)) {
			listeners.remove(type);
		}
	}

	/**
	 * Remove all listeners for all event types.
	 */
	public static void unregisterAll() {
		listeners.clear();
	}

	/**
	 * Dispatch the event to all the registered event listeners, in the same
	 * order they were registered. If a listener is a ProgressView then it's
	 * made visible if it's not.<br/>
	 * 
	 * @param event
	 *            event to dispatch
	 */
	public static void dispatch(final Event event) {
		if (!event.type.equals(PROGRESS)
				|| !backgroundTasks.contains(Thread.currentThread())) {
			final Vector v = (Vector) listeners.get(event.type);
			if (v != null) {
				final int count = v.size();
				for (int i = 0; i < count && !event.consumed; i++) {
					final EventListener listener = (EventListener) v
							.elementAt(i);
					listener.handleEvent(event);
					Thread.yield();
				}
				event.consumed = false;
			}
		}
	}

	/**
	 * Get registered listeners for a given event type.
	 * 
	 * @param type
	 *            type
	 * @return listeners, null if none
	 */
	public static Vector getListeners(final Integer type) {
		return (Vector) listeners.get(type);
	}

	/**
	 * Set the list of registered listeners for a given event type.
	 * 
	 * @param type
	 *            type
	 * @param list
	 *            listeners, null if none
	 */
	public static void setListeners(final Integer type, final Vector list) {
		listeners.put(type, list);
	}

	public Event() {
	}

	public Event(final int type) {
		this.type = new Integer(type);
	}

	public Event(final Integer type) {
		this.type = type;
	}

	public void dispatch() {
		dispatch(this);
	}

	/**
	 * Get the event type.
	 * 
	 * @return event type
	 */
	public Integer getType() {
		return type;
	}

	/**
	 * Set the event type.
	 * 
	 * @param type
	 *            event type
	 */
	public synchronized void setType(final int type) {
		this.type = new Integer(type);
	}

	/**
	 * Set the event type.
	 * 
	 * @param type
	 *            event type
	 */
	public synchronized void setType(final Integer type) {
		this.type = type;
	}

	/**
	 * Check if the event has been consumed.
	 * 
	 * @return consumed
	 */
	public boolean isConsumed() {
		return consumed;
	}

	/**
	 * Mark the event as consumed or not.<br/>
	 * When an event is consumed, the dispatcher stops passing the event to
	 * further registered listeners.
	 * 
	 * @param consumed
	 *            consumed
	 */
	public synchronized void setConsumed(final boolean consumed) {
		this.consumed = consumed;
	}

	/**
	 * Attach an object to the event.
	 * 
	 * @param key
	 *            key
	 * @param attachment
	 *            object
	 */
	public synchronized void attach(final String key, final Object attachment) {
		if (attachments == null) // Conserve memory unless needed
		{
			attachments = new Hashtable(2);
		}
		attachments.put(key, attachment);
	}

	/**
	 * Detach an object.
	 * 
	 * @param key
	 *            key
	 */
	public void detach(final String key) {
		if (attachments != null) {
			attachments.remove(key);
		}
	}

	/**
	 * Detach all objects.
	 */
	public void detachAll() {
		if (attachments != null) {
			attachments.clear();
		}
	}

	/**
	 * Get an attachment.
	 * 
	 * @param key
	 *            key
	 * @return object or null if not attached
	 */
	public Object getAttachment(final String key) {
		return attachments != null ? attachments.get(key) : null;
	}

	/**
	 * Check if a given listener is currently listening to a given event type.
	 * 
	 * @param type
	 *            event type
	 * @param listener
	 *            the listener
	 * @return true if so
	 */
	public static boolean isRegistered(final Integer type,
			final EventListener listener) {
		Vector v = (Vector) listeners.get(type);
		return v != null && v.contains(listener);
	}
}
