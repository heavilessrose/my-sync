package luke.android.utils.algorithms.sort;

/**
 * 插入排序 该算法在数据规模小的时候十分高效，该算法每次插入第K+1到前K个有序数组中一个合适位置， K从0开始到N-1,从而完成排序：
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