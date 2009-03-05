package com.winksi.j2me.cc.view;

import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.control.GUIController;
import com.winksi.j2me.cc.model.main.PimListener;
import com.winksi.j2me.cc.tools.ConstImage;
import com.winksi.j2me.cc.tools.ImageTools;
import com.winksi.j2me.cc.tools.StringTools;
import java.util.Vector;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

/**
 * 正在进行系统初始化 的闪屏
 * @author Jerry
 */
public class SplashCanvas extends Canvas implements Runnable, PimListener {

	private GUIController controller;
	private Image progress1Img;
	private Image progress2Img;
	private Vector infoVector;
	private final int showWidthMax = 200;
	private final int showHeighMax = 100;
	private final int info_x = 20;
	private final int info_y = 172;
	private static final int PROGRESS_Y = 60;//图片y坐标，x坐标居中（屏幕宽减去图片宽除以2）
	private int keyCode = -100;
	private Thread splashThread = null;
	private boolean isrunning = false;
	private int count = 0;
	private long oldtime = 0;
	private long currenttime = 0;
	private long delay = 0;
	private String msg = "";

	public SplashCanvas(GUIController controller) {
		setFullScreenMode(true);
		this.controller = controller;

		progress1Img = ImageTools.createImage("/res/progress1.png");
		progress2Img = ImageTools.createImage("/res/progress2.png");

		start();
	}

	public void start() {
		if (splashThread == null) {
			isrunning = true;
			splashThread = new Thread(this);
			splashThread.start();
		}
	}

	public void stop() {
		isrunning = false;
		splashThread = null;
	}

	public void drawButton(Graphics g) {
		g.setColor(CPProperty.COLOR_CHAR_WHITE);
		g
				.drawString(CPProperty.COMMAND_EXIT[controller.getLanguage()],
						CPProperty.SCREENWIDTH
								- CPProperty.defaultFont
										.stringWidth(CPProperty.COMMAND_BACK[controller
												.getLanguage()]) - 1, CPProperty.SCREENHEIGHT
								- CPProperty.dfheight - 2, 20);
	}

	protected void paint(Graphics g) {
		g.setColor(0);
		g.fillRect(0, 0, getWidth(), getHeight());
		g.drawImage(ConstImage.waitbg, 0, 0, 20);

		g.setClip((CPProperty.SCREENWIDTH - progress2Img.getWidth()) / 2,
				(PROGRESS_Y + progress2Img.getHeight()) - count * (progress2Img.getHeight() - 14)
						/ 50, progress2Img.getWidth(), progress2Img.getHeight() - 14);
		g.drawImage(progress2Img, (CPProperty.SCREENWIDTH - progress2Img.getWidth()) / 2,
				PROGRESS_Y, 20);
		g.setClip(0, 0, CPProperty.SCREENWIDTH, CPProperty.SCREENHEIGHT);
		g.drawImage(progress1Img, (CPProperty.SCREENWIDTH - progress1Img.getWidth()) / 2,
				PROGRESS_Y, 20);

		g.setColor(CPProperty.COLOR_CHAR_WHITE);

		g.setClip(info_x, info_y, showWidthMax, showHeighMax);

		g
				.drawString(CPProperty.INIT_LOADING_PIM[controller.getLanguage()] + msg,
						(CPProperty.SCREENWIDTH - CPProperty.defaultFont
								.stringWidth(CPProperty.INIT_LOADING_PIM[controller.getLanguage()]
										+ msg)) / 2, info_y
								+ (CPProperty.dfheight + CPProperty.ROW_SPACE), 20);

		g.setClip(0, 0, CPProperty.SCREENWIDTH, CPProperty.SCREENHEIGHT);
		//        if (!infoVector.isEmpty()) {
		//            g.setClip(info_x, info_y, showwidthmax, showheightmax);
		//            for (int i = 0; i < infoVector.size(); i++) {
		//                g.drawString((infoVector.elementAt(i).toString()).trim(), (CPProperty.SCREENWIDTH - CPProperty.defaultFont.stringWidth((infoVector.elementAt(i).toString()).trim())) / 2, info_y + (CPProperty.dfheight + CPProperty.ROW_SPACE) * i, 20);
		//            }
		//            g.setClip(0, 0, CPProperty.SCREENWIDTH, CPProperty.SCREENHEIGHT);
		//        }

		drawButton(g);
	}

	public void run() {
		while (isrunning) {
			count++;
			if (count > 50) {
				controller.setBacklight(50);
				count = 0;
			}

			try {
				repaint();
				Thread.sleep(100);
			} catch (InterruptedException ex) {
				ex.printStackTrace();
			}
		}
	}

	protected void keyPressed(int keycode) {
		keyCode = keycode;
		if (keyCode == CPProperty.KEY_NUM0) {// 0

		} else if (keyCode == CPProperty.KEY_NUM1) {// 1

		} else if (keyCode == CPProperty.KEY_NUM2) {// 2

		} else if (keyCode == CPProperty.KEY_NUM3) {// 3

		} else if (keyCode == CPProperty.KEY_NUM4) {// 4

		} else if (keyCode == CPProperty.KEY_NUM5) {// 5

		} else if (keyCode == CPProperty.KEY_NUM6) {// 6

		} else if (keyCode == CPProperty.KEY_NUM7) {// 7

		} else if (keyCode == CPProperty.KEY_NUM8) {// 8

		} else if (keyCode == CPProperty.KEY_NUM9) {// 9

		} else if (keyCode == CPProperty.KEY_STAR) {// *

		} else if (keyCode == CPProperty.KEY_POUND) {// #

		} else if (keyCode == CPProperty.KEY_SOFT_LEFT) {// left soft key

		} else if (keyCode == CPProperty.KEY_SOFT_RIGHT) {// right soft key
			controller.handleEvent(GUIController.EventID.EVENT_EXIT, null);
		} else if (keyCode == CPProperty.KEY_BACK) {// back key

		} else if (keyCode == CPProperty.KEY_CLEAR) {// clear key

		} else if (keyCode == CPProperty.KEY_SOUND_UP) {// sound up key

		} else if (keyCode == CPProperty.KEY_SOUND_DWON) {// sound down
			// key

		} else if (keyCode == CPProperty.KEY_UP) {// up

		} else if (keyCode == CPProperty.KEY_DOWN) {// down

		} else if (keyCode == CPProperty.KEY_LEFT) {// left

		} else if (keyCode == CPProperty.KEY_RIGHT) {// right

		} else if (keyCode == CPProperty.KEY_OK) {// ok

		}
	}

	protected void showNotify() {

	}

	protected void hideNotify() {
		//        stop();
	}

	public void setInfo(String s) {
		infoVector = new Vector();
		infoVector = StringTools.getSubsection(s, CPProperty.defaultFont, showWidthMax, ".,;?!");
	}

	/** 显示显示的联系人名字 */
	public void setMessage(String msg) {
		this.msg = msg;
	}
}
