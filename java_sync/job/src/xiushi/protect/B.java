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
		// 即使是A的子类, 但是如果不在同一个包中的话也不能访问父类的protected成员
		System.out.println(new A().c);
		// c的值没有变?? 
		// 无法赋初值?
		System.out.println(new B().newA(2).c);
	}
}
