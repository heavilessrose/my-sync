package winkCC.core.event;

import java.util.Hashtable;
import java.util.Vector;

/**
 * 
 * <p>
 * �¼���װ��: �����¼���, �����¼�ע��,ע��,�ַ��Ⱦ�̬����
 * </p>
 * 
 * ����������core��Ҫ�õ����¼�����, �ɼ̳и������������¼���
 * 
 * @author WangYinghua
 * @version 0.1
 */
public class Event {
	public static final int ET_GLOBAL = 0;
	// progress�¼�, ����Ǵ�����, ��ô����Ӧ����ProgressEvent��ʵ��
	public static final int ET_PROGRESS = 1;
	// ResourceManager�����Ըı��¼�
	public static final int ET_LANGUAGE_CHANGE = 2;

	public static final Integer GLOBAL = new Integer(ET_GLOBAL);
	public static final Integer PROGRESS = new Integer(ET_PROGRESS);
	public static final Integer LANGUAGE_CHANGE = new Integer(
			ET_LANGUAGE_CHANGE);

	private static Vector backgroundTasks = new Vector(10);
	private static Hashtable listeners = new Hashtable(10);
	// Ԥ�����3���¼�
	static {
		listeners.put(GLOBAL, new Vector(1));
		listeners.put(PROGRESS, new Vector(1));
		listeners.put(LANGUAGE_CHANGE, new Vector(1));
	}

	protected Integer type;
	protected boolean consumed;
	protected Hashtable attachments;

	/**
	 * ���һ����̨����. ��ע��Ϊ background task���̲߳�����PROGRESS�¼�
	 * 
	 * @param thread
	 *            thread
	 */
	public static void addBackgroundTask(final Thread thread) {
		backgroundTasks.addElement(thread);
	}

	/**
	 * ɾ��һ��background task
	 * 
	 * @param thread
	 *            thread
	 */
	public static void removeBackgroundTask(final Thread thread) {
		backgroundTasks.removeElement(thread);
	}

	/**
	 * ע��һ��ȫ���¼��ļ�����
	 * 
	 * @param listener
	 *            listener
	 */
	public static void register(final IEventListener listener) {
		register(listener, GLOBAL);
	}

	/**
	 * ע��һ��ָ���¼����͵ļ�����
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
	 * ע��һ��ȫ���¼��ļ�����
	 * 
	 * @param listener
	 *            listener
	 */
	public static synchronized void unregister(final IEventListener listener) {
		unregister(listener, GLOBAL);
	}

	/**
	 * ע��һ��ָ���¼����͵ļ�����
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
	 * ע������ָ���¼����͵ļ�����
	 * 
	 * @param type
	 *            event type
	 */
	public static void unregisterAll(final Integer type) {
		if (listeners.containsKey(type)) {
			listeners.remove(type);
		}
	}

	/** ע�������¼����͵ļ����� */
	public static void unregisterAll() {
		listeners.clear();
	}

	/**
	 * �ַ��¼�����ע�������.
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
	 * �õ�ָ���¼����͵����м�����
	 * 
	 * @param type
	 *            type
	 * @return listeners, null if none
	 */
	public static Vector getListeners(final Integer type) {
		return (Vector) listeners.get(type);
	}

	/**
	 * Ϊָ���¼������趨һ�������
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
	 * ����¼��Ƿ񱻴�����
	 * 
	 * @return consumed
	 */
	public boolean isConsumed() {
		return consumed;
	}

	/**
	 * ����¼��Ƿ񱻴���<br/>
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
	 * Ϊ�¼����Ӷ���
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
	 * ȥ���¼���һ�����Ӷ���
	 * 
	 * @param key
	 *            key
	 */
	public void detach(final String key) {
		if (attachments != null) {
			attachments.remove(key);
		}
	}

	/** Ϊ�¼�ȥ�����и��Ӷ��� */
	public void detachAll() {
		if (attachments != null) {
			attachments.clear();
		}
	}

	public Object getAttachment(final String key) {
		return attachments != null ? attachments.get(key) : null;
	}

	/**
	 * ���һ�������������Ƿ����ڼ����������¼�����
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
