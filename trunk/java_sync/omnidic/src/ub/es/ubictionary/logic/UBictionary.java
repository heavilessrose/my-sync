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

package ub.es.ubictionary.logic;

import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import ub.es.ubictionary.visual.*;

/**
 * @author  aram
 * @version
 */
public class UBictionary extends MIDlet {
    public SearchCanvas pBusqueda;
    public MenuCanvas pMenu;
    public Config cfg;
    private Display pantalla;
    
    public UBictionary(){
        
        cfg = new Config();
        pantalla = Display.getDisplay(this);
        pBusqueda = new SearchCanvas(this);
        pMenu = new MenuCanvas(this);
        //FileReader fl = new FileReader(this.getClass().getResourceAsStream("/prueba.txt"));
        
    }
    public void startApp() {
        pantalla.setCurrent(pMenu);
    }
    
    public void pauseApp() {
    }
    
    public void destroyApp(boolean unconditional) {
    }
    
    public void exitMIDlet(){
        destroyApp(true);
        notifyDestroyed();
    }
}
