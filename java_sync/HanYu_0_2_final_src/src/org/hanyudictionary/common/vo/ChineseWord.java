package org.hanyudictionary.common.vo;

import java.io.Serializable;

public class ChineseWord implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public String tradition;
	public String simplified;
	public String pinyin;
	public String[] englishMeanings;
	public String[] getEnglishMeanings() {
		return englishMeanings;
	}
	public void setEnglishMeanings(String[] englishMeanings) {
		this.englishMeanings = englishMeanings;
	}
	public String getPinyin() {
		return pinyin;
	}
	public void setPinyin(String pinyin) {
		this.pinyin = pinyin;
	}
	public String getSimplified() {
		return simplified;
	}
	public void setSimplified(String simplified) {
		this.simplified = simplified;
	}
	public String getTradition() {
		return tradition;
	}
	public void setTradition(String tradition) {
		this.tradition = tradition;
	}
	public String toString() {
		return getSimplified();
	}
}
