/*
 * Copyright (C) 2007 Aram Julhakyan
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as published by
 * the Free Software Foundation.
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You can find the completa text of the license in LICENSE.TXT distributed with Omnidic's source files.
 */

package ub.es.ubictionary.visual;

import java.io.IOException;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;
import ub.es.ubictionary.logic.UBictionary;

/**
 *
 * @author aram
 */
public class AboutCanvas extends Canvas implements CommandListener{
    
    UBictionary midlet;
    Command cmdVolver;
    
    /** Creates a new instance of KeyboardCanvas */
    public AboutCanvas(UBictionary midlet) {
        this.midlet = midlet;
        cmdVolver = new Command(midlet.cfg.getPalabra("volver"), Command.BACK, 2);
        addCommand(cmdVolver);
        setCommandListener(this);
        repaint();
    }
    
    protected void keyPressed(int keyCode) {
        if (FIRE == getGameAction(keyCode) || keyCode == KEY_NUM5){
            Display.getDisplay(midlet).setCurrent(midlet.pMenu);
        }
    }
    
    public void commandAction(Command command, Displayable displayable) {
        if (command == cmdVolver){  //Mostar el Menu principal
            Display.getDisplay(midlet).setCurrent(midlet.pMenu);
        }
    }

    protected void paint(Graphics g) {
        g.setColor(255, 255, 255);
        g.fillRect(0, 0, getWidth(), getHeight());
        int sh = Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_MEDIUM).getHeight()+3;
        g.setColor(0, 0, 128);
        g.fillRect(0, 0, getWidth(), sh);
        int th = sh;
        
        g.setColor(255, 255, 255);
        g.drawString(midlet.cfg.getPalabra("about"), getWidth()/2, 1, g.TOP|g.HCENTER);
        sh = Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_MEDIUM).getHeight();
        g.setFont(Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_PLAIN, Font.SIZE_SMALL));
        
        g.setColor(0, 0, 0);
        th += sh;
        g.drawString("Developed by:", getWidth()/2, th, g.BOTTOM|g.HCENTER);
        g.setColor(128, 128, 128);
        th+=sh + 1;
        g.drawString("Aram Julhakyan", getWidth()/2, th, g.BOTTOM|g.HCENTER);
        g.setColor(0, 0, 0);
        th+=sh + 3;
        g.drawString("More info in:", getWidth()/2, th, g.BOTTOM|g.HCENTER);
        g.setColor(128, 128, 128);
        th+=sh + 1;
        g.drawString("www.openmobiledictionary.com", getWidth()/2, th, g.BOTTOM|g.HCENTER);
        
        th+=sh + 1;
        g.drawString("www.omnidic.com", getWidth()/2, th, g.BOTTOM|g.HCENTER);
    }
    
}
