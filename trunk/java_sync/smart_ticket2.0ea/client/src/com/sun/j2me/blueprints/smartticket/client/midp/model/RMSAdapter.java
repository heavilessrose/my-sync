package com.sun.j2me.blueprints.smartticket.client.midp.model;

import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;
import com.sun.j2me.blueprints.smartticket.shared.midp.IndexedResourceBundle;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;
import java.io.*;
import javax.microedition.rms.*;

/** RMS适配器 */
public class RMSAdapter {

	public static final String RECORD_STORE_INDEX = "INDEX";
	public static final String RECORD_STORE_REMOTE_DATA = "REMOTE_DATA";
	public static final String RECORD_STORE_LOCAL_DATA = "LOCAL_DATA";
	private RecordStore indexRecordStore;
	// Should we additionally keep an updated copy of IndexEntries (cf. listener)?
	private RecordStore remoteDataRecordStore;
	private RecordStore localDataRecordStore;

	/**
	 * @link dependency
	 */
	/* # IndexEntry lnkIndexEntry; */
	private static class IndexEntryFilter implements RecordFilter {

		private String key;
		private int type;
		private int mode;

		public IndexEntryFilter(String key, int type) {
			this(key, type, IndexEntry.MODE_ANY);
		}

		public IndexEntryFilter(String key, int type, int mode) {
			this.key = key;
			this.type = type;
			this.mode = mode;

			return;
		}

		public boolean matches(byte[] candidate) {
			try {
				return IndexEntry.matches(candidate, key, type, mode);
			} catch (IOException ioe) {
				ioe.printStackTrace();

				return false;
			}
		}
	}

