/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.winksi.j2me.cc.view;

import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.control.GUIController;
import com.winksi.j2me.cc.tools.ConstImage;
//import java.util.Vector;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Graphics;

/**
 * 退出程序要求确认的界面
 * @author jerry
 */
public class ExitCanvas extends Canvas {

	private GUIController controller;
	private int keyCode = -100;

	public ExitCanvas(GUIController controller) {
		setFullScreenMode(true);
		this.controller = controller;
	}

	public void drawButton(Graphics g) {
		g.setColor(CPProperty.COLOR_CHAR_WHITE);
		g.drawString(CPProperty.COMMAND_SURE[controller.getLanguage()], 1, CPProperty.SCREENHEIGHT
				- CPProperty.dfheight - 2, 20);
		g
				.drawString(CPProperty.COMMAND_BACK[controller.getLanguage()],
						CPProperty.SCREENWIDTH
								- CPProperty.defaultFont
										.stringWidth(CPProperty.COMMAND_BACK[controller
												.getLanguage()]) - 1, CPProperty.SCREENHEIGHT
								- CPProperty.dfheight - 2, 20);
	}

	protected void paint(Graphics g) {
		g.setColor(0);
		g.fillRect(0, 0, getWidth(), getHeight());
		g.drawImage(ConstImage.main, 0, 0, 20);

		g.setColor(CPProperty.COLOR_CHAR_WHITE);
		g.drawString(CPProperty.EXIT_TITLE[controller.getLanguage()],
				(CPProperty.SCREENWIDTH - CPProperty.defaultFont
						.stringWidth(CPProperty.EXIT_TITLE[controller.getLanguage()])) >> 1,
				CPProperty.TITLE_OFF_Y, 20);

		g.drawImage(ConstImage.alertbg, 0, (CPProperty.SCREENHEIGHT - ConstImage.alertbg
				.getHeight()) / 2, 20);
		g.setColor(CPProperty.COLOR_CHAR_BLACK);
		g.drawString(CPProperty.EXIT_INFO[controller.getLanguage()],
				(CPProperty.SCREENWIDTH - CPProperty.defaultFont
						.stringWidth(CPProperty.EXIT_INFO[controller.getLanguage()])) >> 1,
				CPProperty.SCREENHEIGHT / 2, 20);
		drawButton(g);
	}

	protected void keyPressed(int keycode) {
		keyCode = keycode;

		if (keyCode == CPProperty.KEY_SOFT_LEFT) { // 确认关闭,并注册定时启动
			controller.handleEvent(GUIController.EventID.EVENT_EXIT, null);
		} else if (keyCode == CPProperty.KEY_SOFT_RIGHT) {// 不退出,返回前一事件继续执行
			controller.handleEvent(GUIController.EventID.EVENT_EXIT_BACK, null);
		}
		//        repaint();
	}
}
