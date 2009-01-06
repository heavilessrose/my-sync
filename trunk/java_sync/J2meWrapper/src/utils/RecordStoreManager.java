package utils;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.lang.String;
import java.util.Hashtable;

import javax.microedition.rms.InvalidRecordIDException;
import javax.microedition.rms.RecordEnumeration;
import javax.microedition.rms.RecordListener;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import javax.microedition.rms.RecordStoreNotOpenException;

public class RecordStoreManager {
    private static final byte BOOLEAN = 1;
    private static final byte CHAR = 2;
    private static final byte INT = 3;
    private static final byte STRING = 4;
    private static final byte DOUBLE = 5;
    private static final byte FLOAT = 6;
    private static final byte LONG = 7;
    private static final byte BYTE = 8;
    private static final byte SHORT = 9;
    private static final byte BYTEARRAY = 10;

    private static String m_recordStoreName = null;    // the record store name associated with rsm
    private static ByteArrayOutputStream m_baos;        // used in saving records to record store
    private static DataOutputStream m_os;                        // used in saving records to record store
    private static DataInputStream m_is;                        // used in retrieving records from record store
    private static Hashtable m_rsHash;                            // saves variable name/record id mapping
    private static RecordStore m_recordStore;                // instance of record store
    private static boolean rsmLoaded = false;                // specifies that the record store has been loaded
    // (loadRecordStore has run)

    /**
     * @param recordStoreName
     * @throws RecordStoreException
     * @throws IOException          Opens and instantiates (if necessary) the record store, then calls initRecordIDVarNameMapping
     *                              to initialize variable name/record id hashtable
     */
    public static void loadRecordStore( String recordStoreName ) throws RecordStoreException, IOException {
        // Set recordstore name
        m_recordStoreName = recordStoreName;

        // Open the recordstore
        m_recordStore = RecordStore.openRecordStore( m_recordStoreName, true );

        // Load recordstore into memory
        initRecordIDVarNameMapping();

        // This indicates that the record store has been loaded
        rsmLoaded = true;
    }

    /**
     * Reads all of the records from the recordstore and stores
     * variable name/record id in a hashtable
     *
     * @throws RecordStoreException
     * @throws IOException
     */
    public static void initRecordIDVarNameMapping() throws RecordStoreException, IOException {
        // instantiate hash table
        m_rsHash = new Hashtable();

        // get enumeration object (we'll parse through this to get the record store record ids)
        RecordEnumeration recordEnum = m_recordStore.enumerateRecords( null, null, false );

        // will store record data in this array
        byte[] rsBytes = null;

        // get record ids from enumeration and extract variable names from records
        while (recordEnum.hasNextElement()) {
            int recordID = recordEnum.nextRecordId();

            // read in record @ recordID into byte array
            rsBytes = m_recordStore.getRecord( recordID );

            // instantiate data input stream
            m_is = new DataInputStream( new ByteArrayInputStream( rsBytes ) );

            // skip past variable type field
            m_is.skipBytes( 1 );

            // next field is the variable name
            String varName = m_is.readUTF();

            // need an Integer representation of the recordID so we can cast as an Object
            Integer p = new Integer( recordID );

            // add variable name/record id to the hash table
            m_rsHash.put( (Object) varName, (Object) p );

            m_is.close();
        }

        // we're done with the enumeration...destroy it
        recordEnum.destroy();
    }


    // ######################
    // Load functions
    // ######################
    /**
     * Retrieves a boolean variable from the recordstore
     *
     * @param varName
     * @return
     * @throws RecordStoreException
     * @throws IOException
     */
    public static boolean loadBooleanVar( String varName ) throws RecordStoreException, IOException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else if( !m_rsHash.containsKey( (Object) varName ) )
            throw new RMSException( "Variable not found in record store." );
        else {
            boolean recordValue = false;

            // read in record
            byte[] rsBytes = m_recordStore.getRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue() );

            // instantiate data input stream
            m_is = new DataInputStream( new ByteArrayInputStream( rsBytes ) );

            // first byte is the stored variable's type
            byte variableType = m_is.readByte();
            if( variableType != BOOLEAN )
                throw new RMSException( "Record store data type mismatch - variable: \"" + varName + "\"" );

