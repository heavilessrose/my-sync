package thread.sync;

import java.util.*;

/**
 * ��ʾ����ͬ����ʱ������
 * 
 * @author WangYinghua
 * 
 */
class synch {
	synchronized int locking(int a, int b) {
		return a + b;
	}

	int not_locking(int a, int b) {
		StringBuffer strBuff = new StringBuffer("1234");
		System.out.println(strBuff.reverse());
		return a + b;
	}

	private static final int ITERATIONS = 5;

	static public void main(String[] args) {
		synch tester = new synch();
		double start = new Date().getTime();
		for (long i = ITERATIONS; --i >= 0;)
			tester.locking(0, 0);
		double end = new Date().getTime();
		double locking_time = end - start;

		start = new Date().getTime();
		for (long i = ITERATIONS; --i >= 0;)
			tester.not_locking(0, 0);
		end = new Date().getTime();
		double not_locking_time = end - start;

		double time_in_synchronization = locking_time - not_locking_time;
		System.out.println("ͬ�����ĵ�ʱ�� (millis.): " + time_in_synchronization);
		System.out.println("ÿ��ͬ�����õ�����: "
				+ (time_in_synchronization / ITERATIONS));
		System.out.println(not_locking_time / locking_time * 100.0
				+ "% �������ӵİٷֱ�");
	}
}
