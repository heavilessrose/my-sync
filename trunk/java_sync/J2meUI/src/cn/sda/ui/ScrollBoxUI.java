package cn.sda.ui;

import cn.sda.event.PointerEvent;
import javax.microedition.lcdui.Graphics;

/**
 * <p>
 * Title:
 * </p>
 * 
 * <p>
 * Description:
 * </p>
 * 
 * <p>
 * Copyright: Copyright (c) 2008
 * </p>
 * 
 * <p>
 * Company:
 * </p>
 * 
 * @author not attributable
 * @version 1.0
 */
public class ScrollBoxUI extends BaseControl {
	// 边框
	private int borderStyle = ConstsUI.bsFixedSingle;
	// 滚动边框(多行)
	private int scrollBars = ConstsUI.srBoth;
	private int barwidth = 10;
	// 边框线颜色
	private int borderColor = ConstsUI.clBlack;
	// 滚动条颜色
	private int scrollBarColor = ConstsUI.clBtnFace;
	// 内部变量
	// 行数，行最大长度
	private int maxLenght = 0;
	private int maxHeight = 0;
	// 垂直开始显示位置
	private int startTop = 0;
	// 水平开始显示位置
	private int startLeft = 0;
	// 变动前位置
	private int oldStartLine = 0;
	private int oldStartLeft = 0;
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
	// 子组件参照组件
	private BaseControl refLeftControl = null;
	private BaseControl refTopControl = null;
	// 容器
	private Bevel backPanel = null;

	public ScrollBoxUI() {
		super();
		backColor = ConstsUI.clWhite;
		backPanel = new Bevel();
		backPanel.setShape(ConstsUI.bvSpacer);
		backPanel.setStyle(ConstsUI.pbNone);
		backPanel.setBackColor(backColor);
		super.AddControl(backPanel);
		internalSDAScrollBox();
	}

	private void internalSDAScrollBox() {
		this.setVisible(true);
		setLeft(0);
		setTop(0);
		setBorderStyle(ConstsUI.bsFixedSingle);
		super.setWidth(160);
		setHeight(100);
		super.setBackColor(ConstsUI.clWhite);
		this.onPointerPressed = new PointerEvent() {

			public void Event(BaseControl ctrl, int x, int y) {
				doPointerPressed(x, y);
			}
		};
		this.onPointerReleased = new PointerEvent() {

			public void Event(BaseControl ctrl, int x, int y) {
				doPointerReleased(x, y);
			}
		};
	}

	// 画
	public void paint() {
		internalPaint();
	}

	private void internalPaint() {
		if (!IsCanPaint()) {
			return;
		}
		Graphics g = form.getGraphics();
		g.setFont(getFont());
		InternalPaint(g);
		PaintChilds();
	}

