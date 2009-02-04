package com.srijeeb.jme;

import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.media.*;
import javax.microedition.media.control.*;
import java.io.*;

import javax.microedition.lcdui.*;
import javax.microedition.media.MediaException;
import javax.microedition.media.control.VideoControl;

public class VideoCanvas
    extends Canvas {

  private VideoControl vc = null;
  private Player plr = null;

  public VideoCanvas() {

  }

  public void initControls(VideoControl videoControl, Player player) {

    int width = getWidth();
    int height = getHeight();
    this.vc = videoControl;
    this.plr = player;

    videoControl.initDisplayMode(VideoControl.USE_DIRECT_VIDEO, this);
    //videoControl.initDisplayMode(VideoControl.USE_GUI_PRIMITIVE, this);
    try {
      videoControl.setDisplayLocation(2, 2);
      videoControl.setDisplaySize(width - 4, height - 4);
    }
    catch (MediaException me) {
      try { videoControl.setDisplayFullScreen(true); }
      catch (MediaException me2) {}
    }
    videoControl.setVisible(true);
  }

  public void paint(Graphics g) {
    int width = getWidth();
    int height = getHeight();

    // Draw a green border around the VideoControl.
    g.setColor(0x00ff00);
    g.drawRect(0, 0, width - 1, height - 1);
    g.drawRect(1, 1, width - 3, height - 3);

  }


}
