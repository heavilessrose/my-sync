package com.winksi.j2me.cc.view;

import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.control.GUIController;
//import com.winksi.j2me.cc.model.main.DownloadManager;
import com.winksi.j2me.cc.model.main.MenuEvent;
import com.winksi.j2me.cc.model.main.MenuItemChangeManager;
//import com.winksi.j2me.cc.model.main.PIMModel;
import com.winksi.j2me.cc.model.net.HttpListener;
import com.winksi.j2me.cc.tools.ConstImage;
//import java.io.UnsupportedEncodingException;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Vector;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Graphics;

/**
 *
 * @author JERRY
 */
public class CCCanvas extends Canvas implements HttpListener {

	private GUIController controller;
	//    private DownloadManager downloadManager;
	private Timer timer;
	private Menu mainMenu;
	private MenuItem helpMenuItem, aboutMenuItem, exitMenuItem, skipMenuItem, uploadMenuItem, backMenuItem,
			reviewMenuItem, payMenuItem, notPayMenuItem, nextStep2GreetingMenuItem, nextStep2NumberMenuItem;
	private MenuItemChangeManager menuItemChangeManager;
	private AlertBoard alertBoard;
	private Vector fileVector;
	private Hashtable fullnameht = new Hashtable();
	private Vector namevector;
	private Vector numbervector;
	private int event = 0;
	public static final int MAIN = 0;
	public static final int DIY = 1;
	public static final int DIY_IMG = 2;
	public static final int DIY_INFO = 3;
	public static final int DIY_NAME = 4;
	public static final int DIY_NUM = 5;
	public static final int PAY = 6;
	private int keyCode = -100;
	private int selectIndex = 0;
	private int diySelectIndex = 0;
	private int fileSelectIndex = 0;
	private int fileSelectIndexOff = 0;
	//    private int filemoveoff=0;
	private int nameSelectIndex = 0;
	private int nameSelectIndexOff = 0;
	private int numberSelectIndex = 0;
	//    private int numberSelectIndexOff = 0;
	private int itemheight = 0;
	public static final int MAX_SHOW = 6;
	public static final int ITEM_MAX = 3;
	public static final int ITEM_X = 20;
	public static final int ITEM_Y = 77;
	public static final int ITEM_ROW_SPACE = 9;
	public static final int VISIBLE_X = 12;
	public static final int VISIBLE_Y = 64;
	public static final int VISIBLE_W = CPProperty.SCREENWIDTH - 2 * VISIBLE_X;
	public static final int VISIBLE_H = 220;
	private long oldtime = 0;
	private long delay = 0;
	private int fontMove = 0;
	private int count = 0;
	private String curfilename = "";
	private boolean isGprs = false;
	private boolean isAlert = false;
	private String title = "";

	public CCCanvas(GUIController controller) {
		setFullScreenMode(true);
		this.controller = controller;
		//        downloadManager = new DownloadManager(this);
		fileVector = new Vector();
		itemheight = ConstImage.item[0].getHeight();
		mainMenu = new Menu(this.controller);
		initMainmenuItem();
		menuItemChangeManager = new MenuItemChangeManager(this.controller, mainMenu);
		alertBoard = new AlertBoard();
		start();
	}

	public void start() {
		if (timer == null) {//启动定时器
			this.delay = 100000;
			oldtime = System.currentTimeMillis();
			timer = new Timer();
			//            System.out.println("start");
			timer.schedule(new NumberTimerTask(), 0, 100);
		}
	}

	private void stop() {
		if (timer != null) {//停止线程
			timer.cancel();
			timer = null;
		}
	}

	public void showMainMenu() {
		mainMenu.setFocus(true);
		mainMenu.toShow();
	}

	public void removeMenuItem() {
		mainMenu.removeAll();
	}

