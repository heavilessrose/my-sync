package org.hanyudictionary.common.vo;

import java.io.Serializable;

public class EnglishWord implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public String word;
	public ChineseWord[] chineseMeaning;

	public String getWord() {
		return word;
	}

	public void setWord(String word) {
		this.word = word;
	}

	public ChineseWord[] getChineseMeaning() {
		return chineseMeaning;
	}

	public void setChineseMeaning(ChineseWord[] chineseMeaning) {
		this.chineseMeaning = chineseMeaning;
	}
	
	public String toString() {
		return getWord();
	}
}
