/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: 深圳尘日软件工作室 </p>
 * @author  wangsong
 * @version 1.1
 */

package py.wmw.ui.textui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.beans.*;
import javax.swing.text.*;
import javax.swing.plaf.*;
import javax.swing.event.*;
import javax.accessibility.*;
import java.io.ObjectOutputStream;
import java.io.ObjectInputStream;
import java.io.IOException;
import java.io.Serializable;

public class WJTextField extends javax.swing.JTextField {
	private BoundedRangeModel visibility;
	private int columns;
	public WJTextField wTextField = this;

	public WJTextField(Document doc, String text, int columns) {
		if (columns < 0) {
			throw new IllegalArgumentException("columns less than zero.");
		}
		visibility = new DefaultBoundedRangeModel();
		visibility.addChangeListener(new ScrollRepainter());
		this.columns = columns;
		if (doc == null) {
			doc = createDefaultModel();
		}
		setDocument(doc);
		if (text != null) {
			setText(text);
		}

	}

	public WJTextField() {
		this(null, null, 0);
	}

	/**
	 * Constructs a new <code>TextField</code> initialized with the specified
	 * text. A default model is created and the number of columns is 0.
	 * 
	 * @param text
	 *            the text to be displayed, or <code>null</code>
	 */
	public WJTextField(String text) {
		this(null, text, 0);
	}

	/**
	 * Constructs a new empty <code>TextField</code> with the specified number
	 * of columns. A default model is created and the initial string is set to
	 * <code>null</code>.
	 * 
	 * @param columns
	 *            the number of columns to use to calculate the preferred width;
	 *            if columns is set to zero, the preferred width will be
	 *            whatever naturally results from the component implementation
	 */
	public WJTextField(int columns) {
		this(null, null, columns);
	}

	public void setDocument(Document doc) {
		if (doc != null) {
			doc.putProperty("filterNewlines", Boolean.TRUE);
		}
		super.setDocument(doc);

		this.addMouseListener(new MouseAdapter() {
			public void mousePressed(MouseEvent mouseevent) {
				WTextPopMenu popupMenu = new WTextPopMenu(wTextField);
				if (mouseevent.isMetaDown()) {
					Dimension screenSize = Toolkit.getDefaultToolkit()
							.getScreenSize();
					int wdt = mouseevent.getX();
					int hgt = mouseevent.getY();
					if (wdt + wTextField.getLocationOnScreen().x
							+ popupMenu.getPreferredSize().width > screenSize.width) {
						wdt = wdt - popupMenu.getPreferredSize().height;
					}
					if (hgt + wTextField.getLocationOnScreen().y
							+ popupMenu.getPreferredSize().height + 20 > screenSize.height) {
						hgt = hgt - popupMenu.getPreferredSize().height;
					}
					popupMenu.show(wTextField, wdt, hgt);

				}
			}
		});

	}

	/**
	 * Constructs a new <code>TextField</code> initialized with the specified
	 * text and columns. A default model is created.
	 * 
	 * @param text
	 *            the text to be displayed, or <code>null</code>
	 * @param columns
	 *            the number of columns to use to calculate the preferred width;
	 *            if columns is set to zero, the preferred width will be
	 *            whatever naturally results from the component implementation
	 */
	public WJTextField(String text, int columns) {
		this(null, text, columns);
	}

	class ScrollRepainter implements ChangeListener, Serializable {

		public void stateChanged(ChangeEvent e) {
			repaint();
		}

	}

}
