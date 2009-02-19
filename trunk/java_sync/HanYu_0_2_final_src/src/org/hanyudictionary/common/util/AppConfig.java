package org.hanyudictionary.common.util;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Properties;

/**
 * AppConfig is a class that contain all application configuration value
 * @author Ken
 * @version %I%,%G%
 */
public class AppConfig {
	private static AppConfig instance;
	private static final String APP_CONFIG_PATH = "config/application.conf";
	private Properties appConfig;
	public String locale;
	
	private AppConfig(){
		
	}
	
	public static AppConfig getInstance(){
		if (instance == null){
				instance = new AppConfig();
			return instance;
		}
		return instance;
	}
	
	/**
	 * Load application configuration from application.conf
	 * @throws FileNotFoundException
	 * @throws IOException
	 */
	public void loadAppConf() throws FileNotFoundException, IOException{
		appConfig = new Properties();
		appConfig.load(new FileInputStream(APP_CONFIG_PATH));
		locale = appConfig.getProperty("lang");
		
	}
	
	public void storeAppConf(){
		if (appConfig != null){
			try {
				appConfig.store(new FileOutputStream(APP_CONFIG_PATH),null);
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * Get config value from input
	 * @param key Config's key
	 * @return value of config's key
	 */
	public String getKey(String key){
		return appConfig.getProperty(key);
	}
	
	/**
	 * Set config
	 * @param key  Config's key to set
	 * @param value of config's key to set
	 */
	public void setKey(String key,String value){
		appConfig.setProperty(key,value);
	}
}
