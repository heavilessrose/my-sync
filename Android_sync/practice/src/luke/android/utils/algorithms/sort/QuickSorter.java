package luke.android.utils.algorithms.sort;

/**
 * �������� ����������Ŀǰʹ�ÿ�����㷺�������㷨�ˡ� һ������²��裺 1��ѡ��һ����ŦԪ�أ��кܶ�ѡ�����ҵ�ʵ�������ȥ�м�Ԫ�صļ򵥷�����
 * 2��ʹ�ø���ŦԪ�طָ����飬ʹ�ñȸ�Ԫ��С��Ԫ����������ߣ�����������ұߡ�������ŦԪ�ط��ں��ʵ�λ�á�
 * 3��������ŦԪ�����ȷ����λ�ã�������ֳ������֣���ߵģ��ұߵģ���ŦԪ���Լ�������ߵģ��ұߵķֱ�ݹ���ÿ��������㷨���ɡ�
 * ��������ĺ������ڷָ��㷨��Ҳ����˵�����м��ɵĲ��֡�
 * 
 * @author WangYinghua
 * 
 * @param <E>
 */
public class QuickSorter<E extends Comparable<E>> extends Sorter<E> {

	public static void main(String[] args) {
		Integer[] datas = { 8, 5, 2, 1, 3, 9, 4, 6, 7 };
		QuickSorter dd = new QuickSorter();
		dd.sort(datas, 0, datas.length);
		for (int i = 0; i < datas.length; i++) {
			System.out.print(datas[i] + ", ");
		}
	}

	@Override
	public void sort(E[] array, int left, int len) {
		q_sort(array, left, left + len - 1);
	}

	/**
	 * �ݹ�Ľ��������еĿ�������
	 * 
	 * @param array
	 * @param left
	 * @param right
	 */
	private final void q_sort(E[] array, int left, int right) {
		// �����������1(�������������)
		if (right - left < 1)
			return;
		// �õ��ؼ���(����������)
		int pivot_loc = selectPivot(array, left, right);
		// һ������(��Ԫ�ذ����ڼ�С�ڹؼ����˳����õ��ؼ�������)
		pivot_loc = partion(array, left, right, pivot_loc);
		// �Ըùؼ������������н��еݹ��������
		q_sort(array, left, pivot_loc - 1);
		q_sort(array, pivot_loc + 1, right);

	}

	/**
	 * ����(һ������)
	 * 
	 * @param array
	 * @param left
	 * @param right
	 * @param pivot_loc
	 *            ���ݴ�pivotλ��������һ������
	 * @return ����һ��������ɺ�,ȷ����pivot��λ��
	 */
	private int partion(E[] array, int left, int right, int pivot_loc) {
		E tmp = array[pivot_loc];
		array[pivot_loc] = array[right];// now right's position is available

		while (left != right) { // left, right �±겻�غ�ʱ
			while (left < right && array[left].compareTo(tmp) <= 0)
				left++;
			if (left < right) {
				array[right] = array[left];// now left's position is available
				right--;
			}
			while (left < right && array[right].compareTo(tmp) >= 0)
				right--;
			if (left < right) {
				array[left] = array[right];// now right's position is available
				left++;
			}
		}
		// left, right�غ�ʱ,
		// �����Ͻ���һ�����������(����˹ؼ������ΪС�ڹؼ����,�ұ�Ϊ���ڹؼ����),�ѹؼ����ֵ�ŵ���־�غϵ�λ��
		array[left] = tmp;
		// 
		return left;
	}

	// �˴���һ���򵥵�ȥ�м�Ԫ���±�ķ����õ��ؼ���(����ǿ�������ȽϹؼ��ĵط�, ��ʲô�������ôѡ��ؼ���?)
	private int selectPivot(E[] array, int left, int right) {

		return (left + right) / 2;
	}
}