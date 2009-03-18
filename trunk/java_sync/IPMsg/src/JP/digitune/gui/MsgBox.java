/*
 *	Message Box Class
 *		1997/10/21 (C) Copyright T.Kazawa(Digitune)
 */

package JP.digitune.gui;

import java.awt.Frame;
import java.awt.Dialog;
import java.awt.Label;
import java.awt.Button;
import java.awt.Panel;
import java.awt.Point;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.BorderLayout;
import java.awt.event.WindowEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.StringTokenizer;
import java.util.ResourceBundle;

public class MsgBox extends Dialog {
	protected ResourceBundle rc
		= ResourceBundle.getBundle("JP.digitune.gui.resources");
	protected Panel regbutton = new Panel(new FlowLayout());
	private Button ok;

	protected void createBody(String msg) {
		StringTokenizer tokenizer = new StringTokenizer(msg, "\n");
		Label[] labels = new Label[tokenizer.countTokens()];
		Panel p1 = new Panel(new GridLayout(0, 1, 0, 0));
		for (int i = 0; tokenizer.hasMoreTokens(); i++) {
			labels[i] = new Label(tokenizer.nextToken(), Label.CENTER);
			p1.add(labels[i]);
		}
		add(p1, "Center");
	}

	protected void createButton() {
		ok = new Button(rc.getString("okLabel"));
		ok.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				dispose();
			}
		});
		regbutton.add(ok, "Center");
	}

	public MsgBox(Frame parent, String title, String msg, boolean b) {
		super(parent, title, b);
		setLayout(new BorderLayout());
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent we) {
				dispose();
			}
		});
		createBody(msg);
		createButton();
		add(regbutton, "South");
		pack();
		Dimension scsize = getToolkit().getScreenSize();
		Dimension size = getSize();
		setLocation(scsize.width / 2 - size.width / 2
			, scsize.height / 2 - size.height / 2);
		setVisible(true);
	}
}
