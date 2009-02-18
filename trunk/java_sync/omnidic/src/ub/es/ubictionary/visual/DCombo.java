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

import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;

/**
 *
 * @author aram
 */
public class DCombo extends DControl{
     private Flecha fl;
     private int selectedItem;
     private String [] items;
     private DList  lista;
     private Font fuente;
     
    public DCombo(int x, int y, int ancho, int alto, int screenHeight) {
        super(x, y, ancho, alto);
        
        fl = new Flecha(x+ancho-16, y+alto-4, 7);
        selectedItem = 0; // Por defecto el primero
        fuente = Font.getFont(Font.FACE_SYSTEM, Font.STYLE_PLAIN, Font.SIZE_SMALL);
        lista = new DList(1, 1, ancho+3, screenHeight-1);
        lista.setVisible(false);
        lista.setSelected(selectedItem);
    }
    
    public void render(Graphics g){
        if (hasFocus())
            g.setColor(216, 232, 233);
        else
            g.setColor(252, 225, 199);
        g.fillRoundRect(x+1, y+1, ancho-1, alto-1, 3, 3);
        
//-- dibujo el borde
        g.setColor(128, 64, 0);
        g.drawRoundRect(x, y, ancho, alto, 3, 3);
        
        //Dibujo el boton de la flecha
        g.drawRoundRect(x+ancho-18, y+1, 17, 15, 3, 3);
        g.setColor(255, 255, 255);
        g.fillRoundRect(x+ancho-17, y+2, 16, 14, 3, 3);
        fl.render(g);
        
//-- Dibujo el texto --
        g.setColor(128, 64, 0);
        g.setFont(fuente);
        g.drawString(recortarString(items[selectedItem], ancho-20), x+2, y+2, g.TOP|g.LEFT);
  
        lista.render(g);
        
    }
    
    public void gestionarEvento(int cod){
        if (lista.isVisible()){
            lista.gestionarEvento(cod);
            selectedItem = lista.getSelected();
        }else{
            switch (cod){
                case 50:
                    selectedItem--;
                    if (selectedItem < 0)
                        selectedItem = 0;
                    lista.setSelected(selectedItem);
                    break;
                case 56:
                    selectedItem++;
                    if (selectedItem >= items.length)
                        selectedItem = items.length-1;
                    lista.setSelected(selectedItem);
                    break;
                case 53:
                    lista.setVisible(true);
                    break;
                case -5:
                    lista.setVisible(true);
                    break;
            }
        }
    }
    
    public void setItems(String [] items){
        this.items = items;
        lista.setItems(items);
    }
    
    public boolean isListExpanded(){
        return lista.isVisible();
    }
    
    public String getSelectedText(){
        return items[selectedItem];
    }
    
    private String recortarString(String cad,  int a){
        String s = cad;
        while(fuente.stringWidth(s) > a){
            s = s.substring(0, s.length()-1);
            System.out.println(s);
        }
        return s;
    }
}
