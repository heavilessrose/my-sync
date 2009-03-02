/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.winksi.j2me.cc.view;

import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.control.GUIController;
import com.winksi.j2me.cc.tools.ConstImage;
import com.winksi.j2me.cc.tools.StringTools;
import java.util.Vector;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Graphics;

/**
 *
 * @author Administrator
 */
public class HelpCanvas extends Canvas{
    private GUIController controller;
    private Vector infoVector=new Vector();
    private int infoindex=0;
    private int maxindex=0;
    private final int showwidthmax=200;
    private final int showheightmax=200;
    private final int info_x=20;
    private final int info_y=72;
    private String title="";
    
    public HelpCanvas(GUIController controller){
        setFullScreenMode(true);
        this.controller=controller;
    }

    protected void paint(Graphics g) {
        g.drawImage(ConstImage.main, 0, 0, 20);
        g.setColor(CPProperty.COLOR_CHAR_WHITE);
        g.drawString(title, (CPProperty.SCREENWIDTH - CPProperty.defaultFont.stringWidth(title)) >> 1, CPProperty.TITLE_OFF_Y, 20);
        
        if(!infoVector.isEmpty()){
            g.setClip(info_x, info_y, showwidthmax, showheightmax);
            for(int i=0;i<infoVector.size();i++){
                g.drawString((infoVector.elementAt(i).toString()).trim(), info_x, info_y+(CPProperty.dfheight+CPProperty.ROW_SPACE)*(i-infoindex), 20);
            }
            g.setClip(0, 0, CPProperty.SCREENWIDTH, CPProperty.SCREENHEIGHT);
        }
        drawButton(g);
    }
    
    public void drawButton(Graphics g) {
        g.setColor(CPProperty.COLOR_CHAR_WHITE);
        g.drawString(CPProperty.COMMAND_BACK[controller.getLanguage()], CPProperty.SCREENWIDTH - CPProperty.defaultFont.stringWidth(CPProperty.COMMAND_BACK[controller.getLanguage()]) - 1, CPProperty.SCREENHEIGHT - CPProperty.dfheight - 2, 20);
    }
    
    public void setString(String title,String s){
        this.title=title;
        infoVector=new Vector();
        infoVector=StringTools.getSubsection(s, CPProperty.defaultFont, showwidthmax, ".,;?!");
        
        infoindex=0;
        maxindex=infoVector.size()-CPProperty.MAX_LINE+1;
        if(maxindex<0){
            maxindex=0;
        }
    }
    
    protected void keyPressed(int keyCode){
        if(keyCode==CPProperty.KEY_BACK||keyCode==CPProperty.KEY_SOFT_RIGHT){
            controller.handleEvent(GUIController.EventID.EVENT_SHOW_MAIN, null);
        }else if(keyCode==CPProperty.KEY_NUM2||keyCode==CPProperty.KEY_UP){
            infoindex--;
            if(infoindex<0){
                infoindex=0;
            }
        }else if(keyCode==CPProperty.KEY_NUM8||keyCode==CPProperty.KEY_DOWN){
            infoindex++;
            if(infoindex>maxindex){
                infoindex=maxindex;
            }
        }
        repaint();
    }

}
