package luke.android.utils;

public class Sort {
	/**
	 * ð����������
	 * 
	 * @param n
	 *            Ҫ���������
	 * @param direct
	 *            ����0 ��ʾ��С����, С��0 ��ʾ�Ӵ�С
	 */
	public static final void bullonSort(int[] n, int direct) {
		int temp;
		boolean exchange = false;
		for (int i = 0; i < n.length - 1; i++) { // �����n-1������
			exchange = false;
			for (int j = n.length - 1; j > i; j--) {// ���µ��ϱȽ�
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
			// �������û�н��н������ʾ�Ѿ��������
			if (!exchange)
				return;
		}
	}

	// ======================== 
	public static final void quickSort(int[] n, int low, int high){
		//��R[low..high]��������
	     int pivotpos; //���ֺ�Ļ�׼��¼��λ��
	     if(low<high){//�������䳤�ȴ���1ʱ��������
	        pivotpos=Partition(n,low,high); //��R[low..high]������
	        quickSort(n,low,pivotpos-1); //��������ݹ�����
	        quickSort(n,pivotpos+1,high); //��������ݹ�����
	      }
		}

		public static void main(String[] args) {
			int[] n = { 23, 35, 12, 20, -34, 64, 71, 41 };
			int low = n[0];
			int high = n[n.length-1];
			
			for (int i = 0; i < n.length; i++) {
				System.out.print(n[i] + ", ");
			}
		}
}
