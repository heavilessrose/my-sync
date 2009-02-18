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
import ub.es.ubictionary.logic.Keyboard;

/**
 *
 * @author aram
 */
public class DTextField extends DControl {
    
    
    private StringBuffer contenido;  //Contenido del textBox
    private String contenidoMostrado;
    //private int x, y; //Posición del text box
    //private int ancho, alto; //Tamaño del textbox
    //private boolean focus; //indicara si el control esta seleccionado
    private Font fuente; //fuente del textbox
    private int cursorPos; //Posicion del cursor en el texto
    private int cpos; //Posicion fisica del cursor
    private String[] keys = {"-_@", "abcáàäABC", "deféèäDEF", "ghiíìïGHI", "jklJKL", "mnoñóòöMNO", "pqrsPQRS", "tuvúùüTUV", "wxyzWXYZ"}; 
    private int lastKey = 0, keyIndex=0; //variables para trabajar con las repeticiones de las teclas
    private long tick;
    
    public DTextField(String contenido, String[] teclado, int x, int y, int ancho, int alto) {
        super(x, y, ancho, alto);
        
        this.contenido = new StringBuffer(contenido);
        
        keys = teclado;
        fuente = Font.getFont(Font.FACE_SYSTEM, Font.STYLE_PLAIN, Font.SIZE_SMALL);
        cursorPos = contenido.length();
        cpos = fuente.substringWidth(contenido, 0, cursorPos);
    }
    
    public void setKeyboard(String [] teclado){
        keys = teclado;
    }
    public void render(Graphics g){
//-- dibujo el fondo del textfield
            g.setColor(223, 223, 223);
        
        
        g.fillRect(x, y, ancho, alto);
        
        if (hasFocus()){
//-- Dibujo el cursor
            g.setColor(0x00000000);
            g.drawLine(x+2+cpos, y + 2, x+2+cpos, y+alto-2);
        }
        
//-- dibujo el borde del textbox
        //g.setColor(128, 64, 0);
        //g.drawRoundRect(x, y, ancho, alto, 3, 3);
        
//-- Dibujo el texo en el textbox
        g.setColor(128, 0, 0);
        g.setFont(fuente);
        g.drawString(contenido.toString(), x+3, y+2, g.TOP|g.LEFT);
        

    }
    
  
    
    public void gestionarEvento(int cod){
        
        if (Keyboard.isASCII(cod) && ancho - fuente.stringWidth(contenido.toString())>7){
            if ('\b' == (char) cod){
                //backspace
                contenido.delete(cursorPos-1,cursorPos);
                cursorPos--;
            }else{
                cursorPos++;
                contenido.insert(cursorPos-1, (char) cod);
            }
        }
         
         if (cod >= Canvas.KEY_NUM1 && cod <= Canvas.KEY_NUM9 && ancho - fuente.stringWidth(contenido.toString())>7){  //Gestion de entrada de letras
            if (cod == lastKey){
                if (System.currentTimeMillis() - tick < 900){
                    keyIndex ++;
                    contenido.delete(cursorPos-1,cursorPos);
                }else{
                    keyIndex = 0;
                    cursorPos++;
                }
            }else{
                keyIndex = 0;
                cursorPos++;
            }
            
            int pushedKey=0;
            
            switch(cod){
                case Canvas.KEY_NUM1: pushedKey=0; break;
                case Canvas.KEY_NUM2: pushedKey=1; break;
                case Canvas.KEY_NUM3: pushedKey=2; break;
                case Canvas.KEY_NUM4: pushedKey=3; break;
                case Canvas.KEY_NUM5: pushedKey=4; break;
                case Canvas.KEY_NUM6: pushedKey=5; break;
                case Canvas.KEY_NUM7: pushedKey=6; break;
                case Canvas.KEY_NUM8: pushedKey=7; break;
                case Canvas.KEY_NUM9: pushedKey=8; break;
            }

            if (keys[pushedKey].length() == keyIndex)
                keyIndex = 0;

            contenido.insert(cursorPos-1, keys[pushedKey].charAt(keyIndex));
         }else{
             if (cod == Canvas.KEY_STAR || cod == -8){  //Borrar letrass
                 if (cursorPos != 0){
                     contenido.delete(cursorPos-1,cursorPos);
                     cursorPos--;
                 }
             }else{ // -- MOvimiento del cursor
                 switch (cod){
                     case -3:
                         if (cursorPos !=0)
                             cursorPos--;
                         break;
                     case -4:
                         if (cursorPos < contenido.length())
                             cursorPos++;
                 }
             }
        }
        cpos = fuente.substringWidth(contenido.toString(), 0, cursorPos);
        lastKey = cod;
        tick = System.currentTimeMillis();
    }

    
    public String getText(){
        return contenido.toString();
    }
    
    public void setText(String text){
        this.contenido.delete(0, contenido.length());
        if (text !=null)
            contenido.append(text);
        cursorPos = contenido.length();
        cpos = fuente.substringWidth(contenido.toString(), 0, cursorPos);
        
    }
   
}
