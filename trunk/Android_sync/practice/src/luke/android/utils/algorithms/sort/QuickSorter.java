package luke.android.utils.algorithms.sort;

/**
 * 快速排序 快速排序是目前使用可能最广泛的排序算法了。 一般分如下步骤： 1）选择一个枢纽元素（有很对选法，我的实现里采用去中间元素的简单方法）
 * 2）使用该枢纽元素分割数组，使得比该元素小的元素在它的左边，比它大的在右边。并把枢纽元素放在合适的位置。
 * 3）根据枢纽元素最后确定的位置，把数组分成三部分，左边的，右边的，枢纽元素自己，对左边的，右边的分别递归调用快速排序算法即可。
 * 快速排序的核心在于分割算法，也可以说是最有技巧的部分。
 * 
 * @author WangYinghua
 * 
 * @param <E>
 */
public class QuickSorter<E extends Comparable<E>> extends Sorter<E> {

	/*
	 * (non-Javadoc)
	 * 
	 * @see algorithms.Sorter#sort(E[], int, int)
	 */
	@Override
	public void sort(E[] array, int from, int len) {
		q_sort(array, from, from + len - 1);
	}

	private final void q_sort(E[] array, int from, int to) {
		if (to - from < 1)
			return;
		int pivot = selectPivot(array, from, to);

		pivot = partion(array, from, to, pivot);

		q_sort(array, from, pivot - 1);
		q_sort(array, pivot + 1, to);

	}

	private int partion(E[] array, int from, int to, int pivot) {
		E tmp = array[pivot];
		array[pivot] = array[to];// now to's position is available

		while (from != to) {
			while (from < to && array[from].compareTo(tmp) <= 0)
				from++;
			if (from < to) {
				array[to] = array[from];// now from's position is available
				to--;
			}
			while (from < to && array[to].compareTo(tmp) >= 0)
				to--;
			if (from < to) {
				array[from] = array[to];// now to's position is available now
				from++;
			}
		}
		array[from] = tmp;
		return from;
	}

	private int selectPivot(E[] array, int from, int to) {

		return (from + to) / 2;
	}

}