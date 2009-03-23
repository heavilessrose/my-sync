package winkCC.rms.finals;

import java.io.*;
import javax.microedition.rms.*;

public class RMSAnalyzer {

	public interface Logger {
		void logEnd(RecordStore rs);

		void logException(String name, Throwable e);

		void logException(RecordStore rs, Throwable e);

		void logRecord(RecordStore rs, int id, byte[] data, int size);

		void logStart(RecordStore rs);
	}

	private Logger logger;

	public RMSAnalyzer() {
		this(null);
	}

	public RMSAnalyzer(Logger logger) {
		this.logger = (logger != null) ? logger : new SystemLogger();
	}

	public void analyzeAllRecordStores() {
		String[] names = RecordStore.listRecordStores();

		for (int i = 0; names != null && i < names.length; ++i) {
			analyzeRs(names[i]);
		}
	}

	public void analyzeRs(String rsName) {
		RecordStore rs = null;

		try {
			rs = RecordStore.openRecordStore(rsName, false);
			analyzeRs(rs);
		} catch (RecordStoreException e) {
			logger.logException(rsName, e);
		} finally {
			try {
				rs.closeRecordStore();
			} catch (RecordStoreException e) {
			}
		}
	}

	public synchronized void analyzeRs(RecordStore rs) {
		try {
			logger.logStart(rs);

			int lastID = rs.getNextRecordID();
			int numRecords = rs.getNumRecords();
			int count = 0;
			byte[] data = null;

			for (int id = 0; id < lastID && count < numRecords; ++id) {
				try {
					int size = rs.getRecordSize(id);

					if (data == null || data.length < size) {
						data = new byte[size + 20];
					}

					rs.getRecord(id, data, 0);
					logger.logRecord(rs, id, data, size);

					++count; // only increase if record exists
				} catch (InvalidRecordIDException e) {
					// just ignore and move to the next one
				} catch (RecordStoreException e) {
					logger.logException(rs, e);
				}
			}

		} catch (RecordStoreException e) {
			logger.logException(rs, e);
		} finally {
			logger.logEnd(rs);
		}
	}

	public static class PrintStreamLogger implements Logger {
		public static final int COLS_MIN = 10;
		public static final int COLS_DEFAULT = 20;

		private int cols;
		private int numBytes;
		private StringBuffer hBuf;
		private StringBuffer cBuf;
		private StringBuffer pBuf;
		private PrintStream out;

		public PrintStreamLogger(PrintStream out) {
			this(out, COLS_DEFAULT);
		}

		public PrintStreamLogger(PrintStream out, int cols) {
			this.out = out;
			this.cols = (cols > COLS_MIN ? cols : COLS_MIN);
		}

		private char convertChar(char ch) {
			if (ch < 0x20)
				return '.';
			return ch;
		}

		public void logEnd(RecordStore rs) {
			out.println("\nActual size of records = " + numBytes);
			printChar('-', cols * 4 + 1);

			hBuf = null;
			cBuf = null;
			pBuf = null;
		}

		public void logException(String name, Throwable e) {
			out.println("Exception while analyzing " + name + ": " + e);
		}

		public void logException(RecordStore rs, Throwable e) {
			String name;

			try {
				name = rs.getName();
			} catch (RecordStoreException rse) {
				name = "";
			}

			logException(name, e);
		}

		public void logRecord(RecordStore rs, int id, byte[] data, int len) {
			if (len < 0 && data != null) {
				len = data.length;
			}

			hBuf.setLength(0);
			cBuf.setLength(0);

			numBytes += len;

			out.println("Record #" + id + " of length " + len + " bytes");

			for (int i = 0; i < len; ++i) {
				int b = Math.abs(data[i]);
				String hStr = Integer.toHexString(b);

				if (b < 0x10) {
					hBuf.append('0');
				}

				hBuf.append(hStr);
				hBuf.append(' ');

				cBuf.append(convertChar((char) b));

				if (cBuf.length() == cols) {
					out.println(hBuf + " " + cBuf);

					hBuf.setLength(0);
					cBuf.setLength(0);
				}
			}

			len = cBuf.length();

			if (len > 0) {
				while (len++ < cols) {
					hBuf.append("   ");
					cBuf.append(' ');
				}

				out.println(hBuf + " " + cBuf);
			}
		}

		public void logStart(RecordStore rs) {
			hBuf = new StringBuffer(cols * 3);
			cBuf = new StringBuffer(cols);
			pBuf = new StringBuffer();

			printChar('=', cols * 4 + 1);

			numBytes = 0;

			try {
				out.println("Record store: " + rs.getName());
				out.println("    Number of records = " + rs.getNumRecords());
				out.println("    Total size = " + rs.getSize());
				out.println("    Version = " + rs.getVersion());
				out.println("    Last modified = " + rs.getLastModified());
				out.println("    Size available = " + rs.getSizeAvailable());
				out.println("");
			} catch (RecordStoreException e) {
				logException(rs, e);
			}
		}

		private void printChar(char ch, int num) {
			pBuf.setLength(0);
			while (num-- > 0) {
				pBuf.append(ch);
			}
			out.println(pBuf.toString());
		}
	}

	public static class SystemLogger extends PrintStreamLogger {
		public SystemLogger() {
			super(System.out);
		}

		public SystemLogger(int cols) {
			super(System.out, cols);
		}
	}
}