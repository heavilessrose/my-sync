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
public class ResultCanvas extends Canvas  implements CommandListener {
    
    private int firstVisible, lastVisible, lineasVisibles;
    private String textoBuscado, textoResultado;
    private UBictionary midlet;
    private Command cmdVolver;
    /** Creates a new instance of ResultCanvas */
    public ResultCanvas(UBictionary dict) {
        midlet = dict;
        cmdVolver = new Command(midlet.cfg.getPalabra("volver"), Command.BACK, 2);
        addCommand(cmdVolver);
        setCommandListener(this);
        
        firstVisible = 0;
        lineasVisibles = (getHeight()-Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_MEDIUM).getHeight()+2)/Font.getFont(Font.FACE_SYSTEM, Font.STYLE_PLAIN, Font.SIZE_SMALL).getHeight();
        lastVisible = lineasVisibles;
    }
    
    public void setTextoBuscado(String s){
        if (s.length() > 0)
            textoBuscado = s.substring(0,1).toUpperCase() + s.substring(1, s.length());
        else
            textoBuscado = "";
    }
    
    public void setResultado(String r){
        if (r !=null)
            textoResultado = r.substring(0, 1).toUpperCase() + r.substring(1, r.length());
        else
            textoResultado = "";
        
    }

    protected void paint(Graphics g) {
        g.setColor(255, 255, 255);
        g.fillRect(0, 0, getWidth(), getHeight());
        
        
        g.setColor(128, 0, 0); //-- Es el color del texto
        g.setFont(Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_MEDIUM));
        g.drawString(textoBuscado, 2, 2, g.TOP|g.LEFT);
        
        g.setColor(0, 0, 128); //-- Es el color del texto
        g.drawLine(1, Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_MEDIUM).getHeight()+3, getWidth()-1, Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_MEDIUM).getHeight()+3);
        
        g.setColor(21, 21, 21); //-- Es el color del texto
        g.setFont(Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_PLAIN, Font.SIZE_MEDIUM));
        this.mostrarTexto(g, textoResultado);
    }
    
    protected void keyPressed(int keyCode) {
        if (getGameAction(keyCode)==FIRE || keyCode == KEY_NUM5){
            Display.getDisplay(midlet).setCurrent(midlet.pBusqueda);
        }
        
        if (getGameAction(keyCode)==UP || keyCode==KEY_NUM2 || keyCode == KEY_NUM0){ //Hacer scroll arriba en la ayuda
            firstVisible--;
            if (firstVisible < 0)
                firstVisible = 0;
            lastVisible--;
            if (lastVisible < lineasVisibles)
                lastVisible = lineasVisibles;        
        }
        if (getGameAction(keyCode)==DOWN || keyCode==KEY_NUM8 || keyCode == KEY_POUND){ //Hacer scroll abajo ayuda
            firstVisible++;
            lastVisible++;
        }
        repaint();
    }

    public void commandAction(Command command, Displayable displayable) {
        if (command == cmdVolver){  //Mostar el Menu principal
            Display.getDisplay(midlet).setCurrent(midlet.pBusqueda);
        }
    }
    
    private void mostrarTexto(Graphics g, String texto){
        String longText = texto;
        Font myFont = Font.getFont(Font.FACE_SYSTEM, Font.STYLE_PLAIN, Font.SIZE_SMALL);
        LineEnumeration e = new LineEnumeration(myFont,longText,getWidth()-25);
        int startY = Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_MEDIUM).getHeight()+5;
        int i = 0;
        String linea="";
        while ( e.hasMoreElements() ){
            //System.out.println("First: " + firstVisible + " Last: " + lastVisible + " i: " + i);
            linea = e.nextElement().toString();
            if (i>=firstVisible && i <= lastVisible){
                g.drawString(linea, 2,startY, Graphics.TOP | Graphics.LEFT);
                startY += myFont.getHeight();
            }
            i++;
        } 
    }
    
}
