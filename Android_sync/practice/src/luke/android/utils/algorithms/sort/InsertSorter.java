package luke.android.utils.algorithms.sort;

/**
 * �������� ���㷨�����ݹ�ģС��ʱ��ʮ�ָ�Ч�����㷨ÿ�β����K+1��ǰK������������һ������λ�ã� K��0��ʼ��N-1,�Ӷ��������
 * 
 * @author WangYinghua
 * 
 * @param <E>
 */
public class InsertSorter<E extends Comparable<E>> extends Sorter<E> {

	/*
	 * (non-Javadoc)
	 * 
	 * @see algorithms.Sorter#sort(E[], int, int)
	 */
	public void sort(E[] array, int from, int len) {
		E tmp = null;
		for (int i = from + 1; i < from + len; i++) {
			tmp = array[i];
			int j = i;
			for (; j > from; j--) {
				if (tmp.compareTo(array[j - 1]) < 0) {
					array[j] = array[j - 1];
				} else
					break;
			}
			array[j] = tmp;
		}
	}

}