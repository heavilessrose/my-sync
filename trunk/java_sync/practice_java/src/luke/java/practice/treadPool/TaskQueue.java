package luke.java.practice.treadPool;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

/**
 * ��ͬһʱ�̣������кܶ�������Ҫִ�У���������ͬһʱ��ֻ��ִ��һ�����������񣬵���Ҫִ�е������������˳������ܳ��ܵ�������ʱ��ô���أ����������ִ����Щ����
 * ����ִ����Щ����Ĺ���TaskQueue��Ͷ�������Щ�����е�һ�֣������õ���FIFO���Ƚ��ȳ���Ӣ������First In First
 * Out���ķ�ʽ��Ҳ���ǰ������񵽴���Ⱥ�˳��ִ�С�
 * 
 * @author WangYinghua
 * 
 */
public class TaskQueue {
	private List<Task> queue = new LinkedList<Task>();

	// ���һ������
	public synchronized void addTask(Task task) {
		if (task != null) {
			queue.add(task);
		}
	}

	// ���������������������ɾ��
	public synchronized void finishTask(Task task) {
		if (task != null) {
			task.setState(Task.State.FINISHED);
			queue.remove(task);
		}
	}

	// ȡ��һ���ִ������
	public synchronized Task getTask() {
		Iterator<Task> it = queue.iterator();
		Task task;
		while (it.hasNext()) {
			task = it.next();
			// Ѱ��һ���½�������
			if (Task.State.NEW.equals(task.getState())) {
				// ������״̬��Ϊ������
				task.setState(Task.State.RUNNING);
				return task;
			}
		}
		return null;
	}
}
