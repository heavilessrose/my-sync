package com.winksi.j2me.cc.tools;

import javax.microedition.lcdui.Image;


public class ConstImage {
    public static Image waitbg;
    public static Image main;
    public static Image alertbg;
    public static Image lprogress1;
    public static Image lprogress2;
    public static Image[] item=new Image[6];
    public static Image tri;
    public static Image selector;
    public static Image selector1;
    public static Image mult_on;
    public static Image mult_off;
    public static Image single_on;
    public static Image single_off;
    
    
    public ConstImage() {     
        waitbg=ImageTools.createImage("/res/waitbg.png");
        main=ImageTools.createImage("/res/main.png");
        alertbg=ImageTools.createImage("/res/alertbg.png");
        lprogress1=ImageTools.createImage("/res/lprogress1.png");
        lprogress2=ImageTools.createImage("/res/lprogress2.png");
        for(int i=0;i<item.length;i++){
            item[i]=ImageTools.createImage("/res/"+(i+1)+".png");
        }
        tri = ImageTools.createImage("/res/tri.png");
        selector=ImageTools.createImage("/res/selector.png");
        selector1=ImageTools.createImage("/res/selector1.png");
        mult_on=ImageTools.createImage("/res/mult_on.png");
        mult_off=ImageTools.createImage("/res/mult_off.png");
        single_on=ImageTools.createImage("/res/single_on.png");
        single_off=ImageTools.createImage("/res/single_off.png");
    }
}

