package cn.sda.ui;

import cn.sda.event.KeybordEvent;
import cn.sda.event.PointerEvent;
import cn.sda.event.ScrollBarChangeEvent;
import javax.microedition.lcdui.Graphics;

/**
 * 
 * @author Administrator
 */
public class SDAScrollBar extends SDABaseControl {

	private byte kind = SDAConsts.ktHorizontal;
	private int borderColor = SDAConsts.clBlack;
	private int arrowWidth = 15;
	private int arrowBackColor = SDAConsts.clBtnFace;
	private int maxValue = 100;
	private int minValue = 0;
	private int value = 0;
	private int increment = 1;
	private int thumbWidth = 15;
	// 点击的滚动条的位置
	private int oldScrollPointx = 0;
	private int oldScrollPointy = 0;
	// 按下滚动条
	private boolean isscrollbarpointdown = false;
	// 滚动事件
	private ScrollBarChangeEvent onScrollBarChange = null;

	public SDAScrollBar() {
		super();
		visible = true;
		width = 100;
		height = 15;
		backColor = SDAConsts.clWhite;
		setOnKeyUp(new KeybordEvent() {

			public void Event(SDABaseControl ctrl, int keyCode) {
				// 键盘
				doKeyUp(keyCode);
			}
		});
		setOnPointerPressed(new PointerEvent() {

			public void Event(SDABaseControl ctrl, int x, int y) {
				// 点
				doPointerPress(x, y);
			}
		});
		setOnPointerReleased(new PointerEvent() {

			public void Event(SDABaseControl ctrl, int x, int y) {
				// 抬起
				doPointerReleased(x, y);
			}
		});
	}

	public byte getKind() {
		return kind;
	}

	public void setKind(byte kind) {
		this.kind = kind;
		paint();
	}

	public int getArrowBackColor() {
		return arrowBackColor;
	}

	public void setArrowBackColor(int arrowBackColor) {
		this.arrowBackColor = arrowBackColor;
		paint();
	}

	public int getArrowWidth() {
		return arrowWidth;
	}

	public void setArrowWidth(int arrowWidth) {
		this.arrowWidth = arrowWidth;
		paint();
	}

	public int getBorderColor() {
		return borderColor;
	}

	public void setBorderColor(int borderColor) {
		this.borderColor = borderColor;
		paint();
	}

	public int getIncrement() {
		return increment;
	}

	public void setIncrement(int increment) {
		this.increment = increment;
	}

	public int getMaxValue() {
		return maxValue;
	}

	public void setMaxValue(int maxValue) {
		if (maxValue > minValue) {
			this.maxValue = maxValue;
			setValue(value);
		}
	}

	public int getMinValue() {
		return minValue;
	}

	public void setMinValue(int minValue) {
		if (minValue < maxValue) {
			this.minValue = minValue;
			setValue(value);
		}
	}

	public int getValue() {
		return value;
	}

	public void setValue(int value) {
		if (value > maxValue) {
			value = maxValue;
		}
		if (value < minValue) {
			value = minValue;
		}
		this.value = value;
		paint();
	}

	public int getThumbWidth() {
		return thumbWidth;
	}

	public void setThumbWidth(int thumbWidth) {
		this.thumbWidth = thumbWidth;
		paint();
	}

	public void paint() {
		if (IsCanPaint()) {
			internalPaint();
		}
	}

	private void internalPaint() {
		// 画
		Graphics g = form.getGraphics();
		g.setFont(getFont());
		SetClip(g);
		// 框
		drawControlRect(g);
		PaintChilds();
	}

