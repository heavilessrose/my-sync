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

import ub.es.ubictionary.visual.DList;
import ub.es.ubictionary.visual.SearchCanvas;
import java.io.IOException;
import java.io.UnsupportedEncodingException;

/**
 *
 * @author aram
 */
public class ProcesoBuscador extends Thread {
    
    /** Creates a new instance of ProcesoBuscador */
    DList lista;
    Buscador buscador;
    String texto, txt;
    boolean quit, stop;
    SearchCanvas searchCanvas;
    public ProcesoBuscador(SearchCanvas sc, String indexfile) {
        searchCanvas = sc;
        stop = false;
        quit = false;
        texto = "";
        lista = sc.getListado();
        buscador = new Buscador();
    }
    
    public void setIndexFile(String file){
        try {
            buscador.setIndexFile(file);
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }
    
    public String buscarPalabra(String palabra){
        if (palabra != null){
            try {
                return buscador.buscarPalabraExacta(palabra);
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
        return null;
    }
    
    public  synchronized void setTextoABuscar(String t){
        texto = t;
        stop = false;
    }
    
    public synchronized void parar(boolean b){
        stop = b;
    }
    
    public  void apagarBuscador(){
        quit = true;
    }
    
    public void run(){
        while (!quit){
            txt = new String(texto);
            if (texto.compareTo("")!=0 && stop==false){
                lista.setItems(buscador.empezarBusqueda(texto));
                searchCanvas.repaint();
            }
            try {
                //Si despues de hacer la b´suqueda el texto es el mismo entonces esperamos sino no
                if (txt.compareTo(texto)==0){
                    synchronized(searchCanvas){
                        searchCanvas.wait();
                    }
                }
            } catch (InterruptedException ex) {
                ex.printStackTrace();
            }
        }
    }
    
}
