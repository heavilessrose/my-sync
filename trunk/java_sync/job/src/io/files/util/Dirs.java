package io.files.util;

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class Dirs {

	// 一个目录树.即管理该目录下所有文件和文件夹列表
	public static class TreeInfo implements Iterable {
		public List<File> files = new ArrayList<File>();
		public List<File> dirs = new ArrayList<File>();

		// 默认返回文件的迭代器
		public Iterator iterator() {
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

	/** 递归目录下所有文件及子文件夹 */
	TreeInfo recurseDirs(File startDir, String regex) {
		TreeInfo result = new TreeInfo();
		for (File item : startDir.listFiles()) {
			if (item.isDirectory()) {
				result.dirs.add(item);
				result.addAll(recurseDirs(item, regex));
			} else {
				if (item.getName().matches(regex))
					result.files.add(item);
			}
		}

		return result;
	}
	
	
}
