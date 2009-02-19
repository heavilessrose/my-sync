package winkCC.core.event;

import java.util.Hashtable;
import java.util.Vector;

/**
 * 
 * <p>
 * 事件封装类: 基本事件类, 包含事件注册,注销,分发等静态方法
 * </p>
 * 
 * 定义了两个core包要用到的事件类型, 可继承该类产生特殊的事件类
 * 
 * @author WangYinghua
 * @version 0.1
 */
public class Event {
	public static final int ET_GLOBAL = 0;
	// progress事件, 如果是此类型, 那么对象应该是ProgressEvent的实例
	public static final int ET_PROGRESS = 1;
	// ResourceManager的语言改变事件
	public static final int ET_LANGUAGE_CHANGE = 2;

	public static final Integer GLOBAL = new Integer(ET_GLOBAL);
	public static final Integer PROGRESS = new Integer(ET_PROGRESS);
	public static final Integer LANGUAGE_CHANGE = new Integer(
			ET_LANGUAGE_CHANGE);

	private static Vector backgroundTasks = new Vector(10);
	private static Hashtable listeners = new Hashtable(10);
	// 预定义的3类事件
	static {
		listeners.put(GLOBAL, new Vector(1));
		listeners.put(PROGRESS, new Vector(1));
		listeners.put(LANGUAGE_CHANGE, new Vector(1));
	}

	protected Integer type;
	protected boolean consumed;
	protected Hashtable attachments;

	/**
	 * 添加一个后台任务. 被注册为 background task的线程不产生PROGRESS事件
	 * 
	 * @param thread
	 *            thread
	 */
	public static void addBackgroundTask(final Thread thread) {
		backgroundTasks.addElement(thread);
	}

	/**
	 * 删除一个background task
	 * 
	 * @param thread
	 *            thread
	 */
	public static void removeBackgroundTask(final Thread thread) {
		backgroundTasks.removeElement(thread);
	}

	/**
	 * 注册一个全局事件的监听器
	 * 
	 * @param listener
	 *            listener
	 */
	public static void register(final IEventListener listener) {
		register(listener, GLOBAL);
	}

	/**
	 * 注册一个指定事件类型的监听器
	 * 
	 * @param listener
	 *            listener
	 * @param type
	 *            event type
	 */
	public static synchronized void register(final IEventListener listener,
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
	 * 注销一个全局事件的监听器
	 * 
	 * @param listener
	 *            listener
	 */
	public static synchronized void unregister(final IEventListener listener) {
		unregister(listener, GLOBAL);
	}

	/**
	 * 注销一个指定事件类型的监听器
	 * 
	 * @param listener
	 *            listener
	 * @param type
	 *            event type
	 */
	public static synchronized void unregister(final IEventListener listener,
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
	 * 注销所有指定事件类型的监听器
	 * 
	 * @param type
	 *            event type
	 */
	public static void unregisterAll(final Integer type) {
		if (listeners.containsKey(type)) {
			listeners.remove(type);
		}
	}

	/** 注销所有事件类型的监听器 */
	public static void unregisterAll() {
		listeners.clear();
	}

	/**
	 * 分发事件到已注册监听器.
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
					final IEventListener listener = (IEventListener) v
							.elementAt(i);
					listener.handleEvent(event);
					Thread.yield();
				}
				event.consumed = false;
			}
		}
	}

	/**
	 * 得到指定事件类型的所有监听器
	 * 
	 * @param type
	 *            type
	 * @return listeners, null if none
	 */
	public static Vector getListeners(final Integer type) {
		return (Vector) listeners.get(type);
	}

	/**
	 * 为指定事件类型设定一组监听器
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

	public Integer getType() {
		return type;
	}

	public synchronized void setType(final int type) {
		this.type = new Integer(type);
	}

	public synchronized void setType(final Integer type) {
		this.type = type;
	}

	/**
	 * 检查事件是否被处理了
	 * 
	 * @return consumed
	 */
	public boolean isConsumed() {
		return consumed;
	}

	/**
	 * 标记事件是否被处理<br/>
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
	 * 为事件附加对象
	 * 
	 * @param key
	 *            key
	 * @param attachment
	 *            object
	 */
	public synchronized void attach(final String key, final Object attachment) {
		if (attachments == null) {
			attachments = new Hashtable(2);
		}
		attachments.put(key, attachment);
	}

	/**
	 * 去掉事件的一个附加对象
	 * 
	 * @param key
	 *            key
	 */
	public void detach(final String key) {
		if (attachments != null) {
			attachments.remove(key);
		}
	}

	/** 为事件去掉所有附加对象 */
	public void detachAll() {
		if (attachments != null) {
			attachments.clear();
		}
	}

	public Object getAttachment(final String key) {
		return attachments != null ? attachments.get(key) : null;
	}

	/**
	 * 检查一个给定监听器是否正在监听给定的事件类型
	 * 
	 * @param type
	 *            event type
	 * @param listener
	 *            the listener
	 * @return true if so
	 */
	public static boolean isRegistered(final Integer type,
			final IEventListener listener) {
		Vector v = (Vector) listeners.get(type);
		return v != null && v.contains(listener);
	}
}
