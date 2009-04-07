package winkCC.core.property_i18n;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UTFDataFormatException;
import java.util.Hashtable;

import winkCC.core.WinksConstants;

/**
 * ϵͳ��������ʻ�.
 * 
 * @author WangYinghua
 * 
 */
public class Properties {
	/**
	 * �����ļ�����·��, Ĭ��ΪSD��winks_ccĿ¼��.
	 */
	public String CC_PATH = "e:/winks_cc/";
	/**
	 * ������ϵ���ļ�cc_contacts.xml.
	 */
	public String CC_CONTACTS_FILE = "c:/winks_property/cc_contacts.xml";

	/**
	 * ���ñ�������·��.
	 * 
	 * @param path
	 */
	public void setCCPath(String path) {
		CC_PATH = path;
	}

	public String _confServer;
	public String _commServer;
	public String _ccServer;
	public String _smsAddr;
	public String _smsPort;
	public String _imsi;
	public String _uid;
	public String _version;
	public String _step;

	//-------------�û�״̬
	public static byte _userState = 0;
	/** δ���� */
	public static final byte USR_STATE_NONACTIVED = 0;
	/** ���� */
	public static final byte USR_STATE_ACTIVED = 1;
	/** ��Ĭ */
	public static final byte USR_STATE_SILENT = 2;
	/** �˶� */
	public static final byte USR_STATE_QUIT = 3;
	/** ���� */
	public static final byte USR_STATE_FORBIDEN = 4;

	//-------------����״̬
	public static byte _subscribeState = 0;
	/** ���� */
	public static final byte SUB_STATE_LIMITED = 1;
	/** ���� */
	public static final byte SUB_STATE_TRIAL = 2;
	/** ���ܹ�� */
	public static final byte SUB_STATE_ADWARE = 3;
	/** ������ */
	public static final byte SUB_STATE_SUBSCRIBER = 4;

	//--------------------------

	/**
	 * ģ���ʼ��. �������ʱ�����������, �͹��ʻ�����.
	 */
	public void init() {

	}

	/**
	 * �趨������
	 * 
	 * @param name
	 *            ������
	 * @param value
	 *            ����ֵ
	 */
	public static void setProperty(String name, String value) {

	}

	/**
	 * ��ȡ����
	 * 
	 * @param name
	 *            ������
	 * @return ���õ�ֵ
	 */
	public static String getProperty(String name) {
		return null;
	}

	/**
	 * �����û�״̬.
	 * 
	 * @param userState
	 */
	public static void setUserState(byte userState) {
		_userState = userState;
	}

	/**
	 * ��ȡ�û�״̬.
	 * 
	 * @param userState
	 * @return
	 */
	public static byte getUserState() {
		return _userState;
	}

	/**
	 * ��ȡ�û�����״̬.
	 * 
	 * @return
	 */
	public static byte getSubscribeState() {
		return _subscribeState;
	}

	/**
	 * �����û�����״̬.
	 * 
	 * @param subscribeState
	 */
	public static void setSubscribeState(byte subscribeState) {
		_subscribeState = subscribeState;
	}

	//---------------------------------------------
	/** �ֻ���ǰ��������趨 */
	public static String _locale = "zh-CN";
	public static String _encoding = "ISO-8859-1";
	private static Hashtable _messageTable = null;

	// key��value�ķָ���
	private static final String KEY_VALUE_SEPARATORS = "=: \t\r\n\f";

	// key��value���ϸ�ָ���
	private static final String STRICT_KEY_VALUE_SEPARTORS = "=:";

	// ���ԵĿհ��ַ�
	private static final String WHITESPACE_CHARS = " \t\r\n\f";

	/**
	 * ���locale ��Ĭ��locale zh-CN.
	 * 
	 * @return
	 */
	public static String getLocale() {
		try {
			_locale = System.getProperty("microedition.locale");
		} catch (Exception e) {
		}
		return _locale;
	}

	/**
	 * ��ʼ�����ʻ�
	 * 
	 * @param messageBundle
	 *            ���ʻ��ļ�.
	 * @param locale
	 * @return
	 */
	public static boolean initI18nSupport(String messageBundle, String locale) {

		_messageTable = new Hashtable();

		// ����locale
		_locale = locale;

		loadI18nBundle(WinksConstants.DEFAULT_I18N_MESSAGES_BUNDLE);
		loadI18nBundle(messageBundle);

		return _messageTable != null;
	}

