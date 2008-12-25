package luke.android.utils;

public class Sort {
	// ///////////////////////////交换排序///////////////////////
	/**
	 * 冒泡排序数组 O(n2)
	 * 
	 * @param n
	 *            要排序的数组
	 * @param direct
	 *            大于0 表示从小到大, 小于0 表示从大到小
	 */
	public static final void bullonSort(int[] n, int direct) {
		int temp;
		boolean exchange = false;
		for (int i = 0; i < n.length - 1; i++) { // 最多做n-1趟排序
			exchange = false;
			for (int j = n.length - 1; j > i; j--) {// 从下到上比较
				if (direct < 0) {
					if (n[j - 1] > n[j]) {
						temp = n[j];
						n[j] = n[j - 1];
						n[j - 1] = temp;
						exchange = true;
					}
				} else {
					if (n[j - 1] < n[j]) {
						temp = n[j];
						n[j] = n[j - 1];
						n[j - 1] = temp;
						exchange = true;
					}
				}
			}
			// 如果本次没有进行交换则表示已经排序完成
			if (!exchange)
				return;
		}
	}

	public static final void quickSort(int[] n, int low, int high) {
		// 对R[low..high]快速排序
		int pivotpos; // 划分后的基准记录的位置
		if (low < high) {// 仅当区间长度大于1时才须排序
			pivotpos = Partition(n, low, high); // 对R[low..high]做划分
			quickSort(n, low, pivotpos - 1); // 对左区间递归排序
			quickSort(n, pivotpos + 1, high); // 对右区间递归排序
		}
	}

	// /////////////////////////////////选择排序//////////////////////
	/**
	 * 直接选择排序数组, 平均时间复杂度为O(n2)
	 * 
	 * @param n
	 * @param direct
	 */
	public static final void straightSelectionSort(int[] n, int direct) {
		int temp;
		int k;// 指示比较过程中最小值的位置
		for (int i = 0; i < n.length - 2; i++) {// 进行n-1趟排序, 每次把最小的放到已排序的下标后面
			k = i;
			for (int j = i + 1; j < n.length; j++) {
				if (n[k] > n[j]) {
					k = j;
				}
			}
			temp = n[i];
			n[i] = n[k];
			n[k] = temp;
			// test
			for (int m = 0; m < n.length; m++) {
				System.out.print(n[m] + ", ");
			}
			System.out.println();
		}
	}

	public static void main(String[] args) {
		int[] n = { 23, 35, 12, 20, -34, 64, 71, 41 };
		int low = n[0];
		int high = n[n.length - 1];

		for (int i = 0; i < n.length; i++) {
			System.out.print(n[i] + ", ");
		}
	}
}
