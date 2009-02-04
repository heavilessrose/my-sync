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
public class DTOMediaRetrieveIn {
	public String mediaId = null;
	public void serialize(DataOutputStream dataStream) throws IOException {
		writeUTF(dataStream, mediaId);
	}
	public static DTOMediaRetrieveIn deserialize(DataInputStream dataStream) throws IOException {
		DTOMediaRetrieveIn retObj = new DTOMediaRetrieveIn();
		retObj.mediaId = readUTF(dataStream);
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
