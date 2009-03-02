package com.winksi.j2me.cc.tools;

import java.util.Vector;
import javax.microedition.lcdui.*;

import com.winksi.j2me.cc.common.*;
import java.io.InputStream;
import java.util.Hashtable;

public final class StringTools {

	private StringTools() {
	}

	public static Vector getItem(String content, String split) {
		Vector vector = new Vector();
		int i = 0;
		while (content.indexOf(split) != -1) {
			vector.addElement(content.substring(0, content.indexOf(split)));
			i = content.indexOf(split) + split.length();
			content = content.substring(i);
		}
		return vector;
	}

	public static Hashtable getItemHt(String content, String split, String split2) {
		Hashtable ht = new Hashtable();
		Vector v = getItem(content, split);
		for (int i = 0; i < v.size(); i++) {
			String temp = v.elementAt(i).toString();
			String key = temp.substring(0, temp.indexOf(split2));
			String volue = temp.substring(temp.indexOf(split2) + 1);
			ht.put(key, volue);
		}
		return ht;
	}

	/**
	 * �ָ��ַ�
	 * 
	 * @param strSource
	 *            Դ�ַ�
	 * @param font
	 *            ����
	 * @param width
	 *            �ָ��ÿ�е�����ȣ�����ֵ��
	 * @param strSplit
	 *            ����Ӣ�ĵȲ��ܴ�һ����м任�����ʱ����Ӧ�Ŀ��Ի��еķ�š�����?.,?!��
	 * @return Vector ʸװ���˷ָ�õ��ַ�?
	 */
	public static Vector getSubsection(String strSource, Font font, int width, String strSplit) {
		Vector vector = new Vector();
		String temp = strSource;
		int i, j;
		int LastLength = 1;
		int step = 0;
		try {
			while (!temp.equals("")) {
				i = temp.indexOf("\n");
				if (i > 0) {
					if (font.stringWidth(temp.substring(0, i - 1)) >= width) {
						i = -1;
					}
				}
				if (i == -1) {
					if (LastLength > temp.length()) {
						i = temp.length();
					} else {
						i = LastLength;
						step = font.stringWidth(temp.substring(0, i)) > width ? -1 : 1;
						// System.out.println("s="+temp.substring(0, i));
						// System.out.println("swidth="+font.stringWidth(temp.substring(0,
						// i)));
						// System.out.println("width="+width);
						// System.out.println("step="+step);
						if (i < temp.length()) {
							while (!(font.stringWidth(temp.substring(0, i)) <= width && font.stringWidth(temp
									.substring(0, i + 1)) > width)) {
								i = i + step;
								if (i == temp.length()) {
									break;
								}
								// System.out.println("i="+i);
							}
						}
					}
					if (!strSplit.equals("")) {
						j = i;
						if (i < temp.length()) {
							while (strSplit.indexOf(temp.substring(i - 1, i)) == -1) {
								// System.out.println("-1=");
								i--;
								if (i == 0) {
									i = j;
									break;
								}
								// System.out.println("i2="+i);
							}
						}
					}
				}
				LastLength = i;
				vector.addElement(temp.substring(0, i));
				if (i == temp.length()) {
					temp = "";
				} else {
					temp = temp.substring(i);
					if (temp.substring(0, 1).equals("\n")) {
						temp = temp.substring(1);
					}
				}
			}
		} catch (Exception e) {
		}
		return vector;
	}

	public static String urlEncoding(final String str) {
		if (str == null || str.length() == 0) {
			return str;
		}
		String _tmp = str;
		_tmp = replaceAll(_tmp, " ", "%20");
		_tmp = replaceAll(_tmp, "#", "%23");
		_tmp = replaceAll(_tmp, "%", "%25");
		_tmp = replaceAll(_tmp, "&", "%26");
		_tmp = replaceAll(_tmp, "+", "%2B");
		_tmp = replaceAll(_tmp, "\\", "%2F");
		_tmp = replaceAll(_tmp, "=", "%3D");
		_tmp = replaceAll(_tmp, "?", "%3F");
		return _tmp;
	}

