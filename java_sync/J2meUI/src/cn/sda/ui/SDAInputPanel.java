package cn.sda.ui;

import cn.sda.event.PointerEvent;
import cn.sda.pinyin.InputInterface;
import cn.sda.pinyin.Spell2Chars;
import java.util.Hashtable;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Font;

/**
 * 
 * @author Administrator
 */
public class SDAInputPanel extends SDABaseControl {

	// 类型
	public static final int imNone = 0;
	public static final int imUpperCase = 1;
	public static final int imLowerCase = 2;
	public static final int imDigit = 3;
	public static final int imPoint = 4;
	public static final int imSymbol = 5;
	public static final int imPinYin = 6;
	// 输入法类型
	protected int ImeType = imPinYin;
	// 颜色
	private int borderColor = SDAConsts.clBlack;
	// 选中颜色
	private int selectedBackColor = SDAConsts.clBlack;
	private int selectedForeColor = SDAConsts.clWhite;
	// 当前行位置
	private int curHeight = 0;
	// 是否打开全角
	private boolean SBCCase = false;
	// 输入显示
	protected inputCharsPanel charsPanel = null;
	// 点的位置()
	private int charXPos = -1;
	private int charYPos = -1;
	// 当前被选中的Char内容(可见)
	protected char selectedChar = 0;
	// 功能键名字
	// private String funcKeyName="";
	// graphic
	private Graphics gg = null;
	// 拼音处理
	// 输入的字母（包括拼音，非触摸屏下的字母(字母和数字靠输入法类型区分)）
	protected StringBuffer inputChars;
	// 选择的内容（包括拼音或者非触摸屏下的字母等）
	private StringBuffer selectedChars;
	// 对应一个拼音组合的所有汉字
	protected char[] hanziChars;
	// 选择的汉字index
	protected int selectedHanziIndex = 0;
	// 查询到的汉字数
	private int hanziCount = 0;
	// 大写字母
	private char[] UpperCaseChars = null;
	private char[] SBCUpperCaseChars = null;
	// 小写字母
	private char[] LowerCaseChars = null;
	private char[] SBCLowerCaseChars = null;
	// 数字
	private char[] DigitChars = null;
	private char[] SBCDigitChars = null;
	// 标点
	private char[] PointChars = null;
	private char[] SBCPointChars = null;
	// 符号
	private char[] SymbolChars = null;
	private char[] SBCSymbolChars = null;
	// 对于一键多字母的切换处理
	private Hashtable hashLowerKeyChars = null;
	private Hashtable hashUpperKeyChars = null;
	// 获取的一键多字母的所有字母组合
	protected char[] numChars = null;
	// 这些字母的选择的index
	private int charIndex = 0;

	// 输入内容显示和选择框
	protected class inputCharsPanel extends SDABaseControl {

		// 光标在屏幕中的绝对位置
		protected int oraX = 0, oraY = 0;
		private int pageScrollBarColor = SDAConsts.clBtnFace;
		private int pageScrollBarFontColor = SDAConsts.clBlack;
		private int inputBarColor = SDAConsts.clWhite;
		// 选择
		private int selectedBackColor = SDAConsts.clFocusShadow;
		private int selectedFontColor = SDAConsts.clWhite;
		// 当前页
		private int pageNo = 1;
		// 总页
		private int pageCount = 1;
		// 输入内容焦点位置(拼音才有)
		public int cursorPos = 0;
		// 输入状态(拼音)
		protected boolean isInput = true;

		public inputCharsPanel() {
			super();
			setVisible(false);
			tabStop = false;
			setWidth(60);
			setHeight(80);
			setBackColor(SDAConsts.clWhite);
			foreColor = SDAConsts.clBlack;
			setOnPointerPressed(new PointerEvent() {

				public void Event(SDABaseControl ctrl, int x, int y) {
					doPointerPressed(x, y);
				}
			});
		}

		// 点击
		private void doPointerPressed(int x, int y) {
			int posx = screenXToClient(x);
			int posy = screenYToClient(y);
			int fontHeight = getFont().getHeight();
			// 根据点击的位置判断
			if (ImeType == imPinYin) {
				if (InClientRect(posx, posy, 0, fontHeight, width, height - 2
						* fontHeight)) {
					selectedHanziIndex = (posy - fontHeight) / fontHeight + 4
							* (posx / (width / 2));
					getHanziByIndex();
				}
				// 翻页
				if (InClientRect(posx, posy, width / 2, height - fontHeight,
						width / 4, fontHeight)) {
					// 左
					priorPage();
				}
				if (InClientRect(posx, posy, 3 * width / 4,
						height - fontHeight, width / 4, fontHeight)) {
					// 左
					nextpage();
				}
			}
			// 字母
			if (ImeType == imUpperCase || ImeType == imLowerCase) {
				charIndex = posy / fontHeight;
				char ch = 13;
				inputFuncChar(ch);
			}
		}

		// 点击选择汉字
		protected boolean getHanziByIndex() {
			boolean result = false;
			if (hanziChars != null) {
				if (selectedHanziIndex + (pageNo - 1) * 8 < hanziChars.length
						&& selectedHanziIndex + (pageNo - 1) * 8 > -1) {
					result = true;
					selectedChar = hanziChars[selectedHanziIndex + (pageNo - 1)
							* 8];
					inputVisibleChar();
					selectedHanziIndex = -1;
					inputChars.setLength(0);
					hanziChars = null;
					isInput = true;
					visible = false;
					form.repaintControl();
				}
			}
			return result;
		}

		public int getPageScrollBarColor() {
			return pageScrollBarColor;
		}

		public void setPageScrollBarColor(int pageScrollBarColor) {
			this.pageScrollBarColor = pageScrollBarColor;
		}

		public int getPageScrollBarFontColor() {
			return pageScrollBarFontColor;
		}

		public void setPageScrollBarFontColor(int pageScrollBarFontColor) {
			this.pageScrollBarFontColor = pageScrollBarFontColor;
		}

		public int getSelectedBackColor() {
			return selectedBackColor;
		}

		public void setSelectedBackColor(int selectedBackColor) {
			this.selectedBackColor = selectedBackColor;
		}

		public int getSelectedFontColor() {
			return selectedFontColor;
		}

		public void setSelectedFontColor(int selectedFontColor) {
			this.selectedFontColor = selectedFontColor;
		}

		public int getInputBarColor() {
			return inputBarColor;
		}

		public void setInputBarColor(int inputBarColor) {
			this.inputBarColor = inputBarColor;
		}

