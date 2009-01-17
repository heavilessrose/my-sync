package cn.sda.ui;

import java.util.*;

import javax.microedition.lcdui.*;

import cn.sda.event.*;

/**
 * @author not attributable
 * @version 1.0
 */
public abstract class SDABaseControl {

	protected SDABaseControl parent = null;
	protected SDAForm form;
	protected Vector ctrlList = new Vector();
	protected String name;
	protected boolean tabStop = true;
	protected boolean down;
	protected int left;
	protected int top;
	protected int width;
	protected int height;
	private int originLeft, originTop;
	protected int foreColor;
	protected int backColor;
	protected boolean visible;
	protected boolean enabled;
	public boolean ctl3d;
	protected String text;
	protected int fontFace, fontStyle, fontSize;
	protected boolean parentFont;
	protected boolean transparent;
	protected int alignment;
	// 对齐方式
	protected int dock = SDAConsts.dsNone;
	// event
	protected NotifyEvent onClick;
	protected KeybordEvent onKeyDown;
	protected KeybordEvent onKeyUp;
	protected KeybordEvent onKeyPress;
	protected PointerEvent onPointerPressed;
	protected PointerEvent onPointerReleased;
	protected PointerEvent onPointerDragged;
	protected NotifyEvent onFocused, onInternalFocused;
	protected NotifyEvent onLostFocused, onInternalLostFocused;
	// 对齐方式的设置的记录列表
	private Vector AlignmentList = null;

	private class Align {

		public SDABaseControl referControl = null;
		public SDABaseControl[] arrayControl = null;
		public int alignType = -1;
		public int paramValue = 0;
	}

	public SDABaseControl() {
		this.name = "";
		this.onClick = null;
		this.onKeyDown = null;
		this.onKeyUp = null;
		this.onKeyPress = null;
		this.onPointerPressed = null;
		this.onPointerReleased = null;
		this.onPointerPressed = null;
		this.left = 0;
		this.top = 0;
		this.originLeft = 0;
		this.originTop = 0;
		this.visible = true;
		this.alignment = SDAConsts.alignCenter;
		this.ctl3d = false;
		this.enabled = true;
		this.tabStop = true;
		this.backColor = SDAConsts.clBtnFace;
		this.foreColor = SDAConsts.clBlack;
		this.parentFont = true;
		this.transparent = false;
		this.text = "";
		this.fontFace = Font.getDefaultFont().getFace();
		this.fontStyle = Font.getDefaultFont().getStyle();
		// this.fontSize = Font.getDefaultFont().getSize();
		this.fontSize = Font.SIZE_SMALL;
		// 对齐管理
		AlignmentList = new Vector();
	}

	protected boolean canLeftTabPrior() {
		return true;
	}

	protected boolean canDownTabNext() {
		return true;
	}

	protected boolean canUpTabPrior() {
		return true;
	}

	protected boolean canRightTabNext() {
		return true;
	}

	public final void SetClip(Graphics g) {
		if (parent != null) {
			int pleft = parent.originLeft;
			int ptop = parent.originTop;
			int pright = pleft + parent.width + 1;
			int pbottom = ptop + parent.height + 1;
			pleft = pleft > this.originLeft ? pleft : this.originLeft;
			ptop = ptop > this.originTop ? ptop : this.originTop;
			pright = pright < this.originLeft + this.width ? pright
					: this.originLeft + this.width;
			pbottom = pbottom < this.originTop + this.height ? pbottom
					: this.originTop + this.height;
			g.setClip(pleft, ptop, pright - pleft + 1, pbottom - ptop + 1);
		} else {
			g.setClip(this.originLeft, this.originTop, this.width + 1,
					this.height + 1);
		}
	}

	public final void SetClip(Graphics g, int x, int y, int width, int height) {
		if (parent != null) {
			int pleft = parent.originLeft;
			int ptop = parent.originTop;
			int pright = pleft + parent.width + 1;
			int pbottom = ptop + parent.height + 1;
			pleft = pleft > originLeft + x ? pleft : originLeft + x;
			ptop = ptop > originTop + y ? ptop : originTop + y;
			pright = pright < originLeft + x + width ? pright : originLeft
					+ width + x;
			pbottom = pbottom < originTop + y + height ? pbottom : originTop
					+ height + y;
			g.setClip(pleft, ptop, pright - pleft, pbottom - ptop);
		} else {
			g.setClip(this.originLeft + x, this.originTop + y, width, height);
		}
	}

	public void Click() {
		if (this.onClick != null) {
			this.onClick.Event(this);
		}
	}

	protected final int calcTextHeight() {
		return getFont().getHeight();
	}

