package org.hanyudictionary.ui.search.panel;

import java.awt.BorderLayout;
import java.awt.Font;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.text.BadLocationException;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyleContext;
import javax.swing.text.StyledDocument;

import org.hanyudictionary.common.util.AppConfig;
import org.hanyudictionary.common.util.MenuResource;
import org.hanyudictionary.common.util.WordUtil;
import org.hanyudictionary.common.vo.ChineseWord;
import org.hanyudictionary.common.vo.EnglishWord;
import org.hanyudictionary.common.vo.PYWord;
import org.hanyudictionary.ui.MainFrame;
/**
 * 
 * @author Ken
 *
 */
public class SearchResultPanel extends JPanel {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	// private MainFrame parentFrame;
	private JLabel indexLabel;

	private JTextPane searchDetail;
	
	private static final String STYLE_TRADITION = "tradition";
	private static final String STYLE_SIMPLIFIED = "simplified";
	private static final String STYLE_PINYIN = "pinyin";
	private static final String STYLE_ENGLISH = "english";
	private static final String STYLE_TRADITION_HEADER = "traditionheader";
	private static final String STYLE_SIMPLIFIED_HEADER = "simplifiedheader";
	private static final String STYLE_PINYIN_HEADER = "pinyinheader";
	private static final String STYLE_ENGLISH_HEADER = "englishheader";
	
	private String chineseSimplifiedFont;
	private String chineseTraditionFont;
	private String englishFont;
	private int headerFontSize;
	private int textFontSize;
	private Object word;
	public SearchResultPanel() {
		init();
	}

	public SearchResultPanel(MainFrame parentFrame) {
		// this.parentFrame = parentFrame;
		init();
	}
	
	/**
	 * Load font configuration for this component
	 * normally use for reload font configuration
	 */
	public void loadFonts() {
		AppConfig config = AppConfig.getInstance();
        textFontSize = new Integer(config.getKey("fontsize.text")).intValue();
        headerFontSize = new Integer(config.getKey("fontsize.text")).intValue()+4;	
		int fontSize = new Integer(AppConfig.getInstance().getKey("fontsize.label")).intValue();
		Font font = new Font(AppConfig.getInstance().getKey("font."+AppConfig.getInstance().getKey("lang")),Font.PLAIN,fontSize);
		
		indexLabel.setFont(font);
		indexLabel.setText(MenuResource.getKey("hanyu.searchresult.label.result"));
        chineseSimplifiedFont = config.getKey("font.zh.simplified");
        chineseTraditionFont = config.getKey("font.zh.tradition");
        englishFont = config.getKey("font.en");
        loadStyles();
		displayWord(word);
	}
	
	private void init() {
		indexLabel = new JLabel(MenuResource.getKey("hanyu.searchresult.label.result"));
		indexLabel.setHorizontalAlignment(JLabel.CENTER);
		
		searchDetail = new JTextPane();
		searchDetail.setEditable(false);
		this.setLayout(new BorderLayout());
		JScrollPane detailScrollList = new JScrollPane(searchDetail);
		
		this.add(indexLabel, BorderLayout.NORTH);
		this.add(detailScrollList, BorderLayout.CENTER);
		loadFonts();
		displayWord(word);
	}

	/**
	 * Display search result on textarea follow search type
	 * @param word
	 * @author Popcorn
	 */
	public void displayWord(Object word) {
		this.word = word;
		if (word instanceof EnglishWord) {
			generateECResult((EnglishWord) word);
		} else if (word instanceof ChineseWord) {
			generateCEResult((ChineseWord) word);
		} else if (word instanceof PYWord) {
			generatePYResult((PYWord) word);
		} else {
			searchDetail.setText("");
		}
		this.searchDetail.setCaretPosition(0);
	}
	

