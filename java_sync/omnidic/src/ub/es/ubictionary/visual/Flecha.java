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

import javax.microedition.lcdui.Graphics;

/**
 *
 * @author aram
 */
public class Flecha {
    
    /** Creates a new instance of Flecha */
    private int x, y, size;
    private boolean arriba;

    public void setX(int x) {
        this.x = x;
    }

    public void setArriba(boolean arriba) {
        this.arriba = arriba;
    }

    public void setY(int y) {
        this.y = y;
    }

    public Flecha(int x, int y, int size) {
        this.x = x;
        this.y = y;
        this.size = size;
        arriba = true;
    }

    public void render(Graphics g){
      
        for (int i = 0; i < size; i++){
            if (arriba){
                g.drawLine(x+i, y, x+i, y-i);
                g.drawLine(x+i+size, y, x+i+size, y-size+i);
            }else{
                g.drawLine(x+i, y, x+i, y+i);
                g.drawLine(x+i+size, y, x+i+size, y+size-i);
            }
        }
        /*for (int i = 0; i < 5; i++){
            g.drawLine(x+i, y, x+7, y-10 + i);
            g.drawLine(x+7, y-i-5, x+9+i ,y);
        }*/
    }
    
}
