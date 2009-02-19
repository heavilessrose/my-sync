package org.hanyudictionary.dao.file;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

import org.hanyudictionary.common.util.AppConfig;
import org.hanyudictionary.common.vo.ChineseWord;
import org.hanyudictionary.dao.DictionaryDAO;

/**
 * Chinese-English use File DAO
 * @author Ken
 */
public class CEDefDAO implements DictionaryDAO{

	public List findWord(String searchWord) throws IOException {
		
		String chDefFolder = AppConfig.getInstance().getKey("CEDef");
		
		ArrayList<ChineseWord> resultList = new ArrayList<ChineseWord>();
		FileInputStream fis = null;
		InputStreamReader isr = null;
		BufferedReader br = null;
		
		try{
			fis = new FileInputStream(new File(chDefFolder+File.separator+"chDef"));
			isr = new InputStreamReader(fis,"UTF-8");
			br = new BufferedReader(isr);
			String tmpWord;
			while ((tmpWord = br.readLine())!=null){
				int beginPinyin = tmpWord.indexOf("[");
				int endPinyin = tmpWord.indexOf("]");
				int firstEng = tmpWord.indexOf("/");
				String chineseWords = tmpWord.substring(0,beginPinyin);
				String simplified = chineseWords.split(" ")[1];
				String tradition = chineseWords.split(" ")[0];
				String pinyin = tmpWord.substring(beginPinyin,endPinyin+1);
				String engMeaning = tmpWord.substring(firstEng+1);
				String[] engWords = engMeaning.trim().split("/");
				
				int count = 0;
				if (simplified.startsWith(searchWord) || tradition.startsWith(searchWord)){
					ChineseWord chineseWord = new ChineseWord();
					chineseWord.setSimplified(simplified);
					chineseWord.setTradition(tradition);
					chineseWord.setPinyin(pinyin);
					chineseWord.setEnglishMeanings(engWords);
					resultList.add(chineseWord);
					count++;
				} else {
					if (count >0){
						break;
					}
				}
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} finally{
			br.close();
			isr.close();
			fis.close();
		}
		
		return resultList;
	
	}
}
