package sort;

public class InsertionSort2 {

	private int[] sort(int[] array) {
		for (int j = 1; j < array.length; j++) {
			int swapLoc = j; // init the location where to insert
			for (int i = j - 1; i >= 0; i--) { // get the location where to
				// insert
				if (array[i] > array[j]) {
					swapLoc = i;
				}
			}
			// backward the elems between swapLoc and j
			int temp = array[j];
			for (int k = j - 1; k >= swapLoc; k--) {
				array[k + 1] = array[k];
			}
			// swap the elem (swapLoc,j)
			array[swapLoc] = temp;
		}

		return array;
	}

	public static void main(String[] args) {
		int[] array = { 1, 21, 34, 79, 98, 23, 68, 2, 3, 8, 6, 33, 7, 87, 32,
				24, 776 };
		InsertionSort2 insertionSort = new InsertionSort2();
		int[] result = insertionSort.sort(array);
		for (int i = 0; i < result.length; i++) {
			System.out.print(result[i] + " ");
		}
	}

}