            // skip past variable name
            m_is.readUTF();

            // read in the record value
            recordValue = m_is.readBoolean();

            m_is.close();

            return recordValue;
        }
    }

    /**
     * Retrieves a char variable from the recordstore
     *
     * @param varName
     * @return
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */
    public static char loadCharVar( String varName ) throws RecordStoreException, IOException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else if( !m_rsHash.containsKey( (Object) varName ) )
            throw new RMSException( "Variable not found in record store." );
        else {
            char recordValue;

            // read in record
            byte[] rsBytes = m_recordStore.getRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue() );

            // instantiate data input stream
            m_is = new DataInputStream( new ByteArrayInputStream( rsBytes ) );

            // first byte is the stored variable's type
            byte variableType = m_is.readByte();
            if( variableType != CHAR )
                throw new RMSException( "Record store data type mismatch - variable: \"" + varName + "\"" );

            // skip past variable name
            m_is.readUTF();

            // read in the record value
            recordValue = m_is.readChar();

            m_is.close();

            return recordValue;
        }
    }

    /**
     * Retrieves an int variable from the recordstore
     *
     * @param varName
     * @return
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */
    public static int loadIntVar( String varName ) throws RecordStoreException, IOException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else if( !m_rsHash.containsKey( (Object) varName ) )
            throw new RMSException( "Variable not found in record store." );
        else {
            int recordValue = 0;

            // read in record
            byte[] rsBytes = m_recordStore.getRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue() );

            // instantiate data input stream
            m_is = new DataInputStream( new ByteArrayInputStream( rsBytes ) );

            // first byte is the stored variable's type
            byte variableType = m_is.readByte();
            if( variableType != INT )
                throw new RMSException( "Record store data type mismatch - variable: \"" + varName + "\"" );

            // skip past variable name
            m_is.readUTF();

            // read in the record value
            recordValue = m_is.readInt();

            m_is.close();

            return recordValue;
        }
    }

    /**
     * Retrieves a string variable from the recordstore
     *
     * @param varName
     * @return
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */
    public static String loadStringVar( String varName ) throws RecordStoreException, IOException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else if( !m_rsHash.containsKey( (Object) varName ) )
            throw new RMSException( "Variable not found in record store." );
        else {
            String recordValue = null;

            // read in record
            byte[] rsBytes = m_recordStore.getRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue() );

            // instantiate data input stream
            m_is = new DataInputStream( new ByteArrayInputStream( rsBytes ) );

            // first byte is the stored variable's type
            byte variableType = m_is.readByte();
            if( variableType != STRING )
                throw new RMSException( "Record store data type mismatch - variable: \"" + varName + "\"" );

            // skip past variable name
            m_is.readUTF();

            // read in the record value
            recordValue = m_is.readUTF();

            m_is.close();

            return recordValue;
        }
    }
    /**
     * Retrieves a double variable from the recordstore
     * @param varName
     * @return
     * @throws RecordStoreException
     * @throws IOException
     * @throws RSMException
     */
