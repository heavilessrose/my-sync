package net.http.cookie;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Vector;

import javax.microedition.rms.RecordEnumeration;
import javax.microedition.rms.RecordFilter;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;

/**
 * This class is used to manage cookie on the mobile device. not implemented
 * 
 * @author mingjava
 * @version 0.1 05/06/2006
 */
public class CookieManager {

	private RecordStore rs = null;
	public static final String COOKIE_STORE = "httpmecookie";
	private Vector buffer = new Vector();// buffer the session cookie only

	private static class CookieFilter implements RecordFilter {
		private String path = "/";

		CookieFilter(String path) {
			this.path = path;
		}

		public boolean matches(byte[] data) {
			ByteArrayInputStream bais = new ByteArrayInputStream(data);
			DataInputStream dis = new DataInputStream(bais);
			try {
				Cookie cookie = Cookie.deserialize(dis);
				return path.startsWith(cookie.getPath());
			} catch (IOException ex) {
				ex.printStackTrace();
				return false;
			}
		}
	}

	/** Creates a new instance of CookieManager */
	public CookieManager() {
		try {
			rs = RecordStore.openRecordStore(COOKIE_STORE, true);
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		}
	}

	public void release() {
		try {
			rs.closeRecordStore();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		}
	}

	public void addCookie(Cookie cookie) {
		int i = buffer.indexOf(cookie);
		// not in buffer
		if (i == -1) {
			if (cookie.getExpire() == Cookie.SESSION_COOKIE) {
				buffer.addElement(cookie);
				return;
			}
		} else {
			// in the buffer
			if (cookie.isExpired()) {
				buffer.removeElementAt(i);
			} else {
				// replace the cookie
				buffer.setElementAt(cookie, i);
			}
		}
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		DataOutputStream dos = new DataOutputStream(baos);
		try {
			cookie.serialize(dos);
			byte[] data = baos.toByteArray();
			int id = isExist(cookie);
			if (id == -1) {
				rs.addRecord(data, 0, data.length);
			} else if (cookie.isExpired()) {
				deleteCookie(cookie);
			} else {
				rs.setRecord(id, data, 0, data.length);
			}
		} catch (IOException ex) {
			ex.printStackTrace();
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		} finally {
			try {
				dos.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	private int isExist(Cookie cookie) {
		try {
			RecordEnumeration re = rs.enumerateRecords(null, null, false);
			int size = re.numRecords();
			if (size == 0)
				return -1;
			else {
				while (re.hasNextElement()) {
					int id = re.nextRecordId();
					byte[] data = rs.getRecord(id);
					ByteArrayInputStream bais = new ByteArrayInputStream(data);
					DataInputStream dis = new DataInputStream(bais);
					try {
						Cookie c = Cookie.deserialize(dis);
						if (cookie.equals(c))
							return id;
					} catch (IOException ex) {
						ex.printStackTrace();
					} finally {
						try {
							dis.close();
						} catch (IOException ex) {

						}
					}
				}
			}
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		}
		return -1;
	}

	public Cookie[] getCookie(String path) {
		Vector v = new Vector();
		try {
			RecordEnumeration re = rs.enumerateRecords(new CookieFilter(path),
					null, false);
			int size = re.numRecords();
			if (size != 0) {
				int i = 0;
				while (re.hasNextElement()) {
					int id = re.nextRecordId();
					byte[] data = rs.getRecord(id);
					ByteArrayInputStream bais = new ByteArrayInputStream(data);
					DataInputStream dis = new DataInputStream(bais);
					try {
						Cookie cookie = Cookie.deserialize(dis);
						if (cookie.isExpired()) {
							rs.deleteRecord(id);
						} else {
							v.addElement(cookie);
						}
					} catch (IOException ex) {
						ex.printStackTrace();
					} finally {
						try {
							dis.close();
						} catch (IOException e) {
							// do nothing
						}
					}
				}
			}
		} catch (RecordStoreException ex) {
			ex.printStackTrace();
		}
		for (int i = 0; i < buffer.size(); i++) {
			if (path.startsWith(((Cookie) buffer.elementAt(i)).getPath())) {
				v.addElement(buffer.elementAt(i));
			}
		}
		int vsize = v.size();
		if (vsize == 0)
			return null;
		else {
			Cookie[] cookies = new Cookie[vsize];
			v.copyInto(cookies);
			v.removeAllElements();
			v = null;
			return cookies;
		}
	}

	public void deleteCookie(Cookie cookie) {
		int id = isExist(cookie);
		if (id != -1) {
			try {
				rs.deleteRecord(id);
			} catch (RecordStoreException ex) {
				ex.printStackTrace();
			}
		}
	}
}