		// 对功能键的处理
		protected void inputFuncChar(char inChar) {
			// 左(输入时移动光标，输入后汉字翻页)
			if (inChar == 37) {
				if (ImeType == imPinYin) {
					if (isInput && cursorPos > 0) {
						cursorPos--;
					}
					if (!isInput) {
						priorPage();
					}
				}
				if (ImeType == imUpperCase || ImeType == imLowerCase) {
					if (numChars != null) {
						priorChar();
					}
				}
			}
			// 上(对汉字等上下滚动)
			if (inChar == 38) {
				if (!isInput && ImeType == imPinYin) {
					// 翻页
					if (pageNo > 1 && selectedHanziIndex == (pageNo - 1) * 8) {
						priorPage();
						return;
					}
					// 选择向上
					if (selectedHanziIndex > 0) {
						selectedHanziIndex--;
					} else {
						isInput = true;
					}
				}
				if (ImeType == imUpperCase || ImeType == imLowerCase) {
					if (numChars != null) {
						priorChar();
					}
				}
			}
			// 右
			if (inChar == 39) {
				if (ImeType == imPinYin) {
					if (isInput && cursorPos < inputChars.length()) {
						cursorPos++;
					}
					if (!isInput) {
						nextpage();
					}
				}
				if (ImeType == imUpperCase || ImeType == imLowerCase) {
					if (numChars != null) {
						nextChar();
					}
				}
			}
			// 下(滚动选择内容)
			if (inChar == 40) {
				if (ImeType == imPinYin) {
					if (!isInput) {
						if (pageNo < pageCount
								&& selectedHanziIndex == (pageNo - 1) * 8 + 7) {
							nextpage();
							return;
						}
						if (selectedHanziIndex < hanziCount - 1) {
							selectedHanziIndex++;
						}
					} else {
						if (hanziChars != null) {
							selectedHanziIndex = 0;
							isInput = false;
						}
					}
				}
				if (ImeType == imLowerCase || ImeType == imUpperCase) {
					if (numChars != null) {
						nextChar();
					}
				}
			}
			// 删除
			if (inChar == 46) {
				if (ImeType == imPinYin) {
					if (cursorPos < inputChars.length()) {
						inputChars.deleteCharAt(cursorPos);
					}
					if (inputChars.length() == 0) {
						visible = false;
					}
				}
				if (ImeType == imLowerCase || ImeType == imUpperCase) {
					numChars = null;
					visible = false;
				}
			}
			// 退格
			if (inChar == 8) {
				if (ImeType == imPinYin) {
					if (cursorPos > 0 && cursorPos < inputChars.length() + 1) {
						inputChars.deleteCharAt(cursorPos - 1);
						cursorPos--;
					}
					if (inputChars.length() == 0) {
						visible = false;
					}
				}
				if (ImeType == imLowerCase || ImeType == imUpperCase) {
					numChars = null;
					visible = false;
				}
			}
			// 回车
			if (inChar == 13) {
				if (ImeType == imPinYin) {
					if (isInput) {
						if (hanziChars != null) {
							if (hanziChars.length > 0) {
								isInput = false;
								selectedHanziIndex = 0;
							}
						}
					} else {
						// 选择的内容
						if (hanziChars != null) {
							if (selectedHanziIndex > -1
									&& selectedHanziIndex < hanziCount) {
								selectedChar = hanziChars[selectedHanziIndex];
								inputVisibleChar();
								selectedHanziIndex = -1;
								inputChars.setLength(0);
								hanziChars = null;
								charsPanel.visible = false;
								isInput = true;
							}
						}
					}
				}
				// 小写字母
				if (ImeType == imLowerCase || ImeType == imUpperCase) {
					if (numChars != null) {
						selectedChar = numChars[charIndex];
						inputVisibleChar();
						charsPanel.visible = false;
						numChars = null;
					}
				}
			}
			// 空格
			if (inChar == ' ' || inChar == '　') {
			}
			if (isVisible()) {
				repaintControl();
			} else {
				if (form != null) {
					form.repaintControl();
				}
			}
		}

		// 翻页
		private void priorPage() {
			if (pageNo > 1) {
				pageNo--;
				selectedHanziIndex = (pageNo - 1) * 8;
			}
		}

		private void nextpage() {
			if (pageNo < pageCount) {
				pageNo++;
				selectedHanziIndex = (pageNo - 1) * 8;
			}
		}

		// 设置大小
		protected void setSize(int colNum, int rowNum) {
			setWidth(30 * colNum);
			setHeight(getFont().getHeight() * rowNum);
		}

		public void paint() {
			if (!IsCanPaint()) {
				return;
			}
			Graphics g = form.getGraphics();
			g.setFont(getFont());
			SetClip(g);
			// 输入内容
			if (ImeType == imPinYin) {
				// 画框
				paintRect(g);
				paintInputPYChars(g);
				paintHanzi(g);
				// 翻页
				paintPages(g);
			}
			if (ImeType == imLowerCase || ImeType == imUpperCase) {
				// 画框
				paintRect(g);
				// 画选择的一键多值
				paintCase(g);
			}
		}

		// 根据当前的组件输入点位置画输入内容显示框和选择框
		public void getCursorPosInForm() {
			SDABaseEdit control = null;
			if (form != null) {
				if (form.focusControl != null) {
					if (form.focusControl instanceof SDABaseEdit) {
						control = (SDABaseEdit) form.focusControl;
						control.setCursorVisible();
						oraX = control.getCursorX() + 2;
						oraY = control.getCursorY();
					}
				}
			}
		}

		// 画
		private void paintRect(Graphics g) {
			g.setColor(backColor);
			fillRect(g, 0, 0, width, height);
			g.setColor(foreColor);
			drawRect(g, 0, 0, width, height);
		}

		// 画一键多字母
		private void paintCase(Graphics g) {
			Font ft = getFont();
			int fontHeight = ft.getHeight();
			if (ImeType == imLowerCase || ImeType == imUpperCase) {
				if (numChars != null) {
					for (int i = 0; i < numChars.length; i++) {
						if (i == charIndex) {
							g.setColor(selectedBackColor);
							fillRect(g, 1, fontHeight * i + 1, width - 1,
									fontHeight - 1);
							g.setColor(selectedFontColor);
							drawString(g, String.valueOf(numChars[i]),
									(width - ft.charWidth(numChars[i])) / 2,
									fontHeight * i);
						} else {
							g.setColor(foreColor);
							drawString(g, String.valueOf(numChars[i]),
									(width - ft.charWidth(numChars[i])) / 2,
									fontHeight * i);
						}
					}
				}
			}
		}

		// 画翻页
		private void paintPages(Graphics g) {
			int fontHeight = getFont().getHeight();
			g.setColor(pageScrollBarColor);
			fillRect(g, 0, 5 * fontHeight, width, fontHeight);
			g.setColor(pageScrollBarFontColor);
			drawRect(g, 0, 5 * fontHeight, width, fontHeight);
			// 计算页码
			pageCount = (hanziCount % 8 > 0) ? hanziCount / 8 + 1
					: hanziCount / 8;
			if (pageCount == 0) {
				pageNo = 0;
			} else {
				if (pageNo == 0) {
					pageNo = 1;
				}
			}
			drawString(g, pageNo + "/" + pageCount, 1, 5 * getFont()
					.getHeight());
			// 箭头
			if (pageNo > 1) {
				fillTriangle(g, width / 2 + width / 8 - 2, 5 * fontHeight
						+ fontHeight / 2, width / 2 + width / 8 + 2, 5
						* fontHeight + fontHeight / 2 - 4, width / 2 + width
						/ 8 + 2, 5 * fontHeight + fontHeight / 2 + 4);
			}
			if (pageCount > pageNo) {
				fillTriangle(g, width - width / 8 + 2, 5 * fontHeight
						+ fontHeight / 2, width - width / 8 - 2, 5 * fontHeight
						+ fontHeight / 2 - 4, width - width / 8 - 2, 5
						* fontHeight + fontHeight / 2 + 4);
			}
		}

		// 画输入内容（拼音输入字母）
		private void paintInputPYChars(Graphics g) {
			Font ft = getFont();
			int fontHeight = ft.getHeight();
			String inString = inputChars.toString();
			g.setColor(inputBarColor);
			fillRect(g, 0, 0, width, fontHeight);
			g.setColor(foreColor);
			drawRect(g, 0, 0, width, fontHeight);
			drawString(g, inString, 2, 0);
			// 画光标
			if (isInput) {
				if (cursorPos > -1 && cursorPos < inString.length() + 1) {
					// System.out.println(inString.substring(0, cursorPos));
					drawLine(
							g,
							ft.stringWidth(inString.substring(0, cursorPos)) + 2,
							2,
							ft.stringWidth(inString.substring(0, cursorPos)) + 2,
							fontHeight - 3);
				}
			}
			// 画未选择的一键多字母
			if (numChars != null && numChars.length > 0) {
				drawString(g, String.valueOf(numChars), width
						- ft.charsWidth(numChars, 0, numChars.length) - 1, 0);
				// 根据索引画选择的
				g.setColor(selectedBackColor);
				fillRect(g, width
						- ft.charsWidth(numChars, charIndex, numChars.length
								- charIndex) - 1, 1, ft
						.charWidth(numChars[charIndex]), fontHeight - 1);
				g.setColor(selectedFontColor);
				drawString(g, String.valueOf(numChars[charIndex]), width
						- ft.charsWidth(numChars, charIndex, numChars.length
								- charIndex) - 1, 0);
			}
		}