	protected void paint(Graphics g) {
		g.setColor(0);
		g.fillRect(0, 0, getWidth(), getHeight());
		g.drawImage(ConstImage.main, 0, 0, 20);
		switch (event) {
		case MAIN:
			drawMainList(g);
			break;
		case DIY:
			drawDIYList(g);
			break;
		case DIY_IMG:
			title = CPProperty.DIY_LIST[0][controller.getLanguage()];
			g.setColor(CPProperty.COLOR_CHAR_WHITE);
			g.drawString(title, (CPProperty.SCREENWIDTH + ConstImage.item[3].getWidth() - CPProperty.defaultFont
					.stringWidth(title)) >> 1, CPProperty.TITLE_OFF_Y, 20);

			//                g.setColor(CPProperty.COLOR_CHAR_WHITE);
			//                g.drawString(CPProperty.DIY_LIST[controller.getLanguage()][0], ((CPProperty.SCREENWIDTH + ConstImage.item[3].getWidth()) - CPProperty.defaultFont.stringWidth(CPProperty.DIY_LIST[controller.getLanguage()][0])) >> 1, 0, 20);
			g.drawImage(ConstImage.item[3], 0, 0, 20);

			if (!fileVector.isEmpty()) {
				drawDIYImageList(g);
			}
			break;
		case DIY_NAME:
			g.drawImage(ConstImage.item[3], 0, 0, 20);
			title = CPProperty.DIY_SELECTUSER_LIST[controller.getLanguage()];
			g.setColor(CPProperty.COLOR_CHAR_WHITE);
			g.drawString(title, (CPProperty.SCREENWIDTH + ConstImage.item[3].getWidth() - CPProperty.defaultFont
					.stringWidth(title)) >> 1, CPProperty.TITLE_OFF_Y, 20);

			if (!namevector.isEmpty()) {
				drawNameList(g);
			}
			break;
		case DIY_NUM:
			title = CPProperty.DIY_SELECTNUMBER_LIST[controller.getLanguage()];
			g.setColor(CPProperty.COLOR_CHAR_WHITE);
			g.drawString(title, (CPProperty.SCREENWIDTH + ConstImage.item[3].getWidth() - CPProperty.defaultFont
					.stringWidth(title)) >> 1, CPProperty.TITLE_OFF_Y, 20);

			//                g.setColor(CPProperty.COLOR_CHAR_WHITE);
			//                g.drawString(CPProperty.DIY_SELECTNUMBER_LIST[controller.getLanguage()], ((CPProperty.SCREENWIDTH + ConstImage.item[3].getWidth()) - CPProperty.defaultFont.stringWidth(CPProperty.DIY_SELECTNUMBER_LIST[controller.getLanguage()])) >> 1, 0, 20);

			g.drawImage(ConstImage.item[3], 0, 0, 20);

			if (!numbervector.isEmpty()) {
				drawNumberList(g);
			}
			break;
		case PAY:
			if (!isGprs) {
				alertBoard.drawBorad(g, -1);
			}
			break;
		}

		drawButton(g);
		if (mainMenu != null && mainMenu.isVisible()) {
			mainMenu.drawMenu(g);
		}

		if (isGprs) {
			alertBoard.drawBorad(g, count);
			//            g.setColor(CPProperty.COLOR_CHAR_WHITE);
			//            g.drawString(CPProperty.COMMAND_CANCEL[controller.getLanguage()], CPProperty.SCREENWIDTH - CPProperty.defaultFont.stringWidth(CPProperty.COMMAND_CANCEL[controller.getLanguage()]) - 1, CPProperty.SCREENHEIGHT - CPProperty.dfheight - 2, 20);            
		}

		if (isAlert) {
			alertBoard.drawBorad(g, count);
			g.setColor(CPProperty.COLOR_CHAR_WHITE);
			g.drawString(CPProperty.COMMAND_CANCEL[controller.getLanguage()], CPProperty.SCREENWIDTH
					- CPProperty.defaultFont.stringWidth(CPProperty.COMMAND_CANCEL[controller.getLanguage()]) - 1,
					CPProperty.SCREENHEIGHT - CPProperty.dfheight - 2, 20);
		}
	}

