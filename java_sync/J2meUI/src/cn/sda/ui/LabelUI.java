package cn.sda.ui;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Font;

/**
 *
 * @author not attributable
 * @version 1.0
 */
public class LabelUI extends BaseControl {

    private boolean autoSize = false;
    //文字对齐方式
    private int textAlign = ConstsUI.alignLeft;

    private void InitLabel(String text) {
        this.width = 40;
        this.height = 24;
        this.text = text;
        this.tabStop = false;
        this.autoSize = false;
        this.transparent = false;
        this.alignment = ConstsUI.alignLeft;
        backColor=ConstsUI.clWhite;
    }

    public LabelUI(String text) {
        InitLabel(text);
    }

    public LabelUI() {
        InitLabel("Label");
    }

    public boolean isAutoSize() {
        return autoSize;
    }

    public void setText(String text) {
        super.setText(text);
        setAutoSize(autoSize);
    }

    public void setAutoSize(boolean autoSize) {
        this.autoSize = autoSize;
        if (autoSize) {
            this.height = calcTextHeight() + 4;
            this.width = calcTextWidth() + 2;
        }
        paint();
    }

    public void paint() {
        if (!IsCanPaint()) {
            return;
        }
        Graphics g = form.getGraphics();
        //判断autoSize
        if (autoSize) {
            this.height = calcTextHeight() + 4;
            this.width = calcTextWidth() + 2;
        }
        g.setFont(getFont());
        SetClip(g);
        if (!this.transparent) {
            g.setColor(backColor);
            fillRect(g, 0, 0, getWidth(), getHeight());
        }
        g.setColor(foreColor);
        int drawTop = (this.height - getFont().getHeight()) / 2;
        if (textAlign == ConstsUI.alignLeft) {
            drawString(g, this.text, 1, drawTop);
        }
        if (textAlign == ConstsUI.alignCenter) {
            //文字长度
            int Len = getFont().stringWidth(getText());
            int sleft = (width - Len) / 2;
            drawString(g, this.text, sleft, drawTop);
        }
        if (textAlign == ConstsUI.alignRight) {
            //文字长度
            int Len = getFont().stringWidth(getText());
            int sleft = (width - Len) - 1;
            drawString(g, this.text, sleft, drawTop);
        }
        PaintChilds();
    }

    public int getTextAlign() {
        return textAlign;
    }

    public void setTextAlign(int textAlign) {
        this.textAlign = textAlign;
    }
}
