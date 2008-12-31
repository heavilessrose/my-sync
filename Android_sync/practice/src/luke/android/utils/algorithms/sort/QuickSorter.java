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
	 * 递归的进行子序列的快速排序
	 * 
	 * @param array
	 * @param left
	 * @param right
	 */
	private final void q_sort(E[] array, int left, int right) {
		// 子序列需大于1(总排序完成条件)
		if (right - left < 1)
			return;
		// 得到关键点(划分子序列)
		int pivot_loc = selectPivot(array, left, right);
		// 一趟排序(将元素按大于及小于关键点的顺序放置到关键点左右)
		pivot_loc = partion(array, left, right, pivot_loc);
		// 对该关键点左右子序列进行递归快速排序
		q_sort(array, left, pivot_loc - 1);
		q_sort(array, pivot_loc + 1, right);

	}

	/**
	 * 划分(一趟排序)
	 * 
	 * @param array
	 * @param left
	 * @param right
	 * @param pivot_loc
	 *            根据此pivot位置来进行一次排序
	 * @return 序列一次排序完成后,确定的pivot的位置
	 */
	private int partion(E[] array, int left, int right, int pivot_loc) {
		E tmp = array[pivot_loc];
		array[pivot_loc] = array[right];// now right's position is available

		while (left != right) { // left, right 下标不重合时
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
		// left, right重合时,
		// 即符合结束一次排序的条件(完成了关键点左边为小于关键点的,右边为大于关键点的),把关键点的值放到标志重合的位置
		array[left] = tmp;
		// 
		return left;
	}

	// 此处用一个简单的去中间元素下标的方法得到关键点(这个是快速排序比较关键的地方, 在什么情况下怎么选择关键点?)
	private int selectPivot(E[] array, int left, int right) {

		return (left + right) / 2;
	}
}