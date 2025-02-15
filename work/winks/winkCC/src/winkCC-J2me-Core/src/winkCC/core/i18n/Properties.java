package winkCC.core.i18n;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.Hashtable;

import winkCC.core.WinksConstants;

/**
 * 属性对。管理配置信息,并未国际化提供支持.
 * 
 * @author WangYinghua
 * 
 */
public class Properties {
	/** 属性对 */
	private static Hashtable _propertyTable = null;

	// key和value的分隔符
	private static final String KEY_VALUE_SEPARATORS = "=: \t\r\n\f";

	// key和value的严格分隔符
	private static final String STRICT_KEY_VALUE_SEPARTORS = "=:";

	// 忽略的空白字符
	private static final String WHITESPACE_CHARS = " \t\r\n\f";

	/**
	 * 初始化property.<br>
	 * 处理属性文件只应调用这个方法.
	 * 
	 * @param messageBundle
	 *            property文件.
	 * @return 返回解析出来的键值对hashtable
	 */
	public static Hashtable initProperty(String messageBundle) {

		if (_propertyTable != null) {
			if (!_propertyTable.isEmpty())
				_propertyTable.clear();
			if (_propertyTable != null)
				_propertyTable = null;
		}
		_propertyTable = new Hashtable();

		loadPropertyBundle(WinksConstants.DEFAULT_PROPERTY_MESSAGES_BUNDLE);
		loadPropertyBundle(messageBundle);

		if (_propertyTable != null && !_propertyTable.isEmpty())
			return _propertyTable;
		else
			return null;
	}

