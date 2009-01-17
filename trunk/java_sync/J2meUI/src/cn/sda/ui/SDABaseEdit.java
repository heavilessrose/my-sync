package cn.sda.ui;

import cn.sda.event.BaseEditTextChangeEvent;
import cn.sda.event.KeybordEvent;
import cn.sda.event.NotifyEvent;
import cn.sda.event.PointerEvent;
import cn.sda.pinyin.InputInterface;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Font;

/**
 * @author not attributable
 * @version 1.0
 */
public class SDABaseEdit extends SDABaseControl implements InputInterface {
	// 边框
	private int borderStyle = SDAConsts.bsNone;
	// 是不是显示焦点框
	private boolean showFocusRect = false;
	// 隐藏选定内容
	private boolean hideSelection = true;
	// 最大长度
	private int maxLength = 32767;
	// 是否显示多行
	private boolean multiLine = false;
	// 密码
	private char passwordChar = 0;
	// 只读
	protected boolean readOnly = false;
	// 滚动边框(多行)
	private int scrollBars = SDAConsts.srNone;
	// 文字对齐方式
	private int textAlign = SDAConsts.alignLeft;
	// 是否自动换行(多行)
	private boolean wordWrap = true;
	private int barwidth = 10;
	// 边框线颜色
	private int borderColor = SDAConsts.clBlack;
	// 滚动条颜色
	private int scrollBarColor = SDAConsts.clBtnFace;
	// 内部变量
	// 行数，行最大长度
	private int lineNum = 0;
	private int maxLineLenght = 0;
	// 垂直开始显示行
	private int startLine = 0;
	// 水平开始显示位置
	private int startLeft = 0;
	// 变动前位置
	private int oldStartLine = 0;
	private int oldStartLeft = 0;
	// 行数
	private int lineNo = 0;
	// 水平滚条位置
	private int HSLeft = 0;
	private int HSTop = 0;
	private int HSWidth = 0;
	private int HSHeight = 0;
	// 垂直滚动条位置
	private int VSLeft = 0;
	private int VSTop = 0;
	private int VSWidth = 0;
	private int VSHeight = 0;
	// 点击的滚动条的位置
	private int oldScrollPointx = 0;
	private int oldScrollPointy = 0;
	// 按下滚动条
	private boolean isscrollbarpointdown = false;
	// 记录按下的滚动条0:水平，1：垂直的
	private byte scrollbardownHV = 0;
	// 当前光标位置
	private int cursorPos = 0;
	// 光标行列
	private int cursorRow = 0;
	protected int cursorCol = 0;
	// 光标绝对位置
	private int cursorX = 0;
	private int cursorY = 0;
	// 选择行的起始位置和内容
	private int cursorLineStartPos = 0;
	private String cursorLineText = "";
	// 前一行长度
	private int priorLineLen = 0;
	// 临时buffer
	StringBuffer strBuffer = new StringBuffer();
	// 对齐方式
	private int oldAlignType = SDAConsts.alignLeft;
	// 输入法
	private int imeType = SDAInputPanel.imNone;
	// 文字变动
	private BaseEditTextChangeEvent onTextChange = null;
	//
	Graphics gg = null;

	protected SDABaseEdit() {
		super();
		internalSDAEdit();
	}

	private void internalSDAEdit() {
		setVisible(true);
		setLeft(0);
		setTop(0);
		setBorderStyle(SDAConsts.bsFixedSingle);
		super.setWidth(60);
		// 高度根据字体高度来
		setHeight(getFont().getHeight() + 4);
		super.setBackColor(SDAConsts.clWhite);

		onPointerPressed = new PointerEvent() {

			public void Event(SDABaseControl ctrl, int x, int y) {
				doPointerPressed(x, y);
			}
		};
		onPointerReleased = new PointerEvent() {

			public void Event(SDABaseControl ctrl, int x, int y) {
				doPointerReleased(x, y);
			}
		};
		onKeyUp = new KeybordEvent() {

			public void Event(SDABaseControl ctrl, int keyCode) {
				doKeyUp(keyCode);
			}
		};
		onInternalLostFocused = new NotifyEvent() {

			public void Event(SDABaseControl ctrl) {
				doLostFocus();
			}
		};
		onInternalFocused = new NotifyEvent() {

			public void Event(SDABaseControl ctrl) {
				doGetFocus();
			}
		};
		// 设置系统菜单
		/*
		 * systemMenu=new SDAPopMenu(); systemMenu.AddItem("back", "退格");
		 * systemMenu.AddItem("delete", "删除"); systemMenu.AddItem("enter",
		 * "插入回车"); systemMenu.AddItem("", "-"); systemMenu.AddItem("ABC",
		 * "大写字母").setRadioItem(true); systemMenu.AddItem("abc",
		 * "小写字母").setRadioItem(true); systemMenu.AddItem("123",
		 * "数字").setRadioItem(true); systemMenu.AddItem("拼音",
		 * "中文拼音").setRadioItem(true); systemMenu.AddItem("标点",
		 * "插入标点").setRadioItem(true); systemMenu.AddItem("符号",
		 * "插入符号").setRadioItem(true); systemMenu.AddItem("", "-");
		 * systemMenu.AddItem("全", "全角"); systemMenu.setOnClick(new
		 * PopMenuOnClickEvent() { public void Event(SDAMenuItem item, String
		 * itemName, String itemCaption) { //处理 if(itemName.equals("back")){
		 * char ch=8; inputFuncChar(ch, false); } if(itemName.equals("delete")){
		 * char ch=46; inputFuncChar(ch, false); } if(itemName.equals("enter")){
		 * char ch=13; inputFuncChar(ch, false); } if (itemName.equals("ABC")) {
		 * form.Application.inputPanel.setImeType(SDAInputPanel.imUpperCase); }
		 * if (itemName.equals("abc")) {
		 * form.Application.inputPanel.setImeType(SDAInputPanel.imLowerCase); }
		 * if (itemName.equals("123")) {
		 * form.Application.inputPanel.setImeType(SDAInputPanel.imDigit); } if
		 * (itemName.equals("拼音")) {
		 * form.Application.inputPanel.setImeType(SDAInputPanel.imPinYin); } if
		 * (itemName.equals("标点")) {
		 * form.Application.inputPanel.setImeType(SDAInputPanel.imPoint);
		 * SDASelectPointForm.getInstance().open(getSelf()); } if
		 * (itemName.equals("符号")) {
		 * form.Application.inputPanel.setImeType(SDAInputPanel.imSymbol);
		 * SDASelectSymbolForm.getInstance().open(getSelf()); } if
		 * (itemName.equals("全")) {
		 * form.Application.inputPanel.setSBCCase(!form.
		 * Application.inputPanel.isSBCCase());
		 * item.setChecked(form.Application.inputPanel.isSBCCase()); } else {
		 * item.setChecked(true); } } });
		 */
	}