	private void drawControlRect(Graphics g) {
		int thisHeight = getHeight();
		int thisWidth = getWidth();
		g.setColor(backColor);
		fillRect(g, 0, 0, thisWidth, thisHeight);
		g.setColor(borderColor);
		drawRect(g, 0, 0, thisWidth, thisHeight);
		g.setColor(arrowBackColor);
		if (kind == SDAConsts.ktHorizontal) {
			// 两边的框
			fillRect(g, 0, 0, arrowWidth, thisHeight);
			fillRect(g, thisWidth - arrowWidth, 0, arrowWidth, thisHeight);
			g.setColor(borderColor);
			drawRect(g, 0, 0, thisWidth, thisHeight);
			drawRect(g, 0, 0, arrowWidth, thisHeight);
			drawRect(g, thisWidth - arrowWidth, 0, arrowWidth, thisHeight);
			// 左箭头
			g.setColor(borderColor);
			fillTriangle(g, arrowWidth / 2 - 2, thisHeight / 2,
					arrowWidth / 2 + 2, thisHeight / 2 - 4, arrowWidth / 2 + 2,
					thisHeight / 2 + 4);
			// 右箭头
			fillTriangle(g, thisWidth - arrowWidth / 2 - 2, thisHeight / 2 - 4,
					thisWidth - arrowWidth / 2 - 2, thisHeight / 2 + 4,
					thisWidth - arrowWidth / 2 + 2, thisHeight / 2);
			// 中间的滚动条
			int thumbLeft = (value - minValue)
					* (thisWidth - 2 * arrowWidth - thumbWidth)
					/ (maxValue - minValue) + arrowWidth;
			g.setColor(arrowBackColor);
			fillRect(g, thumbLeft, 0, thumbWidth, thisHeight);
			g.setColor(borderColor);
			drawRect(g, thumbLeft, 0, thumbWidth, thisHeight);
			// 竖线
			drawLine(g, thumbLeft + thumbWidth / 2 - 2, 2, thumbLeft
					+ thumbWidth / 2 - 2, thisHeight - 2);
			drawLine(g, thumbLeft + thumbWidth / 2, 2, thumbLeft + thumbWidth
					/ 2, thisHeight - 2);
			drawLine(g, thumbLeft + thumbWidth / 2 + 2, 2, thumbLeft
					+ thumbWidth / 2 + 2, thisHeight - 2);
		} else {
			// 两边的框
			fillRect(g, 0, 0, thisWidth, arrowWidth);
			fillRect(g, 0, thisHeight - arrowWidth, thisWidth, arrowWidth);
			g.setColor(borderColor);
			drawRect(g, 0, 0, thisWidth, thisHeight);
			drawRect(g, 0, 0, thisWidth, arrowWidth);
			drawRect(g, 0, thisHeight - arrowWidth, thisWidth, arrowWidth);
			// 左箭头
			g.setColor(borderColor);
			fillTriangle(g, thisWidth / 2, arrowWidth / 2 - 2,
					thisWidth / 2 - 4, arrowWidth / 2 + 2, thisWidth / 2 + 4,
					arrowWidth / 2 + 2);
			// 右箭头
			fillTriangle(g, thisWidth / 2 - 4, thisHeight - arrowWidth / 2 - 2,
					thisWidth / 2 + 4, thisHeight - arrowWidth / 2 - 2,
					thisWidth / 2, thisHeight - arrowWidth / 2 + 2);
			// 中间的滚动条
			int thumbTop = (value - minValue)
					* (thisHeight - 2 * arrowWidth - thumbWidth)
					/ (maxValue - minValue) + arrowWidth;
			g.setColor(arrowBackColor);
			fillRect(g, 0, thumbTop, thisWidth, thumbWidth);
			g.setColor(borderColor);
			drawRect(g, 0, thumbTop, thisWidth, thumbWidth);
			// 竖线
			drawLine(g, 2, thumbTop + thumbWidth / 2 - 2, thisWidth - 2,
					thumbTop + thumbWidth / 2 - 2);
			drawLine(g, 2, thumbTop + thumbWidth / 2, thisWidth - 2, thumbTop
					+ thumbWidth / 2);
			drawLine(g, 2, thumbTop + thumbWidth / 2 + 2, thisWidth - 2,
					thumbTop + thumbWidth / 2 + 2);
		}
		// 焦点
		if (isFoucsed()) {
			g.setColor(SDAConsts.clFocusShadow);
			drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
		}
	}

	private void doKeyUp(int keyCode) {
		String key = form.getKeyName(keyCode).toUpperCase();
		if ((key.equals(SDAConsts.KEY_LEFT) && (kind == SDAConsts.ktHorizontal))
				|| (key.equals(SDAConsts.KEY_UP) && (kind == SDAConsts.ktVertical))) {
			// 左
			if (value > minValue) {
				value -= increment;
				doScrollChange();
			}
		}
		if ((key.equals(SDAConsts.KEY_RIGHT) && (kind == SDAConsts.ktHorizontal))
				|| (key.equals(SDAConsts.KEY_DOWN) && (kind == SDAConsts.ktVertical))) {
			// 右
			if (value < maxValue) {
				value += increment;
				doScrollChange();
			}
		}
		repaintControl();
	}

