package winkCC.xml;

import java.io.InputStreamReader;

import org.kxml2.io.KXmlParser;
import org.xmlpull.v1.XmlPullParserException;

/**
 * XMLÎÄ¼þ½âÎö.
 * 
 * @author WangYinghua
 * 
 */
public class XmlPullParser {
	private static KXmlParser parser = new KXmlParser();

	public static String WINKS_BOOK_FOLDER = "/";
	public static String WINKS_BOOK_FILE = "WinksBook.xml";

	public static void parse() throws XmlPullParserException {

		parser.setInput(new InputStreamReader(parser.getClass()
				.getResourceAsStream(WINKS_BOOK_FOLDER + WINKS_BOOK_FILE)));
	}
}
