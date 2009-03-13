package winkCC.rms.sort_traverse_search;

import java.io.*;

// The contact information for a person

public class Contact {
	private String _firstName;
	private String _lastName;
	private String _phoneNumber;

	public Contact() {
	}

	public Contact(String firstName, String lastName, String phoneNumber) {
		_firstName = firstName;
		_lastName = lastName;
		_phoneNumber = phoneNumber;
	}

	public String getFirstName() {
		return _firstName != null ? _firstName : "";
	}

	public String getLastName() {
		return _lastName != null ? _lastName : "";
	}

	public String getPhoneNumber() {
		return _phoneNumber != null ? _phoneNumber : "";
	}

	public void setFirstName(String name) {
		_firstName = name;
	}

	public void setLastName(String name) {
		_lastName = name;
	}

	public void setPhoneNumber(String number) {
		_phoneNumber = number;
	}

	public void fromByteArray(byte[] data) throws IOException {
		ByteArrayInputStream bin = new ByteArrayInputStream(data);
		DataInputStream din = new DataInputStream(bin);

		fromDataStream(din);
		din.close();
	}

	public byte[] toByteArray() throws IOException {
		ByteArrayOutputStream bout = new ByteArrayOutputStream();
		DataOutputStream dout = new DataOutputStream(bout);

		toDataStream(dout);
		dout.close();

		return bout.toByteArray();
	}

	public void fromDataStream(DataInputStream din) throws IOException {
		_firstName = din.readUTF();
		_lastName = din.readUTF();
		_phoneNumber = din.readUTF();
	}

	public void toDataStream(DataOutputStream dout) throws IOException {
		dout.writeUTF(getFirstName());
		dout.writeUTF(getLastName());
		dout.writeUTF(getPhoneNumber());
	}
}
