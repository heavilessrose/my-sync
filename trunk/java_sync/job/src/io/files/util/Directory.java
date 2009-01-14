package io.files.util;

import java.util.regex.*;
import java.io.*;
import java.util.*;

public final class Directory {
	public static File[] local(File dir, final String regex) {
		return dir.listFiles(new FilenameFilter() {
			private Pattern pattern = Pattern.compile(regex);

			public boolean accept(File dir, String name) {
				return pattern.matcher(new File(name).getName()).matches();
			}
		});
	}

	public static File[] local(String path, final String regex) { // Overloaded
		return local(new File(path), regex);
	}

	// A two-tuple for returning a pair of objects:
	/** 一个目录树.即管理该目录下所有文件和文件夹列表 */
	public static class TreeInfo implements Iterable<File> {
		public List<File> files = new ArrayList<File>();
		public List<File> dirs = new ArrayList<File>();

		// The default iterable element is the file list:
		public Iterator<File> iterator() {
			return files.iterator();
		}

		void addAll(TreeInfo other) {
			files.addAll(other.files);
			dirs.addAll(other.dirs);
		}

		public String toString() {
			return "dirs: " + PPrint.pformat(dirs) + "\n\nfiles: "
					+ PPrint.pformat(files);
		}
	}

	// Begin recursion
	public static TreeInfo walk(String start, String regex) {
		return recurseDirs(new File(start), regex);
	}

	// Overloaded
	public static TreeInfo walk(File start, String regex) {
		return recurseDirs(start, regex);
	}

	// Everything
	public static TreeInfo walk(File start) {
		return recurseDirs(start, ".*");
	}

	public static TreeInfo walk(String start) {
		return recurseDirs(new File(start), ".*");
	}

	/** 递归目录下所有文件及子文件夹 */
	static TreeInfo recurseDirs(File startDir, String regex) {
		TreeInfo result = new TreeInfo();
		for (File item : startDir.listFiles()) {
			if (item.isDirectory()) {
				result.dirs.add(item);
				result.addAll(recurseDirs(item, regex));
			} else // Regular file
			if (item.getName().matches(regex))
				result.files.add(item);
		}
		return result;
	}

	// Simple validation test:
	public static void main(String[] args) {
		if (args.length == 0)
			System.out.println(walk("."));
		else
			for (String arg : args)
				System.out.println(walk(arg));
	}
} // /:~