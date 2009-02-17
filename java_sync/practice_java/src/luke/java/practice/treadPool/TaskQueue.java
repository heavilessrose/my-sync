package luke.java.practice.treadPool;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

/**
 * 在同一时刻，可能有很多任务需要执行，而程序在同一时刻只能执行一定数量的任务，当需要执行的任务数超过了程序所能承受的任务数时怎么办呢？这就有了先执行哪些任务
 * ，后执行哪些任务的规则。TaskQueue类就定义了这些规则中的一种，它采用的是FIFO（先进先出，英文名是First In First
 * Out）的方式，也就是按照任务到达的先后顺序执行。
 * 
 * @author WangYinghua
 * 
 */
public class TaskQueue {
	private List<Task> queue = new LinkedList<Task>();

	// 添加一项任务
	public synchronized void addTask(Task task) {
		if (task != null) {
			queue.add(task);
		}
	}

	// 完成任务后将它从任务队列中删除
	public synchronized void finishTask(Task task) {
		if (task != null) {
			task.setState(Task.State.FINISHED);
			queue.remove(task);
		}
	}

	// 取得一项待执行任务
	public synchronized Task getTask() {
		Iterator<Task> it = queue.iterator();
		Task task;
		while (it.hasNext()) {
			task = it.next();
			// 寻找一个新建的任务
			if (Task.State.NEW.equals(task.getState())) {
				// 把任务状态置为运行中
				task.setState(Task.State.RUNNING);
				return task;
			}
		}
		return null;
	}
}