	public void drawButton(Graphics g) {
		if (isGprs || isAlert) {
			return;
		}
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

	public void drawMainList(Graphics g) {
		title = CPProperty.MAIN_TITLE[controller.getLanguage()];
		g.setColor(CPProperty.COLOR_CHAR_WHITE);
		g.drawString(title, (CPProperty.SCREENWIDTH - CPProperty.defaultFont.stringWidth(title)) >> 1,
				CPProperty.TITLE_OFF_Y, 20);
		//        g.setColor(CPProperty.COLOR_CHAR_WHITE);
		//        g.drawString(CPProperty.MAIN_TITLE[controller.getLanguage()], ((CPProperty.SCREENWIDTH + ConstImage.item[3].getWidth()) - CPProperty.defaultFont.stringWidth(CPProperty.MAIN_TITLE[controller.getLanguage()])) >> 1, 0, 20);               
		g.setColor(CPProperty.COLOR_CHAR_WHITE);
		g.drawImage(ConstImage.selector, CPProperty.SCREENWIDTH - ConstImage.selector.getWidth(), ITEM_Y - 3
				+ (itemheight + ITEM_ROW_SPACE) * selectIndex, 20);
		for (int i = 0; i < ITEM_MAX; i++) {
			g.drawImage(ConstImage.item[i], ITEM_X, ITEM_Y + (itemheight + ITEM_ROW_SPACE) * i, 20);
			g.drawString(CPProperty.MAIN_LIST[controller.getLanguage()][i], ITEM_X + ConstImage.item[i].getWidth() + 4,
					ITEM_Y + (itemheight + ITEM_ROW_SPACE) * i + (itemheight - CPProperty.dfheight) / 2, 20);
		}
		g.drawImage(ConstImage.item[selectIndex + ITEM_MAX], ITEM_X, ITEM_Y + (itemheight + ITEM_ROW_SPACE)
				* selectIndex, 20);
	}

	public void drawDIYList(Graphics g) {
		title = CPProperty.MAIN_LIST[controller.getLanguage()][0];
		g.setColor(CPProperty.COLOR_CHAR_WHITE);
		g.drawString(title, (CPProperty.SCREENWIDTH - CPProperty.defaultFont.stringWidth(title)) >> 1,
				CPProperty.TITLE_OFF_Y, 20);

		//        g.setColor(CPProperty.COLOR_CHAR_WHITE);
		//        g.drawString(CPProperty.MAIN_LIST[controller.getLanguage()][0], ((CPProperty.SCREENWIDTH + ConstImage.item[3].getWidth()) - CPProperty.defaultFont.stringWidth(CPProperty.MAIN_LIST[controller.getLanguage()][0])) >> 1, 0, 20);
		g.drawImage(ConstImage.item[3], 0, 0, 20);
		g.drawImage(ConstImage.selector1, CPProperty.SCREENWIDTH - ConstImage.selector.getWidth(), ITEM_Y - 5
				+ (CPProperty.dfheight + ITEM_ROW_SPACE + 2) * diySelectIndex, 20);
		for (int i = 0; i < 2; i++) {
			g.drawImage(ConstImage.single_off, ITEM_X, ITEM_Y + (CPProperty.dfheight + ITEM_ROW_SPACE + 2) * i, 20);
			g.setColor(0xffffff);
			g.drawString(CPProperty.DIY_LIST[i][controller.getLanguage()], ITEM_X + ConstImage.single_off.getWidth()
					+ 3, ITEM_Y + (CPProperty.dfheight + ITEM_ROW_SPACE + 2) * i, 20);
		}
		g.drawImage(ConstImage.single_on, ITEM_X, ITEM_Y + (CPProperty.dfheight + ITEM_ROW_SPACE + 2) * diySelectIndex,
				20);
	}

	public void drawDIYImageList(Graphics g) {
		g.drawImage(ConstImage.selector1, CPProperty.SCREENWIDTH - ConstImage.selector.getWidth(), ITEM_Y
				+ (CPProperty.dfheight + ITEM_ROW_SPACE + 3) * (fileSelectIndex - fileSelectIndexOff) - 5, 20);

		g.setClip(VISIBLE_X, VISIBLE_Y, VISIBLE_W, VISIBLE_H);
		String filename = "";
		for (int i = 0; i < fileVector.size(); i++) {
			g.drawImage(ConstImage.single_off, ITEM_X, ITEM_Y + (CPProperty.dfheight + ITEM_ROW_SPACE + 3)
					* (i - fileSelectIndexOff), 20);
			g.setColor(0xffffff);
			if (fileSelectIndex != i) {
				filename = fileVector.elementAt(i).toString();
				if (filename.startsWith("file:///c")) {
					filename += CPProperty.DIYPATH_C_NAME[controller.getLanguage()];
				} else {
					filename += CPProperty.DIYPATH_E_NAME[controller.getLanguage()];
				}
				filename = filename.substring(filename.lastIndexOf('/') + 1);
				g.drawString(filename, ITEM_X + ConstImage.single_off.getWidth() + 3, ITEM_Y
						+ (CPProperty.dfheight + ITEM_ROW_SPACE + 3) * (i - fileSelectIndexOff), 20);
			}
		}
		g.drawImage(ConstImage.single_on, ITEM_X, ITEM_Y + (CPProperty.dfheight + ITEM_ROW_SPACE + 3)
				* (fileSelectIndex - fileSelectIndexOff), 20);
		g.setClip(0, 0, CPProperty.SCREENWIDTH, CPProperty.SCREENHEIGHT);

		g.setClip(ITEM_X + ConstImage.single_off.getWidth() + 3, ITEM_Y + (CPProperty.dfheight + ITEM_ROW_SPACE + 3)
				* (fileSelectIndex - fileSelectIndexOff), VISIBLE_W - ConstImage.single_off.getWidth() - 3,
				ConstImage.single_off.getHeight());
		String name = fileVector.elementAt(fileSelectIndex).toString();
		if (name.startsWith("file:///c")) {
			name += CPProperty.DIYPATH_C_NAME[controller.getLanguage()];
		} else {
			name += CPProperty.DIYPATH_E_NAME[controller.getLanguage()];
		}
		name = name.substring(name.lastIndexOf('/') + 1);
		int namewidth = CPProperty.defaultFont.stringWidth(name);
		//        System.out.println("fw : "+namewidth);
		//        System.out.println("nw : "+(VISIBLE_W - ITEM_X-ConstImage.single_off.getWidth() - 3));
		if (namewidth - (VISIBLE_W - ITEM_X - ConstImage.single_off.getWidth() - 3) > 0) {
			g.drawString(name, ITEM_X + ConstImage.single_off.getWidth() + 3 - fontMove, ITEM_Y
					+ (CPProperty.dfheight + ITEM_ROW_SPACE + 3) * (fileSelectIndex - fileSelectIndexOff), 20);
			if (fontMove > (VISIBLE_W - ConstImage.single_off.getWidth() - 3)) {
				fontMove = 0;
			}
		} else {
			g.drawString(name, ITEM_X + ConstImage.single_off.getWidth() + 3, ITEM_Y
					+ (CPProperty.dfheight + ITEM_ROW_SPACE + 3) * (fileSelectIndex - fileSelectIndexOff), 20);
		}
		g.setClip(0, 0, CPProperty.SCREENWIDTH, CPProperty.SCREENHEIGHT);
	}

	public void drawNameList(Graphics g) {
		g.drawImage(ConstImage.selector1, CPProperty.SCREENWIDTH - ConstImage.selector.getWidth(), ITEM_Y
				+ (CPProperty.dfheight + ITEM_ROW_SPACE + 3) * (nameSelectIndex - nameSelectIndexOff) - 5, 20);

		g.setClip(VISIBLE_X, VISIBLE_Y, VISIBLE_W, VISIBLE_H);
		for (int i = 0; i < namevector.size(); i++) {
			g.drawImage(ConstImage.single_off, ITEM_X, ITEM_Y + (CPProperty.dfheight + ITEM_ROW_SPACE + 3)
					* (i - nameSelectIndexOff), 20);
			g.setColor(0xffffff);
			g.drawString(namevector.elementAt(i).toString(), ITEM_X + ConstImage.single_off.getWidth() + 3, ITEM_Y
					+ (CPProperty.dfheight + ITEM_ROW_SPACE + 3) * (i - nameSelectIndexOff), 20);
		}
		g.drawImage(ConstImage.single_on, ITEM_X, ITEM_Y + (CPProperty.dfheight + ITEM_ROW_SPACE + 3)
				* (nameSelectIndex - nameSelectIndexOff), 20);
		g.setClip(0, 0, CPProperty.SCREENWIDTH, CPProperty.SCREENHEIGHT);
	}

	public void drawNumberList(Graphics g) {
		g.drawImage(ConstImage.selector1, CPProperty.SCREENWIDTH - ConstImage.selector.getWidth(), ITEM_Y
				+ (CPProperty.dfheight + ITEM_ROW_SPACE + 3) * numberSelectIndex - 5, 20);

		g.setClip(VISIBLE_X, VISIBLE_Y, VISIBLE_W, VISIBLE_H);
		for (int i = 0; i < numbervector.size() - 1; i++) {
			g.drawImage(ConstImage.single_off, ITEM_X, ITEM_Y + (CPProperty.dfheight + ITEM_ROW_SPACE + 3) * (i), 20);
			g.setColor(0xffffff);
			g.drawString(numbervector.elementAt(i + 1).toString(), ITEM_X + ConstImage.single_off.getWidth() + 3,
					ITEM_Y + (CPProperty.dfheight + ITEM_ROW_SPACE + 3) * (i), 20);
		}
		g.drawImage(ConstImage.single_on, ITEM_X, ITEM_Y + (CPProperty.dfheight + ITEM_ROW_SPACE + 3)
				* numberSelectIndex, 20);
		g.setClip(0, 0, CPProperty.SCREENWIDTH, CPProperty.SCREENHEIGHT);
	}

	protected void keyPressed(int keycode) {
		keyCode = keycode;
		if (controller.getIsPushWakeup()) {
			return;
		}

		if (!isGprs) {
			if (isAlert) {
				if (keyCode == CPProperty.KEY_SOFT_RIGHT) {// right soft key
					isAlert = false;
				}
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
					switch (event) {
					case MAIN:
						selectIndex = upSelector(selectIndex, ITEM_MAX);
						break;
					case DIY:
						diySelectIndex = upSelector(diySelectIndex, 2);
						break;
					case DIY_IMG:
						fontMove = 0;
						fileSelectIndex--;
						if (fileSelectIndex < 0) {
							fileSelectIndex = 0;
							fileSelectIndexOff = 0;
						} else {
							if (fileSelectIndex - fileSelectIndexOff < 0) {
								fileSelectIndexOff--;
							}
						}
						if (fileVector.size() > 0) {
							curfilename = /* "file:///" + CPProperty.DIYPATH + "/" + */(String) fileVector
									.elementAt(fileSelectIndex);
						}
						break;
					case DIY_NAME:
						nameSelectIndex--;
						if (nameSelectIndex < 0) {
							nameSelectIndex = 0;
							nameSelectIndexOff = 0;
						} else {
							if (nameSelectIndex - nameSelectIndexOff < 0) {
								nameSelectIndexOff--;
							}
						}
						break;
					case DIY_NUM:
						numberSelectIndex = upSelector(numberSelectIndex, numbervector.size() - 1);
						String phonenumber = (String) numbervector.elementAt(numberSelectIndex + 1);
						controller.setDiyNum(phonenumber.getBytes());
						break;
					}
				} else if (keyCode == CPProperty.KEY_NUM3) {// 3

				} else if (keyCode == CPProperty.KEY_NUM4) {// 4

				} else if (keyCode == CPProperty.KEY_NUM5) {// 5

				} else if (keyCode == CPProperty.KEY_NUM6) {// 6

				} else if (keyCode == CPProperty.KEY_NUM7) {// 7

				} else if (keyCode == CPProperty.KEY_NUM8) {// 8
					switch (event) {
					case MAIN:
						selectIndex = downSelector(selectIndex, ITEM_MAX);
						break;
					case DIY:
						diySelectIndex = downSelector(diySelectIndex, 2);
						break;
					case DIY_IMG:
						fontMove = 0;
						fileSelectIndex++;
						if (fileSelectIndex > fileVector.size() - 1) {
							fileSelectIndex = fileVector.size() - 1;
						} else {
							if (fileSelectIndex > MAX_SHOW - 1) {
								fileSelectIndexOff++;
								if (fileSelectIndex - fileSelectIndexOff < MAX_SHOW - 1) {
									fileSelectIndexOff = fileSelectIndex - MAX_SHOW - 1;
								}
							}
						}
						if (fileVector.size() > 0) {
							curfilename = /*"file:///" + CPProperty.DIYPATH + "/" + */(String) fileVector
									.elementAt(fileSelectIndex);
						}
						break;
					case DIY_NAME:
						nameSelectIndex++;
						if (nameSelectIndex > namevector.size() - 1) {
							nameSelectIndex = namevector.size() - 1;
						} else {
							if (nameSelectIndex - nameSelectIndexOff > MAX_SHOW - 1) {
								nameSelectIndexOff++;
								if (nameSelectIndex - nameSelectIndexOff < MAX_SHOW - 1) {
									nameSelectIndexOff = nameSelectIndex - MAX_SHOW - 1;
								}
							}
						}
						break;
					case DIY_NUM:
						numberSelectIndex = downSelector(numberSelectIndex, numbervector.size() - 1);
						String phonenumber = (String) numbervector.elementAt(numberSelectIndex + 1);
						controller.setDiyNum(phonenumber.getBytes());
						break;
					}
				} else if (keyCode == CPProperty.KEY_NUM9) {// 9

				} else if (keyCode == CPProperty.KEY_STAR) {// *

				} else if (keyCode == CPProperty.KEY_POUND) {// #

				} else if (keyCode == CPProperty.KEY_BACK) {// back key
					controller.handleEvent(GUIController.EventID.EVENT_BACK, null);
				} else if (keyCode == CPProperty.KEY_CLEAR) {// clear key

				} else if (keyCode == CPProperty.KEY_SOUND_UP) {// sound up key

				} else if (keyCode == CPProperty.KEY_SOUND_DWON) {// sound down
					// key

				} else if (keyCode == CPProperty.KEY_UP) {// up
					switch (event) {
					case MAIN:
						selectIndex = upSelector(selectIndex, ITEM_MAX);
						break;
					case DIY:
						diySelectIndex = upSelector(diySelectIndex, 2);
						break;
					case DIY_IMG:
						fontMove = 0;
						fileSelectIndex--;
						if (fileSelectIndex < 0) {
							fileSelectIndex = 0;
							fileSelectIndexOff = 0;
						} else {
							if (fileSelectIndex - fileSelectIndexOff < 0) {
								fileSelectIndexOff--;
							}
						}
						if (fileVector.size() > 0) {
							curfilename = /*"file:///" + CPProperty.DIYPATH + "/" +*/(String) fileVector
									.elementAt(fileSelectIndex);
						}
						break;
					case DIY_NAME:
						nameSelectIndex--;
						if (nameSelectIndex < 0) {
							nameSelectIndex = 0;
							nameSelectIndexOff = 0;
						} else {
							if (nameSelectIndex - nameSelectIndexOff < 0) {
								nameSelectIndexOff--;
							}
						}
						break;
					case DIY_NUM:
						numberSelectIndex = upSelector(numberSelectIndex, numbervector.size() - 1);
						String phonenumber = (String) numbervector.elementAt(numberSelectIndex + 1);
						controller.setDiyNum(phonenumber.getBytes());
						break;
					}

				} else if (keyCode == CPProperty.KEY_DOWN) {// down
					switch (event) {
					case MAIN:
						selectIndex = downSelector(selectIndex, ITEM_MAX);
						break;
					case DIY:
						diySelectIndex = downSelector(diySelectIndex, 2);
						break;
					case DIY_IMG:
						fontMove = 0;
						fileSelectIndex++;
						if (fileSelectIndex > fileVector.size() - 1) {
							fileSelectIndex = fileVector.size() - 1;
						} else {
							if (fileSelectIndex - fileSelectIndexOff > MAX_SHOW - 1) {
								fileSelectIndexOff++;
								if (fileSelectIndex - fileSelectIndexOff < MAX_SHOW - 1) {
									fileSelectIndexOff = fileSelectIndex - MAX_SHOW - 1;
								}
							}
						}
						if (fileVector.size() > 0) {
							curfilename = /*"file:///" + CPProperty.DIYPATH + "/" +*/(String) fileVector
									.elementAt(fileSelectIndex);
						}
						break;
					case DIY_NAME:
						nameSelectIndex++;
						if (nameSelectIndex > namevector.size() - 1) {
							nameSelectIndex = namevector.size() - 1;
						} else {
							if (nameSelectIndex - nameSelectIndexOff > MAX_SHOW - 1) {
								nameSelectIndexOff++;
								if (nameSelectIndex - nameSelectIndexOff < MAX_SHOW - 1) {
									nameSelectIndexOff = nameSelectIndex - MAX_SHOW - 1;
								}
							}
						}
						break;
					case DIY_NUM:
						numberSelectIndex = downSelector(numberSelectIndex, numbervector.size() - 1);
						String phonenumber = (String) numbervector.elementAt(numberSelectIndex + 1);
						controller.setDiyNum(phonenumber.getBytes());
						break;
					}
				} else if (keyCode == CPProperty.KEY_LEFT) {// left

				} else if (keyCode == CPProperty.KEY_RIGHT) {// right

				} else if (keyCode == CPProperty.KEY_OK) {// ok
					switch (event) {
					case MAIN:
						if (selectIndex == 0) {//DIY
							controller.pushPreEvent(GUIController.EventID.EVENT_SHOW_MAIN);
							controller.handleEvent(GUIController.EventID.EVENT_DIY, null);
							diySelectIndex = 0;
						} else if (selectIndex == 1) {//UPDATA
							controller.handleEvent(GUIController.EventID.EVENT_UPDATASOFTWARE, null);
						} else if (selectIndex == 2) {//REGISTER
							controller.handleEvent(GUIController.EventID.EVENT_REGISTER_BACK, null);
						}
						break;
					case DIY:
						if (diySelectIndex == 0) {//DIY image                                 
							controller.handleEvent(GUIController.EventID.EVENT_FILELIST, null);
							if (fileVector.size() > 0) {
								controller.pushPreEvent(GUIController.EventID.EVENT_DIY);
								fileSelectIndex = 0;
								fileSelectIndexOff = 0;
								fontMove = 0;
								curfilename = /*"file:///" + CPProperty.DIYPATH + "/" + */(String) fileVector
										.elementAt(fileSelectIndex);
								//                                    controller.setDiyImg(currfilename);
								//                                    System.out.println("currfilename : "+currfilename);
							} else {
								event = MAIN;
								Object[] objs = { CPProperty.ERROR_NO_FILE[controller.getLanguage()] };
								controller.handleEvent(GUIController.EventID.EVENT_ALERT, objs);
							}
						} else if (diySelectIndex == 1) {//edit greeting
							curfilename = "";
							controller.pushPreEvent(GUIController.EventID.EVENT_DIY);
							controller.handleEvent(GUIController.EventID.EVENT_SHOW_GREETING, null);
						}
						break;
					case DIY_IMG:
						go2Greeting();
						break;
					case DIY_NAME:
						go2Number();
						break;
					case DIY_NUM:
						//                            String phonenumber = (String) numbervector.elementAt(numberSelectIndex+1);
						//                            controller.setDiyNum(phonenumber.getBytes());
						controller.handleEvent(GUIController.EventID.EVENT_INIT_UPLOAD_DIY, null);
						numberSelectIndex = 0;
						break;
					}
				} else if (keyCode == CPProperty.KEY_SOFT_LEFT) {// left soft key
					if (!mainMenu.isVisible()) {
						switch (event) {
						case MAIN:
							controller.handleEvent(GUIController.EventID.EVENT_MAIN_MENU, null);
							break;
						case DIY:
							controller.handleEvent(GUIController.EventID.EVENT_DIY_MENU, null);
							break;
						case DIY_IMG:
							controller.handleEvent(GUIController.EventID.EVENT_MAIN_DIY_IMG_MENU, null);
							break;
						case DIY_NAME:
							controller.handleEvent(GUIController.EventID.EVENT_MAIN_DIY_NAME_MENU, null);
							break;
						case DIY_NUM:
							controller.handleEvent(GUIController.EventID.EVENT_MAIN_DIY_NUM_MENU, null);
							break;
						case PAY:
							controller.handleEvent(GUIController.EventID.EVENT_MAIN_PAY_MENU, null);
							break;
						}

					}
				} else if (keyCode == CPProperty.KEY_SOFT_RIGHT) {// right soft key

				}
			}
		} else {
			if (keyCode == CPProperty.KEY_SOFT_RIGHT) {// right soft key
				//                if (isgprs) {
				//                    controller.initDiy();
				//                    controller.cancelGprs();                    
				//                    controller.handleEvent(GUIController.EventID.EVENT_CURRENT, null);
				//                    isgprs=false;
				//                }
				if (isAlert) {
					isAlert = false;
				}
			}
		}
		repaint();
	}