/*
	public static double loadDoubleVar(String varName) throws RecordStoreException, IOException, RSMException
	{
		if (!rsmLoaded)
			throw new RSMException("Record store manager has not been loaded.");
		else if (!m_rsHash.containsKey((Object)varName))
			throw new RSMException("Variable not found in record store.");
		else
		{
			double recordValue = 0;

			// read in record
			byte[] rsBytes = m_recordStore.getRecord(((Integer)m_rsHash.get((Object)varName)).intValue());

			// instantiate data input stream
			m_is = new DataInputStream(new ByteArrayInputStream(rsBytes));

			// first byte is the stored variable's type
			byte variableType = m_is.readByte();
			if (variableType != DOUBLE)
				throw new RSMException("Record store data type mismatch - variable: \"" + varName + "\"");

			// skip past variable name
			m_is.readUTF();

			// read in the record value
			recordValue = m_is.readDouble();

			m_is.close();

			return recordValue;
		}
	}
	*/
    /**
     * Retrieves a float variable from the recordstore
     * @param varName
     * @return
     * @throws RecordStoreException
     * @throws IOException
     * @throws RSMException
     */
    /*
     public static float loadFloatVar(String varName) throws RecordStoreException, IOException, RSMException
     {
         if (!rsmLoaded)
             throw new RSMException("Record store manager has not been loaded.");
         else if (!m_rsHash.containsKey((Object)varName))
             throw new RSMException("Variable not found in record store.");
         else
         {
             float recordValue = 0;

             // read in record
             byte[] rsBytes = m_recordStore.getRecord(((Integer)m_rsHash.get((Object)varName)).intValue());

             // instantiate data input stream
             m_is = new DataInputStream(new ByteArrayInputStream(rsBytes));

             // first byte is the stored variable's type
             byte variableType = m_is.readByte();
             if (variableType != FLOAT)
                 throw new RSMException("Record store data type mismatch - variable: \"" + varName + "\"");

             // skip past variable name
             m_is.readUTF();

             // read in the record value
             recordValue = m_is.readFloat();

             m_is.close();

             return recordValue;
         }
     }

     */
    /**
     * Retrieves a long variable from the recordstore
     *
     * @param varName
     * @return
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */
    public static long loadLongVar( String varName ) throws RecordStoreException, IOException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else if( !m_rsHash.containsKey( (Object) varName ) )
            throw new RMSException( "Variable not found in record store." );
        else {
            long recordValue = 0;

            // read in record
            byte[] rsBytes = m_recordStore.getRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue() );

            // instantiate data input stream
            m_is = new DataInputStream( new ByteArrayInputStream( rsBytes ) );

            // first byte is the stored variable's type
            byte variableType = m_is.readByte();
            if( variableType != LONG )
                throw new RMSException( "Record store data type mismatch - variable: \"" + varName + "\"" );

            // skip past variable name
            m_is.readUTF();

            // read in the record value
            recordValue = m_is.readLong();

            m_is.close();

            return recordValue;
        }
    }

    /**
     * Retrieves a byte variable from the recordstore
     *
     * @param varName
     * @return
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */
    public static byte loadByteVar( String varName ) throws RecordStoreException, IOException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else if( !m_rsHash.containsKey( (Object) varName ) )
            throw new RMSException( "Variable not found in record store." );
        else {
            byte recordValue = 0;

            // read in record
            byte[] rsBytes = m_recordStore.getRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue() );

            // instantiate data input stream
            m_is = new DataInputStream( new ByteArrayInputStream( rsBytes ) );

            // first byte is the stored variable's type
            byte variableType = m_is.readByte();
            if( variableType != BYTE )
                throw new RMSException( "Record store data type mismatch - variable: \"" + varName + "\"" );

            // skip past variable name
            m_is.readUTF();

            // read in the record value
            recordValue = m_is.readByte();

            m_is.close();

            return recordValue;
        }
    }

    /**
     * Retrieves a byte array variable from the recordstore
     *
     * @param varName
     * @return
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */
    public static byte[] loadByteArrayVar( String varName ) throws RecordStoreException, IOException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else if( !m_rsHash.containsKey( (Object) varName ) )
            throw new RMSException( "Variable not found in record store." );
        else {
            byte[] recordValue;

            // read in record
            byte[] rsBytes = m_recordStore.getRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue() );

            // instantiate data input stream
            m_is = new DataInputStream( new ByteArrayInputStream( rsBytes ) );

            // first byte is the stored variable's type
            byte variableType = m_is.readByte();
            if( variableType != BYTEARRAY )
                throw new RMSException( "Record store data type mismatch - variable: \"" + varName + "\"" );

            // get the variable name
            String tmpStr = m_is.readUTF();

            // this is the size of the record
            int recordSize = m_recordStore.getRecordSize( ((Integer) m_rsHash.get( (Object) varName )).intValue() );

            // get an array of bytes representing the string
            byte[] tmpStrBytes = tmpStr.getBytes();

            // get the size of the data (in bytes)
            int sizeOfData = recordSize - 1 - tmpStrBytes.length - 2;

            recordValue = new byte[sizeOfData];

            // read in the record value
            m_is.read( recordValue );

            m_is.close();

            return recordValue;
        }
    }

    /**
     * Retrieves a short variable from the recordstore
     *
     * @param varName
     * @return
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */
    public static short loadShortVar( String varName ) throws RecordStoreException, IOException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else if( !m_rsHash.containsKey( (Object) varName ) )
            throw new RMSException( "Variable not found in record store." );
        else {
            short recordValue = 0;

            // read in record
            byte[] rsBytes = m_recordStore.getRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue() );

            // instantiate data input stream
            m_is = new DataInputStream( new ByteArrayInputStream( rsBytes ) );

            // first byte is the stored variable's type
            byte variableType = m_is.readByte();
            if( variableType != SHORT )
                throw new RMSException( "Record store data type mismatch - variable: \"" + varName + "\"" );

            // skip past variable name
            m_is.readUTF();

            // read in the record value
            recordValue = m_is.readShort();

            m_is.close();

            return recordValue;
        }
    }


    // ######################
    // Save functions
    // ######################
    /**
     * Saves a boolean variable to the recordstore
     *
     * @param varName
     * @param val
     * @return
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */
    public static void saveBooleanVar( String varName, boolean val ) throws RecordStoreException, IOException, RMSException {
        if( rsmLoaded ) {
            m_baos = new ByteArrayOutputStream();
            m_os = new DataOutputStream( m_baos );

            // Write record to output stream
            m_os.writeByte( BOOLEAN );
            m_os.writeUTF( varName );
            m_os.writeBoolean( val );
            m_os.close();

            // Get the bytes that were written to the byte array stream
            byte[] recordBytes = m_baos.toByteArray();

            // If this variable is in the hash table, set the record
            if( m_rsHash.containsKey( (Object) varName ) )
                m_recordStore.setRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue(), recordBytes, 0, recordBytes.length );
            else    // Add new record
            {
                Integer newRecordID = new Integer( m_recordStore.addRecord( recordBytes, 0, recordBytes.length ) );
                m_rsHash.put( (Object) varName, (Object) newRecordID );
            }
            m_baos.close();
        } else
            throw new RMSException( "Record store manager has not been loaded." );
    }

    /**
     * Saves a char variable to the recordstore
     *
     * @param varName
     * @param val
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */
    public static void saveCharVar( String varName, char val ) throws RecordStoreException, IOException, RMSException {
        if( rsmLoaded ) {
            m_baos = new ByteArrayOutputStream();
            m_os = new DataOutputStream( m_baos );

            // Write record to output stream
            m_os.writeByte( CHAR );
            m_os.writeUTF( varName );
            m_os.writeChar( val );
            m_os.close();

            // Get the bytes that were written to the byte array stream
            byte[] recordBytes = m_baos.toByteArray();

            // If this variable is in the hash table, set the record
            if( m_rsHash.containsKey( (Object) varName ) )
                m_recordStore.setRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue(), recordBytes, 0, recordBytes.length );
            else    // Add new record
            {
                Integer newRecordID = new Integer( m_recordStore.addRecord( recordBytes, 0, recordBytes.length ) );
                m_rsHash.put( (Object) varName, (Object) newRecordID );
            }
            m_baos.close();
        } else
            throw new RMSException( "Record store manager has not been loaded." );
    }

    /**
     * Saves an int variable to the recordstore
     *
     * @param varName
     * @param val
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */
    public static void saveIntVar( String varName, int val ) throws RecordStoreException, IOException, RMSException {
        if( rsmLoaded ) {
            m_baos = new ByteArrayOutputStream();
            m_os = new DataOutputStream( m_baos );

            // Write record to output stream
            m_os.writeByte( INT );
            m_os.writeUTF( varName );
            m_os.writeInt( val );
            m_os.close();

            // Get the bytes that were written to the byte array stream
            byte[] recordBytes = m_baos.toByteArray();

            // If this variable is in the hash table, set the record
            if( m_rsHash.containsKey( (Object) varName ) )
                m_recordStore.setRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue(), recordBytes, 0, recordBytes.length );
            else    // Add new record
            {
                Integer newRecordID = new Integer( m_recordStore.addRecord( recordBytes, 0, recordBytes.length ) );
                m_rsHash.put( (Object) varName, (Object) newRecordID );
            }
            m_baos.close();
        } else
            throw new RMSException( "Record store manager has not been loaded." );
    }

    /**
     * Saves a string variable to the recordstore
     *
     * @param varName
     * @param val
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */

    public static void saveStringVar( String varName, String val ) throws RecordStoreException, IOException, RMSException {
        if( rsmLoaded ) {
            m_baos = new ByteArrayOutputStream();
            m_os = new DataOutputStream( m_baos );

            // Write record to output stream
            m_os.writeByte( STRING );
            m_os.writeUTF( varName );
            m_os.writeUTF( val );
            m_os.close();

            // Get the bytes that were written to the byte array stream
            byte[] recordBytes = m_baos.toByteArray();

            // If this variable is in the hash table, set the record
            if( m_rsHash.containsKey( (Object) varName ) )
                m_recordStore.setRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue(), recordBytes, 0, recordBytes.length );
            else    // Add new record
            {
                Integer newRecordID = new Integer( m_recordStore.addRecord( recordBytes, 0, recordBytes.length ) );
                m_rsHash.put( (Object) varName, (Object) newRecordID );
            }
            m_baos.close();
        } else
            throw new RMSException( "Record store manager has not been loaded." );
    }
    /**
     * Saves a double variable to the recordstore
     * @param varName
     * @param val
     * @throws RecordStoreException
     * @throws IOException
     * @throws RSMException
     */
