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
import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;

/**
 *
 * @author aram
 */
public class DList extends DControl {
    
    private String[] contenido;
    private int itemsVisibles; //Numero de items visibles en la lista
    private int selectedItem; //El item seleccionado
    private int hoveredItem; //Item que tiene la sombra
    private int firstVisible, lastVisible; //Con esto sabre la parte visible de la lista
    private boolean seleccionable; //-- Una propiedad de la lista que indica si se puede selccionar un item
    
    private int itemSize; //indica el alto de cada item para pintar

    
    
    public DList(int x, int y, int ancho, int alto) {
        super(x, y, ancho, alto);
        itemsVisibles = (alto-1)/15;
        itemSize = (alto-1)/itemsVisibles;
        selectedItem = 0;
        hoveredItem = 0;
        firstVisible = 0;
        lastVisible = itemsVisibles-1;
        seleccionable = true;
        contenido = null;
    }

    public void render (Graphics g){
        if (isVisible()){
            
            
            g.setColor(255, 255, 255);
            g.fillRect(x+1, y+1, ancho-1, alto-1);
            //g.drawRoundRect(x, y, ancho, alto, 3, 3);

            g.setFont(Font.getFont(Font.FACE_SYSTEM, Font.STYLE_PLAIN, Font.SIZE_SMALL));
//-- Pinto el texto de la lista
            int y1 = y+1;
            if (contenido !=null)
            for(int i = firstVisible; i <= lastVisible; i++){
                if (i < contenido.length){

                    /*if (i == selectedItem){
                        g.setColor(190, 190, 190);
                        if (seleccionable)
                            g.fillRoundRect(x+1, y1, ancho-1, itemSize, 3, 3);
                    }*/
                    if (i == hoveredItem){
                        g.setColor(49, 106, 197);
                        g.fillRect(x+1, y1, ancho-1, itemSize);
                        g.setColor(255, 255, 255);
                    }else{
                        g.setColor(0, 0, 0);
                    }
                    
                    g.drawString(contenido[i], x+2, y1+2, g.TOP|g.LEFT);
                    y1 += itemSize;
                }
            }
        }
    }
    
     public void gestionarEvento(int cod){
        /*switch (cod){
            case -1:
                hoveredItem--;
                if (hoveredItem < 0)
                    hoveredItem = 0;
                break;
            case -2:
                hoveredItem++;
                if (hoveredItem >= contenido.length)
                    hoveredItem = contenido.length-1;
                break;
            case -5:
                selectedItem = hoveredItem;
                break;
        }*/
        
        //Subir en la lista 
        if (cod== Canvas.UP || cod==Canvas.KEY_NUM0){
        ///if (cod==-1 || cod==-59 || cod==35){
            hoveredItem--;
            if (hoveredItem < 0)
                hoveredItem = 0;
        }
        
        //Bajar en la lista
        if (cod== Canvas.DOWN || cod==Canvas.KEY_POUND){
        //if (cod == -2 || cod == -60 || cod==48){
            hoveredItem++;
            if (contenido != null && hoveredItem >= contenido.length)
                hoveredItem = contenido.length-1;
        }
        
        //Seleccionar un elemento de la lista
        if (cod == -5 || cod == -26)
            selectedItem = hoveredItem;
        
        if (hoveredItem > lastVisible){
                    firstVisible++;
                    lastVisible++;
                }
        if (hoveredItem < firstVisible){
            firstVisible--;
            lastVisible--;
        }
    }
   
    public void setSelected(int s){
        selectedItem = s;
    }
    
    public void setSelectedByValue(String s){
        if (contenido !=null){
            for(int i=0; i < contenido.length; i++){
                if(s.compareTo(contenido[i])==0){
                    selectedItem = i;
                    hoveredItem = i;
                    break;
                }
            }
        }
    }
    
    public int getSelected(){
        return selectedItem;
    }
    
    public String getSelectedText(){
        if (contenido !=null)
            return contenido[hoveredItem];
        else
            return "";
    }
    
    public void setItems(String [] items){
        contenido = items;
        hoveredItem = 0;
        selectedItem = 0;
        firstVisible = 0;
        lastVisible = itemsVisibles-1;
        /*
        if (items == null){
            contenido = new String[1];
            contenido[0] = "";
        }*/
    }
    public void setSeleccionable(boolean seleccionable) {
        this.seleccionable = seleccionable;
    }
}
