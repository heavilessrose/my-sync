package cn.sda.ui;

import javax.microedition.lcdui.*;

/**
 * @author not attributable
 * @version 1.0
 */
public class SDABevel extends SDABaseControl {
	// Shape
	public static int bsTopLine = 1;
	public static int bsLeftLine = 2;
	public static int bsRightLine = 3;
	public static int bsBottomLine = 4;
	public static int bsBox = 5;
	public static int bsFrame = 6;
	public static int bsSpacer = 7;
	public static int styleLowered = 1;
	public static int styleRaised = 2;
	private int shape;
	private int style;

	public SDABevel() {
		this.width = 60;
		this.height = 60;
		this.backColor = SDAConsts.clBtnFace;
		this.tabStop = false;
		this.shape = bsBottomLine;
		this.style = styleLowered;
	}

	private void DrawShadowRect(Graphics g) {
		g.setColor(this.getBackColor());
		fillRect(g, 1, 1, this.width - 1, this.height - 1);
		int lColor = (this.style == styleLowered ? SDAConsts.clBtnShadow
				: SDAConsts.clWhite);
		int rColor = (this.style == styleLowered ? SDAConsts.clWhite
				: SDAConsts.clBtnShadow);
		if (this.shape == bsTopLine) {
			g.setColor(lColor);
			drawLine(g, 1, 1, this.width - 1, 1);
			g.setColor(rColor);
			drawLine(g, 1, 2, this.width - 1, 2);
		}
		if (this.shape == bsBottomLine) {
			g.setColor(lColor);
			drawLine(g, 1, this.height - 2, this.width - 1, this.height - 2);
			g.setColor(rColor);
			drawLine(g, 1, this.height - 1, this.width - 1, this.height - 1);
		}
		if (this.shape == bsLeftLine) {
			g.setColor(lColor);
			drawLine(g, 1, 1, 1, this.height - 1);
			g.setColor(rColor);
			drawLine(g, 2, 1, 2, this.height - 1);
		}
		if (this.shape == bsRightLine) {
			g.setColor(lColor);
			drawLine(g, this.width - 2, 1, this.width - 2, this.height - 1);
			g.setColor(rColor);
			drawLine(g, this.width - 1, 1, this.width - 1, this.height - 1);
		}
		if (this.shape == bsFrame) {
			g.setColor(rColor);
			drawRect(g, 2, 2, this.width - 2, this.height - 2);
			g.setColor(lColor);
			drawRect(g, 1, 1, this.width - 2, this.height - 2);
		}
		if (this.shape == bsBox) {
			g.setColor(lColor);
			drawLine(g, 1, 1, this.width - 1, 1);
			drawLine(g, 1, 1, 1, this.height - 1);
			g.setColor(rColor);
			drawLine(g, this.width - 1, this.height - 1, 1, this.height - 1);
			drawLine(g, this.width - 1, this.height - 1, this.width - 1, 1);
		}
	}

	public void paint() {
		if (!IsCanPaint()) {
			return;
		}
		Graphics g = form.getGraphics();
		g.setFont(getFont());
		SetClip(g, 0, 0, this.width, this.height);
		DrawShadowRect(g);
		DrawText(g);
		PaintChilds();
	}

	public int getShape() {
		return shape;
	}

	public int getStyle() {
		return style;
	}

	public void setShape(int shape) {
		this.shape = shape;
		repaintControl();
	}

	public void setStyle(int style) {
		this.style = style;
		repaintControl();
	}
}
