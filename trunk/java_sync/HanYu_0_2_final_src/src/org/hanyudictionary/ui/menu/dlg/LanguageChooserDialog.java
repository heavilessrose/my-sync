package org.hanyudictionary.ui.menu.dlg;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

import org.hanyudictionary.common.util.AppConfig;
import org.hanyudictionary.common.util.MenuResource;

public class LanguageChooserDialog extends JDialog implements ActionListener {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private JLabel languageLabel;
	private JComboBox languageList;
	private JButton submitButton;
	private JButton cancelButton;
	
	public LanguageChooserDialog(JFrame parentFrame){
		super(parentFrame,"Language Chooser",true);
		init(parentFrame);
	}
	
	/**
	 * Load font configuration for this component
	 * normally use for reload font configuration
	 */
	public void loadFonts(){
		int labelFontSize = new Integer(AppConfig.getInstance().getKey("fontsize.label")).intValue();
		Font labelFont = new Font(AppConfig.getInstance().getKey("font."+AppConfig.getInstance().getKey("lang")),Font.PLAIN,labelFontSize);
		int comboFontSize = new Integer(AppConfig.getInstance().getKey("fontsize.combo")).intValue();
		Font comboFont = new Font(AppConfig.getInstance().getKey("font."+AppConfig.getInstance().getKey("lang")),Font.PLAIN,comboFontSize);
		
		languageLabel.setFont(labelFont);
		languageList.setFont(comboFont);
		submitButton.setFont(labelFont);
		cancelButton.setFont(labelFont);
	}

	/**
	 * init language chooser dialog
	 * @param parentFrame
	 */
	private void init(JFrame parentFrame) {
		// TODO Auto-generated method stub
		
		this.setTitle(MenuResource.getKey("dialog.language.title"));
		
		Container container = this.getContentPane();
		JPanel panel = new JPanel();
		panel.setLayout(new FlowLayout());
		languageLabel = new JLabel(MenuResource.getKey("dialog.language.select"));
		panel.add(languageLabel);
		languageList = new JComboBox(loadAvailableLanguage());
		panel.add(languageList);
		AppConfig config = AppConfig.getInstance();
		String locale = config.getKey("lang");
		
		Language language =  new Language();
		language.setLocale(locale);
		language.setName(MenuResource.getKey("language."+locale));
		languageList.setSelectedItem(language);
		
		container.add(panel,BorderLayout.NORTH);
		
		JPanel panel1 = new JPanel();
		panel1.setLayout(new FlowLayout());
		submitButton = new JButton(MenuResource.getKey("dialog.button.ok"));
		panel1.add(submitButton);
		cancelButton = new JButton(MenuResource.getKey("dialog.button.cancel"));
		panel1.add(cancelButton);
		
		submitButton.addActionListener(this);
		cancelButton.addActionListener(this);
		
		container.add(panel1,BorderLayout.SOUTH);
		loadFonts();
		
		Point parentFramePoint = parentFrame.getLocation();
		double xPos = parentFramePoint.getX();
		double yPos = parentFramePoint.getY();
		this.setLocation(new Point((int)xPos+100,(int)yPos+100));		
		this.setResizable(false);
		this.pack();
	}
	
	/**
	 * load all available language
	 * @return
	 */
	private Language[] loadAvailableLanguage(){
		AppConfig config = AppConfig.getInstance();
		String avail = config.getKey("lang.support");
		String[] availArray = avail.split(",");
		Language[] languageList = new Language[availArray.length];
		for (int i=0;i<availArray.length;i++){
			Language lang = new Language();
			lang.setLocale(availArray[i]);
			lang.setName(MenuResource.getKey("language."+availArray[i]));
			languageList[i] = lang;
		}
		

		
		return languageList;
	}

	/**
	 * save language config to configuration file
	 *
	 */
	private void updateLanguageConfig() {
		// TODO Auto-generated method stub
		Language lang = (Language) languageList.getSelectedItem();
		AppConfig config = AppConfig.getInstance();
		config.setKey("lang",lang.getLocale());
		config.storeAppConf();
	}
	
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		if (submitButton == e.getSource()){
			updateLanguageConfig();
			this.dispose();
		}else {
			this.dispose();
		}
	}


}
