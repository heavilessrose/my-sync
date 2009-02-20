package winkCC.core.rms;

import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import javax.microedition.rms.RecordStoreFullException;
import javax.microedition.rms.RecordStoreNotFoundException;

import winkCC.core.log.ILog;
import winkCC.core.log.LogFactory;

public class RMS {

	public static RecordStore open(String name, boolean autoCreat) {
		ILog log = LogFactory.getLog("RMS");

		try {
			return RecordStore.openRecordStore(name, autoCreat);
		} catch (RecordStoreFullException e) {
			log.error("RecordStoreFullException", e);
			return null;
		} catch (RecordStoreNotFoundException e) {
			log.error("RecordStoreNotFoundException", e);
			return null;
		} catch (RecordStoreException e) {
			log.error("RecordStoreException", e);
			return null;
		}
	}
}
