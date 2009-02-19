package org.hanyudictionary.common.util;

import java.awt.GraphicsEnvironment;


public class FontUtil {
	private static String[] fontList;
	private FontUtil(){
		
	}
	
	public static String[] getFontList() {
		if (fontList == null){
			fontList = GraphicsEnvironment.getLocalGraphicsEnvironment().getAvailableFontFamilyNames();
		}
		return fontList;
	}
}