package org.hanyudictionary.ui.menu.dlg;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridLayout;
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
import org.hanyudictionary.common.util.FontUtil;
import org.hanyudictionary.common.util.MenuResource;

public class FontChooserDialog extends JDialog implements ActionListener{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private JComboBox chineseSimplifiedFontList;
	private JComboBox chineseTraditionFontList;
	private JComboBox englishFontList;
	private JComboBox indexFontSizeList;
	private JComboBox resultFontSizeList;
	private JLabel indexFontLabel;
	private JLabel resultFontLabel;
	private JLabel englishFontLabel;
	private JLabel chineseSimplifiedFontLabel;
	private JLabel chineseTraditionFontLabel;
	private String selectedChineseSimplifiedFont;
	private String selectedChineseTraditionFont;
	private String selectedEnglishFont;
	private String selectedIndexFontSize;
	private String selectedResultFontSize;
	private JButton submitButton;
	private JButton cancelButton;
	
	public FontChooserDialog(JFrame parentFrame){
	    super(parentFrame, "Font Chooser", true);
	    init(parentFrame);
	    loadFontConfig();
	}

	/**
	 * Load font configuration for this component
	 * normally use for reload font configuration
	 */
	public void loadFonts(){
		int labelFontSize = new Integer(AppConfig.getInstance().getKey("fontsize.label")).intValue();
		Font labelFont = new Font(AppConfig.getInstance().getKey("font."+AppConfig.getInstance().getKey("lang")),Font.PLAIN,labelFontSize);
		//int comboFontSize = new Integer(AppConfig.getInstance().getKey("fontsize.combo")).intValue();
		//Font comboFont = new Font(AppConfig.getInstance().getKey("font.default"),Font.PLAIN,comboFontSize);
		
		//chineseSimplifiedFontList.setFont(comboFont);
		//chineseTraditionFontList.setFont(comboFont);
		//englishFontList.setFont(comboFont);
		//indexFontSizeList.setFont(comboFont);
		//resultFontSizeList.setFont(comboFont);
		
		indexFontLabel.setFont(labelFont);
		resultFontLabel.setFont(labelFont);
		englishFontLabel.setFont(labelFont);
		chineseSimplifiedFontLabel.setFont(labelFont);
		chineseTraditionFontLabel.setFont(labelFont);
		submitButton.setFont(labelFont);
		cancelButton.setFont(labelFont);
	}
	
	/**
	 * init font chooser dialog
	 * @param parentFrame
	 */
	private void init(JFrame parentFrame){
		Container container = getContentPane();
		String[] sizeList = {"12","14","16","18","20","24","32"};
		
		this.setTitle(MenuResource.getKey("dialog.font.title"));
		
		JPanel pane = new JPanel();
		pane.setLayout(new FlowLayout());
		
		JPanel panel = new JPanel();
		panel.setLayout(new GridLayout(5,2));
		
		chineseSimplifiedFontLabel = new JLabel(MenuResource.getKey("dialog.font.simplified"));
		chineseSimplifiedFontList = new JComboBox(FontUtil.getFontList());
		panel.add(chineseSimplifiedFontLabel);
		panel.add(chineseSimplifiedFontList);
		
		chineseTraditionFontLabel = new JLabel(MenuResource.getKey("dialog.font.tradition"));
		chineseTraditionFontList = new JComboBox(FontUtil.getFontList());
		panel.add(chineseTraditionFontLabel);
		panel.add(chineseTraditionFontList);
		
		englishFontLabel = new JLabel(MenuResource.getKey("dialog.font.english"));
		englishFontList = new JComboBox(FontUtil.getFontList());
		panel.add(englishFontLabel);
		panel.add(englishFontList);
		
		indexFontLabel = new JLabel(MenuResource.getKey("dialog.font.indexfontsize"));
		indexFontSizeList = new JComboBox(sizeList);
		panel.add(indexFontLabel);
		panel.add(indexFontSizeList);
		
		resultFontLabel = new JLabel(MenuResource.getKey("dialog.font.resultfontsize"));
		resultFontSizeList = new JComboBox(sizeList);
		panel.add(resultFontLabel);
		panel.add(resultFontSizeList);
		panel.setPreferredSize(new Dimension(280,100));
		
		pane.add(panel);
		container.add(pane,BorderLayout.NORTH);
		
		JPanel panel1 = new JPanel();
		panel1.setLayout(new FlowLayout());
		submitButton = new JButton (MenuResource.getKey("dialog.button.ok"));
		cancelButton = new JButton (MenuResource.getKey("dialog.button.cancel"));
		submitButton.addActionListener(this);
		cancelButton.addActionListener(this);
		panel1.add(submitButton);
		panel1.add(cancelButton);
		container.add(panel1,BorderLayout.SOUTH);
		this.setResizable(false);
		this.pack();
		Point parentFramePoint = parentFrame.getLocation();
		double xPos = parentFramePoint.getX();
		double yPos = parentFramePoint.getY();
		loadFonts();
		this.setLocation(new Point((int)xPos+100,(int)yPos+100));		
	}

	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		if (submitButton == e.getSource()){
			updateFontConfig();
			this.dispose();
		}else {
			this.dispose();
		}
	}
	
	/**
	 * Load font configuration to fill data in dialog
	 * not the same as loadFonts()
	 */
	private void loadFontConfig() {
		// TODO Auto-generated method stub
		AppConfig config = AppConfig.getInstance();
		selectedChineseSimplifiedFont = config.getKey("font.zh.simplified");
		selectedChineseTraditionFont = config.getKey("font.zh.tradition");
		selectedEnglishFont = config.getKey("font.en");
		selectedIndexFontSize = config.getKey("fontsize.list");
		selectedResultFontSize = config.getKey("fontsize.text");
		
		chineseSimplifiedFontList.setSelectedItem(selectedChineseSimplifiedFont);
		chineseTraditionFontList.setSelectedItem(selectedChineseTraditionFont);
		englishFontList.setSelectedItem(selectedEnglishFont);
		indexFontSizeList.setSelectedItem(selectedIndexFontSize);
		resultFontSizeList.setSelectedItem(selectedResultFontSize);
	}
	
	/**
	 * Save font configuration to config file
	 *
	 */
	private void updateFontConfig(){
		selectedChineseSimplifiedFont = (String) chineseSimplifiedFontList.getSelectedItem();
		selectedChineseTraditionFont = (String) chineseTraditionFontList.getSelectedItem();
		selectedEnglishFont = (String) englishFontList.getSelectedItem();
		selectedIndexFontSize = (String) indexFontSizeList.getSelectedItem();
		selectedResultFontSize = (String) resultFontSizeList.getSelectedItem();
		
		AppConfig config = AppConfig.getInstance();
		config.setKey("font.zh",selectedChineseSimplifiedFont);
		config.setKey("font.zh.simplified",selectedChineseSimplifiedFont);
		config.setKey("font.zh.tradition",selectedChineseTraditionFont);
		config.setKey("font.en",selectedEnglishFont);
		config.setKey("fontsize.list",selectedIndexFontSize);
		config.setKey("fontsize.text",selectedResultFontSize);
		config.storeAppConf();

	}
}
