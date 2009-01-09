package callback.innerclassImpl;

import static util.Print.*;

interface Incrementable {
	void increment();
}

// Very simple to just implement the interface:
// 通常的回调方式
class Callee1 implements Incrementable {
	private int i = 0;

	public void increment() {
		i++;
		print(i);
	}
}

// /////////////////
class MyIncrement {
	public void increment() {
		print("Other operation");
	}

	static void f(MyIncrement mi) {
		mi.increment();
	}
}

// If your class must implement increment() in
// some other way, you must use an inner class:
class Callee2 extends MyIncrement {
	private int i = 0;

	// 与接口要求的方法相同, 但意义不同
	public void increment() {
		super.increment();
		i++;
		print(i);
	}

	// 使用内部类来完成接口的功能, 实现回调
	private class Closure implements Incrementable {
		public void increment() {
			// Specify outer-class method, otherwise
			// you'd get an infinite recursion:
			Callee2.this.increment();
		}
	}

	Incrementable getCallbackReference() {
		return new Closure();
	}
}

class Caller {
	private Incrementable callbackReference;

	Caller(Incrementable cbh) {
		callbackReference = cbh;
	}

	void go() {
		callbackReference.increment();
	}
}

public class Callbacks {
	public static void main(String[] args) {
		Callee1 c1 = new Callee1();
		Callee2 c2 = new Callee2();
		MyIncrement.f(c2);
		Caller caller1 = new Caller(c1);
		// 注意此时caller2的回调方式
		Caller caller2 = new Caller(c2.getCallbackReference());
		caller1.go();
		caller1.go();
		caller2.go();
		caller2.go();
	}
}
