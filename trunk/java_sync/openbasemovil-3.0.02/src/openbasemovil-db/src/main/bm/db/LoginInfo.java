package bm.db;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Database Library
    Copyright (C) 2004-2008 Elondra S.L.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.
    If not, see <a href="http://www.gnu.org/licenses">http://www.gnu.org/licenses</a>.
----------------------------------------------------------------------------- */

import bm.core.io.Serializable;
import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.tools.MD5Digest;

import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import javax.microedition.rms.RecordStoreNotFoundException;
import java.io.ByteArrayOutputStream;
import java.io.ByteArrayInputStream;
/*
 * File Information
 *
 * Created on       : 15-may-2007 2:35:42
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Stores login information.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class LoginInfo
        implements Serializable
{
    private static final String RS_NAME = "sys_login_info";

    String  deviceId = "";
    Long    deviceNumber;
    Long    userId;
    String  userCustomId;
    String  userFullName;
    byte[]  password;
    String  companyName;

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.db.LoginInfo";
    }

    /**
     * Write object status to stream.
     *
     * @param out output stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public void serialize( SerializerOutputStream out )
        throws SerializationException
    {
        out.writeByte( (byte) 2 ); // Version
        out.writeString( deviceId );
        out.writeLong( deviceNumber );
        out.writeLong( userId );
        out.writeNullableString( userCustomId );
        out.writeNullableString( userFullName );
        out.writeNullableBlob( password );
        out.writeNullableString( companyName );
    }

    /**
     * Read object status from stream.
     *
     * @param in input stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public void deserialize( SerializerInputStream in )
            throws SerializationException
    {
        final byte version = in.readByte();
        deviceId = in.readString();
        deviceNumber = in.readNullableLong();
        userId = in.readNullableLong();
        userCustomId = in.readNullableString();
        userFullName = in.readNullableString();
        password = in.readNullableBlob();
        if( version > 1 )
        {
            companyName = in.readNullableString();
        }
        else
        {
            companyName = null;
        }
    }

    /**
     * Set the password, creating a digital digest of it (the password itself is
     * not stored).
     * @param password clear text passsword
     */
    public void setPassword( final String password )
    {
        MD5Digest md5 = new MD5Digest();
        final byte[] data = password.getBytes();
        md5.update( data, 0, data.length );
        final byte[] digest = new byte[16];
        md5.doFinal( digest, 0 );
        this.password = digest;
    }

    public synchronized void load()
            throws DBException
    {
        RecordStore rs = null;
        try
        {
            try
            {
                rs = RecordStore.openRecordStore( RS_NAME, false );
                final ByteArrayInputStream bais = new ByteArrayInputStream(
                        rs.getRecord( 1 )
                );
                final SerializerInputStream in = new SerializerInputStream(
                        bais
                );
                deserialize( in );
            }
            catch( RecordStoreNotFoundException e )
            {
            }
        }
        catch( RecordStoreException e )
        {
            throw new DBException( Constants.ERR_LIF_LOAD, e );
        }
        catch( SerializationException e )
        {
            throw new DBException( Constants.ERR_LIF_LOAD, e );
        }
        finally
        {
            if( rs != null ) try{ rs.closeRecordStore(); }catch( Exception e ){}
        }
    }

    public synchronized void save()
            throws DBException
    {
        RecordStore rs = null;
        try
        {
            final ByteArrayOutputStream baos = new ByteArrayOutputStream();
            final SerializerOutputStream out = new SerializerOutputStream( baos );
            serialize( out );
            final byte[] data = baos.toByteArray();
            rs = RecordStore.openRecordStore( RS_NAME, true );
            if( rs.getNumRecords() == 0 )
            {
                rs.addRecord( data, 0, data.length );
            }
            else
            {
                rs.setRecord( 1, data, 0, data.length );
            }
        }
        catch( RecordStoreException e )
        {
            throw new DBException( Constants.ERR_LIF_SAVE, e );
        }
        catch( SerializationException e )
        {
            throw new DBException( Constants.ERR_LIF_SAVE, e );
        }
        finally
        {
            if( rs != null ) try{ rs.closeRecordStore(); }catch( Exception e ){}
        }
    }
}
