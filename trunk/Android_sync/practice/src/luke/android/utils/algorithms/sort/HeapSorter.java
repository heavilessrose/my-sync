package luke.android.utils.algorithms.sort;

/**
 * ������ ����һ����ȫ��������һ��ʹ��������ʵ�֡� ����Ҫ�����ֺ��Ĳ����� 1����ָ���ڵ����ϵ���(shiftUp)
 * 2����ָ���ڵ����µ���(shiftDown) ���ѣ��Լ�ɾ���Ѷ��ڵ�ʹ��shiftDwon,���ڲ���ڵ�ʱһ�������ֲ���һ��ʹ�á�
 * ������������ֵ����ʵ�֣���i�δӶѶ��Ƴ����ֵ�ŵ�����ĵ�����i��λ�ã�Ȼ��shiftDown��������i+1��λ��,һ��ִ��N�˵��������������
 * ��Ȼ��������Ҳ��һ��ѡ���Ե�����ÿ��ѡ���i���Ԫ�ء�
 * 
 * @author WangYinghua
 * 
 * @param <E>
 */
public class HeapSorter<E extends Comparable<E>> extends Sorter<E> {

	/*
	 * (non-Javadoc)
	 * 
	 * @see algorithms.Sorter#sort(E[], int, int)
	 */
	@Override
	public void sort(E[] array, int from, int len) {
		build_heap(array, from, len);

		for (int i = 0; i < len; i++) {
			// swap max value to the (len-i)-th position
			swap(array, from, from + len - 1 - i);
			shift_down(array, from, len - 1 - i, 0);// always shiftDown from 0
		}
	}

	private final void build_heap(E[] array, int from, int len) {
		int pos = (len - 1) / 2;// we start from (len-1)/2, because branch's
								// node +1=leaf's node, and all leaf node is
								// already a heap
		for (int i = pos; i >= 0; i--) {
			shift_down(array, from, len, i);
		}

	}

	private final void shift_down(E[] array, int from, int len, int pos) {

		E tmp = array[from + pos];
		int index = pos * 2 + 1;// use left child
		while (index < len)// until no child
		{
			if (index + 1 < len
					&& array[from + index].compareTo(array[from + index + 1]) < 0)// right
																					// child
																					// is
																					// bigger
			{
				index += 1;// switch to right child
			}
			if (tmp.compareTo(array[from + index]) < 0) {
				array[from + pos] = array[from + index];
				pos = index;
				index = pos * 2 + 1;

			} else {
				break;
			}

		}
		array[from + pos] = tmp;

	}

}