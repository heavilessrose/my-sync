package winkCC.core.property_i18n;

import java.io.InputStream;
import java.io.UTFDataFormatException;
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
	 * ��ʼ�����ʻ�property.
	 * 
	 * @param messageBundle
	 *            property�ļ�.
	 * @param locale
	 * @return
	 */
	public static boolean initI18nProperty(String messageBundle, String locale) {

		_messageTable = new Hashtable();

		// ����locale
		_locale = locale;

		loadI18nBundle(WinksConstants.DEFAULT_I18N_MESSAGES_BUNDLE);
		loadI18nBundle(messageBundle);

		return _messageTable != null;
	}

	/**
	 * ������ʻ�property.
	 * 
	 * @param messageBundle
	 *            property�ļ���·��(֧�־���·����ֻ�����ļ���).
	 * @return ��ȡproperty�ļ��Ƿ�ɹ�.
	 */
	public static boolean loadI18nBundle(String messageBundle) {
		// ���·��
		if (messageBundle != null && !messageBundle.startsWith("/")) {
			messageBundle = new StringBuffer(
					WinksConstants.DEFAULT_PROPERTY_RES_FOLDER).append(
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
				// ���δ�ҵ���localeû����������Ĭ��locale
				inputStream = clazz.getResourceAsStream(messageBundle);
			}
			if (inputStream != null) {
				// load messages to messageTable hashtable
				_messageTable = Properties.loadProperty(inputStream);
			}
		} catch (UTFDataFormatException e) {
			System.err
					.println("Property Error : *.properties files need to be UTF-8 encoded");
		} catch (Exception e) {
			e.printStackTrace();
		}

		return _messageTable != null;
	}
}
