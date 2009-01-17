package cn.sda.enterprise;

import java.util.Calendar;
import java.util.TimeZone;
import java.util.Vector;

/**
 * 
 * @author Administrator
 */
public class SDATools {

	public static String getNowTime() {
		return internalGetNowTime();
	}

	private static String internalGetNowTime() {
		Calendar cl = Calendar.getInstance(TimeZone.getDefault());
		String Year = String.valueOf(cl.get(Calendar.YEAR));
		String Month = String.valueOf(cl.get(Calendar.MONTH) + 1);
		String Day = String.valueOf(cl.get(Calendar.DATE));
		String Hour = String.valueOf(cl.get(Calendar.HOUR_OF_DAY));
		String Minute = String.valueOf(cl.get(Calendar.MINUTE));
		String Secend = String.valueOf(cl.get(Calendar.SECOND));
		return Year + "-" + Month + "-" + Day + " " + Hour + ":" + Minute + ":"
				+ Secend;
	}

	public static String formatFloat(String formatString, double floatValue) {
		return internalFormatFloat(formatString, floatValue);
	}

	private static String internalFormatFloat(String formatString,
			double floatValue) {
		if (formatString.equals("")) {
			return Double.toString(floatValue);
		}
		// 根据格式对double内容进行转换
		int pos = 0;
		StringBuffer strbuffer = new StringBuffer();
		String dbValue = doubleToString(floatValue);
		String format = formatString;
		char ch;
		boolean findE = false;
		byte Etype = -1;
		int Epos = -1;
		// E值
		int eValue = 0;
		// 格式整数个数
		int intNum = 0;
		// 格式小数个数
		int decNum = 0;
		// double值的整数长度
		int doubleIntNum = 0;
		long DbInt = 0;
		// double的小数长度
		int doubledecNum = 0;
		long DBdecint = 0;
		// 查找小数点
		boolean findDot = false;
		int dotPos = -1;
		// 查找 ","
		boolean findDelim = false;
		int delimPos = -1;
		// 查找"."
		dotPos = format.indexOf(".");
		if (dotPos > -1) {
			findDot = true;
		} else {
			findDot = false;
		}
		// 查找","
		delimPos = format.indexOf(",");
		if (delimPos > -1) {
			findDelim = true;
		} else {
			findDelim = false;
		}
		// 判断是否存在E+-
		String upstring = format.toUpperCase();
		if ((upstring.indexOf("E+") > -1)) {
			findE = true;
			Etype = 0;
			Epos = upstring.indexOf("E+");
		}
		if ((upstring.indexOf("E-") > -1)) {
			findE = true;
			Etype = 1;
			Epos = upstring.indexOf("E-");
		}
		String eString = "";
		if (findE) {
			// 查找完整E段
			eString = format.substring(Epos, Epos + 2);
			Epos += 2;
			// 查找后面的0
			while (Epos < format.length()) {
				ch = format.charAt(Epos);
				if (ch == '0') {
					eString += ch;
				} else {
					break;
				}
				Epos++;
			}
		}
		// 记录E位置
		Vector fList = null;
		Vector eList = null;
		if (findE) {
			fList = new Vector();
			eList = new Vector();
		}
		// 格式的前后0#个数
		pos = 0;
		String foreString = findDot ? format.substring(0, dotPos) : format;
		if (findE) {
			strbuffer.setLength(0);
			strbuffer.append(foreString);
			// 去掉所有的e标识
			while (strbuffer.toString().indexOf(eString) > -1) {
				pos = strbuffer.toString().indexOf(eString);
				fList.addElement(String.valueOf(pos));
				strbuffer.delete(pos, pos + eString.length());
			}
			foreString = strbuffer.toString();
		}
		// 去掉所有的","
		strbuffer.setLength(0);
		strbuffer.append(foreString);
		while (strbuffer.toString().indexOf(",") > -1) {
			pos = strbuffer.toString().indexOf(",");
			strbuffer.delete(pos, pos + 1);
		}
		foreString = strbuffer.toString();
		// 计算0＃个数
		pos = 0;
		while (pos < foreString.length()) {
			ch = foreString.charAt(pos);
			if ((ch == '#') || (ch == '0')) {
				intNum++;
			}
			pos++;
		}
		pos = 0;
		String backString = findDot ? format.substring(dotPos + 1, format
				.length()) : "";
		if (findE) {
			strbuffer.setLength(0);
			strbuffer.append(backString);
			// 去掉所有的e标识
			while (strbuffer.toString().indexOf(eString) > -1) {
				pos = strbuffer.toString().indexOf(eString);
				eList.addElement(String.valueOf(pos));
				strbuffer.delete(pos, pos + eString.length());
			}
			backString = strbuffer.toString();
		}
		// 去掉所有的","
		strbuffer.setLength(0);
		strbuffer.append(backString);
		while (strbuffer.toString().indexOf(",") > -1) {
			pos = strbuffer.toString().indexOf(",");
			strbuffer.delete(pos, pos + 1);
		}
		backString = strbuffer.toString();
		pos = 0;
		while (pos < backString.length()) {
			ch = backString.charAt(pos);
			if ((ch == '#') || (ch == '0')) {
				decNum++;
			}
			pos++;
		}
		// double的整数部分长度
		pos = dbValue.indexOf(".");
		doubleIntNum = pos > -1 ? dbValue.substring(0, pos).length() : dbValue
				.length();
		doubledecNum = dbValue.length() - doubleIntNum - 1;
		// 存在E+,E-的处理(E+00,E-00连续的)
		if (findE) {
			// 处理double字符串
			int cint = intNum + decNum - dbValue.length() + 1;
			if (cint > 0) {
				while (cint > 0) {
					dbValue += "0";
					cint--;
				}
			} else if (cint < 0) {
				strbuffer.setLength(0);
				strbuffer.append(dbValue);
				pos = dbValue.indexOf(".");
				strbuffer.deleteCharAt(pos);
				dbValue = strbuffer.toString();
				if (Integer.parseInt(dbValue.charAt(intNum + decNum) + "") > 4) {
					dbValue = dbValue.substring(0, intNum + decNum);
					dbValue = doubleToString(Double.parseDouble(dbValue) + 1);
				} else {
					dbValue = dbValue.substring(0, intNum + decNum);
				}
			}
			// 拼凑格式化的结果
			pos = 0;
			strbuffer.setLength(0);
			// System.out.println(dbValue);
			// 整数部分
			int j = 0;
			while (pos < foreString.length()) {
				ch = foreString.charAt(pos);
				if ((ch == '0') || (ch == '#')) {
					strbuffer.append(dbValue.charAt(j));
					j++;
				} else {
					strbuffer.append(ch);
				}
				pos++;
			}
			// 处理","
			// System.out.println(strbuffer.toString());
			if (findDelim) {
				pos = foreString.length() - 1;
				int i = 0;
				while (pos > -1) {
					ch = foreString.charAt(pos);
					if ((ch == '0') || (ch == '#')) {
						i++;
					}
					if (i == 3) {
						if ((foreString.substring(0, pos).indexOf("0") > -1)
								|| (foreString.substring(0, pos).indexOf("#") > -1)) {
							strbuffer.insert(pos, ",");
							i = 0;
						} else {
							i = 0;
						}
					}
					pos--;
				}
			}
			// System.out.println(strbuffer.toString());
			// 小数点
			pos = 0;
			strbuffer.append(".");
			// 小数部分
			int k = 0;
			while (pos < backString.length()) {
				ch = backString.charAt(pos);
				if ((ch == '0') || (ch == '#')) {
					strbuffer.append(dbValue.charAt(intNum + k));
					k++;
				} else {
					strbuffer.append(ch);
				}
				pos++;
			}
			// 计算E部分
			eValue = doubleIntNum - intNum;
			String st = getfmZeroString(eString.length() - 2, eValue, false);
			if (Etype == 0) {
				st = eString.charAt(0) + (eValue > 0 ? "+" : "-") + st;
			} else {
				st = eString.charAt(0) + (eValue > 0 ? "" : "-") + st;
			}
			// 整数部分的E
			k = 0;
			for (int i = 0; i < fList.size(); i++) {
				pos = Integer.parseInt((String) fList.elementAt(i))
						+ (findDelim ? 1 : 0);
				if (pos >= strbuffer.length()) {
					strbuffer.append(st);
				} else {
					strbuffer.insert(pos + 1, st);
				}
				k += st.length();
			}
			// 小数部分的E
			// System.out.println(strbuffer.toString() + "/" + (String)
			// eList.elementAt(0) + "|" + foreString.length());
			for (int i = 0; i < eList.size(); i++) {
				pos = Integer.parseInt((String) eList.elementAt(i))
						+ foreString.length() + (findDelim ? 1 : 0) + k;
				if (pos >= strbuffer.length()) {
					strbuffer.append(st);
				} else {
					strbuffer.insert(pos + 1, st);
				}
			}
			return strbuffer.toString();
		} else {
			// 不存在E(0,#通用，主要查找",.")
			// 整体处理
			pos = dbValue.indexOf(".");
			int k = decNum - doubledecNum;
			if (k < 0) {
				ch = dbValue.charAt(pos + decNum + 1);
				dbValue = dbValue.substring(0, pos + decNum + 1);
				if (Integer.parseInt(ch + "") > 4) {
					// dbValue = String.valueOf(Double.parseDouble(dbValue) +
					// 1.0 / getExp(10, decNum));
					dbValue = doubleToString(Double.parseDouble(dbValue) + 1.0
							/ getExp(10, decNum));
				}
			}
			// 处理double整数部分
			DbInt = Long.parseLong(dbValue.substring(0, doubleIntNum));
			pos = intNum - doubleIntNum;
			String dbintstr = dbValue.substring(0, doubleIntNum);
			if (pos > 0) {
				dbintstr = getfmZeroString(intNum, DbInt, false);
			}
			pos = dbValue.indexOf(".");
			DBdecint = Long.parseLong(dbValue.substring(pos + 1, dbValue
					.length()));
			String dbdecstr = dbValue.substring(pos + 1);
			if (k > 0) {
				dbdecstr = getfmZeroString(decNum, DBdecint, true);
			}
			dbValue = dbintstr + "." + dbdecstr;
			// 拼凑格式化的结果
			pos = 0;
			strbuffer.setLength(0);
			// 整数部分
			int j = 0;
			while (pos < foreString.length()) {
				ch = foreString.charAt(pos);
				if ((ch == '0') || (ch == '#')) {
					strbuffer.append(dbintstr.charAt(j));
					j++;
				} else {
					strbuffer.append(ch);
				}
				pos++;
			}
			// 加入剩下的
			if (dbintstr.length() > j) {
				strbuffer.append(dbintstr.substring(j, dbintstr.length()));
			}
			// 处理","
			if (findDelim) {
				while (foreString.length() < strbuffer.length()) {
					foreString += "0";
				}
				pos = foreString.length() - 1;
				// System.out.println(pos);
				int i = 0;
				while (pos > -1) {
					ch = foreString.charAt(pos);
					if ((ch == '0') || (ch == '#')) {
						i++;
					}
					if (i == 3) {
						strbuffer.insert(pos, ",");
						i = 0;
					}
					pos--;
				}
			}
			// 小数点
			pos = 0;
			strbuffer.append(".");
			// 小数部分
			k = 0;
			while (pos < backString.length()) {
				ch = backString.charAt(pos);
				if ((ch == '0') || (ch == '#')) {
					strbuffer.append(dbValue.charAt(dbintstr.length() + 1 + k));
					k++;
				} else {
					strbuffer.append(ch);
				}
				pos++;
			}
			return strbuffer.toString();
		}
	}

