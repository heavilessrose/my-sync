package com.winksi.j2me.cc.view;

import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.tools.ConstImage;
import com.winksi.j2me.cc.tools.StringTools;
import java.util.Vector;
import javax.microedition.lcdui.Graphics;

/**
 *
 * @author Administrator
 */
public class AlertBoard {

    private Vector alertinfoVector = new Vector();
//    private String alertInfo="";
    private int PROGRESS_Y = (CPProperty.SCREENHEIGHT - ConstImage.alertbg.getHeight()) / 2 + 58;
//    private int count=0;
    public AlertBoard() {

    }

    public void drawBorad(Graphics g, int count) {
        g.drawImage(ConstImage.alertbg, 0, (CPProperty.SCREENHEIGHT - ConstImage.alertbg.getHeight()) / 2, 20);
        g.setColor(CPProperty.COLOR_CHAR_BLACK);
        if (!alertinfoVector.isEmpty()) {
            for (int i = 0; i < alertinfoVector.size(); i++) {
                g.drawString(alertinfoVector.elementAt(i).toString(), (CPProperty.SCREENWIDTH - CPProperty.defaultFont.stringWidth(alertinfoVector.elementAt(i).toString())) / 2, (CPProperty.SCREENHEIGHT - ConstImage.alertbg.getHeight()) / 2 + 13+(CPProperty.dfheight+1)*i, 20);
            }
        }
//        g.drawString(alertInfo, (CPProperty.SCREENWIDTH-CPProperty.defaultFont.stringWidth(alertInfo))/2,(CPProperty.SCREENHEIGHT-ConstImage.alertbg.getHeight())/2+13, 20);
        if (count > -1) {
            g.setClip((CPProperty.SCREENWIDTH - ConstImage.lprogress2.getWidth()) / 2, (PROGRESS_Y + ConstImage.lprogress2.getHeight()) - count * (ConstImage.lprogress2.getHeight()) / 100, ConstImage.lprogress2.getWidth(), ConstImage.lprogress2.getHeight() - 14);
            g.drawImage(ConstImage.lprogress2, (CPProperty.SCREENWIDTH - ConstImage.lprogress2.getWidth()) / 2, PROGRESS_Y, 20);
            g.setClip(0, 0, CPProperty.SCREENWIDTH, CPProperty.SCREENHEIGHT);
            g.drawImage(ConstImage.lprogress1, (CPProperty.SCREENWIDTH - ConstImage.lprogress1.getWidth()) / 2, PROGRESS_Y, 20);
        }
    }

    public void setAlertInfo(String s) {
        alertinfoVector = new Vector();
//        this.alertInfo=s;
        alertinfoVector = StringTools.getSubsection(s, CPProperty.defaultFont, ConstImage.alertbg.getWidth() - 4, "");
    }
}
