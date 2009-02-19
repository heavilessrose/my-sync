package org.hanyudictionary.ui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.util.Locale;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.JTabbedPane;
import javax.swing.UIManager;

import org.hanyudictionary.common.util.AppConfig;
import org.hanyudictionary.common.util.ComponentUtil;
import org.hanyudictionary.common.util.MenuResource;
import org.hanyudictionary.ui.menu.MainMenuBar;
import org.hanyudictionary.ui.search.panel.SearchIndexPanel;
import org.hanyudictionary.ui.search.panel.SearchPanel;
import org.hanyudictionary.ui.search.panel.SearchResultPanel;

/**
 * This is a main display class of HanYu. All initial setup will appear in this
 * class (such as config loading, language loading, etc..)
 * 
 * @author Ken
 * @version %I%,%G%
 */
public class MainFrame extends JFrame {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private JSplitPane splitPaneH;
	private SearchResultPanel searchResultPanel;
	private SearchIndexPanel searchIndexPanel;
	private SearchPanel searchPanel;
	private JTabbedPane indexTab;
	private MainMenuBar menuBar;

	/**
	 * load all configurations and resources on startup (will develope
	 * configuration menu later)
	 */
	MainFrame() {
		System.out.println("... HanYu Start ...");
		System.out.println("... Load application.conf ...");
		initConfig(); // load application configuration
		System.out.println("... Setting up language ...");
		initLanguage(); // load menu language
		// initDictionaryDefinition(); not use yet
		System.out.println("... Initial MainFrame ...");
		initUI(); // construct main ui
		setVisible(true);
	}

	/**
	 * Load font configuration for this component normally use to reload font
	 * config
	 */
	public void loadFonts() {
		initLanguage();
		setTitle(MenuResource.getKey("hanyu.main.title"));
		searchResultPanel.loadFonts();
		searchIndexPanel.loadFonts();
		searchPanel.loadFonts();
		menuBar.loadFonts();

	}

	/**
	 * Load application configuration (change config required restart program)
	 */
	private void initConfig() {
		try {
			AppConfig.getInstance().loadAppConf();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Load menu language from configuration locale
	 */
	private void initLanguage() {
		try {
			Locale
					.setDefault(new Locale(AppConfig.getInstance().getKey(
							"lang")));
			MenuResource.loadMenuResouce(new Locale(AppConfig.getInstance()
					.getKey("lang")));
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Construct main frame
	 * 
	 */
	private void initUI() {
		AppConfig config = AppConfig.getInstance();
		setTitle(MenuResource.getKey("hanyu.main.title"));

		setIconImage(new ImageIcon(config.getKey("icon")).getImage());
		setBackground(Color.WHITE);
		setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

		JPanel topPanel = new JPanel();
		topPanel.setLayout(new BorderLayout());
		getContentPane().add(topPanel);

		searchPanel = new SearchPanel(this); // Create SearchPanel
		searchIndexPanel = new SearchIndexPanel(this); // Create IndexListPanel
		searchResultPanel = new SearchResultPanel(this); // Create
		menuBar = new MainMenuBar(this);
		// SearchResultPanel

		constructIndexTab();
		constructSplitPane();

		topPanel.add(searchPanel, BorderLayout.NORTH);
		topPanel.add(splitPaneH, BorderLayout.CENTER);
		topPanel.setPreferredSize(new Dimension(500, 300));
		setJMenuBar(menuBar);
		this.pack();
	}

	/**
	 * Construct Frame template
	 * 
	 */
	private void constructSplitPane() {
		splitPaneH = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);
		splitPaneH.setLeftComponent(searchIndexPanel);
		splitPaneH.setRightComponent(searchResultPanel);
		splitPaneH.setOneTouchExpandable(true);
	}

	/**
	 * Construct index tab
	 * 
	 */
	private void constructIndexTab() {
		/* Load font configuration */
		int fontSize = new Integer(AppConfig.getInstance().getKey(
				"fontsize.label")).intValue();
		Font font = new Font(AppConfig.getInstance().getKey(
				"font." + AppConfig.getInstance().getKey("lang")), Font.PLAIN,
				fontSize);

		indexTab = new JTabbedPane();
		indexTab.setFont(font);
		indexTab.addTab(MenuResource.getKey("hanyu.indexTab.label.search"),
				searchIndexPanel);
	}

	public SearchIndexPanel getSearchIndexPanel() {
		return searchIndexPanel;
	}

	public SearchResultPanel getSearchResultPanel() {
		return searchResultPanel;
	}

	public SearchPanel getSearchPanel() {
		return searchPanel;
	}

	public static void main(String[] args) {
		// use cross platform look & feel
		try {
			UIManager.setLookAndFeel(UIManager
					.getCrossPlatformLookAndFeelClassName());
		} catch (Exception e1) {
			e1.printStackTrace();
		}
		// }
		MainFrame mainFrame = new MainFrame();
		ComponentUtil.setComponentBoundsToCenterScreen(mainFrame);
	}
}
