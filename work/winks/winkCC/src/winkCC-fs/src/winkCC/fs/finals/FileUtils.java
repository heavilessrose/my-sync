package winkCC.fs.finals;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;

public class FileUtils {

	/**
	 * �ر��ļ�����.
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
	 * ���߳��д���Ŀ¼.
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
	 * ���߳��д����ļ�.
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
	 * ���߳��д����ļ���Ŀ¼.
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
	 * �����ļ���Ŀ¼.
	 * 
	 * @param filePath
	 */
	private void createFile(String filePath) {
		if (isDir(filePath)) {
			mkDirs(filePath);
		} else {
			String parentDir = getParentPath(filePath);
			if (!exists(parentDir)) { // Ŀ¼������,����Ŀ¼.
				System.out.println("make dirs: " + parentDir);
				mkDirs(parentDir);
			}

			System.out.println("start create file");
			mkFile(filePath);
		}
	}

	/**
	 * �����ļ�.
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
	 * ���߳���д�ļ�.
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
	 * ������д���ļ�.
	 * 
	 * @param filePath
	 *            ����·�����ļ���,�ļ�·����Ӧ��"/"��β.
	 * @param data
	 *            Ҫд�������.
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
			// �ļ�·����Ӧ��"/"��β.
		}
	}

	/**
	 * ���߳��ж�ȡ�ļ���ָ��buffer.
	 * 
	 * @param filePath
	 *            �ļ�����·��.
	 * @param buffer
	 *            ���ļ������buffer.
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
	 * ���ļ����ݶ���byte[] buffer.
	 * 
	 * @param fileName
	 *            ����·��.
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
	 * �����Ƶ�16����.
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
	 * �г��ļ����е������ļ�.
	 * 
	 * @param dirPath
	 *            �ļ�������·��.
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
					// ������, �޷�����, ��IO����.
					e.printStackTrace();
				}
			} else {
				System.out.println("dir does not exist");
			}
		}
		return files;
	}

	/**
	 * �г��ļ����е��ļ�.
	 * 
	 * @param dirPath
	 * @param wildcard
	 *            An Enumeration of strings, denoting the files and directories
	 *            in the directory matching the filter.The Enumeration has zero
	 *            length if the directory is empty or no files and/or
	 *            directories are found matching the given filter.
	 * @param showHidden
	 *            �Ƿ���ʾ�����ļ�.
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
	 * �ж��Ƿ�Ϊ�ļ���.
	 * 
	 * @param path
	 *            �ļ�����·��������.
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
	 * ���߳���ɾ��ָ���ļ�.
	 * 
	 * @param filePath
	 *            �ļ�·��.
	 */
	public void executeDelete(final String filePath) {
		new Thread(new Runnable() {

			public void run() {
				delete(filePath);
			}
		}).start();
	}

	/**
	 * ɾ���ļ����ļ���.
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
	 * �������ļ����ļ���.
	 * 
	 * @param filePath
	 *            �ļ��л��ļ�����·��.
	 * @param newName
	 *            ���ļ���.
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
	// ��������ļ�·��
	private String path = null;
	// ��������ļ���
	private String fileName = null;
	// ������ĸ�Ŀ¼
	private String parentPath = null;

	/**
	 * �ֽ��ļ�·��.
	 * 
	 * @param pathName
	 *            �ļ����ļ�������·��.
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
	 * �õ�·��.
	 * 
	 * @param pathName
	 * @return
	 */
	private String getPathString(String pathName) {
		parsePath(pathName);
		return path;
	}

	/**
	 * �ֽ�·���õ��ļ���.
	 * 
	 * @param pathName
	 * @return
	 */
	private String getFileNameString(String pathName) {
		parsePath(pathName);
		return fileName;
	}

	/**
	 * �õ����ļ��е�����·��.
	 * 
	 * @param dirpath
	 * @return
	 */
	private String getParentPath(String dirpath) {
		parsePath(dirpath);
		return parentPath;
	}

	/**
	 * �ж��ļ����ļ����Ƿ����.
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
	 * ���������ļ���.
	 * 
	 * @param dirPath
	 *            �ļ��н�βӦ����"/", ��:"e:/11/"
	 * @return ʧ�ܷ���false, �����ɹ�����true.
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
	 * �ݹ�Ĵ���Ŀ¼.
	 * 
	 * @param dirPath
	 *            �ļ��н�βӦ����"/", ��:"e:/11/22/33/"
	 * @return ʧ�ܷ���false, �����ɹ�����true.
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