	/**
	 * ������ʻ�.
	 * 
	 * @param messageBundle
	 *            ���ʻ��ļ���·��(֧�־���·����ֻ�����ļ���).
	 * @return ��ȡ���ʻ��ļ��Ƿ�ɹ�.
	 */
	public static boolean loadI18nBundle(String messageBundle) {

		// ���·��
		if (messageBundle != null && !messageBundle.startsWith("/")) {
			messageBundle = new StringBuffer(
					WinksConstants.DEFAULT_I18N_RES_FOLDER).append(
					messageBundle).toString();
		}

		InputStream inputStream = null;
		Class clazz = _locale.getClass();
		try {

			// Construct messageBundle
			if ((_locale != null) && (_locale.length() > 1)) {
				int lastIndex = messageBundle.lastIndexOf('.');
				String prefix = messageBundle.substring(0, lastIndex);
				String suffix = messageBundle.substring(lastIndex);
				// replace '-' with '_', some phones returns locales with
				// '-' instead of '_'. For example Nokia or Motorola
				_locale = _locale.replace('-', '_');
				inputStream = clazz
						.getResourceAsStream(new StringBuffer(prefix).append(
								'.').append(_locale).append(suffix).toString());
				if (inputStream == null) {
					// û�ҵ���Դ�� ȡ��zh_CN����zh
					_locale = _locale.substring(0, 2);
					inputStream = clazz.getResourceAsStream(new StringBuffer(
							prefix).append('.').append(_locale).append(suffix)
							.toString());
				}
			}
			if (inputStream == null) {
				// if not found or locale is not set, try default locale
				inputStream = clazz.getResourceAsStream(messageBundle);
			}
			if (inputStream != null) {
				// load messages to messageTable hashtable
				loadMessages(inputStream);
			}
		} catch (UTFDataFormatException e) {
			System.err
					.println("I18N Error : *.properties files need to be UTF-8 encoded");
		} catch (Exception e) {
			e.printStackTrace();
		}

		return _messageTable != null;
	}

	/**
	 * ����key��value.
	 * 
	 * @param inStream
	 * @throws Exception
	 */
	private static synchronized void loadMessages(InputStream inStream)
			throws Exception {

		InputStreamReader inputStream = new InputStreamReader(inStream, "UTF-8");
		while (true) {
			// Get next line
			String line = readLine(inputStream);
			if (line == null)
				return;

			if (line.length() > 0) {

				// Find start of key
				int len = line.length();
				int keyStart;
				for (keyStart = 0; keyStart < len; keyStart++) {
					if (WHITESPACE_CHARS.indexOf(line.charAt(keyStart)) == -1) {
						break;
					}
				}

				// Blank lines are ignored
				if (keyStart == len) {
					continue;
				}

				// �ݹ��ҵ�һ����������
				char firstChar = line.charAt(keyStart);
				if ((firstChar != '#') && (firstChar != '!')) {
					while (continueLine(line)) {
						String nextLine = readLine(inputStream);
						if (nextLine == null) {
							nextLine = "";
						}
						String loppedLine = line.substring(0, len - 1);
						// Advance beyond whitespace on new line
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

					// Find separation between key and value
					int separatorIndex;
					for (separatorIndex = keyStart; separatorIndex < len; separatorIndex++) {
						char currentChar = line.charAt(separatorIndex);
						if (currentChar == '\\') {
							separatorIndex++;
						} else if (KEY_VALUE_SEPARATORS.indexOf(currentChar) != -1) {
							break;
						}
					}

					// Skip over whitespace after key if any
					int valueIndex;
					for (valueIndex = separatorIndex; valueIndex < len; valueIndex++) {
						if (WHITESPACE_CHARS.indexOf(line.charAt(valueIndex)) == -1) {
							break;
						}
					}

					// Skip over one non whitespace key value separators if any
					if (valueIndex < len) {
						if (STRICT_KEY_VALUE_SEPARTORS.indexOf(line
								.charAt(valueIndex)) != -1) {
							valueIndex++;
						}
					}

					// Skip over white space after other separators if any
					while (valueIndex < len) {
						if (WHITESPACE_CHARS.indexOf(line.charAt(valueIndex)) == -1) {
							break;
						}
						valueIndex++;
					}
					String key = line.substring(keyStart, separatorIndex);
					String value = (separatorIndex < len) ? line.substring(
							valueIndex, len) : "";

					// Convert then store key and value
					key = convertString(key);
					value = convertString(value);
					_messageTable.put(key, value);
				}
			}
		}
	}

	/**
	 * ��ȡһ��. �����ݷ���null
	 * 
	 * @param in
	 *            ������
	 * @return һ���ַ�
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
	 * ����(��һ�д���)
	 * 
	 * @param line
	 *            a line of String
	 * @return true if the string continues on the next line, false otherwise
	 */
	private static boolean continueLine(String line) {
		int slashCount = 0;
		int index = line.length() - 1;
		while ((index >= 0) && (line.charAt(index--) == '\\'))
			slashCount++;
		return (slashCount % 2 == 1);
	}

	/**
	 * Decodes a String which uses unicode characters in \\uXXXX format.
	 * 
	 * @param theString
	 *            String with \\uXXXX characters
	 * @return resolved string
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
					// Read the xxxx
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
							// return unknown
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
