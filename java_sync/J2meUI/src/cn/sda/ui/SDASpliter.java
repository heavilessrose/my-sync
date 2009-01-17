package cn.sda.ui;

import cn.sda.event.PointerEvent;
import javax.microedition.lcdui.Graphics;

/**
 *
 * @author Administrator
 */
public class SDASpliter extends SDABaseControl {

    private int minSize = 30;    //改变大小的组件
    private SDABaseControl changeControl = null;
    //原位置
    private int oldx = 0,  oldy = 0;

    public SDASpliter() {
        super();
        width = 3;
        height = 3;
        dock = SDAConsts.dsLeft;
        transparent = false;
        backColor = SDAConsts.clFocusShadow;
        setOnPointerPressed(new PointerEvent() {

            public void Event(SDABaseControl ctrl, int x, int y) {
                doPointerPress(x, y);
            }
        });
        setOnPointerReleased(new PointerEvent() {

            public void Event(SDABaseControl ctrl, int x, int y) {
                doPointerReleased(x, y);
            }
        });
    }

    public void paint() {
        if (!IsCanPaint()) {
            return;
        }
        Graphics g = form.getGraphics();
        g.setFont(getFont());
        SetClip(g);
        //变量
        int thisWidth = getWidth();
        int thisHeight = getHeight();
        //填充
        if (!transparent) {
            g.setColor(backColor);
            fillRect(g, 0, 0, thisWidth, thisHeight);
        }
        g.setColor(foreColor);
        drawRect(g, 0, 0, thisWidth, thisHeight);
        PaintChilds();
    }

    public int getMinSize() {
        return minSize;
    }

    public void setMinSize(int minSize) {
        this.minSize = minSize;
    }

    protected void setChangeControl(SDABaseControl changeControl) {
        this.changeControl = changeControl;
    }

    private void doPointerPress(int x, int y) {
        oldx = x;
        oldy = y;
        setPopVisible();
    }

    private void doPointerReleased(int x, int y) {
        //改变位置        
        if (changeControl != null) {
            if (dock == SDAConsts.dsLeft) {
                changeControl.width += x - oldx;
                if (changeControl.width < minSize) {
                    changeControl.width = minSize;
                }
            }
            if (dock == SDAConsts.dsRight) {
                changeControl.width -= x - oldx;
                if (changeControl.width < minSize) {
                    changeControl.width = minSize;
                }
            }
            if (dock == SDAConsts.dsTop) {
                changeControl.height += y - oldy;
                if (changeControl.height < minSize) {
                    changeControl.height = minSize;
                }
            }
            if (dock == SDAConsts.dsBottom) {
                changeControl.height -= y - oldy;
                if (changeControl.height < minSize) {
                    changeControl.height = minSize;
                }
            }
            form.closePopCtrl();
            //刷新
            form.repaintControl();
        }
    }
}
