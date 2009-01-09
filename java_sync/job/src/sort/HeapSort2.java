package sort;

public class HeapSort2 {

	private int heapLen;

	private int[] sort(int[] array) {
		heapLen = array.length;
		buildHeap(array); // init the heap
		for (int i = heapLen - 1; i > 0; i--) { // swap root and last node, up
												// to down
			swap(array, i, 0);
			heapLen--;
			heapify(array, 0); // reheapify the root node from 0 to n-1
		}
		return array;
	}

	private void buildHeap(int[] array) {
		for (int i = heapLen / 2; i >= 0; i--) { // down to up
			heapify(array, i);
		}
	}

	private void heapify(int[] array, int i) { // heapify tree which root is i
		int left = 2 * i + 1;
		int right = 2 * i + 2;
		int largest = 0;
		if (left < heapLen && array[left] > array[i]) {
			largest = left;
		} else
			largest = i;
		if (right < heapLen && array[right] > array[largest]) {
			largest = right;
		}
		if (largest != i) {
			swap(array, i, largest);
			heapify(array, largest);
		}
	}

	private void swap(int[] array, int i, int j) {
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}

	public static void main(String[] args) {
		int[] array = { 1, 21, 34, 79, 98, 23, 68, 2, 3, 8, 33, 7, 87, 32, 24,
				6, 776 };
		HeapSort2 heapSort = new HeapSort2();
		int[] result = heapSort.sort(array);
		for (int i = 0; i < result.length; i++) {
			System.out.print(result[i] + " ");
		}
	}

}