	protected final int calcTextWidth() {
		if (this.text != null) {
			char[] txtarr = this.text.toCharArray();
			return getFont().charsWidth(txtarr, 0, txtarr.length);
		}
		return this.width;
	}

	protected void DrawText(Graphics g, int LeftSpace, int RightSpace,
			int TopSpace, int BottomSpace) {
		int dtX = this.originLeft + 1 + LeftSpace;
		int dtY = this.originTop + 1;

		int clientWidth = width - 2 - LeftSpace - RightSpace;
		int clientHeight = height - 2;
		String drawText = text;
		char[] texts = drawText.toCharArray();
		int textWidth = getFont().charsWidth(texts, 0, texts.length);
		int textHeight = getFont().getHeight();

		if (textWidth < clientWidth) {
			if (this.alignment == SDAConsts.alignCenter) {

				dtX += (clientWidth - textWidth) / 2;
			}
			if (this.alignment == SDAConsts.alignRight) {

				dtX += (clientWidth - textWidth);
			}
		} else {
			textWidth = clientWidth;
		}
		if (textHeight < clientHeight) {
			dtY += (clientHeight - textHeight) / 2;
		}
		dtY = dtY + TopSpace - BottomSpace;
		g.setClip(this.originLeft + 1 + LeftSpace, this.originTop + 1,
				clientWidth, height - 2);
		g.setColor(getForeColor());
		g.setFont(getFont());
		g.drawString(drawText, dtX, dtY, 0);
	}

	protected void DrawText(Graphics g, int LeftSpace, int RightSpace) {
		DrawText(g, LeftSpace, RightSpace, 0, 0);
	}

	protected void DrawText(Graphics g) {
		DrawText(g, 0, 0);
	}

	protected void serviceRepaints() {
		if (IsCanPaint() && this.form.Application.isDoubleBuffered()) {
			form.canvas.repaint();
			form.canvas.serviceRepaints();
		}
	}

	protected void repaintControl() {
		if (IsCanPaint()) {
			if (isTransparent() && parent != null) {
				parent.paint();
			} else {
				paint();
			}
			serviceRepaints();
		}
	}

	public final Vector FindControls(Class AClass) {
		Vector result = new Vector();
		for (int i = ctrlList.size() - 1; i >= 0; i--) {
			SDABaseControl ctrl = (SDABaseControl) ctrlList.elementAt(i);
			if (ctrl.getClass() == AClass) {
				result.addElement(ctrl);
			}
		}
		return result;
	}

	public final Vector FindControls(Class AClass, String CtrlName) {
		Vector result = new Vector();
		for (int i = ctrlList.size() - 1; i >= 0; i--) {
			SDABaseControl ctrl = (SDABaseControl) ctrlList.elementAt(i);
			if (ctrl.getClass() == AClass && ctrl.getName().equals(CtrlName)) {
				result.addElement(ctrl);
			}
		}
		return result;
	}

	protected final SDABaseControl FindControl(int x, int y) {
		if (!this.visible || !this.enabled) {
			return null;
		}
		SDABaseControl result = this;
		for (int i = ctrlList.size() - 1; i >= 0; i--) {
			SDABaseControl ctrl = (SDABaseControl) ctrlList.elementAt(i);
			if (ctrl.visible && ctrl.enabled) {
				if (ctrl.InScreenRect(x, y)) {
					result = ctrl.FindControl(x, y);
					if (result != null) {
						break;
					}
				}
			}
		}
		/*
		 * if (result != null) { System.out.println(result.getClass().getName()
		 * + ".findControl(" + x + "," + y + ")"); }
		 */
		if (result instanceof SDAForm) {
			result = null;
		}
		return result;
	}

	protected final boolean IsCanPaint() {
		boolean parentVisable = false;
		if (this.visible && this.form != null && this.form.canvas != null
				&& this.form.graphics != null) {
			SDABaseControl ctrl = this;
			parentVisable = true;
			while (ctrl.parent != null) {
				parentVisable = this.parent.visible;
				if (!parentVisable) {
					break;
				}
				ctrl = ctrl.parent;
				if (ctrl instanceof SDAForm) {
					break;
				}
			}
		}
		this.originLeft = this.clientXToScreen(this.left);
		this.originTop = this.clientYToScreen(this.top);
		return parentVisable;
	}

	protected final void PaintChilds() {
		if (!hasChild()) {
			return;
		}
		SDABaseControl ctrl;
		setChildsLayout();
		for (int i = 0; i < ctrlList.size(); i++) {
			ctrl = (SDABaseControl) ctrlList.elementAt(i);
			ctrl.ctl3d = this.ctl3d;
			ctrl.paint();
		}
	}

