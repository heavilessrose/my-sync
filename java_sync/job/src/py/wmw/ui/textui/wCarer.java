package py.wmw.ui.textui;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: 深圳尘日软件工作室 </p>
 * @author  wangsong
 * @version 1.1
 */

import java.awt.*;
import java.awt.event.*;
import java.awt.datatransfer.*;
import java.beans.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.plaf.*;
import java.util.EventListener;

public class wCarer extends javax.swing.text.DefaultCaret {
	public wCarer() {
		super();
	}

	/**
	 * If button 1 is pressed, this is implemented to request focus on the
	 * associated text component, and to set the caret position. If the shift
	 * key is held down, the caret will be moved, potentially resulting in a
	 * selection, otherwise the caret position will be set to the new location.
	 * If the component is not enabled, there will be no request for focus. If
	 * isMetaDown, show popmenu that have the function of cut copy past you can
	 * add some else menuItem to PopMemu do something that your wanted! 王明伟
	 * 2005.5.1 上海 Have qestion Mail the: wmwcpu88@hotmail.com
	 * 
	 * @param e
	 *            the mouse event
	 * @see MouseListener#mousePressed
	 */

	public void mousePressed(MouseEvent mouseevent) {
		super.mousePressed(mouseevent);
		try {
			if (mouseevent.isMetaDown()) {
				WTextPopMenu popupMenu;
				if (this.getComponent() instanceof javax.swing.JTextField) {
					javax.swing.JTextField jt = (javax.swing.JTextField) this
							.getComponent();
					popupMenu = new WTextPopMenu(jt);
					Dimension screenSize = Toolkit.getDefaultToolkit()
							.getScreenSize();
					int wdt = mouseevent.getX();
					int hgt = mouseevent.getY();
					if (wdt + jt.getLocationOnScreen().x
							+ popupMenu.getPreferredSize().width > screenSize.width) {
						wdt = wdt - popupMenu.getPreferredSize().height;
					}
					if (hgt + jt.getLocationOnScreen().y
							+ popupMenu.getPreferredSize().height + 20 > screenSize.height) {
						hgt = hgt - popupMenu.getPreferredSize().height;
					}
					popupMenu.show(jt, wdt, hgt);

				}

				if ((this.getComponent() instanceof javax.swing.JTextArea)) {
					javax.swing.JTextArea jti = (javax.swing.JTextArea) this
							.getComponent();
					popupMenu = new WTextPopMenu(jti);
					Dimension screenSize = Toolkit.getDefaultToolkit()
							.getScreenSize();
					int wdt = mouseevent.getX();
					int hgt = mouseevent.getY();
					if (wdt + jti.getLocationOnScreen().x
							+ popupMenu.getPreferredSize().width > screenSize.width) {
						wdt = wdt - popupMenu.getPreferredSize().height;
					}
					if (hgt + jti.getLocationOnScreen().y
							+ popupMenu.getPreferredSize().height + 20 > screenSize.height) {
						hgt = hgt - popupMenu.getPreferredSize().height;
					}
					popupMenu.show(jti, wdt, hgt);
				}
				if ((this.getComponent() instanceof javax.swing.JTextPane)) {
					javax.swing.JTextPane jti = (javax.swing.JTextPane) this
							.getComponent();
					popupMenu = new WTextPopMenu(jti);
					Dimension screenSize = Toolkit.getDefaultToolkit()
							.getScreenSize();
					int wdt = mouseevent.getX();
					int hgt = mouseevent.getY();
					if (wdt + jti.getLocationOnScreen().x
							+ popupMenu.getPreferredSize().width > screenSize.width) {
						wdt = wdt - popupMenu.getPreferredSize().height;
					}
					if (hgt + jti.getLocationOnScreen().y
							+ popupMenu.getPreferredSize().height + 20 > screenSize.height) {
						hgt = hgt - popupMenu.getPreferredSize().height;
					}
					popupMenu.show(jti, wdt, hgt);
				}

			}

		} catch (Exception eer) {
		}
	}

}
