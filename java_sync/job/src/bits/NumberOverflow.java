package bits;

/** ��ֵ��� */
public class NumberOverflow {
	public static void main(String[] args) {

		// i����Ϊ1000000���ڳ˷�ʱJava���ֽ����1000000000000���Ѿ�������int�����������͵����Χ��2147483647����
		// ��������Ĭ�ϵ�����������type promotion�����м�����Ϊlong���ʹ�ţ����ؽ��ʱĿ����������int���ܹ������½����
		// ���Ǹ���Java�Ļ������͵ı�խת����Narrowing
		// primitiveconversion�����򣬰ѽ������int���Ϳ�ȵĲ���ȫ��������Ҳ����ֻȡ����ĵ�32λ�����Ǿ͵õ�������Ľ����

		// ������һ��ʮ�����Ʊ�ʾ�����Ӳ����������
		// int i3 = 1000000;
		// System.out.println (Long.toHexString (i3*i3).toUpperCase());
		// System.out.println (Long.toHexString (i3*i3).toUpperCase());
		// System.out.println (Integer.toHexString (i3*i3).toUpperCase());
		// System.out.println ((int)i3*i3);
		// ---------------------------------------------------
		// FFFFFFFFD4A51000
		// 1000000000000
		// D4A51000
		// -727379968
		// ---------------------------------------------------

		int i = 1000000;
		System.out.println(i * i);
	}
}
