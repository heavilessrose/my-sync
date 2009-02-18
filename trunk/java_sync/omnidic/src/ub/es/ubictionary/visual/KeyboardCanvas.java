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
import ub.es.ubictionary.logic.Keyboard;
import ub.es.ubictionary.logic.UBictionary;

/**
 *
 * @author aram
 */
public class KeyboardCanvas extends Canvas implements CommandListener{
    
    DList lista;
    UBictionary midlet;
    Command cmdVolver;
    
    /** Creates a new instance of KeyboardCanvas */
    public KeyboardCanvas(UBictionary midlet, String selectedKeyboard) {
        this.midlet = midlet;
        lista = new DList(1, Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_MEDIUM).getHeight()+5, getWidth()-5, getHeight() - (Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_MEDIUM).getHeight()+3));
        lista.setItems(midlet.cfg.getKyeboards());
        lista.setSelectedByValue(selectedKeyboard);
        
        
        cmdVolver = new Command(midlet.cfg.getPalabra("aceptar"), Command.BACK, 2);
        addCommand(cmdVolver);
        setCommandListener(this);
    }
    
    protected void keyPressed(int keyCode) {
        lista.gestionarEvento(Keyboard.getValidKeyCode(getGameAction(keyCode), keyCode));
        
        repaint();
    }
    
    public void commandAction(Command command, Displayable displayable) {
        if (command == cmdVolver){  //Mostar el Menu principal
            midlet.cfg.setKeyboard(lista.getSelectedText());
            Display.getDisplay(midlet).setCurrent(midlet.pMenu);
        }
    }

    protected void paint(Graphics g) {
        g.setColor(255, 255, 255);
        g.fillRect(0, 0, getWidth(), getHeight());
        
        g.setColor(0, 0, 128);
        g.fillRect(0, 0, getWidth(), Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_MEDIUM).getHeight()+3);
        
        g.setColor(255, 255, 255);
        g.drawString(midlet.cfg.getPalabra("teclado"), getWidth()/2, 1, g.TOP|g.HCENTER);
        
        lista.render(g);
    }
    
}
