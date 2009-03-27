package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class Theater {

	private String primaryKey;
	private String name;
	private String address;
	private String zipCode;

	/**
	 * @link aggregationByValue
	 */
	transient private TheaterSchedule theaterSchedule = null;

	public Theater(String primaryKey, String name, String address, String zipCode) {
		this.primaryKey = primaryKey;
		this.name = name;
		this.address = address;
		this.zipCode = zipCode;

		return;
	}

	private Theater() {
	}

	public String getPrimaryKey() {
		return primaryKey;
	}

	public String getName() {
		return name;
	}

	public String getAddress() {
		return address;
	}

	public String getZipCode() {
		return zipCode;
	}

	public TheaterSchedule getTheaterSchedule() throws ApplicationException {
		if (theaterSchedule == null
				|| theaterSchedule.getExpirationDate() < System.currentTimeMillis()) { // ???
			try {
				theaterSchedule = ModelObjectLoader.getInstance().getTheaterSchedule(this);
			} catch (ModelException me) {
				throw new ApplicationException();
			}
		}

		return theaterSchedule;
	}

	public boolean equals(Object object) {
		return object != null
				&& (object instanceof Theater && (object == this || ((Theater) object)
						.getPrimaryKey().equals(primaryKey)));
	}

	public void serialize(DataOutputStream dataStream) throws ApplicationException {
		try {
			dataStream.writeUTF(zipCode);
			dataStream.writeUTF(primaryKey);
			dataStream.writeUTF(name);
			dataStream.writeUTF(address);

			return;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static Theater deserialize(DataInputStream dataStream) throws ApplicationException {
		try {
			Theater theater = new Theater();

			theater.zipCode = dataStream.readUTF();
			theater.primaryKey = dataStream.readUTF();
			theater.name = dataStream.readUTF();
			theater.address = dataStream.readUTF();

			return theater;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	// Optimization to avoid deserializing and instantiating a Theater when matching RMS records
	public static boolean matches(byte[] data, String zipCode) throws IOException {
		ByteArrayInputStream byteStream = new ByteArrayInputStream(data);
		DataInputStream dataStream = new DataInputStream(byteStream);

		return zipCode.equals(dataStream.readUTF());
	}

}
