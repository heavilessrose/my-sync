package py.wmw.ui;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: 深圳尘日软件工作室 </p>
 * @author  wangsong
 * @version 1.1
 */
import javax.swing.*;

public class UItool implements UItoolinterface {
	public UItool() {
	}

	public static void main(String[] args) {
		UItool UItool = new UItool();
	}

	public void SetUI() {
		try {
			// LookAndFeel alloyLnF = new
			// javax.swing.plaf.metal.MetalLookAndFeel();
			// LookAndFeel alloyLnF = new
			// soft.wes.feels.BedouinThemeAlloyLookAndFeel();
			// javax.swing.UIManager.setLookAndFeel(alloyLnF);
			UIManager.put("TextFieldUI", "wmw.ui.textui.WTextFieldUi");
			UIManager.put("TextAreaUI", "wmw.ui.textui.WTextAreaUi");// 实现又键功能
			UIManager.put("ComboBoxUI", "wmw.ui.WComboBoxUi");// 实现定位
		} catch (Exception eer) {
		}

	}

	public String ObjectToString(java.lang.Object elem) {
		if (elem instanceof java.lang.String[]) {
			return ((String[]) elem)[0].toString().toLowerCase();
		}
		return elem.toString().toLowerCase();
	}

}
