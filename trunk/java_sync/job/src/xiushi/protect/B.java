package xiushi.protect;

import xiushi.A;

public class B extends A{
	A newA(int cc){
		return new A(){
			int a = 2;
			int c = 2;
//			c = cc;
		};
	}
	public static void main(String[] args){
		// ��ʹ��A������, �����������ͬһ�����еĻ�Ҳ���ܷ��ʸ����protected��Ա
		System.out.println(new A().c);
		// c��ֵû�б�?? 
		// �޷�����ֵ?
		System.out.println(new B().newA(2).c);
	}
}