	public abstract void paint();

	protected void SetForm(SDAForm form) {
		this.form = form;
		if (ctrlList.isEmpty()) {
			return;
		}
		for (int i = 0; i < ctrlList.size(); i++) {
			((SDABaseControl) (ctrlList.elementAt(i))).SetForm(form);
		}
	}

	public void AddControl(SDABaseControl control) {
		if (control instanceof SDAMainMenu) {
			return;
		}
		if (!ctrlList.contains(control)) {
			ctrlList.addElement(control);
			control.parent = this;
			if (this.form != null) {
				control.SetForm(this.form);
			}
		}
	}

	public final void RemoveControl(SDABaseControl control) {
		if (ctrlList.contains(control)) {
			ctrlList.removeElement(control);
			control.setParent(null);
		}
	}

	// 查找下一个焦点组件
	protected SDABaseControl FindNextFocusedCtrl(SDABaseControl focusedCtrl,
			boolean Reverse) {
		SDABaseControl result = null;
		if (ctrlList.isEmpty()) {
			return null;
		}

		if (focusedCtrl == null) {
			int index = Reverse ? ctrlList.size() - 1 : 0;
			int endIndex = Reverse ? 0 : ctrlList.size() - 1;
			while (Reverse ? (index >= endIndex) : (index <= endIndex)) {
				SDABaseControl ctrl = (SDABaseControl) (ctrlList
						.elementAt(index));
				if (ctrl.CanFocused()) {
					result = ctrl;
					break;
				}
				if (ctrl.visible && ctrl.enabled && this.form != null) {
					result = ctrl.FindNextFocusedCtrl(null, Reverse);
				}
				if (result != null) {
					break;
				}
				index += Reverse ? (-1) : (1);
			}
		} else {
			int index = ctrlList.indexOf(focusedCtrl);
			index = Reverse ? (--index) : (++index);
			int endIndex = Reverse ? 0 : ctrlList.size() - 1;
			while (Reverse ? (index >= endIndex) : (index <= endIndex)) {
				SDABaseControl ctrl = (SDABaseControl) (ctrlList
						.elementAt(index));
				if (ctrl.CanFocused()) {
					result = ctrl;
					break;
				}
				index += Reverse ? (-1) : (1);
			}
			if (result == null && !(this instanceof SDAForm)
					&& this.parent != null) {
				result = this.parent.FindNextFocusedCtrl(focusedCtrl, Reverse);
			}
		}
		return result;
	}

	// 执行所有布局处理
	protected void setChildsLayout() {
		setChildsDock();
		setChildsAlign();
	}

	// 停靠对齐计算
	protected void setChildsDock() {
		SDABaseControl ctrl = null;
		SDABaseControl oldctrl = null;
		int dockleft = 0;
		int docktop = 0;
		int dockbottom = getHeight();
		int dockright = getWidth();
		// 下
		for (int i = 0; i < ctrlList.size(); i++) {
			ctrl = (SDABaseControl) ctrlList.elementAt(i);
			if (!ctrl.visible) {
				continue;
			}
			if (ctrl.dock == SDAConsts.dsBottom) {
				ctrl.left = 0;
				ctrl.top = dockbottom - ctrl.getHeight();
				ctrl.width = getWidth();
				dockbottom -= ctrl.getHeight();
				if (ctrl instanceof SDASpliter) {
					if (oldctrl != null) {
						((SDASpliter) ctrl).setChangeControl(oldctrl);
					}
				}
				oldctrl = ctrl;
			}
		}
		// 上
		for (int i = 0; i < ctrlList.size(); i++) {
			ctrl = (SDABaseControl) ctrlList.elementAt(i);
			if (!ctrl.visible) {
				continue;
			}
			if (ctrl.dock == SDAConsts.dsTop) {
				ctrl.left = 0;
				ctrl.top = docktop;
				ctrl.width = getWidth();
				docktop += ctrl.getHeight();
				if (ctrl instanceof SDASpliter) {
					if (oldctrl != null) {
						((SDASpliter) ctrl).setChangeControl(oldctrl);
					}
				}
				oldctrl = ctrl;
			}
		}
		// 右
		for (int i = 0; i < ctrlList.size(); i++) {
			ctrl = (SDABaseControl) ctrlList.elementAt(i);
			if (!ctrl.visible) {
				continue;
			}
			if (ctrl.dock == SDAConsts.dsRight) {
				ctrl.left = getWidth() - ctrl.getWidth();
				ctrl.top = docktop;
				ctrl.height = dockbottom - docktop > 0 ? dockbottom - docktop
						: 0;
				dockright -= ctrl.getWidth();
				if (ctrl instanceof SDASpliter) {
					if (oldctrl != null) {
						((SDASpliter) ctrl).setChangeControl(oldctrl);
					}
				}
				oldctrl = ctrl;
			}
		}
		// 左
		for (int i = 0; i < ctrlList.size(); i++) {
			ctrl = (SDABaseControl) ctrlList.elementAt(i);
			if (!ctrl.visible) {
				continue;
			}
			if (ctrl.dock == SDAConsts.dsLeft) {
				ctrl.left = dockleft;
				ctrl.top = docktop;
				ctrl.height = dockbottom - docktop > 0 ? dockbottom - docktop
						: 0;
				dockleft += ctrl.getWidth();
				if (ctrl instanceof SDASpliter) {
					if (oldctrl != null) {
						((SDASpliter) ctrl).setChangeControl(oldctrl);
					}
				}
				oldctrl = ctrl;
			}
		}
		// 中
		for (int i = 0; i < ctrlList.size(); i++) {
			ctrl = (SDABaseControl) ctrlList.elementAt(i);
			if (!ctrl.visible) {
				continue;
			}
			if (ctrl.dock == SDAConsts.dsFill) {
				ctrl.left = dockleft;
				ctrl.top = docktop;
				ctrl.height = dockbottom - docktop > 0 ? dockbottom - docktop
						: 0;
				ctrl.width = dockright - dockleft > 0 ? dockright - dockleft
						: 0;
			}
		}
	}

