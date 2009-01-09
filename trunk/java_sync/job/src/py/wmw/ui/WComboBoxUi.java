package py.wmw.ui;

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
import javax.swing.*;
import javax.swing.plaf.*;
import javax.swing.border.*;
import javax.swing.plaf.basic.*;
import java.io.Serializable;
import java.beans.*;

import javax.swing.plaf.metal.MetalComboBoxUI;

public class WComboBoxUi extends BasicComboBoxUI {
	public WComboBoxUi() {
		super();
	}

	JComboBox.KeySelectionManager keySelectionManager;

	public static ComponentUI createUI(JComponent c) {
		return new WComboBoxUi();
	}

	public void installUI(JComponent c) {

		super.installUI(c);
		if (keySelectionManager == null) {
			keySelectionManager = new WComBoxKeySelectManager(comboBox);
		}
		comboBox.setKeySelectionManager(keySelectionManager);

	}

	protected ComboPopup createPopup() {
		return new WComboPopup(comboBox);
	}

	public class MetalComboPopup extends WComboPopup {
		public MetalComboPopup(JComboBox cBox) {
			super(cBox);
		}
	}

	public static void main(String[] args) {
		WComboBoxUi WComboBoxUi1 = new WComboBoxUi();
	}

	protected ComboBoxEditor createEditor() {
		return new WComboBoxEditor.UIResource(comboBox);
	}

}