	/**
	 * ���ַ���ָ����ʽ�ָ�
	 *
	 * @param source
	 *            String ���ָ���ַ�?
	 * @param separate
	 *            String �ָ��?
	 * @return String[]
	 */
	public static String[] split(String source, String separate) {
		// WaitWindow.errStr = "split...1...";
		Vector tmpVector = new Vector();
		int equalLen = 0, startIndex = 0;
		for (int i = 0, n = source.length(); i < n; i++) {
			if (source.charAt(i) == separate.charAt(equalLen)) {
				equalLen++;
				if (equalLen == separate.length()) {
					// ģʽƥ��ɹ�?
					tmpVector.addElement(source.substring(startIndex, i - separate.length() + 1));
					startIndex = i + 1;
					equalLen = 0;
				}
			} else {
				equalLen = 0;
			}
		}
		// WaitWindow.errStr = "split...2...";
		tmpVector.addElement(source.substring(startIndex));
		String[] arr = new String[tmpVector.size()];
		for (int i = 0, n = arr.length; i < n; i++) {
			arr[i] = (String) tmpVector.elementAt(i);
		}
		// WaitWindow.errStr = "split...3...";
		return arr;
	}

	/**
	 * �滻Դ�ַ��е�ĳ���ַ�,��indexλ�ÿ�ʼ�滻һ��
	 *
	 * @param source
	 *            String
	 * @param searchStr
	 *            String
	 * @param replaceStr
	 *            String
	 * @param index
	 *            int
	 * @return String
	 */
	public static String replace(final String source, final String searchStr, final String replaceStr, final int index) {
		if (source == null || source.length() == 0) {
			return source;
		}
		int i = source.indexOf(searchStr, index);

		if (i != -1) {
			StringBuffer sb = new StringBuffer();
			sb.append(source.substring(0, i));
			sb.append(replaceStr);
			sb.append(source.substring(i + searchStr.length()));

			return sb.toString();
		} else {
			return source;
		}
	}

	/**
	 * ���Դ�������, ִ��ʱ��40ms long startTime = System.currentTimeMillis();
	 * System.out.println(StringTools.replaceAll("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890",
	 * "12", "ABCD")); for(int i=0; i<1000; i++) {
	 * StringTools.replaceAll("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890",
	 * "12", "ABCD"); } System.out.println("ִ��ʱ��:
	 * "+(System.currentTimeMillis()-startTime)+"ms");
	 */
	private static String replaceAll_backup(final String source, final String searchStr, final String replaceStr) {
		if (source == null || source.length() == 0 || (source.indexOf(searchStr) == -1)) {
			return source;
		}
		StringBuffer sb = new StringBuffer();
		int startIndex = 0, i;
		while ((i = source.indexOf(searchStr, startIndex)) != -1) {
			sb.append(source.substring(startIndex, i));
			sb.append(replaceStr);
			startIndex = i + searchStr.length();
		}
		sb.append(source.substring(startIndex));
		return sb.toString();
	}

	/**
	 * ���Դ�������, ִ��ʱ��30ms long startTime = System.currentTimeMillis();
	 * System.out.println(StringTools.replaceAll("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890",
	 * "12", "ABCD")); for(int i=0; i<1000; i++) {
	 * StringTools.replaceAll("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890",
	 * "12", "ABCD"); } System.out.println("ִ��ʱ��:
	 * "+(System.currentTimeMillis()-startTime)+"ms");
	 */
	public static String replaceAll(final String source, final String searchStr, final String replaceStr) {
		if (source == null || source.length() == 0) {
			return source;
		}

		StringBuffer sb = new StringBuffer();
		char[] _arrChar = source.toCharArray();
		char[] _arrSearch = searchStr.toCharArray();
		int equalsNum = 0;
		for (int i = 0, n = _arrChar.length; i < n; i++) {
			if (_arrChar[i] != _arrSearch[equalsNum]) {
				sb.append(_arrChar[i]);
				if (equalsNum > 0) {
					for (int j = equalsNum; j > 0; j--) {
						sb.append(_arrChar[i - j]);
					}
					equalsNum = 0;
				}
			} else {
				equalsNum++;
				if (equalsNum == _arrSearch.length) {
					sb.append(replaceStr);
					equalsNum = 0;
				}
			}
		}
		if (equalsNum > 0) {
			for (int j = equalsNum; j > 0; j--) {
				sb.append(_arrChar[_arrChar.length - j]);
			}
		}
		return sb.toString();
	}