	public RMSAdapter() throws ApplicationException {
		try {
			indexRecordStore = RecordStore.openRecordStore(RECORD_STORE_INDEX, true);
			remoteDataRecordStore = RecordStore.openRecordStore(RECORD_STORE_REMOTE_DATA, true);
			localDataRecordStore = RecordStore.openRecordStore(RECORD_STORE_LOCAL_DATA, true);

			return;
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public void closeRecordStores() throws ApplicationException {
		try {
			indexRecordStore.closeRecordStore();
			remoteDataRecordStore.closeRecordStore();
			localDataRecordStore.closeRecordStore();

			return;
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public void addIndexEntry(IndexEntry indexEntry) throws ApplicationException {
		try {
			byte[] data = indexEntry.serialize();

			if (indexRecordStore.getNumRecords() > 0) {
				RecordEnumeration records = indexRecordStore.enumerateRecords(new IndexEntryFilter(
						indexEntry.getKey(), indexEntry.getType()), null, false);

				if (records.hasNextElement()) {
					indexRecordStore.setRecord(records.nextRecordId(), data, 0, data.length);
				} else {
					indexRecordStore.addRecord(data, 0, data.length);
				}
			} else {
				indexRecordStore.addRecord(data, 0, data.length);
			}

			return;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public IndexEntry[] getIndexEntries(int type, int mode) throws ApplicationException {
		try {
			if (indexRecordStore.getNumRecords() > 0) {
				RecordEnumeration records = indexRecordStore.enumerateRecords(new IndexEntryFilter(
						null, type, mode), null, false);
				IndexEntry[] indexEntries = new IndexEntry[records.numRecords()];

				for (int i = 0; i < indexEntries.length; i++) {
					byte[] data = indexRecordStore.getRecord(records.nextRecordId());

					indexEntries[i] = IndexEntry.deserialize(data);
				}

				return indexEntries;
			} else {
				return new IndexEntry[0];
			}
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public IndexEntry getIndexEntry(String key, int type, int mode) throws ApplicationException {
		try {
			if (indexRecordStore.getNumRecords() > 0) {
				RecordEnumeration records = indexRecordStore.enumerateRecords(new IndexEntryFilter(
						key, type, mode), null, false);

				if (records.hasNextElement()) {
					byte[] data = indexRecordStore.getRecord(records.nextRecordId());

					return IndexEntry.deserialize(data);
				}
			}

			return null;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public void deleteIndexEntry(String key, int type) throws ApplicationException {
		try {
			if (indexRecordStore.getNumRecords() > 0) {
				RecordEnumeration records = indexRecordStore.enumerateRecords(new IndexEntryFilter(
						key, type), null, false);

				while (records.hasNextElement()) {
					indexRecordStore.deleteRecord(records.nextRecordId());
				}
			}

			return;
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	private void delete(RecordStore recordStore, int recordId) throws ApplicationException {
		try {
			recordStore.deleteRecord(recordId);

			return;
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public Theater loadTheater(int recordId) throws ApplicationException {
		try {
			byte[] data = remoteDataRecordStore.getRecord(recordId);

			return Theater.deserialize(new DataInputStream(new ByteArrayInputStream(data)));
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public int storeTheater(Theater theater, int recordId) throws ApplicationException {
		try {
			ByteArrayOutputStream stream = new ByteArrayOutputStream();
			DataOutputStream dataStream = new DataOutputStream(stream);

			theater.serialize(dataStream);
			dataStream.flush();

			if (recordId > 0) {
				remoteDataRecordStore.setRecord(recordId, stream.toByteArray(), 0, stream.size());
			} else {
				recordId = remoteDataRecordStore.addRecord(stream.toByteArray(), 0, stream.size());
			}

			return recordId;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public void deleteTheater(int recordId) throws ApplicationException {
		delete(remoteDataRecordStore, recordId);

		return;
	}

	public TheaterSchedule loadTheaterSchedule(int recordId) throws ApplicationException {
		try {
			byte[] data = remoteDataRecordStore.getRecord(recordId);

			return TheaterSchedule.deserialize(new DataInputStream(new ByteArrayInputStream(data)));
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public int storeTheaterSchedule(TheaterSchedule theaterSchedule, int recordId)
			throws ApplicationException {
		try {
			ByteArrayOutputStream stream = new ByteArrayOutputStream();
			DataOutputStream dataStream = new DataOutputStream(stream);

			theaterSchedule.serialize(dataStream, false); // Movies are
			// not serialized as part of the theater schedule
			dataStream.flush();

			if (recordId > 0) {
				remoteDataRecordStore.setRecord(recordId, stream.toByteArray(), 0, stream.size());
			} else {
				recordId = remoteDataRecordStore.addRecord(stream.toByteArray(), 0, stream.size());
			}

			return recordId;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public void deleteTheaterSchedule(int recordId) throws ApplicationException {
		delete(remoteDataRecordStore, recordId);

		return;
	}

	public Movie loadMovie(int recordId) throws ApplicationException {
		try {
			byte[] data = remoteDataRecordStore.getRecord(recordId);

			return Movie.deserialize(new DataInputStream(new ByteArrayInputStream(data)));
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public int storeMovie(Movie movie, int recordId) throws ApplicationException {
		try {
			ByteArrayOutputStream stream = new ByteArrayOutputStream();
			DataOutputStream dataStream = new DataOutputStream(stream);

			movie.serialize(dataStream);
			dataStream.flush();

			if (recordId > 0) {
				remoteDataRecordStore.setRecord(recordId, stream.toByteArray(), 0, stream.size());
			} else {
				recordId = remoteDataRecordStore.addRecord(stream.toByteArray(), 0, stream.size());
			}

			return recordId;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public void deleteMovie(int recordId) throws ApplicationException {
		delete(remoteDataRecordStore, recordId);

		return;
	}

	public MovieRating loadMovieRating(int recordId) throws ApplicationException {
		try {
			byte[] data = remoteDataRecordStore.getRecord(recordId);

			return MovieRating.deserialize(new DataInputStream(new ByteArrayInputStream(data)));
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public int storeMovieRating(MovieRating movieRating, int recordId) throws ApplicationException {
		try {
			ByteArrayOutputStream stream = new ByteArrayOutputStream();
			DataOutputStream dataStream = new DataOutputStream(stream);

			movieRating.serialize(dataStream);
			dataStream.flush();

			if (recordId > 0) {
				remoteDataRecordStore.setRecord(recordId, stream.toByteArray(), 0, stream.size());
			} else {
				recordId = remoteDataRecordStore.addRecord(stream.toByteArray(), 0, stream.size());
			}

			return recordId;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public void deleteMovieRating(int recordId) throws ApplicationException {
		delete(remoteDataRecordStore, recordId);

		return;
	}

	public Preferences loadPreferences(int recordId) throws ApplicationException {
		try {
			byte[] data = localDataRecordStore.getRecord(recordId);

			return Preferences.deserialize(new DataInputStream(new ByteArrayInputStream(data)));
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public int storePreferences(Preferences preferences, int recordId) throws ApplicationException {
		try {
			ByteArrayOutputStream stream = new ByteArrayOutputStream();
			DataOutputStream dataStream = new DataOutputStream(stream);

			preferences.serialize(dataStream);
			dataStream.flush();

			if (recordId > 0) {
				localDataRecordStore.setRecord(recordId, stream.toByteArray(), 0, stream.size());
			} else {
				recordId = localDataRecordStore.addRecord(stream.toByteArray(), 0, stream.size());
			}

			return recordId;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public SyncAnchor loadSyncAnchor(int recordId) throws ApplicationException {
		try {
			byte[] data = localDataRecordStore.getRecord(recordId);

			return SyncAnchor.deserialize(new DataInputStream(new ByteArrayInputStream(data)));
		} catch (RecordStoreException rse) {
			throw new ApplicationException(rse);
		}
	}

	public int storeSyncAnchor(SyncAnchor syncAnchor, int recordId) throws ApplicationException {
		try {
			ByteArrayOutputStream stream = new ByteArrayOutputStream();
			DataOutputStream dataStream = new DataOutputStream(stream);

			syncAnchor.serialize(dataStream);
			dataStream.flush();

			if (recordId > 0) {
				localDataRecordStore.setRecord(recordId, stream.toByteArray(), 0, stream.size());
			} else {
				recordId = localDataRecordStore.addRecord(stream.toByteArray(), 0, stream.size());
			}

			return recordId;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public AccountInfo loadAccountInfo(int recordId) throws ApplicationException {
		try {
			byte[] data = localDataRecordStore.getRecord(recordId);

			return AccountInfo.deserialize(new DataInputStream(new ByteArrayInputStream(data)));
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public int storeAccountInfo(AccountInfo accountInfo, int recordId) throws ApplicationException {
		try {
			ByteArrayOutputStream stream = new ByteArrayOutputStream();
			DataOutputStream dataStream = new DataOutputStream(stream);

			accountInfo.serialize(dataStream);
			dataStream.flush();

			if (recordId > 0) {
				localDataRecordStore.setRecord(recordId, stream.toByteArray(), 0, stream.size());
			} else {
				recordId = localDataRecordStore.addRecord(stream.toByteArray(), 0, stream.size());
			}

			return recordId;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		} catch (RecordStoreException rse) {
			rse.printStackTrace();

			throw new ApplicationException(rse);
		}
	}

	public IndexedResourceBundle loadResourceBundle(int recordId) throws ApplicationException {
		try {
			byte[] data = localDataRecordStore.getRecord(recordId);

			return IndexedResourceBundle.deserialize(new DataInputStream(new ByteArrayInputStream(
					data)));
		} catch (RecordStoreException rse) {
			throw new ApplicationException(rse);
		}
	}

	public int storeResourceBundle(IndexedResourceBundle bundle, int recordId)
			throws ApplicationException {
		try {
			ByteArrayOutputStream stream = new ByteArrayOutputStream();
			DataOutputStream dataStream = new DataOutputStream(stream);

			bundle.serialize(dataStream);
			dataStream.flush();

			if (recordId > 0) {
				localDataRecordStore.setRecord(recordId, stream.toByteArray(), 0, stream.size());
			} else {
				recordId = localDataRecordStore.addRecord(stream.toByteArray(), 0, stream.size());
			}

			return recordId;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		} catch (RecordStoreException rse) {
			throw new ApplicationException(rse);
		}
	}

}
