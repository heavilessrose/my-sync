/*
 * Copyright (C) 2006 Astrient Labs, LLC Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance 
 * with the License. You may obtain a copy of the License at 
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the 
 * License for the specific language governing permissions and limitations under
 * the License.
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.text;

import java.util.Calendar;
import java.util.Date;
import java.util.Vector;

public class Strings {
	public static final double KIB = 2L << 9;
	public static final double MIB = 2L << 19;
	public static final double GIB = 2L << 29;

	public static String getText(String text, int len, String suffix) {
		if (text != null) {
			if (text.length() > len) {
				return text.substring(0, len).trim() + suffix;
			}
		}

		return text;
	}

	public static String[] split(String s, int i) {
		int len = s.length();
		String[] sa = new String[len / i + (((len % i) > 0) ? 1 : 0)];
		for (int j = 0; j < (sa.length - 1)
				|| ((sa[j] = s.substring(j * i)) == null); sa[j] = s.substring(
				j * i, j++ * i + i))
			;
		return sa;
	}

	public static boolean isNull(String s) {
		return (s == null || s.trim().length() == 0);
	}

	public static Vector split(String s, char c) {
		Vector parts = new Vector();
		if (s != null) {
			int lastfound = 0;
			int pos = 0;
			while ((lastfound = s.indexOf(c, pos)) != -1) {
				parts.addElement(s.substring(pos, lastfound));
				pos = lastfound + 1;
			}
			if (pos < s.length())
				parts.addElement(s.substring(pos));
		}

		return parts;
	}

	public static String longToDateString(long date) {
		Calendar c = Calendar.getInstance();
		c.setTime(new Date(date));

		StringBuffer buffer = new StringBuffer();
		buffer.append(c.get(Calendar.MONTH) + 1).append("/").append(
				c.get(Calendar.DAY_OF_MONTH)).append("/").append(
				c.get(Calendar.YEAR)).append(" ").append(
				c.get(Calendar.HOUR_OF_DAY)).append(":");

		int min = c.get(Calendar.MINUTE);
		if (min > 10) {
			buffer.append(min);
		} else {
			buffer.append("0").append(min);
		}

		return buffer.toString();
	}

	public static String getSizeString(long number) {
		String suffix;
		double sz;

		if (number > GIB) {
			// gb
			sz = (number / GIB);
			suffix = "GB";
		} else if (number > MIB) {
			// mb
			sz = (number / MIB);
			suffix = "MB";
		} else if (number > KIB) {
			// kb
			sz = number / KIB;
			suffix = "KB";
		} else {
			sz = number;
			suffix = "b";
		}

		return (Math.ceil(sz * 100.00) / 100.0) + suffix;
	}

	public static String URLEncode(String url) {
		StringBuffer buffer = new StringBuffer();

		for (int i = 0; i < url.length(); i++) {
			switch (url.charAt(i)) {
			case ' ':
				buffer.append("%20");
				break;
			case '+':
				buffer.append("%2b");
				break;
			case '\'':
				buffer.append("%27");
				break;
			case '<':
				buffer.append("%3c");
				break;
			case '>':
				buffer.append("%3e");
				break;
			case '#':
				buffer.append("%23");
				break;
			case '%':
				buffer.append("%25");
				break;
			case '{':
				buffer.append("%7b");
				break;
			case '}':
				buffer.append("%7d");
				break;
			case '\\':
				buffer.append("%5c");
				break;
			case '^':
				buffer.append("%5e");
				break;
			case '~':
				buffer.append("%73");
				break;
			case '[':
				buffer.append("%5b");
				break;
			case ']':
				buffer.append("%5d");
				break;
			case '-':
				buffer.append("%2D");
				break;
			case '/':
				buffer.append("%2F");
				break;
			case ':':
				buffer.append("%3A");
				break;
			case '=':
				buffer.append("%3D");
				break;
			case '?':
				buffer.append("%3F");
				break;
			case '\r':
				buffer.append("%0D");
				break;
			case '\n':
				buffer.append("%0A");
				break;
			default:
				buffer.append(url.charAt(i));
				break;
			}
		}
		return buffer.toString();
	}
}