	// 加入布局
	public void setAlignment(SDABaseControl referControl,
			SDABaseControl[] arrayControl, int alignType, int paramValue) {
		internalSetAlignment(referControl, arrayControl, alignType, paramValue);
	}

	public void setAlignment(SDABaseControl referControl,
			SDABaseControl arrayControl, int alignType, int paramValue) {
		internalSetAlignment(referControl,
				new SDABaseControl[] { arrayControl }, alignType, paramValue);
	}

	private void internalSetAlignment(SDABaseControl referControl,
			SDABaseControl[] arrayControl, int alignType, int paramValue) {
		if ((referControl == null) || (arrayControl.length == 0)) {
			return;
		}
		// 加入到列表
		Align alignItem = new Align();
		alignItem.referControl = referControl;
		alignItem.arrayControl = arrayControl;
		alignItem.alignType = alignType;
		alignItem.paramValue = paramValue;
		AlignmentList.addElement(alignItem);
	}

	// 执行布局
	protected void setChildsAlign() {
		if (AlignmentList.size() == 0) {
			return;
		}
		Align alignItem = null;
		SDABaseControl referControl = null;
		SDABaseControl[] arrayControl = null;
		SDABaseControl ctrl = null;
		int alignType = -1;
		int paramValue = 0;

		int refLeft = 0;
		int refTop = 0;
		int refWidth = 0;
		int refHeight = 0;

		int minLeft = 0;
		int minTop = 0;
		int maxRight = 0;
		int maxBottom = 0;

		for (int j = 0; j < AlignmentList.size(); j++) {
			alignItem = (Align) AlignmentList.elementAt(j);
			if (!alignItem.referControl.visible) {
				continue;
			}
			referControl = alignItem.referControl;
			arrayControl = alignItem.arrayControl;
			alignType = alignItem.alignType;
			paramValue = alignItem.paramValue;

			refLeft = referControl.getLeft();
			refTop = referControl.getTop();
			refWidth = referControl.getWidth();
			refHeight = referControl.getHeight();

			// 居中
			minLeft = Integer.MAX_VALUE;
			minTop = Integer.MAX_VALUE;
			maxRight = Integer.MIN_VALUE;
			maxBottom = Integer.MIN_VALUE;
			// 相对于参照组件水平居中
			if (alignType == SDAConsts.amhInCenter) {
				refLeft = 0;
				refTop = 0;
				for (int i = 0; i < arrayControl.length; i++) {
					ctrl = arrayControl[i];
					if (!ctrl.visible) {
						continue;
					}
					if (ctrl != referControl) {
						minLeft = ctrl.getLeft() < minLeft ? ctrl.getLeft()
								: minLeft;
						maxRight = ctrl.getLeft() + ctrl.getWidth() > maxRight ? ctrl
								.getLeft()
								+ ctrl.getWidth()
								: maxRight;
					}
				}
				// 中线比较
				int stepx = refWidth / 2 + refLeft
						- ((maxRight - minLeft) / 2 + minLeft);
				// 全部重新定位
				for (int i = 0; i < arrayControl.length; i++) {
					ctrl = arrayControl[i];
					if (!ctrl.visible) {
						continue;
					}
					if (ctrl != referControl) {
						if (ctrl.dock == SDAConsts.dsNone) {
							ctrl.left += stepx;
						}
					}
				}
			} else if (alignType == SDAConsts.amvInCenter) { // 相对于参照组件垂直居中
				refLeft = 0;
				refTop = 0;
				for (int i = 0; i < arrayControl.length; i++) {
					ctrl = arrayControl[i];
					if (!ctrl.visible) {
						continue;
					}
					if (ctrl != referControl) {
						minTop = ctrl.getTop() < minTop ? ctrl.getTop()
								: minTop;
						maxBottom = ctrl.getTop() + ctrl.getHeight() > maxBottom ? ctrl
								.getTop()
								+ ctrl.getHeight()
								: maxBottom;
					}
				}
				// 中线比较
				int stepy = refHeight / 2 + refTop
						- ((maxBottom - minTop) / 2 + minTop);
				// 全部重新定位
				for (int i = 0; i < arrayControl.length; i++) {
					ctrl = arrayControl[i];
					if (!ctrl.visible) {
						continue;
					}
					if (ctrl != referControl) {
						if (ctrl.dock == SDAConsts.dsNone) {
							ctrl.top += stepy;
						}
					}
				}
			} else if (alignType == SDAConsts.amParenLeftSpace) {
				refLeft = 0;
				refTop = 0;
				for (int i = 0; i < arrayControl.length; i++) {
					ctrl = arrayControl[i];
					if (!ctrl.visible) {
						continue;
					}
					if (ctrl != referControl) {
						minLeft = ctrl.getLeft() < minLeft ? ctrl.getLeft()
								: minLeft;
						// maxRight = ctrl.getLeft() + ctrl.getWidth() >
						// maxRight ? ctrl.getLeft() + ctrl.getWidth() :
						// maxRight;
					}
				}
				// 比较
				int stepx = minLeft - paramValue;
				// 全部重新定位
				for (int i = 0; i < arrayControl.length; i++) {
					ctrl = arrayControl[i];
					if (!ctrl.visible) {
						continue;
					}
					if (ctrl != referControl) {
						if (ctrl.dock == SDAConsts.dsNone) {
							ctrl.left -= stepx;
						}
					}
				}
			} else if (alignType == SDAConsts.amParentRightSpace) {
				refLeft = 0;
				refTop = 0;
				for (int i = 0; i < arrayControl.length; i++) {
					ctrl = arrayControl[i];
					if (!ctrl.visible) {
						continue;
					}
					if (ctrl != referControl) {
						// minLeft = ctrl.getLeft() < minLeft ? ctrl.getLeft() :
						// minLeft;
						maxRight = ctrl.getLeft() + ctrl.getWidth() > maxRight ? ctrl
								.getLeft()
								+ ctrl.getWidth()
								: maxRight;
					}
				}
				// 比较
				int stepx = getWidth() - maxRight - paramValue;
				// 全部重新定位
				for (int i = 0; i < arrayControl.length; i++) {
					ctrl = arrayControl[i];
					if (!ctrl.visible) {
						continue;
					}
					if (ctrl != referControl) {
						if (ctrl.dock == SDAConsts.dsNone) {
							ctrl.left += stepx;
						}
					}
				}
			} else if (alignType == SDAConsts.amParentTopSpace) {
				refLeft = 0;
				refTop = 0;
				for (int i = 0; i < arrayControl.length; i++) {
					ctrl = arrayControl[i];
					if (!ctrl.visible) {
						continue;
					}
					if (ctrl != referControl) {
						minTop = ctrl.getTop() < minTop ? ctrl.getTop()
								: minTop;
						// maxBottom = ctrl.getTop() + ctrl.getHeight() >
						// maxBottom ? ctrl.getTop() + ctrl.getHeight() :
						// maxBottom;
					}
				}
				// 中线比较
				int stepy = minTop - paramValue;
				// 全部重新定位
				for (int i = 0; i < arrayControl.length; i++) {
					ctrl = arrayControl[i];
					if (!ctrl.visible) {
						continue;
					}
					if (ctrl != referControl) {
						if (ctrl.dock == SDAConsts.dsNone) {
							ctrl.top -= stepy;
						}
					}
				}
			} else if (alignType == SDAConsts.amParentBottomSpace) {
				refLeft = 0;
				refTop = 0;
				for (int i = 0; i < arrayControl.length; i++) {
					ctrl = arrayControl[i];
					if (!ctrl.visible) {
						continue;
					}
					if (ctrl != referControl) {
						// minTop = ctrl.getTop() < minTop ? ctrl.getTop() :
						// minTop;
						maxBottom = ctrl.getTop() + ctrl.getHeight() > maxBottom ? ctrl
								.getTop()
								+ ctrl.getHeight()
								: maxBottom;
					}
				}
				// 中线比较
				int stepy = getHeight() - maxBottom - paramValue;
				// 全部重新定位
				for (int i = 0; i < arrayControl.length; i++) {
					ctrl = arrayControl[i];
					if (!ctrl.visible) {
						continue;
					}
					if (ctrl != referControl) {
						if (ctrl.dock == SDAConsts.dsNone) {
							ctrl.top += stepy;
						}
					}
				}
			} else {
				for (int i = 0; i < arrayControl.length; i++) {
					ctrl = arrayControl[i];
					if (!ctrl.visible) {
						continue;
					}
					if (ctrl != referControl) {
						// 左对齐
						if (alignType == SDAConsts.amhleft) {
							if (ctrl.dock == SDAConsts.dsNone) {
								ctrl.setLeft(refLeft);
							}
						}
						// 右对齐
						if (alignType == SDAConsts.amhRight) {
							if (ctrl.dock == SDAConsts.dsNone) {
								ctrl.setLeft(refLeft
										+ (refWidth - ctrl.getWidth()));
							}
						}
						// 平移居中对齐
						if (alignType == SDAConsts.amhCenter) {
							if (ctrl.dock == SDAConsts.dsNone) {
								ctrl.setLeft(refLeft
										+ (refWidth - ctrl.getWidth()) / 2);
							}
						}
						// 上对齐
						if (alignType == SDAConsts.amvTop) {
							if (ctrl.dock == SDAConsts.dsNone) {
								ctrl.setTop(refTop);
							}
						}
						// 下对齐
						if (alignType == SDAConsts.amvBottom) {
							if (ctrl.dock == SDAConsts.dsNone) {
								ctrl.setTop(refTop
										+ (refHeight - ctrl.getHeight()));
							}
						}
						// 垂直移动居中对齐
						if (alignType == SDAConsts.amvCenter) {
							if (ctrl.dock == SDAConsts.dsNone) {
								ctrl.setTop(refTop
										+ (refHeight - ctrl.getHeight()) / 2);
							}
						}
						// 水平平移等距排列
						if (alignType == SDAConsts.amhSpaceEqually) {
							if (ctrl.dock == SDAConsts.dsNone) {
								ctrl.setLeft(refLeft + refWidth + paramValue);
								refLeft += refWidth + paramValue;
								refWidth = ctrl.getWidth();
							}
						}
						// 垂直移动等距排列
						if (alignType == SDAConsts.amvSpaceEqually) {
							if (ctrl.dock == SDAConsts.dsNone) {
								ctrl.setTop(refTop + refHeight + paramValue);
								refTop += refHeight + paramValue;
								refHeight = ctrl.getHeight();
							}
						}
						// 等宽
						if (alignType == SDAConsts.amWidthEqually) {
							if (ctrl.dock == SDAConsts.dsNone) {
								ctrl.setWidth(refWidth);
							}
						}
						// 等高
						if (alignType == SDAConsts.amHeightEqually) {
							if (ctrl.dock == SDAConsts.dsNone) {
								ctrl.setHeight(refHeight);
							}
						}
					}
				}
			}
		}
	}

