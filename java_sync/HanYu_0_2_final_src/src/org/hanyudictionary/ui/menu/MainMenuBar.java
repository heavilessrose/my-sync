package org.hanyudictionary.ui.menu;

import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;

import org.hanyudictionary.common.util.AppConfig;
import org.hanyudictionary.common.util.MenuResource;
import org.hanyudictionary.ui.MainFrame;
import org.hanyudictionary.ui.menu.dlg.AboutDialog;
import org.hanyudictionary.ui.menu.dlg.FontChooserDialog;
import org.hanyudictionary.ui.menu.dlg.LanguageChooserDialog;

public class MainMenuBar extends JMenuBar {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private MainFrame parentFrame;
	private JMenu helpMenu = new JMenu();
	private JMenu settingMenu = new JMenu();
	private JMenuItem aboutMenu = new JMenuItem();
	private JMenuItem fontMenu = new JMenuItem();
	private JMenuItem languageMenu = new JMenuItem();
	
	private Font font;
	public MainMenuBar(){
		init();
	}
	
	public MainMenuBar(MainFrame parentFrame){
		this.parentFrame = parentFrame;
		init();
	}
	
	/**
	 * Load font configuration for this component
	 * normally use for reload font configuration
	 */
	public void loadFonts(){
		int fontSize = new Integer(AppConfig.getInstance().getKey("fontsize.menu")).intValue();
		font = new Font(AppConfig.getInstance().getKey("font." + AppConfig.getInstance().getKey("lang")), Font.PLAIN,fontSize);
		
		aboutMenu.setFont(font);
		fontMenu.setFont(font);
		languageMenu.setFont(font);
		helpMenu.setFont(font);
		settingMenu.setFont(font);
		aboutMenu.setText(MenuResource.getKey("menu.help.about"));
		fontMenu.setText(MenuResource.getKey("menu.setting.font"));
		languageMenu.setText(MenuResource.getKey("menu.setting.language"));
		helpMenu.setText(MenuResource.getKey("menu.help"));
		settingMenu.setText(MenuResource.getKey("menu.setting"));
	}
	
	/**
	 * init menu bar
	 *
	 */
	private void init(){
		loadFonts();
		this.add(settingMenu);
		settingMenu.add(fontMenu);
		settingMenu.add(languageMenu);
		this.add(helpMenu);
		helpMenu.add(aboutMenu);
		
		fontMenu.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				if (event.getSource() == fontMenu){
					new FontChooserDialog(parentFrame).setVisible(true);
					parentFrame.loadFonts();
				} 	
			}
		});
		
		aboutMenu.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				if (event.getSource() == aboutMenu)
					new AboutDialog(parentFrame).setVisible(true);
			}
		});
		
		languageMenu.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent event) {
				if (event.getSource() == languageMenu){
					new LanguageChooserDialog(parentFrame).setVisible(true);
					parentFrame.loadFonts();
				}	
			}
			
		});
	}
}
