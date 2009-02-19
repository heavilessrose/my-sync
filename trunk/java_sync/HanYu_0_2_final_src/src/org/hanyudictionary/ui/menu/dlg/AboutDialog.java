package org.hanyudictionary.ui.menu.dlg;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextField;

import org.hanyudictionary.common.util.AppConfig;
import org.hanyudictionary.common.util.MenuResource;

public class AboutDialog extends JDialog {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private JTextField aboutTextField = new JTextField();
	private JButton submitButton = new JButton();
	
	public AboutDialog(JFrame parentFrame){
		super(parentFrame,"About HanYu",true);
		init(parentFrame);
	}

	public void loadFonts(){

		Font font = new Font(AppConfig.getInstance().getKey("font."+AppConfig.getInstance().getKey("lang")),Font.PLAIN,14);
		
		try {
			FileReader reader = new FileReader("about.txt");
			aboutTextField.read(reader,null);
			reader.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		aboutTextField.setFont(font);
		aboutTextField.setBackground(Color.WHITE);
		aboutTextField.setEditable(false);
		submitButton.setFont(font);
		submitButton.setText(MenuResource.getKey("dialog.button.ok"));	
	}
	
	private void init(JFrame parentFrame) {
		// TODO Auto-generated method stub
		setTitle(MenuResource.getKey("dialog.abouthanyu.title"));
		Container container = this.getContentPane();
		JPanel panel = new JPanel();
		panel.setLayout(new FlowLayout());
		panel.add(aboutTextField);
		aboutTextField.setPreferredSize(new Dimension(300,100));
		container.add(panel,BorderLayout.NORTH);
		
		JPanel panel1 = new JPanel();
		panel1.setLayout(new FlowLayout());
	
		panel1.add(submitButton);
			
		loadFonts();
		Point parentFramePoint = parentFrame.getLocation();
		double xPos = parentFramePoint.getX();
		double yPos = parentFramePoint.getY();
		this.setLocation(new Point((int)xPos+100,(int)yPos+100));		
		container.add(panel1, BorderLayout.SOUTH);
		
		this.setResizable(false);
		this.pack();
		
		submitButton.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent event) {
				// TODO Auto-generated method stub
				if (submitButton == event.getSource())
					dispose();
			}
		});
		
		
	}
}