	public static String[] getStrings(String textString, Font textFont) {
		int lineLength = CPProperty.SCREENWIDTH - 26 - 15;
		Vector strings = new Vector();
		int curWordIndex = 0;
		int curLineLength = 0;
		int textEndIndex = textString.length();
		String stringToDraw = null, prevString;

		while (true) {
			int newWordIndex = curWordIndex;
			int newLineIndex = textString.indexOf('\n', newWordIndex);
			int prevWordIndex;
			while (true) {
				prevWordIndex = newWordIndex;
				prevString = stringToDraw;

				// get the next word index
				newWordIndex = getWordIndex(textString, newWordIndex);

				if (newLineIndex > -1 && newLineIndex < newWordIndex) {
					newWordIndex = newLineIndex;
				}

				stringToDraw = textString.substring(curWordIndex, newWordIndex).trim();

				if (textFont.stringWidth(stringToDraw) > lineLength) {
					if (prevWordIndex == curWordIndex) {
						// first word of the line is longer than the line length
						// make the text wrap here
						for (int i = stringToDraw.length() - 1; i > 0; i--) {
							String newString = stringToDraw.substring(0, i);
							if (textFont.stringWidth(newString) <= lineLength) {
								// the newString fits the line length
								newWordIndex = prevWordIndex + i;
								stringToDraw = newString;
								break;
							}
						}
					} else {
						newWordIndex = prevWordIndex;
						stringToDraw = prevString;
					}
					break;
				} else if (newWordIndex == newLineIndex) {
					newWordIndex++;
					break;
				} else if (newWordIndex >= textEndIndex) {
					break;
				}
			}
			strings.addElement(stringToDraw);

			curWordIndex = newWordIndex;
			if (curWordIndex >= textEndIndex) {
				break;
			}
		}

		String[] stringsToDraw = new String[strings.size()];
		strings.copyInto(stringsToDraw);
		return stringsToDraw;
	}

	private static int getWordIndex(String srcString, int fromIndex) {
		char curChar = srcString.charAt(fromIndex);
		// if the first character is a word, return it
		if (charIsWord(curChar) || charIsPunctuation(curChar)) {
			return fromIndex + 1;
		}

		int toIndex = 0;
		// get rid of the spaces in the front
		while (true) {
			int spaceIndex = srcString.indexOf(' ', fromIndex);
			if (spaceIndex == 0) {
				fromIndex++;
			} else {
				toIndex = spaceIndex;
				break;
			}
		}

		if (toIndex == -1) {
			toIndex = srcString.length();
		} else {
			toIndex++;
		}

		for (int i = fromIndex + 1; i < toIndex; i++) {
			curChar = srcString.charAt(i);
			if (charIsPunctuation(curChar)) {
				return i + 1;
			} else if (charIsWord(curChar)) {
				return i;
			}
		}

		// else a word is right up to the space character
		return toIndex;
	}

	private static boolean charIsPunctuation(int theChar) {
		if (theChar == '.') { // so an URL would wrap at the "." places
			return true;
		}
		return false;
	}

	private static boolean charIsWord(int theChar) {
		if ( // theChar == '.' || // so an URL would wrap at the "." places
		(theChar >= 0x002E80 && theChar < 0x00AC00) || (theChar >= 0x00F900 && theChar < 0x00FB00)
				|| (theChar >= 0x00FF00 && theChar < 0x00FFE0)) {
			// this char is treated as a word
			return true;
		}
		return false;
	}

	public static int toInteger(byte[] b) {
		int intValue = 0;
		for (int i = 0; i < b.length; i++) {
			intValue += (b[i] & 0xFF) << (8 * (3 - i));
		}
		return intValue;
	}

	public static int bytes2int(byte b[]) {
		return b[3] & 0xff | (b[2] & 0xff) << 8 | (b[1] & 0xff) << 16 | (b[0] & 0xff) << 24;
	}

	public static short byte2short(byte b[]) {
		return (short) (b[1] & 0xff | (b[0] & 0xff) << 8);
	}

	public static long byte2long(byte b[]) {
		return (long) b[7] & (long) 255 | ((long) b[6] & (long) 255) << 8 | ((long) b[5] & (long) 255) << 16
				| ((long) b[4] & (long) 255) << 24 | ((long) b[3] & (long) 255) << 32
				| ((long) b[2] & (long) 255) << 40 | ((long) b[1] & (long) 255) << 48 | (long) b[0] << 56;
	}

