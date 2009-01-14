package io.files;

import java.util.regex.*;
import java.io.*;
import java.util.*;

// args: \D.*\.java == ^[0-9].*\.java ��ĸ��ͷ��*.java
// .*\.java ������.java��β��
// \D*\.java == ^[0-9].*\.java �ļ����в��ܰ�������
// ֱ��д��java�ļ��е�regex����Ϊ�����в�����regex�ǲ�ͬ��, ��һ��ת���ַ�\
public class DirList {
	public static void main(final String[] args) {
		// boolean b = Pattern.matches("\\D.*\\.java", "asdf.java");
		 boolean b = Pattern.matches("\\D.*\\.java", "1asdf.java");
		// boolean b = Pattern.matches("a*b", "aaaaab");
		 System.out.println(b);
		// System.out.println(args.length + ", " + args[0]);
		File path = new File("src");
		String[] list;
		if (args.length == 0)
			list = path.list();
		else
			// ��������ָ��pattern���ļ������ַ�������
			list = path.list(new FilenameFilter() {
				private Pattern pattern = Pattern.compile(args[0]);

				// Ϊÿ���ļ�(�ļ���)�ص��˲���
				public boolean accept(File dir, String fileName) {
					boolean b = pattern.matcher(fileName).matches();
					// System.out.println(fileName + ": " + b);
					return b;
				}
			});
		Arrays.sort(list, String.CASE_INSENSITIVE_ORDER);
		System.out.println(list.length);
		for (int i = 0; i < list.length; i++) {
			System.out.println(list[i]);
		}
		// for (String dirItem : list)
		// System.out.println(dirItem);
	}
}
