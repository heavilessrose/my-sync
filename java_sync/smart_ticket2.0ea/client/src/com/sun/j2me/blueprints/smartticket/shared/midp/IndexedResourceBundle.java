package com.sun.j2me.blueprints.smartticket.shared.midp;

import java.io.*;
import java.util.*;

/**
 * 资源及locale 封装
 */
public class IndexedResourceBundle {

	private static IndexedResourceBundle instance = null;
	private String locale;
	private String[] resources;

	public IndexedResourceBundle(String locale, String[] resources) {
		this.locale = locale;
		this.resources = resources;

		return;
	}

	public String getString(int resourceId) {
		return (resourceId >= 0 && resourceId < resources.length) ? resources[resourceId] : null;
	}

	public String getLocale() {
		return locale;
	}

	public int size() {
		return resources.length;
	}

	public void serialize(DataOutputStream out) throws ApplicationException {
		try {
			out.writeUTF(locale);
			out.writeInt(resources.length);

			for (int i = 0; i != resources.length; i++) {
				out.writeUTF(resources[i]);
			}
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static IndexedResourceBundle deserialize(DataInputStream in) throws ApplicationException {
		try {
			String locale = in.readUTF();
			String[] resources = new String[in.readInt()];

			for (int i = 0; i != resources.length; i++) {
				resources[i] = in.readUTF();
			}

			return new IndexedResourceBundle(locale, resources);
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static IndexedResourceBundle getBundleFromPropertyFile(String locale, InputStream in)
			throws IOException {
		Vector resourcesVector = new Vector();
		ByteArrayOutputStream out = null;

		try {
			out = new ByteArrayOutputStream();

			int c;
			int index = 0;

			while ((c = in.read()) != -1) {
				if (c == '\n' || c == '\r') {
					String s = out.toString();
					int i = s.indexOf('=');

					if (i != -1) {
						if (s.substring(0, i).equals(String.valueOf(index))) {
							resourcesVector.addElement(s.substring(i + 1));

							index++;
						} else {
							// FIXME - Throw an exception.
						}
					}

					out.reset();
				} else {
					out.write(c);
				}
			}
		} finally {
			if (out != null) {
				out.close();
			}

			if (in != null) {
				in.close();
			}
		}

		String[] resources = new String[resourcesVector.size()];

		resourcesVector.copyInto(resources);

		return new IndexedResourceBundle(locale, resources);
	}

	public String toString() {
		StringBuffer buffer = new StringBuffer();

		buffer.append("Contents of resource bundle: " + locale + "\n");

		for (int i = 0; i != resources.length; i++) {
			buffer.append(i);
			buffer.append('=');
			buffer.append(resources[i]);
			buffer.append('\n');
		}

		return buffer.toString();
	}

}