	public void paint() {
		internalPaint();
	}

	private void internalPaint() {
		if (!IsCanPaint()) {
			return;
		}
		Graphics g = form.getGraphics();
		gg = g;
		// 设置字体
		g.setFont(getFont());
		gg.setFont(getFont());
		// 可视下才画
		if (isVisible()) {
			// 初始化引用变量
			int thisWidth = width;
			int thisHeight = height;
			int HBarWidth = getHBarWidth();
			int VBarHeight = getVBarHeight();
			int fontHeight = getFont().getHeight();
			// 对齐方式处理后，画组件
			// 外框
			SetClip(g);
			// 透明
			if (!isTransparent()) {
				g.setColor(backColor);
				fillRect(g, 0, 0, thisWidth, thisHeight);
			}
			if (borderStyle == SDAConsts.bsFixed3D) {
				g.setColor(SDAConsts.clGray);
				drawLine(g, 0, 0, thisWidth, 0);
				drawLine(g, 0, 0, 0, thisHeight);
				g.setColor(SDAConsts.clBlack);
				drawLine(g, 1, 1, thisWidth - 1, 1);
				drawLine(g, 1, 1, 1, thisHeight - 1);
				g.setColor(SDAConsts.clBtnFace);
				drawLine(g, 2, thisHeight - 1, thisWidth - 2, thisHeight - 1);
				drawLine(g, thisWidth - 1, 2, thisWidth - 1, thisHeight - 1);
				g.setColor(SDAConsts.clWhite);
				drawLine(g, 1, thisHeight, thisWidth, thisHeight);
				drawLine(g, thisWidth, 1, thisWidth, thisHeight);
				if (isFoucsed() && showFocusRect) {
					g.setColor(SDAConsts.clFocusShadow);
					drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
				}
			}
			if (borderStyle == SDAConsts.bsFixedSingle) {
				// 边框
				g.setColor(getBorderColor());
				drawRect(g, 0, 0, thisWidth, thisHeight);
				if (isFoucsed() && showFocusRect) {
					g.setColor(SDAConsts.clFocusShadow);
					drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
				}
			}
			// 打字
			g.setColor(getForeColor());
			// 文字对齐方式
			if (!isMultiLine()) {
				if (textAlign == SDAConsts.alignLeft) {
					paintString(g, getText(), 2, (height - fontHeight) / 2);
				}
				if (textAlign == SDAConsts.alignCenter) {
					// 文字长度
					int Len = getFont().stringWidth(getText());
					int sleft = (thisWidth - Len) / 2;
					paintString(g, getText(), sleft, (height - fontHeight) / 2);
				}
				if (textAlign == SDAConsts.alignRight) {
					// 文字长度
					int Len = getFont().stringWidth(getText());
					int sleft = (thisWidth - Len) - 1;
					paintString(g, getText(), sleft, (height - fontHeight) / 2);
				}
			} else {
				paintString(g, getText(), 2, (height - fontHeight) / 2);
			}
			// 多行滚动条
			if (borderStyle == SDAConsts.bsFixed3D) {
				SetClip(g, 2, 2, thisWidth - 2, height - 2);
			} else {
				SetClip(g, 1, 1, thisWidth - 1, height - 1);
			}
			if (isMultiLine()) {
				if ((scrollBars == SDAConsts.srHorizontal)
						|| (scrollBars == SDAConsts.srBoth)) {
					// 水平
					g.setColor(getScrollBarColor());
					fillRect(g, 0, height - barwidth, barwidth, barwidth);
					fillRect(g, HBarWidth - barwidth, height - barwidth,
							barwidth, barwidth);
					g.setColor(getBorderColor());
					drawRect(g, 0, thisHeight - barwidth, barwidth, barwidth);
					drawRect(g, HBarWidth - barwidth, height - barwidth,
							barwidth, barwidth);
					// 滚动块
					g.setColor(backColor);
					fillRect(g, barwidth, height - barwidth, HBarWidth - 2
							* barwidth, barwidth);
					g.setColor(getBorderColor());
					drawRect(g, barwidth, thisHeight - barwidth, HBarWidth - 2
							* barwidth, barwidth);
					// 块大小
					int pwidth = HBarWidth - 2 * barwidth;
					if (!isWordWrap()) {
						if (maxLineLenght > HBarWidth) {
							pwidth = (HBarWidth * (HBarWidth - 2 * barwidth))
									/ maxLineLenght;
						}
					}
					// 计算块位置
					int ppos = (startLeft * (HBarWidth - 2 * barwidth))
							/ maxLineLenght;
					// 修正位置
					if (ppos + pwidth > HBarWidth - 2 * barwidth) {
						ppos = HBarWidth - 2 * barwidth - pwidth;
					}
					// 画块
					g.setColor(getScrollBarColor());
					fillRect(g, barwidth + ppos, thisHeight - barwidth, pwidth,
							barwidth);
					g.setColor(getBorderColor());
					drawRect(g, barwidth + ppos, thisHeight - barwidth, pwidth,
							barwidth);
					// 记录大小
					HSLeft = barwidth + ppos;
					HSTop = thisHeight - barwidth;
					HSWidth = pwidth;
					HSHeight = barwidth;
					// 画块上的线
					int tpos = HSWidth / 2 + HSLeft;
					drawRect(g, tpos, HSTop + 2, 0, HSHeight - 4);
					if (tpos - 2 > HSLeft) {
						drawRect(g, tpos - 2, HSTop + 2, 0, HSHeight - 4);
					}
					if (tpos + 2 < HSLeft + HSWidth) {
						drawRect(g, tpos + 2, HSTop + 2, 0, HSHeight - 4);
					}
					// 三角
					g.setColor(getBorderColor());
					fillTriangle(g, barwidth / 2 - 2,
							thisHeight - barwidth / 2, barwidth / 2 + 2,
							thisHeight - barwidth / 2 - 4, barwidth / 2 + 2,
							thisHeight - barwidth / 2 + 4);
					fillTriangle(g, HBarWidth - barwidth + barwidth / 2 + 2,
							thisHeight - barwidth / 2, HBarWidth - barwidth
									+ barwidth / 2 - 2, thisHeight - barwidth
									/ 2 - 4, HBarWidth - barwidth + barwidth
									/ 2 - 2, thisHeight - barwidth / 2 + 4);
				}
				if ((scrollBars == SDAConsts.srVertical)
						|| (scrollBars == SDAConsts.srBoth)) {
					// 垂直
					g.setColor(getScrollBarColor());
					fillRect(g, thisWidth - barwidth, 0, barwidth, barwidth);
					fillRect(g, thisWidth - barwidth, VBarHeight - barwidth,
							barwidth, barwidth);
					g.setColor(getBorderColor());
					drawRect(g, thisWidth - barwidth, 0, barwidth, barwidth);
					drawRect(g, thisWidth - barwidth, VBarHeight - barwidth,
							barwidth, barwidth);
					// 滚动块
					g.setColor(backColor);
					fillRect(g, thisWidth - barwidth, barwidth, barwidth,
							VBarHeight - 2 * barwidth);
					g.setColor(borderColor);
					drawRect(g, thisWidth - barwidth, barwidth, barwidth,
							VBarHeight - 2 * barwidth);
					// 块大小
					int pheight = VBarHeight - 2 * barwidth;
					if (lineNum * getFont().getHeight() > VBarHeight) {
						pheight = (VBarHeight * (VBarHeight - 2 * barwidth) / (lineNum * getFont()
								.getHeight()));
					}
					// 计算块位置
					int ppos = barwidth;
					if (lineNum > 0) {
						ppos = (startLine * getFont().getHeight() * (VBarHeight - 2 * barwidth))
								/ (lineNum * getFont().getHeight());
					}
					// 修正位置
					if (ppos + pheight > VBarHeight - 2 * barwidth) {
						ppos = VBarHeight - 2 * barwidth - pheight;
					}
					// 画块
					g.setColor(getScrollBarColor());
					fillRect(g, thisWidth - barwidth, barwidth + ppos,
							barwidth, pheight);
					g.setColor(getBorderColor());
					drawRect(g, thisWidth - barwidth, barwidth + ppos,
							barwidth, pheight);
					// 记录大小
					VSLeft = thisWidth - barwidth;
					VSTop = barwidth + ppos;
					VSWidth = barwidth;
					VSHeight = pheight;
					// 画块上的线
					int tpos = VSHeight / 2 + VSTop;
					drawRect(g, VSLeft + 2, tpos, barwidth - 4, 0);
					if (tpos - 2 > VSTop) {
						drawRect(g, VSLeft + 2, tpos - 2, VSWidth - 4, 0);
					}
					if (tpos + 2 < VSTop + VSHeight) {
						drawRect(g, VSLeft + 2, tpos + 2, VSWidth - 4, 0);
					}
					// 三角
					g.setColor(getBorderColor());
					fillTriangle(g, thisWidth - barwidth + barwidth / 2,
							barwidth / 2 - 2, thisWidth - barwidth + barwidth
									/ 2 - 4, barwidth / 2 + 2, thisWidth
									- barwidth + barwidth / 2 + 4,
							barwidth / 2 + 2);
					fillTriangle(g, thisWidth - barwidth + barwidth / 2 - 4,
							VBarHeight - barwidth + barwidth / 2 - 2, thisWidth
									- barwidth + barwidth / 2 + 4, VBarHeight
									- barwidth + barwidth / 2 - 2, thisWidth
									- barwidth + barwidth / 2, VBarHeight
									- barwidth / 2 + 2);
				}
			}
			PaintChilds();
		}
	}