		// 切换选择的一键多字母
		public void nextChar() {
			if (numChars != null) {
				if (charIndex < numChars.length - 1) {
					charIndex++;
				}
			}
		}

		public void priorChar() {
			if (numChars != null) {
				if (charIndex > 0) {
					charIndex--;
				}
			}
		}

		// 画汉字选择
		private void paintHanzi(Graphics g) {
			int fontHeight = getFont().getHeight();
			g.setColor(backColor);
			fillRect(g, 0, fontHeight, width, 4 * fontHeight);
			g.setColor(foreColor);
			drawRect(g, 0, fontHeight, width, 4 * fontHeight);
			// 中间分隔条，显示两列汉字
			g.setColor(borderColor);
			drawLine(g, width / 2, fontHeight, width / 2, 5 * fontHeight);
			// 查询对应汉字
			hanziChars = Spell2Chars.getSpell2Chars(inputChars);
			hanziCount = 0;
			if (hanziChars != null) {
				// 计算汉字数目
				hanziCount = hanziChars.length;
				if (selectedHanziIndex >= hanziCount) {
					selectedHanziIndex = 0;
				}
				if (pageNo == 0) {
					pageNo = 1;
				}
				// 画汉字
				int HanziLine = 1;
				int index = 1;
				for (int i = (pageNo - 1) * 8; i < hanziChars.length; i++) {
					// 左边的
					if (!isInput) {
						if (i == selectedHanziIndex) {
							g.setColor(selectedBackColor);
							fillRect(g, 1, HanziLine * fontHeight + 1,
									width / 2 - 1, fontHeight - 1);
							g.setColor(selectedFontColor);
							drawString(g, index + ":" + hanziChars[i], 2,
									HanziLine * fontHeight);
						} else {
							g.setColor(foreColor);
							drawString(g, index + ":" + hanziChars[i], 2,
									HanziLine * fontHeight);
						}
					} else {
						g.setColor(foreColor);
						drawString(g, index + ":" + hanziChars[i], 2, HanziLine
								* fontHeight);
					}
					// 右边的
					if (!isInput) {
						if (i + 4 < hanziChars.length) {
							if (i + 4 == selectedHanziIndex) {
								g.setColor(selectedBackColor);
								fillRect(g, width / 2 + 1, HanziLine
										* fontHeight + 1, width / 2 - 1,
										fontHeight - 1);
								g.setColor(selectedFontColor);
								drawString(g, index + 4 + ":"
										+ hanziChars[i + 4], 2 + width / 2,
										HanziLine * fontHeight);
							} else {
								g.setColor(foreColor);
								drawString(g, index + 4 + ":"
										+ hanziChars[i + 4], 2 + width / 2,
										HanziLine * fontHeight);
							}
						}
					} else {
						if (i + 4 < hanziChars.length) {
							g.setColor(foreColor);
							drawString(g, index + 4 + ":" + hanziChars[i + 4],
									2 + width / 2, HanziLine * fontHeight);
						}
					}
					if (i >= (pageNo - 1) * 8 + 3) {
						return;
					}
					index++;
					HanziLine++;
				}
			}
		}
	}

	protected SDAInputPanel() {
		super();
		setVisible(false);
		setLeft(0);
		setTop(0);
		setWidth(200);
		setHeight(getFont().getHeight() * 4);
		super.setDock(SDAConsts.dsBottom);
		tabStop = false;
		backColor = SDAConsts.clWhite;
		foreColor = SDAConsts.clBlack;
		charsPanel = new inputCharsPanel();

		hashLowerKeyChars = new Hashtable();
		hashUpperKeyChars = new Hashtable();
		// 拼音
		inputChars = new StringBuffer();
		selectedChars = new StringBuffer();
		setOnPointerReleased(new PointerEvent() {

			public void Event(SDABaseControl ctrl, int x, int y) {
				doPointerReleased(x, y);
			}
		});
		setOnPointerReleased(new PointerEvent() {

			public void Event(SDABaseControl ctrl, int x, int y) {
				doPointerReleased(x, y);
			}
		});
		// 字符内容
		UpperCaseChars = new char[] { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
				'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
				'U', 'V', 'W', 'X', 'Y', 'Z' };
		SBCUpperCaseChars = new char[] { 'Ａ', 'Ｂ', 'Ｃ', 'Ｄ', 'Ｅ', 'Ｆ', 'Ｇ',
				'Ｈ', 'Ｉ', 'Ｊ', 'Ｋ', 'Ｌ', 'Ｍ', 'Ｎ', 'Ｏ', 'Ｐ', 'Ｑ', 'Ｒ', 'Ｓ',
				'Ｔ', 'Ｕ', 'Ｖ', 'Ｗ', 'Ｘ', 'Ｙ', 'Ｚ' };
		LowerCaseChars = new char[] { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
				'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
				'u', 'v', 'w', 'x', 'y', 'z' };
		SBCLowerCaseChars = new char[] { 'ａ', 'ｂ', 'ｃ', 'ｄ', 'ｅ', 'ｆ', 'ｇ',
				'ｈ', 'ｉ', 'ｊ', 'ｋ', 'ｌ', 'ｍ', 'ｎ', 'ｏ', 'ｐ', 'ｑ', 'ｒ', 'ｓ',
				'ｔ', 'ｕ', 'ｖ', 'ｗ', 'ｘ', 'ｙ', 'ｚ' };
		DigitChars = new char[] { '0', '1', '2', '3', '4', '5', '6', '7', '8',
				'9' };
		SBCDigitChars = new char[] { '０', '１', '２', '３', '４', '５', '６', '７',
				'８', '９' };
		PointChars = new char[] { '`', ';', '.', ',', '\'', '.', '/', '~', '!',
				'@', '#', '%', '^', '&', '*', '(', ')', '_', '+', '{', '}',
				'[', ']', ':', '"', '<', '>', '?', ' ' };
		SBCPointChars = new char[] { '｀', '；', '．', '，', '＇', '。', '／', '～',
				'！', '＠', '＃', '％', '＾', '＆', '＊', '（', '）', '＿', '＋', '｛',
				'｝', '［', '］', '：', '＂', '＜', '＞', '？', '　' };
		SymbolChars = new char[] { '\\', '%', '=', '±', '/', '-', '+', '￥',
				'€', '￡', '$', '(', ')', '◎', '『', '』', '○', '「', '」', '●',
				'《', '》', '§', '】', '【', '※' };
		SBCSymbolChars = new char[] { '＼', '％', '＝', '±', '／', '－', '＋', '￥',
				'€', '￡', '＄', '（', '）', '◎', '『', '』', '○', '「', '」', '●',
				'《', '》', '§', '】', '【', '※' };
		// 键盘一键多字母组合
		setKeyChars('2', new char[] { 'a', 'b', 'c' }, new char[] { 'A', 'B',
				'C' });
		setKeyChars('3', new char[] { 'd', 'e', 'f' }, new char[] { 'D', 'E',
				'F' });
		setKeyChars('4', new char[] { 'g', 'h', 'i' }, new char[] { 'G', 'H',
				'I' });
		setKeyChars('5', new char[] { 'j', 'k', 'l' }, new char[] { 'J', 'K',
				'L' });
		setKeyChars('6', new char[] { 'm', 'n', 'o' }, new char[] { 'M', 'N',
				'O' });
		setKeyChars('7', new char[] { 'p', 'q', 'r', 's' }, new char[] { 'P',
				'Q', 'R', 'S' });
		setKeyChars('8', new char[] { 't', 'u', 'v' }, new char[] { 'T', 'U',
				'V' });
		setKeyChars('9', new char[] { 'w', 'x', 'y', 'z' }, new char[] { 'W',
				'X', 'Y', 'Z' });
	}

