package winkCC.fs.finals;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;

public class FileUtils {

	/**
	 * 关闭文件连接.
	 * 
	 * @param fc
	 */
	private void closeFc(FileConnection fc) {
		try {
			fc.close();
			fc = null;
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 新线程中创建目录.
	 * 
	 * @param path
	 */
	private void executeMkDirs(final String path) {
		new Thread() {
			public void run() {
				mkDirs(path);
			}
		}.start();
	}

	/**
	 * 新线程中创建文件.
	 * 
	 * @param path
	 */
	private void executeMkFile(final String path) {
		new Thread() {
			public void run() {
				mkFile(path);
			}
		}.start();
	}

	/**
	 * 新线程中创建文件或目录.
	 * 
	 * @param name
	 * @param isDir
	 */
	public void executeCreateFile(final String filePath) {
		new Thread() {
			public void run() {
				createFile(filePath);
			}
		}.start();
	}

	/**
	 * 创建文件或目录.
	 * 
	 * @param filePath
	 */
	private void createFile(String filePath) {
		if (isDir(filePath)) {
			mkDirs(filePath);
		} else {
			String parentDir = getParentPath(filePath);
			if (!exists(parentDir)) { // 目录不存在,建立目录.
				System.out.println("make dirs: " + parentDir);
				mkDirs(parentDir);
			}

			System.out.println("start create file");
			mkFile(filePath);
		}
	}

	/**
	 * 创建文件.
	 * 
	 * @param filePath
	 */
	private void mkFile(String filePath) {
		if (!filePath.endsWith("/")) {
			FileConnection fc = null;
			try {
				fc = (FileConnection) Connector.open("file:///" + filePath);
				fc.create();
			} catch (IOException e) {
				e.printStackTrace();
			} finally {
				closeFc(fc);
			}
		} else {
			// 
		}
	}

	/**
	 * 新线程中写文件.
	 * 
	 * @param fileName
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
	 *            完整路径及文件名,文件路径不应以"/"结尾.
	 * @param data
	 *            要写入的数据.
	 */
	private void writeToFile(String filePath, byte[] data) {
		if (!filePath.endsWith("/")) {
			FileConnection fc = null;
			DataOutputStream out = null;

			try {
				if (!exists(filePath)) {
					createFile(filePath);
				}

				fc = (FileConnection) Connector.open("file:///" + filePath);

				out = fc.openDataOutputStream();
				out.write(data);
				out.flush();
			} catch (IOException e) {
				e.printStackTrace();
			} finally {
				try {
					out.close();
					closeFc(fc);
				} catch (IOException e) {
					System.out.println("writeToFile: exception on close");
					e.printStackTrace();
				}
			}
		} else {
			// 文件路径不应以"/"结尾.
		}
	}

	/**
	 * 新线程中读取文件到指定buffer.
	 * 
	 * @param filePath
	 *            文件完整路径.
	 * @param buffer
	 *            将文件读入该buffer.
	 */
	public void executeReadFile(final String filePath, final byte[] buffer) {
		new Thread(new Runnable() {
			public void run() {
				readFile(filePath, buffer);

				System.out.println(toHex(buffer));
			}
		}).start();
	}

	/**
	 * 将文件数据读入byte[] buffer.
	 * 
	 * @param fileName
	 *            完整路径.
	 * @param buffer
	 * @return
	 */
	private void readFile(String fileName, byte[] buffer) {
		if (!fileName.endsWith("/")) {
			FileConnection fc = null;
			InputStream in = null;
			try {
				fc = (FileConnection) Connector.open("file:///" + fileName);
				in = fc.openInputStream();
				in.read(buffer);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * 二进制到16进制.
	 * 
	 * @param data
	 * @return
	 */
	final public String toHex(byte[] data) {
		StringBuffer buf = new StringBuffer();
		for (int i = 0; i < data.length; i++) {
			int intData = (int) data[i] & 0xFF;
			if (intData < 0x10)
				buf.append("0");
			buf.append(Integer.toHexString(intData));
			buf.append(' ');
		}
		return (buf.toString());
	}

	/**
	 * 列出文件夹中的所有文件.
	 * 
	 * @param dirPath
	 *            文件夹完整路径.
	 * @return An Enumeration of strings, denoting the files and directories in
	 *         the directory.The string returned contain only the file or
	 *         directory name and does not contain any path prefix.The
	 *         Enumeration has zero length if the directory is empty. Any hidden
	 *         files and directories in the directory are not included in the
	 *         returned list.
	 */
	public Enumeration listFiles(String dirPath) {
		Enumeration files = null;
		if (dirPath.endsWith("/")) {
			if (exists(dirPath)) {
				try {
					FileConnection fc = (FileConnection) Connector
							.open("file:///" + dirPath);
					files = fc.list();
				} catch (IOException e) {
					// 不存在, 无法访问, 或IO错误.
					e.printStackTrace();
				}
			} else {
				System.out.println("dir does not exist");
			}
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
			e.printStackTrace();
		}
		return files;
	}

	/**
	 * 判断是否为文件夹.
	 * 
	 * @param path
	 *            文件完整路径及名字.
	 * @return
	 */
	public boolean isDir(String path) {
		FileConnection fc = null;
		try {
			fc = (FileConnection) Connector.open("file:///" + path);
			if (fc.exists()) {
				return fc.isDirectory();
			} else {
				if (path.endsWith("/"))
					return true;
				return false;
			}
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		} finally {
			closeFc(fc);
		}
	}

	/**
	 * 新线程中删除指定文件.
	 * 
	 * @param filePath
	 *            文件路径.
	 */
	public void executeDelete(final String filePath) {
		new Thread(new Runnable() {

			public void run() {
				delete(filePath);
			}
		}).start();
	}

	/**
	 * 删除文件或文件夹.
	 * 
	 * @param name
	 */
	private void delete(String name) {
		try {
			FileConnection fc = (FileConnection) Connector.open("file:///"
					+ name);
			fc.delete();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 重命名文件或文件夹.
	 * 
	 * @param filePath
	 *            文件夹或文件完整路径.
	 * @param newName
	 *            新文件名.
	 */
	public void rename(String filePath, String newName) {
		try {
			FileConnection fc = (FileConnection) Connector.open("file:///"
					+ filePath);
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
	 *            文件或文件夹完整路径.
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
		if (!pathName.endsWith("/")) {
			parentPathIndex = pathIndex;
		}

		parentPath = pathName.substring(0, parentPathIndex);
		path = pathName.substring(0, pathIndex);
		fileName = pathName.substring(pathIndex);

		System.out.println("parentPath: " + parentPath);
		System.out.println("path: " + path);
		System.out.println("fileName: " + fileName);
	}

	/**
	 * 得到路径.
	 * 
	 * @param pathName
	 * @return
	 */
	private String getPathString(String pathName) {
		parsePath(pathName);
		return path;
	}

	/**
	 * 分解路径得到文件名.
	 * 
	 * @param pathName
	 * @return
	 */
	private String getFileNameString(String pathName) {
		parsePath(pathName);
		return fileName;
	}

	/**
	 * 得到父文件夹的完整路径.
	 * 
	 * @param dirpath
	 * @return
	 */
	private String getParentPath(String dirpath) {
		parsePath(dirpath);
		return parentPath;
	}

	/**
	 * 判断文件或文件夹是否存在.
	 * 
	 * @param dirPath
	 * @return
	 */
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
	 * 创建单个文件夹.
	 * 
	 * @param dirPath
	 *            文件夹结尾应包含"/", 如:"e:/11/"
	 * @return 失败返回false, 创建成功返回true.
	 */
	private boolean mkDir(String dirPath) {
		FileConnection fc = null;
		try {
			fc = (FileConnection) Connector.open("file:///" + dirPath);
			fc.mkdir();
		} catch (IOException e) {
			//			e.printStackTrace();
			return false;
		}
		return true;
	}

	/**
	 * 递归的创建目录.
	 * 
	 * @param dirPath
	 *            文件夹结尾应包含"/", 如:"e:/11/22/33/"
	 * @return 失败返回false, 创建成功返回true.
	 */
	private boolean mkDirs(String dirPath) {
		if (exists(dirPath))
			return true;
		if (mkDir(dirPath))
			return true;

		String parentPath = getParentPath(dirPath);
		mkDirs(parentPath);
		return mkDir(dirPath);
	}
}
