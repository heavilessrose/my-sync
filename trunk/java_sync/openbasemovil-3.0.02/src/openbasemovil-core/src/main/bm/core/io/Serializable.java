package bm.core.io;

/*
 * File Information
 *
 * Created on       : 13-jul-2005 14:00:20
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:34:57 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:4 $
 */

/**
 * An object that can be written and read to/from a stream.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public interface Serializable {
	/**
	 * Get the name of the class to be used for serialization/deserialization of
	 * complex/nested objects.
	 * 
	 * @return class name
	 */
	String getSerializableClassName();

	/**
	 * Write object status to stream.
	 * 
	 * @param out
	 *            output stream
	 * @throws SerializationException
	 *             on errors
	 */
	void serialize(SerializerOutputStream out) throws SerializationException;

	/**
	 * Read object status from stream.
	 * 
	 * @param in
	 *            input stream
	 * @throws SerializationException
	 *             on errors
	 */
	void deserialize(SerializerInputStream in) throws SerializationException;
}
