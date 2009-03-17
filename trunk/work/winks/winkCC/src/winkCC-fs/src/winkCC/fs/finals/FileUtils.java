package winkCC.fs.finals;

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Enumeration;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;

public class FileUtils {

	/**
	 * 新线程中创建文件或目录.
	 * 
	 * @param name
	 * @param isDir
	 */
	public void executeCreateFile(final String name, final boolean isDir) {
		new Thread(new Runnable() {

			public void run() {
				if (isDir)
					createDir(name);
				else
					createFile(name);
			}
		}).start();
	}

	/**
	 * 创建文件.
	 * 
	 * @param filePath
	 */
	public void createFile(String filePath) {
		try {
			FileConnection fc = (FileConnection) Connector.open("file:///"
					+ filePath);
			if (!fc.exists()) {
				fc.create();
			} else {
				// 已存在
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 创建目录.
	 * 
	 * @param dirPath
	 */
	public void createDir(String dirPath) {
		if (dirPath.endsWith("/"))
			try {
				FileConnection fc = (FileConnection) Connector.open("file:///"
						+ dirPath);
				if (!fc.exists()) {
					fc.mkdir();
				} else {
					// 已存在
					return;
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
	}

	/**
	 * 新线程中写文件.
	 * 
	 * @param data
	 */
	public void executeWriteFile(final String fileName, final byte[] data) {
		new Thread(new Runnable() {

			public void run() {
				writeToFile(fileName, data);
			}
		}).start();
	}

	/**
	 * 将数据写入文件.
	 * 
	 * @param filePath
	 *            完整路径及文件名
	 * @param data
	 *            要写入的数据
	 */
	public void writeToFile(String filePath, byte[] data) {
		if (!filePath.endsWith("/")) {
			FileConnection fc = null;
			DataOutputStream out = null;

			String dir = null;
			String fileName = null;
			try {
				fc = (FileConnection) Connector.open("file:///root1/");
				if (!fc.exists())
					fc.mkdir();
				fc = (FileConnection) Connector.open("file:///root1/"
						+ filePath + ".jpg");
				if (!fc.exists())
					fc.create();

				out = fc.openDataOutputStream();
				out.write(data);
				out.flush();
			} catch (IOException e1) {
				e1.printStackTrace();
			} finally {
				try {
					out.close();
					fc.close();
				} catch (IOException e) {
					System.out.println("writeToFile: exception on close");
					e.printStackTrace();
				}
			}
		} else {
			// 路径不应以"/"结尾
		}
	}

	/**
	 * 列出文件夹中的所有文件.
	 * 
	 * @param dirPath
	 * @return An Enumeration of strings, denoting the files and directories in
	 *         the directory.The string returned contain only the file or
	 *         directory name and does not contain any path prefix.The
	 *         Enumeration has zero length if the directory is empty. Any hidden
	 *         files and directories in the directory are not included in the
	 *         returned list.
	 */
	public Enumeration listFiles(String dirPath) {
		Enumeration files = null;
		try {
			FileConnection fc = (FileConnection) Connector.open("file:///"
					+ dirPath);
			files = fc.list();
		} catch (IOException e) {
			//if invoked on a file, the directory does not exist, the directory is not accessible, or an I/O error occurs.
			e.printStackTrace();
		}
		return files;
	}

	/**
	 * 列出文件夹中的文件.
	 * 
	 * @param dirPath
	 * @param wildcard
	 *            An Enumeration of strings, denoting the files and directories
	 *            in the directory matching the filter.The Enumeration has zero
	 *            length if the directory is empty or no files and/or
	 *            directories are found matching the given filter.
	 * @param showHidden
	 *            是否显示隐藏文件.
	 * @return
	 */
	public Enumeration listFiles(String dirPath, String wildcard,
			boolean showHidden) {
		Enumeration files = null;
		try {
			FileConnection fc = (FileConnection) Connector.open("file:///"
					+ dirPath);
			files = fc.list(wildcard, showHidden);
		} catch (IOException e) {
			//if invoked on a file, the directory does not exist, the directory is not accessible, or an I/O error occurs.
			e.printStackTrace();
		}
		return files;
	}

	/**
	 * 判断是否为文件夹.
	 * 
	 * @param name
	 *            文件完整路径及名字.
	 * @return
	 */
	public boolean isDir(String name) {
		boolean isDir = false;
		FileConnection fc = null;
		try {
			fc = (FileConnection) Connector.open("file:///" + name);
			isDir = fc.isDirectory();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				fc.close();
				fc = null;
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return isDir;
	}

	/**
	 * 删除文件.
	 * 
	 * @param name
	 */
	public void delete(String name) {
		try {
			FileConnection fc = (FileConnection) Connector.open("file:///"
					+ name);
			fc.delete();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 重命名.
	 * 
	 * @param name
	 * @param newName
	 */
	public void rename(String name, String newName) {
		try {
			FileConnection fc = (FileConnection) Connector.open("file:///"
					+ name);
			fc.rename(newName);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	///////////////////
	// 解析后的文件路径
	private String path = null;
	// 解析后的文件名
	private String fileName = null;
	// 解析后的父目录
	private String parentPath = null;

	/**
	 * 分解文件路径.
	 * 
	 * @param pathName
	 */
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

	private String getPathString(String pathName) {
		parsePath(pathName);
		return path;
	}

	private String getFileNameString(String pathName) {
		parsePath(pathName);
		return fileName;
	}

	private String getParentPath(String dirpath) {
		parsePath(dirpath);
		return parentPath;
	}

	private boolean exists(String dirPath) {
		FileConnection fc = null;
		try {
			fc = (FileConnection) Connector.open("file:///" + dirPath);
		} catch (IOException e) {
			e.printStackTrace();
		}

		return fc.exists();
	}

	/**
	 * 创建目录.
	 * 
	 * @param dirPath
	 * @return
	 */
	public boolean mkDir(String dirPath) {
		FileConnection fc = null;
		try {
			fc = (FileConnection) Connector.open("file:///" + dirPath);
			if (exists(dirPath)) {
				return true;
			} else {
				fc.mkdir();
				if (exists(dirPath)) {
					return true;
				} else
					return false;
			}
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
	}

	/**
	 * 递归的创建目录.
	 * 
	 * @param dirPath
	 * @return 已存在返回false, 创建成功返回true.
	 */
	public boolean mkDirs(String dirPath) {
		if (exists(dirPath))
			return false;
		if (mkDir(dirPath)) {
			return true;
		}

		String parentPath = getParentPath(dirPath);
		return mkDirs(parentPath);
	}
}
