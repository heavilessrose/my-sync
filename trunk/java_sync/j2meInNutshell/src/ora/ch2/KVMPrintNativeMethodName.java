package ora.ch2;


public class KVMPrintNativeMethodName {

    public static void main(String[] args) {
        String className = args[0];
        String methodName = args[1];
        String signature = (args.length > 2 ? args[2] : null);
        
        System.out.println(convertToJNIName(className, methodName, signature));
    }
    
    // CODE taken from JavaCodeCompact
    // Create a JNI external name from the given classname and methodname.
    // If typename is not null, then we are producing a JNI name for an 
    // overloaded method, and we must include the type name, also.
    public static String
    convertToJNIName(String classname, String methodname, String typename) { 
	StringBuffer result = new StringBuffer("Java_");
	stringToJNI(result, classname);
	result.append('_');
	stringToJNI(result, methodname);
	if (typename != null) {
	    result.append("__");
	    // Only include the stuff inside the parentheses.
	    stringToJNI(result, typename.substring(1, typename.indexOf(')')));
	} 
	return result.toString();
    }

    private static void 
    stringToJNI(StringBuffer result, String name) { 
	int length = name.length();
	for (int i = 0; i < length; i++) { 
	    char ch = name.charAt(i);
	    if (ch <= 0x7f && Character.isLetterOrDigit(ch)) {
		result.append(ch);
	    } else { 
		result.append('_');
		switch(ch) { 
		   case '/':  break; // the _ is all we need
		   case '_':  result.append('1'); break;
		   case ';':  result.append('2'); break;
		   case '[':  result.append('3'); break;
		   default: { 
		       // Adding 0x100000 to a 16-bit number forces 
		       // toHexString to produce a string of the form "10xxxx".
		       // Discard the initial "1" to get the right result.
		       String t = Integer.toHexString(ch + 0x100000);
		       result.append(t.substring(1));
		   }
		}
	    }
	}
    }


}