	// 字母组合
	public void setKeyChars(char ch, char[] lowerArrayChar,
			char[] UpperArrayChar) {
		hashLowerKeyChars.put(String.valueOf(ch), lowerArrayChar);
		hashUpperKeyChars.put(String.valueOf(ch), UpperArrayChar);
	}

	// 获取输入的char对应的数组
	protected char[] getLowerKeyChars(char ch) {
		charIndex = 0;
		return (char[]) (hashLowerKeyChars.get(String.valueOf(ch)));
	}

	protected char[] getUpperKeyChars(char ch) {
		charIndex = 0;
		return (char[]) (hashUpperKeyChars.get(String.valueOf(ch)));
	}

	protected char getKeyChar() {
		char ch = 0;
		if (numChars != null) {
			ch = numChars[charIndex];
			charIndex = 0;
		}
		return ch;
	}

	public void paint() {
		if (!IsCanPaint()) {
			charsPanel.paint();
			return;
		}
		Graphics g = form.getGraphics();
		gg = g;
		form.AddControl(charsPanel);
		g.setFont(getFont());
		SetClip(g);
		// 画框
		g.setColor(backColor);
		fillRect(g, 0, 0, width - 1, height - 1);
		g.setColor(borderColor);
		drawRect(g, 0, 0, width - 1, height - 1);
		if (ImeType != imNone) {
			paintInputPanel(g);
		}
		// 判断是否是Bseedit组件
		if (form.focusControl instanceof InputInterface) {
			// 输入内容的选择框
			if (inputChars.length() > 0
					|| (numChars != null && numChars.length > 0)) {
				paintInputChars();
			}
		} else {
			inputChars.setLength(0);
		}

	}

	// 根据不同的输入法画面板
	private void paintInputPanel(Graphics g) {
		curHeight = 0;
		if (ImeType == imUpperCase) {
			// 功能键
			paintFunctionKey(g);
			// 大写
			paintUpperCase(g);
			// 输入法选择
			paintInputMethodSelect(g);
		}

		if (ImeType == imLowerCase) {
			// 功能键
			paintFunctionKey(g);
			// 小写
			paintLowerCase(g);
			// 输入法选择
			paintInputMethodSelect(g);
		}

		if (ImeType == imPinYin) {
			// 功能键
			paintFunctionKey(g);
			// 小写
			paintLowerCase(g);
			// 输入法选择
			paintInputMethodSelect(g);
		}

		if (ImeType == imDigit) {
			// 功能键
			paintFunctionKey(g);
			// 数字
			paintDigit(g);
			// 输入法选择
			paintInputMethodSelect(g);
		}

		if (ImeType == imSymbol) {
			// 功能键
			paintFunctionKey(g);
			// 符号
			paintSymbol(g);
			// 输入法选择
			paintInputMethodSelect(g);
		}

		if (ImeType == imPoint) {
			// 功能键
			paintFunctionKey(g);
			// 标点
			paintPoint(g);
			// 输入法选择
			paintInputMethodSelect(g);
		}

	}

	// 计算输入法外框尺寸
	public void setInputPanelSize() {
		// width = form.width;
		if (ImeType == imLowerCase || ImeType == imUpperCase
				|| ImeType == imPinYin | ImeType == imPoint
				|| ImeType == imSymbol) {
			height = getFont().getHeight() * 4;
		}

		if (ImeType == imDigit) {
			height = getFont().getHeight() * 3;
		}

	}

	public int getBorderColor() {
		return borderColor;
	}

	public void setBorderColor(int borderColor) {
		this.borderColor = borderColor;
	}

	public int getImeType() {
		return ImeType;
	}

	public void setImeType(int ImeType) {
		this.ImeType = ImeType;
		setInputPanelSize();
	}

	public int getSelectedBackColor() {
		return selectedBackColor;
	}

	public void setSelectedBackColor(int selectedBackColor) {
		this.selectedBackColor = selectedBackColor;
	}

	public int getSelectedForeColor() {
		return selectedForeColor;
	}

	public void setSelectedForeColor(int selectedForeColor) {
		this.selectedForeColor = selectedForeColor;
	}

	// 非触摸屏的不可见
	public void setVisible(boolean visible) {
		if (form != null) {
			if (form.Application.hasPointer() && visible) {
				super.setVisible(true);
			} else {
				super.setVisible(false);
			}

			form.paint();
		} else {
			super.setVisible(visible);
		}

	}

	public boolean isVisible() {
		boolean result = false;
		if (form != null) {
			if (form.Application.hasPointer()) {
				result = super.isVisible();
			} else {
				result = false;
			}

		}
		return result;
	}

