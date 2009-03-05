package winkCC.core.io;

public interface Serializable {

	String getSerializableClassName();

	void serialize(SerializerOutputStream out) throws SerializationException;
}
