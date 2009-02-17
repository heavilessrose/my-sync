package luke.java.practice.treadPool;

/**
 * ���Լ򵥵��̳߳�
 * 
 * @author WangYinghua
 * 
 */
public class SimpleTaskTest extends Task {
	static int taskCount = 0;

	@Override
	public void deal() {
		System.out.println("task " + (++taskCount));
	}

	public static void main(String[] args) throws InterruptedException {
		ThreadPoolService service = new ThreadPoolService();
		service.start();
		// ִ��ʮ������
		for (int i = 0; i < 10; i++) {
			service.runTask(new SimpleTaskTest());
		}
		// ˯��1���ӣ��ȴ���������ִ�����
		Thread.sleep(1000);
		service.stop();
	}
}