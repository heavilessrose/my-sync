package org.hanyudictionary.translator.ce;

import java.util.ArrayList;
import java.util.List;

import org.hanyudictionary.dao.DAOFactory;
import org.hanyudictionary.dao.DictionaryDAO;
import org.hanyudictionary.translator.Translator;

/**
 * Translator use sequential search direct to file
 * (will optimize performance later)
 * @author Ken
 */
public class CETranslatorUseFile implements Translator {

	public static final String ENGLISH_WORD = "en";

	public static final String CHINESE_WORD = "zh";

	/**
	 * Implementation of direct search
	 */
	public List directSearch(String searchWord) {
		List resultList = null;
		char firstChar = searchWord.charAt(0);
		
		/*
		 * Check what type of word to search
		 */ 
		if (((firstChar >= '\u3400') && (firstChar <= '\u4DBF')) // Chinese unicode length
				|| ((firstChar >= '\u4E00') && (firstChar <= '\u9FAF'))) {
			System.out.println("    ** Chinese Search **");
			resultList = (ArrayList) ceSearch(searchWord);
		} else if (((firstChar >= '\u0041') && (firstChar <= '\u005A'))
				|| ((firstChar >= '\u0061') && (firstChar <= '\u007A'))){
			String tmpWord = searchWord.trim();
			char lastChar = tmpWord.charAt(tmpWord.length() - 1);
			if ((lastChar >= '\u0030') && (lastChar <= '\u0039')){ // Check pinyin (may occur some bugs -*-)
				System.out.println("    ** Pinyin Search **");
				resultList = (ArrayList) pySearch(searchWord);
			}
			else {// anything else go english search
				System.out.println("    ** English Search **");
				resultList = (ArrayList) ecSearch(searchWord);
			}
		}

		return resultList;
	}

	/**
	 * English-Chinese search
	 * @param searchWord
	 * @return
	 */
	private List ecSearch(String searchWord) {
		DictionaryDAO dao = DAOFactory.getECDefDAOUseFile(); // note that this class use direct sequencetial search from file
		List resultList = null;
		try {
			resultList = dao.findWord(searchWord);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return resultList;
	}

	/**
	 * Chinese-English search
	 * @param searchWord
	 * @return
	 */
	private List ceSearch(String searchWord) {
		DictionaryDAO dao = DAOFactory.getCEDefDAOUseFile();
		List resultList = null;
		try {
			resultList = dao.findWord(searchWord);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return resultList;
	}
	
	/**
	 * Pinyin search
	 * @param searchWord
	 * @return
	 */
	private List pySearch(String searchWord) {
		DictionaryDAO dao = DAOFactory.getPYDAOUseFile();
		List resultList = null;
		try {
			resultList = dao.findWord(searchWord);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return resultList;
	}
}