	/**
	 * 载入属性文件, 提供属性文件的输入流.
	 * 
	 * @param propertyBundle 属性文件
	 * @return 是否成功
	 */
	public static boolean loadPropertyBundle(String propertyBundle) {
		// 相对路径
		if (propertyBundle != null && !propertyBundle.startsWith("/")) {
			propertyBundle = new StringBuffer(
					WinksConstants.DEFAULT_PROPERTY_RES_FOLDER).append(
					propertyBundle).toString();
		}

		InputStream inputStream = null;

		Class clazz = WHITESPACE_CHARS.getClass();
		inputStream = clazz.getResourceAsStream(propertyBundle);
		if (inputStream != null) {
			try {
				Properties.loadProperty(inputStream);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		return _propertyTable != null;
	}

	/**
	 * 载入key和value.
	 * 
	 * @param inStream 属性文件输入流
	 * @throws IOException 
	 */
	public static synchronized Hashtable /* void */loadProperty(
			InputStream inStream) throws IOException  {
		InputStreamReader inputStream = new InputStreamReader(inStream, "UTF-8");
		while (true) {
			// 去下一行
			String line = readLine(inputStream);
			if (line == null)
				return _propertyTable;

			if (line.length() > 0) {

				// key的起始位置
				int len = line.length();
				int keyStart;
				for (keyStart = 0; keyStart < len; keyStart++) {
					if (WHITESPACE_CHARS.indexOf(line.charAt(keyStart)) == -1) {
						break;
					}
				}

				// 忽略空行
				if (keyStart == len) {
					continue;
				}

				// 找到一行所有数据
				char firstChar = line.charAt(keyStart);
				if ((firstChar != '#') && (firstChar != '!')) {
					while (continueLine(line)) {
						String nextLine = readLine(inputStream);
						if (nextLine == null) {
							nextLine = "";
						}
						String loppedLine = line.substring(0, len - 1);
						int startIndex;
						for (startIndex = 0; startIndex < nextLine.length(); startIndex++) {
							if (WHITESPACE_CHARS.indexOf(nextLine
									.charAt(startIndex)) == -1) {
								break;
							}
						}
						nextLine = nextLine.substring(startIndex, nextLine
								.length());
						line = new String(loppedLine + nextLine);
						len = line.length();
					}

					// 找到key与value之间的分隔符位置
					int separatorIndex;
					for (separatorIndex = keyStart; separatorIndex < len; separatorIndex++) {
						char currentChar = line.charAt(separatorIndex);
						if (currentChar == '\\') {
							separatorIndex++;
						} else if (KEY_VALUE_SEPARATORS.indexOf(currentChar) != -1) {
							break;
						}
					}

					// 跳过key后的white space
					int valueIndex;
					for (valueIndex = separatorIndex; valueIndex < len; valueIndex++) {
						if (WHITESPACE_CHARS.indexOf(line.charAt(valueIndex)) == -1) {
							break;
						}
					}

					// 跳过非white space分隔符
					if (valueIndex < len) {
						if (STRICT_KEY_VALUE_SEPARTORS.indexOf(line
								.charAt(valueIndex)) != -1) {
							valueIndex++;
						}
					}

					// 跳过分隔符后的white space
					while (valueIndex < len) {
						if (WHITESPACE_CHARS.indexOf(line.charAt(valueIndex)) == -1) {
							break;
						}
						valueIndex++;
					}
					String key = line.substring(keyStart, separatorIndex);
					String value = (separatorIndex < len) ? line.substring(
							valueIndex, len) : "";

					key = convertString(key);
					value = convertString(value);
					if (_propertyTable == null)
						_propertyTable = new Hashtable();
					_propertyTable.put(key, value);
				}
			}
		}
	}

	/**
	 * 读取一行. 无数据返回null
	 * 
	 * @param in
	 *            输入流
	 * @return 一行字符
	 * @throws IOException
	 */
	private static String readLine(InputStreamReader in) throws IOException {
		StringBuffer strBuf = new StringBuffer("");
		int i;
		while ((i = in.read()) != -1) {
			if ((char) i == '\r' || (char) i == '\n') {
				return strBuf.toString();
			}
			strBuf.append((char) i);
		}
		return strBuf.length() > 0 ? strBuf.toString() : null;
	}

	/**
	 * 折行(按一行处理)
	 * 
	 * @param line
	 *            一行String
	 * @return true 如果String延续到下一行
	 */
	private static boolean continueLine(String line) {
		int slashCount = 0;
		int index = line.length() - 1;
		while ((index >= 0) && (line.charAt(index--) == '\\'))
			slashCount++;
		return (slashCount % 2 == 1);
	}

	/**
	 * 解码\\uxxxx 格式的unicode字符
	 * 
	 * @param theString
	 *            unicode字符
	 * @return 解码后的String
	 */
	private static String convertString(String theString) {
		char aChar;
		int len = theString.length();
		StringBuffer outBuffer = new StringBuffer(len);

		for (int x = 0; x < len;) {
			aChar = theString.charAt(x++);
			if (aChar == '\\') {
				aChar = theString.charAt(x++);
				if (aChar == 'u') {
					// xxxx
					int value = 0;
					for (int i = 0; i < 4; i++) {
						aChar = theString.charAt(x++);
						switch (aChar) {
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							value = (value << 4) + aChar - '0';
							break;
						case 'a':
						case 'b':
						case 'c':
						case 'd':
						case 'e':
						case 'f':
							value = (value << 4) + 10 + aChar - 'a';
							break;
						case 'A':
						case 'B':
						case 'C':
						case 'D':
						case 'E':
						case 'F':
							value = (value << 4) + 10 + aChar - 'A';
							break;
						default:
							// 未知
							return "???";
						}
					}
					outBuffer.append((char) value);
				} else {
					if (aChar == 't') {
						aChar = '\t';
					} else if (aChar == 'r') {
						aChar = '\r';
					} else if (aChar == 'n') {
						aChar = '\n';
					} else if (aChar == 'f') {
						aChar = '\f';
					}
					outBuffer.append(aChar);
				}
			} else {
				outBuffer.append(aChar);
			}
		}
		return outBuffer.toString();
	}
}
