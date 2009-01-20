package strings;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.StringWriter;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Pattern;

public class StringsTest {

	public static void main(String[] args) {

		String str = "����Ƽ���what are are you doing now? my dear xiaoZi ��iehˮ���Ƽ��ϵ۷�Ŷ�����̽�ʡ����˿Ƽ�ʱ����iuqw��ʦ�佱�Է�.�ռ�������Ŷ ";
		char[] chars = str.toCharArray();
		System.out.println(new String(chars, 2, 5));
		System.out.println(str.substring(2, 5));
		// ///////
		System.out.println(str.indexOf("are", 2));
		System.out.println(str.contains(" are "));
		System.out.println(str.startsWith("����"));
		System.out.println(str.endsWith("Ŷ "));
		System.out.println(str.indexOf("are"));
		System.out.println(str.lastIndexOf("are"));
		System.out.println(str.substring(0, 5));
		System.out.println(str.valueOf(123));

		System.out.printf("row 1 %s: [%d, %f]\n", "is", 123, 123.0D);

		int[] data = new int[] { 1, 2, 3 };
		for (int a : data) {
			System.out.println(a);
		}

		System.out.println("-1234".matches("-?\\d+"));
		System.out.println("+1234".matches("(-|\\+)?\\d+"));
		// ///////////
		List<String> ls = new LinkedList<String>();
		BufferedReader in = null;
		try {
			in = new BufferedReader(new FileReader("c:/ss.txt"));
			StringBuilder sb = new StringBuilder();
			String s = null;
			while ((s = in.readLine()) != null) {
				sb.append(s);
				ls.add(s.toUpperCase());
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				in.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		// Pattern p = Pattern.compile("exceptions.Oops2");

		for (String s : ls) {
			if (s.matches("\\w.* .*")) // ƥ���ַ���ͷ������һ���ո�
				System.out.println(s);
		}
	}
}
