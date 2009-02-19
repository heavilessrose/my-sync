package org.hanyudictionary.common.vo;

import java.io.Serializable;

import org.hanyudictionary.common.util.WordUtil;

public class PYWord implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private String pinyin;

	private ChineseWord[] chinese;

	public ChineseWord[] getChinese() {
		return chinese;
	}

	public void setChinese(ChineseWord[] chinese) {
		this.chinese = chinese;
	}

	public String getPinyin() {
		return pinyin;
	}

	public void setPinyin(String pinyin) {
		this.pinyin = pinyin;
	}
	
	public String toString() {
		return WordUtil.toToneMarkPinyin(getPinyin());
	}
}
