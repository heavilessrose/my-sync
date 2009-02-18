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

import ub.es.ubictionary.logic.Buscador;
import ub.es.ubictionary.logic.Config;
import ub.es.ubictionary.logic.Keyboard;
import ub.es.ubictionary.logic.ProcesoBuscador;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import javax.microedition.lcdui.*;
import ub.es.ubictionary.logic.UBictionary;

/**
 *
 * @author aram
 */
public class SearchCanvas extends Canvas implements CommandListener {
    
    /** Creates a new instance of Menu */
    private DTextField txtBuscar;
    private DList lstResults;
    private Command cmdBuscar, cmdVolver;
    private UBictionary midlet; //El midlet en si
    private ResultCanvas rcanvas;
    private String index_file;
    
    //private Buscador buscador;
    private ProcesoBuscador pBuscador;
    
    public SearchCanvas(UBictionary midlet) {
       this.midlet = midlet; 
       index_file = "";
       lstResults = new DList(2, 20, getWidth()-5, getHeight() - 22);
       lstResults.setSeleccionable(true);
//-- TextTbox
       txtBuscar = new DTextField("", midlet.cfg.getKeyboard(),2, 2, getWidth()-5, 17);
       txtBuscar.setFocus(true);
       
       pBuscador = new ProcesoBuscador(this, midlet.cfg.getIndexFile());
        pBuscador.start();
      inicializar();
      setCommandListener(this);
    }
    
    
    
    public void  inicializar(){
        rcanvas = new ResultCanvas(midlet);
        removeCommand(cmdVolver);
        removeCommand(cmdBuscar);
      cmdVolver = new Command(midlet.cfg.getPalabra("volver"), Command.BACK, 2);
      cmdBuscar = new Command(midlet.cfg.getPalabra("buscar"), Command.OK, 1);
      addCommand(cmdVolver);
      addCommand(cmdBuscar);
      txtBuscar.setText(null);
      txtBuscar.setKeyboard(midlet.cfg.getKeyboard());
      lstResults.setItems(null);
      if (midlet.cfg.getIndexFile().compareTo(index_file)!=0){
          index_file = midlet.cfg.getIndexFile();  
          pBuscador.setIndexFile(index_file);
      }
    }
    
    public void paint(Graphics g) {
        g.setColor(255, 255, 255);
        g.fillRect(0, 0, getWidth(), getHeight());
        txtBuscar.render(g);
        lstResults.render(g);
    }

    protected void keyPressed(int keyCode) {
          if ( Keyboard.isTextButtonPushed(keyCode)){
            txtBuscar.gestionarEvento(keyCode);
          }else{
              if (getGameAction(keyCode)==LEFT || (keyCode==-2 && getGameAction(keyCode)==0)) //-2 and -5 are Motorola phones
                  txtBuscar.gestionarEvento(-3);
              if (getGameAction(keyCode)==RIGHT || (keyCode==-5 && getGameAction(keyCode)==0))
                  txtBuscar.gestionarEvento(-4);
          }
          
          //Hago las búsqueda solo si ha cambiado el contenido en el textbox
          if ( (keyCode >=49 && keyCode <=57) || keyCode ==-8 || keyCode == 42 || Keyboard.isASCII(keyCode)){
              pBuscador.setTextoABuscar(txtBuscar.getText());
              synchronized(this){
                this.notify();
              }
          }
      
          lstResults.gestionarEvento(Keyboard.getValidKeyCode(getGameAction(keyCode), keyCode));
          
      
      
      if ((keyCode!=KEY_NUM5 && getGameAction(keyCode) ==FIRE && keyCode!=32) || keyCode==10){
         //pBuscador.buscarPalabra(lstResults.getSelectedText());
         this.buscar();
      }
      repaint();
    }

    public void commandAction(Command command, Displayable displayable) {
        if (command == cmdBuscar){ //Realizar la búsqueda
           this.buscar();
           repaint(); 
        }
        if (command == cmdVolver){  //Mostar el Menu principal
            Display.getDisplay(midlet).setCurrent(midlet.pMenu);
        }
    }
    
    private void buscar(){
        pBuscador.parar(true);
        rcanvas.setResultado(pBuscador.buscarPalabra(lstResults.getSelectedText()));
        rcanvas.setTextoBuscado(lstResults.getSelectedText());
        Display.getDisplay(midlet).setCurrent(rcanvas);
    }
    public DList getListado(){
        return lstResults;
    }

    
}
