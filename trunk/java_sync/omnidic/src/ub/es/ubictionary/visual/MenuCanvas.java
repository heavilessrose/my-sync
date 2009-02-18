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

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.Vector;
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
public class MenuCanvas extends Canvas implements CommandListener {
    
    private UBictionary midlet;
    private Command cmdSalir, cmdAyuda, cmdVolver, cmdSi, cmdNo;
    private String []  menuItems;
    private int selectedItem;
    private int firstVisible, lastVisible, lineasVisibles; //-- Se usaran para hacer scroll en la ayuda
    private Flecha flecha;
    private boolean mostrarAyuda;
    private Vector ayuda;
    //private DiccionaryCanvas dictCanvas;
    //private KeyboardCanvas keyCanvas;
    public MenuCanvas(UBictionary midlet) {
      this.midlet = midlet;
      //dictCanvas = new DiccionaryCanvas(midlet);
      //keyCanvas = new KeyboardCanvas(midlet);
      firstVisible = 0;
      lineasVisibles = (getHeight()-Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_MEDIUM).getHeight()+2)/Font.getFont(Font.FACE_SYSTEM, Font.STYLE_PLAIN, Font.SIZE_SMALL).getHeight();
      lastVisible = lineasVisibles;
      
      menuItems = new String[7];
      menuItems[0] = "Search";
      menuItems[1] = "Ditionary";
      menuItems[2] = "Keyboard";
      menuItems[3] = "Language";
      menuItems[4] = "Help";
      menuItems[5] = "About";
      menuItems[6] = "Exit";
      selectedItem = 0;
      
      mostrarAyuda = false;
      
      flecha = new Flecha(getWidth()/2 -8, 49, 7);
     
      cmdSalir = new Command("Exit", Command.EXIT, 2);
      cmdAyuda = new Command("Help", Command.HELP, 1);
      cmdVolver = new Command("Back", Command.BACK, 1);
      addCommand(cmdSalir);
      addCommand(cmdAyuda);
      
