package io.files;

import java.util.regex.*;
import java.io.*;
import java.util.*;

// args: \D.*\.java == ^[0-9].*\.java 字母开头的*.java
// .*\.java 所有以.java结尾的
// \D*\.java == ^[0-9].*\.java 文件名中不能包含数字
// 直接写到java文件中的regex与作为命令行参数的regex是不同的, 多一个转义字符\
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
			// 返回满足指定pattern的文件名的字符串数组
			list = path.list(new FilenameFilter() {
				private Pattern pattern = Pattern.compile(args[0]);

				// 为每个文件(文件名)回调此策略
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
