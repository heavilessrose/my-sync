package luke.java.practice.deadLinkChecker;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CheckTool {

	String regEx = "http://\\w+";
	String pageString;
	public CheckTool() {
	}
	
	public void match(){
		Pattern p = Pattern.compile("regEx");
		 Matcher m = p.matcher("aaaaab");
		 boolean b = m.matches();
	}
}
