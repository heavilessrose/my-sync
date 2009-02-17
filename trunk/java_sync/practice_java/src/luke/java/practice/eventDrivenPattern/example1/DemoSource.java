package luke.java.practice.eventDrivenPattern.example1;

import java.util.ArrayList;
import java.util.List;

/**
 * DemoEvent�¼����¼�Դ
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
	 * ���value�ı�, ����DemoEvent�¼�
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
	 * ע�������
	 * <p>
	 * һ����������ע�ᣬ�������ü��������¼�Դ�������¼�����Ȥ�����¼�����ʱ���ü��������¼������������¼�Դ���á�
	 * </p>
	 * 
	 * @param listener
	 * @return
	 */
	public synchronized boolean addListener(IDemoListener listener) {
		return listenerList.add(listener);
	}

	/**
	 * ע��������
	 * 
	 * @param listener
	 * @return
	 */
	public synchronized boolean removeListener(IDemoListener listener) {
		return listenerList.remove(listener);
	}

	/**
	 * �㲥�¼�
	 * <p>
	 * ���е�ע����������ܹ����¼����������¼��������ж������Ӧ��
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