	/** 彩像 屏(主界面)菜单项初始化 */
	public void initMainmenuItem() {
		int[] itemevent1 = { MenuEvent.BACK, MenuEvent.HELP };
		helpMenuItem = new MenuItem(CPProperty.COMMAND_HELP[controller.getLanguage()], itemevent1, false);
		int[] itemevent2 = { MenuEvent.BACK, MenuEvent.ABOUT };
		aboutMenuItem = new MenuItem(CPProperty.COMMAND_ABOUT[controller.getLanguage()], itemevent2, false);
		int[] itemevent3 = { MenuEvent.BACK, MenuEvent.EXIT };
		exitMenuItem = new MenuItem(CPProperty.COMMAND_EXIT[controller.getLanguage()], itemevent3, false);
		mainMenu.addMenuItem(helpMenuItem);
		mainMenu.addMenuItem(aboutMenuItem);
		mainMenu.addMenuItem(exitMenuItem);
	}

	/** 自定义 屏菜单项初始化 */
	public void initDiy_MenuItem() {
		int[] itemevent1 = { MenuEvent.BACK, MenuEvent.HELP };
		helpMenuItem = new MenuItem(CPProperty.COMMAND_HELP[controller.getLanguage()], itemevent1, false);
		int[] itemevent2 = { MenuEvent.BACK, MenuEvent.ABOUT };
		aboutMenuItem = new MenuItem(CPProperty.COMMAND_ABOUT[controller.getLanguage()], itemevent2, false);
		int[] itemevent4 = { MenuEvent.BACK, MenuEvent.DIY_BACK };
		backMenuItem = new MenuItem(CPProperty.COMMAND_BACK[controller.getLanguage()], itemevent4, false);
		//        int[] itemevent3 = {MenuEvent.BACK, MenuEvent.EXIT};
		//        exitMenuItem = new MenuItem(CPProperty.COMMAND_EXIT[controller.getLanguage()], itemevent3, false);

		mainMenu.addMenuItem(helpMenuItem);
		mainMenu.addMenuItem(aboutMenuItem);
		mainMenu.addMenuItem(backMenuItem);
		//        mainMenu.addMenuItem(exitMenuItem);
	}