	protected boolean InScreenRect(int x, int y) {

		return (x >= this.originLeft && x <= this.originLeft + this.width)
				&& (y >= this.originTop && y <= this.originTop + this.height);
	}

	protected boolean InClientRect(int x, int y) {

		return (x >= this.left && x <= this.left + this.width)
				&& (y >= this.top && y <= this.top + this.height);
	}

	protected boolean InClientRect(int x, int y, int left, int top, int width,
			int height) {
		return (x >= left) && (x <= left + width) && (y >= top)
				&& (y <= top + height);
	}

	// 坐标系转换
	public int screenXToClient(int x) {
		int clientX = x;
		SDABaseControl ctrl = this;

		while (ctrl != null) {
			clientX -= ctrl.getLeft();
			ctrl = ctrl.parent;
		}
		return clientX;
	}

	public int screenYToClient(int y) {
		int clientY = y;
		SDABaseControl ctrl = this;
		while (ctrl != null) {
			clientY -= ctrl.getTop();
			ctrl = ctrl.parent;
		}
		return clientY;
	}

	public int clientXToScreen(int x) {
		int clientX = x;
		SDABaseControl ctrl = this.parent;
		while (ctrl != null) {
			clientX += ctrl.getLeft();
			ctrl = ctrl.parent;
		}
		return clientX;
	}

