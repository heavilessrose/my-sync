package cn.sda.ui;

import cn.sda.event.KeybordEvent;
import cn.sda.event.PointerEvent;
import cn.sda.event.ScrollBarChangeEvent;
import javax.microedition.lcdui.Graphics;

/**
 *
 * @author Administrator
 */
public class SDATrackBar extends SDABaseControl {

    private byte kind = SDAConsts.ktHorizontal;
    private int borderColor = SDAConsts.clBlack;
    private int arrowBackColor = SDAConsts.clBtnFace;
    private byte tickMarks = SDAConsts.tmTopLeft;
    private int maxValue = 10;
    private int minValue = 0;
    private int value = 0;
    private int frequency = 1;
    private int thumbWidth = 8;
    //点击的滚动条的位置
    private int oldScrollPointx = 0;
    private int oldScrollPointy = 0;
    //按下滚动条
    private boolean isscrollbarpointdown = false;
    //滚动事件
    private ScrollBarChangeEvent onScrollBarChange = null;

    public SDATrackBar() {
        super();
        visible = true;
        width = 100;
        height = 15;
        backColor = SDAConsts.clWhite;
        setOnKeyUp(new KeybordEvent() {

            public void Event(SDABaseControl ctrl, int keyCode) {
                //键盘
                doKeyUp(keyCode);
            }
        });
        setOnPointerPressed(new PointerEvent() {

            public void Event(SDABaseControl ctrl, int x, int y) {
                //点
                doPointerPress(x, y);
            }
        });
        setOnPointerReleased(new PointerEvent() {

            public void Event(SDABaseControl ctrl, int x, int y) {
                //抬起
                doPointerReleased(x, y);
            }
        });
    }

    public byte getKind() {
        return kind;
    }

    public void setKind(byte kind) {
        this.kind = kind;
        repaintControl();
    }

    public int getArrowBackColor() {
        return arrowBackColor;
    }

    public void setArrowBackColor(int arrowBackColor) {
        this.arrowBackColor = arrowBackColor;
        repaintControl();
    }

    public int getBorderColor() {
        return borderColor;
    }

    public void setBorderColor(int borderColor) {
        this.borderColor = borderColor;
        repaintControl();
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
        repaintControl();
    }

    public int getThumbWidth() {
        return thumbWidth;
    }

    public void setThumbWidth(int thumbWidth) {
        this.thumbWidth = thumbWidth;
        repaintControl();
    }

    public int getFrequency() {
        return frequency;
    }

    public void setFrequency(int frequency) {
        this.frequency = frequency;
    }

    public byte getTickMarks() {
        return tickMarks;
    }

    public void setTickMarks(byte tickMarks) {
        this.tickMarks = tickMarks;
        repaintControl();
    }

    public void paint() {
        if (IsCanPaint()) {
            internalPaint();
        }
    }

    private void internalPaint() {
        //画
        Graphics g = form.getGraphics();
        SetClip(g);
        //框
        drawControlRect(g);
        PaintChilds();
    }

