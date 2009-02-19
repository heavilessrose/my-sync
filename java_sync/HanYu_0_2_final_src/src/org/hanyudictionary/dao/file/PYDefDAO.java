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
import org.hanyudictionary.common.vo.PYWord;
import org.hanyudictionary.dao.DictionaryDAO;

/**
 * Pinyin use file DAO
 * @author Ken
 */
public class PYDefDAO implements DictionaryDAO {

	public List findWord(String searchWord) throws IOException {
		String tmp = searchWord.toUpperCase();
		String pyDefFolder = AppConfig.getInstance().getKey("PYDef");
		FileInputStream fis = null;
		InputStreamReader isr = null;
		BufferedReader br = null;
		
		List<PYWord> resultList = new ArrayList<PYWord>();
		
		try {
			fis = new FileInputStream(new File(pyDefFolder+File.separator+tmp.charAt(0)+"_pinyinDef"));
			isr = new InputStreamReader(fis,"UTF-8");
			br = new BufferedReader(isr);
			String word;
			while ((word = br.readLine()) != null) {
				
				
				int splitter = word.indexOf("|");
	
				String pinyinWords = word.substring(0, splitter);
				
				int count =0;
				if (pinyinWords.toLowerCase().startsWith(searchWord.toLowerCase())){
					String chWords = word.substring(splitter + 1);
					
					String[] chineseWordStr = chWords.split("/");
					ChineseWord[] chineseWords = new ChineseWord[chineseWordStr.length];
		
					for (int i = 0; i < chineseWordStr.length; i++) {
						String[] chineseWordDetails = chineseWordStr[i].split(":");
						String simplified = chineseWordDetails[1];
						String tradition = chineseWordDetails[0];
						String[] engWord = chineseWordDetails[2].split(",");
		
						ChineseWord chineseWord = new ChineseWord();
						chineseWord.setSimplified(simplified);
						chineseWord.setTradition(tradition);
						chineseWord.setEnglishMeanings(engWord);
						chineseWords[i] = chineseWord;
					}
		
					PYWord pyWord = new PYWord();
					pyWord.setPinyin(pinyinWords);
					pyWord.setChinese(chineseWords);
		
					resultList.add(pyWord);
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
		} finally{
			br.close();
			isr.close();
			fis.close();
		}
		return resultList;
	}

}