	public int clientYToScreen(int y) {
		int clientY = y;
		SDABaseControl ctrl = this.parent;
		while (ctrl != null) {
			clientY += ctrl.getTop();
			ctrl = ctrl.parent;
		}
		return clientY;
	}

	public void fillRect(Graphics g, int left, int top, int width, int height) {
		g.fillRect(this.originLeft + left, this.originTop + top, width, height);
	}

	public void drawRect(Graphics g, int left, int top, int width, int height) {
		g.drawRect(this.originLeft + left, this.originTop + top, width, height);
	}

	public void drawLine(Graphics g, int x1, int y1, int x2, int y2) {
		g.drawLine(this.originLeft + x1, this.originTop + y1, this.originLeft
				+ x2, this.originTop + y2);
	}

	public void drawString(Graphics g, String text, int left, int top) {
		g.drawString(text, this.originLeft + left, this.originTop + top, 0);
	}

	public void drawImage(Graphics g, Image img, int x, int y, int anchor) {
		g.drawImage(img, this.originLeft + x, this.originTop + y, anchor);
	}

	public void drawCircle(Graphics g, double x, double y, double r,
			int startAngle, int arcAngle) {
		g.drawArc((int) (this.originLeft + x - r),
				(int) (this.originTop + y - r), (int) (r * 2), (int) (r * 2),
				startAngle, arcAngle);
	}

