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

/**
 * 
 * Provides the look and feel for a plain text editor. In this implementation
 * the default UI is extended to act as a simple view factory.
 * <p>
 * <strong>Warning:</strong> Serialized objects of this class will not be
 * compatible with future Swing releases. The current serialization support is
 * appropriate for short term storage or RMI between applications running the
 * same version of Swing. As of 1.4, support for long term storage
 * 
 * @version 2005.5.1
 */

public class WTextAreaUi extends BasicTextAreaUI {
	public WTextAreaUi() {
	}

	public static class wmBasicCaret extends wCarer implements UIResource {
	}

	public static ComponentUI createUI(JComponent c) {
		return new WTextAreaUi();
	}

	protected Caret createCaret() {
		wmBasicCaret wmBasicCaret = new wmBasicCaret();
		return wmBasicCaret;
	}

}
