package org.hanyudictionary.ui.search.panel;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import org.hanyudictionary.common.util.AppConfig;
import org.hanyudictionary.common.util.MenuResource;
import org.hanyudictionary.translator.Translator;
import org.hanyudictionary.translator.TranslatorFactory;
import org.hanyudictionary.ui.MainFrame;

public class SearchPanel extends JPanel implements KeyListener, ActionListener{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private MainFrame parentFrame;
	private JComboBox searchComboBox;
	private JButton searchButton = new JButton();
	private JLabel searchInputLabel = new JLabel();
	
	public SearchPanel(MainFrame parentFrame){
		this.parentFrame = parentFrame;
		init();
	}
	
	/**
	 * Load font configuration for this component
	 * normally use for reload font configuration
	 */
	public void loadFonts(){
		int searchLabelFontSize = new Integer(AppConfig.getInstance().getKey("fontsize.label")).intValue();
		Font searchLabelFont = new Font(AppConfig.getInstance().getKey("font."+AppConfig.getInstance().getKey("lang")),Font.PLAIN,searchLabelFontSize);
		int searchComboFontSize = new Integer(AppConfig.getInstance().getKey("fontsize.combo")).intValue();
		Font searchComboFont = new Font(AppConfig.getInstance().getKey("font.zh"),Font.PLAIN,searchComboFontSize);
		
		searchButton.setFont(searchLabelFont);
		searchButton.setText(MenuResource.getKey("hanyu.search.button.search"));
		searchInputLabel.setFont(searchLabelFont);
		searchInputLabel.setText(MenuResource.getKey("hanyu.search.label.input"));
		searchComboBox.setFont(searchComboFont);
	}
	
	/**
	 * initial search panel setup
	 */
	private void init(){
		Dimension minimumSize = new Dimension(0, 0);
		this.setLayout(new BorderLayout());
		searchComboBox = new JComboBox();
		loadFonts();
		
		searchInputLabel.setPreferredSize(new Dimension(60,20));
		searchInputLabel.setHorizontalAlignment(JLabel.CENTER);
		
		searchComboBox.setEditable(true);
		searchComboBox.setMinimumSize(minimumSize);
		
		searchComboBox.getEditor().getEditorComponent().addKeyListener(this);
		searchComboBox.getEditor().getEditorComponent().setFocusable(true);
		
		this.add(searchInputLabel,BorderLayout.WEST);
		this.add(searchComboBox,BorderLayout.CENTER);
		this.add(searchButton,BorderLayout.EAST);
		
		searchButton.addActionListener(this);
		searchComboBox.addActionListener(this);
		
	}
	
/*	I do not agree that searchCombobox should have any side-effect outside this class.
 *  SearchPanel should have full responsibility to this instance, isn't it?
 *  
 *  - Jua, 4:29 Jan 20 2006 -
 *  
	public JComboBox getSearchComboBox() {
		return searchComboBox;
	}
	
	public void setSearchComboBox(JComboBox searchComboBox) {
		this.searchComboBox = searchComboBox;
	}
 */
	
	public void keyPressed(KeyEvent event) {
		if (this.parentFrame != null){
			char inputChar  = event.getKeyChar();
			String searchWord = "";
			if (inputChar=='\n'){
				searchWord = (String) this.searchComboBox.getEditor().getItem();
			}
			if (searchWord!= null && !searchWord.trim().equals("")){
				search(searchWord.trim());
				SearchPanel searchPane = parentFrame.getSearchPanel();
				searchPane.addNewSearchKey(searchWord.toString());
			} 
		}
	}
	
	public void keyReleased(KeyEvent event) {
		if (this.parentFrame != null){
			String searchWord = (String) this.searchComboBox.getEditor().getItem();
			if (searchWord!= null && !searchWord.trim().equals("")){
				search(searchWord.trim());
			} else {
				parentFrame.getSearchIndexPanel().clearList();
				parentFrame.getSearchResultPanel().displayWord(null);
			}
		}
	}
	
	public void keyTyped(KeyEvent event) {
		// do nothing
	}
	
	/**
	 * This method is using to add new search key to searchComboBox. If 
	 * searchCombobox already contains searchKey then no need to add new one, just only
	 * setSelectedIndex to searchKey is enough.
	 * 
	 * @param searchKey
	 * @author Jua
	 */
	public void addNewSearchKey(String searchKey) {
		boolean containsSearchKey = false;
		int i;
		for(i=0; i<searchComboBox.getItemCount(); i++) {
			Object element = searchComboBox.getItemAt(i);
			if(element.equals(searchKey)) {
				containsSearchKey = true;
				break;
			}
		}
		if(containsSearchKey) {
			searchComboBox.setSelectedIndex(i);
		} else {
			searchComboBox.insertItemAt(searchKey, 0);
			searchComboBox.setSelectedIndex(0);
		}
	}
	
	
	/**
	 * This method contains logical algorithm for searching word
	 * @param searchWord
	 */
	private void search(String searchWord) {
		System.out.println("  -- Do search \""+searchWord+"\" --");
		Translator translator = TranslatorFactory.getInstance(AppConfig.getInstance().getKey("translator.ce"));
		if (searchWord != null && !searchWord.trim().equals("")){
			final List resultList = translator.directSearch(searchWord);

			if (resultList != null  && resultList.size()>0){
				SwingUtilities.invokeLater(new Runnable(){
					public void run() {
						parentFrame.getSearchIndexPanel().updateList(resultList);
					}
				});
				System.out.println("    -- Found "+resultList.size()+" record(s) --");
			} else {
				this.parentFrame.getSearchResultPanel().displaySearchNotFound();
				new Thread(){
					public void run(){
						parentFrame.getSearchIndexPanel().displaySearchNotFound();
					}
				}.start();
				System.out.println("    -- Search not found --");
			}
		} 
	}
	
	/**
	 * handle search button and search combo box event
	 */
	public void actionPerformed(ActionEvent e) {
		Object source = e.getSource();
		Object value = searchComboBox.getSelectedItem();
		if(value != null) {
			String searchWord = value.toString();
			if(source.equals(searchButton)) { // search button event
				search(searchWord);
			} else if(source.equals(searchComboBox)) { // search combo box event (not use yet)
				//search(searchWord);
			}
		}
	}
}