    private void drawControlRect(Graphics g) {
        int thisHeight = getHeight();
        int thisWidth = getWidth();
        g.setColor(backColor);
        if (!transparent) {
            fillRect(g, 0, 0, thisWidth + 1, thisHeight + 1);
        }
        if (kind == SDAConsts.ktHorizontal) {
            //标尺
            int bcWidth = thisWidth - thumbWidth;
            int pos = thumbWidth / 2;
            int id = 0;
            while (pos < thisWidth) {
                g.setColor(borderColor);
                if ((tickMarks == SDAConsts.tmTopLeft) || (tickMarks == SDAConsts.tmBoth)) {
                    drawLine(g, pos, 0, pos, 2);
                }
                if ((tickMarks == SDAConsts.tmBottomRight) || (tickMarks == SDAConsts.tmBoth)) {
                    drawLine(g, pos, thisHeight - 2, pos, thisHeight);
                }
                id++;
                pos = thumbWidth / 2 + frequency * bcWidth * id / (maxValue - minValue);
            }
            //框
            int topLine = 0;
            int bottomLine = 0;
            if ((tickMarks == SDAConsts.tmTopLeft) || (tickMarks == SDAConsts.tmBoth)) {
                topLine = 2;
            }
            if ((tickMarks == SDAConsts.tmBottomRight) || (tickMarks == SDAConsts.tmBoth)) {
                bottomLine = 2;
            }
            g.setColor(borderColor);
            drawRect(g, 0, 2 + topLine, thisWidth, thisHeight - (4 + topLine + bottomLine));
            if (isFoucsed()) {
                g.setColor(SDAConsts.clFocusShadow);
                drawRect(g, 1, 3 + topLine, thisWidth - 2, thisHeight - (6 + topLine + bottomLine));
            }
            g.setColor(arrowBackColor);
            //中间的滚动条
            int thumbLeft = (value - minValue) * (thisWidth - thumbWidth) / (maxValue - minValue);
            SetClip(g, thumbLeft, topLine, thumbWidth + 1, thisHeight - topLine - bottomLine + 1);
            g.setColor(arrowBackColor);
            fillRect(g, thumbLeft, topLine, thumbWidth, thisHeight - topLine - bottomLine);
            g.setColor(borderColor);
            drawRect(g, thumbLeft, topLine, thumbWidth, thisHeight - topLine - bottomLine);
            //竖线
            drawLine(g, thumbLeft + thumbWidth / 2 - 2, 2 + topLine, thumbLeft + thumbWidth / 2 - 2, thisHeight - 2 - bottomLine);
            drawLine(g, thumbLeft + thumbWidth / 2, 2 + topLine, thumbLeft + thumbWidth / 2, thisHeight - 2 - bottomLine);
            drawLine(g, thumbLeft + thumbWidth / 2 + 2, 2 + topLine, thumbLeft + thumbWidth / 2 + 2, thisHeight - 2 - bottomLine);
        } else {
            //标尺
            int bcWidth = thisHeight - thumbWidth;
            int pos = thumbWidth / 2;
            int id = 0;
            while (pos < thisHeight) {
                g.setColor(borderColor);
                if ((tickMarks == SDAConsts.tmTopLeft) || (tickMarks == SDAConsts.tmBoth)) {
                    drawLine(g, 0, pos, 2, pos);
                }
                if ((tickMarks == SDAConsts.tmBottomRight) || (tickMarks == SDAConsts.tmBoth)) {
                    drawLine(g, thisWidth - 2, pos, thisWidth, pos);
                }
                id++;
                pos = thumbWidth / 2 + frequency * bcWidth * id / (maxValue - minValue);
            }
            //框
            int topLine = 0;
            int bottomLine = 0;
            if ((tickMarks == SDAConsts.tmTopLeft) || (tickMarks == SDAConsts.tmBoth)) {
                topLine = 2;
            }
            if ((tickMarks == SDAConsts.tmBottomRight) || (tickMarks == SDAConsts.tmBoth)) {
                bottomLine = 2;
            }
            g.setColor(borderColor);
            drawRect(g, 2 + topLine, 0, thisWidth - (4 + topLine + bottomLine), thisHeight);
            if (isFoucsed()) {
                g.setColor(SDAConsts.clFocusShadow);
                drawRect(g, 3 + topLine, 1, thisWidth - (6 + topLine + bottomLine), thisHeight - 2);
            }
            g.setColor(arrowBackColor);
            //中间的滚动条
            int thumbTop = (value - minValue) * (thisHeight - thumbWidth) / (maxValue - minValue);
            SetClip(g, topLine, thumbTop, thisWidth - topLine - bottomLine + 1, thumbWidth + 1);
            g.setColor(arrowBackColor);
            fillRect(g, topLine, thumbTop, thisWidth - topLine - bottomLine, thumbWidth);
            g.setColor(borderColor);
            drawRect(g, topLine, thumbTop, thisWidth - topLine - bottomLine, thumbWidth);
            //竖线
            drawLine(g, 2 + topLine, thumbTop + thumbWidth / 2 - 2, thisWidth - 2 - bottomLine, thumbTop + thumbWidth / 2 - 2);
            drawLine(g, 2 + topLine, thumbTop + thumbWidth / 2, thisWidth - 2 - bottomLine, thumbTop + thumbWidth / 2);
            drawLine(g, 2 + topLine, thumbTop + thumbWidth / 2 + 2, thisWidth - 2 - bottomLine, thumbTop + thumbWidth / 2 + 2);
        }
    }

