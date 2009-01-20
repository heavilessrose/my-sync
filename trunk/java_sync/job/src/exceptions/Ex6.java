package exceptions;

import java.util.logging.*;
import java.io.*;

class Oops1 extends Exception {
	private static Logger logger = Logger.getLogger("LoggingException");

	public Oops1() throws FileNotFoundException {
		StringWriter trace = new StringWriter();
		// 输出到指定流
		printStackTrace(new PrintWriter(trace));
		// 将track信息输出到标准错误
		logger.severe(trace.toString());
	}
}

class Oops2 extends Exception {
	private static Logger logger = Logger.getLogger("LoggingException");

	public Oops2() throws IOException {
		StringWriter trace = new StringWriter();
		FileWriter out = new FileWriter(new File("c:/ss.txt"));
		// 输出到指定流
		printStackTrace(new PrintWriter(out));
		out.close();
	}
}

public class Ex6 {
	static void f() throws Oops1, Oops2 {
		try {
			throw new Oops1();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}

	static void g() throws Oops2 {
		try {
			throw new Oops2();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		try {
			f();
		} catch (Exception Oops1) {
		}
		try {
			g();
		} catch (Exception Oops2) {
		}
	}
}