	// 功能键
	private void paintFunctionKey(Graphics g) {
		g.setColor(borderColor);
		drawRect(g, 0, 0, width - 1, getFont().getHeight());
		int fontHeight = getFont().getHeight();
		Font ft = getFont();
		int[] rightPos = new int[9];
		// 画具体功能(Esc,tab,space,backSpace,enter,del,left,top,right,down)
		for (int i = 1; i < 10; i++) {
			drawLine(g, i * width / 10, curHeight, i * width / 10, curHeight
					+ fontHeight);
			rightPos[i - 1] = i * width / 10;
		}

		if (charYPos == 0 && charXPos == 0) {
			g.setColor(selectedBackColor);
			fillRect(g, 1, curHeight + 1, rightPos[0] - 1, fontHeight - 1);
			g.setColor(selectedForeColor);
			// ESC
			selectedChar = 27;
		} else {
			g.setColor(foreColor);
		}

		drawString(g, "Esc", (width / 10 - ft.stringWidth("Esc")) / 2,
				curHeight);
		if (charYPos == 0 && charXPos == 1) {
			g.setColor(selectedBackColor);
			fillRect(g, width / 10 + 1, curHeight + 1, rightPos[1] - width / 10
					- 1, fontHeight - 1);
			g.setColor(selectedForeColor);
			// Tab
			selectedChar = 9;
		} else {
			g.setColor(foreColor);
		}

		drawLine(g, width / 10 + width / 10 / 4, curHeight + fontHeight / 2,
				width / 10 + 3 * width / 10 / 4, curHeight + fontHeight / 2);
		drawLine(g, width / 10 + 3 * width / 10 / 4, curHeight + fontHeight / 2
				- 4, width / 10 + 3 * width / 10 / 4, curHeight + fontHeight
				/ 2 + 4);
		fillTriangle(g, width / 10 + 3 * width / 10 / 4, curHeight + fontHeight
				/ 2, width / 10 + 3 * width / 10 / 4 - 4, curHeight
				+ fontHeight / 2 - 4, width / 10 + 3 * width / 10 / 4 - 4,
				curHeight + fontHeight / 2 + 4);
		// 空格
		if (charYPos == 0 && charXPos == 2) {
			g.setColor(selectedBackColor);
			fillRect(g, 2 * width / 10 + 1, curHeight + 1, rightPos[2] - 2
					* width / 10 - 1, fontHeight - 1);
			if (!SBCCase) {
				selectedChar = ' ';
			} else {
				selectedChar = '　';
			}

		}
		// ←
		if (charYPos == 0 && charXPos == 3) {
			g.setColor(selectedBackColor);
			fillRect(g, 3 * width / 10 + 1, curHeight + 1, rightPos[3] - 3
					* width / 10 - 1, fontHeight - 1);
			g.setColor(selectedForeColor);
			selectedChar = 37;
		} else {
			g.setColor(foreColor);
		}

		drawString(g, "←", 3 * width / 10 + (width / 10 - ft.stringWidth("←"))
				/ 2, curHeight);
		/*
		 * drawLine(g, 3 width / 10 + width / 10 / 4, curHeight + fontHeight /
		 * 2, 3 width / 10 + 3 width / 10 / 4, curHeight + fontHeight / 2);
		 * fillTriangle(g, 3 width / 10 + width / 10 / 4, curHeight + fontHeight
		 * / 2, 3 width / 10 + width / 10 / 4 + 4, curHeight + fontHeight / 2 -
		 * 4, 3 width / 10 + width / 10 / 4 + 4, curHeight + fontHeight / 2 +
		 * 4);
		 */
		// ↑
		if (charYPos == 0 && charXPos == 4) {
			g.setColor(selectedBackColor);
			fillRect(g, 4 * width / 10 + 1, curHeight + 1, rightPos[4] - 4
					* width / 10 - 1, fontHeight - 1);
			g.setColor(selectedForeColor);
			selectedChar = 38;
		} else {
			g.setColor(foreColor);
		}

		drawString(g, "↑", 4 * width / 10 + (width / 10 - ft.stringWidth("↑"))
				/ 2, curHeight);
		/*
		 * drawLine(g, 4 width / 10 + width / 10 / 2, curHeight + fontHeight /
		 * 4, 4 width / 10 + width / 10 / 2, curHeight + 3 fontHeight / 4);
		 * fillTriangle(g, 4 width / 10 + width / 10 / 2, curHeight + fontHeight
		 * / 4, 4 width / 10 + width / 10 / 2 - 4, curHeight + fontHeight / 4 +
		 * 4, 4 width / 10 + width / 10 / 2 + 4, curHeight + fontHeight / 4 +
		 * 4);
		 */
		// ↓
		if (charYPos == 0 && charXPos == 5) {
			g.setColor(selectedBackColor);
			fillRect(g, 5 * width / 10 + 1, curHeight + 1, rightPos[5] - 5
					* width / 10 - 1, fontHeight - 1);
			g.setColor(selectedForeColor);
			selectedChar = 40;
		} else {
			g.setColor(foreColor);
		}

		drawString(g, "↓", 5 * width / 10 + (width / 10 - ft.stringWidth("↓"))
				/ 2, curHeight);
		/*
		 * drawLine(g, 5 width / 10 + width / 10 / 2, curHeight + fontHeight /
		 * 4, 5 width / 10 + width / 10 / 2, curHeight + 3 fontHeight / 4);
		 * fillTriangle(g, 5 width / 10 + width / 10 / 2, curHeight + 3
		 * fontHeight / 4 , 5 width / 10 + width / 10 / 2 - 4, curHeight + 3
		 * fontHeight / 4 - 4, 5 width / 10 + width / 10 / 2 + 4, curHeight + 3
		 * fontHeight / 4 - 4);
		 */
		// →
		if (charYPos == 0 && charXPos == 6) {
			g.setColor(selectedBackColor);
			fillRect(g, 6 * width / 10 + 1, curHeight + 1, rightPos[6] - 6
					* width / 10 - 1, fontHeight - 1);
			g.setColor(selectedForeColor);
			selectedChar = 39;
		} else {
			g.setColor(foreColor);
		}

		drawString(g, "→", 6 * width / 10 + (width / 10 - ft.stringWidth("→"))
				/ 2, curHeight);
		/*
		 * drawLine(g, 6 width / 10 + width / 10 / 4, curHeight + fontHeight /
		 * 2, 6 width / 10 + 3 width / 10 / 4, curHeight + fontHeight / 2);
		 * fillTriangle(g, 6 width / 10 + 3 width / 10 / 4 + 1, curHeight +
		 * fontHeight / 2, 6 width / 10 + 3 width / 10 / 4 - 3, curHeight +
		 * fontHeight / 2 - 4, 6 width / 10 + 3 width / 10 / 4 - 3, curHeight +
		 * fontHeight / 2 + 4);
		 */
		if (charYPos == 0 && charXPos == 7) {
			g.setColor(selectedBackColor);
			fillRect(g, 7 * width / 10 + 1, curHeight + 1, rightPos[7] - 7
					* width / 10 - 1, fontHeight - 1);
			g.setColor(selectedForeColor);
			selectedChar = 46;
		} else {
			g.setColor(foreColor);
		}

		drawString(g, "Del", 7 * width / 10
				+ (width / 10 - ft.stringWidth("Del")) / 2, curHeight);
		// 退格
		if (charYPos == 0 && charXPos == 8) {
			g.setColor(selectedBackColor);
			fillRect(g, 8 * width / 10 + 1, curHeight + 1, rightPos[8] - 8
					* width / 10 - 1, fontHeight - 1);
			g.setColor(selectedForeColor);
			selectedChar = 8;
		} else {
			g.setColor(foreColor);
		}

		fillRect(g, 8 * width / 10 + width / 10 / 4 + 4, curHeight + fontHeight
				/ 2 - 1, width / 10 / 3, 3);
		fillTriangle(g, 8 * width / 10 + width / 10 / 4, curHeight + fontHeight
				/ 2, 8 * width / 10 + width / 10 / 4 + 5, curHeight
				+ fontHeight / 2 - 5, 8 * width / 10 + width / 10 / 4 + 5,
				curHeight + fontHeight / 2 + 5);
		// 回车
		if (charYPos == 0 && charXPos == 9) {
			g.setColor(selectedBackColor);
			fillRect(g, 9 * width / 10 + 1, curHeight + 1, width - 9 * width
					/ 10 - 1, fontHeight - 1);
			g.setColor(selectedForeColor);
			selectedChar = 13;
		} else {
			g.setColor(foreColor);
		}

		drawLine(g, 9 * width / 10 + width / 10 / 4, curHeight + 3 * fontHeight
				/ 4, 9 * width / 10 + 3 * width / 10 / 4, curHeight + 3
				* fontHeight / 4);
		drawLine(g, 9 * width / 10 + 3 * width / 10 / 4, curHeight + fontHeight
				/ 4, 9 * width / 10 + 3 * width / 10 / 4, curHeight + 3
				* fontHeight / 4);
		fillTriangle(g, 9 * width / 10 + width / 10 / 4, curHeight + 3
				* fontHeight / 4, 9 * width / 10 + width / 10 / 4 + 4,
				curHeight + 3 * fontHeight / 4 - 4, 9 * width / 10 + width / 10
						/ 4 + 4, curHeight + 3 * fontHeight / 4 + 4);
		curHeight += fontHeight;
	}

	private void getFuncChar() {
		if (charYPos == 0 && charXPos == 0) {
			// ESC
			selectedChar = 27;
		}

		if (charYPos == 0 && charXPos == 1) {
			// Tab
			selectedChar = 9;
		}

		if (charYPos == 0 && charXPos == 2) {
			if (!SBCCase) {
				selectedChar = ' ';
			} else {
				selectedChar = '　';
			}

		}
		// ←
		if (charYPos == 0 && charXPos == 3) {
			selectedChar = 37;
		}
		// ↑
		if (charYPos == 0 && charXPos == 4) {
			selectedChar = 38;
		}
		// ↓
		if (charYPos == 0 && charXPos == 5) {
			selectedChar = 40;
		}
		// →
		if (charYPos == 0 && charXPos == 6) {
			selectedChar = 39;
		}

		if (charYPos == 0 && charXPos == 7) {
			selectedChar = 46;
		}
		// 退格
		if (charYPos == 0 && charXPos == 8) {
			selectedChar = 8;
		}
		// 回车
		if (charYPos == 0 && charXPos == 9) {
			selectedChar = 13;
		}

	}