	public static long byte2long(byte b[], int offset) {
		return (long) b[offset + 7] & (long) 255 | ((long) b[offset + 6] & (long) 255) << 8
				| ((long) b[offset + 5] & (long) 255) << 16 | ((long) b[offset + 4] & (long) 255) << 24
				| ((long) b[offset + 3] & (long) 255) << 32 | ((long) b[offset + 2] & (long) 255) << 40
				| ((long) b[offset + 1] & (long) 255) << 48 | (long) b[offset] << 56;
	}

	public static byte[] int2ByteArr(int i) {
		byte[] b = new byte[2];
		b[0] = (byte) ((i >> 8) & 0xff);
		b[1] = (byte) ((i) & 0xff);
		return b;
	}

	/** 
	 * 长度为2的字节数组转换为int
	 * 
	 * @param byte2Arr 长度为2的字节数组
	 * @return int
	 */
	public static int byteArr2Int(byte[] byte2Arr) {
		if (byte2Arr == null || byte2Arr.length != 2) {
			return -1;
		}
		return (((byte2Arr[0] & 0xff) << 8) | (byte2Arr[1] & 0xff));
	}

	public static byte[] int2byte(int n) {
		byte b[] = new byte[4];
		b[0] = (byte) (n >> 24);
		b[1] = (byte) (n >> 16);
		b[2] = (byte) (n >> 8);
		b[3] = (byte) n;
		return b;
	}

	public static void int2byte(int n, byte buf[], int offset) {
		buf[offset] = (byte) (n >> 24);
		buf[offset + 1] = (byte) (n >> 16);
		buf[offset + 2] = (byte) (n >> 8);
		buf[offset + 3] = (byte) n;
	}

	public static byte[] short2byte(short n) {
		byte b[] = new byte[2];
		b[0] = (byte) (n >> 8);
		b[1] = (byte) n;
		return b;
	}

	public static void short2byte(short n, byte buf[], int offset) {
		buf[offset] = (byte) (n >> 8);
		buf[offset + 1] = (byte) n;
	}

	public static byte[] long2byte(long n) {
		byte b[] = new byte[8];
		b[0] = (byte) (int) (n >> 56);
		b[1] = (byte) (int) (n >> 48);
		b[2] = (byte) (int) (n >> 40);
		b[3] = (byte) (int) (n >> 32);
		b[4] = (byte) (int) (n >> 24);
		b[5] = (byte) (int) (n >> 16);
		b[6] = (byte) (int) (n >> 8);
		b[7] = (byte) (int) n;
		return b;
	}

	public static void long2byte(long n, byte buf[], int offset) {
		buf[offset] = (byte) (int) (n >> 56);
		buf[offset + 1] = (byte) (int) (n >> 48);
		buf[offset + 2] = (byte) (int) (n >> 40);
		buf[offset + 3] = (byte) (int) (n >> 32);
		buf[offset + 4] = (byte) (int) (n >> 24);
		buf[offset + 5] = (byte) (int) (n >> 16);
		buf[offset + 6] = (byte) (int) (n >> 8);
		buf[offset + 7] = (byte) (int) n;
	}

	/** 
	 * 两个数组合并, b1+b2
	 * @param b1
	 * @param b2
	 * @return 合并后的数组
	 */
	public static byte[] linkbytes(byte[] b1, byte[] b2) {
		byte[] b = new byte[b1.length + b2.length];
		System.arraycopy(b1, 0, b, 0, b1.length);
		System.arraycopy(b2, 0, b, b1.length, b2.length);
		return b;
	}

	/** 
	 * 将三个字节数组合并
	 * @param b1
	 * @param b2
	 * @param b3
	 * @return 合并后的数组
	 */
	public static byte[] linkbytes(byte[] b1, byte[] b2, byte[] b3) {
		byte[] b = new byte[b1.length + b2.length + b3.length];
		System.arraycopy(b1, 0, b, 0, b1.length);
		System.arraycopy(b2, 0, b, b1.length, b2.length);
		System.arraycopy(b3, 0, b, b1.length + b2.length, b3.length);
		return b;
	}

	public static byte[] replacebytes(byte[] b, byte[] b1, int start) {
		System.arraycopy(b1, 0, b, start, b1.length);
		return b;
	}