	/** 上传DIY图片 屏菜单项初始化 */
	public void initDiy_ImgMenuItem() {
		int[] itemevent = { MenuEvent.BACK, MenuEvent.DIY_IMG_REVIEW };
		reviewMenuItem = new MenuItem(CPProperty.COMMAND_REVIEW[controller.getLanguage()], itemevent, false);
		int[] itemevent1 = { MenuEvent.BACK, MenuEvent.DIY_IMG_SKIP };
		skipMenuItem = new MenuItem(CPProperty.COMMAND_SKIP[controller.getLanguage()], itemevent1, false);
		int[] itemevent2 = { MenuEvent.BACK, MenuEvent.UPLOAD };
		uploadMenuItem = new MenuItem(CPProperty.COMMAND_UPLOAD[controller.getLanguage()], itemevent2, false);
		int[] itemevent3 = { MenuEvent.BACK, MenuEvent.DIY_NEXTSTEP2GREETING };
		nextStep2GreetingMenuItem = new MenuItem(CPProperty.COMMAND_NEXT[controller.getLanguage()], itemevent3, false);
		int[] itemevent4 = { MenuEvent.BACK, MenuEvent.DIY_BACK };
		backMenuItem = new MenuItem(CPProperty.COMMAND_BACK[controller.getLanguage()], itemevent4, false);
		mainMenu.addMenuItem(nextStep2GreetingMenuItem);
		mainMenu.addMenuItem(reviewMenuItem);
		mainMenu.addMenuItem(skipMenuItem);
		mainMenu.addMenuItem(uploadMenuItem);
		mainMenu.addMenuItem(backMenuItem);
		//        mainMenu.addMenuItem(exitMenuItem);
	}