	// 大写字母
	private void paintUpperCase(Graphics g) {
		Font ft = getFont();
		char[] Chars1 = new char[13];
		System.arraycopy(UpperCaseChars, 0, Chars1, 0, 13);
		char[] Chars2 = new char[13];
		System.arraycopy(UpperCaseChars, 13, Chars2, 0, 13);
		char[] Chars3 = new char[13];
		System.arraycopy(SBCUpperCaseChars, 0, Chars3, 0, 13);
		char[] Chars4 = new char[13];
		System.arraycopy(SBCUpperCaseChars, 13, Chars4, 0, 13);

		g.setColor(borderColor);
		int fontHeight = getFont().getHeight();
		g.setFont(getFont());
		drawRect(g, 0, curHeight, width - 1, fontHeight);
		drawRect(g, 0, curHeight + fontHeight, width - 1, fontHeight);
		// 画具体(ABCD....)
		for (int i = 0; i < 13; i++) {
			if (charYPos == 1 && charXPos == i) {
				g.setColor(selectedBackColor);
				fillRect(g, i * width / 13 + 1, curHeight + 1, (i + 1) * width
						/ 13 - i * width / 13 - 1, fontHeight - 1);
				g.setColor(selectedForeColor);
				if (SBCCase) {
					selectedChar = Chars3[i];
				} else {
					selectedChar = Chars1[i];
				}

			} else {
				g.setColor(foreColor);
			}

			drawString(g, String.valueOf(Chars1[i]), i * width / 13
					+ (width / 13 - ft.charWidth(Chars1[i])) / 2, curHeight);
			g.setColor(foreColor);
			drawLine(g, i * width / 13, curHeight, i * width / 13, curHeight
					+ fontHeight);
		}

		for (int i = 0; i < 13; i++) {
			if (charYPos == 2 && charXPos == i) {
				g.setColor(selectedBackColor);
				fillRect(g, i * width / 13 + 1, curHeight + fontHeight + 1,
						(i + 1) * width / 13 - i * width / 13 - 1,
						fontHeight - 1);
				g.setColor(selectedForeColor);
				if (SBCCase) {
					selectedChar = Chars4[i];
				} else {
					selectedChar = Chars2[i];
				}

			} else {
				g.setColor(foreColor);
			}

			drawString(g, String.valueOf(Chars2[i]), i * width / 13
					+ (width / 13 - ft.charWidth(Chars2[i])) / 2, curHeight
					+ fontHeight);
			g.setColor(foreColor);
			drawLine(g, i * width / 13, curHeight + fontHeight, i * width / 13,
					curHeight + 2 * fontHeight);
		}

		curHeight += 2 * fontHeight;
	}

	// 小写字母
	private void paintLowerCase(Graphics g) {
		// ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ
		Font ft = getFont();
		char[] Chars1 = new char[13];
		System.arraycopy(LowerCaseChars, 0, Chars1, 0, 13);
		char[] Chars2 = new char[13];
		System.arraycopy(LowerCaseChars, 13, Chars2, 0, 13);
		char[] Chars3 = new char[13];
		System.arraycopy(SBCLowerCaseChars, 0, Chars3, 0, 13);
		char[] Chars4 = new char[13];
		System.arraycopy(SBCLowerCaseChars, 13, Chars4, 0, 13);

		g.setColor(borderColor);
		int fontHeight = getFont().getHeight();
		g.setFont(getFont());
		drawRect(g, 0, curHeight, width - 1, fontHeight);
		drawRect(g, 0, curHeight + fontHeight, width - 1, fontHeight);
		// 画具体(ABCD....)
		for (int i = 0; i < 13; i++) {
			if (charYPos == 1 && charXPos == i) {
				g.setColor(selectedBackColor);
				fillRect(g, i * width / 13 + 1, curHeight + 1, (i + 1) * width
						/ 13 - i * width / 13 - 1, fontHeight - 1);
				g.setColor(selectedForeColor);
				if (SBCCase) {
					selectedChar = Chars3[i];
				} else {
					selectedChar = Chars1[i];
				}

			} else {
				g.setColor(foreColor);
			}

			drawString(g, String.valueOf(Chars1[i]), i * width / 13
					+ (width / 13 - ft.charWidth(Chars1[i])) / 2, curHeight);
			g.setColor(foreColor);
			drawLine(g, i * width / 13, curHeight, i * width / 13, curHeight
					+ fontHeight);
		}

		for (int i = 0; i < 13; i++) {
			if (charYPos == 2 && charXPos == i) {
				g.setColor(selectedBackColor);
				fillRect(g, i * width / 13 + 1, curHeight + fontHeight + 1,
						(i + 1) * width / 13 - i * width / 13 - 1,
						fontHeight - 1);
				g.setColor(selectedForeColor);
				if (SBCCase) {
					selectedChar = Chars4[i];
				} else {
					selectedChar = Chars2[i];
				}

			} else {
				g.setColor(foreColor);
			}

			drawString(g, String.valueOf(Chars2[i]), i * width / 13
					+ (width / 13 - ft.charWidth(Chars2[i])) / 2, curHeight
					+ fontHeight);
			g.setColor(foreColor);
			drawLine(g, i * width / 13, curHeight + fontHeight, i * width / 13,
					curHeight + 2 * fontHeight);
		}

		curHeight += 2 * fontHeight;
	}

	// 数字
	private void paintDigit(Graphics g) {
		char[] Chars1 = new char[10];
		System.arraycopy(SBCDigitChars, 0, Chars1, 0, 10);
		char[] Chars2 = new char[10];
		System.arraycopy(DigitChars, 0, Chars2, 0, 10);

		g.setColor(borderColor);
		int fontHeight = getFont().getHeight();
		Font ft = getFont();
		drawRect(g, 0, curHeight, width - 1, fontHeight);
		// 画具体(A,a,1,,.,#,PY,全)
		g.setColor(foreColor);
		for (int i = 1; i < 10; i++) {
			drawLine(g, i * width / 10, curHeight, i * width / 10, curHeight
					+ fontHeight);
		}

		for (int i = 0; i < 10; i++) {
			if (charYPos == 1 && charXPos == i) {
				g.setColor(selectedBackColor);
				fillRect(g, i * width / 10 + 1, curHeight + 1, (i + 1) * width
						/ 10 - i * width / 10 - 1, fontHeight - 1);
				g.setColor(selectedForeColor);
				if (SBCCase) {
					selectedChar = Chars1[i];
				} else {
					selectedChar = Chars2[i];
				}

			} else {
				g.setColor(foreColor);
			}

			drawString(g, String.valueOf(i), i * width / 10
					+ (width / 10 - ft.stringWidth(String.valueOf(i))) / 2,
					curHeight);
		}

		curHeight += fontHeight;
	}

