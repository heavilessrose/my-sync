package sort;

public class MergeSort2 {

	public int[] sort(int[] data) {
		int[] temp = new int[data.length];
		mergeSort(data, temp, 0, data.length - 1);
		return data;
	}

	private void mergeSort(int[] data, int[] temp, int l, int r) {
		int mid = (l + r) / 2;
		System.out.println(l + ", " + mid + ", " + r);
		if (l == r)
			return;

		mergeSort(data, temp, l, mid);
		mergeSort(data, temp, mid + 1, r);
		for (int i = l; i <= r; i++) {
			System.out.println("i=" + i);
			temp[i] = data[i];
		}
		int i1 = l;
		int i2 = mid + 1;
		for (int cur = l; cur <= r; cur++) {
			if (i1 == mid + 1)
				data[cur] = temp[i2++];
			else if (i2 > r)
				data[cur] = temp[i1++];
			else if (temp[i1] < temp[i2])
				data[cur] = temp[i1++];
			else
				data[cur] = temp[i2++];
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		int[] datas = { 1, 21, 34, 79, 98, 23, 68, 2, 3, 8, 6, 33, 6, 7, 87,
				32, 24, 6, 776 };
		MergeSort2 mergeSort = new MergeSort2();
		datas = mergeSort.sort(datas);
		for (int i = 0; i < datas.length; i++) {
			System.out.print(datas[i] + ",");
		}
	}

}