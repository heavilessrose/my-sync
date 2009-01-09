/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: 深圳尘日软件工作室 </p>
 * @author  wangsong
 * @version 1.1
 */

package py.wmw.ui;

import javax.swing.*;
import javax.swing.plaf.metal.*;
import javax.swing.border.*;
import java.io.Serializable;
import java.awt.*;
import java.awt.event.*;
import javax.swing.plaf.basic.BasicComboBoxEditor;

/**
 * The default editor for WComboBoxUi editable combo boxes
 * <p>
 * <strong>Warning:</strong> Serialized objects of this class will not be
 * compatible with future Swing releases. The current serialization support is
 * appropriate for short term storage or RMI between applications running the
 * same version of Swing. As of 1.4, support for long term storage of all
 * JavaBeans<sup><font size="-2">TM</font></sup> has been added to the
 * <code>java.beans</code> package. Please see {@link java.beans.XMLEncoder}.
 * 
 * @version 1.1
 * @author cpu--@163.com
 */
public class WComboBoxEditor extends BasicComboBoxEditor {
	protected KeyListener keyListener;
	final JComboBox comboBox;
	WComBoxKeySelectManager keymanager;

	public WComboBoxEditor(final JComboBox comboBox) {
		super();
		this.comboBox = comboBox;
		this.keymanager = new WComBoxKeySelectManager(this.comboBox);
		editor = new JTextField("", 9) {
			// workaround for 4530952
			public void setText(String s) {
				if (getText().equals(s)) {
					return;
				}
				super.setText(s);
			}
		};
		// JTextField editorttt = new JTextField("", 9);
		// editor.setBorder(new EditorBorder());
		editor.addKeyListener(new KeyAdapter() {
			public void keyPressed(KeyEvent e) {
				if (comboBox.getModel().getSize() == 0) {
					return;
				}
				if (e.isShiftDown() && e.getKeyCode() == e.VK_SPACE) {
					editor.setText(comboBox.getModel().getSelectedItem()
							.toString());
					comboBox.setPopupVisible(false);
				} else if (!e.isActionKey()) {
					String h = editor.getText();
					// System.out.println(editor.getText());
					int f = editor.getSelectionStart();
					int d = editor.getSelectionEnd();
					// keymanager.selectionForKey7();
					keymanager.selectionForKey(e.getKeyChar(), comboBox
							.getModel());
					// if ( e.isShiftDown() && e.getKeyCode() == e.VK_SPACE) {
					// editor.setText(comboBox.getModel().getSelectedItem().toString());
					// }
					// else { }
					editor.setText(h);
					editor.setSelectionStart(f);
					editor.setSelectionEnd(d);

				}

			}

			public void keyReleased(KeyEvent e) {
				// String h = editor.getText();
				// System.out.println(editor.getText());
				// int f = editor.getSelectionStart();
				// int d = editor.getSelectionEnd();
				// // keymanager.selectionForKey7();
				// keymanager.selectionForKey(e.getKeyChar(),
				// comboBox.getModel());
				// if ( e.isShiftDown() && e.getKeyCode() == e.VK_SPACE) {
				// editor.setText(comboBox.getModel().getSelectedItem().toString());
				// }
				// else { }
				// editor.setText(h);
				// editor.setSelectionStart(f);
				// editor.setSelectionEnd(d);

			}
		});

	}

	protected static Insets editorBorderInsets = new Insets(2, 2, 2, 0);

	class EditorBorder extends AbstractBorder {
		public void paintBorder(Component c, Graphics g, int x, int y, int w,
				int h) {
			g.translate(x, y);
			g.setColor(MetalLookAndFeel.getControlDarkShadow());
			g.drawLine(0, 0, w - 1, 0);
			g.drawLine(0, 0, 0, h - 2);
			g.drawLine(0, h - 2, w - 1, h - 2);
			g.setColor(MetalLookAndFeel.getControlHighlight());
			g.drawLine(1, 1, w - 1, 1);
			g.drawLine(1, 1, 1, h - 1);
			g.drawLine(1, h - 1, w - 1, h - 1);
			g.setColor(MetalLookAndFeel.getControl());
			g.drawLine(1, h - 2, 1, h - 2);

			g.translate(-x, -y);
		}

		public Insets getBorderInsets(Component c) {
			return editorBorderInsets;
		}
	}

	public static class UIResource extends WComboBoxEditor implements
			javax.swing.plaf.UIResource {
		public UIResource(JComboBox comboBox) {
			super(comboBox);
		}

	}

}