	/** 选择对方用户 屏菜单项初始化 */
	public void initDiy_selectUserMenuItem() {
		int[] itemevent3 = { MenuEvent.BACK, MenuEvent.DIY_NEXTSTEP2NUMBER };
		nextStep2NumberMenuItem = new MenuItem(CPProperty.COMMAND_NEXT[controller.getLanguage()], itemevent3, false);
		int[] itemevent4 = { MenuEvent.BACK, MenuEvent.DIY_BACK };
		backMenuItem = new MenuItem(CPProperty.COMMAND_BACK[controller.getLanguage()], itemevent4, false);

		mainMenu.addMenuItem(nextStep2NumberMenuItem);
		mainMenu.addMenuItem(backMenuItem);
	}

	/**　选择用户号码　屏菜单项初始化 */
	public void initDiy_selectUserNumberMenuItem() {
		int[] itemevent2 = { MenuEvent.BACK, MenuEvent.UPLOAD };
		uploadMenuItem = new MenuItem(CPProperty.COMMAND_UPLOAD[controller.getLanguage()], itemevent2, false);

		int[] itemevent4 = { MenuEvent.BACK, MenuEvent.DIY_BACK };
		backMenuItem = new MenuItem(CPProperty.COMMAND_BACK[controller.getLanguage()], itemevent4, false);

		mainMenu.addMenuItem(uploadMenuItem);
		mainMenu.addMenuItem(backMenuItem);
		//        mainMenu.addMenuItem(exitMenuItem);
	}

