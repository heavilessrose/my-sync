package cn.sda.ui;

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
public class SDAProgress extends SDABaseControl {
	public static final byte dtHORIZONTAL = 0;
	public static final byte dtVERTICAL = 1;
	private int borderColor = SDAConsts.clBlack;
	private int progress = 0;
	private byte directionType = dtHORIZONTAL;
	private boolean showProgressLabel = true;

	private void InitProgress(byte directionType) {
		setDirectionType(directionType);
		this.backColor = SDAConsts.clWhite;
		this.foreColor = SDAConsts.clBlack;
		this.tabStop = false;
	}

	public SDAProgress(byte directionType) {
		InitProgress(directionType);
	}

	public SDAProgress() {
		InitProgress(dtHORIZONTAL);
	}

	private void DrawProgress(Graphics g) {
		g.setColor(this.borderColor);
		drawRect(g, 0, 0, this.width - 1, this.height - 1);
		g.setColor(this.backColor);
		fillRect(g, 1, 1, this.width - 3, this.height - 3);
		if (progress < 0) {
			progress = 0;
		} else if (progress > 100) {
			progress = 100;
		}
		if (this.directionType == dtHORIZONTAL) {
			int pgrWidth = 0;
			if (progress > 0) {
				pgrWidth = (int) (this.width * (progress / 100.00));
			}
			g.setColor(this.getForeColor());
			fillRect(g, 1, 1, pgrWidth, this.height - 1);
			if (showProgressLabel) {
				// 画Label
				g.setFont(getFont());
				String pgrLabel = progress + "%";
				this.text = pgrLabel;
				int pgrLabelWidth = calcTextWidth();
				int pgrLabelX = (this.width - pgrLabelWidth) / 2;
				int prgLabelY = (this.height - getFont().getHeight()) / 2;
				SetClip(g, 1, 0, pgrWidth, this.getHeight());
				g.setColor(0x00FFFFFF - this.getForeColor());
				drawString(g, pgrLabel, pgrLabelX, prgLabelY);
				SetClip(g, pgrWidth + 1, 0, this.getWidth() - pgrLabelWidth,
						this.getHeight());
				g.setColor(this.getForeColor());
				drawString(g, pgrLabel, pgrLabelX, prgLabelY);
			}
		} else {
			int pgrHeigth = 0;
			if (progress > 0) {
				pgrHeigth = (int) (this.height * (progress / 100.00));
			}
			g.setColor(this.getForeColor());
			fillRect(g, 1, this.height - pgrHeigth, this.width - 1, pgrHeigth);
			if (showProgressLabel) {
				// 画Label
				g.setFont(getFont());
				String pgrLabel = progress + "%";
				this.text = pgrLabel;
				int pgrLabelHeight = getFont().getHeight();
				int pgrLabelWidth = calcTextWidth();
				int pgrLabelX = this.getOriginLeft()
						+ (this.width - pgrLabelWidth) / 2;
				int prgLabelY = this.getOriginTop() - pgrLabelHeight;
				g.setClip(pgrLabelX, prgLabelY, pgrLabelWidth, pgrLabelHeight);
				g.setColor(this.getForeColor());
				g.drawString(pgrLabel, pgrLabelX, prgLabelY, 0);
			}
		}
	}

	/**
	 * paint
	 * 
	 * @todo Implement this cn.sdaui.cldc.SDABaseControl method
	 */
	public void paint() {
		if (!IsCanPaint()) {
			return;
		}
		Graphics g = form.getGraphics();
		g.setFont(getFont());
		SetClip(g, 0, 0, this.width, this.height);
		DrawProgress(g);
		PaintChilds();
	}

	public int getBorderColor() {
		return borderColor;
	}

	public int getProgress() {
		return progress;
	}

	public boolean isShowProgressLabel() {
		return showProgressLabel;
	}

	public byte getDirectionType() {
		return directionType;
	}

	public void setBorderColor(int borderColor) {
		this.borderColor = borderColor;
	}

	public void setProgress(int progress) {
		this.progress = progress;
		this.paint();
	}

	public void setDirectionType(byte directionType) {
		this.directionType = directionType;
	}

	public void setShowProgressLabel(boolean showProgressLabel) {
		this.showProgressLabel = showProgressLabel;
	}
}