/*
	public static void saveDoubleVar(String varName, double val) throws RecordStoreException, IOException, RSMException
	{
		if (rsmLoaded)
		{
			m_baos = new ByteArrayOutputStream();
			m_os = new DataOutputStream(m_baos);

			// Write record to output stream
			m_os.writeByte(DOUBLE);
			m_os.writeUTF(varName);
			m_os.writeDouble(val);
			m_os.close();

			// Get the bytes that were written to the byte array stream
			byte[] recordBytes = m_baos.toByteArray();

			// If this variable is in the hash table, set the record
			if (m_rsHash.containsKey((Object)varName))
				m_recordStore.setRecord(((Integer)m_rsHash.get((Object)varName)).intValue(), recordBytes, 0, recordBytes.length);
			else	// Add new record
			{
				Integer newRecordID = new Integer(m_recordStore.addRecord(recordBytes, 0, recordBytes.length));
				m_rsHash.put((Object)varName, (Object)newRecordID);
			}
			m_baos.close();
		}
		else
			throw new RSMException("Record store manager has not been loaded.");
	}
	*/
    /**
     * Saves a float variable to the recordstore
     * @param varName
     * @param val
     * @throws RecordStoreException
     * @throws IOException
     * @throws RSMException
     */
    /*
     public static void saveFloatVar(String varName, float val) throws RecordStoreException, IOException, RSMException
     {
         if (rsmLoaded)
         {
             m_baos = new ByteArrayOutputStream();
             m_os = new DataOutputStream(m_baos);

             // Write record to output stream
             m_os.writeByte(FLOAT);
             m_os.writeUTF(varName);
             m_os.writeFloat(val);
             m_os.close();

             // Get the bytes that were written to the byte array stream
             byte[] recordBytes = m_baos.toByteArray();

             // If this variable is in the hash table, set the record
             if (m_rsHash.containsKey((Object)varName))
                 m_recordStore.setRecord(((Integer)m_rsHash.get((Object)varName)).intValue(), recordBytes, 0, recordBytes.length);
             else	// Add new record
             {
                 Integer newRecordID = new Integer(m_recordStore.addRecord(recordBytes, 0, recordBytes.length));
                 m_rsHash.put((Object)varName, (Object)newRecordID);
             }
             m_baos.close();
         }
         else
             throw new RSMException("Record store manager has not been loaded.");
     }

     */
    /**
     * Saves a long variable to the recordstore
     *
     * @param varName
     * @param val
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */

    public static void saveLongVar( String varName, long val ) throws RecordStoreException, IOException, RMSException {
        if( rsmLoaded ) {
            m_baos = new ByteArrayOutputStream();
            m_os = new DataOutputStream( m_baos );

            // Write record to output stream
            m_os.writeByte( LONG );
            m_os.writeUTF( varName );
            m_os.writeLong( val );
            m_os.close();

            // Get the bytes that were written to the byte array stream
            byte[] recordBytes = m_baos.toByteArray();

            // If this variable is in the hash table, set the record
            if( m_rsHash.containsKey( (Object) varName ) )
                m_recordStore.setRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue(), recordBytes, 0, recordBytes.length );
            else    // Add new record
            {
                Integer newRecordID = new Integer( m_recordStore.addRecord( recordBytes, 0, recordBytes.length ) );
                m_rsHash.put( (Object) varName, (Object) newRecordID );
            }
            m_baos.close();
        } else
            throw new RMSException( "Record store manager has not been loaded." );
    }

    /**
     * Saves a byte variable to the recordstore
     *
     * @param varName
     * @param val
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */
    public static void saveByteVar( String varName, byte val ) throws RecordStoreException, IOException, RMSException {
        if( rsmLoaded ) {
            m_baos = new ByteArrayOutputStream();
            m_os = new DataOutputStream( m_baos );

            // Write record to output stream
            m_os.writeByte( BYTE );
            m_os.writeUTF( varName );
            m_os.writeByte( val );
            m_os.close();

            // Get the bytes that were written to the byte array stream
            byte[] recordBytes = m_baos.toByteArray();

            // If this variable is in the hash table, set the record
            if( m_rsHash.containsKey( (Object) varName ) )
                m_recordStore.setRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue(), recordBytes, 0, recordBytes.length );
            else    // Add new record
            {
                Integer newRecordID = new Integer( m_recordStore.addRecord( recordBytes, 0, recordBytes.length ) );
                m_rsHash.put( (Object) varName, (Object) newRecordID );
            }
            m_baos.close();
        } else
            throw new RMSException( "Record store manager has not been loaded." );
    }

    /**
     * Saves a short variable to the recordstore
     *
     * @param varName
     * @param val
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */
    public static void saveShortVar( String varName, short val ) throws RecordStoreException, IOException, RMSException {
        if( rsmLoaded ) {
            m_baos = new ByteArrayOutputStream();
            m_os = new DataOutputStream( m_baos );

            // Write record to output stream
            m_os.writeByte( SHORT );
            m_os.writeUTF( varName );
            m_os.writeShort( val );
            m_os.close();

            // Get the bytes that were written to the byte array stream
            byte[] recordBytes = m_baos.toByteArray();

            // If this variable is in the hash table, set the record
            if( m_rsHash.containsKey( (Object) varName ) )
                m_recordStore.setRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue(), recordBytes, 0, recordBytes.length );
            else    // Add new record
            {
                Integer newRecordID = new Integer( m_recordStore.addRecord( recordBytes, 0, recordBytes.length ) );
                m_rsHash.put( (Object) varName, (Object) newRecordID );
            }
            m_baos.close();
        } else
            throw new RMSException( "Record store manager has not been loaded." );
    }

    /**
     * Saves a byte array variable to the recordstore
     *
     * @param varName
     * @param val
     * @param off
     * @param len
     * @throws RecordStoreException
     * @throws IOException
     * @throws RMSException
     */
    public static void saveByteVar( String varName, byte[] val, int off, int len ) throws RecordStoreException, IOException, RMSException {
        if( rsmLoaded ) {
            m_baos = new ByteArrayOutputStream();
            m_os = new DataOutputStream( m_baos );

            // Write record to output stream
            m_os.writeByte( BYTEARRAY );
            m_os.writeUTF( varName );
            m_os.write( val, off, len );
            m_os.close();

            // Get the bytes that were written to the byte array stream
            byte[] recordBytes = m_baos.toByteArray();

            // If this variable is in the hash table, set the record
            if( m_rsHash.containsKey( (Object) varName ) )
                m_recordStore.setRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue(), recordBytes, 0, recordBytes.length );
            else    // Add new record
            {
                Integer newRecordID = new Integer( m_recordStore.addRecord( recordBytes, 0, recordBytes.length ) );
                m_rsHash.put( (Object) varName, (Object) newRecordID );
            }
            m_baos.close();
        } else
            throw new RMSException( "Record store manager has not been loaded." );
    }

    /**
     * Deletes a record from the record store (based on varName) and
     * removes variable from the hash table
     *
     * @param varName
     * @throws RecordStoreNotOpenException
     * @throws InvalidRecordIDException
     * @throws RecordStoreException
     * @throws RMSException
     */
    public static void deleteVar( String varName ) throws RecordStoreNotOpenException, InvalidRecordIDException, RecordStoreException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else if( !m_rsHash.containsKey( (Object) varName ) )
            throw new RMSException( "Variable not found in record store." );
        else {
            m_recordStore.deleteRecord( ((Integer) m_rsHash.get( (Object) varName )).intValue() );
            m_rsHash.remove( (Object) varName );
        }
    }

    /**
     * Closes the record store and nulls out the hash table
     *
     * @throws RecordStoreException
     * @throws RMSException
     */
    public static void closeRecordStore() throws RecordStoreException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else {
            m_recordStore.closeRecordStore();
            rsmLoaded = false;
            m_rsHash = null;
        }
    }

    /**
     * Gets the record store name
     *
     * @return
     * @throws RecordStoreException
     * @throws RMSException
     */
    public static String getName() throws RecordStoreException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else
            return m_recordStoreName;
    }

    /**
     * Gets the time the record store was last modified
     *
     * @return
     * @throws RecordStoreException
     * @throws RMSException
     */
    public static long getLastModified() throws RecordStoreException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else
            return m_recordStore.getLastModified();
    }

    /**
     * Gets the number of records in the record store
     *
     * @return
     * @throws RecordStoreException
     * @throws RMSException
     */
    public static int getNumRecords() throws RecordStoreException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else
            return m_recordStore.getNumRecords();
    }

    /**
     * Gets the size of the record store
     *
     * @return
     * @throws RecordStoreException
     * @throws RMSException
     */
    public static int getSize() throws RecordStoreException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else
            //return m_recordStore.getSize();
            return 0;
        // TODO fix me
    }

    /**
     * Gets the size available for the record store
     *
     * @return
     * @throws RecordStoreException
     * @throws RMSException
     */
    public static int getSizeAvailable() throws RecordStoreException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else
            return m_recordStore.getSizeAvailable();
    }

    /**
     * Gets the current version for this record store
     *
     * @return
     * @throws RecordStoreException
     * @throws RMSException
     */
    public static int getVersion() throws RecordStoreException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else
            return m_recordStore.getVersion();
    }

    /**
     * Lists the record stores owned by the MIDlet suite
     *
     * @return
     * @throws RecordStoreException
     */
    public static String[] listRecordStores() throws RecordStoreException {
        return RecordStore.listRecordStores();
    }

    /**
     * Removes the record listener
     *
     * @param listener
     * @throws RecordStoreException
     * @throws RMSException
     */
    public static void removeRecordListener( RecordListener listener ) throws RecordStoreException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        else
            m_recordStore.removeRecordListener( listener );
    }

    /**
     * Changes the access mode for this RecordStore
     *
     * @param authmode
     * @param writable
     * @throws RecordStoreException
     * @throws RMSException
     */
    public static void setMode( int authmode, boolean writable ) throws RecordStoreException, RMSException {
        if( !rsmLoaded )
            throw new RMSException( "Record store manager has not been loaded." );
        //else
        //m_recordStore.setMode(authmode, writable);
        // TODO fix me
    }

    /**
     * Deletes the record store given recordStoreName
     *
     * @param recordStoreName
     * @throws RecordStoreException
     */
    public static void deleteRecordStore( String recordStoreName ) throws RecordStoreException {
        // If the record store being deleted is the one associated with the record manager, reinitialize
		if (recordStoreName == m_recordStoreName){
			rsmLoaded = false;
			m_rsHash = null;
		}

		RecordStore.deleteRecordStore(recordStoreName);
	}

	/**
     * Checks the record store to see if the specified record exists
     *
     * @param recordName Name of the record
     * @return true if it exists, false otherwise
     * @throws RMSException
     */
	public static boolean contains(String recordName) throws RMSException
	{
		if (!rsmLoaded)
			throw new RMSException("Record store manager has not been loaded.");
		else if (m_rsHash.containsKey((Object)recordName))
			return true;
		else
			return false;
	}
}