	public void initMenuItem5() {
		int[] itemevent1 = { MenuEvent.BACK, MenuEvent.NOTPAY };
		notPayMenuItem = new MenuItem(CPProperty.COMMAND_BACK[controller.getLanguage()], itemevent1, false);
		int[] itemevent2 = { MenuEvent.BACK, MenuEvent.PAY };
		payMenuItem = new MenuItem(CPProperty.COMMAND_SURE[controller.getLanguage()], itemevent2, false);
		//        int[] itemevent3 = {MenuEvent.BACK, MenuEvent.EXIT};
		//        exitMenuItem = new MenuItem(CPProperty.COMMAND_EXIT[controller.getLanguage()], itemevent3, false);
		mainMenu.addMenuItem(notPayMenuItem);
		mainMenu.addMenuItem(payMenuItem);
		//        mainMenu.addMenuItem(exitMenuItem);
	}

	private int upSelector(int select, int max) {
		select--;
		if (select < 0) {
			select = max - 1;
		}
		return select;
	}

	private int downSelector(int select, int max) {
		select++;
		if (select > max - 1) {
			select = 0;
		}
		return select;
	}

	/** 选择要使用的DIY图片 */
	public void setFileList(Vector v) {
		//        fileSelectIndex = 0;
		//        fileSelectIndexOff = 0;
		if (fileSelectIndex < 0) {
			fileSelectIndex = 0;
			fileSelectIndexOff = 0;
		}
		fileVector = v;
		if (fileSelectIndex > fileVector.size() - 1) {
			fileSelectIndex = fileVector.size() - 1;
			fileSelectIndexOff = fileSelectIndex - MAX_SHOW + 1;
		}
		if (fileVector.size() <= MAX_SHOW) {
			fileSelectIndexOff = 0;
		}
		if (fileVector.size() > 0) {
			curfilename = /*"file:///" + CPProperty.DIYPATH + "/" + */(String) fileVector.elementAt(fileSelectIndex);
		}
	}

