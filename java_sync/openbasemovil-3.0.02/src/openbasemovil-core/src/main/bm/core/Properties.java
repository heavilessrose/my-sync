package bm.core;

import bm.core.io.SerializationException;
import bm.core.io.Serializable;
import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializerInputStream;

import java.io.*;
import java.util.Hashtable;
import java.util.Enumeration;

/*
 * File Information
 *
 * Created on       : 13-sep-2005 12:51:22
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:34:57 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:4 $
 */

/**
 * Implementation of the missing class Properties from the j2se.<br/>
 * Handles file encoding, with a default encoding of UTF-8.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public class Properties extends Hashtable implements Serializable {
	/**
	 * Void constructor.
	 */
	public Properties() {
	}

	/**
	 * Get the name of the class to be used for serialization/deserialization of
	 * complex/nested objects.
	 * 
	 * @return class name
	 */
	public String getSerializableClassName() {
		return "bm.core.Properties";
	}

	/**
	 * Save the contents to a stream.
	 * 
	 * @param os
	 *            output stream
	 * @throws IOException
	 *             on errors
	 */
	public void save(final OutputStream os) throws IOException {
		final OutputStreamWriter out = new OutputStreamWriter(os,
				CoreConstants.DEFAULT_ENCODING);
		for (Enumeration i = keys(); i.hasMoreElements();) {
			final String key = (String) i.nextElement();
			final String value = (String) get(key);
			out.write(key);
			out.write("=");
			out.write(escape(value));
			out.write("\n");
		}
		out.flush();
	}

	private String escape(final String value) {
		final StringBuffer buffer = new StringBuffer();
		final int count = value.length();
		for (int i = 0; i < count; i++) {
			char c = value.charAt(i);
			switch (c) {
			case '\\':
				buffer.append("\\\\");
				break;

			case '\n':
				buffer.append("\\n");
				break;

			case '\r':
				buffer.append("\\r");
				break;

			case '\t':
				buffer.append("\\t");

			default:
				buffer.append(c);
			}
		}
		return buffer.toString();
	}

	/**
	 * Create a properties object loading it from the given stream.
	 * 
	 * @param is
	 *            input
	 * @throws IOException
	 *             on errors
	 */
	public Properties(final InputStream is) throws IOException {
		load(is, CoreConstants.DEFAULT_ENCODING);
	}

	/**
	 * Create a properties object loading it from the given stream with the
	 * given encoding.
	 * 
	 * @param is
	 *            input
	 * @param encoding
	 *            encoding
	 * @throws IOException
	 *             on errors
	 */
	public Properties(final InputStream is, final String encoding)
			throws IOException {
		load(is, encoding);
	}

	/**
	 * Load the properties from the given file, does not delete existing
	 * properties.
	 * 
	 * @param is
	 *            input
	 * @throws IOException
	 *             on errors
	 * @see #load(java.io.InputStream, String)
	 */
	public void load(final InputStream is) throws IOException {
		load(is, CoreConstants.DEFAULT_ENCODING);
	}

	/**
	 * Load the properties from the given file with the given encoding, does not
	 * delete the existing properties.<br/>
	 * This method handles comments (lines starting with #), and escaping
	 * through the \ character. If a backslash is followed by a new line
	 * character, the new line is ignored, if it's followed by a n or a t the
	 * respective control character is inserted (new line or tab).<br/>
	 * Carriage return characeters are alaways ignored (\r).<br/>
	 * Invalid lines (whithout an equal sign or starting whit an equal sign) are
	 * ignored.<br/>
	 * Empty properties are setProperty to an empty string, not to null.
	 * 
	 * @param is
	 *            input
	 * @param encoding
	 *            encoding
	 * @throws IOException
	 *             on errors
	 */
	public synchronized void load(final InputStream is, final String encoding)
			throws IOException {
		final PropertiesReader r = new PropertiesReader(is, encoding);
		String[] pair = r.next();
		while (pair != null) {
			put(pair[0], pair[1]);
			pair = r.next();
		}
	}

	public String getProperty(final String key) {
		return (String) get(key);
	}

	/**
	 * Put all elements from another properties object, eventually overriding
	 * the ones at this object.
	 * 
	 * @param props
	 *            properties to add
	 */
	public void putAll(final Properties props) {
		for (final Enumeration i = props.keys(); i.hasMoreElements();) {
			final String key = (String) i.nextElement();
			put(key, props.getProperty(key));
		}
	}

	/**
	 * Get a property as an int, with a default value of 0.
	 * 
	 * @param key
	 *            property key
	 * @return value
	 */
	public int getInt(final String key) {
		return getInt(key, 0);
	}

	/**
	 * Get a property as an int.
	 * 
	 * @param key
	 *            property key
	 * @param defaultValue
	 *            default value if null or not a number
	 * @return value
	 */
	public int getInt(final String key, final int defaultValue) {
		final String value = (String) get(key);
		try {
			return value != null ? Integer.parseInt(value) : defaultValue;
		} catch (NumberFormatException e) {
			return defaultValue;
		}
	}

	/**
	 * Get a property as a long, with a default value of 0.
	 * 
	 * @param key
	 *            property key
	 * @return value
	 */
	public long getLong(final String key) {
		return getLong(key, 0);
	}

	/**
	 * Get a property as a long.
	 * 
	 * @param key
	 *            property key
	 * @param defaultValue
	 *            default value if null or not a number
	 * @return value
	 */
	public long getLong(final String key, final long defaultValue) {
		final String value = (String) get(key);
		try {
			return value != null ? Long.parseLong(value) : defaultValue;
		} catch (NumberFormatException e) {
			return defaultValue;
		}
	}

	/**
	 * Get a property as a short, with a default value of 0.
	 * 
	 * @param key
	 *            property key
	 * @return value
	 */
	public short getShort(final String key) {
		return getShort(key, (short) 0);
	}

	/**
	 * Get a property as a short.
	 * 
	 * @param key
	 *            property key
	 * @param defaultValue
	 *            default value if null or not a number
	 * @return value
	 */
	public short getShort(final String key, final short defaultValue) {
		final String value = (String) get(key);
		try {
			return value != null ? Short.parseShort(value) : defaultValue;
		} catch (NumberFormatException e) {
			return defaultValue;
		}
	}

	/**
	 * Get a property as a byte, with a default value of 0.
	 * 
	 * @param key
	 *            property key
	 * @return value
	 */
	public byte getByte(final String key) {
		return getByte(key, (byte) 0);
	}

	/**
	 * Get a property as a byte.
	 * 
	 * @param key
	 *            property key
	 * @param defaultValue
	 *            default value if null or not a number
	 * @return value
	 */
	public byte getByte(final String key, final byte defaultValue) {
		final String value = (String) get(key);
		try {
			return value != null ? Byte.parseByte(value) : defaultValue;
		} catch (NumberFormatException e) {
			return defaultValue;
		}
	}

	/**
	 * Get a property as a boolean, with a default value of false.
	 * 
	 * @param key
	 *            property key
	 * @return value
	 */
	public boolean getBoolean(final String key) {
		return getBoolean(key, false);
	}

	/**
	 * Get a property as a boolean.<br/>
	 * The values 1, s, y and t are interpreted as true, anything else as false.
	 * 
	 * @param key
	 *            property key
	 * @param defaultValue
	 *            default value if null
	 * @return value
	 */
	public boolean getBoolean(final String key, final boolean defaultValue) {
		final String value = (String) get(key);
		return value != null ? "1syt".indexOf(value.toLowerCase().charAt(0)) > -1
				: defaultValue;
	}

	public void serialize(final SerializerOutputStream out)
			throws SerializationException {
		try {
			// log.debug( "serialize: " + out );
			save(out);
		} catch (IOException e) {
			throw new SerializationException(CoreConstants.ERR_PRP_SERIALIZE, e);
		}
	}

	public void deserialize(final SerializerInputStream in)
			throws SerializationException {
		try {
			// log.debug( "deserialize: " + in );
			load(in);
		} catch (IOException e) {
			throw new SerializationException(CoreConstants.ERR_PRP_DESERIALIZE,
					e);
		}
	}
}