	protected void InternalPaint(Graphics g) {
		// 可视下才画
		if (isVisible()) {
			// 初始化引用变量
			int thisWidth = getWidth();
			int thisHeight = getHeight();
			int HBarWidth = getHBarWidth();
			int VBarHeight = getVBarHeight();
			// 对齐方式处理后，画组件
			// 设置字体
			g.setFont(super.getFont());
			// 外框
			SetClip(g);
			// 透明
			if (!isTransparent()) {
				g.setColor(backColor);
				fillRect(g, 0, 0, thisWidth, thisHeight);
			}
			if (borderStyle == ConstsUI.bsFixed3D) {
				g.setColor(ConstsUI.clGray);
				drawLine(g, 0, 0, thisWidth, 0);
				drawLine(g, 0, 0, 0, thisHeight);
				g.setColor(ConstsUI.clBlack);
				drawLine(g, 1, 1, thisWidth - 1, 1);
				drawLine(g, 1, 1, 1, thisHeight - 1);
				g.setColor(ConstsUI.clBtnFace);
				drawLine(g, 2, thisHeight - 1, thisWidth - 2, thisHeight - 1);
				drawLine(g, thisWidth - 1, 2, thisWidth - 1, thisHeight - 1);
				g.setColor(ConstsUI.clWhite);
				drawLine(g, 1, thisHeight, thisWidth, thisHeight);
				drawLine(g, thisWidth, 1, thisWidth, thisHeight);
				if (isFoucsed()) {
					g.setColor(ConstsUI.clFocusShadow);
					drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
				}
			}
			if (borderStyle == ConstsUI.bsFixedSingle) {
				// 边框
				g.setColor(this.getBorderColor());
				drawRect(g, 0, 0, thisWidth, thisHeight);
				if (isFoucsed()) {
					g.setColor(ConstsUI.clFocusShadow);
					drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
				}
			}
			// 子组件
			setChildsPos();
			// 滚动条
			if (borderStyle == ConstsUI.bsFixed3D) {
				SetClip(g, 2, 2, thisWidth - 2, thisHeight - 2);
			} else {
				SetClip(g, 1, 1, thisWidth - 1, thisHeight - 1);
			}
			if ((scrollBars == ConstsUI.srHorizontal)
					|| (scrollBars == ConstsUI.srBoth)) {
				// 水平
				g.setColor(scrollBarColor);
				fillRect(g, 0, thisHeight - barwidth, barwidth, barwidth);
				fillRect(g, HBarWidth - barwidth, thisHeight - barwidth,
						barwidth, barwidth);
				g.setColor(getBorderColor());
				drawRect(g, 0, thisHeight - barwidth, barwidth, barwidth);
				drawRect(g, HBarWidth - barwidth, thisHeight - barwidth,
						barwidth, barwidth);
				// 滚动块
				g.setColor(backColor);
				fillRect(g, barwidth, thisHeight - barwidth, HBarWidth - 2
						* barwidth, barwidth);
				g.setColor(this.getBorderColor());
				drawRect(g, barwidth, thisHeight - barwidth, HBarWidth - 2
						* barwidth, barwidth);
				// 块大小
				int pwidth = HBarWidth - 2 * barwidth;

				if (maxLenght > HBarWidth) {
					pwidth = ((HBarWidth) * (HBarWidth - 2 * barwidth))
							/ maxLenght;
				}

				// 计算块位置
				int ppos = (startLeft * (HBarWidth - 2 * barwidth)) / maxLenght;
				// 修正位置
				if (ppos + pwidth > HBarWidth - 2 * barwidth) {
					ppos = HBarWidth - 2 * barwidth - pwidth;
				}
				// 画块
				g.setColor(scrollBarColor);
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
				fillTriangle(g, barwidth / 2 - 2, thisHeight - barwidth / 2,
						barwidth / 2 + 2, thisHeight - barwidth / 2 - 4,
						barwidth / 2 + 2, thisHeight - barwidth / 2 + 4);
				fillTriangle(g, HBarWidth - barwidth + barwidth / 2 + 2,
						thisHeight - barwidth / 2, HBarWidth - barwidth
								+ barwidth / 2 - 2, thisHeight - barwidth / 2
								- 4, HBarWidth - barwidth + barwidth / 2 - 2,
						thisHeight - barwidth / 2 + 4);
			}
			if ((scrollBars == ConstsUI.srVertical)
					|| (scrollBars == ConstsUI.srBoth)) {
				// 垂直
				g.setColor(scrollBarColor);
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
				g.setColor(getBorderColor());
				drawRect(g, thisWidth - barwidth, barwidth, barwidth,
						VBarHeight - 2 * barwidth);
				// 块大小
				int pheight = VBarHeight - 2 * barwidth;
				if (maxHeight > VBarHeight) {
					pheight = ((VBarHeight) * (VBarHeight - 2 * barwidth) / maxHeight);
				}
				// 计算块位置
				int ppos = barwidth;
				if (maxHeight > 0) {
					ppos = (startTop * (VBarHeight - 2 * barwidth)) / maxHeight;
				}
				// 修正位置
				if (ppos + pheight > VBarHeight - 2 * barwidth) {
					ppos = VBarHeight - 2 * barwidth - pheight;
				}
				// 画块
				g.setColor(scrollBarColor);
				fillRect(g, thisWidth - barwidth, barwidth + ppos, barwidth,
						pheight);
				g.setColor(getBorderColor());
				drawRect(g, thisWidth - barwidth, barwidth + ppos, barwidth,
						pheight);
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
						barwidth / 2 - 2, thisWidth - barwidth + barwidth / 2
								- 4, barwidth / 2 + 2, thisWidth - barwidth
								+ barwidth / 2 + 4, barwidth / 2 + 2);
				fillTriangle(g, thisWidth - barwidth + barwidth / 2 - 4,
						VBarHeight - barwidth + barwidth / 2 - 2, thisWidth
								- barwidth + barwidth / 2 + 4, VBarHeight
								- barwidth + barwidth / 2 - 2, thisWidth
								- barwidth + barwidth / 2, VBarHeight
								- barwidth / 2 + 2);
			}
			PaintChilds();
		}
	}

	// 对子组件重新定位
	private void setChildsPos() {
		// 设置容器位置
		backPanel.left = 2;
		backPanel.top = 2;
		backPanel.width = this.getHBarWidth() - 4;
		backPanel.height = this.getVBarHeight() - 4;
		backPanel.setBackColor(backColor);
		backPanel.setTransparent(this.transparent);
		// 最大信息
		getMaxInfo();
		BaseControl ctrl = null;
		// 全部重新定位
		for (int i = 0; i < backPanel.ctrlList.size(); i++) {
			ctrl = (BaseControl) backPanel.ctrlList.elementAt(i);
			if (!ctrl.visible) {
				continue;
			}
			if (ctrl.dock == ConstsUI.dsNone) {
				if (!ctrl.equals(refLeftControl)) {
					ctrl.left = (ctrl.left - refLeftControl.left) - startLeft;
				}
				if (!ctrl.equals(refTopControl)) {
					ctrl.top = (ctrl.top - refTopControl.top) - startTop;
				}
			}
		}
		refLeftControl.left = -startLeft;
		refTopControl.top = -startTop;
	}

	public final void AddControl(BaseControl control) {
		backPanel.AddControl(control);
	}

	// 获取最大长度和高度
	private void getMaxInfo() {
		int minLeft = Integer.MAX_VALUE;
		int minTop = Integer.MAX_VALUE;
		int maxRight = Integer.MIN_VALUE;
		int maxBottom = Integer.MIN_VALUE;
		BaseControl ctrl = null;
		for (int i = 0; i < backPanel.ctrlList.size(); i++) {
			ctrl = (BaseControl) backPanel.ctrlList.elementAt(i);
			if (!ctrl.visible) {
				continue;
			}
			if (ctrl.getLeft() < minLeft) {
				minLeft = ctrl.getLeft();
				refLeftControl = ctrl;
			}
			maxRight = ctrl.getLeft() + ctrl.getWidth() > maxRight ? ctrl
					.getLeft()
					+ ctrl.getWidth() : maxRight;
			if (ctrl.getTop() < minTop) {
				minTop = ctrl.getTop();
				refTopControl = ctrl;
			}
			maxBottom = ctrl.getTop() + ctrl.getHeight() > maxBottom ? ctrl
					.getTop()
					+ ctrl.getHeight() : maxBottom;
		}
		maxHeight = maxBottom - minTop + 4;
		maxLenght = maxRight - minLeft + 4;
	}

	public int getBorderStyle() {
		return borderStyle;
	}

	public void setBorderStyle(int borderStyle) {
		this.borderStyle = borderStyle;
		internalPaint();
	}

	public int getScrollBars() {
		return scrollBars;
	}

	public void setScrollBars(int scrollBars) {
		this.scrollBars = scrollBars;
		internalPaint();
	}

	public int getScrollBarWidth() {
		return barwidth;
	}

	public void setScrollBarWidth(int barwidth) {
		this.barwidth = barwidth;
		internalPaint();
	}

	public int getBorderColor() {
		return borderColor;
	}

	public void setBorderColor(int borderColor) {
		this.borderColor = borderColor;
		internalPaint();
	}

	public int getScrollBarColor() {
		return scrollBarColor;
	}

	public void setScrollBarColor(int scrollBarColor) {
		this.scrollBarColor = scrollBarColor;
		internalPaint();
	}

	private int getHBarWidth() {
		int swidth = 0;
		if ((scrollBars == ConstsUI.srHorizontal)
				|| (scrollBars == ConstsUI.srNone)) {
			swidth = getWidth();
		}
		if ((scrollBars == ConstsUI.srBoth)
				|| (scrollBars == ConstsUI.srVertical)) {
			swidth = getWidth() - barwidth;
		}
		return swidth;
	}

	private int getVBarHeight() {
		int sheight = 0;
		if ((scrollBars == ConstsUI.srVertical)
				|| (scrollBars == ConstsUI.srNone)) {
			sheight = getHeight();
		}
		if ((scrollBars == ConstsUI.srBoth)
				|| (scrollBars == ConstsUI.srHorizontal)) {
			sheight = getHeight() - barwidth;
		}
		return sheight;
	}

	// 处理事件的执行
	// 点箭头滚动内容
	protected void doPointerPressed(int x, int y) {
		int posx = screenXToClient(x);
		int posy = screenYToClient(y);
		int thisWidth = getWidth();
		int thisHeight = getHeight();
		int VBarHeight = getVBarHeight();
		int HBarWidth = getHBarWidth();
		// 确定点击了滚动条区域
		if ((scrollBars == ConstsUI.srHorizontal)
				|| (scrollBars == ConstsUI.srBoth)) {
			// 判断是否点击了左箭头
			if (InClientRect(posx, posy, 0, thisHeight - barwidth, barwidth,
					barwidth)) {
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
				if (maxLenght - startLeft > HBarWidth) {
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
					startLeft = ((tpos - barwidth) * (maxLenght))
							/ (HBarWidth - 2 * barwidth);
				}
			}
		}

		if ((scrollBars == ConstsUI.srVertical)
				|| (scrollBars == ConstsUI.srBoth)) {

			// 只有垂直滚动条
			// 判断是否点击了上箭头
			if (InClientRect(posx, posy, thisWidth - barwidth, 0, barwidth,
					barwidth)) {
				// 向下滚动
				if (startTop > 0) {
					startTop--;
				}
			} else // 下箭头
			if (InClientRect(posx, posy, thisWidth - barwidth, VBarHeight
					- barwidth, barwidth, barwidth)) {
				// 向上滚动
				startTop = (maxHeight - startTop > VBarHeight) ? startTop + 1
						: startTop;
			} else // 滚动条
			if (InClientRect(posx, posy, VSLeft, VSTop, VSWidth, VSHeight)) {
				// 记录位置
				oldScrollPointx = posx;
				oldScrollPointy = posy;
				isscrollbarpointdown = true;
				scrollbardownHV = 1;
				oldStartLine = startTop;
			} else {
				if (InClientRect(posx, posy, thisWidth - barwidth, 0, barwidth,
						VBarHeight)) {
					// 空白的
					// 计算滚动块要到位置
					int tpos = posy > VSTop ? (posy - VSHeight) : (posy);
					// 计算StartLine
					int oldline = startTop;
					startTop = ((tpos - barwidth) * maxHeight)
							/ (VBarHeight - 2 * barwidth);
					if (oldline == startTop) {
						startTop = posy > VSTop ? startTop + 1 : startTop - 1;
					}
				}

			}
		}
	}

	// 拖动事件处理
	protected void doPointerReleased(int x, int y) {
		int posx = screenXToClient(x);
		int posy = screenYToClient(y);
		int VBarHeight = getVBarHeight();
		int HBarWidth = getHBarWidth();
		// 根据点击的位置,判断滚动的多少
		if ((scrollBars == ConstsUI.srHorizontal)
				|| ((scrollBars == ConstsUI.srBoth) && (scrollbardownHV == 0))) {
			if (isscrollbarpointdown) {
				int stepx = posx - oldScrollPointx;
				// 根据滚动多少来重新定位
				// 计算滚动块要到位置
				int tpos = HSLeft + stepx;
				HSLeft = tpos < barwidth ? barwidth : tpos;
				HSLeft = HSLeft + HSWidth > HBarWidth - barwidth ? HBarWidth
						- barwidth - HSWidth : HSLeft;
				// 计算StartLeft
				startLeft = ((HSLeft - barwidth) * (maxLenght))
						/ (HBarWidth - 2 * barwidth);
				if (oldStartLeft != startLeft) {
					oldScrollPointx = posx;
					oldStartLeft = startLeft;
				} else {
					oldStartLeft = startLeft;
					if (stepx > 0) {
						startLeft = maxLenght - startLeft < getHBarWidth() ? startLeft + 1
								: startLeft;
					}
					if (stepx < 0) {
						startLeft = startLeft == 0 ? 0 : startLeft - 1;
					}
				}
			}
		}
		if ((scrollBars == ConstsUI.srVertical)
				|| ((scrollBars == ConstsUI.srBoth) && (scrollbardownHV == 1))) {
			if (isscrollbarpointdown) {
				int stepy = posy - oldScrollPointy;
				// 根据滚动多少来重新定位
				// 计算滚动块要到位置
				int tpos = VSTop + stepy;
				VSTop = tpos < barwidth ? barwidth : tpos;
				VSTop = VSTop + VSHeight > VBarHeight - barwidth ? VBarHeight
						- barwidth - VSHeight : VSTop;
				// 计算StartLine
				startTop = ((VSTop - barwidth) * (maxHeight * getFont()
						.getHeight()))
						/ (VBarHeight - 2 * barwidth) / getFont().getHeight();
				if (oldStartLine == startTop) {
					if (stepy > 0) {
						startTop = ((maxHeight - startTop)
								* getFont().getHeight() > getVBarHeight()) ? startTop + 1
								: startTop;
					}
					if (stepy < 0) {
						startTop = startTop > 0 ? startTop - 1 : startTop;
					}
					oldStartLine = startTop;
				} else {
					if ((stepy > 0) && (startTop < oldStartLine)) {
						startTop = oldStartLine;
					}
					oldStartLine = startTop;
				}
			}
		}
		isscrollbarpointdown = false;
	}
}
