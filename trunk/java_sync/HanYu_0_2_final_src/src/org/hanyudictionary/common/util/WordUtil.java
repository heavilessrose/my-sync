package org.hanyudictionary.common.util;

public class WordUtil {
	private WordUtil(){
		
	} 
	
	public static String toToneMarkPinyin(String before){
		before = before.replace('[',' ');
		before = before.replace(']',' ');
		String [] beforeToneMarks = before.trim().split(" ");
		StringBuffer result = new StringBuffer();
		for (int i=0; i<beforeToneMarks.length; i++){
			String tmp = beforeToneMarks[i].trim().toLowerCase();
			if (tmp.indexOf("a") >=0){
				char tone = tmp.charAt(tmp.length()-1);
				tmp = tmp.substring(0,tmp.length()-1);
				switch (tone){
					case '1': tmp = tmp.replace('a','\u0101'); break;
					case '2': tmp = tmp.replace('a','\u00e1'); break;
					case '3': tmp = tmp.replace('a','\u0103'); break;
					case '4': tmp = tmp.replace('a','\u00e0'); break;
					default : break;
				}		
			} else if (tmp.indexOf("e") >=0){
				char tone = tmp.charAt(tmp.length()-1);
				tmp = tmp.substring(0,tmp.length()-1);
				switch (tone){
					case '1': tmp = tmp.replace('e','\u0113'); break;
					case '2': tmp = tmp.replace('e','\u00E9'); break;
					case '3': tmp = tmp.replace('e','\u0115'); break;
					case '4': tmp = tmp.replace('e','\u00E8'); break;
					default : break;
				}	
			} else if (tmp.indexOf("o") >=0){
				char tone = tmp.charAt(tmp.length()-1);
				tmp = tmp.substring(0,tmp.length()-1);
				switch (tone){
					case '1': tmp = tmp.replace('o','\u014D'); break;
					case '2': tmp = tmp.replace('o','\u00F3'); break;
					case '3': tmp = tmp.replace('o','\u014F'); break;
					case '4': tmp = tmp.replace('o','\u00F2'); break;
					default : break;
				}	
			} else if (tmp.indexOf("iu") >= 0){
				char tone = tmp.charAt(tmp.length()-1);
				tmp = tmp.substring(0,tmp.length()-1);
				switch (tone){
					case '1': tmp = tmp.replace('u','\u016B'); break;
					case '2': tmp = tmp.replace('u','\u00FA'); break;
					case '3': tmp = tmp.replace('u','\u016D'); break;
					case '4': tmp = tmp.replace('u','\u00F9'); break;
					default : break;
				}	
			} else if (tmp.indexOf("ui") >= 0){
				char tone = tmp.charAt(tmp.length()-1);
				tmp = tmp.substring(0,tmp.length()-1);
				switch (tone){
					case '1': tmp = tmp.replace('i','\u012B'); break;
					case '2': tmp = tmp.replace('i','\u00ED'); break;
					case '3': tmp = tmp.replace('i','\u012D'); break;
					case '4': tmp = tmp.replace('i','\u00EC'); break;
					default : break;
				}	
			} else if (tmp.indexOf("i") >= 0){
				char tone = tmp.charAt(tmp.length()-1);
				tmp = tmp.substring(0,tmp.length()-1);
				switch (tone){
					case '1': tmp = tmp.replace('i','\u012B'); break;
					case '2': tmp = tmp.replace('i','\u00ED'); break;
					case '3': tmp = tmp.replace('i','\u012D'); break;
					case '4': tmp = tmp.replace('i','\u00EC'); break;
					default : break;
				}
			} else if (tmp.indexOf("u") >= 0){
				char tone = tmp.charAt(tmp.length()-1);
				tmp = tmp.substring(0,tmp.length()-1);
				switch (tone){
					case '1': tmp = tmp.replace('u','\u016B'); break;
					case '2': tmp = tmp.replace('u','\u00FA'); break;
					case '3': tmp = tmp.replace('u','\u016D'); break;
					case '4': tmp = tmp.replace('u','\u00F9'); break;
					default : break;
				}	
			} else if (tmp.indexOf("v") >= 0){
				char tone = tmp.charAt(tmp.length()-1);
				tmp = tmp.substring(0,tmp.length()-1);
				switch (tone){
					case '1': tmp = tmp.replace('v','\u01D6'); break;
					case '2': tmp = tmp.replace('v','\u01D8'); break;
					case '3': tmp = tmp.replace('v','\u01DA'); break;
					case '4': tmp = tmp.replace('v','\u01DC'); break;
					default : break;
				}	
			}
			result.append(tmp+" ");
		}
		return result.toString();
	}
}