	/**
	 * Load textarea display styles
	 * @author Kendo
	 */
	public void loadStyles(){
		StyledDocument doc = searchDetail.getStyledDocument();
        Style def = StyleContext.getDefaultStyleContext().getStyle(StyleContext.DEFAULT_STYLE);
        
        Style regular = doc.addStyle("regular",def);
        
        Style style = doc.addStyle(STYLE_TRADITION,regular);
        StyleConstants.setFontSize(style,textFontSize);
        StyleConstants.setFontFamily(style,chineseTraditionFont); 
        
        style = doc.addStyle(STYLE_TRADITION_HEADER,regular);
        StyleConstants.setFontSize(style,headerFontSize);
        StyleConstants.setFontFamily(style,chineseTraditionFont); 
        StyleConstants.setBold(style,true);
        
        style = doc.addStyle(STYLE_SIMPLIFIED,regular);
        StyleConstants.setFontSize(style,textFontSize);
        StyleConstants.setFontFamily(style,chineseSimplifiedFont);
        style = doc.addStyle(STYLE_SIMPLIFIED_HEADER,regular);
        StyleConstants.setFontSize(style,headerFontSize);
        StyleConstants.setFontFamily(style,chineseSimplifiedFont);
        StyleConstants.setBold(style,true);
        
        style = doc.addStyle(STYLE_PINYIN,regular);
        StyleConstants.setFontSize(style,textFontSize);
        StyleConstants.setFontFamily(style,englishFont);
        style = doc.addStyle(STYLE_PINYIN_HEADER,regular);
        StyleConstants.setFontSize(style,textFontSize);
        StyleConstants.setFontFamily(style,englishFont);
        StyleConstants.setBold(style,true);
        
        style = doc.addStyle(STYLE_ENGLISH,regular);
        StyleConstants.setFontFamily(style,englishFont);
        style = doc.addStyle(STYLE_ENGLISH_HEADER,regular);
        StyleConstants.setFontSize(style,headerFontSize);
        StyleConstants.setFontFamily(style,englishFont);
        StyleConstants.setBold(style,true);
	}
	
	/**
	 * Generate Chinese-English search result
	 * @param word
	 */
	private void generateCEResult(ChineseWord word) {
		// TODO Auto-generated method stub
		searchDetail.setText("");
		String[] englishMeaning = word.getEnglishMeanings();
		
		StyledDocument doc = searchDetail.getStyledDocument();
        try {
			doc.insertString(doc.getLength(),"  "+word.getTradition(),doc.getStyle(STYLE_TRADITION_HEADER));
			doc.insertString(doc.getLength(),"  "+word.getSimplified(),doc.getStyle(STYLE_SIMPLIFIED_HEADER));
			doc.insertString(doc.getLength(),"  ["+WordUtil.toToneMarkPinyin(word.getPinyin())+"]\n",doc.getStyle(STYLE_PINYIN_HEADER));
			for (int i=0;i< englishMeaning.length;i++){
				doc.insertString(doc.getLength()," "+(i+1)+") "+englishMeaning[i]+"\n",doc.getStyle(STYLE_ENGLISH));
			}
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	/**
	 * Generate English-Chinese search result
	 * @param word
	 */
	private void generateECResult(EnglishWord word) {
		// TODO Auto-generated method stub
		searchDetail.setText("");
		ChineseWord[] chineseMeanings = word.getChineseMeaning();
		
		StyledDocument doc = searchDetail.getStyledDocument();
        try {
			doc.insertString(doc.getLength(),"  "+word.getWord()+"\n",doc.getStyle(STYLE_ENGLISH_HEADER));
			for (int i=0;i< chineseMeanings.length;i++){
				doc.insertString(doc.getLength()," "+(i+1)+") "+chineseMeanings[i].getTradition()+"  ",doc.getStyle(STYLE_TRADITION));
				doc.insertString(doc.getLength(),chineseMeanings[i].getSimplified()+"\n",doc.getStyle(STYLE_SIMPLIFIED));
				doc.insertString(doc.getLength(),"    ["+WordUtil.toToneMarkPinyin(chineseMeanings[i].getPinyin())+"]\n",doc.getStyle(STYLE_PINYIN));
			}
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/**
	 * Generate Pinyin search result
	 * @param word
	 */
	private void generatePYResult(PYWord word) {
		searchDetail.setText("");
		ChineseWord[] chineseMeanings = word.getChinese();
		StyledDocument doc = searchDetail.getStyledDocument();
        try {
			doc.insertString(doc.getLength(),"  "+WordUtil.toToneMarkPinyin(word.getPinyin())+"\n",doc.getStyle(STYLE_ENGLISH_HEADER));
			for (int i=0;i< chineseMeanings.length;i++){
				doc.insertString(doc.getLength()," "+(i+1)+") "+chineseMeanings[i].getTradition()+"  ",doc.getStyle(STYLE_TRADITION));
				doc.insertString(doc.getLength(),chineseMeanings[i].getSimplified()+"\n",doc.getStyle(STYLE_SIMPLIFIED));
				String[] englishMeaning = chineseMeanings[i].getEnglishMeanings();
				for (int j = 0; j < englishMeaning.length; j++) {					
					doc.insertString(doc.getLength(),"    " + englishMeaning[j]+"\n",doc.getStyle(STYLE_PINYIN));
				}
			}
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/**
	 * Display search not found
	 */
	public void displaySearchNotFound() {
		int fontSize = new Integer(AppConfig.getInstance().getKey("fontsize.label")).intValue();
		Font font = new Font(AppConfig.getInstance().getKey("font."+AppConfig.getInstance().getKey("lang")),Font.PLAIN,fontSize);
		searchDetail.setFont(font);
		searchDetail.setText(MenuResource.getKey("message.search.result.notfound"));
	}



}
