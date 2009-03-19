package luke.java.practice.dir;

public class DirSplit {
	String path = null;
	String fileName = null;
	String parentPath = null;

	private void parsePath(String pathName) {
		int pathIndex = 0;
		int parentPathIndex = 0;
		char[] chars = pathName.toCharArray();
		for (int i = 0; i < chars.length; i++) {
			if (chars[i] == '/') {
				if (pathIndex != 0) {
					parentPathIndex = pathIndex;
				}
				pathIndex = i + 1;
			}
		}
		parentPath = pathName.substring(0, parentPathIndex);
		path = pathName.substring(0, pathIndex);
		fileName = pathName.substring(pathIndex);
	}

	public String getParentPath(String dirpath) {
		parsePath(dirpath);
		return parentPath;
	}

	public void test(String a) {
		//		int i = 1;
		//		try {
		//			i = 2;
		//			throw new Exception();
		//		} catch (Exception e) {
		//			i = 3;
		//			i++;
		//			e.printStackTrace();
		//			return i;
		//		}
		/////////////
		a = "1";
	}

	public static void main(String[] args) {
		DirSplit ss = new DirSplit();
		ss.parsePath("c:/local_repo/j2me/jsrs/mmapi/sr/main/java/");

		System.out.println("path: " + ss.path);
		System.out.println("fileName: " + ss.fileName);
		System.out.println("parentPath: " + ss.parentPath);

		String a = "0";
		ss.test(a);
		System.out.println(a);
	}
}