	// 获取最大可视行
	private int getMaxVisibleLineNum() {
		return getVBarHeight() / getFont().getHeight();
	}

	// 对单行处理
	private void paintWordWrap(Graphics g, String str, int left) {
		int strLength = str.length();
		int currentWidth = 0;
		int startPoint = 0;
		int id = 0;
		Font ft = getFont();
		for (int i = 0; i < strLength; i++) {
			currentWidth += ft.charWidth(str.charAt(i));
			id++;
			if (currentWidth > maxLineLenght) {
				// 如果cursorCol在最后，应该换行
				if (cursorCol > 0 && cursorCol == id && lineNo == cursorRow) {
					cursorCol = 1;
					cursorRow++;
				}
				// 返回一个字符
				i--;
				id = 0;
				String subStr = str.substring(startPoint, i + 1);
				drawString(g, subStr, left, (lineNo - startLine)
						* getFont().getHeight());
				if (lineNo < cursorRow) {
					cursorLineStartPos += subStr.length();
					priorLineLen = subStr.length();
				}
				if (lineNo == cursorRow) {
					paintCursor(g, 0, subStr);
				}
				startPoint = i + 1;
				currentWidth = 0;
				lineNo++;
			}
		}
		String subStr = str.substring(startPoint, strLength);
		drawString(g, subStr, left, (lineNo - startLine)
				* getFont().getHeight());
		if (lineNo < cursorRow) {
			if (cursorRow < lineNum) {
				cursorLineStartPos += subStr.length() + 1;
			}
			priorLineLen = subStr.length();
		}
		if (lineNo == cursorRow) {
			paintCursor(g, 0, subStr);
		}
		lineNo++;
	}

	// 按指定字符转换
	private String getPassString(String str, char ch) {
		String rstr = "";
		for (int i = 0; i < str.length(); i++) {
			rstr += ch;
		}
		return rstr;
	}

	// 画光标
	private void paintCursor(Graphics g, int left, String lineText) {
		if (isFoucsed()) {
			cursorLineText = lineText;
			int curLeft = 0;
			if (lineText.length() > cursorCol) {
				curLeft = getFont().stringWidth(
						lineText.substring(0, cursorCol));
			} else {
				curLeft = getFont().stringWidth(lineText);
				cursorCol = lineText.length();
			}
			g.setColor(foreColor);
			if (isMultiLine()) {
				drawLine(g, 2 + curLeft - startLeft, getFont().getHeight()
						* (cursorRow - startLine), 2 + curLeft - startLeft,
						getFont().getHeight() * (cursorRow - startLine + 1));
			} else {
				drawLine(g, curLeft - startLeft + left, 0, curLeft - startLeft
						+ left, getFont().getHeight());
			}
		}
	}