	public void fillCircel(Graphics g, double x, double y, double r,
			int startAngle, int arcAngle) {
		g.fillArc((int) (this.originLeft + x - r),
				(int) (this.originTop + y - r), (int) (r * 2), (int) (r * 2),
				startAngle, arcAngle);
	}

	public void fillTriangle(Graphics g, int x1, int y1, int x2, int y2,
			int x3, int y3) {
		g.fillTriangle(this.originLeft + x1, this.originTop + y1,
				this.originLeft + x2, this.originTop + y2,
				this.originLeft + x3, this.originTop + y3);
	}

	public void fillArc(Graphics g, int x, int y, int xWidth, int yHeight,
			int startAngle, int arcAngle) {
		g.fillArc(this.originLeft + x - xWidth / 2, this.originTop + y
				- yHeight / 2, xWidth, yHeight, startAngle, arcAngle);
	}

	public void DrawDotRect(Graphics g, int left, int top, int width, int height) {
		int storkeStyle = g.getStrokeStyle();
		g.setStrokeStyle(Graphics.DOTTED);
		g.drawRect(this.originLeft + left, this.originTop + top, width, height);
		g.setStrokeStyle(storkeStyle);
	}

	public void drawDotLine(Graphics g, int x1, int y1, int xWidth, int yHeight) {
		g.setStrokeStyle(Graphics.DOTTED);
		// 画x方向
		for (int i = 0; i < xWidth; i += 2) {
			drawLine(g, x1 + i, y1, x1 + i + 1, y1);
		}
		// 画y方向
		for (int i = 0; i < yHeight; i += 2) {
			drawLine(g, x1, y1 + i, x1, y1 + i + 1);
		}
		g.setStrokeStyle(Graphics.SOLID);
	}

	public int getForeColor() {

		return foreColor;
	}

	public boolean isEnabled() {
		return enabled;
	}

	public int getHeight() {
		return height;
	}

	public int getLeft() {
		return left;
	}

	public int getOriginLeft() {
		return originLeft;
	}

	public int getOriginTop() {
		return originTop;
	}

	public SDABaseControl getParent() {
		return parent;
	}

	public int getBackColor() {
		return backColor;
	}

	public int getTop() {
		return top;
	}

	public boolean isVisible() {
		return visible;
	}

	public int getWidth() {
		return width;
	}

	public boolean isCtl3d() {
		return ctl3d;
	}

	public Font getFont() {
		return Font.getFont(this.fontFace, this.fontStyle, this.fontSize);
	}

	public boolean isParentFont() {
		return parentFont;
	}

	public String getText() {
		return text;
	}

	public int getFontFace() {
		return fontFace;
	}

	public int getFontSize() {
		return fontSize;
	}

	public int getFontStyle() {
		return fontStyle;
	}

	public boolean isTransparent() {
		return transparent;
	}

	public boolean isFoucsed() {
		return this.form == null ? false : this.equals(this.form.focusControl);
	}

	public int getAlignment() {
		return alignment;
	}

	public boolean isTabStop() {
		return tabStop;
	}

