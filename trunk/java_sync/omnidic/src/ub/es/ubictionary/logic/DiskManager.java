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

import java.io.UnsupportedEncodingException;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;

/**
 *
 * @author aram
 */
public class DiskManager {
    
    /** Creates a new instance of DiskManager */
    private final int dictionary = 1;
    private final int language = 2;
    private final int keyboard = 3;
    private String slanguage, skeyboard, sdictionary;
    public DiskManager() {
        slanguage = "";
        skeyboard = "";
        sdictionary = "";
        RecordStore rs = null;
        try {
            rs = RecordStore.openRecordStore( "preferencias", true );
            if (rs.getNextRecordID() == 1){ //Inicializar el recordstore con valores por defecto
                String s = "nada";
                byte[] data = s.getBytes();
                rs.addRecord( data, 0, data.length );
                rs.addRecord( data, 0, data.length );
                rs.addRecord( data, 0, data.length );
            }else{ //leer los datos guardados 
                byte [] data;
                data  = rs.getRecord(language);
                if (data !=null)
                    
                    try {
                        slanguage = new String(data, "UTF-8");
                    } catch (UnsupportedEncodingException ex) {
                        ex.printStackTrace();
                    }
                data = rs.getRecord(keyboard);
                if (data != null)
                    
                    try {
                        skeyboard = new String(data, "UTF-8");
                    } catch (UnsupportedEncodingException ex) {
                        ex.printStackTrace();
                    }
                data = rs.getRecord(dictionary);
                if ( data != null)
                    try {
                        sdictionary = new String(data, "UTF-8");
                    } catch (UnsupportedEncodingException ex) {
                        ex.printStackTrace();
                    }
            }
            rs.closeRecordStore();
        } catch( RecordStoreException e ){e.printStackTrace();}
    }
    
    public void saveLanguage(String lang){
        slanguage = lang;
        save(language, lang);
    }
    
    public void saveDictionary(String dic){
        sdictionary = dic;
        save(dictionary, dic);
    }
    
    public void saveKeyboard(String k){
        skeyboard = k;
        save(keyboard, k);
    }
    
    private void save(int id, String dato){
        RecordStore rs = null;
        byte[] data=null;
        try {
            data = dato.getBytes("UTF-8");
        } catch (UnsupportedEncodingException ex) {
            ex.printStackTrace();
        }
        try {
            rs = RecordStore.openRecordStore( "preferencias", true );
            rs.setRecord(id, data, 0, data.length); 
            rs.closeRecordStore();
        } catch( RecordStoreException e ){}
    }
    
    public String getLanguage(){
        return slanguage;
    }
    
    public String getKeyboard(){
        return skeyboard;
    }
    
    public String getDictionary(){
        return sdictionary;
    }
    
}
