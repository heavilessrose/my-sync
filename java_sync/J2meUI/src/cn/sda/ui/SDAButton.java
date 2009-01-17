package cn.sda.ui;

import javax.microedition.lcdui.*;

/**
 * @author not attributable
 * @version 1.0
 */
public class SDAButton extends SDABaseControl {

	private Image image = null;
	private int glyphAlignType = SDAConsts.blGlyphLeft;
	private int downBackColor = SDAConsts.clWhite;
	private int downForeColor = SDAConsts.clBlack;
	private int borderColor = SDAConsts.clBlack;

	private void InitButton(String Caption) {
		this.text = Caption;
		this.visible = true;
		this.width = 60;
		this.height = 20;
		this.backColor = SDAConsts.clBtnFace;
		this.parentFont = false;
		ctl3d = false;
	}

	public SDAButton() {
		InitButton("");
	}

	public SDAButton(String text) {
		InitButton(text);
	}

	private void DrawSkinsRect(Graphics g) {
		Image skin = this.down ? form.Application.getSkins().getImage(
				"button_down_bg.png") : form.Application.getSkins().getImage(
				"button_bg.png");
		skin = SDAImageUtils.processImage(skin, this.width - 2,
				this.height - 2, SDAImageUtils.MODE_STRETCH_VERTICAL
						| SDAImageUtils.MODE_REPEAT);
		drawImage(g, skin, 1, 1, 0);
		skin = null;
		g
				.setColor(isFoucsed() ? SDAConsts.clFocusShadow
						: SDAConsts.clAarkGray);
		drawRect(g, 1, 1, getWidth() - 2, getHeight() - 2);
	}

	private void DrawShadowRect(Graphics g) {
		if (isCtl3d()) {
			g.setColor(this.down ? SDAConsts.clBtnShadow : SDAConsts.clWhite);
			drawLine(g, 0, 0, getWidth(), 0);
			drawLine(g, 0, 0, 0, getHeight());
			g.setColor(SDAConsts.clBtnShadow);
			drawLine(g, getWidth() - 1, getHeight() - 1, 0, getHeight() - 1);
			drawLine(g, getWidth() - 1, getHeight() - 1, getWidth() - 1, 0);
		} else {
			g.setColor(borderColor);
			drawRect(g, 0, 0, getWidth() - 1, getHeight() - 1);
		}
		if (!transparent) {
			if (ctl3d) {
				g.setColor(SDAConsts.clBtnFace);
			} else {
				g.setColor(this.down ? downBackColor : backColor);
			}
			fillRect(g, 1, 1, getWidth() - 2, getHeight() - 2);
		}
		if (isFoucsed()) {
			g.setColor(SDAConsts.clFocusShadow);
			drawRect(g, 1, 1, getWidth() - 3, getHeight() - 3);
		}
	}

	public void paint() {
		if (!IsCanPaint()) {
			return;
		}
		Graphics g = form.getGraphics();
		g.setFont(getFont());
		SetClip(g, 0, 0, this.width, this.height);
		if (this.form.Application.getSkins() == null) {
			DrawShadowRect(g);
		} else {
			DrawSkinsRect(g);
		}
		Font ft = getFont();
		int fontHeight = ft.getHeight();
		g.setColor(this.down ? downForeColor : foreColor);
		if (image != null) {
			if (text.length() > 0) {
				if (glyphAlignType == SDAConsts.blGlyphTop) {
					drawImage(
							g,
							image,
							(getWidth() - image.getWidth()) / 2 + 1,
							(getHeight() - image.getHeight() - fontHeight) / 2 + 1,
							0);
					drawString(g, text,
							(getWidth() - ft.stringWidth(text)) / 2 + 1,
							(getHeight() - image.getHeight() - fontHeight) / 2
									+ image.getHeight() + 1);
				}
				if (glyphAlignType == SDAConsts.blGlyphBottom) {
					drawImage(g, image,
							(getWidth() - image.getWidth()) / 2 + 1,
							(getHeight() - image.getHeight() - fontHeight) / 2
									+ fontHeight + 1, 0);
					drawString(
							g,
							text,
							(getWidth() - ft.stringWidth(text)) / 2 + 1,
							(getHeight() - image.getHeight() - fontHeight) / 2 + 1);
				}
				if (glyphAlignType == SDAConsts.blGlyphLeft) {
					drawImage(g, image, (getWidth() - image.getWidth()
							- ft.stringWidth(text) - 2) / 2 + 1,
							(getHeight() - image.getHeight()) / 2, 0);
					drawString(g, text, (getWidth() - ft.stringWidth(text)
							- image.getWidth() - 2)
							/ 2 + image.getWidth() + 3,
							(getHeight() - fontHeight) / 2);
				}
				if (glyphAlignType == SDAConsts.blGlyphRight) {
					drawImage(g, image, (getWidth() - image.getWidth()
							- ft.stringWidth(text) - 2)
							/ 2 + ft.stringWidth(text) + 3,
							(getHeight() - image.getHeight()) / 2, 0);
					drawString(g, text, (getWidth() - ft.stringWidth(text)
							- image.getWidth() - 2) / 2 + 1,
							(getHeight() - fontHeight) / 2);
				}

			} else {
				drawImage(g, image, (getWidth() - image.getWidth()) / 2 + 1,
						(getHeight() - image.getHeight()) / 2, 0);
			}
		} else {
			drawString(g, text, (getWidth() - ft.stringWidth(text)) / 2 + 1,
					(getHeight() - fontHeight) / 2);
		}
		PaintChilds();
	}

	public Image getButtonImage() {
		return image;
	}

	public void setButtonImage(Image buttonImage) {
		this.image = buttonImage;
	}

	public int getGlyphAlignType() {
		return glyphAlignType;
	}

	public void setGlyphAlignType(int glyphAlignType) {
		this.glyphAlignType = glyphAlignType;
	}

	public int getDownBackColor() {
		return downBackColor;
	}

	public void setDownBackColor(int downBackColor) {
		this.downBackColor = downBackColor;
	}

	public int getDownForeColor() {
		return downForeColor;
	}

	public void setDownForeColor(int downForeColor) {
		this.downForeColor = downForeColor;
	}

	public int getBorderColor() {
		return borderColor;
	}

	public void setBorderColor(int borderColor) {
		this.borderColor = borderColor;
	}

}