	// 获取换行
	private int getChangeLinePos(String text) {
		int intNpos = text.indexOf('\n');
		int intRpos = text.indexOf('\r');
		int pos = -1;
		if (intNpos > -1 && intRpos > -1) {
			if (intNpos > intRpos) {
				pos = intRpos;
			} else {
				pos = intNpos;
			}
		} else {
			if (intNpos > -1) {
				pos = intNpos;
			}
			if (intRpos > -1) {
				pos = intRpos;
			}
		}
		return pos;
	}

	// 打字
	private void paintString(Graphics g, String text, int left, int top) {
		int fontHeight = getFont().getHeight();
		g.setColor(getForeColor());
		if (!isMultiLine()) {
			SetClip(g, 2, 2, width - 4, height - 4);
		} else {
			if (scrollBars == SDAConsts.srHorizontal) {
				SetClip(g, 2, 2, width - 4, height - 4 - barwidth);
			}
			if (scrollBars == SDAConsts.srVertical) {
				SetClip(g, 2, 2, width - 4 - barwidth, height - 4);
			}
			if (scrollBars == SDAConsts.srBoth) {
				SetClip(g, 2, 2, width - 4 - barwidth, height - 4 - barwidth);
			}
		}
		if ((scrollBars == SDAConsts.srVertical)
				|| (scrollBars == SDAConsts.srBoth)) {
			maxLineLenght = width - barwidth;
		} else {
			maxLineLenght = width;
		}
		cursorLineStartPos = 0;
		lineNo = 0;
		if (!isMultiLine()) {
			// 判断密码字符
			String str = (passwordChar == 0) ? text : (getPassString(text,
					passwordChar));
			if (textAlign == SDAConsts.alignLeft) {
				drawString(g, str, left - startLeft, top);
			} else {
				drawString(g, str, left, top);
			}
			paintCursor(g, left, str);
		} else {
			// 如果是空的
			if (text.length() == 0) {
				paintCursor(g, 0, text);
			}
			// 多行的
			if (isWordWrap()) {
				// 长行自动分行，分行符自动到一行
				String str = "";
				int pos = 0;
				while (getChangeLinePos(text) > -1) {
					pos = getChangeLinePos(text);
					str = text.substring(0, pos);
					String str1 = (passwordChar == 0) ? str : (getPassString(
							str, passwordChar));
					paintWordWrap(g, str1, left);
					text = text.substring(pos + 1);
				}
				String str1 = (passwordChar == 0) ? text : (getPassString(text,
						passwordChar));
				paintWordWrap(g, str1, left);
				lineNum = lineNo;
			} else {
				// 不自动换行（根据"\n"换行）
				String str = "";
				lineNo = 0;
				maxLineLenght = 0;
				int pos = 0;
				if (isMultiLine()) {
					while (getChangeLinePos(text) > -1) {
						pos = getChangeLinePos(text);
						str = text.substring(0, pos);
						String str1 = (passwordChar == 0) ? str
								: (getPassString(str, passwordChar));
						if (maxLineLenght < getFont().stringWidth(str1)) {
							maxLineLenght = getFont().stringWidth(str1);
						}
						if (lineNo < cursorRow) {
							cursorLineStartPos += str1.length() + 1;
						}
						if (lineNo == cursorRow) {
							paintCursor(g, 0, str1);
						}
						drawString(g, str1, left - startLeft, top + fontHeight
								* (lineNo - startLine));
						text = text.substring(pos + 1);
						lineNo += 1;
					}
				}
				if (text.length() > 0) {
					String str1 = (passwordChar == 0) ? text : (getPassString(
							text, passwordChar));
					drawString(g, str1, left - startLeft, top + fontHeight
							* (lineNo - startLine));
					if (lineNo < cursorRow) {
						cursorLineStartPos += str1.length() + 1;
					}
					if (lineNo == cursorRow) {
						paintCursor(g, 0, str1);
					}
					lineNo += 1;
					if (maxLineLenght < getFont().stringWidth(str1)) {
						maxLineLenght = getFont().stringWidth(str1);
					}
				}
				lineNum = lineNo;
				maxLineLenght += fontHeight / 2;
			}
		}
		lineNum = lineNo;
	}

	public int getHeight() {
		return internalGetHeight();
	}

	private int internalGetHeight() {
		if (!isMultiLine()) {
			return super.getFont().getHeight() + 3;
		} else {
			return super.getHeight();
		}
	}

	public void setHeight(int height) {
		internalSetHeight(height);
	}

	private void internalSetHeight(int height) {
		// 对于单行不起作用
		if (isMultiLine()) {
			super.setHeight(height);
		} else {
			super.setHeight(getFont().getHeight() + 3);
		}
		internalPaint();
	}

	public int getBorderStyle() {
		return borderStyle;
	}

	public void setBorderStyle(int borderStyle) {
		this.borderStyle = borderStyle;
		internalPaint();
	}

	public boolean isHideSelection() {
		return hideSelection;
	}

	public void setHideSelection(boolean hideSelection) {
		this.hideSelection = hideSelection;
		internalPaint();
	}

	public int getMaxLength() {
		return maxLength;
	}

	public void setMaxLength(int maxLength) {
		this.maxLength = maxLength;
		// 内容处理
		setText(getText());
		doTextChange();
		internalPaint();
	}

	public boolean isMultiLine() {
		return multiLine;
	}

	public void setMultiLine(boolean multiLine) {
		this.multiLine = multiLine;
		internalPaint();
	}

	public char getPasswordChar() {
		return passwordChar;
	}

	public void setPasswordChar(char passwordChar) {
		this.passwordChar = passwordChar;
		repaintControl();
	}

	public boolean isReadOnly() {
		return readOnly;
	}

	public void setReadOnly(boolean readOnly) {
		this.readOnly = readOnly;
	}

	public int getScrollBars() {
		return scrollBars;
	}

	public void setScrollBars(int scrollBars) {
		this.scrollBars = scrollBars;
		repaintControl();
	}

	public int getTextAlign() {
		return textAlign;
	}

	public void setTextAlign(int textAlign) {
		this.textAlign = textAlign;
		repaintControl();
	}

	public boolean isWordWrap() {
		return wordWrap;
	}

	public void setWordWrap(boolean wordWrap) {
		this.wordWrap = wordWrap;
		repaintControl();
	}

	public int getScrollBarWidth() {
		return barwidth;
	}

	public void setScrollBarWidth(int barwidth) {
		this.barwidth = barwidth;
		repaintControl();
	}

	public void setText(String text) {
		internalSetText(text);
	}

