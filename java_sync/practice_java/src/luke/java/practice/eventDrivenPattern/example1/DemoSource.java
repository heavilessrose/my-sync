package luke.java.practice.eventDrivenPattern.example1;

import java.util.ArrayList;
import java.util.List;

/**
 * DemoEvent事件的事件源
 * 
 * @author WangYinghua
 * 
 */
public class DemoSource {
	public DemoSource() {

	}

	private int value = 0;
	private List<IDemoListener> listenerList = new ArrayList<IDemoListener>();

	public int getValue() {
		return this.value;
	}

	/**
	 * 如果value改变, 触发DemoEvent事件
	 * 
	 * @param value
	 */
	public void setValue(int value) {
		int oldValue = this.getValue();
		this.value = value;
		if (oldValue != value) {
			DemoEvent event = new DemoEvent(this);
			broadcast(event);
		}
	}

	/**
	 * 注册监听器
	 * <p>
	 * 一旦监听器被注册，即表明该监听器对事件源产生的事件感兴趣，在事件产生时，该监听器的事件处理方法将被事件源调用。
	 * </p>
	 * 
	 * @param listener
	 * @return
	 */
	public synchronized boolean addListener(IDemoListener listener) {
		return listenerList.add(listener);
	}

	/**
	 * 注销监听器
	 * 
	 * @param listener
	 * @return
	 */
	public synchronized boolean removeListener(IDemoListener listener) {
		return listenerList.remove(listener);
	}

	/**
	 * 广播事件
	 * <p>
	 * 所有的注册监听器都能够对事件作出其在事件处理方法中定义的响应。
	 * </p>
	 * 
	 * @param event
	 */
	public void broadcast(DemoEvent event) {
		for (IDemoListener listener : listenerList) {
			listener.handleEvent(event);
		}
	}

	public static void main(String[] args) {
		DemoSource demo = new DemoSource();
		demo.addListener(new MyListener());
		demo.setValue(123);
	}
}
