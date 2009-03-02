package com.winksi.j2me.cc.view;

import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.control.GUIController;
import com.winksi.j2me.cc.model.main.MenuEvent;
import com.winksi.j2me.cc.model.main.MenuItemChangeManager;
import com.winksi.j2me.cc.model.net.HttpListener;
//import com.winksi.j2me.cc.model.sms.ThreadMMSHandler;
import com.winksi.j2me.cc.tools.ConstImage;
//import com.winksi.j2me.cc.tools.HttpTools;
import com.winksi.j2me.cc.tools.ImageTools;
import com.winksi.j2me.cc.model.sms.SMSTools;
//import java.io.UnsupportedEncodingException;
//import java.util.Hashtable;
//import java.util.Timer;
//import javax.microedition.io.PushRegistry;
import com.winksi.j2me.cc.tools.StringTools;
//import java.io.UnsupportedEncodingException;
import java.util.Vector;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

/**
 *
 * @author jerry
 */
public class WaitingCanvas extends Canvas implements Runnable, HttpListener {

	private GUIController controller;
	private SMSTools smsTools = null;
	//    private Timer timer;
	//    private HttpTools httpTools;
	private Menu mainMenu;
	private MenuItem continueMenuItem, exitMenuItem;
	private MenuItemChangeManager menuItemChangeManager;
	private Image progress1Img;
	private Image progress2Img;

	private Vector infoVector;
	private final int showwidthmax = 200;
	private final int showheightmax = 100;
	private final int info_x = 20;
	private final int info_y = 172;

	private static final int PROGRESS_Y = 60;//图片y坐标，x坐标居中（屏幕宽减去图片宽除以2）
	private int keyCode = -100;
	/** 是否正在进行联网操作 */
	private boolean isgprs = false;
	private Thread self = null;
	private boolean isrunning = false;
	private int count = 0;
	//    private boolean initpim = false;
	//    private String info = "";
	//    private long oldtime = 0;
	//    private long currenttime = 0;
	//    private long delay = 0;
	//    private boolean isregister = false;
	//    private byte[] testresult={};
	private String result = "";

	//    private String title="";

	public WaitingCanvas(GUIController controller) {
		setFullScreenMode(true);
		this.controller = controller;
		//        httpTools=new HttpTools();

		progress1Img = ImageTools.createImage("/res/progress1.png");
		progress2Img = ImageTools.createImage("/res/progress2.png");
		mainMenu = new Menu(this.controller);
		initMenuItem();
		menuItemChangeManager = new MenuItemChangeManager(this.controller, mainMenu);
		start();
	}

	/** 启动等待界面绘图线程 */
	public void start() {
		if (self == null) {
			isrunning = true;
			self = new Thread(this);
			self.start();
		}
	}

	/** 停止等待界面绘图线程 */
	public void stop() {
		isrunning = false;
		self = null;
	}

	public void initMenuItem() {
		int[] itemevent1 = { MenuEvent.BACK, MenuEvent.REGISTER };
		continueMenuItem = new MenuItem(CPProperty.COMMAND_REGISTER[controller.getLanguage()], itemevent1, false);
		int[] itemevent2 = { MenuEvent.BACK, MenuEvent.EXIT };
		exitMenuItem = new MenuItem(CPProperty.COMMAND_EXIT[controller.getLanguage()], itemevent2, false);
		mainMenu.addMenuItem(continueMenuItem);
		mainMenu.addMenuItem(exitMenuItem);
	}

	public void drawButton(Graphics g) {
		if (mainMenu != null && mainMenu.isVisible()) {
			g.drawString(CPProperty.COMMAND_BACK[controller.getLanguage()], CPProperty.SCREENWIDTH
					- CPProperty.defaultFont.stringWidth(CPProperty.COMMAND_BACK[controller.getLanguage()]) - 1,
					CPProperty.SCREENHEIGHT - CPProperty.dfheight - 2, 20);
		} else {
			g.setColor(CPProperty.COLOR_CHAR_WHITE);
			g.drawString(CPProperty.COMMAND_OPTIONS[controller.getLanguage()], 1, CPProperty.SCREENHEIGHT
					- CPProperty.dfheight - 2, 20);
		}
	}

