package cn.sda.ui;

import cn.sda.event.PointerEvent;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.DataInputStream;
import java.io.ByteArrayInputStream;
import javax.microedition.lcdui.*;

/**
 */
public class SDASignPanel extends SDABaseControl {

    //按下初始点
    private int ox,  oy;
    //记录鼠标点的轨迹流
    private ByteArrayOutputStream SecByteStream;
    private DataOutputStream SecStream;
    private ByteArrayOutputStream ByteStream;
    private DataOutputStream PointStream = null;
    //读信息画图    
    private ByteArrayInputStream arrayInputStream;
    private DataInputStream dataInputStream;
    private ByteArrayInputStream secArrayInputStream;
    private DataInputStream secDataInputStream;
    //按下
    private boolean mousedown = false;
    //设置线条粗细
    private int LineSize = 1;
    //设置边框线颜色
    private int BorderColor = SDAConsts.clBlack;

    public int getBorderColor() {
        return BorderColor;
    }

    public void setBorderColor(int BorderColor) {
        this.BorderColor = BorderColor;
        internalPaint();
    }

    public int getLineSize() {
        return LineSize;
    }

    public void setLineSize(int LineSize) {
        this.LineSize = LineSize;
        internalPaint();
    }

    public SDASignPanel() {
        super();
        internalSDASignPanel();
    }

    private void internalSDASignPanel() {
        this.setVisible(true);
        super.setWidth(100);
        super.setHeight(50);
        super.setBackColor(SDAConsts.clWhite);
        setParentFont(false);
        //流
        ByteStream = new ByteArrayOutputStream();
        PointStream = new DataOutputStream(ByteStream);
        //事件
        onPointerDragged = new PointerEvent() {

            public void Event(SDABaseControl ctrl, int x, int y) {
                doPointerDragged(x, y);
            }
        };
        onPointerPressed = new PointerEvent() {

            public void Event(SDABaseControl ctrl, int x, int y) {
                doPointerPressed(x, y);
            }
        };
        onPointerReleased = new PointerEvent() {

            public void Event(SDABaseControl ctrl, int x, int y) {
                doPointerReleased(x, y);
            }
        };
    }

    public void clear() {
        ByteStream = null;
        PointStream = null;
        ByteStream = new ByteArrayOutputStream();
        PointStream = new DataOutputStream(ByteStream);
        internalPaint();
    }

    public void paint() {
        internalPaint();
    }

    private void internalPaint() {
        if (!IsCanPaint()) {
            return;
        }
        if (mousedown) {
            return;
        }
        Graphics g = form.getGraphics();
        g.setFont(getFont());
        SetClip(g);
        g.setColor(getBackColor());
        fillRect(g, 0, 0, getWidth(), getHeight());
        g.setColor(BorderColor);
        drawRect(g, 0, 0, getWidth(), getHeight());
        //焦点
        if (isFoucsed()) {
            g.setColor(SDAConsts.clFocusShadow);
            drawRect(g, 1, 1, getWidth() - 2, getHeight() - 2);
        }
        g.setColor(getForeColor());
        //画
        drawSign(g, true);
        PaintChilds();
    }

    private void drawSign(Graphics g, boolean self) {
        if (ByteStream == null) {
            return;
        }
        int lx, ly, rx, ry;
        arrayInputStream = new ByteArrayInputStream(ByteStream.toByteArray());
        dataInputStream = new DataInputStream(arrayInputStream);
        try {
            while (dataInputStream.available() > 0) {
                //读片段长度
                short SecLen = dataInputStream.readShort();
                byte[] SecByte = new byte[SecLen];
                dataInputStream.read(SecByte);
                secArrayInputStream = new ByteArrayInputStream(SecByte);
                secDataInputStream = new DataInputStream(secArrayInputStream);
                //读第一个点
                short x = secDataInputStream.readShort();
                short y = secDataInputStream.readShort();
                //初始
                lx = x;
                ly = y;
                while (secDataInputStream.available() > 0) {
                    //读点
                    byte ax = secDataInputStream.readByte();
                    byte ay = secDataInputStream.readByte();
                    rx = lx + ax;
                    ry = ly + ay;
                    //注意画线粗细
                    for (int i = 0; i < LineSize; i++) {
                        if (self) {
                            drawLine(g, lx, ly + i, rx, ry + i);
                            drawLine(g, lx + i, ly, rx + i, ry);
                        } else {
                            g.drawLine(lx, ly + i, rx, ry + i);
                            g.drawLine(lx + i, ly, rx + i, ry);
                        }
                    }
                    lx = rx;
                    ly = ry;
                }
                secArrayInputStream.close();
                secDataInputStream.close();
                secArrayInputStream = null;
                secDataInputStream = null;
            }
        } catch (Exception e) {
        } finally {
            try {
                arrayInputStream.close();
                dataInputStream.close();
                arrayInputStream = null;
                dataInputStream = null;
            } catch (Exception E) {
            }
        }
    }

    private void doPointerPressed(int x, int y) {
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        mousedown = true;
        ox = posx;
        oy = posy;
        //创建片段记录流
        SecByteStream = new ByteArrayOutputStream();
        SecStream = new DataOutputStream(SecByteStream);
        //记录第一个点
        try {
            SecStream.writeShort(posx);
            SecStream.writeShort(posy);
        } catch (Exception e) {
        }
    }

    private void doPointerReleased(int x, int y) {
        mousedown = false;
        //写到记录内容里面
        byte[] SecByte = SecByteStream.toByteArray();
        //写片段程度
        try {
            PointStream.writeShort(SecByte.length);
            //写片段具体内容
            PointStream.write(SecByte);
        } catch (Exception e) {
        }
        repaintControl();
    }

    private void doPointerDragged(int x, int y) {
        if (!mousedown) {
            return;
        }
        int rx, ry, stepx, stepy;
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        rx = posx;
        ry = posy;
        //记录当前差距
        stepx = rx - ox;
        stepy = ry - oy;
        if ((Byte.MAX_VALUE >= stepx) && (Byte.MIN_VALUE <= stepx) && (Byte.MAX_VALUE >= stepy) && (Byte.MIN_VALUE <= stepy)) {
            try {
                if ((stepx != 0) | (stepy != 0)) {
                    SecStream.writeByte(stepx);
                    SecStream.writeByte(stepy);
                }
            } catch (Exception e) {
            }
        }
        ox = rx;
        oy = ry;
    }

    public byte[] getSignData() {
        return ByteStream.toByteArray();
    }

    public void setSignData(byte[] data) {
        clear();
        try {
            ByteStream.write(data);
        } catch (Exception e) {
        }
    }
    //获取图片
    public Image getSignImage() {
        Image img = null;
        img = Image.createImage(width, height);
        Graphics g = img.getGraphics();
        //g.setColor(SDAConsts.clBtnFace);
        //g.fillRect(0, 0, width, height);
        g.setColor(foreColor);
        drawSign(g,false);
        return img;
    }
}
