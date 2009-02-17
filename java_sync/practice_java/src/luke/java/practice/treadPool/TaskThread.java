package luke.java.practice.treadPool;

/**
 * ����ִ����������еĴ�ִ������
 * 
 * @author WangYinghua
 * 
 */
public class TaskThread extends Thread {
	// ���߳��������̳߳�
	private ThreadPoolService service;

	public TaskThread(ThreadPoolService tps) {
		service = tps;
	}

	public void run() {
		// ���̳߳����е�״̬��ִ����������е�����
		while (service.isRunning()) {
			TaskQueue queue = service.getTaskQueue();
			Task task = queue.getTask();
			if (task != null) {
				task.deal();
			}
			queue.finishTask(task);
		}
	}
}