	protected void paint(Graphics g) {
		g.setColor(0);
		g.fillRect(0, 0, getWidth(), getHeight());
		g.drawImage(ConstImage.waitbg, 0, 0, 20);

		g.setClip((CPProperty.SCREENWIDTH - progress2Img.getWidth()) / 2, (PROGRESS_Y + progress2Img.getHeight())
				- count * (progress2Img.getHeight() - 14) / 50, progress2Img.getWidth(), progress2Img.getHeight() - 14);
		g.drawImage(progress2Img, (CPProperty.SCREENWIDTH - progress2Img.getWidth()) / 2, PROGRESS_Y, 20);
		g.setClip(0, 0, CPProperty.SCREENWIDTH, CPProperty.SCREENHEIGHT);
		g.drawImage(progress1Img, (CPProperty.SCREENWIDTH - progress1Img.getWidth()) / 2, PROGRESS_Y, 20);

		g.setColor(CPProperty.COLOR_CHAR_WHITE);
		if (!infoVector.isEmpty()) {
			g.setClip(info_x, info_y, showwidthmax, showheightmax);
			for (int i = 0; i < infoVector.size(); i++) {
				g.drawString((infoVector.elementAt(i).toString()).trim(),
						(CPProperty.SCREENWIDTH - CPProperty.defaultFont.stringWidth((infoVector.elementAt(i)
								.toString()).trim())) / 2, info_y + (CPProperty.dfheight + CPProperty.ROW_SPACE) * i,
						20);
			}
			g.setClip(0, 0, CPProperty.SCREENWIDTH, CPProperty.SCREENHEIGHT);
		}

		drawButton(g);

		if (mainMenu != null && mainMenu.isVisible()) {
			mainMenu.drawMenu(g);
		}

		//        if (isregister) {
		//            g.setColor(CPProperty.COLOR_CHAR_WHITE);
		//            for (int i = 0; i < CPProperty.ERROR_REGISTER[controller.getLanguage()].length; i++) {
		//                g.drawString(CPProperty.ERROR_REGISTER[controller.getLanguage()][i], 0, 140 + i * CPProperty.dfheight, 20);
		//            }
		//        }

	}