	private void internalSetText(String text) {
		// 判断最大长度
		if (text.length() > getMaxLength()) {
			text = text.substring(0, getMaxLength());
		}
		super.setText(text);
		repaintControl();
	}

	public int getBorderColor() {
		return borderColor;
	}

	public void setBorderColor(int borderColor) {
		this.borderColor = borderColor;
		repaintControl();
	}

	public int getScrollBarColor() {
		return scrollBarColor;
	}

	public void setScrollBarColor(int scrollBarColor) {
		this.scrollBarColor = scrollBarColor;
		repaintControl();
	}

	protected boolean isShowFocusRect() {
		return showFocusRect;
	}

	protected void setShowFocusRect(boolean showFocusRect) {
		this.showFocusRect = showFocusRect;
	}

	private int getHBarWidth() {
		int swidth = 0;
		if ((scrollBars == SDAConsts.srHorizontal)
				|| (scrollBars == SDAConsts.srNone)) {
			swidth = getWidth();
		}
		if ((scrollBars == SDAConsts.srBoth)
				|| (scrollBars == SDAConsts.srVertical)) {
			swidth = getWidth() - barwidth;
		}
		if (!isMultiLine()) {
			swidth = getWidth();
		}
		return swidth;
	}

	private int getVBarHeight() {
		int sheight = 0;
		if ((scrollBars == SDAConsts.srVertical)
				|| (scrollBars == SDAConsts.srNone)) {
			sheight = getHeight();
		}
		if ((scrollBars == SDAConsts.srBoth)
				|| (scrollBars == SDAConsts.srHorizontal)) {
			sheight = getHeight() - barwidth;
		}
		if (!isMultiLine()) {
			sheight = getHeight();
		}
		return sheight;
	}

	// 处理事件的执行
	// 点箭头滚动内容
	private void doPointerPressed(int x, int y) {
		int posx = screenXToClient(x);
		int posy = screenYToClient(y);
		int thisWidth = getWidth();
		int thisHeight = getHeight();
		int VBarHeight = getVBarHeight();
		int HBarWidth = getHBarWidth();
		// 确定点击了滚动条区域
		if (isMultiLine()) {
			if ((scrollBars == SDAConsts.srHorizontal)
					|| (scrollBars == SDAConsts.srBoth)) {
				// 判断是否点击了左箭头
				if (InClientRect(posx, posy, 0, thisHeight - barwidth,
						barwidth, barwidth)) {
					// 向右滚动
					if (startLeft > 0) {
						int step = getFont().charWidth('x');
						startLeft -= step;
						if (startLeft < 0) {
							startLeft = 0;
						}
					}
				} else // 右箭头
				if (InClientRect(posx, posy, HBarWidth - barwidth, thisHeight
						- barwidth, barwidth, barwidth)) {
					// 向左滚动
					if (maxLineLenght - startLeft > HBarWidth) {
						int step = getFont().charWidth('x');
						startLeft += step;
					}
				} else // 滚动条
				if (InClientRect(posx, posy, HSLeft, HSTop, HSWidth, HSHeight)) {
					// 记录点击的滚动条位置
					oldScrollPointx = posx;
					oldScrollPointy = posy;
					isscrollbarpointdown = true;
					scrollbardownHV = 0;
					oldStartLeft = startLeft;
				} else {
					if (InClientRect(posx, posy, 0, thisHeight - barwidth,
							HBarWidth, barwidth)) {
						// 点了空白的，滚动到点击的位置
						// 计算滚动块要到位置
						int tpos = posx > HSLeft ? (posx - HSWidth) : (posx);
						// 计算StartLeft
						startLeft = ((tpos - barwidth) * (maxLineLenght))
								/ (HBarWidth - 2 * barwidth);
					}
				}
			}

			if ((scrollBars == SDAConsts.srVertical)
					|| (scrollBars == SDAConsts.srBoth)) {

				// 只有垂直滚动条
				// 判断是否点击了上箭头
				if (InClientRect(posx, posy, thisWidth - barwidth, 0, barwidth,
						barwidth)) {
					// 向下滚动
					if (startLine > 0) {
						startLine--;
					}
				} else // 下箭头
				if (InClientRect(posx, posy, thisWidth - barwidth, VBarHeight
						- barwidth, barwidth, barwidth)) {
					// 向上滚动
					startLine = ((lineNum - startLine) * getFont().getHeight() > VBarHeight) ? startLine + 1
							: startLine;
				} else // 滚动条
				if (InClientRect(posx, posy, VSLeft, VSTop, VSWidth, VSHeight)) {
					// 记录位置
					oldScrollPointx = posx;
					oldScrollPointy = posy;
					isscrollbarpointdown = true;
					scrollbardownHV = 1;
					oldStartLine = startLine;
				} else {
					if (InClientRect(posx, posy, thisWidth - barwidth, 0,
							barwidth, VBarHeight)) {
						// 空白的
						// 计算滚动块要到位置
						int tpos = posy > VSTop ? (posy - VSHeight) : (posy);
						// 计算StartLine
						int oldline = startLine;
						startLine = ((tpos - barwidth) * (lineNum * getFont()
								.getHeight()))
								/ (VBarHeight - 2 * barwidth)
								/ getFont().getHeight();
						if (oldline == startLine) {
							startLine = posy > VSTop ? startLine + 1
									: startLine - 1;
						}
					}
				}
			}
		}
		// 点到空白处，计算光标
		if (InClientRect(posx, posy, 0, 0, getHBarWidth(), getVBarHeight())) {
			calCursorFromXY(posx, posy);
			if (form.Application.inputPanel.charsPanel.isVisible()) {
				form.Application.inputPanel.numChars = null;
				form.Application.inputPanel.inputChars.setLength(0);
				form.Application.inputPanel.charsPanel.visible = false;
				form.repaintControl();
			}
		}
	}

	private void calCursorFromXY(int x, int y) {
		cursorRow = startLine + y / getFont().getHeight();
		if (cursorRow + 1 > lineNum) {
			cursorRow = lineNum - 1;
			if (cursorRow < 0) {
				cursorRow = 0;
			}
		}
		paintString(gg, getText(), 2, -100);
		int len = 0;
		int cw = 0;
		Font ft = getFont();
		String textStr = "";
		if (isMultiLine()) {
			textStr = cursorLineText;
		} else {
			textStr = getText();
		}
		if (passwordChar != 0) {
			textStr = getPassString(textStr, passwordChar);
		}
		cursorCol = textStr.length();
		for (int i = 0; i < textStr.length(); i++) {
			cw = ft.charWidth(textStr.charAt(i));
			if (len + cw / 2 - startLeft > x) {
				cursorCol = i;
				break;
			}
			if (len + cw - startLeft > x) {
				cursorCol = i + 1;
				break;
			}
			len += ft.charWidth(textStr.charAt(i));
		}
	}

