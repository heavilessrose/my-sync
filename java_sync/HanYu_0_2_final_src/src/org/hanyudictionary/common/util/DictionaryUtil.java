package org.hanyudictionary.common.util;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class DictionaryUtil {
	private static DictionaryUtil instance;
	private Map<String, ArrayList<?>> dictionaryMap = new HashMap<String, ArrayList<?>>();

	private DictionaryUtil(){
		
	}
	
	public static DictionaryUtil getInstance(){
		if (instance == null){
			instance = new DictionaryUtil();
		}
		return instance;
	}
	
	public void loadDictionaryDefintion(String lang,String name) throws IOException, ClassNotFoundException{
		FileInputStream fis = new FileInputStream(AppConfig.getInstance().getKey(lang)+"/"+name);
		ObjectInputStream ois = new ObjectInputStream(fis);
		ArrayList dictionaryDefinition = (ArrayList) ois.readObject();

		dictionaryMap.put(name,dictionaryDefinition);
		
		ois.close();
		fis.close();
	}
	
	public static void loadDictionaryDefinitions(){
		
	}
	
	public ArrayList getDictionary(String lang,String name) throws IOException, ClassNotFoundException{
		if (dictionaryMap.get(name)==null){
			loadDictionaryDefintion(lang,name);
		} 
			
		return (ArrayList) dictionaryMap.get(name);
	}
}
