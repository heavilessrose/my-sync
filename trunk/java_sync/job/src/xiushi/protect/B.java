package xiushi.protect;

import xiushi.A;

public class B extends A{
	A newA(){
		return new A(){
			int a = 2;
			int c = 2;
		};
	}
	public static void main(String[] args){
		System.out.println(new A().c);
		System.out.println(new B().newA().c);
	}
}
