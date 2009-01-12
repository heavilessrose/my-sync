package utils;

import java.util.Vector;

import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;

public class FontUtil {

	// ˮƽ�����־
	public final static int ALIGN_LEFT = 0;
	public final static int ALIGN_CENTER = 1;
	public final static int ALIGN_RIGHT = 2;

	// ��������־
	public final static int ALIGN_TOP = 3;
	public final static int ALIGN_MIDDLE = 4;
	public final static int ALIGN_BOTTOM = 5;

	/**
	 * ����һ��String������λ��(����y)
	 *
	 * @param font
	 * @param string Ҫ������ַ���
	 * @param box_y    top left���y����
	 * @param box_height The vertical height of the bounding box
	 * @param align �����־
	 * @return String������y����
	 */
	public static int getVerticalPosition(Font font, String string, int box_y, int box_height, int align) {
		if (align == ALIGN_MIDDLE)
			return ((box_y + (box_height / 2)) - font.getHeight() / 2);
		else if (align == ALIGN_BOTTOM)
			return (box_y + box_height - font.getHeight());
		else
			return box_y;
	}

	/**
	 * ����һ��String��ˮƽλ��(����x)
	 *
	 * @param font
	 * @param string Ҫ������ַ���
	 * @param box_x   top left���x����
	 * @param box_width 
	 * @param align �����־
	 * @return Ҫ����String��x
	 */
	public static int getHorizontalPosition(Font font, String string, int box_x, int box_width, int align) {
		if (align == ALIGN_CENTER)
			return (((box_x + (box_width - box_x)) / 2) - font.stringWidth(string) / 2);
		else if (align == ALIGN_RIGHT)
			return ((box_x + box_width) - font.stringWidth(string));
		else
			return box_x;
	}

	/**
	 * ���ַ�����������ȷָ�Ϊ�ַ�������
	 *
	 * @param font
	 * @param inputStr
	 * @param box_width
	 * @param breakOnSpaces ����ո�(�ɱ�֤Ӣ�ĵ��ʲ����ж�)
	 * @return �и����ַ�������
	 */
	public static String[] splitByBox(Font font, String inputStr, int box_width, boolean breakOnSpaces) {
		if (font.stringWidth(inputStr) <= box_width)
			return new String[] { inputStr };

		Vector lines = new Vector();

		int lastSpacePos = 0; // ���һ���ո�λ��
		int lineLen = 0; // ÿ�е�ǰ����
		int offset = 0; // ��ʼ��ȡ��λ��

		char[] chars = inputStr.toCharArray();

		for (int i = 0; i < chars.length; i++) {
			if (breakOnSpaces && chars[i] == ' ')
				lastSpacePos = i;

			lineLen += font.charWidth(chars[i]);

			if (lineLen > box_width || i == (chars.length - 1)) {
				if (lineLen > box_width)
					i--;

				int count = 0;

				if (!breakOnSpaces || i == (chars.length - 1) || lastSpacePos == 0)
					count = (i - offset);
				else {
					count = (lastSpacePos - offset);

					i = lastSpacePos;
				}

				lines.addElement(new String(chars, offset, (count + 1)));

				lastSpacePos = 0;
				lineLen = 0;
				offset = ((offset + count) + 1);
			}
		}

		String[] result = new String[lines.size()];
		CollectionUtil.vectorToArray(lines, result);
		return result;
	}

	/** 
	 * ����־�ָ��ַ���
	 * @param source 
	 * @param delim �ָ���
	 * @return
	 */
	public static String[] splitStringByDelim(String source, String delim) {
		if (source == null || delim == null)
			return null;

		Vector output = new Vector();
		int lastIndex = 0;
		int temp;
		String buf;

		while (true) {
			temp = source.indexOf(delim, lastIndex);

			if (temp == -1) {
				// we're done no more delims, check if we have a last token
				if (lastIndex == 0) {
					// we didn't have any delims, return the whole string
					output.addElement(source);
					break;
				} else {
					buf = source.substring(lastIndex, source.length());
					output.addElement(buf);
					break;
				}

			} else {
				buf = source.substring(lastIndex, temp);
				output.addElement(buf);
				// account for delimeters longer than 1 char
				lastIndex = temp + delim.length();
			}
		}

		String[] realOutput = new String[output.size()];
		for (int i = 0; i < realOutput.length; i++)
			realOutput[i] = (String) output.elementAt(i);

		return realOutput;
	}

	/**
	 * ����ʹ��ָ��������һ�����������������ʾ������
	 *
	 * @param font
	 * @param box_height
	 * @param space
	 * @return ����
	 */
	public static int getVerticalCount(Font font, int box_height, int space) {
		int baseCount = (box_height / (font.getHeight() + space));

		if (((baseCount * (font.getHeight() + space)) + font.getHeight()) < box_height)
			return ++baseCount;
		else
			return baseCount;
	}

	public static void drawStringInBox(Graphics g, Font font, String str, int box_x, int box_y, int start_y, int box_width,
			int box_height, int space, int align) {
		//		align = 0 | 3; // top left
		String[] lines = splitByBox(font, str, box_width - space * 2, false);

		g.setClip(box_x, box_y, box_width, box_height);
		drawBox(g, str, box_x, box_y, box_width, box_height);
		g.setColor(0, 0, 0);
		// �������������ʾ
		if (lines.length > getVerticalCount(font, box_height, space)) {
			for (int i = 0; i < lines.length; i++)
				g.drawString(lines[i], box_x + space, start_y + space + (font.getHeight() + space) * i, align);
			
		} else {
			for (int i = 0; i < lines.length; i++)
				g.drawString(lines[i], box_x + space, start_y + space + (font.getHeight() + space) * i, align);
		}
	}

	private static void drawBox(Graphics g, String str, int box_x, int box_y, int box_width, int box_height) {
		g.setColor(125, 125, 125);
		g.fillRect(box_x, box_y, box_width, box_height);
	}

}
