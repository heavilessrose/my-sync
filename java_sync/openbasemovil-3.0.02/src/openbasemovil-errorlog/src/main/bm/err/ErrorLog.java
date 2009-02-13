package bm.err;
/* -----------------------------------------------------------------------------
    OpenBaseMovil ErrorLog Library
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
import bm.core.J2MEException;
import bm.core.ResourceManager;
import bm.core.event.ProgressEvent;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.core.tools.LocalFormatTools;

import javax.microedition.rms.*;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.util.Date;

/*
 * File Information
 *
 * Created on       : 03-ago-2006 16:54:47
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2006-09-26 08:56:53 +0200 (mar, 26 sep 2006) $
 * Revision         : $Revision: 11 $
 */

/**
 * Keeps track of all errors until they're sent to the server.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 11 $
 */
public class ErrorLog
{
    private static Log log = LogFactory.getLog( "ErrorLog" );
    public static final int MAX_RECORDS = 100;

    private static final String RS_NAME = "sys_error_log";

    private static ByteArrayOutputStream baos = new ByteArrayOutputStream();
    private static DataOutputStream dos = new DataOutputStream( baos );
    private static SerializerOutputStream sos = new SerializerOutputStream( dos );
    private static ProgressEvent pe = new ProgressEvent();

    public static String getErrors()
            throws J2MEException
    {
        final ProgressEvent pe = ErrorLog.pe;
        final StringBuffer buffer = new StringBuffer();
        RecordStore rs = null;
        try
        {
            pe.setAnimate( true );
            pe.setPhase( ResourceManager.getResource( "errorlog.building" ) );
            pe.setMessage( "" );
            pe.dispatch();
            rs = RecordStore.openRecordStore( RS_NAME, true );
            int first = 1;
            int count = rs.getNumRecords();
            if( count > MAX_RECORDS )
            {
                first = count - MAX_RECORDS;
                count = MAX_RECORDS;
            }
            pe.setAnimate( false );
            pe.setMaxValue( new Integer( count ) );
            pe.setCurrentValue( 0 );
            pe.dispatch();
            int index = first;
            for( int i = 0; i < count; i++, index++ )
            {
                final ByteArrayInputStream bais = new ByteArrayInputStream(
                        rs.getRecord( index )
                );
                final DataInputStream dis = new DataInputStream( bais );
                final SerializerInputStream in =
                        new SerializerInputStream( dis );
                buffer.append( in.readString() ).append( '\n' );
                pe.increment();
                pe.dispatch();
            }
            return buffer.toString();
        }
        catch( Exception e )
        {
            log.error( e );
            throw new J2MEException( 0, e );
        }
        finally
        {
            try{ if( rs != null ) rs.closeRecordStore(); }catch( Exception e ){}
        }
    }

    public static void writeErrors( final SerializerOutputStream out )
            throws J2MEException
    {
        final ProgressEvent pe = ErrorLog.pe;
        RecordStore rs = null;
        try
        {
            pe.setAnimate( true );
            pe.setPhase( ResourceManager.getResource( "errorlog.building" ) );
            pe.setMessage( "" );
            pe.dispatch();
            rs = RecordStore.openRecordStore( RS_NAME, true );
            int first = 1;
            int count = rs.getNumRecords();
            if( count > MAX_RECORDS )
            {
                first = count - MAX_RECORDS;
                count = MAX_RECORDS;
            }
            out.writeShort( (short) count );
            pe.setAnimate( false );
            pe.setMaxValue( new Integer( count ) );
            pe.setCurrentValue( 0 );
            pe.dispatch();
            //noinspection MethodCallInLoopCondition
            int index = first;
            for( int i = 0; i < count; i++, index++ )
            {
                out.write( rs.getRecord( index ) );
                pe.increment();
                pe.dispatch();
            }
        }
        catch( Exception e )
        {
            log.error( e );
            throw new J2MEException( 0, e );
        }
        finally
        {
            if( rs != null ) try{ rs.closeRecordStore(); } catch( Exception e ){}
        }
    }

    public static synchronized void addError(
            final String        className,
            final String        method,
            final Object[]      args,
            final String        message,
            final Exception     e
    )
    {
        RecordStore rs = null;
        try
        {
            rs = RecordStore.openRecordStore( RS_NAME, true );
            baos.reset();
            sos.writeString(
                    LocalFormatTools.format( new Date(), "yyyyMMdd hhmmss" ) +
                    ' ' +
                    buildMessage( className, method, args, message, e )
            );
            sos.flush();
            final byte[] data = baos.toByteArray();
            final int recordId = rs.addRecord( data, 0, data.length );
            if( rs.getNumRecords() > MAX_RECORDS )
            {
                rs.deleteRecord( recordId - MAX_RECORDS );
            }
        }
        catch( Exception e1 )
        {
            log.error( e1 );
        }
        finally
        {
            try{ if( rs != null ) rs.closeRecordStore(); }catch( Exception e1 ){}
        }
    }

    private static String buildMessage(
            final String    className,
            final String    method,
            final Object[]  args,
            final String    message,
            final Exception e
    )
    {
        final StringBuffer msg = new StringBuffer();
        msg.append( className )
                .append( '.' ).append( method )
                .append( '(' );
        if( args != null )
        {
            final int length = args.length;
            for( int i = 0; i < length; i++ )
            {
                msg.append( args[i] );
                if( i < length - 1 )
                {
                    msg.append( ',' );
                }
            }
        }
        msg.append( ')' );
        if( message != null )
        {
            msg.append( '\t' ).append( '[' ).append( message ).append( ']' );
        }
        if( e != null )
        {
            msg.append( '\t' ).append(
                    e instanceof J2MEException ?
                    ((J2MEException) e).getTrace() :
                    e.getClass().getName() + ": " + e.getMessage()
            );
        }
        msg.append( "\n" );
        return msg.toString();
    }

    public static synchronized void clear()
    {
        safeCloseRecordStore( RS_NAME );
        try
        {
            RecordStore.deleteRecordStore( RS_NAME );
        }
        catch( RecordStoreNotFoundException e )
        {
        }
        catch( RecordStoreException e )
        {
        }
    }

    private static synchronized void safeCloseRecordStore( final String name )
    {
        final Log log = ErrorLog.log;
        try
        {
            //noinspection RecordStoreOpenedButNotSafelyClosed
            final RecordStore rs = RecordStore.openRecordStore( name, false );
            while( true )
            {
                try
                {
                    rs.closeRecordStore();
                }
                catch( RecordStoreNotOpenException e )
                {
                    break;
                }
                catch ( RecordStoreException e )
                {
                    log.error( e );
                }
            }
        }
        catch( RecordStoreNotFoundException e )
        {
            log.debug( "RecordStore did not exist" );
        }
        catch( RecordStoreException e )
        {
            log.error( e );
        }
    }
}
