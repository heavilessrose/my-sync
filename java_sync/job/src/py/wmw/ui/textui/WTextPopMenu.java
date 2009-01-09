package py.wmw.ui.textui;

import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.awt.*;
import java.awt.datatransfer.*;

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

public class WTextPopMenu extends JPopupMenu {
	javax.swing.JMenuItem c = new javax.swing.JMenuItem("剪切");
	javax.swing.JMenuItem co = new javax.swing.JMenuItem("复制");
	javax.swing.JMenuItem p = new javax.swing.JMenuItem("粘贴");
	int s = 0;
	int n = 0;

	private String ClipHave() {
		java.awt.datatransfer.Transferable content;
		String PaData = "";
		java.awt.datatransfer.Clipboard clipboard = Toolkit.getDefaultToolkit()
				.getSystemClipboard();
		content = clipboard.getContents(this);
		if (content == null) {
			return "";
		}
		try {
			if (content
					.isDataFlavorSupported(java.awt.datatransfer.DataFlavor.stringFlavor)) {
				PaData = (String) content
						.getTransferData(java.awt.datatransfer.DataFlavor.stringFlavor);
			}
		} catch (Exception eee) {
			eee.printStackTrace();
			return "";
		}
		return PaData;
	}

	public WTextPopMenu(final JTextArea Wt) {
		c.setEnabled(false);
		co.setEnabled(false);
		p.setEnabled(false);
		if (Wt.getSelectedText() != null && Wt.getSelectedText().length() > 0) {
			c.setEnabled(true);
			co.setEnabled(true);
		}
		String ClipHave = ClipHave();
		if (ClipHave.length() > 0) {
			p.setEnabled(true);
		}
		if (!Wt.isEditable()) {
			c.setEnabled(false);
			p.setEnabled(false);
		}
		s = Wt.getSelectionStart();
		n = Wt.getSelectionEnd();
		addli();
	}

	public WTextPopMenu(final JTextPane Wt) {
		c.setEnabled(false);
		co.setEnabled(false);
		p.setEnabled(false);
		if (Wt.getSelectedText() != null && Wt.getSelectedText().length() > 0) {
			c.setEnabled(true);
			co.setEnabled(true);
		}
		String ClipHave = ClipHave();
		if (ClipHave.length() > 0) {
			p.setEnabled(true);
		}
		if (!Wt.isEditable()) {
			c.setEnabled(false);
			p.setEnabled(false);
		}
		s = Wt.getSelectionStart();
		n = Wt.getSelectionEnd();
		addli();
	}

	public WTextPopMenu(final JTextField Wt) {
		c.setEnabled(false);
		co.setEnabled(false);
		p.setEnabled(false);
		if (Wt.getSelectedText() != null && Wt.getSelectedText().length() > 0) {
			c.setEnabled(true);
			co.setEnabled(true);
		}
		String ClipHave = ClipHave();
		if (ClipHave.length() > 0) {
			p.setEnabled(true);
		}
		if (!Wt.isEditable()) {
			c.setEnabled(false);
			p.setEnabled(false);
		}
		s = Wt.getSelectionStart();
		n = Wt.getSelectionEnd();
		addli();
	}

	void addli() {
		c
				.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_X,
						Event.CTRL_MASK));
		co.setAccelerator(KeyStroke
				.getKeyStroke(KeyEvent.VK_C, Event.CTRL_MASK));
		p
				.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_V,
						Event.CTRL_MASK));
		c.setFont(new java.awt.Font("新宋体", 0, 12));
		co.setFont(new java.awt.Font("新宋体", 0, 12));
		p.setFont(new java.awt.Font("新宋体", 0, 12));
		this.add(c);
		this.add(co);
		this.add(p);
		c.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					/**
					 * Cuts the selected region and place its contents into the
					 * system clipboard.
					 * <p>
					 * <strong>Warning:</strong> Serialized objects of this
					 * class will not be compatible with future Swing releases.
					 * The current serialization support is appropriate for
					 * short term storage or RMI between applications running
					 * the same version of Swing. As of 1.4, support for long
					 * term storage of all JavaBeans<sup><font
					 * size="-2">TM</font></sup> has been added to the
					 * <code>java.beans</code> package. Please see
					 * {@link java.beans.XMLEncoder}.
					 * 
					 * @see DefaultEditorKit#cutAction
					 * @see DefaultEditorKit#getActions
					 */

					new javax.swing.text.DefaultEditorKit.CutAction()
							.actionPerformed(e);

				} catch (Exception eeer) {
					eeer.printStackTrace();
				}
			}
		});

		co.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					/**
					 * Copies the selected region and place its contents into
					 * the system clipboard.
					 * <p>
					 * <strong>Warning:</strong> Serialized objects of this
					 * class will not be compatible with future Swing releases.
					 * The current serialization support is appropriate for
					 * short term storage or RMI between applications running
					 * the same version of Swing. As of 1.4, support for long
					 * term storage of all JavaBeans<sup><font
					 * size="-2">TM</font></sup> has been added to the
					 * <code>java.beans</code> package. Please see
					 * {@link java.beans.XMLEncoder}.
					 * 
					 * @see DefaultEditorKit#copyAction
					 * @see DefaultEditorKit#getActions
					 */

					new javax.swing.text.DefaultEditorKit.CopyAction()
							.actionPerformed(e);
				} catch (Exception eeer) {
					eeer.printStackTrace();
				}

			}
		});
		p.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					/**
					 * Pastes the contents of the system clipboard into the
					 * selected region, or before the caret if nothing is
					 * selected.
					 * <p>
					 * <strong>Warning:</strong> Serialized objects of this
					 * class will not be compatible with future Swing releases.
					 * The current serialization support is appropriate for
					 * short term storage or RMI between applications running
					 * the same version of Swing. As of 1.4, support for long
					 * term storage of all JavaBeans<sup><font
					 * size="-2">TM</font></sup> has been added to the
					 * <code>java.beans</code> package. Please see
					 * {@link java.beans.XMLEncoder}.
					 * 
					 * @see DefaultEditorKit#pasteAction
					 * @see DefaultEditorKit#getActions
					 */

					new javax.swing.text.DefaultEditorKit.PasteAction()
							.actionPerformed(e);
				} catch (Exception eeer) {
					eeer.printStackTrace();
				}
			}
		});
	}

	public void show(Component invoker, int x, int y) {
		super.show(invoker, x, y);
	}
}
