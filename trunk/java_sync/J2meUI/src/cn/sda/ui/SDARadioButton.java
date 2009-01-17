package cn.sda.ui;

import java.util.*;

import javax.microedition.lcdui.*;

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
public class SDARadioButton extends SDABaseControl {

	private static int CBW = 12;
	private boolean checked;
	protected int groupIndex = 0;
	private boolean autoSize = true;

	private void InitRadioButton(String text) {
		this.backColor = SDAConsts.clWhite;
		this.foreColor = SDAConsts.clBlack;
		this.transparent = true;
		this.autoSize = true;
		this.text = text;
		this.width = 60;
		this.height = 24;
	}

	public SDARadioButton(String text) {
		InitRadioButton(text);
	}

	public SDARadioButton() {
		InitRadioButton("RadioButton");
	}

	public void Click() {
		if (!IsCanPaint()) {
			return;
		}
		Vector radios = this.parent.FindControls(this.getClass());
		if (!this.checked) {
			for (int i = 0; i < radios.size(); i++) {
				SDARadioButton radio = ((SDARadioButton) radios.elementAt(i));
				if (radio.groupIndex == this.groupIndex) {
					radio.checked = false;
					radio.paint();
				}
			}
		}
		this.checked = true;
		this.paint();
		super.Click();
	}

	private void DrawRadioBox(Graphics g) {
		int dtX = 1;
		int dtY = 0;
		if (getHeight() < CBW) {
			setHeight(CBW);
		} else {
			dtY += (getHeight() - CBW) / 2;
		}
		if (!transparent) {
			g.setColor(this.getBackColor());
			fillRect(g, 0, 0, getWidth(), getHeight());
		}
		double arcX = dtX + CBW / 2;
		double arcY = dtY + CBW / 2 - 1;
		if (isCtl3d()) {
			g.setColor(SDAConsts.clBtnShadow);
			drawCircle(g, arcX, arcY, 5.5, 45, 180);
			g.setColor(SDAConsts.clWhite);
			drawCircle(g, arcX, arcY, 5.5, 225, 180);
			// g.setColor(SDAConsts.clBlack);
			// drawCircle(g, getLeft() + dtX + CBW / 2,
			// getTop() + dtY + CBW / 2 - 1, 4,0,360);
		} else {
			g.setColor(SDAConsts.clBlack);
			drawCircle(g, arcX, arcY, 5.5, 0, 360);
		}
		g.setColor(getBackColor());
		fillCircel(g, arcX, arcY, 4.5, 0, 360);
		if (this.checked) {
			g.setColor(getForeColor());
			fillCircel(g, arcX, arcY, 3, 0, 360);
		}
	}

	public void paint() {
		if (!IsCanPaint()) {
			return;
		}
		Graphics g = form.getGraphics();
		// 判断autoSize
		if (autoSize) {
			this.height = calcTextHeight();
			if (this.height < CBW) {
				this.height = CBW;
			}
			this.height += 4;
			this.width = calcTextWidth() + 4 + CBW;
		}
		SetClip(g, 0, 0, this.width, this.height);
		DrawRadioBox(g);
		int drawTop = (this.height - getFont().getHeight()) / 2;
		g.setFont(getFont());
		g.setColor(getForeColor());
		drawString(g, this.text, 15, drawTop);
		if (isFoucsed()) {
			g.setColor(SDAConsts.clFocusShadow);
			DrawDotRect(g, 15, drawTop - 1, this.width - 16, this.height
					- drawTop * 2);
		}
		PaintChilds();
	}

	public final int getGroupIndex() {
		return groupIndex;
	}

	public final void setGroupIndex(int groupIndex) {
		this.groupIndex = groupIndex;
	}

	public final boolean isChecked() {
		return checked;
	}

	public boolean isAutoSize() {
		return autoSize;
	}

	public final void setChecked(boolean checked) {
		this.checked = checked;
	}

	public void setAutoSize(boolean autoSize) {
		this.autoSize = autoSize;
	}
}