	private void doPointerPress(int x, int y) {
		int posx = screenXToClient(x);
		int posy = screenYToClient(y);
		int thisHeight = getHeight();
		int thisWidth = getWidth();
		if (InClientRect(posx, posy, 0, 0, thisWidth, thisHeight)) {
			if (InClientRect(posx, posy, 0, 0,
					kind == SDAConsts.ktHorizontal ? arrowWidth : thisWidth,
					kind == SDAConsts.ktHorizontal ? thisHeight : arrowWidth)) {
				if (value > minValue) {
					value -= increment;
					doScrollChange();
				}
			} else if (InClientRect(
					posx,
					posy,
					kind == SDAConsts.ktHorizontal ? thisWidth - arrowWidth : 0,
					kind == SDAConsts.ktHorizontal ? 0 : thisHeight
							- arrowWidth,
					kind == SDAConsts.ktHorizontal ? arrowWidth : thisWidth,
					kind == SDAConsts.ktHorizontal ? thisHeight : arrowWidth)) {
				if (value < maxValue) {
					value += increment;
					doScrollChange();
				}
			} else {
				if (kind == SDAConsts.ktHorizontal) {
					int thumbLeft = (value - minValue)
							* (thisWidth - 2 * arrowWidth - thumbWidth)
							/ (maxValue - minValue) + arrowWidth;
					if (InClientRect(posx, posy, thumbLeft, 0, thumbWidth,
							thisHeight)) {
						// 记录
						oldScrollPointx = posx;
						oldScrollPointy = posy;
						isscrollbarpointdown = true;
					} else {
						// 空白处，滚动
						if (posx < thumbLeft) {
							thumbLeft = posx;
						} else {
							thumbLeft = posx - thumbWidth;
						}
						value = (thumbLeft - arrowWidth)
								* (maxValue - minValue)
								/ (thisWidth - 2 * arrowWidth - thumbWidth)
								+ minValue;
						doScrollChange();
					}
				} else {
					int thumbTop = (value - minValue)
							* (thisHeight - 2 * arrowWidth - thumbWidth)
							/ (maxValue - minValue) + arrowWidth;
					if (InClientRect(posx, posy, 0, thumbTop, thisWidth,
							thumbWidth)) {
						// 记录
						oldScrollPointx = posx;
						oldScrollPointy = posy;
						isscrollbarpointdown = true;
					} else {
						// 空白处
						// 空白处，滚动
						if (posy < thumbTop) {
							thumbTop = posy;
						} else {
							thumbTop = posy - thumbWidth;
						}
						value = (thumbTop - arrowWidth) * (maxValue - minValue)
								/ (thisHeight - 2 * arrowWidth - thumbWidth)
								+ minValue;
						doScrollChange();
					}
				}
			}
		}
	}

	private void doPointerReleased(int x, int y) {
		int posx = screenXToClient(x);
		int posy = screenYToClient(y);
		int thisHeight = getHeight();
		int thisWidth = getWidth();
		if (kind == SDAConsts.ktHorizontal) {
			if (isscrollbarpointdown) {
				int stepx = posx - oldScrollPointx;
				value = stepx * (maxValue - minValue)
						/ (thisWidth - 2 * arrowWidth - thumbWidth) + value;
				if (value < minValue) {
					value = minValue;
				}
				if (value > maxValue) {
					value = maxValue;
				}
				doScrollChange();
			}
		}
		if (kind == SDAConsts.ktVertical) {
			if (isscrollbarpointdown) {
				int stepy = posy - oldScrollPointy;
				value = stepy * (maxValue - minValue)
						/ (thisHeight - 2 * arrowWidth - thumbWidth) + value;
				if (value < minValue) {
					value = minValue;
				}
				if (value > maxValue) {
					value = maxValue;
				}
				doScrollChange();
			}
		}
		repaintControl();
		isscrollbarpointdown = false;
	}

	private void doScrollChange() {
		if (this.onScrollBarChange != null) {
			onScrollBarChange.Event(value);
		}
	}

	public void setOnScrollBarChange(ScrollBarChangeEvent onScrollBarChange) {
		this.onScrollBarChange = onScrollBarChange;
	}

	protected boolean canDownTabNext() {
		if (kind == SDAConsts.ktVertical) {
			return false;
		} else {
			return true;
		}
	}

	protected boolean canLeftTabPrior() {
		if (kind == SDAConsts.ktHorizontal) {
			return false;
		} else {
			return true;
		}
	}

	protected boolean canRightTabNext() {
		if (kind == SDAConsts.ktHorizontal) {
			return false;
		} else {
			return true;
		}
	}

	protected boolean canUpTabPrior() {
		if (kind == SDAConsts.ktVertical) {
			return false;
		} else {
			return true;
		}
	}
}
