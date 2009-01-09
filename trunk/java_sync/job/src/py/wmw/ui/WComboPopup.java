package py.wmw.ui;

import javax.swing.*;
import javax.swing.plaf.basic.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;
import java.io.Serializable;

/**
 * <p>
 * Title:
 * </p>
 * <p>
 * Description:
 * </p>
 * <p>
 * Copyright: Copyright (c) 2004
 * </p>
 * <p>
 * Company: 深圳尘日软件工作室
 * </p>
 * 
 * @author wangsong
 * @version 1.1
 */

public class WComboPopup extends BasicComboPopup {
	// protected KeyListener keyListener;
	protected JList list;

	public WComboPopup(JComboBox combo) {
		super(combo);
	}

	public static void main(String[] args) {
		// WComboPopup WComboPopup1 = new WComboPopup();
	}
	// public KeyListener getKeyListener() {
	// if (keyListener == null) {
	// keyListener = new InvocationKeyHandler();
	// }
	// return keyListener;
	// }
	// protected class InvocationKeyHandler extends KeyAdapter {
	// public void keyPressed(KeyEvent e) {
	// System.out.println("InvocationKeyHandler");
	// }}
}