	// 标点
	private void paintPoint(Graphics g) {
		Font ft = getFont();
		char[] Chars1 = new char[14];
		System.arraycopy(PointChars, 0, Chars1, 0, 14);
		char[] Chars2 = new char[14];
		System.arraycopy(PointChars, 14, Chars2, 0, 14);
		char[] Chars3 = new char[14];
		System.arraycopy(SBCPointChars, 0, Chars3, 0, 14);
		char[] Chars4 = new char[14];
		System.arraycopy(SBCPointChars, 14, Chars4, 0, 14);

		g.setColor(borderColor);
		int fontHeight = getFont().getHeight();
		g.setFont(getFont());
		drawRect(g, 0, curHeight, width - 1, fontHeight);
		drawRect(g, 0, curHeight + fontHeight, width - 1, fontHeight);
		// 画具体(ABCD....)
		for (int i = 0; i < 14; i++) {
			if (charYPos == 1 && charXPos == i) {
				g.setColor(selectedBackColor);
				fillRect(g, i * width / 14 + 1, curHeight + 1, (i + 1) * width
						/ 14 - i * width / 14 - 1, fontHeight - 1);
				g.setColor(selectedForeColor);
				if (SBCCase) {
					selectedChar = Chars3[i];
				} else {
					selectedChar = Chars1[i];
				}

			} else {
				g.setColor(foreColor);
			}

			drawString(g, String.valueOf(Chars1[i]), i * width / 14
					+ (width / 14 - ft.charWidth(Chars1[i])) / 2, curHeight);
			g.setColor(foreColor);
			drawLine(g, i * width / 14, curHeight, i * width / 14, curHeight
					+ fontHeight);
		}

		for (int i = 0; i < 14; i++) {
			if (charYPos == 2 && charXPos == i) {
				g.setColor(selectedBackColor);
				fillRect(g, i * width / 14 + 1, curHeight + fontHeight + 1,
						(i + 1) * width / 14 - i * width / 14 - 1,
						fontHeight - 1);
				g.setColor(selectedForeColor);
				if (SBCCase) {
					selectedChar = Chars4[i];
				} else {
					selectedChar = Chars2[i];
				}

			} else {
				g.setColor(foreColor);
			}

			drawString(g, String.valueOf(Chars2[i]), i * width / 14
					+ (width / 14 - ft.charWidth(Chars2[i])) / 2, curHeight
					+ fontHeight);
			g.setColor(foreColor);
			drawLine(g, i * width / 14, curHeight + fontHeight, i * width / 14,
					curHeight + 2 * fontHeight);
		}

		curHeight += 2 * fontHeight;
	}

	// 符号
	private void paintSymbol(Graphics g) {
		Font ft = getFont();
		char[] Chars1 = new char[13];
		System.arraycopy(SymbolChars, 0, Chars1, 0, 13);
		char[] Chars2 = new char[13];
		System.arraycopy(SymbolChars, 13, Chars2, 0, 13);
		char[] Chars3 = new char[13];
		System.arraycopy(SBCSymbolChars, 0, Chars3, 0, 13);
		char[] Chars4 = new char[13];
		System.arraycopy(SBCSymbolChars, 13, Chars4, 0, 13);
		g.setColor(borderColor);
		int fontHeight = getFont().getHeight();
		g.setFont(getFont());
		drawRect(g, 0, curHeight, width - 1, fontHeight);
		drawRect(g, 0, curHeight + fontHeight, width - 1, fontHeight);
		// 画具体(ABCD....)
		for (int i = 0; i < 13; i++) {
			if (charYPos == 1 && charXPos == i) {
				g.setColor(selectedBackColor);
				fillRect(g, i * width / 13 + 1, curHeight + 1, (i + 1) * width
						/ 13 - i * width / 13 - 1, fontHeight - 1);
				g.setColor(selectedForeColor);
				if (SBCCase) {
					selectedChar = Chars3[i];
				} else {
					selectedChar = Chars1[i];
				}

			} else {
				g.setColor(foreColor);
			}

			drawString(g, String.valueOf(Chars1[i]), i * width / 13
					+ (width / 13 - ft.charWidth(Chars1[i])) / 2, curHeight);
			g.setColor(foreColor);
			drawLine(g, i * width / 13, curHeight, i * width / 13, curHeight
					+ fontHeight);
		}

		for (int i = 0; i < 13; i++) {
			if (charYPos == 2 && charXPos == i) {
				g.setColor(selectedBackColor);
				fillRect(g, i * width / 13 + 1, curHeight + fontHeight + 1,
						(i + 1) * width / 13 - i * width / 13 - 1,
						fontHeight - 1);
				g.setColor(selectedForeColor);
				if (SBCCase) {
					selectedChar = Chars4[i];
				} else {
					selectedChar = Chars2[i];
				}

			} else {
				g.setColor(foreColor);
			}

			drawString(g, String.valueOf(Chars2[i]), i * width / 13
					+ (width / 13 - ft.charWidth(Chars2[i])) / 2, curHeight
					+ fontHeight);
			g.setColor(foreColor);
			drawLine(g, i * width / 13, curHeight + fontHeight, i * width / 13,
					curHeight + 2 * fontHeight);
		}

		curHeight += 2 * fontHeight;
	}

	// 拼音采用小写字母
	// 输入法选择
	private void paintInputMethodSelect(Graphics g) {
		g.setColor(borderColor);
		int fontHeight = getFont().getHeight();
		Font ft = getFont();
		drawRect(g, 0, curHeight, width - 1, fontHeight);
		// 画具体(A,a,1,,.,#,PY,全)
		int[] rightPos = new int[6];
		for (int i = 1; i < 7; i++) {
			drawLine(g, i * width / 7, curHeight, i * width / 7, curHeight
					+ fontHeight);
			rightPos[i - 1] = i * width / 7;
		}

		drawString(g, "ABC", (width / 7 - ft.stringWidth("ABC")) / 2, curHeight);
		drawString(g, "abc", width / 7 + (width / 7 - ft.stringWidth("abc"))
				/ 2, curHeight);
		drawString(g, "123", 2 * width / 7
				+ (width / 7 - ft.stringWidth("123")) / 2, curHeight);
		drawString(g, "，。", 3 * width / 7 + (width / 7 - ft.stringWidth("，。"))
				/ 2, curHeight);
		drawString(g, "※", 4 * width / 7 + (width / 7 - ft.stringWidth("※"))
				/ 2, curHeight);
		drawString(g, "中", 5 * width / 7 + (width / 7 - ft.stringWidth("中"))
				/ 2, curHeight);
		drawString(g, "全", 6 * width / 7 + (width / 7 - ft.stringWidth("全"))
				/ 2, curHeight);
		// 画被选中的
		switch (ImeType) {
		case imUpperCase:
			g.setColor(selectedBackColor);
			fillRect(g, 1, curHeight + 1, rightPos[0] - 1, fontHeight - 2);
			g.setColor(selectedForeColor);
			drawString(g, "ABC", (width / 7 - ft.stringWidth("ABC")) / 2,
					curHeight);
			break;
		case imLowerCase:
			g.setColor(selectedBackColor);
			fillRect(g, width / 7 + 1, curHeight + 1, rightPos[1] - width / 7
					- 1, fontHeight - 2);
			g.setColor(selectedForeColor);
			drawString(g, "abc", width / 7
					+ (width / 7 - ft.stringWidth("abc")) / 2, curHeight);
			break;
		case imDigit:
			g.setColor(selectedBackColor);
			fillRect(g, 2 * width / 7 + 1, curHeight + 1, rightPos[2] - 2
					* width / 7 - 1, fontHeight - 2);
			g.setColor(selectedForeColor);
			drawString(g, "123", 2 * width / 7
					+ (width / 7 - ft.stringWidth("123")) / 2, curHeight);
			break;
		case imPoint:
			g.setColor(selectedBackColor);
			fillRect(g, 3 * width / 7 + 1, curHeight + 1, rightPos[3] - 3
					* width / 7 - 1, fontHeight - 2);
			g.setColor(selectedForeColor);
			drawString(g, "，。", 3 * width / 7
					+ (width / 7 - ft.stringWidth("，。")) / 2, curHeight);
			break;
		case imSymbol:
			g.setColor(selectedBackColor);
			fillRect(g, 4 * width / 7 + 1, curHeight + 1, rightPos[4] - 4
					* width / 7 - 1, fontHeight - 2);
			g.setColor(selectedForeColor);
			drawString(g, "※", 4 * width / 7
					+ (width / 7 - ft.stringWidth("※")) / 2, curHeight);
			break;
		case imPinYin:
			g.setColor(selectedBackColor);
			fillRect(g, 5 * width / 7 + 1, curHeight + 1, rightPos[5] - 5
					* width / 7 - 1, fontHeight - 2);
			g.setColor(selectedForeColor);
			drawString(g, "中", 5 * width / 7
					+ (width / 7 - ft.stringWidth("中")) / 2, curHeight);
			break;
		}
		// 全角
		if (SBCCase) {
			g.setColor(selectedBackColor);
			fillRect(g, 6 * width / 7 + 1, curHeight + 1, width - 6 * width / 7
					- 2, fontHeight - 2);
			g.setColor(selectedForeColor);
			drawString(g, "全", 6 * width / 7
					+ (width / 7 - ft.stringWidth("全")) / 2, curHeight);
		}

		curHeight += fontHeight;
	}