    private void doKeyUp(int keyCode) {
        String key = form.getKeyName(keyCode).toUpperCase();
        if ((key.equals(SDAConsts.KEY_LEFT) && (kind == SDAConsts.ktHorizontal)) ||
                (key.equals(SDAConsts.KEY_UP) && (kind == SDAConsts.ktVertical))) {
            //左
            if (value > minValue) {
                value -= frequency;
                doScrollChange();
            }
        }
        if ((key.equals(SDAConsts.KEY_RIGHT) && (kind == SDAConsts.ktHorizontal)) ||
                (key.equals(SDAConsts.KEY_DOWN) && (kind == SDAConsts.ktVertical))) {
            //右
            if (value < maxValue) {
                value += frequency;
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
            if (kind == SDAConsts.ktHorizontal) {
                int thumbLeft = (value - minValue) * (thisWidth - thumbWidth) / (maxValue - minValue);
                if (InClientRect(posx, posy, thumbLeft, 0, thumbWidth, thisHeight)) {
                    //记录
                    oldScrollPointx = posx;
                    oldScrollPointy = posy;
                    isscrollbarpointdown = true;
                } else {
                    //空白处，滚动                    
                    if (posx < thumbLeft) {
                        if (value > minValue) {
                            value -= frequency;
                        }
                    } else {
                        if (value < maxValue) {
                            value += frequency;
                        }
                    }
                    doScrollChange();
                }
            } else {
                int thumbTop = (value - minValue) * (thisHeight - thumbWidth) / (maxValue - minValue);
                if (InClientRect(posx, posy, 0, thumbTop, thisWidth, thumbWidth)) {
                    //记录
                    oldScrollPointx = posx;
                    oldScrollPointy = posy;
                    isscrollbarpointdown = true;
                } else {
                    //空白处，滚动
                    if (posy < thumbTop) {
                        if (value > minValue) {
                            value -= frequency;
                        }
                    } else {
                        if (value < maxValue) {
                            value += frequency;
                        }
                    }
                    doScrollChange();
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
                int step = 0;
                if ((stepx * (maxValue - minValue) / (thisWidth - thumbWidth)) % frequency != 0) {
                    step = ((stepx * (maxValue - minValue) / (thisWidth - thumbWidth)) / frequency + stepx > 0 ? 1 : -1) * frequency;
                } else {
                    step = (stepx * (maxValue - minValue) / (thisWidth - thumbWidth));
                }
                value = step + value;
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
                int step = 0;
                if ((stepy * (maxValue - minValue) / (thisHeight - thumbWidth)) % frequency > 0) {
                    step = ((stepy * (maxValue - minValue) / (thisHeight - thumbWidth)) / frequency + stepy > 0 ? 1 : -1) * frequency;
                } else {
                    step = (stepy * (maxValue - minValue) / (thisHeight - thumbWidth));
                }
                value = step + value;
                if (value < minValue) {
                    value = minValue;
                }
                if (value > maxValue) {
                    value = maxValue;
                }
                doScrollChange();
            }
        }
        isscrollbarpointdown = false;
        repaintControl();
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
        if (kind == SDAConsts.ktHorizontal) {
            return true;
        } else {
            if (value == maxValue) {
                return true;
            } else {
                return false;
            }
        }
    }

    protected boolean canLeftTabPrior() {
        if (kind == SDAConsts.ktVertical) {
            return true;
        } else {
            if (value == minValue) {
                return true;
            } else {
                return false;
            }
        }
    }

    protected boolean canRightTabNext() {
        if (kind == SDAConsts.ktVertical) {
            return true;
        } else {
            if (value == maxValue) {
                return true;
            } else {
                return false;
            }
        }
    }

    protected boolean canUpTabPrior() {
        if (kind == SDAConsts.ktHorizontal) {
            return true;
        } else {
            if (value == minValue) {
                return true;
            } else {
                return false;
            }
        }
    }
}
