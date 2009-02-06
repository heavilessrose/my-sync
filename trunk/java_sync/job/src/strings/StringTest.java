package strings;

/** 字符串反转 */
public class StringTest {
	private static int count = 0;

	// 递归实现(按单词反转)
	private static String func(String str) {
		// 得到' '的位置, 如果未发现该字符返回 -1
		int i = str.indexOf(' ');
		System.out.println(i);
		count++;
		if (i < 0)
			return str;
		else {
			String w = str.substring(0, i);
			return func(str.substring(i + 1)) + " " + w;
		}
	}

	// 按字符反转
	public static String reverseString(String str) {
		String result = "";
		for (int i = 0; i < str.length(); i++) {
			result = result
					+ str.substring(str.length() - i - 1, str.length() - i);
		}

		return result;
	}

	public static void main(String args[]) {
		String out = func("one two three four");
		for (int i = 0; i < count; i++) {
			out += " " + (i + 1);
		}
		System.out.println(out);
		// ============
		System.out.println(StringTest.reverseString("one two three four"));
	}
}