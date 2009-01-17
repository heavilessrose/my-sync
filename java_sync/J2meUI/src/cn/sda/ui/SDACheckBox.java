package cn.sda.ui;

import javax.microedition.lcdui.*;

/**
 * <p>Title: </p>
 *
 * <p>Description: </p>
 *
 * <p>Copyright: Copyright (c) 2008</p>
 *
 * <p>Company: </p>
 *
 * @author not attributable
 * @version 1.0
 */
public class SDACheckBox extends SDABaseControl {

    private static int CBW = 12;
    private boolean checked;
    private boolean autoSize = true;

    public SDACheckBox() {
        this.width = 60;
        this.height = 24;
        this.foreColor = SDAConsts.clBlack;
        this.backColor = SDAConsts.clWhite;
        this.transparent = false;
    }

    public void Click() {
        this.checked = !this.checked;
        this.paint();
        super.Click();
    }

    private void DrawCheckBox(Graphics g) {
        int dtX = 1;
        int dtY = 0;
        if (getHeight() < CBW) {
            setHeight(CBW);
        } else {
            dtY += (getHeight() - CBW) / 2;
        }
        if (!this.transparent) {
            g.setColor(this.getBackColor());
            fillRect(g, 0, 0, getWidth(), getHeight());
        }
        if (isCtl3d()) {
            int lColor = SDAConsts.clBtnShadow;
            int rColor = SDAConsts.clWhite;
            g.setColor(lColor);
            drawLine(g, dtX, dtY, CBW - 1, dtY);
            drawLine(g, dtX, dtY, dtX, dtY + CBW - 1);
            g.setColor(rColor);
            drawLine(g, CBW - 1, dtY + CBW - 1, dtX, dtY + CBW - 1);
            drawLine(g, CBW - 1, dtY + CBW - 1, CBW - 1, dtY);
        } else {
            g.setColor(SDAConsts.clBlack);
            drawRect(g, dtX, dtY, CBW - 1, CBW - 1);
        }
        g.setColor(SDAConsts.clWhite);
        fillRect(g, dtX + 1, dtY + 1, CBW - 2, CBW - 2);
        if (this.checked) {
            g.setColor(getForeColor());
            g.setFont(getFont());
            drawString(g, "√", dtX, dtY - 2);
        }
    }

    public void paint() {
        if (!IsCanPaint()) {
            return;
        }
        Graphics g = form.getGraphics();
        //判断autoSize
        if (autoSize) {
            this.height = calcTextHeight();
            if (this.height < CBW) {
                this.height = CBW;
            }
            this.height += 4;
            this.width = calcTextWidth() + 4 + CBW;
        }
        SetClip(g, 0, 0, this.width, this.height);
        DrawCheckBox(g);
        if (text.length() == 0) {
            return;
        }
        int drawTop = (this.height - getFont().getHeight()) / 2;
        g.setFont(getFont());
        g.setColor(getForeColor());
        drawString(g, this.text, 15, drawTop);
        if (isFoucsed()) {
            g.setColor(SDAConsts.clFocusShadow);
            DrawDotRect(g, 15, drawTop - 1, this.width - 16,
                    this.height - drawTop * 2);
        }
        PaintChilds();
    }

    public boolean isChecked() {
        return checked;
    }

    public boolean isAutoSize() {
        return autoSize;
    }

    public void setChecked(boolean checked) {
        this.checked = checked;
    }

    public void setAutoSize(boolean autoSize) {
        this.autoSize = autoSize;
    }
}
