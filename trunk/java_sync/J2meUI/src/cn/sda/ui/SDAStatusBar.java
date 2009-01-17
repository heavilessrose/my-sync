package cn.sda.ui;

import java.util.Vector;
import javax.microedition.lcdui.Graphics;

/**
 *
 * @author Administrator
 */
public class SDAStatusBar extends SDABaseControl{

    private int borderColor=SDAConsts.clBlack;
    private Vector statusPanels=null;

    public int getBorderColor() {
        return borderColor;
    }

    public void setBorderColor(int borderColor) {
        this.borderColor = borderColor;
    }
    
    public SDAStatusBar() {
        super();
        setWidth(100);
        setHeight(20);
        visible=true;
        setDock(SDAConsts.dsBottom);
        statusPanels=new Vector();
    }

    public void paint() {
        if (!IsCanPaint()) {
            return;
        }
        Graphics g = form.getGraphics();
        g.setFont(getFont());
        SetClip(g);
        //变量
        int thisWidth=getWidth();
        int thisHeight=getHeight();
        int fontHeight=getFont().getHeight();
        //填充
        if(!transparent){
            g.setColor(backColor);
            fillRect(g, 0, 0, thisWidth, thisHeight);
        }
        //画外框
        g.setColor(borderColor);
        drawRect(g, 0, 0, thisWidth, thisHeight);
        //根据每个panel特点画
        SDAStatusPanel panel=null;
        int pos=2;
        byte st=0;
        int st1=0;
        for(int i=0;i<statusPanels.size();i++){
            panel=(SDAStatusPanel)statusPanels.elementAt(i);
            //画
            st=panel.getStatusBevel();
            if (st == SDAConsts.pbRaised) {
                g.setColor(panel.getHighLightColor());
                drawLine(g, pos, 2, pos + panel.getWidth(), 2);
                drawLine(g, pos, 2, pos, thisHeight - 1);
                g.setColor(panel.getShadowColor());
                drawLine(g, pos + panel.getWidth(), 2, pos + panel.getWidth(), thisHeight - 1);
                drawLine(g, pos, thisHeight - 1, pos + panel.getWidth(), thisHeight - 1);
            }
            if (st == SDAConsts.pbLowered) {
                g.setColor(panel.getShadowColor());
                drawLine(g, pos, 2, pos + panel.getWidth(), 2);
                drawLine(g, pos, 2, pos, thisHeight - 1);
                g.setColor(panel.getHighLightColor());
                drawLine(g, pos + panel.getWidth(), 2, pos + panel.getWidth(), thisHeight - 1);
                drawLine(g, pos, thisHeight - 1, pos + panel.getWidth(), thisHeight - 1);
            }
            if(st==SDAConsts.pbNone){
                g.setColor(borderColor);
                drawRect(g, pos, 2, panel.getWidth(),thisHeight-3);
            }
            //字
            st1=panel.getAlignType();
            if(st1==SDAConsts.alignLeft){
                SetClip(g, pos, 2, panel.getWidth(),thisHeight-3);
                g.setColor(foreColor);
                drawString(g, panel.getText(), pos+1, (thisHeight-fontHeight)/2+1);
                SetClip(g);
            }
            if(st1==SDAConsts.alignCenter){
                SetClip(g, pos, 2, panel.getWidth(),thisHeight-3);
                g.setColor(foreColor);
                drawString(g, panel.getText(), pos+(panel.getWidth()-getFont().stringWidth(panel.getText()))/2, (thisHeight-fontHeight)/2+1);
                SetClip(g);
            }
            if(st1==SDAConsts.alignRight){
                SetClip(g, pos, 2, panel.getWidth(),thisHeight-3);
                g.setColor(foreColor);
                drawString(g, panel.getText(), pos+(panel.getWidth()-getFont().stringWidth(panel.getText())), (thisHeight-fontHeight)/2+1);
                SetClip(g);
            }
            pos+=panel.getWidth()+2;
        }
        PaintChilds();
    }
    //列处理
    public SDAStatusPanel addStatusPanel(String panelText){
        SDAStatusPanel panel=new SDAStatusPanel();
        panel.setText(panelText);
        panel.setParentBar(this);
        statusPanels.addElement(panel);
        paint();
        return panel;
    }
    public void addStatusPanel(SDAStatusPanel panel){
        if(!statusPanels.contains(panel)){
            statusPanels.addElement(panel);
            panel.setParentBar(this);
            paint();
        }
    }
    public void removeStatusPanel(int panelIndex){
        if(panelIndex>-1&&panelIndex<statusPanels.size()){
            statusPanels.removeElementAt(panelIndex);
            paint();
        }
    }
    public void removeStatusPanel(SDAStatusPanel panel){
        if(!statusPanels.contains(panel)){
            statusPanels.removeElement(panel);
        }
    }
    public int getStatusPanelCount(){
        return statusPanels.size();
    }
    public SDAStatusPanel getStatusPanel(int panelIndex){
        SDAStatusPanel result=null;
        if(panelIndex>-1&&panelIndex<statusPanels.size()){
            result=(SDAStatusPanel)statusPanels.elementAt(panelIndex);
        }
        return result;
    }
}
