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
import org.hanyudictionary.common.vo.EnglishWord;
import org.hanyudictionary.dao.DictionaryDAO;

/**
 * English-Chinese use file DAO
 * @author Ken
 */
public class ECDefDAO implements DictionaryDAO {

	public List findWord(String searchWord) throws IOException {
		String tmp = searchWord.toUpperCase();
		String ecDefFolder = AppConfig.getInstance().getKey("ECDef");
		
		FileInputStream fis = null;
		InputStreamReader isr = null;
		BufferedReader br = null;
		ArrayList<EnglishWord> resultList = new ArrayList<EnglishWord>();

		try{
			fis = new FileInputStream(new File(ecDefFolder+File.separator+tmp.charAt(0)+"_enDef"));
			isr = new InputStreamReader(fis,"UTF-8");
			br = new BufferedReader(isr);
			
			String word = br.readLine();
			while ((word = br.readLine())!=null){
				int splitter = word.indexOf("|");
	
				String engWord = word.substring(0, splitter);
				int count = 0;
				if (engWord.toLowerCase().startsWith(searchWord.toLowerCase())){
					String chWords = word.substring(splitter + 1);
		
					String[] chineseWordStr = chWords.split("/");
					ChineseWord[] chineseWords = new ChineseWord[chineseWordStr.length];
					
					for (int i = 0; i < chineseWordStr.length; i++) {
						String[] chineseWordDetails = chineseWordStr[i].split(":");
						String simplified = chineseWordDetails[1];
						String tradition = chineseWordDetails[0];
						String pinyin = chineseWordDetails[2];
		
						ChineseWord chineseWord = new ChineseWord();
						chineseWord.setSimplified(simplified);
						chineseWord.setTradition(tradition);
						chineseWord.setPinyin(pinyin);
						chineseWords[i] = chineseWord;
					}
		
					EnglishWord englishWord = new EnglishWord();
					englishWord.setWord(engWord);
					englishWord.setChineseMeaning(chineseWords);
					resultList.add(englishWord);
					count++;
				} else {
					if (count > 0){
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
		} finally {
			br.close();
			isr.close();
			fis.close();
		}
		
		return resultList;
	}

}
