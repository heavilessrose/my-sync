package xiushi;

import xiushi.A;

public class B /*extends A*/{
	public static void main(String[] args){
		// 在一个包中即使不是A的子类也可以访问protected成员
		System.out.println(new A().a);
	}
}
