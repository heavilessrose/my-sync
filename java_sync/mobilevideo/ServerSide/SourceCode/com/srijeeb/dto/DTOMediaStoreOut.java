/*
 * Created on Jun 15, 2007
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package com.srijeeb.dto;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

/**
 * @author Srijeeb Roy
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class DTOMediaStoreOut {

	public String successFlag = null;
	public void serialize(DataOutputStream dataStream) throws IOException {
		writeUTF(dataStream, successFlag);
	}
	public static DTOMediaStoreOut deserialize(DataInputStream dataStream) throws IOException {
		DTOMediaStoreOut retObj = new DTOMediaStoreOut();
		retObj.successFlag = readUTF(dataStream);
		return retObj;
	}
	private static void writeUTF(DataOutputStream dataStream, String attribute) throws IOException {
		if ( attribute == null ) attribute = "";
		dataStream.writeUTF(attribute);
	}
	private static String readUTF(DataInputStream dataStream) throws IOException {
		return dataStream.readUTF();
	}
}
