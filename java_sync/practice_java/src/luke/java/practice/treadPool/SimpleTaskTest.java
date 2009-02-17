package luke.java.practice.treadPool;

/**
 * 测试简单的线程池
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
		// 执行十次任务
		for (int i = 0; i < 10; i++) {
			service.runTask(new SimpleTaskTest());
		}
		// 睡眠1秒钟，等待所有任务执行完毕
		Thread.sleep(1000);
		service.stop();
	}
}