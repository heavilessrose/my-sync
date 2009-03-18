/*
 *	Yes/No Dialog Box
 *		1997/10/21 (C) Copyright T.Kazawa(Digitune)
 */

package JP.digitune.gui;

import java.awt.Frame;
import java.awt.Dialog;
import java.awt.Label;
import java.awt.Button;
import java.awt.Panel;
import java.awt.FlowLayout;
import java.awt.BorderLayout;
import java.awt.event.WindowEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ResourceBundle;

public class YesNoBox extends MsgBox {
	private Label label1;
	private Button yes, no;
	private boolean state = false;

	public YesNoBox(Frame parent, String title, String msg) {
		super(parent, title, msg, true);
	}

	protected void createButton() {
		yes = new Button(rc.getString("yesLabel"));
		no = new Button(rc.getString("noLabel"));
		yes.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				state = true;
				dispose();
			}
		});
		no.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				state = false;
				dispose();
			}
		});
		regbutton.add(yes, "Center");
		regbutton.add(no, "Center");
	}

	public synchronized boolean getState() {
		return state;
	}
}
