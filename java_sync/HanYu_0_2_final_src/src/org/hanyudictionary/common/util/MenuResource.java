package org.hanyudictionary.common.util;

import java.util.Locale;
import java.util.ResourceBundle;

/**
 * MenuResource is a class that contain all menu message. 
 * It also internatinalize menu content following specify locale.
 * @author Ken
 * @version %I%,%G%
 */
public class MenuResource {
	
	private static MenuResource instance;
	private static final String DEFAULT_LOCALE = "en";
	private static final String MSG_RESOURCE = "MenuResource";
	private static ResourceBundle menuResource;
	
	private MenuResource(){
		
	}
	
	public static MenuResource getInstance(){
		if (instance == null){
			instance = new MenuResource();
		}
		return instance;
	}
	/**
	 * Load language definition from specify locale
	 * if the specify locale not correct, default locale will be loaded instead
	 */
	public static void loadMenuResouce(Locale locale){
		try{
			menuResource = ResourceBundle.getBundle(MSG_RESOURCE,locale);
		} catch(Exception e){
			menuResource = ResourceBundle.getBundle(MSG_RESOURCE,new Locale(DEFAULT_LOCALE));
		}
	}
	
	/**
	 * Get menu content value from input
	 * @param key Menu content's key
	 * @return value of menu content's key
	 */
	public static String getKey(String key){
		return menuResource.getString(key);
	}
}