	// 拖动事件处理
	private void doPointerReleased(int x, int y) {
		int posx = screenXToClient(x);
		int posy = screenYToClient(y);
		int VBarHeight = getVBarHeight();
		int HBarWidth = getHBarWidth();
		// 根据点击的位置,判断滚动的多少
		if ((scrollBars == SDAConsts.srHorizontal)
				|| ((scrollBars == SDAConsts.srBoth) && (scrollbardownHV == 0))) {
			if (isscrollbarpointdown) {
				int stepx = posx - oldScrollPointx;
				// 根据滚动多少来重新定位
				// 计算滚动块要到位置
				int tpos = HSLeft + stepx;
				HSLeft = tpos < barwidth ? barwidth : tpos;
				HSLeft = HSLeft + HSWidth > HBarWidth - barwidth ? HBarWidth
						- barwidth - HSWidth : HSLeft;
				// 计算StartLeft
				startLeft = ((HSLeft - barwidth) * (maxLineLenght))
						/ (HBarWidth - 2 * barwidth);
				if (oldStartLeft != startLeft) {
					oldScrollPointx = posx;
					oldStartLeft = startLeft;
				} else {
					oldStartLeft = startLeft;
					if (stepx > 0) {
						startLeft = maxLineLenght - startLeft < getHBarWidth() ? startLeft + 1
								: startLeft;
					}
					if (stepx < 0) {
						startLeft = startLeft == 0 ? 0 : startLeft - 1;
					}
				}
			}
		}
		if ((scrollBars == SDAConsts.srVertical)
				|| ((scrollBars == SDAConsts.srBoth) && (scrollbardownHV == 1))) {
			if (isscrollbarpointdown) {
				int stepy = posy - oldScrollPointy;
				// 根据滚动多少来重新定位
				// 计算滚动块要到位置
				int tpos = VSTop + stepy;
				VSTop = tpos < barwidth ? barwidth : tpos;
				VSTop = VSTop + VSHeight > VBarHeight - barwidth ? VBarHeight
						- barwidth - VSHeight : VSTop;
				// 计算StartLine
				startLine = ((VSTop - barwidth) * (lineNum * getFont()
						.getHeight()))
						/ (VBarHeight - 2 * barwidth) / getFont().getHeight();
				if (oldStartLine == startLine) {
					if (stepy > 0) {
						startLine = ((lineNum - startLine)
								* getFont().getHeight() > getVBarHeight()) ? startLine + 1
								: startLine;
					}
					if (stepy < 0) {
						startLine = startLine > 0 ? startLine - 1 : startLine;
					}
					oldStartLine = startLine;
				} else {
					if ((stepy > 0) && (startLine < oldStartLine)) {
						startLine = oldStartLine;
					}
					oldStartLine = startLine;
				}
			}
		}
		isscrollbarpointdown = false;
	}

	protected static byte[] string2Byte(String s) {
		try {
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			DataOutputStream bos = new DataOutputStream(baos);
			bos.writeUTF(s);
			byte[] bytes = baos.toByteArray();
			byte[] rbytes = new byte[bytes.length - 2];
			System.arraycopy(bytes, 2, rbytes, 0, rbytes.length);
			// 关闭流
			bos.close();
			baos.close();
			return rbytes;
		} catch (Exception e) {
			return null;
		}
	}

