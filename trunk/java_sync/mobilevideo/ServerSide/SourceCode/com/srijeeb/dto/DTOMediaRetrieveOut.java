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
public class DTOMediaRetrieveOut {

	public String successFlag = null;
	public String contentType = null;
	public byte[] outByteArray = null;

	public void serialize(DataOutputStream dataStream) throws IOException {
		writeUTF(dataStream, successFlag);
		writeUTF(dataStream, contentType);
		writeByteArray(dataStream, outByteArray);
	}
	public static DTOMediaRetrieveOut deserialize(DataInputStream dataStream) throws IOException {
		DTOMediaRetrieveOut retObj = new DTOMediaRetrieveOut();
		retObj.successFlag = readUTF(dataStream);
		retObj.contentType = readUTF(dataStream);
		retObj.outByteArray = readByteArrayFully(dataStream);
		return retObj;
	}
	private void writeByteArray(DataOutputStream dataStream,
			byte[] attribute) throws IOException {
		if ( attribute == null ) {
			dataStream.writeInt(0);
		}
		else {
			int iLen = attribute.length;
			dataStream.writeInt(iLen);
			dataStream.write(attribute, 0, iLen);
		}
	}
	private static void writeUTF(DataOutputStream dataStream, String attribute) throws IOException {
		if ( attribute == null ) attribute = "";
		dataStream.writeUTF(attribute);
	}
	private static String readUTF(DataInputStream dataStream) throws IOException {
		return dataStream.readUTF();
	}
	private static byte[] readByteArrayFully(DataInputStream dataStream) throws IOException {
 		byte[] retObj = null;
 		int iLen = dataStream.readInt();
 		if ( iLen > 0 ) {
 			retObj = new byte[iLen];
 			dataStream.readFully(retObj);
 		}
 		return retObj;
	}
}
