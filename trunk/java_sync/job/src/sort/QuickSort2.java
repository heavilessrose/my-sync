package sort;
public class QuickSort2 {

	/**
	 * 不是很标准的实现
	 * @param args
	 */
	public static void main(String[] args) {
		int[] datas = {  3, 2,  3, 1, 4, 5, 1, 9 , 8 , 6 , 7  };
		QuickSort2 quickSort = new QuickSort2();
		datas = quickSort.sort(datas);
		for (int i = 0; i < datas.length; i++) {
			System.out.print(datas[i] + " ");
		}
	}

	public int[] sort(int[] data) {
		quickSort(data, 0, data.length - 1);
		return data;
	}

	void quickSort(int[] pData, int left, int right) {
		int i, j;
		int iTemp;
		int middle;
		i = left;
		j = right;
		middle = pData[(left + right) / 2];
		do {
//			System.out.println(i + ", " + j);
			while ((pData[i] < middle) && (i < right))
				i++;
			while ((pData[j] > middle) && (j > left))
				j--;
			if (i <= j) {
				iTemp = pData[i];
				pData[i] = pData[j];
				pData[j] = iTemp;
				i++;
				j--;
			}

		} while (i <= j);// 如果两边扫描的下标交错，就停止（完成一次）

		if (left < j)
			quickSort(pData, left, j);
		if (right > i)
			quickSort(pData, i, right);
	}

}