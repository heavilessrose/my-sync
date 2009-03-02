package com.winksi.j2me.cc.tools;

import java.util.*;
import javax.microedition.lcdui.*;
import java.io.*;

/**
 * Title: 
 *
 * Description:
 *
 * Copyright: Copyright (c) 2006
 *
 * Company: 
 *
 * @author jerry
 * @version 1.0
 */
public final class ImageTools {

	//    private final static int LARGESTBYTESTREAM = 170000;   // Current maximum byte stream allowed
	private final static int THUMB_MAX_SIZE = 16284; // Max Thumbnail size

	private ImageTools() {
	}

	public static Image createImage(String path) {
		Image tmpImage = null;
		try {
			tmpImage = Image.createImage(path);
		} catch (IOException e) {
			//            System.out.println(
			//                    "IOException occurs: ImageTools--createImage(String path='" +
			//                    path + "')");
		}
		return tmpImage;
	}

	public static Image createImage(InputStream is) throws Exception {
		Image tmpImage = null;

		tmpImage = Image.createImage(is);

		return tmpImage;
	}

	/**
	 * Checks if two consectutive bytes can be interpreted as a jpeg
	 * @param b1 first byte
	 * @param b2 second byte
	 * @return true if b1 and b2 are jpeg markers
	 */
	private static boolean isJPEG(byte b1, byte b2) {
		return ((b1 & 0xFF) == 0xFF && (b2 & 0xFF) == 0xD8);
	}

	/**
	 * Traverse an inputStream and return a thumbnail image if any. We build the thumbnail directly
	 * from the inputstream, thus avoiding to run out of memory on very large picture files.
	 * @param str the stream
	 * @returns Image - created from thumbnail iside jpeg file.
	 */
	private static Image getThumbFromFile(InputStream str) {

		byte[] tempByteArray = new byte[THUMB_MAX_SIZE]; // how big can a thumb get.
		byte[] bytefileReader = { 0 }; // lazy byte reader
		byte firstByte, secondByte = 0;
		int currentIndex = 0;
		try {
			str.read(bytefileReader);
			firstByte = bytefileReader[0];
			str.read(bytefileReader);
			secondByte = bytefileReader[0];
			if (isJPEG(firstByte, secondByte)) {
				byte rByte = 0;
				do {
					while (rByte != -1) {
						str.read(bytefileReader);
						rByte = bytefileReader[0];
					}
					str.read(bytefileReader);
					rByte = bytefileReader[0];

				} while ((rByte & 0xFF) != 0xD8); // thumb starts
				tempByteArray[currentIndex++] = -1;
				tempByteArray[currentIndex++] = rByte;
				rByte = 0;
				do {
					while (rByte != -1) {
						str.read(bytefileReader);
						rByte = bytefileReader[0];
						tempByteArray[currentIndex++] = rByte;
					}
					str.read(bytefileReader);
					rByte = bytefileReader[0];
					tempByteArray[currentIndex++] = rByte;
				} while ((rByte & 0xFF) != 0xD9); // thumb ends

				// byte[] thumbBytes = new byte[currentIndex-1];
				tempByteArray[currentIndex++] = -1;
				Image im = Image.createImage(tempByteArray, 0, currentIndex - 1);
				tempByteArray = null;
				return im;

			}
			str.close();
		} catch (Exception ex) {
			//      System.out.println (" getThumbFromFile(): " + ex.toString ());
		}
		return null;
	}
}