	public boolean isDown() {
		return down;
	}

	public String getName() {
		return name;
	}

	public NotifyEvent getOnClick() {
		return onClick;
	}

	public void setWidth(int width) {
		this.width = width;
	}

	public void setVisible(boolean visible) {
		this.visible = visible;
		if (visible) {
			repaintControl();
		}
	}

	public void setTop(int top) {
		this.top = top;
	}

	public void setBackColor(int backColor) {
		this.backColor = backColor;
	}

	public void setParent(SDABaseControl parent) {
		this.parent = parent;
	}

	public void setLeft(int left) {
		this.left = left;
	}

	public void setHeight(int height) {
		this.height = height;
	}

	public void setEnabled(boolean enabled) {
		this.enabled = enabled;
	}

	public void setForeColor(int foreColor) {

		this.foreColor = foreColor;
	}

	public void setCtl3d(boolean ctl3d) {
		this.ctl3d = ctl3d;
	}

	public void setParentFont(boolean parentFont) {
		this.parentFont = parentFont;
	}

	public void setText(String text) {
		this.text = text;
		repaintControl();
	}

	public void setFont(Font font) {
		this.fontFace = font.getFace();
		this.fontStyle = font.getStyle();
		this.fontSize = font.getSize();
	}

	public void setFontStyle(int fontStyle) {
		this.fontStyle = fontStyle;
	}

	public void setFontSize(int fontSize) {
		this.fontSize = fontSize;
	}

	public void setFontFace(int fontFace) {
		this.fontFace = fontFace;
	}

	public void setTransparent(boolean transparent) {
		this.transparent = transparent;
	}

	public final boolean CanFocused() {
		return this.tabStop && this.visible && this.enabled
				&& this.form != null;
	}

	public final void setFoucsed() {
		if (CanFocused() && !isFoucsed()) {
			System.out.println("focused=" + this.getClass().getName());
			SDABaseControl ctrl = this.form.focusControl;
			this.form.focusControl = this;
			if (ctrl != null) {
				ctrl.setDown(false);
				if (ctrl.isTransparent() && this.parent != null) {
					this.parent.paint();
				} else {
					ctrl.paint();
				}
				if (ctrl.onInternalLostFocused != null) {
					ctrl.onInternalLostFocused.Event(ctrl);
				}
				if (ctrl.onLostFocused != null) {
					ctrl.onLostFocused.Event(ctrl);
				}
			}
			if (this.onInternalFocused != null) {
				this.onInternalFocused.Event(this);
			}
			if (this.onFocused != null) {
				this.onFocused.Event(this);
			}
			this.paint();
		}
	}

	public void setAlignment(int alignment) {
		this.alignment = alignment;
	}

	public void setOnClick(NotifyEvent onClick) {
		this.onClick = onClick;
	}

	public void setDown(boolean down) {
		this.down = down;
		this.paint();
	}

	public void setName(String name) {
		this.name = name;
	}

	public boolean hasChild() {
		return ctrlList.size() > 0;
	}

	public int getDock() {
		return dock;
	}

	public boolean isPopVisible() {
		if (this.form == null) {
			return false;
		}
		return this.equals(this.form.popControl);
	}

	public void setDock(int dock) {
		this.dock = dock;
	}

	public void setOnKeyDown(KeybordEvent onKeyDown) {
		this.onKeyDown = onKeyDown;
	}

	public void setOnKeyPress(KeybordEvent onKeyPress) {
		this.onKeyPress = onKeyPress;
	}

	public void setOnKeyUp(KeybordEvent onKeyUp) {
		this.onKeyUp = onKeyUp;
	}

	public void setOnPointerDragged(PointerEvent onPointerDragged) {
		this.onPointerDragged = onPointerDragged;
	}

	public void setOnPointerPressed(PointerEvent onPointerPressed) {
		this.onPointerPressed = onPointerPressed;
	}

	public void setOnPointerReleased(PointerEvent onPointerReleased) {
		this.onPointerReleased = onPointerReleased;
	}

	protected void setOriginTop(int originTop) {
		this.originTop = originTop;
	}

	protected void setOriginLeft(int originLeft) {
		this.originLeft = originLeft;
	}

	public void setOnLostFocused(NotifyEvent onLostFocused) {
		this.onLostFocused = onLostFocused;
	}

	public void setOnFocused(NotifyEvent onFocused) {
		this.onFocused = onFocused;
	}

	public void setTabStop(boolean tabStop) {
		this.tabStop = tabStop;
	}

	public void setPopVisible() {
		if (this.form != null) {
			this.form.popControl = this;
		}
	}
}
