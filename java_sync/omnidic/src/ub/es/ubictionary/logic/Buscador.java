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


/**
 * 
 * @author aram
 */
public class Buscador {
    private final int MAX_REGISTROS = 30;
    String index[][];
    String encoding;
    public Buscador() {
        
        ;//setIndexFile(index_file);
    }
    
    public void setIndexFile(String index_file) throws IOException{
        Vector v = new Vector(50, 10);
        StringBuffer lectura = new StringBuffer(7);
        int ch = 0;
        encoding = "UTF8";
        //System.out.println(index_file);
        // El nombre de archivo debe obttenerse dependiendo del diccionario elegido
        InputStream is = this.getClass().getResourceAsStream("/" + index_file);
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
                if (ch == '#'){
                    v.addElement(lectura.toString());
                    lectura.setLength(0);
                }else{
                    lectura.append((char)ch);
                }
            }
         }
         ir.close();
        if (lectura.length() > 0){
             v.addElement(lectura.toString());
        }
         
        index = new String[v.size()/3][3];
        for(int i=0; i<v.size(); i++){
            
            index[i/3][(i+3)%3] = (String) v.elementAt(i);
            //System.out.println((i/3) + " - " + ((i+3)%3) + " = " + index[i/3][(i+3)%3] );
        }
    }
    
    public String[] empezarBusqueda(String s){
        String ini="";
        String [] r1 = null;
        String [] r2 = null;
        boolean segundo = false;
        for (int i = 0; i < index.length; i++){
            //System.out.println(index[i][0] + " - " + index[i][1] + " - " + index[i][2]);
            ini = index[i][0];
            if (ini.length()>s.length()){
                ini = ini.substring(0, s.length());
            }
            if ( (s.compareTo(ini)>=0 ) && ( s.compareTo(index[i][1])<=0) ){
                
                try {
                    if (segundo == false){
                        r1 = buscar(s, index[i][2], MAX_REGISTROS);
                        segundo = true;
                        if (r1.length == MAX_REGISTROS)
                            return r1;
                    }else{
                        if (r1 !=null){
                            r2 = buscar(s, index[i][2], MAX_REGISTROS - r1.length);
                           return join(r1, r2); 
                        }
                    }
                    //return buscar(s, index[i][2]);
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
        }
        return r1;
    }
    
    public String [] buscar(String palabra, String file, int registros_a_devolver) throws  IOException{
        
       InputStream is = this.getClass().getResourceAsStream("/" + file);
        StringBuffer lectura = new StringBuffer();
        byte b[] = new byte[1];
        boolean finpalabra = false;
        boolean palabrareg = false;
        short encontradas = 0; //indicara el numero de palabras encontradas, lo limitamos a 30
        Vector words = new Vector(20, 10);
        int ch=0;
        //System.out.println(file);
        //System.out.println(file.length());
        InputStreamReader ir;
        try {
            ir = new InputStreamReader(is, encoding);
        } catch (UnsupportedEncodingException ex) {
            ir = new InputStreamReader(is, "UTF-8");
        }
        while ((ch = ir.read()) > -1) {
            if (ch=='\n'){ //Fin de linea
                lectura.setLength(0);
                finpalabra = false;
                palabrareg = false;
            }else{
                if (ch != '#' && finpalabra == false){
                    lectura.append((char)ch);
                }else{
                    if (palabrareg == false){
                        finpalabra = true;
                        if (lectura.length() >= palabra.length())
                            if (palabra.compareTo(lectura.toString().substring(0, palabra.length())) == 0){//SI son iguales
                            //if (palabra.compareTo(lectura.toString().toLowerCase().substring(0, palabra.length())) == 0){//SI son iguales
                                //System.out.println(lectura.toString());
                                words.addElement(lectura.toString());
                                encontradas ++;
                                if (encontradas==registros_a_devolver)
                                    break;
                            }
                        palabrareg = true;
                    }
                }
            }
        }         
        is.close();
        
        //if (lectura.length() > 0)
        //    words.addElement(lectura.toString());
        
        //System.out.println(words.size() + " ++++");
        String wrds[] = new String[words.size()];
        for(int i=0; i<words.size(); i++)
            wrds[i] = (String) words.elementAt(i);
        return wrds;
    }
    
    public String buscarPalabraExacta(String palabra) throws IOException{
        String file=null;
        for (int i = 0; i < index.length; i++){
            if ( (palabra.compareTo(index[i][0])>=0 ) && (palabra.compareTo(index[i][1]) <= 0 ) ){
                   file= index[i][2];
                   break;
            }
        }
        if (file==null)
            return null;
        
        Vector v = new Vector(5, 2);
        StringBuffer lectura = new StringBuffer(13);
        int ch = 0;
        boolean b = false;
        InputStream is = this.getClass().getResourceAsStream("/" + file);
        InputStreamReader ir;
        try {
            ir = new InputStreamReader(is, encoding);
        } catch (UnsupportedEncodingException ex) {
            ir = new InputStreamReader(is, "UTF-8");
        }
        while ((ch = ir.read()) > -1){
            if (ch=='\n'){
                if(b == true)
                    break;
                lectura.setLength(0);
                b = false;
            }else{
                if (ch=='#'){
                    if(lectura.toString().compareTo(palabra)==0){
                        b = true;
                        lectura.setLength(0);
                    }
                }else{
                     lectura.append((char)ch);
                }
            }
        }
        is.close();
        return lectura.toString();
    }

    private String[] join(String[] r1, String[] r2) {
        if (r1 !=null && r2 != null){
            String [] r3 = new String[r1.length + r2.length];
            int j = r1.length;
            for (int i=0; i < r1.length; i++){
                r3[i] = r1[i];
            }
            for (int i = 0; i < r2.length; i++){
                r3[j+i] = r2[i];
            }
            return r3;
        }
        return null;
    }
    
   
}
