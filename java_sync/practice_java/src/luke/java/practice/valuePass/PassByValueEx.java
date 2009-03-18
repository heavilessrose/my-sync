package luke.java.practice.valuePass;

//Java is just like c, everything pass by value!
//C++ is different, parameters can pass by reference!
//These are good interview questions and answers!
//Don't be confused by the word of "reference" and the expression "pass by reference"!

//The following is Java specific!

//For primitive types, value means the value of int, double, boolean, char, etc.
//You can change the value of primitive type inside a method
//but when the method returns, the value of passed in variable will stay 
//the same as before the methods call.

//For Objects, the value is the reference of the Object!
//When you pass an Object as parameter to a method
//You can change the reference of the object inside of a method
//but when the method returns, the reference of the object will stay  
//the same as before the methods call.

//!!Attention!!
//However, the values of data members of the Object can be changed permanently
//Since the copy of reference still refer to the same object.
//In other words, it is a shallow copy.
//See more discussion on this topic here

class MyObject {
	public int n = 10;
}

public class PassByValueEx {
	public static void main(String argv[]) {
		PassByValueEx pbve = new PassByValueEx();
		pbve.aMethod();
	}

	public void aMethod() {
		int i = 99;
		MyObject v = new MyObject();

		v.n = 30;

		// v and i are both pass by value
		bMethod(v, i);

		// after bMethod call, value of v and i are not changed
		// but the data member v.n is changed
		System.out.println(v.n + ", " + i); // 20, 99
	}

	public void bMethod(MyObject v, int i) {
		i = 0; // i value changed inside, but not outside bMethod
		v.n = 20; // The value of v.n is permanently changed
		v = new MyObject(); // reference of v changed inside, but not outside of 
		System.out.println(v.n + ", " + i); // 10, 0
	}
}

// output
// 10, 0
// 20, 99
