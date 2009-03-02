package com.winksi.j2me.cc.midlet;

import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;
import javax.wireless.messaging.*;
import javax.microedition.io.*;

import com.winksi.j2me.cc.control.GUIController;
import java.util.Date;
import java.util.Vector;
import javax.microedition.io.ConnectionNotFoundException;
import javax.microedition.io.PushRegistry;

public class CCMIDlet extends MIDlet implements MessageListener {

	private GUIController controller;
	private Display display;
	private String language = "";

	//    Vector listeners = new Vector();

	public CCMIDlet() {
		language = System.getProperty("microedition.locale"); // 得到语言
		display = Display.getDisplay(this);
	}

	protected void destroyApp(boolean arg0) throws MIDletStateChangeException {
		//        if (listeners != null) {
		//            while (listeners.isEmpty() == false) {
		//                MessageConnection mc = (MessageConnection) listeners.firstElement();
		//                if (mc != null) {
		//                    try {
		//                        mc.setMessageListener(null);
		//                        mc.close();
		//                    } catch (Exception exp) {
		//                    }
		//                }
		//                listeners.removeElementAt(0);
		//            }
		//        }
	}

	protected void pauseApp() {

	}

	protected void startApp() throws MIDletStateChangeException {
		//        long time = System.currentTimeMillis();
		//        System.out.println("start : " + time);
		//        setLight(1000000);
		controller = new GUIController(this);
		try {
			// 当前MIDlet套件中注册的连接列表
			String[] conns = PushRegistry.listConnections(true);//传入true表示仅取得接入的连接
			if (conns.length > 0) {//短信push启动
				//                System.out.println("push : ");
				controller.setIsPushWakeUp(true);//无视图处理
				for (int i = conns.length - 1; i >= 0; i--) {// 利用这个连接字符串打开连接处理(无处理,只是用来判断是否为短信push启动)
					//                    System.out.println("push : " + conns[i]);                   
					MessageConnection mc = (MessageConnection) Connector.open(conns[i]);
					//                        mc.setMessageListener(this);
					//                        mc.setMessageListener(null);
					// 目前的需求比较简单,并没有push什么需要处理的内容,只是知道push来了然后
					mc.close();
					//                        listeners.addElement(mc);                  
				}
			} else {
				//                System.out.println("push true");
				controller.setIsPushWakeUp(false);
				//                controller.initEvent();
			}
		} catch (IllegalArgumentException iae) {
		} catch (SecurityException se) {
			exit();
		} catch (Exception exc) {
		}

		controller.init();
	}

	public void setCurrent(Displayable disp) {
		display.setCurrent(disp);
	}

	public void setCurrent(Alert alert, Displayable disp) {
		display.setCurrent(alert, disp);
	}

	public Displayable getCurrent() {
		return display.getCurrent();
	}

	public void setBacklight(int time) {
		display.flashBacklight(time);
	}

	public int getLanguage() {
		int i = 0;
		if (language.equals("en")) {
			i = 1;
		} else if (language.equals("zh-TW")) {
			i = 2;
		} else if (language.equals("zh-CN")) {
			i = 0;
		}
		return i;
	}

	/** 关闭midlet */
	public void exit() {
		try {
			destroyApp(true);
			notifyDestroyed();
			//            long time = System.currentTimeMillis();
			//            System.out.println("end : " + time);
		} catch (MIDletStateChangeException e) {
		}
	}

	/**　基于时钟的Push动态注册 */
	public long scheduleMIDlet(long deltaTime) throws ClassNotFoundException,
			ConnectionNotFoundException, SecurityException {

		String scheduledMIDlet = this.getClass().getName();
		// Get the current time by calling Date.getTime()
		Date now = new Date();
		long time = now.getTime();
		//        System.out.println("register : " + (time+deltatime));
		long t = PushRegistry.registerAlarm(scheduledMIDlet, time + deltaTime);
		return t;
		//        System.out.println("time=" + t);
	}

	public void notifyIncomingMessage(MessageConnection conn) {
		//        try {
		//            System.out.println("incoming");
		//            TextMessage aMess = (TextMessage) conn.receive();
		//            String text = aMess.getPayloadText();
		//            System.out.println("text : " + text);
		//        } catch (java.lang.SecurityException exp) {
		//        } catch (Exception exp) {
		//        }
	}
}