	/** 定制联系人 */
	public void setNameList(Hashtable ht) {
		fullnameht = ht;
		namevector = new Vector();
		nameSelectIndex = 0;
		nameSelectIndexOff = 0;
		Enumeration e = fullnameht.keys();
		while (e.hasMoreElements()) {
			namevector.addElement((String) e.nextElement());
		}
	}

	/** 定制联系人电话号码 */
	public void setNumberList() {
		numberSelectIndex = 0;
		numbervector = (Vector) fullnameht.get((String) namevector.elementAt(nameSelectIndex));
		String phonenumber = (String) numbervector.elementAt(numberSelectIndex + 1);
		controller.setDiyNum(phonenumber.getBytes());
	}

	/** 到问候语设置 */
	public void go2Greeting() {
		if (fileVector.isEmpty()) {
			return;
		}
		//        String filename = currfilename.substring(currfilename.lastIndexOf('/')+1);
		//        System.out.println("currfilename : "+currfilename);
		if (controller.setDiyImg(curfilename)) {
			//        if (!currfilename.equals("")) {
			controller.pushPreEvent(GUIController.EventID.EVENT_FILELIST);
			controller.handleEvent(GUIController.EventID.EVENT_SHOW_GREETING, null);
			//            nameSelectIndex = 0;
			//            nameSelectIndexOff = 0;
		} else {
			if (fileSelectIndex < 0) {
				fileSelectIndex = 0;
				fileSelectIndexOff = 0;
			}
			//        filevector = v;
			if (fileSelectIndex > fileVector.size() - 1) {
				fileSelectIndex = fileVector.size() - 1;
				fileSelectIndexOff = fileSelectIndex - MAX_SHOW + 1;
			}
			if (fileVector.size() <= MAX_SHOW) {
				fileSelectIndexOff = 0;
			}
			if (fileVector.size() > 0) {
				curfilename = /*"file:///" + CPProperty.DIYPATH + "/" + */(String) fileVector
						.elementAt(fileSelectIndex);
			}
		}
	}

	/** 到选择号码 */
	public void go2Number() {
		if (namevector.size() > 0) {
			controller.pushPreEvent(GUIController.EventID.EVENT_NAMELIST);
			controller.handleEvent(GUIController.EventID.EVENT_NUMBERLIST, null);
			numberSelectIndex = 0;
		}
	}

	public void setEvent(int event) {
		this.event = event;
	}

	public int getEvent() {
		return this.event;
	}

	public void onFinish(byte[] result) {
		System.out.println("CCCanvas onFinish()");
		Object[] obj = { result };
		controller.handleEvent(GUIController.EventID.EVENT_PARSE, obj);
	}

	public void onProgress(int percent) {
	}

	public void onError(int code) {
		if (code == 12) {
			controller.handleEvent(GUIController.EventID.EVENT_EXIT, null);
		}
		if (controller.getIsPushWakeup()) {
			controller.handleEvent(GUIController.EventID.EVENT_EXIT, null);
		} else {
			System.out.println("CCCanvas:onErrro");
			controller.handleEvent(GUIController.EventID.EVENT_ERROR, null);
		}
	}

	public void setGprs(boolean b) {
		this.isGprs = b;
	}

	public void showAlert(boolean b) {
		count = 0;
		this.isAlert = b;
	}

	/** 设置警告(提示)信息 */
	public void setAlertInfo(String s) {
		alertBoard.setAlertInfo(s);
	}

	public void hideMainMenu() {
		this.mainMenu.toHide();
	}

	//    public void setDelay() {        
	//        start();
	//    }

	public void setCurFileName(String name) {
		this.curfilename = name;
	}

	public String getCurFilename() {
		return this.curfilename;
	}

	//    public String getReviewFilename(){
	//        return this.currfilename;
	//    }

	class NumberTimerTask extends TimerTask {

		public void run() {
			if (isGprs && mainMenu.isVisible()) {
				mainMenu.toHide();
			}

			count++;
			if (count > 100) {
				if (count > 50) {
					isAlert = false;
				}
				count = 0;
			}

			fontMove++;
			if (fontMove > 1000) {
				fontMove = 0;
			}
			repaint();
		}
	}
}
