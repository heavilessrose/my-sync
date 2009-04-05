package cn.sda.ui;

import cn.sda.event.PointerEvent;
import javax.microedition.lcdui.Graphics;

/**
 *
 * @author Administrator
 */
public class SpliterUI extends BaseControl {

    private int minSize = 30;    //改变大小的组件
    private BaseControl changeControl = null;
    //原位置
    private int oldx = 0,  oldy = 0;

    public SpliterUI() {
        super();
        width = 3;
        height = 3;
        dock = ConstsUI.dsLeft;
        transparent = false;
        backColor = ConstsUI.clFocusShadow;
        setOnPointerPressed(new PointerEvent() {

            public void Event(BaseControl ctrl, int x, int y) {
                doPointerPress(x, y);
            }
        });
        setOnPointerReleased(new PointerEvent() {

            public void Event(BaseControl ctrl, int x, int y) {
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

    protected void setChangeControl(BaseControl changeControl) {
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
            if (dock == ConstsUI.dsLeft) {
                changeControl.width += x - oldx;
                if (changeControl.width < minSize) {
                    changeControl.width = minSize;
                }
            }
            if (dock == ConstsUI.dsRight) {
                changeControl.width -= x - oldx;
                if (changeControl.width < minSize) {
                    changeControl.width = minSize;
                }
            }
            if (dock == ConstsUI.dsTop) {
                changeControl.height += y - oldy;
                if (changeControl.height < minSize) {
                    changeControl.height = minSize;
                }
            }
            if (dock == ConstsUI.dsBottom) {
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
