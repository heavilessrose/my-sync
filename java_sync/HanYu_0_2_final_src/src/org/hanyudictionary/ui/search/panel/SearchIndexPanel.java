package org.hanyudictionary.ui.search.panel;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Font;
import java.util.List;
import java.util.Vector;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import org.hanyudictionary.common.util.AppConfig;
import org.hanyudictionary.common.util.MenuResource;
import org.hanyudictionary.common.vo.ChineseWord;
import org.hanyudictionary.ui.MainFrame;
/**
 * This class contain information about search result.
 * It also handle the output of SearchResultPanel
 * @author Ken
 */
public class SearchIndexPanel extends JPanel implements ListSelectionListener {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private JFrame parentFrame; //Reference to parent frame. In this project mainframe act like parent frame
	private JList indexList;
	private JLabel indexLabel; //simple label
	private JLabel statusBar;
	
	private List resultList; // searchResultList
	public SearchIndexPanel(){
		init();
	}
	
	/**
	 * 
	 * @param parentFrame
	 */
	public SearchIndexPanel(JFrame parentFrame){
		this.parentFrame = parentFrame;
		init();
	}
	
	/**
	 * Load font configuration for this component
	 * normally use for reload font configuration
	 */
	public void loadFonts() {
		int fontSize = new Integer(AppConfig.getInstance().getKey("fontsize.label")).intValue();
		Font indexFont = new Font(AppConfig.getInstance().getKey("font."+AppConfig.getInstance().getKey("lang")),Font.PLAIN,fontSize);
		
		if (this.resultList != null)	
			updateList(resultList);
		indexLabel.setFont(indexFont);
		indexLabel.setText(MenuResource.getKey("hanyu.indexList.label.index"));
		statusBar.setFont(indexFont);
	}
	
	/**
	 * Initial panel configuration
	 * 
	 */
	private void init(){
		/*Setting up panel layout */
		this.setLayout( new BorderLayout() );
		this.setPreferredSize( new Dimension( 150, 100 ) );
		this.setMinimumSize( new Dimension( 100, 50 ));
	
		statusBar = new JLabel(" ");
		indexList = new JList();
		indexLabel = new JLabel(MenuResource.getKey("hanyu.indexList.label.index"));
		
        indexList.addListSelectionListener(this);	
        
        indexLabel.setHorizontalAlignment(JLabel.CENTER);
        
		JScrollPane indexScrollList = new JScrollPane(indexList);
		this.add( indexLabel , BorderLayout.NORTH);
		this.add( indexScrollList, BorderLayout.CENTER );
		this.add( statusBar , BorderLayout.SOUTH);
		loadFonts();
	}

	public JList getIndexList() {
		return indexList;
	}
	
	/**
	 * Handle onchange event on indexlist component
	 */
	public void valueChanged(ListSelectionEvent e) {
		// TODO Auto-generated method stub
		JList list = (JList) e.getSource();
		if (list.getSelectedValue() != null){
			if(parentFrame != null && parentFrame instanceof MainFrame) {
				MainFrame mainFrame = (MainFrame) parentFrame;
				SearchResultPanel resultPane = mainFrame.getSearchResultPanel();
				resultPane.displayWord(list.getSelectedValue());
			}
		}
	}
	
	/**
	 * update search result to indexlist
	 * @param resultList
	 */
	public void updateList(List resultList) {
		// TODO Auto-generated method stub
		this.resultList = resultList;
		Object o = resultList.get(0);
		String fontName =null;
		String fontSizeStr =null;
		/**
		 * check search type
		 */
		if (o instanceof ChineseWord){ // for Chinese-English search
			fontName = "font.zh";
			fontSizeStr = "fontsize.list";
		} else { // for English and pinyin search
			fontName = "font.en";
			fontSizeStr = "fontsize.list";
		} 
		
		/* set font following search type*/
		int fontSize = new Integer(AppConfig.getInstance().getKey(fontSizeStr)).intValue();
		Font font = new Font(AppConfig.getInstance().getKey(fontName),Font.PLAIN,fontSize);
		this.indexList.setFont(font);
		this.indexList.setListData(resultList.toArray());
		this.indexList.setSelectedIndex(0);
		this.statusBar.setText(MenuResource.getKey("message.search.found")+" "+resultList.size()+" "+MenuResource.getKey("message.search.record"));
	}

	/**
	 * Clear indexlist
	 */
	public void clearList() {
		// TODO Auto-generated method stub
		this.indexList.setListData(new Vector());
		this.statusBar.setText("");
	}
	
	public void displaySearchNotFound() {
		// TODO Auto-generated method stub
		this.indexList.setListData(new Vector());
		this.statusBar.setText(MenuResource.getKey("message.search.indexList.notfound"));
	}
}