	// 键盘事件处理
	private void doKeyUp(int keyCode) {
		String key = form.getKeyName(keyCode).toUpperCase();
		char ch = 0;
		if (key.equals(SDAConsts.KEY_UP)) {
			if (form.Application.inputPanel.numChars != null) {
				form.Application.inputPanel.charsPanel.priorChar();
				form.Application.inputPanel.charsPanel.repaintControl();
			} else {
				ch = 38;
				if (form.Application.inputPanel.charsPanel.isVisible()) {
					form.Application.inputPanel.charsPanel.inputFuncChar(ch);
				} else {
					inputFuncChar(ch, false);
				}
			}
		}

		if (key.equals(SDAConsts.KEY_DOWN)) {
			if (form.Application.inputPanel.numChars != null) {
				form.Application.inputPanel.charsPanel.nextChar();
				form.Application.inputPanel.charsPanel.repaintControl();
			} else {
				ch = 40;
				if (form.Application.inputPanel.charsPanel.isVisible()) {
					form.Application.inputPanel.charsPanel.inputFuncChar(ch);
				} else {
					inputFuncChar(ch, false);
				}
			}
		}
		if (key.equals(SDAConsts.KEY_LEFT)) {
			if (form.Application.inputPanel.numChars != null) {
				form.Application.inputPanel.charsPanel.priorChar();
				form.Application.inputPanel.charsPanel.repaintControl();
			} else {
				ch = 37;
				if (form.Application.inputPanel.charsPanel.isVisible()) {
					form.Application.inputPanel.charsPanel.inputFuncChar(ch);
				} else {
					inputFuncChar(ch, false);
				}
			}
		}
		if (key.equals(SDAConsts.KEY_RIGHT)) {
			if (form.Application.inputPanel.numChars != null) {
				form.Application.inputPanel.charsPanel.nextChar();
				form.Application.inputPanel.charsPanel.repaintControl();
			} else {
				ch = 39;
				if (form.Application.inputPanel.charsPanel.isVisible()) {
					form.Application.inputPanel.charsPanel.inputFuncChar(ch);
				} else {
					inputFuncChar(ch, false);
				}
			}
		}
		// 删除（F2）
		if (key.equals(SDAConsts.KEY_SOFT2) || key.equals(SDAConsts.KEY_CLEAR)) {
			if (form.Application.inputPanel.numChars != null) {
				form.Application.inputPanel.numChars = null;
				if (form.Application.inputPanel.inputChars.length() == 0) {
					form.Application.inputPanel.charsPanel.visible = false;
					form.repaintControl();
				}
			} else {
				ch = 8;
				if (form.Application.inputPanel.charsPanel.isVisible()) {
					form.Application.inputPanel.charsPanel.inputFuncChar(ch);
				} else {
					inputFuncChar(ch, false);
				}
			}
		}
		// ESC
		if (key.equals(SDAConsts.KEY_SOFT1)) {
			if (form.Application.inputPanel.charsPanel.visible) {
				form.Application.inputPanel.numChars = null;
				form.Application.inputPanel.inputChars.setLength(0);
				form.Application.inputPanel.charsPanel.visible = false;
				form.repaintControl();
			}
		}
		// System.out.println(keyCode);
		if (key.equals(SDAConsts.KEY_SELECT) || key.equals(SDAConsts.KEY_ENTER)) {
			if (form.Application.inputPanel.numChars != null) {
				if (form.Application.inputPanel.ImeType == SDAInputPanel.imPinYin) {
					ch = form.Application.inputPanel.getKeyChar();
					form.Application.inputPanel.selectedChar = ch;
					form.Application.inputPanel.inputPinYinChar();
					form.Application.inputPanel.charsPanel.repaintControl();
				}
				if (form.Application.inputPanel.ImeType == SDAInputPanel.imLowerCase
						|| form.Application.inputPanel.ImeType == SDAInputPanel.imUpperCase) {
					form.Application.inputPanel.selectedChar = 13;
					form.Application.inputPanel.inputFuncChar();
				}
				form.Application.inputPanel.numChars = null;
			} else {
				ch = 13;
				if (form.Application.inputPanel.charsPanel.isVisible()) {
					form.Application.inputPanel.charsPanel.inputFuncChar(ch);
				} else {
					if (!form.Application.hasPointer()) {
						form.Application.ShowEditor(this);
					} else {
						inputFuncChar(ch, false);
					}
				}
			}

		}
		if ((keyCode > 47 && keyCode < 58) || keyCode == 35 || keyCode == 42) {
			ch = (char) keyCode;
			char[] charArray = null;
			if (form.Application.inputPanel.ImeType == SDAInputPanel.imPinYin) {
				if (form.Application.inputPanel.charsPanel.isInput) {
					charArray = form.Application.inputPanel
							.getLowerKeyChars(ch);
				} else {
					form.Application.inputPanel.charsPanel.isInput = true;
					if (keyCode > 47 && keyCode < 58) {
						form.Application.inputPanel.selectedHanziIndex = Integer
								.parseInt(String.valueOf(ch)) - 1;
						if (!form.Application.inputPanel.charsPanel
								.getHanziByIndex()) {
							form.Application.inputPanel.numChars = null;
							form.Application.inputPanel.charsPanel.visible = false;
							form.Application.inputPanel.inputChars.setLength(0);
							form.Application.inputPanel.hanziChars = null;
							form.repaintControl();
						}
						return;
					}
				}
			}
			if (form.Application.inputPanel.ImeType == SDAInputPanel.imLowerCase) {
				charArray = form.Application.inputPanel.getLowerKeyChars(ch);
			}
			if (form.Application.inputPanel.ImeType == SDAInputPanel.imUpperCase) {
				charArray = form.Application.inputPanel.getUpperKeyChars(ch);
			}
			if (charArray != null) {
				form.Application.inputPanel.numChars = charArray;
				form.Application.inputPanel.setCharsPanelPos();
				form.Application.inputPanel.charsPanel.visible = true;
				form.Application.inputPanel.charsPanel.repaintControl();
			} else {
				if (form.Application.inputPanel.charsPanel.visible) {
					form.Application.inputPanel.numChars = null;
					form.Application.inputPanel.charsPanel.visible = false;
					form.repaintControl();
				}
				inputVisibleChar(ch);
			}
		}
	}

	// 向前删除
	private void backDeleteChar() {
		strBuffer.setLength(0);
		strBuffer.append(getText());
		if (cursorCol > 0) {
			cursorCol--;
			if (cursorLineStartPos + cursorCol < strBuffer.length()) {
				strBuffer.deleteCharAt(cursorLineStartPos + cursorCol);
				setText(strBuffer.toString());
				doTextChange();
			}
			if (cursorCol == 0) {
				if (cursorRow > lineNum - 1) {
					cursorRow = lineNum - 1;
					paintString(gg, getText(), 2, -100);
					cursorCol = cursorLineText.length();
				}
			}
		} else {
			// System.out.println(cursorLineStartPos);
			if (strBuffer.length() > 0
					&& cursorLineStartPos > 0
					&& (strBuffer.charAt(cursorLineStartPos - 1) == '\n' || strBuffer
							.charAt(cursorLineStartPos - 1) == '\r')) {
				strBuffer.deleteCharAt(cursorLineStartPos - 1);
				if (cursorRow > 0) {
					cursorRow--;
					cursorCol = priorLineLen;
					setText(strBuffer.toString());
					doTextChange();
					if (cursorCol < 0) {
						cursorCol = 0;
					}
				}
			} else if (cursorRow > 0) {
				cursorRow--;
				paintString(gg, getText(), 2, -100);
				cursorCol = cursorLineText.length();
			}
		}
	}

	// 向后删除
	private void deleteChar() {
		strBuffer.setLength(0);
		strBuffer.append(getText());
		if (cursorCol < cursorLineText.length()) {
			strBuffer.deleteCharAt(cursorLineStartPos + cursorCol);
			setText(strBuffer.toString());
			doTextChange();
		} else {
			if (cursorLineStartPos + cursorCol < strBuffer.length()) {
				strBuffer.deleteCharAt(cursorLineStartPos + cursorCol);
				setText(strBuffer.toString());
				doTextChange();
			}
		}
	}

	// 输入内容
	public void InputVisibleString(String inString) {
		inputVisibleString(inString);
	}

	protected void inputVisibleString(String inString) {
		if (readOnly) {
			return;
		}
		strBuffer.setLength(0);
		strBuffer.append(getText());
		strBuffer.insert(cursorLineStartPos + cursorCol, inString);
		cursorCol += inString.length();
		setText(strBuffer.toString());
		doTextChange();
		setCursorVisible();
	}

	public void InputVisibleChar(char inChar) {
		inputVisibleChar(inChar);
	}

	protected void inputVisibleChar(char inChar) {
		if (readOnly) {
			return;
		}
		strBuffer.setLength(0);
		strBuffer.append(getText());
		if (cursorLineStartPos + cursorCol > -1
				&& cursorLineStartPos + cursorCol < strBuffer.length() + 1) {
			strBuffer.insert(cursorLineStartPos + cursorCol, inChar);
			cursorCol += 1;
		} else {
			cursorCol = strBuffer.length() + 1 - cursorLineStartPos;
		}
		setText(strBuffer.toString());
		doTextChange();
		setCursorVisible();
	}