	private static String getfmZeroString(int len, long value, boolean end) {
		String result = "";
		if (value < 0) {
			value = -value;
		}
		int pos = 0;
		if (end) {
			while (pos < len - String.valueOf(value).length()) {
				result += "0";
				pos++;
			}
			result = String.valueOf(value) + result;
		} else {
			while (pos < len - String.valueOf(value).length()) {
				result += "0";
				pos++;
			}
			result += String.valueOf(value);
		}
		return result;
	}

	private static double getExp(int x, int y) {
		double d = x;
		for (int i = 1; i < y; i++) {
			d = d * x;
		}
		return d;
	}

	private static String doubleToString(double dbvalue) {
		int pos = 0;
		StringBuffer strbuffer = new StringBuffer();
		String dbValue = Double.toString(dbvalue);
		pos = dbValue.indexOf("E");
		if (pos > 0) {
			int offset = Integer.parseInt(dbValue.substring(pos + 1, dbValue
					.length()));
			dbValue = dbValue.substring(0, pos);
			strbuffer.setLength(0);
			strbuffer.append(dbValue);
			pos = dbValue.indexOf(".");
			strbuffer.deleteCharAt(pos);
			strbuffer.insert(pos + offset, ".");
			dbValue = strbuffer.toString();
		}
		strbuffer = null;
		return dbValue;
	}
}