	public void run() {
		while (isrunning) {
			if (this.isgprs && this.mainMenu.isVisible()) {
				hideMainMenu();
			}
			count++;
			if (count > 50) {
				controller.setBacklight(50);
				count = 0;
			}
			//            System.out.println("time : "+(currenttime - oldtime)+" delay : "+delay);
			//            currenttime = System.currentTimeMillis();
			//            if (currenttime - oldtime > delay && delay > 0) {
			////                isregister = true;
			//                
			//                if (delay>0) {
			//                    delay = -100;
			//                }
			//            }
			//            System.out.println(count);
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

		if (controller.getIsPushWakeup()) {
			return;
		}
		if (mainMenu.isVisible()) {
			if (mainMenu.isFocus()) {
				mainMenu.process(keyCode);
			}
		} else {
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
				if (!mainMenu.isVisible() && !isgprs) {
					controller.handleEvent(GUIController.EventID.EVENT_WAITING_MENU, null);
				}
				//                if (isregister) {
				//                    isregister = false;
				//                    controller.handleEvent(GUIController.EventID.EVENT_REGISTER, null);
				//                }
			} else if (keyCode == CPProperty.KEY_SOFT_RIGHT) {// right soft key
				//                if (isregister) {
				//                    isregister = false;
				//                    if (delay == controller.getLongCyc()) {
				//                        controller.handleEvent(GUIController.EventID.EVENT_EXIT, null);
				//                    } else {
				//                        delay = controller.getLongCyc();
				//                    }
				//                    oldtime = System.currentTimeMillis();
				//                }
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
		repaint();
	}

	//    public void setShowInfo(String info) {
	//        this.info = info;
	//    }

	public void showMainMenu() {
		mainMenu.setFocus(true);
		mainMenu.toShow();
	}

	public void hideMainMenu() {
		mainMenu.toHide();
	}

	public void removeMenuItem() {
		mainMenu.removeAll();
	}

	/** 从服务器得到数据成功 */
	public void onFinish(byte[] result) {
		System.out.println("WaitingCanvas: start parse data from server");
		//        testresult=result;
		Object[] obj = { result };
		controller.handleEvent(GUIController.EventID.EVENT_PARSE, obj);
		System.out.println("WaitingCanvas: parse data from server successed");
	}

	public void onProgress(int percent) {
	}

	public void onError(int code) {
		if (code == 12) {
			controller.handleEvent(GUIController.EventID.EVENT_EXIT, null);
		} else {
			//        Object[] obj={CPProperty.ERROR_REGISTER[controller.getLanguage()]};
			if (controller.getIsPushWakeup()) {
				controller.handleEvent(GUIController.EventID.EVENT_EXIT, null);
			} else {
				if (controller.getRuntimes() > 3) {
					controller.handleEvent(GUIController.EventID.EVENT_EXIT, null);
				} else {
					System.out.println("WaitingCanvas:onError(): regester error");
					controller.handleEvent(GUIController.EventID.EVENT_ERROR, null);
				}
			}
		}
	}

	public void setGprs(boolean b) {
		this.isgprs = b;
	}

	//    public void setOldTime(long time) {
	//        this.oldtime = time;
	//        this.currenttime = time;
	//        this.delay = controller.getShortCyc()*1000;
	//        this.delay = controller.getShortCyc()*10;
	//        System.out.println("set delay : "+delay);
	//    }

	/** 开始监听服务器返回激活短信 */
	public void setMonitorSMSPort(String port) {
		System.out.println("WaitingCanvas:setSMSPort(): " + port);
		smsTools = new SMSTools(port);
		smsTools.monitorSMSPort();
		new Thread(new Runnable() {

			public void run() {
				try {
					if (smsTools != null) {
						System.out.println("WaitingCanvas:setSMSPort(): waitting sms from server...");
						smsTools.getReceive();
						System.out.println("result");
						result = smsTools.getReceiveMsg();
						System.out.println("is: " + result);
						if (!result.equals("")) {
							smsTools = null;
							//                            Object[] obj = {result};
							System.out.println("WaitingCanvas:setSMSPort(): "
									+ "register sms received and start uploadPIM");
							System.out.println("runtimes 0 : " + controller.getRuntimes());
							controller.handleEvent(GUIController.EventID.EVENT_INIT_UPLOADPIM, null);
						}
					}
				} catch (IllegalArgumentException iae) {
				} catch (SecurityException se) {
					controller.handleEvent(GUIController.EventID.EVENT_EXIT, null);
				} catch (Exception exc) {
				}
			}
		}).start();
	}

	/** 退出短信监控(等待) */
	public void exitSMS() {
		System.out.println("WaitingCanvas:exitSMS start");
		smsTools.exitReceive();
		System.out.println("WaitingCanvas:exitSMS success");
	}

	//    protected void showNotify() { 
	//        System.out.println("sms: "+smsTools);
	//        start();
	//        if(!controller.getIsInitOk()&&!result.equals("")){
	//            controller.restartGprs();
	//        }
	//    }

	//    protected void hideNotify() {
	//        stop();
	//    }

	public void setInfo(String s) {
		infoVector = new Vector();
		infoVector = StringTools.getSubsection(s, CPProperty.defaultFont, showwidthmax, ".,;?!");
	}
}