      setCommandListener(this);
      setLanguage();
    }
    
    public void setLanguage(){
        removeCommand(cmdSalir);
        removeCommand(cmdAyuda);
        cmdSalir = new Command(midlet.cfg.getPalabra("salir"), Command.EXIT, 2);
        cmdAyuda = new Command(midlet.cfg.getPalabra("ayuda"), Command.HELP, 1);
        cmdVolver = new Command(midlet.cfg.getPalabra("volver"), Command.BACK, 1);
        addCommand(cmdSalir);
        addCommand(cmdAyuda);
          menuItems[0] = midlet.cfg.getPalabra("buscar");
          menuItems[1] = midlet.cfg.getPalabra("diccionario");
          menuItems[2] = midlet.cfg.getPalabra("teclado");
          menuItems[3] = midlet.cfg.getPalabra("idioma");
          menuItems[4] = midlet.cfg.getPalabra("ayuda");
          menuItems[5] = midlet.cfg.getPalabra("about");
          menuItems[6] = midlet.cfg.getPalabra("salir");
    }

    protected void paint(Graphics g) {
        g.setColor(255, 255, 255);
        g.fillRect(0, 0, getWidth(), getHeight());
        //g.setColor(128, 64, 0); //-- Es el color del texto
        if (mostrarAyuda == false){
            
            //Pinto el nombre del diccionairo
            g.setColor(128, 0, 0);
            g.setFont(Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_LARGE));
            g.drawString("Omnidic", getWidth()/2, 20, g.BASELINE|g.HCENTER);
            
            //pinto la direccion de la web
            g.setFont(Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_PLAIN, Font.SIZE_SMALL));
            
             g.drawString("www.omnidic.com", getWidth()/2, getHeight() - Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_PLAIN, Font.SIZE_SMALL).getHeight()-5, g.BASELINE|g.HCENTER);
            
            g.setColor(87, 87, 87);
            g.setFont(Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_PLAIN, Font.SIZE_LARGE));
            g.drawString(menuItems[selectedItem], getWidth()/2, getHeight()/2, g.BASELINE|g.HCENTER);
            
            g.setColor(0, 0, 128);
            if(selectedItem != 0){
                flecha.setArriba(true);
                flecha.setY(getHeight()/2-Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_PLAIN, Font.SIZE_LARGE).getHeight()/2 - 4);
                flecha.render(g);
            }

            if (selectedItem != menuItems.length-1){
                flecha.setY(getHeight()/2+4);
                flecha.setArriba(false);
                flecha.render(g);
            }
        }
        if (mostrarAyuda){
//--Titulo
           g.setColor(0, 0, 128);
            g.fillRect(0, 0, getWidth(), Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_MEDIUM).getHeight()+3);

            g.setColor(255, 255, 255);
            g.drawString(midlet.cfg.getPalabra("ayuda"), getWidth()/2, 1, g.TOP|g.HCENTER);
            g.setColor(60, 60, 60);
            g.setFont(Font.getFont(Font.FACE_SYSTEM, Font.STYLE_PLAIN, Font.SIZE_SMALL));
            mostrarTexto(g);
        }
        
    }
    protected void keyPressed(int keyCode) {
        //System.out.println(keyCode);
        
        if (mostrarAyuda == false){ //-- Cuando estamos en Menu Principal
            firstVisible = 0;
            lastVisible = lineasVisibles;
            if (getGameAction(keyCode)==UP || keyCode==KEY_NUM2 || keyCode == KEY_NUM0){ //Si se ha pulsado arriba
                selectedItem--;
                if (selectedItem < 0)
                    selectedItem = 0;
            }
            if (getGameAction(keyCode)==DOWN || keyCode==KEY_NUM8 || keyCode == KEY_POUND){ // Si se ha pulsado abajo
                selectedItem++;
                if (selectedItem >= menuItems.length)
                    selectedItem = menuItems.length-1;
            }
            if (getGameAction(keyCode)==FIRE || keyCode == KEY_NUM5){ //Si se ha pulsado aceptar/ok
                switch(selectedItem){
                    case 0: //-- Buscar
                        midlet.pBusqueda.inicializar();
                        Display.getDisplay(midlet).setCurrent(midlet.pBusqueda);
                        break;
                    case 1: //Mostrar seleccion de diccionario
                        Display.getDisplay(midlet).setCurrent(new DictionaryCanvas(midlet, midlet.cfg.getDictionary()));
                        break;
                    case 2:
                        Display.getDisplay(midlet).setCurrent(new KeyboardCanvas(midlet, midlet.cfg.getSelectedKeyboard()));
                        break;
                    case 3:
                        Display.getDisplay(midlet).setCurrent(new InterfaceCanvas(midlet, midlet.cfg.getLanguage()));
                        break;
                    case 4: //-- Ayuda
                        mostrarAyuda=true;
                        removeCommand(cmdSalir);
                        removeCommand(cmdAyuda);
                        addCommand(cmdVolver);
                        cargarAyuda(midlet.cfg.getPalabra("texto_ayuda"));
                        break;
                    case 5: //-- Acerca de 
                        Display.getDisplay(midlet).setCurrent(new AboutCanvas(midlet));
                        break;
                    case 6: //-- Salir
                        midlet.exitMIDlet();
                }

            }
        }else{
            
            if (mostrarAyuda){
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
                if (getGameAction(keyCode)==FIRE || keyCode == KEY_NUM5){ //Salir de la ayuda
                    if (mostrarAyuda){
                        mostrarAyuda = false;
                        removeCommand(cmdVolver);
                        addCommand(cmdSalir);
                        addCommand(cmdAyuda);
                        ayuda = null;
                    }
                    
                }
            }
        }
        
        repaint();
    }

    public void commandAction(Command command, Displayable displayable) {
        if (command == cmdSalir) //Salir
            midlet.exitMIDlet();
        if (command == cmdAyuda){
            mostrarAyuda = true;
            removeCommand(cmdSalir);
            removeCommand(cmdAyuda);
            addCommand(cmdVolver);
            firstVisible = 0;
            lastVisible = lineasVisibles;
            cargarAyuda(midlet.cfg.getPalabra("texto_ayuda"));
        }
        if (command == cmdVolver){
            mostrarAyuda = false;
            removeCommand(cmdVolver);
            addCommand(cmdSalir);
            addCommand(cmdAyuda);
            ayuda=null;
        }
       
        repaint();
    }
    
    private void mostrarTexto(Graphics g){
        Font myFont = Font.getFont(Font.FACE_SYSTEM, Font.STYLE_PLAIN, Font.SIZE_SMALL);
        int startY = Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_MEDIUM).getHeight()+2;
        
        for (int i = 0; i < ayuda.size(); i++){
            //System.out.println("First: " + firstVisible + " Last: " + lastVisible + " i: " + i);
            
            if (i>=firstVisible && i <= lastVisible){
                g.drawString(ayuda.elementAt(i).toString(), 2,startY, Graphics.TOP | Graphics.LEFT);
                startY += myFont.getHeight();
            }
            if (i >= lastVisible)
                break;
        } 
    }
    
     private void cargarAyuda(String texto){
        Font myFont = Font.getFont(Font.FACE_SYSTEM, Font.STYLE_PLAIN, Font.SIZE_SMALL);
        LineEnumeration e = new LineEnumeration(myFont,texto,getWidth()-40);
        ayuda = new Vector(15);
        while ( e.hasMoreElements() ){
            ayuda.addElement(e.nextElement());
        } 
    } 
}
