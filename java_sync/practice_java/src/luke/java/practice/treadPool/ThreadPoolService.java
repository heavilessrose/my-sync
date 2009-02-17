package luke.java.practice.treadPool;

import java.util.ArrayList;
import java.util.List;

/**
 * �����ࡣ���ڱ�������ʱ��ʹ����˼����̶߳��󣬵�����Щ�̲߳�û���������У���������start()���������̳߳ط���ʱ�����ǲ��������С�
 * 
 * @author WangYinghua
 * 
 */
public class ThreadPoolService {
	// �߳���
	public static final int THREAD_COUNT = 5;

	// �̳߳�״̬
	private Status status = Status.NEW;

	private TaskQueue queue = new TaskQueue();

	public enum Status {
		/* �½� */NEW, /* �ṩ������ */RUNNING, /* ֹͣ���� */TERMINATED,
	}

	private List<Thread> threads = new ArrayList<Thread>();

	public ThreadPoolService() {
		for (int i = 0; i < THREAD_COUNT; i++) {
			Thread t = new TaskThread(this);
			threads.add(t);
		}
	}

	// ��������
	public void start() {
		this.status = Status.RUNNING;
		for (int i = 0; i < THREAD_COUNT; i++) {
			threads.get(i).start();
		}
	}

	// ֹͣ����
	public void stop() {
		this.status = Status.TERMINATED;
	}

	// �Ƿ���������
	public boolean isRunning() {
		return status == Status.RUNNING;
	}

	// ִ������
	public void runTask(Task task) {
		queue.addTask(task);
	}

	protected TaskQueue getTaskQueue() {
		return queue;
	}
}
