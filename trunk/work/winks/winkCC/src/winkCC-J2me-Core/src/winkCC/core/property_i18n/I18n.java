package winkCC.core.property_i18n;

import java.io.InputStream;
import java.io.UTFDataFormatException;
import java.io.UnsupportedEncodingException;
import java.util.Hashtable;

import winkCC.core.WinksConstants;

public class I18n {

	/** �ֻ���ǰlocale */
	public static String _locale = "zh-CN";
	/** �ֻ���ǰ���� */
	public static String _encoding = "ISO-8859-1";
	/** ���Զ� */
	public static Hashtable _messageTable = null;

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
	 * ��ʼ�����ʻ�property.<br>
	 * ������ʻ�ֻӦ���ô˷���.
	 * 
	 * @param messageBundle
	 *            property�ļ�.
	 * @param locale
	 *            locale��Ҫ�ṩ��Ӧ����messages.cn.properties�������ļ�.
	 * @return ������ȡ���Ĺ��ʻ��ļ��е��������ԶԵ�Hashtable. ������null.
	 */
	public static Hashtable initI18nProperty(String messageBundle, String locale) {

		if (_messageTable != null) {
			if (!_messageTable.isEmpty())
				_messageTable.clear();
			if (_messageTable != null)
				_messageTable = null;
		}
		_messageTable = new Hashtable();

		// ����locale
		_locale = locale;

		loadI18nBundle(WinksConstants.DEFAULT_I18N_MESSAGES_BUNDLE);
		loadI18nBundle(messageBundle);

		if (_messageTable != null && !_messageTable.isEmpty())
			return _messageTable;
		else
			return null;
	}

	/**
	 * ������ʻ�property.
	 * 
	 * @param messageBundle
	 *            property�ļ���·��(֧�־���·����ֻ�����ļ���).
	 * @return ��ȡproperty�ļ��Ƿ�ɹ�.
	 */
	private static boolean loadI18nBundle(String messageBundle) {
		// ���·��
		if (messageBundle != null && !messageBundle.startsWith("/")) {
			messageBundle = new StringBuffer(
					WinksConstants.DEFAULT_PROPERTY_RES_FOLDER).append(
					messageBundle).toString();
		}

		InputStream inputStream = null;
		// ���ڻ�ȡ��Դ��.
		Class clazz = _locale.getClass();
		try {
			if ((_locale != null) && (_locale.length() > 1)) {
				int lastIndex = messageBundle.lastIndexOf('.');
				String prefix = messageBundle.substring(0, lastIndex);
				String suffix = messageBundle.substring(lastIndex);

				// Nokia��Moto���ص�localeʹ��"_", ����ͳһ��"-"�滻Ϊ"_"
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
				// ���δ�ҵ���localeû����������Ĭ��locale
				inputStream = clazz.getResourceAsStream(messageBundle);
			}
			if (inputStream != null) {
				_messageTable = Properties.loadProperty(inputStream);
			}
		} catch (UTFDataFormatException e) {
			System.err
					.println("I18N Error : *.properties files need to be UTF-8 encoded");
		} catch (Exception e) {
			e.printStackTrace();
		}

		return _messageTable != null;
	}
}
