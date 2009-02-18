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

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.Vector;
import java.util.Hashtable;

/**
 *
 * @author aram
 */
public class Config {
    String lang_file; //Diccionario seleccionado: guarda informacion de archivo de indices del diccionario
    String diccionario; //El nombre del diccioniario seleccionado
    String[] keys = {"-_@", "abc·‡‰ABC", "defÈË‰DEF", "ghiÌÏÔGHI", "jklJKL", "mnoÒÛÚˆMNO", "pqrsPQRS", "tuv˙˘¸TUV", "wxyzWXYZ"}; //El teclado
    String selectedKeyboard;
    String[] palabras = {"Search", "Dictionary", "Exit", "Help", "Keyboard", "Back", "Accept", "No Results", "Language", "Use joystick or number buttons (2 Up, 8 down and 5 for select) for move in menu. In search screen you can use the joystick or the button 0 down and # for up for move in results list.", "About"};
    String language; //el idioma seleccionado
    DiskManager disk;
    String encoding;
    
    /** Creates a new instance of Config */
    public Config() {
        disk = new DiskManager();
        String dicts[] = null;
        selectedKeyboard = "";
        language = "";
        diccionario = disk.getDictionary();
        encoding = "UTF8";
        
       
            dicts = getDictionaries();
       
        
        //Comprueba que diccionario preferido del usuario sigue estando en el listado de diccionarios
        //Si sigue en el listado selecciono ese diccionario sino eligo el primero que vea
        boolean b = false;
        for(int i = 0; i < dicts.length; i++){
            if(dicts[i].compareTo(diccionario)==0){
                b = true;
                break;
            }
        }
        if (b ==false)
            setDictionary(dicts[0]);
        else
            setDictionary(diccionario);
        
        
        setLanguage(disk.getLanguage());
        setKeyboard(disk.getKeyboard());
        inicializar();
        
    }
    
    public void inicializar(){
        
    }
    
