package io.files;

import java.io.File;
import java.io.FileFilter;
import java.io.FilenameFilter;
import java.util.Arrays;
import java.util.regex.Pattern;

public class SortedDirList {
	String[] allSortedFileList = null;
	File path = null;

	public SortedDirList(File path) {
		this.path = path;
		// allSortedFileList = this.path.list();
		// Arrays.sort(allSortedFileList);
	}

	private String[] list() {
		String[] list = null;
		list = path.list();
		return list;
	}

	private String[] list(final String regex) {
		String[] list = null;
		list = path.list(new FilenameFilter() {
			private Pattern pattern = Pattern.compile(regex);

			public boolean accept(File dir, String filename) {
				return pattern.matcher(filename).matches();
			}

		});
		return list;
	}

	private File[] listFiles(final String regex) {
		File[] files = null;
		files = path.listFiles(new FileFilter() {

			public boolean accept(File pathname) {
				return pathname.isFile();
			}

		});

		return files;
	}

	public static void main(String[] args) {
		SortedDirList ss = new SortedDirList(new File("src"));
		// String[] list = ss.list("\\D.*\\.java");
		// for (String s : list) {
		// System.out.println(s);
		// }
		//
		// File tmp = new File("src/" + list[0]);
		// if (tmp.isFile()) {
		// long tmplen = tmp.length();
		// System.out.println(tmplen);
		// }
		File[] list = ss.listFiles("");
		long allFileLength = 0L;
		for (File ff : list) {
			System.out.println(ff.getName());
			allFileLength += ff.length();
		}
		System.out.println(allFileLength);
	}
}
