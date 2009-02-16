package bm.util;

/* -----------------------------------------------------------------------------
 OpenBaseMovil Util Library, miscellaneous tools
 Copyright (C) 2004-2008 Elondra S.L.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.
 If not, see <a href="http://www.gnu.org/licenses">http://www.gnu.org/licenses</a>.
 ----------------------------------------------------------------------------- */

import bm.core.ResourceManager;
import bm.core.ControlledTask;
import bm.core.zlib.ZInputStream;
import bm.core.event.ProgressEvent;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.storage.RecordStoreFullException;
import bm.storage.RSException;
import bm.storage.Store;
import bm.err.ErrorLog;

import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.game.Sprite;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import java.io.InputStream;
import java.io.IOException;

/*
 * File Information
 *
 * Created on       : 19-jun-2006 14:22:15
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Utility class.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public final class Util {
	public static final Font SMALL_FONT = Font.getFont(Font.FACE_PROPORTIONAL,
			Font.STYLE_PLAIN, Font.SIZE_SMALL);

	/** @noinspection AnalyzingVariableNaming */
	private static final Log log = LogFactory.getLog("Util");

	private static final String UNRESERVED = "-_.!~*'()";

	/**
	 * Pad a string at both sides centering it.
	 * 
	 * @param str
	 *            string to center
	 * @param width
	 *            width
	 * @return centered string or left substring if str.length() > width
	 */
	public static String padCenter(final String str, final int width) {
		if (str.length() > width) {
			return str.substring(0, width);
		} else {
			boolean rigth = true;
			final StringBuffer padded = new StringBuffer(str);
			// noinspection MethodCallInLoopCondition
			while (padded.length() < width) {
				if (rigth) {
					padded.append(' ');
				} else {
					padded.insert(0, ' ');
				}
				rigth = !rigth;
			}
			return padded.toString();
		}
	}

	/**
	 * Pad a string inserting spaces at the begining.
	 * 
	 * @param str
	 *            string
	 * @param length
	 *            desired length
	 * @return padded string
	 */
	public static String padLeft(final String str, final int length) {
		final StringBuffer buffer = new StringBuffer(str);
		// noinspection MethodCallInLoopCondition
		while (buffer.length() < length) {
			buffer.insert(0, ' ');
		}
		return buffer.toString();
	}

	/**
	 * Create a string repeating a single character a given times.
	 * 
	 * @param c
	 *            character
	 * @param length
	 *            length
	 * @return repeated character string
	 */
	public static String repeat(final char c, final int length) {
		final StringBuffer buffer = new StringBuffer();
		for (int i = 0; i < length; i++) {
			buffer.append(c);
		}
		return buffer.toString();
	}

	/**
	 * Pad a string appending spaces at the end.
	 * 
	 * @param str
	 *            string
	 * @param length
	 *            desired length
	 * @return padded string
	 */
	public static String padRight(final String str, final int length) {
		final StringBuffer buffer = new StringBuffer(str);
		// noinspection MethodCallInLoopCondition
		while (buffer.length() < length) {
			buffer.append(' ');
		}
		return buffer.toString();
	}

	/**
	 * Creates a memory report that includes every RecordStore for this midlet
	 * and the total space consumed and free space.
	 * 
	 * @return memory report
	 * @throws RecordStoreException
	 *             on storage errors
	 */
	public static String buildMemoryReport() throws RecordStoreException {
		final ProgressEvent pe = new ProgressEvent();
		pe.setAnimate(true);
		pe.setTitle(ResourceManager.getResource("util.buildingMemoryReport"));
		pe.dispatch();

		final StringBuffer buffer = new StringBuffer();
		final String[] rsNames = RecordStore.listRecordStores();
		int size = 0;
		int sizeAvailable = 0;
		if (rsNames != null) {
			final int count = rsNames.length;
			pe.setAnimate(false);
			pe.setCurrentValue(0);
			pe.setMaxValue(new Integer(count));
			pe.dispatch();
			for (int i = 0; i < count; i++) {
				RecordStore rs = null;
				try {
					rs = RecordStore.openRecordStore(rsNames[i], false);
					size += rs.getSize();
					sizeAvailable = rs.getSizeAvailable();
					buffer.append(rsNames[i]).append(": ").append(rs.getSize())
							.append("\n");
				} catch (RecordStoreException e) {
					throw e;
				} finally {
					try {
						if (rs != null) {
							rs.closeRecordStore();
						}
					} catch (Exception e) {
					}
				}
				pe.increment();
				pe.dispatch();
			}
		} else {
			RecordStore rs = null;
			try {
				rs = RecordStore.openRecordStore("_T_x_s_o_a_s_l_", true);
				sizeAvailable = rs.getSizeAvailable();
				rs.closeRecordStore();
				rs = null;
			} catch (RecordStoreException e) {
				throw e;
			} finally {
				try {
					if (rs != null)
						rs.closeRecordStore();
				} catch (Exception e) {
				}
				RecordStore.deleteRecordStore("_T_x_s_o_a_s_l_");
			}
		}
		buffer.append(ResourceManager.getResource("util.usedStorage")).append(
				": ").append(size).append("\n").append(
				ResourceManager.getResource("util.freeStorage")).append(": ")
				.append(sizeAvailable);
		return buffer.toString();
	}

	public static boolean hasFileAPI() {
		return hasAPI("javax.microedition.io.file.FileConnection");
	}

	public static boolean hasLocationAPI() {
		return hasAPI("javax.microedition.location.Location");
	}

	public static boolean hasBluetoothAPI() {
		return hasAPI("javax.bluetooth.LocalDevice");
	}

	private static boolean hasAPI(final String className) {
		try {
			Class.forName(className);
			return true;
		} catch (ClassNotFoundException e) {
			return false;
		}
	}

	public static Image loadImage(final Class clazz, final String name) {
		try {
			return Image.createImage(clazz.getResourceAsStream(name));
		} catch (Exception e) {
			return null;
		}
	}

	public static Image scale(final Image src, final int width, final int height) {
		int scanline = src.getWidth();
		int srcw = src.getWidth();
		int srch = src.getHeight();
		int buf[] = new int[srcw * srch];
		src.getRGB(buf, 0, scanline, 0, 0, srcw, srch);
		int buf2[] = new int[width * height];
		for (int y = 0; y < height; y++) {
			int c1 = y * width;
			int c2 = (y * srch / height) * scanline;
			for (int x = 0; x < width; x++) {
				buf2[c1 + x] = buf[c2 + x * srcw / width];
			}
		}
		return Image.createRGBImage(buf2, width, height, true);
	}

	public static Image crop(final Image image, int x, int y,
			final int clipWidth, final int clipHeight) {
		int width = image.getWidth();
		if (width > clipWidth) {
			width = clipWidth;
			if (image.getWidth() - x < width) {
				x = image.getWidth() - width;
			}
		} else {
			x = (clipWidth - width) / 2;
		}

		int height = image.getHeight();
		if (height > clipHeight) {
			height = clipHeight;
			if (image.getHeight() - y < height) {
				y = image.getHeight() - height;
			}
		} else {
			y = (clipHeight - height) / 2;
		}

		if (width != image.getWidth() || height != image.getHeight()) {
			return Image.createImage(image, x, y, width, height,
					Sprite.TRANS_NONE);
		} else {
			return image;
		}
	}

	public static String URLEncode(String s) {
		if (s == null) {
			return null;
		}
		final StringBuffer encoded = new StringBuffer();
		final int length = s.length();
		final char[] chars = s.toCharArray();
		for (int i = 0; i < length; i++) {
			if (chars[i] == ' ') {
				encoded.append('+');
			} else if (('A' <= chars[i] && chars[i] <= 'Z')
					|| ('a' <= chars[i] && chars[i] <= 'z')
					|| ('0' <= chars[i] && chars[i] <= '9')
					|| UNRESERVED.indexOf(chars[i]) > 0) {
				encoded.append(chars[i]);
			} else if (chars[i] < '0') {
				encoded.append('%').append(Integer.toHexString(chars[i]));
			} else if (chars[i] <= 0x07FF) {
				encoded.append('%').append(
						Integer.toHexString(0xc0 | (chars[i] >> 6)))
						.append('%').append(
								Integer.toHexString(0x80 | (chars[i] & 0x3F)));
			} else {
				encoded.append('%').append(
						Integer.toHexString(0xe0 | (chars[i] >> 12))).append(
						'%').append(
						Integer.toHexString(0x80 | ((chars[i] >> 6) & 0x3F)))
						.append('%').append(
								Integer.toHexString(0x80 | (chars[i] & 0x3F)));
			}
		}
		return encoded.toString();
	}

	/**
	 * Copy an index definition from an input stream.
	 * 
	 * @param name
	 *            name of index store
	 * @param is
	 *            input stream
	 * @throws SerializationException
	 *             on errors
	 * @throws bm.storage.RecordStoreFullException
	 *             if no space left
	 * @throws bm.storage.RSException
	 *             on errors
	 * @throws java.io.IOException
	 *             on errors
	 */
	public static void copyIndex(final String name, final InputStream is)
			throws SerializationException, RecordStoreFullException,
			RSException, IOException {
		final Log log = Util.log;

		ZInputStream zis = null;
		SerializerInputStream in = null;
		try {
			zis = new ZInputStream(is);
			in = new SerializerInputStream(zis);

			final int order = in.readInt(); // ToDo: should do something with
			// this?
			final byte type = in.readByte();
			final boolean caseSensitive = in.readBoolean();
			Store rs = null;
			boolean burstMode = Store.isBurstMode();
			try {
				System.gc();
				Store.safeDeleteRecordStore(name);
				Store.setBurstMode(!ControlledTask.isBackgroundTask());
				rs = Store.get(name, 1);
				rs.open(true);
				log.debug("created recordstore");
				final int size = in.readInt();
				log.debug("index size (entries): " + size);
				if (size > 0) {
					final ProgressEvent event = new ProgressEvent();
					event.setAnimate(false);
					event.setCurrentValue(0);
					event.setMaxValue(new Integer(size));
					event.dispatch();
					for (int j = 0; j < size; j++) {
						log.debug("record: " + j);
						final byte[] data = in.readBlob();
						int rid = rs.addRecord(data, 0, data.length);
						log.debug("record added (" + rid + "): " + data.length
								+ " bytes");
						event.increment();
						event.dispatch();
						if (j == 0 && rid != 1) {
							ErrorLog.addError("DbTool", "processIndex", null,
									"Old not erased, recordId is not 1 (" + rid
											+ ")", null);
							log.error("RecordId is not 1!!!!");
							throw new IOException("Old not erased");
						}
						if (j % 100 == 0) {
							System.gc();
						}
					}
				}
				System.gc();
				log.debug("index " + name + " finished");
			} finally {
				try {
					if (rs != null)
						rs.close();
				} catch (Exception e) {
				}
				Store.setBurstMode(burstMode);
			}
		} finally {
			try {
				if (in != null)
					in.close();
			} catch (Exception e) {
			}
			try {
				if (zis != null)
					in.close();
			} catch (Exception e) {
			}
		}
	}

	/**
	 * Copy an index definition from a resource file.
	 * 
	 * @param name
	 *            name of index store
	 * @param resourceName
	 *            resource name
	 * @throws SerializationException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space left
	 * @throws RSException
	 *             on errors
	 * @throws IOException
	 *             on errors
	 */
	public static void copyIndex(final String name, final String resourceName)
			throws SerializationException, IOException,
			RecordStoreFullException, RSException {
		InputStream is = null;
		try {
			is = name.getClass().getResourceAsStream(resourceName);
			if (is != null) {
				copyIndex(name, is);
			}
		} finally {
			if (is != null)
				try {
					is.close();
				} catch (Exception e) {
				}
		}
	}
}
