package cn.sda.ui;

import cn.sda.event.KeybordEvent;
import cn.sda.event.NotifyEvent;
import cn.sda.event.PointerEvent;
import javax.microedition.lcdui.Graphics;

/**
 *
 * @author Administrator
 */
public class SDANumbericUpDown extends SDABaseControl {

    private int borderColor = SDAConsts.clBlack;
    private int arrowWidth = 15;
    private int arrowBackColor = SDAConsts.clBtnFace;
    private int maxValue = 32767;
    private int minValue = 0;
    private int value = 0;
    private int increment = 1;
    //编辑框
    private SDABaseEdit NumEdit = null;
    //是否可以编辑
    private boolean canEdit = false;

    public SDANumbericUpDown() {
        super();
        visible = true;
        width = 60;
        height = 20;
        backColor = SDAConsts.clWhite;
        NumEdit = new SDABaseEdit();
        NumEdit.setTextAlign(SDAConsts.alignCenter);
        NumEdit.setBorderStyle(SDAConsts.bsNone);
        AddControl(NumEdit);
        setOnKeyUp(new KeybordEvent() {

            public void Event(SDABaseControl ctrl, int keyCode) {
                //按下
                doKeyUp(keyCode);
            }
        });
        setOnPointerPressed(new PointerEvent() {

            public void Event(SDABaseControl ctrl, int x, int y) {
                //触屏
                doPointerPress(x, y);
            }
        });
        NumEdit.setOnFocused(new NotifyEvent() {

            public void Event(SDABaseControl ctrl) {
                form.Application.inputPanel.ImeType = SDAInputPanel.imDigit;
                if (form.Application.inputPanel.visible) {
                    form.repaintControl();
                } else {
                    if (form.getMainMemu() != null) {
                        form.getMainMemu().repaintControl();
                    }
                }
            }
        });
    }

    //设置编辑框位置
    private void setTextEditPos() {
        NumEdit.left = arrowWidth + 1;
        NumEdit.top = 2;
        NumEdit.width = width - 2 * arrowWidth - 2;
        NumEdit.height = height - 4;
        NumEdit.setImeType(SDAInputPanel.imDigit);
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

    public int getArrowBackColor() {
        return arrowBackColor;
    }

    public void setArrowBackColor(int arrowBackColor) {
        this.arrowBackColor = arrowBackColor;
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
        NumEdit.text = String.valueOf(value);
        paint();
    }

    public int getHeight() {
        return super.getHeight();
    }

    public void setHeight(int height) {
        int sheight = getFont().getHeight() + 4;
        if (sheight < height) {
            sheight = height;
        }
        super.setHeight(sheight);
        paint();
    }

    public boolean isCanEdit() {
        return canEdit;
    }

    public void setCanEdit(boolean canEdit) {
        this.canEdit = canEdit;
        if (canEdit) {
            //this.tabStop = false;
            //NumEdit.tabStop = true;
            NumEdit.enabled = true;
        } else {
            //this.tabStop = true;
            //NumEdit.tabStop = false;
            NumEdit.enabled = false;
        }
    }

    public void paint() {
        if (IsCanPaint()) {
            internalPaint();
        }
    }

    private void internalPaint() {
        //画
        Graphics g = form.getGraphics();
        g.setFont(getFont());
        SetClip(g);
        setTextEditPos();
        //画边框和箭头
        drawControlRect(g);
        //最后打字
        drawControlText(g);
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
        //两边的框
        fillRect(g, 0, 0, arrowWidth, thisHeight);
        fillRect(g, thisWidth - arrowWidth, 0, arrowWidth, thisHeight);
        g.setColor(borderColor);
        drawRect(g, 0, 0, thisWidth, thisHeight);
        drawRect(g, 0, 0, arrowWidth, thisHeight);
        drawRect(g, thisWidth - arrowWidth, 0, arrowWidth, thisHeight);
        //左箭头
        g.setColor(borderColor);
        fillTriangle(g, arrowWidth / 2 - 2, thisHeight / 2, arrowWidth / 2 + 2, thisHeight / 2 - 4, arrowWidth / 2 + 2, thisHeight / 2 + 4);
        //右箭头
        fillTriangle(g, thisWidth - arrowWidth / 2 - 2, thisHeight / 2 - 4, thisWidth - arrowWidth / 2 - 2, thisHeight / 2 + 4, thisWidth - arrowWidth / 2 + 2, thisHeight / 2);
        //焦点
        if (isFoucsed()) {
            g.setColor(SDAConsts.clFocusShadow);
            drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
        }
    }

    private void drawControlText(Graphics g) {
        //显示值
        if (NumEdit.text.equals(String.valueOf(value))) {
            NumEdit.setText(String.valueOf(value));
        } else {
            try {
                value = Integer.parseInt(NumEdit.text);
                setValue(value);
                NumEdit.setText(String.valueOf(value));
            } catch (Exception e) {
                NumEdit.setText(String.valueOf(value));
            }
        }
    }

    private void doKeyUp(int keyCode) {
        String key = form.getKeyName(keyCode).toUpperCase();
        if ((key.equals(SDAConsts.KEY_LEFT)) || (key.equals(SDAConsts.KEY_UP))) {
            //左
            if (value > minValue) {
                value -= increment;
            }
        }
        if ((key.equals(SDAConsts.KEY_RIGHT)) || (key.equals(SDAConsts.KEY_DOWN))) {
            //右
            if (value < maxValue) {
                value += increment;
            }
        }
        setValue(value);
    }

    private void doPointerPress(int x, int y) {
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        int thisHeight = getHeight();
        int thisWidth = getWidth();
        if (InClientRect(posx, posy, 0, 0, thisWidth, thisHeight)) {
            if (InClientRect(posx, posy, 0, 0, arrowWidth, thisHeight)) {
                if (value > minValue) {
                    value -= increment;
                }
            } else if (InClientRect(posx, posy, thisWidth - arrowWidth, 0, arrowWidth, thisHeight)) {
                if (value < maxValue) {
                    value += increment;
                }
            }
            setValue(value);
        }
    }

    protected boolean canDownTabNext() {
        return true;
    }

    protected boolean canLeftTabPrior() {
        return false;
    }

    protected boolean canRightTabNext() {
        return false;
    }

    protected boolean canUpTabPrior() {
        return true;
    }
}