	/** 去掉数据开头的len个字节 */
	public static byte[] removeDataPart(byte[] data, int len) throws Exception {
		byte[] b = new byte[data.length - len];
		System.arraycopy(data, len, b, 0, b.length);
		return b;
	}

	public static byte[] getBytesFromBytes(int maxsize, int total, int index, byte[] data) {
		if (maxsize < 1 || total < 1 || index < 1 || data.length < 1) {
			return null;
		}
		byte[] b = null;
		int start = 0;

		if (index < total) {
			start = (index - 1) * maxsize;
			b = new byte[maxsize];
		} else {
			start = (index - 1) * maxsize;
			b = new byte[data.length - (index - 1) * maxsize];
		}

		//        if (data.length % total == 0) {
		//            start=(data.length/(total))*(index-1);
		//            b = new byte[data.length / total];
		//        } else {
		//            start=(data.length/(total-1))*(index-1);
		//            if (index != total) {
		//                b = new byte[data.length / (total-1)];
		//            } else {
		//                b = new byte[data.length - (data.length / (total-1)) * (total - 1)];
		//            }
		//        }

		System.arraycopy(data, start, b, 0, b.length);
		return b;
	}

	private static final char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	public static byte[] hexStr2Bytes(String hexstr) {
		if (hexstr.length() % 2 != 0) {
			return null;
		}

		byte[] hexdata = hexstr.getBytes();
		byte[] result = new byte[hexdata.length / 2];

		for (int i = 0; i < hexstr.length(); i++) {
			char b = hexstr.charAt(i);
			switch (b) {
			case '0':
				hexdata[i] = 0;
				break;
			case '1':
				hexdata[i] = 1;
				break;
			case '2':
				hexdata[i] = 2;
				break;
			case '3':
				hexdata[i] = 3;
				break;
			case '4':
				hexdata[i] = 4;
				break;
			case '5':
				hexdata[i] = 5;
				break;
			case '6':
				hexdata[i] = 6;
				break;
			case '7':
				hexdata[i] = 7;
				break;
			case '8':
				hexdata[i] = 8;
				break;
			case '9':
				hexdata[i] = 9;
				break;
			case 'A':
				hexdata[i] = 10;
				break;
			case 'B':
				hexdata[i] = 11;
				break;
			case 'C':
				hexdata[i] = 12;
				break;
			case 'D':
				hexdata[i] = 13;
				break;
			case 'E':
				hexdata[i] = 14;
				break;
			case 'F':
				hexdata[i] = 15;
				break;
			}
		}

		for (int i = 0; i < hexdata.length; i += 2) {
			hexdata[i] = (byte) ((hexdata[i] << 4) & 0xf);
			hexdata[i + 1] = (byte) (hexdata[i + 1] & 0xf);
			result[i / 2] = (byte) (hexdata[i] | hexdata[i + 1]);
		}

		return result;
	}

	public static String getHex(byte buf[], int len) {

		int i, j = 0, k;
		String s, rets = null;
		byte[] sepchar = { 0x2E }, tmp = null;

		if (buf == null || len <= 0) {
			return null;
		}

		tmp = new byte[400];
		for (k = 0; k < 400; k++) {
			tmp[k] = 0x20;
		}
		k = 60;

		rets = "\n";

		for (i = 0; i < len; i++) {

			s = Integer.toHexString((buf[i] & 0xff));
			s = (s.getBytes().length < 2) ? "0" + s : s;
			System.arraycopy(s.getBytes(), 0, tmp, j, 2);
			j = (i % 8 == 7) ? (j + 5) : (j + 3);
			if ((buf[i] & 0xff) >= 32 && (buf[i] & 0xff) <= 128) {
				System.arraycopy(buf, i, tmp, k, 1);
			} else {
				System.arraycopy(sepchar, 0, tmp, k, 1);
			}
			k = (i % 8 == 7) ? (k + 4) : (k + 1);
			if (i % 16 == 15) {
				j = 0;
				rets += (new String(tmp).trim()) + "\n";
				for (k = 0; k < 400; k++) {
					tmp[k] = 0x20;
				}
				k = 60;
			}
		}
		if (len % 16 != 0) {
			rets += (new String(tmp).trim()) + "\n";
		}

		return rets;
	}
}