	public void InputFuncChar(char inChar, boolean sbc) {
		inputFuncChar(inChar, sbc);
	}

	protected void inputFuncChar(char inChar, boolean sbc) {
		// 左
		if (inChar == 37) {
			if (cursorCol > 0) {
				cursorCol--;
			} else {
				if (cursorRow > 0) {
					cursorRow--;
					paintString(gg, getText(), 2, -100);
					cursorCol = cursorLineText.length();
				}
			}
		}
		// 上
		if (inChar == 38) {
			if (cursorRow > 0) {
				cursorRow--;
			}
		}
		// 右
		if (inChar == 39) {
			if (cursorCol < cursorLineText.length()) {
				cursorCol++;
			} else {
				if (cursorRow < lineNum - 1) {
					cursorRow++;
					paintString(gg, getText(), 2, -100);
					cursorCol = 0;
				}
			}
		}
		// 下
		if (inChar == 40) {
			if (cursorRow < lineNum - 1) {
				cursorRow++;
			}
		}
		// 删除
		if (inChar == 46) {
			if (!readOnly) {
				deleteChar();
			}
		}
		// 退格
		if (inChar == 8) {
			if (!readOnly) {
				backDeleteChar();
			}
		}
		// 回车
		if (inChar == 13) {
			if (!isMultiLine()) {
				return;
			}
			// 全角半角
			if (!readOnly) {
				if (cursorCol > 0) {
					if (!sbc) {
						inputVisibleString("\n  ");
					} else {
						inputVisibleString("\n　　");
					}
					cursorCol = 2;
				} else {
					inputVisibleString("\n");
					cursorCol = 0;
				}
				cursorRow++;
			}
		}
		// 空格
		if (inChar == ' ' || inChar == '　') {
			if (!readOnly) {
				inputVisibleChar(inChar);
			}
		}
		setCursorVisible();
	}

	// 获取当前行
	public int getCursorRow() {
		return cursorRow;
	}

	// 获取当前列
	public int getCursorCol() {
		return cursorCol;
	}

	// 获取当前位置
	public int getCursorX() {
		Font ft = getFont();
		if (isMultiLine()) {
			cursorX = getOriginLeft()
					+ ft.stringWidth(cursorLineText.substring(0, cursorCol))
					- startLeft;
		} else {
			cursorX = getOriginLeft()
					+ ft.stringWidth(getText().substring(0, cursorCol))
					- startLeft;
		}
		return cursorX;
	}

	public int getCursorY() {
		cursorY = getOriginTop() + (cursorRow - startLine)
				* getFont().getHeight();
		return cursorY;
	}

	// 如果当前行不可见，设置为可见区域(startLine,startLeft)
	protected void setCursorVisible() {
		Font ft = getFont();
		if (isMultiLine()) {
			// 行
			if (cursorRow < startLine) {
				startLine = cursorRow;
			}
			if (cursorRow > startLine + getMaxVisibleLineNum() - 1) {
				startLine = cursorRow - getMaxVisibleLineNum() + 1;
			}
		}
		// 列
		int leftWidth = 0;
		int tp = 0;
		leftWidth = ft.stringWidth(cursorLineText.substring(0, cursorCol));
		tp = cursorCol < cursorLineText.length() ? ft.charWidth(cursorLineText
				.charAt(cursorCol)) : 0;
		if (leftWidth - startLeft + tp > width) {
			startLeft += width / 3;
			if (startLeft + width - 6 > ft.stringWidth(cursorLineText)) {
				startLeft = ft.stringWidth(cursorLineText) - width + 6;
			}
		} else if (ft.stringWidth(cursorLineText) < startLeft + width) {
			startLeft = ft.stringWidth(cursorLineText) - width;
			if (startLeft < 0) {
				startLeft = 0;
			}
		}
		if (leftWidth < startLeft) {
			startLeft -= width / 3;
			if (startLeft < 0) {
				startLeft = 0;
			}
		}
		repaintControl();
	}

	protected boolean canDownTabNext() {
		boolean result = true;
		if (form.Application.inputPanel.charsPanel.isVisible()) {
			result = false;
		}
		if (cursorRow < lineNum - 1) {
			result = false;
		}
		return result;
	}

	// 焦点方法处理
	protected boolean canLeftTabPrior() {
		boolean result = true;
		if (form.Application.inputPanel.charsPanel.isVisible()) {
			result = false;
		}
		if (cursorRow > 0 || cursorCol > 0) {
			result = false;
		}
		return result;
	}

	protected boolean canRightTabNext() {
		boolean result = true;
		if (form.Application.inputPanel.charsPanel.isVisible()) {
			result = false;
		}
		if (cursorCol < cursorLineText.length() || cursorRow < lineNum - 1) {
			result = false;
		}
		return result;
	}

	protected boolean canUpTabPrior() {
		boolean result = true;
		if (form.Application.inputPanel.charsPanel.isVisible()) {
			result = false;
		}
		if (cursorRow > 0) {
			result = false;
		}
		return result;
	}

	// 焦点处理
	private void doLostFocus() {
		// 对齐方式
		textAlign = oldAlignType;
		if (form.Application.inputPanel.charsPanel.visible) {
			form.Application.inputPanel.numChars = null;
			form.Application.inputPanel.charsPanel.visible = false;
			form.Application.inputPanel.inputChars.setLength(0);
			form.Application.inputPanel.hanziChars = null;
			form.repaintControl();
		} else {
			repaintControl();
		}
	}

	private void doGetFocus() {
		// 对齐方式(编辑状态左对齐)
		oldAlignType = textAlign;
		textAlign = SDAConsts.alignLeft;
		// 输入法
		if (imeType != SDAInputPanel.imNone) {
			form.Application.inputPanel.setImeType(imeType);
			if (form.Application.inputPanel.visible) {
				form.repaintControl();
			} else {
				if (form.getMainMemu() != null) {
					form.getMainMemu().repaintControl();
				}
			}
		} else {
			repaintControl();
		}
	}

	// 输入法
	public int getImeType() {
		return imeType;
	}

	public void setImeType(int imeType) {
		this.imeType = imeType;
	}

	// 文字变动
	public void setOnTextChange(BaseEditTextChangeEvent onTextChange) {
		this.onTextChange = onTextChange;
	}

	private void doTextChange() {
		if (onTextChange != null) {
			onTextChange.Event(this);
		}
	}
}
