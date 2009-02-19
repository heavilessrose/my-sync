package org.hanyudictionary.translator;

/**
 * this class use to create Translor engine that config in application.conf
 * @author Ken
 */
public abstract class TranslatorFactory {
	public static Translator getInstance(String translator){
		Class clazz = null;
		try {
			clazz = Class.forName(translator);
			return (Translator) clazz.newInstance();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		return null;
	}
}
