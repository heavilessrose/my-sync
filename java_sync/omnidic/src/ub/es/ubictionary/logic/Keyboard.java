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

import javax.microedition.lcdui.Canvas;

/**
 *
 * @author aram
 */
public class Keyboard {
    
    /** Creates a new instance of Keyboard */
    public Keyboard() {
    }
    
    public static int getValidKeyCode(int gameCode, int rawCode){
        if (rawCode == Canvas.KEY_NUM0 || rawCode == Canvas.KEY_NUM1 || rawCode == Canvas.KEY_NUM2 || rawCode == Canvas.KEY_NUM3 
                || rawCode == Canvas.KEY_NUM4 || rawCode == Canvas.KEY_NUM5 || rawCode == Canvas.KEY_NUM6 || rawCode == Canvas.KEY_NUM7 
                || rawCode == Canvas.KEY_NUM8 || rawCode == Canvas.KEY_NUM9 || rawCode == Canvas.KEY_POUND || rawCode == Canvas.KEY_STAR){
            
            return rawCode;
        }else{
            if (gameCode !=0)
                return gameCode;
            else
                return rawCode;
        }
    }
    
    public static boolean isTextButtonPushed(int keyCode)
    {
        if (keyCode==Canvas.KEY_NUM1 || keyCode==Canvas.KEY_NUM2 || keyCode==Canvas.KEY_NUM3 || keyCode==Canvas.KEY_NUM4 
                 || keyCode==Canvas.KEY_NUM5 || keyCode==Canvas.KEY_NUM6 || keyCode==Canvas.KEY_NUM7 || keyCode==Canvas.KEY_NUM8
                 || keyCode==Canvas.KEY_NUM9  || keyCode==Canvas.KEY_STAR || keyCode==-8 || isASCII(keyCode))
            return true;
        else
            return false;
    }
    
    public static boolean isASCII(int c){
        //if c = space or c = backspace or c = A_Za_z
        if (c == 32 || c == 8 || (c>= 65 && c <= 90) || (c >=97 && c <=122)){
            return true;
        }
        return false;
    }
}
