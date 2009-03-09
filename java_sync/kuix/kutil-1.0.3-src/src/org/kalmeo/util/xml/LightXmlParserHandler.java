package org.kalmeo.util.xml;

import java.util.Hashtable;

/**
 * @author bbeaulant
 */
public interface LightXmlParserHandler {

	/**
	 * Receive notification of the beginning of the document.
	 */
	public void startDocument();

	/**
	 * Receive notification of the start of an element.
	 * 
	 * @param name
	 * @param attributes
	 */
	public void startElement(String name, Hashtable attributes);

	/**
	 * Receive notification of the end of an element.
	 * 
	 * @param name
	 */
	public void endElement(String name);

	/**
	 * Receive notification of character data inside an element.
	 * 
	 * @param characters
	 * @param isCDATA
	 */
	public void characters(String characters, boolean isCDATA);

	/**
	 * Receive notification of the end of the document.
	 */
	public void endDocument();

}
