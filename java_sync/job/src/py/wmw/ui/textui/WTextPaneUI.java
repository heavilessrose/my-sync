package py.wmw.ui.textui;

import java.awt.*;
import java.beans.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.plaf.*;
import javax.swing.plaf.basic.*;

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
public class WTextPaneUI extends BasicTextPaneUI {
	public WTextPaneUI() {
	}

	public static class wmBasicCaret extends wCarer implements UIResource {
	}

	public static ComponentUI createUI(JComponent c) {
		return new WTextPaneUI();
	}

	protected Caret createCaret() {
		wmBasicCaret wmBasicCaret = new wmBasicCaret();
		return wmBasicCaret;
	}

}
