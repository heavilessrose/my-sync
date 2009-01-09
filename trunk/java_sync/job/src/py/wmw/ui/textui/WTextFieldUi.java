/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: 深圳尘日软件工作室 </p>
 * @author  wangsong
 * @version 1.1
 */

package py.wmw.ui.textui;

import java.awt.*;
import java.beans.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.plaf.*;
import javax.swing.plaf.basic.*;

public class WTextFieldUi extends BasicTextFieldUI {
	public static class wmBasicCaret extends wCarer implements UIResource {
	}

	public static ComponentUI createUI(JComponent c) {
		return new WTextFieldUi();
	}

	protected Caret createCaret() {
		wmBasicCaret wmBasicCaret = new wmBasicCaret();
		return wmBasicCaret;
	}

	/**
	 * This method gets called when a bound property is changed on the
	 * associated JTextComponent. This is a hook which UI implementations may
	 * change to reflect how the UI displays bound properties of JTextComponent
	 * subclasses.
	 * 
	 * @param evt
	 *            the property change event
	 */
	public void propertyChange(PropertyChangeEvent evt) {
		super.propertyChange(evt);
	}

}