	// 获取选择的内容
	public char getSelectedChar() {
		return selectedChar;
	}

	// 位置
	protected void setCharsPanelPos() {
		if (ImeType == imPinYin) {
			charsPanel.setSize(2, 6);
		} else {
			if (numChars != null) {
				if (numChars.length * getFont().getHeight() != charsPanel.height) {
					charsPanel.setSize(1, numChars.length);
					form.repaintControl();
				}
			}
		}
		charsPanel.getCursorPosInForm();
		charsPanel.setLeft(charsPanel.oraX);
		charsPanel.setTop(charsPanel.oraY);
		if (charsPanel.getWidth() + charsPanel.oraX > form.canvas.getWidth()) {
			charsPanel.setLeft(form.canvas.getWidth() - charsPanel.getWidth());
		}
		if (charsPanel.getHeight() + charsPanel.oraY > form.canvas.getHeight()
				- charsPanel.getHeight()) {
			int atop = charsPanel.oraY - getFont().getHeight()
					- charsPanel.getHeight();
			if (atop >= 0) {
				charsPanel.setTop(atop);
			} else {
				if (charsPanel.oraX > charsPanel.getWidth()) {
					charsPanel.setLeft(charsPanel.oraX - charsPanel.getWidth());
				} else {
					charsPanel.setLeft(charsPanel.oraX + 10);
				}

				charsPanel.setTop(0);
			}
		}
	}

	// 显示输入内容
	private void paintInputChars() {
		setCharsPanelPos();
		charsPanel.paint();
	}

	// 点面板改变输入法
	private void doPointerReleased(int x, int y) {
		int posx = screenXToClient(x);
		int posy = screenYToClient(y);
		int fontHeight = getFont().getHeight();
		int pos = 0;
		if (InClientRect(posx, posy, 0, height - fontHeight, width, fontHeight)) {
			charXPos = -1;
			charYPos = -1;
			// 根据x位置判断输入法类型
			pos = posx * 7 / width;
			switch (pos) {
			case 0:
				ImeType = imUpperCase;
				break;
			case 1:
				ImeType = imLowerCase;
				break;
			case 2:
				ImeType = imDigit;
				break;
			case 3:
				ImeType = imPoint;
				break;
			case 4:
				ImeType = imSymbol;
				break;
			case 5:
				ImeType = imPinYin;
				SBCCase = false;
				break;
			case 6:
				if (ImeType != imPinYin) {
					SBCCase = !SBCCase;
				}
				break;
			}
			setInputPanelSize();
			charsPanel.visible = false;
			inputChars.setLength(0);
			numChars = null;
			form.repaintControl();
		} else if (InClientRect(posx, posy, 0, 0, width, fontHeight)) {
			charXPos = posx * 10 / width;
			charYPos = 0;
			getFuncChar();
			inputFuncChar();
			paint();
		} else {
			if (ImeType == imDigit) {
				// 三行
				charXPos = posx * 10 / width;
				charYPos = 1;
				paintDigit(gg);
				inputVisibleChar();
			} else {
				// 四行的
				if (ImeType == imUpperCase || ImeType == imLowerCase
						|| ImeType == imPinYin || ImeType == imSymbol) {
					charXPos = posx * 13 / width;
					charYPos = posy / fontHeight;
					if (ImeType == imUpperCase) {
						paintUpperCase(gg);
						inputVisibleChar();
					}

					if (ImeType == imLowerCase) {
						paintLowerCase(gg);
						inputVisibleChar();
					}

					if (ImeType == imSymbol) {
						paintSymbol(gg);
						inputVisibleChar();
					}

					if (ImeType == imPinYin) {
						paintLowerCase(gg);
						if (!SBCCase) {
							inputPinYinChar();
						} else {
							inputVisibleChar();
						}
					}
				}
				if (ImeType == imPoint) {
					charXPos = posx * 14 / width;
					charYPos = posy / fontHeight;
					paintPoint(gg);
					inputVisibleChar();
				}
			}
			if (charsPanel.visible) {
				if (ImeType == imUpperCase || ImeType == imLowerCase) {
					charsPanel.visible = false;
					numChars = null;
					form.repaintControl();
				} else {
					paint();
				}
			} else {
				paint();
			}
		}
	}

	// 根据选择的char输入到焦点组件
	public void inputVisibleChar() {
		InputInterface control = null;
		if (form.focusControl != null) {
			if (form.focusControl instanceof InputInterface) {
				control = (InputInterface) form.focusControl;
				control.InputVisibleChar(selectedChar);
			}
		}
		selectedChar = 0;
	}

	// 拼音输入法处理
	protected void inputPinYinChar() {
		if (form.focusControl == null
				|| (!(form.focusControl instanceof InputInterface))) {
			selectedChar = 0;
			return;
		}
		if (inputChars.length() == 0) {
			charsPanel.cursorPos = 0;
			charsPanel.isInput = true;
			inputChars.append(selectedChar);
			charsPanel.cursorPos++;
		} else {
			inputChars.insert(charsPanel.cursorPos, selectedChar);
			charsPanel.cursorPos++;
		}
		charsPanel.visible = true;
		charsPanel.pageNo = 1;
	}

	protected void inputFuncChar() {
		InputInterface control = null;
		// 判断输入选择框是不是已经显示，显示的话就发向这个选择框
		if (charsPanel.isVisible()) {
			charsPanel.inputFuncChar(selectedChar);
		} else {
			if (form.focusControl != null) {
				if (form.focusControl instanceof InputInterface) {
					control = (InputInterface) form.focusControl;
					control.InputFuncChar(selectedChar, SBCCase);
					selectedChar = 0;
				}
			}
		}
	}

	public void setDock(int dock) {
		super.setDock(SDAConsts.dsBottom);
	}

	// 获取内容数组
	public char[] getUpperChars() {
		return UpperCaseChars;
	}

	public char[] getSBCUpperChars() {
		return SBCUpperCaseChars;
	}

	public char[] getLowerChars() {
		return LowerCaseChars;
	}

	public char[] getSBCLowerCaseChars() {
		return SBCLowerCaseChars;
	}

	public char[] getDigitChars() {
		return DigitChars;
	}

	public char[] getSBCDigitChars() {
		return SBCDigitChars;
	}

	public char[] getSymbolChars() {
		return SymbolChars;
	}

	public char[] getSBCSymbolChars() {
		return SBCSymbolChars;
	}

	public char[] getPointChars() {
		return PointChars;
	}

	public char[] getSBCPointChars() {
		return SBCPointChars;
	}

	public boolean isSBCCase() {
		return SBCCase;
	}

	public void setSBCCase(boolean SBCCase) {
		this.SBCCase = SBCCase;
	}
}