    public String[] getDictionaries(){
        try {
            return getKeys("config");
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        return null;
        /*Vector v = new Vector(5, 2);
        StringBuffer lectura = new StringBuffer(13);
        int ch = 0;
        InputStream is = this.getClass().getResourceAsStream("/config");
        InputStreamReader ir;
        try {
            ir = new InputStreamReader(is, encoding);
        } catch (UnsupportedEncodingException ex) {
            ir = new InputStreamReader(is, "UTF-8");
        }
        while ((ch = ir.read()) > -1){
            if (ch=='\n'){
                v.addElement(lectura.toString());
                lectura.setLength(0);
            }else{
                lectura.append((char)ch);
            }
        }
        if (lectura.length() != 0)
            v.addElement(lectura.toString());
        ir.close();
        String dicts[] = new String[v.size()];
        for(int i=0; i<v.size(); i++){
            dicts[i] = (String) v.elementAt(i);
        }
        return dicts;*/
    }
    
    public void setDictionary(String s){
        try {
            String [] v = getValuesOfKey(s, "config");
            //System.out.println(v[0]);
            if (v !=null)
                lang_file = v[0];
            diccionario = s;
            disk.saveDictionary(s);
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        
        
        /*diccionario = s;
        lang_file = s.toLowerCase();
        lang_file = removeFromString(lang_file, "abcdefghijklmnopqrstuvwxyz");
        
        disk.saveDictionary(s);
        //System.out.println(lang_file);
        //\/:*?"<>|*/
    }
    
    public String getDictionary(){
        return diccionario;
    }
    
    private String removeFromString(String s, String c){
        StringBuffer sb = new StringBuffer();
        for (int i = 0; i < s.length(); i ++) {
          if (c.indexOf(s.charAt(i)) != -1) 
              sb.append(s.charAt(i));
        }
        return sb.toString();
    }
    
    public String getIndexFile(){
        return lang_file;
    }
    
    private String[] getKeys(String file) throws IOException{
        Vector v = new Vector(5, 2);
        StringBuffer lectura = new StringBuffer(13);
        int ch = 0;
        boolean b = true;
        InputStream is = this.getClass().getResourceAsStream("/" + file);
        InputStreamReader ir;
        try {
            ir = new InputStreamReader(is, encoding);
        } catch (UnsupportedEncodingException ex) {
            ir = new InputStreamReader(is, "UTF-8");
        }
        while ((ch = ir.read()) > -1){
            if (ch=='\n'){
                b = true;
                v.addElement(lectura.toString());
                lectura.setLength(0);
            }else{
                if (ch=='#'){
                    b = false;
                }
                if (b == true){
                    lectura.append((char)ch);
                }
            }
        }
        if (lectura.length() != 0)
            v.addElement(lectura.toString());
        ir.close();
        String dicts[] = new String[v.size()];
        for(int i=0; i<v.size(); i++){
            dicts[i] = (String) v.elementAt(i);
        }
        
        return dicts;
    }
    
    public void setKeyboard(String string){
        try {
            String k[] = getValuesOfKey(string, "keyboard");
            //System.out.println(k[1].length());
            if (k.length > 0)
                keys = k;
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        selectedKeyboard = string;
        disk.saveKeyboard(string);
    }
    
    public String getSelectedKeyboard(){
        return selectedKeyboard;
    }
    
    private String[] getValuesOfKey(String key, String file) throws IOException{
        //String teclado[] = new String[9];
        Vector v = new Vector (10, 3);
        StringBuffer lectura = new StringBuffer(13);
        StringBuffer lectura_valores = new StringBuffer(5);
        int ch = 0;
        boolean b = true;
        InputStream is = this.getClass().getResourceAsStream("/" + file);
        InputStreamReader ir;
        try {
            ir = new InputStreamReader(is, encoding);
        } catch (UnsupportedEncodingException ex) {
            ir = new InputStreamReader(is, "UTF-8");
        }
        while ((ch = ir.read()) > -1){
            if (ch=='\n'){
                if (lectura.toString().compareTo(key)==0)
                    break;
                b = true;
                lectura.setLength(0);
            }else{
                if (ch=='#'){
                    b = false;
                }else{
                    if (b==false){ // Empieza la informacion para el array de teclados
                        if (lectura.toString().compareTo(key)==0){

                            if (ch=='|'){
                                v.addElement(lectura_valores.toString());
                                lectura_valores.setLength(0);
                            }else{
                                lectura_valores.append((char) ch);
                                /*if (teclado[i]==null)
                                    teclado[i] ="" + (char) ch;
                                else
                                    teclado[i] = teclado[i] + ((char) ch);
                                 */
                            }
                        }
                    }
                }
                if (b==true)
                    lectura.append((char)ch);
            }
        }
        
        if (lectura_valores.length() > 0)
                v.addElement(lectura_valores.toString());
        ir.close();
        String salida[] = new String[v.size()];
        for(int i=0; i<v.size(); i++){
            salida[i] = (String) v.elementAt(i);
            //System.out.println(salida[i]);
        }
        
        return salida;
    }
    
    /**
     *retorna el arrray con el mapeo del teclado
     */
    public String[] getKeyboard(){
        return keys;
    }
    
    /**
     *Retorna el array con los teclados disponibles
     */
    public String[] getKyeboards(){
        try {
            return getKeys("keyboard");
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        return null;
    }
    
    public void setLanguage(String lang){
        language = lang;
        disk.saveLanguage(lang);
        String w[];
        try {
            w = getValuesOfKey(lang, "interface");
            if (w.length > 0)
                palabras = w;
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        
        //System.out.println(palabras.length);
    }
    
    public String[] getLanguages(){
        try {
            return getKeys("interface");
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        return null;
    }
    
    public String getLanguage(){
        return language;
    }
    
    public String getPalabra(String s){
        if (s.compareTo("buscar")==0)
            return palabras[0];
        if (s.compareTo("diccionario")==0)
            return palabras[1];
        if(s.compareTo("salir")==0)
            return palabras[2];
        if(s.compareTo("ayuda")==0)
            return palabras[3];
        if(s.compareTo("teclado")==0)
            return palabras[4];
        if(s.compareTo("volver")==0)
            return palabras[5];
        if (s.compareTo("aceptar")==0)
            return palabras[6];
        if (s.compareTo("sin_resultado")==0)
            return palabras[7];
        if (s.compareTo("idioma")==0)
            return palabras[8];
        if (s.compareTo("texto_ayuda")==0)
            return palabras[9];
        if (s.compareTo("about")==0)
            return palabras[10];
        return null;
    }
    
}
