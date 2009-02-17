package luke.java.practice.treadPool;

import java.util.ArrayList;
import java.util.List;

/**
 * 核心类。它在被创建了时候就创建了几个线程对象，但是这些线程并没有启动运行，但调用了start()方法启动线程池服务时，它们才真正运行。
 * 
 * @author WangYinghua
 * 
 */
public class ThreadPoolService {
	// 线程数
	public static final int THREAD_COUNT = 5;

	// 线程池状态
	private Status status = Status.NEW;

	private TaskQueue queue = new TaskQueue();

	public enum Status {
		/* 新建 */NEW, /* 提供服务中 */RUNNING, /* 停止服务 */TERMINATED,
	}

	private List<Thread> threads = new ArrayList<Thread>();

	public ThreadPoolService() {
		for (int i = 0; i < THREAD_COUNT; i++) {
			Thread t = new TaskThread(this);
			threads.add(t);
		}
	}

	// 启动服务
	public void start() {
		this.status = Status.RUNNING;
		for (int i = 0; i < THREAD_COUNT; i++) {
			threads.get(i).start();
		}
	}

	// 停止服务
	public void stop() {
		this.status = Status.TERMINATED;
	}

	// 是否正在运行
	public boolean isRunning() {
		return status == Status.RUNNING;
	}

	// 执行任务
	public void runTask(Task task) {
		queue.addTask(task);
	}

	protected TaskQueue getTaskQueue() {
		return queue;
	}
}
