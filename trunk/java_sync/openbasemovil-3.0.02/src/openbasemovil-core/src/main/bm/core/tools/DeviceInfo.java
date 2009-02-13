package bm.core.tools;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Core Library, foundation of the OpenBaseMovil database and tools
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
import bm.core.ResourceManager;
import bm.core.Application;
import bm.core.event.ProgressEvent;
import bm.core.io.Serializable;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import bm.core.log.Log;
import bm.core.log.LogFactory;

import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.List;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
/*
 * File Information
 *
 * Created on       : 14-ago-2006 12:23:33
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * Simple device info to allow better on-the-fly decissions for different
 * abbilities, such as compression level to use, buffer sizes, etc.<br/>
 * Uses some code based on:<br/>
 * TastePhone : Taste your MIDP Phone with this test MIDlet.<br>
 * <br>
 * Copyright (c) 2004, Thibaut REGNIER<br>
 * All rights reserved.<br>
 * <br>
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:<br>
 * <br>
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.<br>
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation and/or other materials
 *     provided with the distribution.<br>
 *   * Neither the name of the "Club des Utilisateurs de Java" nor the names of its contributors may be used to endorse
 *     or promote products derived from this software without specific prior written permission.<br>
 * <br>
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class DeviceInfo
    implements Serializable
{
    private static final Log log = LogFactory.getLog( "DeviceInfo" );

    private static final int SPEEDARRAY_SIZE= 1024 / 4;
    private static final String RSNAME1 = "_DEV_INFO_1_TMP_";
    private static final String RSNAME2 = "_DEV_INFO_2_TMP_";

    private static final String[] JP7 = new String[] {
            "K610", "K618", "V630", "W710", "Z610", "Z710",
            "K550", "K790", "K800", "K810", "S500", "T650",
            "W580", "W610", "W618", "W660", "W830", "W850",
            "W880", "W888"
    };

    public static final int AH_UNKNOWN                  = -1;
    public static final int AH_OK                       = 0;
    public static final int AH_FREES_ON_RECORD_DELETE   = 1;
    public static final int AH_FREES_ON_RS_DELETE       = 2;
    public static final int AH_BOGUS                    = 3;

    public static final int RS_LIMIT_UNKOWN             = -2;
    public static final int RS_LIMIT_NONE               = -1;

    private static DeviceInfo   deviceInfo;
    private static long startHeapSize = Runtime.getRuntime().totalMemory();

    private int     mHz;
    private int     realMHz;
    private int     computationPerformance;
    private int     memoryReadSpeed;
    private int     memoryCopySpeed;
    private int     memoryWriteSpeed;
    private long    rsSizeLimit;
    private int     allocHandling;
    private boolean hasAllocBug;
    private int     freeStorage;
    private boolean growingHeap;
    private int     rs_initialSize;
    private int     rs_availableAfterIncreasedSet;
    private int     rs_availableAfterReducedSet;
    private int     rs_availableAfterDeleteRecord;
    private int     rs_availableAfterSet1000;
    private int     rs_availableAfterSet100;
    private int     rs_availableAfterRSDelete;
    private int     displaySpeed;
    private boolean listFontBug;

    private static synchronized void load()
    {
        RecordStore rs = null;
        try
        {
            rs = RecordStore.openRecordStore( "sys_devinfo", true );
            final byte[] data = rs.getRecord( 1 );
            if( data != null )
            {
                final ByteArrayInputStream bais = new ByteArrayInputStream( data );
                final SerializerInputStream in = new SerializerInputStream( bais );
                final DeviceInfo deviceInfo = new DeviceInfo();
                deviceInfo.deserialize( in );
                DeviceInfo.setDeviceInfo( deviceInfo );
            }
        }
        catch( Exception e )
        {
        }
        finally
        {
            if( rs != null ) try{ rs.closeRecordStore(); }catch( Exception e ){}
        }
    }

    private static synchronized void save()
    {
        if( deviceInfo!= null )
        {
            RecordStore rs = null;
            try
            {
                final ByteArrayOutputStream baos = new ByteArrayOutputStream();
                final SerializerOutputStream out = new SerializerOutputStream( baos );
                deviceInfo.serialize( out );
                final byte[] data = baos.toByteArray();
                rs = RecordStore.openRecordStore( "sys_devinfo", true );
                if( rs.getNumRecords() == 0 )
                {
                    rs.addRecord( data, 0, data.length );
                }
                else
                {
                    rs.setRecord( 1, data, 0, data.length );
                }
            }
            catch( Exception e )
            {
            }
            finally
            {
                if( rs != null ) try{ rs.closeRecordStore(); }catch( Exception e ){}
            }
        }
    }

    /**
     * Calculate device capabilities and return them in a DeviceInfo object.<br/>
     * The class automatically stores the device information for future use in
     * a system recordset called sys_devinfo, so the next time instead of
     * performing calcs it just loads from the record store.
     * @return DeviceInfo object
     */
    public static synchronized DeviceInfo getDeviceInfo()
    {
        if( deviceInfo == null )
        {
            load();
        }
        if( deviceInfo == null )
        {
            deviceInfo = new DeviceInfo();
            if( System.getProperty( "microedition.configuration" ) != null )
            {
                deviceInfo.testFontListBug();

                final ProgressEvent pe = new ProgressEvent();
                pe.setAnimate( true );
                pe.setMessage( "" );
                pe.setPhase( ResourceManager.getResource( "deviceInfo.calc.performance" ) );
                pe.setSource( deviceInfo );
                pe.setTitle( ResourceManager.getResource( "deviceInfo.fetchingInfo" ) );
                pe.dispatch();
                if( !isEmulator() )
                {
                    try
                    {
                        deviceInfo.calcPerformance();
                        pe.setPhase( ResourceManager.getResource( "deviceInfo.calc.memPerformance" ) );
                        pe.dispatch();
                        deviceInfo.calcMemoryPerformance();
                        pe.setPhase( ResourceManager.getResource( "deviceInfo.calc.rsLimit" ) );
                        pe.dispatch();
                        deviceInfo.calcRsSizeLimit();
                        pe.setPhase( ResourceManager.getResource( "deviceInfo.calc.allocHandling" ) );
                        pe.dispatch();
                        deviceInfo.checkAllocHandling();
                        pe.setPhase( ResourceManager.getResource( "deviceInfo.calc.growingHeap" ) );
                        pe.dispatch();
                        deviceInfo.checkGrowingHeap();
                    }
                    catch( Throwable e )
                    {
                    }
                }
                else
                {
                    deviceInfo.rsSizeLimit = RS_LIMIT_NONE;
                }
            }
            else
            {
                deviceInfo.rsSizeLimit = RS_LIMIT_NONE;
            }
            save();
        }
        return deviceInfo;
    }

    private void testFontListBug()
    {
        if( isNokia() )
        {
            final Font font = Font.getFont(
                    Font.FACE_PROPORTIONAL,
                    Font.STYLE_PLAIN,
                    Font.SIZE_SMALL
            );
            final List list = new List( "", List.IMPLICIT );
            for( int i = 0; i < 3; i++ )
            {
                list.append( "", null );
            }
            for( int i = 0; i < 3; i++ )
            {
                list.setFont( i, font );
            }
            list.deleteAll();
            try
        {
            for( int i = 0; i < 4; i++ )
                {
                    list.append( "", null );
                }
                listFontBug = false;
            }
            catch( Throwable e )
            {
                listFontBug = true;
            }
        }
        else
        {
            listFontBug = true;
        }
    }

//    private void testDisplaySpeed()
//    {
//        final long start = System.currentTimeMillis();
//        final String title = ResourceManager.getResource(
//                "deviceInfo.calc.displaySpeed"
//        );
//        final Form form = new Form( title );
//        final Gauge gauge = new Gauge( title, false, 100, 0 );
//        gauge.setLayout( Item.LAYOUT_EXPAND );
//        form.append( gauge );
//        Application.getManager().getDisplay().setCurrent( form );
//        for( int j = 0; j < 3; j++ )
//        {
//            for( int i = 1; i < 100; i++ )
//            {
//                gauge.setValue( i );
//            }
//            Thread.yield();
//        }
//        final long ellapsed = (System.currentTimeMillis() - start);
//        displaySpeed = (int) ellapsed;
//    }

    /**
     * Set the static device info object. Useful when deserializing a deviceInfo
     * object, so calls to getDeviceInfo will just return the deserialized object.
     * @param deviceInfo device info
     */
    public static synchronized void setDeviceInfo( final DeviceInfo deviceInfo )
    {
        DeviceInfo.deviceInfo = deviceInfo;
    }

    /**
     * Creates an empty DeviceInfo object. Useful for deserialization.
     */
    public DeviceInfo()
    {
    }

    public boolean isSlowDisplay()
    {
        final String platform = System.getProperty( "microedition.platform" );
        return platform != null && platform.indexOf( "Nokia6680" ) > -1;
    }

    public int getmHz()
    {
        return mHz;
    }

    public int getRealMHz()
    {
        return realMHz;
    }

    public int getComputationPerformance()
    {
        return computationPerformance;
    }

    public int getMemoryReadSpeed()
    {
        return memoryReadSpeed;
    }

    public int getMemoryCopySpeed()
    {
        return memoryCopySpeed;
    }

    public int getMemoryWriteSpeed()
    {
        return memoryWriteSpeed;
    }

    public long getRsSizeLimit()
    {
        return rsSizeLimit;
    }

    public int getAllocHandling()
    {
        return allocHandling;
    }

    public boolean hasAllocBug()
    {
        return hasAllocBug;
    }

    public int getFreeStorage()
    {
        return freeStorage;
    }

    public boolean isGrowingHeap()
    {
        return growingHeap;
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.core.tools.DeviceInfo";
    }

    /**
     * Write object status to stream.
     *
     * @param out output stream
     * @throws bm.core.io.SerializationException on errors
     */
    public void serialize( SerializerOutputStream out )
            throws SerializationException
    {
        out.writeInt( mHz );
        out.writeInt( realMHz );
        out.writeInt( computationPerformance );
        out.writeInt( memoryReadSpeed );
        out.writeInt( memoryCopySpeed );
        out.writeInt( memoryWriteSpeed );
        out.writeLong( rsSizeLimit );
        out.writeInt( allocHandling );
        out.writeBoolean( hasAllocBug );
        out.writeBoolean( growingHeap );
        out.writeInt( displaySpeed );
        out.writeBoolean( listFontBug );
        log.debug( "mHz: " + mHz );
        log.debug( "realMHz: " + realMHz );
        log.debug( "growingHeap: " + growingHeap );
        log.debug( "listFontBug: " + listFontBug );
    }

    /**
     * Read object status from stream.
     *
     * @param in input stream
     * @throws SerializationException on errors
     */
    public void deserialize( SerializerInputStream in )
            throws SerializationException
    {
        mHz = in.readInt();
        realMHz = in.readInt();
        computationPerformance = in.readInt();
        memoryReadSpeed = in.readInt();
        memoryCopySpeed = in.readInt();
        memoryReadSpeed = in.readInt();
        rsSizeLimit = in.readLong();
        allocHandling = in.readInt();
        hasAllocBug = in.readBoolean();
        growingHeap = in.readBoolean();
        displaySpeed = in.readInt();
        listFontBug = in.readBoolean();
        log.debug( "mHz: " + mHz );
        log.debug( "realMHz: " + realMHz );
        log.debug( "growingHeap: " + growingHeap );
        log.debug( "listFontBug: " + listFontBug );
    }

    public int getRs_initialSize()
    {
        return rs_initialSize;
    }

    public int getRs_availableAfterIncreasedSet()
    {
        return rs_availableAfterIncreasedSet;
    }

    public int getRs_availableAfterReducedSet()
    {
        return rs_availableAfterReducedSet;
    }

    public int getRs_availableAfterDeleteRecord()
    {
        return rs_availableAfterDeleteRecord;
    }

    public int getRs_availableAfterSet1000()
    {
        return rs_availableAfterSet1000;
    }

    public int getRs_availableAfterSet100()
    {
        return rs_availableAfterSet100;
    }

    public int getRs_availableAfterRSDelete()
    {
        return rs_availableAfterRSDelete;
    }

    /** @noinspection UnusedAssignment,JavaDoc*/
    private void checkGrowingHeap()
    {
        if( Runtime.getRuntime().totalMemory() > startHeapSize )
        {
            growingHeap = true;
        }
        else
        {
            try
            {
                //noinspection UnusedDeclaration
                byte[] test;
                try
                {
                    test = new byte[ (int) Runtime.getRuntime().freeMemory() ];
                }
                catch( OutOfMemoryError e )
                {
                }
                growingHeap = Runtime.getRuntime().totalMemory() > startHeapSize;
                test = null;
            }
            catch( Throwable e )
            {
            }
            finally
            {
                System.gc();
            }
        }
    }

    private void checkAllocHandling()
    {
        try
        {
            RecordStore rs1 = RecordStore.openRecordStore( RSNAME1, true );
            rs_initialSize = rs1.getSizeAvailable();
            rs1.addRecord( new byte[ 100 ], 0, 100 );
            rs_availableAfterSet100 = rs1.getSizeAvailable();
            rs1.setRecord( 1, new byte[ 1000 ], 0, 1000 );
            rs_availableAfterSet1000 = rs1.getSizeAvailable();
            rs1.setRecord( 1, new byte[ 10000 ], 0, 10000 );
            rs_availableAfterIncreasedSet = rs1.getSizeAvailable();
            rs1.setRecord( 1, new byte[ 1000 ], 0, 1000 );
            rs_availableAfterReducedSet = rs1.getSizeAvailable();
            rs1.deleteRecord( 1 );
            rs_availableAfterDeleteRecord = rs1.getSizeAvailable();
            rs1.closeRecordStore();

            RecordStore.deleteRecordStore( RSNAME1 );

            rs1 = RecordStore.openRecordStore( RSNAME1, true );
            rs_availableAfterRSDelete = rs1.getSizeAvailable();
            rs1.closeRecordStore();

            RecordStore.deleteRecordStore( RSNAME1 );

            hasAllocBug = rs_availableAfterReducedSet < rs_availableAfterIncreasedSet;
            if( rs_availableAfterDeleteRecord == rs_initialSize )
            {
                allocHandling = AH_FREES_ON_RECORD_DELETE;
            }
            else if( rs_availableAfterRSDelete == rs_initialSize )
            {
                allocHandling = AH_FREES_ON_RS_DELETE;
            }
            else
            {
                allocHandling = AH_BOGUS;
            }
        }
        catch( Throwable e )
        {
            allocHandling = AH_UNKNOWN;
        }
    }

    public static int getCurrentSizeLimit()
    {
        try
        {
            RecordStore rs = RecordStore.openRecordStore( RSNAME1, true );
            final int size = rs.getSizeAvailable();
            rs.closeRecordStore();
            return size;
        }
        catch( RecordStoreException e )
        {
            return -1;
        }
    }

    private void calcRsSizeLimit()
    {
        final byte[] record = new byte[100];
        try
        {
            RecordStore rs1 = RecordStore.openRecordStore( RSNAME1, true );
            final int size1_0 = rs1.getSizeAvailable();
            freeStorage = size1_0;
            rs1.addRecord( record, 0, 100 );
            rs1.closeRecordStore();

            RecordStore rs2 = RecordStore.openRecordStore( RSNAME2, true );
            final int size2_0 = rs2.getSizeAvailable();
            rs2.closeRecordStore();

            RecordStore.deleteRecordStore( RSNAME1 );
            RecordStore.deleteRecordStore( RSNAME2 );

            if( size1_0 == size2_0 )
            {
                rsSizeLimit = size1_0;
            }
            else
            {
                rsSizeLimit = RS_LIMIT_NONE;
            }
        }
        catch( Throwable e )
        {
            rsSizeLimit = -2;
        }
    }

    /**
	 * Calculate performance.
	 */
	private void calcPerformance()
	{
        try
        {
            long start, duration;
            mHz= 0;
            int nbIter= 10;
            // This first value allows the JIT compiler to optimize the function getMHz(int)
            final int minimumDuration= 1500;
            // minimum duration of the benchmark in ms
            do
            {
                if( nbIter < 0 )
                { // if by multiplying nbIter we have past the MAX_VALUE, nbIter becomes negative so we go back to MAX_VALUE.
                    nbIter= Integer.MAX_VALUE;
                }
                start= System.currentTimeMillis();
                mHz= getMHz( nbIter );
                duration= System.currentTimeMillis() - start;
                Thread.yield();
                if (duration < 50)
                {
                    // while test lasts less than 50ms, increase 10 times the iterations
                    nbIter *= 10;
                }
                else if (duration < minimumDuration)
                {
                    // Try to estimate the number of iterations to last minimumDuration
                    int newNbIter= minimumDuration * nbIter / (int)duration * 12 / 10 + 1;
                    // add 20% by *12/10
                    if( ( duration <= minimumDuration / 2 ) &&
                        ( newNbIter > nbIter ) )
                    {
                        nbIter = newNbIter;
                    }
                    else
                    {
                        nbIter *= 2;
                    }
                }
            }
            while (((mHz == 0) || (duration < minimumDuration)) && (nbIter != Integer.MAX_VALUE));
            //
            realMHz= mHz * 18;
            computationPerformance= pi(10);
            //allows the JIT compiler to optimize the function pi(long)
            computationPerformance= 400000 / pi(500000);
        }
        catch( Throwable e )
        {
        }
    }

    /**
	 * Calculate memory performance.
	 */
	private void calcMemoryPerformance()
	{
		long memoryDuration;
		// This first value allows the JIT compiler to optimize the function getMHz(int)
		final int minimumDuration= 1500;
		// minimum duration of the benchmark in ms
		int memoryNbIter;
        final int[] arrayP = new int[ SPEEDARRAY_SIZE ];
		// Compute the speed of the memory during at least minimumDuration ms
		memoryNbIter= 100;
        int maxInt= 0;
        if( !isBlackBerry() )
        {
            try
            {
                // Memory read
                maxInt = Integer.MAX_VALUE;
                do
                {
                    if( memoryNbIter <= 0 )
                    {
                        memoryNbIter = maxInt;
                    }
                    memoryDuration= memoryReadSpeed(memoryNbIter, arrayP );
                    Thread.yield();
                    if (memoryNbIter != maxInt)
                    {
                        if (memoryDuration < 50)
                        {
                            // while test lasts less than 50ms, increase 10 times the iterations
                            memoryNbIter *= 10;
                        }
                        else if (memoryDuration < minimumDuration)
                        {
                            // Try to estimate the number of iterations to last minimumDuration
                            int newNbIter= minimumDuration * memoryNbIter / (int)memoryDuration * 12 / 10 + 1;
                            // add 20% by *12/10
                            if( ( memoryDuration <= minimumDuration / 2 ) &&
                                ( newNbIter > memoryNbIter ) )
                            {
                                memoryNbIter = newNbIter;
                            }
                            else
                            {
                                memoryNbIter *= 2;
                            }
                        }
                    }
                }
                while ((memoryNbIter != maxInt) && (memoryDuration < minimumDuration));
                memoryReadSpeed= (int) ((long)SPEEDARRAY_SIZE * 4l * (long)memoryNbIter * 1000l / memoryDuration / 1024l);
                // *4 because ints are 4 bytes
                // *1000 because duration is in ms; /1024 to have speed in Kb/s
            }
            catch( Throwable e )
            {
            }
        }

        try
        {
            // Memory write
            memoryNbIter= 100;
            do
            {
                if( memoryNbIter <= 0 )
                {
                    memoryNbIter = maxInt;
                }
                memoryDuration= memoryCopySpeed(memoryNbIter, arrayP );
                Thread.yield();
                if (memoryNbIter != maxInt)
                {
                    if (memoryDuration < 50)
                    {
                        // while test lasts less than 50ms, increase 10 times the iterations
                        memoryNbIter *= 10;
                    }
                    else if (memoryDuration < minimumDuration)
                    {
                        // Try to estimate the number of iterations to last minimumDuration
                        int newNbIter= minimumDuration * memoryNbIter / (int)memoryDuration * 12 / 10 + 1;
                        // add 20% by *12/10
                        if( ( memoryDuration <= minimumDuration / 2 ) &&
                            ( newNbIter > memoryNbIter ) )
                        {
                            memoryNbIter = newNbIter;
                        }
                        else
                        {
                            memoryNbIter *= 2;
                        }
                    }
                }
            }
            while ((memoryNbIter != maxInt) && (memoryDuration < minimumDuration));
            memoryCopySpeed= (int) ((long)SPEEDARRAY_SIZE * 4l * (long)memoryNbIter * 1000l / memoryDuration / 1024l);
            // *4 because ints are 4 bytes
            // *1000 because duration is in ms; /1024 to have speed in Kb/s
        }
        catch( Throwable e )
        {
        }

        try
        {
            // Memory copy
            memoryNbIter= 100;
            do
            {
                if( memoryNbIter <= 0 )
                {
                    memoryNbIter = maxInt;
                }
                memoryDuration= memoryWriteSpeed(memoryNbIter, arrayP);
                Thread.yield();
                if (memoryNbIter != maxInt)
                {
                    if (memoryDuration < 50)
                    {
                        // while test lasts less than 50ms, increase 10 times the iterations
                        memoryNbIter *= 10;
                    }
                    else if (memoryDuration < minimumDuration)
                    {
                        // Try to estimate the number of iterations to last minimumDuration
                        int newNbIter= minimumDuration * memoryNbIter / (int)memoryDuration * 12 / 10 + 1;
                        // add 20% by *12/10
                        if( ( memoryDuration <= minimumDuration / 2 ) &&
                            ( newNbIter > memoryNbIter ) )
                        {
                            memoryNbIter = newNbIter;
                        }
                        else
                        {
                            memoryNbIter *= 2;
                        }
                    }
                }
            }
            while ((memoryNbIter != maxInt) && (memoryDuration < minimumDuration));
            memoryWriteSpeed= (int) ((long)SPEEDARRAY_SIZE * 4l * (long)memoryNbIter * 1000l / memoryDuration / 1024l);
            // *4 because ints are 4 bytes
            // *1000 because duration is in ms; /1024 to have speed in Kb/s
        }
        catch( Throwable e )
        {
        }
    }

    /**
     * Check if the device is a RIM BlackBerry.
     * @return true if it's a BlackBerry.
     */
    public static boolean isBlackBerry()
    {
        return checkPlatform( "RIM" );
    }

    /**
     * Check if microedition.platform system property contains the given text.
     * <br/>
     * Performs a case-insensitive test.
     *
     * @param key text to find
     * @return true if found
     */
    public static boolean checkPlatform( final String key )
    {
        final String platform = System.getProperty( "microedition.platform" );
        return platform != null && platform.toUpperCase().indexOf(
                key.toUpperCase()
        ) > -1;
    }

    /**
     * Check if user-agent jad property contains the given text.<br/>
     * Case-insensitive.
     *
     * @param key text ot find
     * @return true if found
     */
    public static boolean checkUserAgent( final String key )
    {
        final String userAgent = Application.getManager().getProperty(
                "user-agent"
        );
        return userAgent != null && userAgent.toUpperCase().indexOf(
                key.toUpperCase()
        ) > -1;
    }

    /**
     * Check if any of the given key is found at microedition.platform.
     *
     * @param keys texts to find
     * @return true if any of them is found
     */
    public static boolean checkPlatform( final String[] keys )
    {
        final int length = keys.length;
        for( int i = 0; i < length; i++ )
        {
            if( checkPlatform( keys[i] ) )
            {
                return true;
            }
        }
        return false;
    }

    /**
     * Check if the device is a Nokia.
     * @return true if it's a Nokia.
     */
    public static boolean isNokia()
    {
        return checkPlatform( "Nokia" );
    }

    /**
     * Check if we're running in the emulator.
     * @return true if so
     */
    public static boolean isEmulator()
    {
        return checkPlatform( new String[] { "j2me", "SunMicrosystems_wtk" } );
    }

    /**
     * Check if the device is a SonyEricsson.
     * @return true if it's a SonyEricsson.
     */
    public static boolean isSonyEricsson()
    {
        return checkPlatform( "SonyEricsson" );
    }

    /**
     * Check if the device is a SonyEricsson Java Platform 7.
     * @return true if so
     */
    public static boolean isSonyEricssonJP7()
    {
        return isSonyEricsson() && checkPlatform( JP7 );
    }

    /**
     * Check if the device runs on the Symbian Operating System.
     * @return true if so
     */
    public static boolean isSymbian()
    {
        return checkUserAgent( "SymbianOS" );
    }

    /**
     * Check if the device is a Symbian S60 one.
     * @return true if so
     */
    public static boolean isSeries60()
    {
        return checkUserAgent( "Series60" );
    }

    /**
     * Check if the device is a Symbian S60 2nd edition one.
     * @return true if so
     */
    public static boolean isSeries60_2nd()
    {
        return checkUserAgent( "Series60/2" );
    }

    /**
     * Check if the device is a Symbian S60 3rd edition one.
     * @return true if so
     */
    public static boolean isSeries60_3rd()
    {
        return checkUserAgent( "Series60/3" );
    }

    /**
     * Get the optimal compression factor depending on the device speed.
     * @return compression factor, between 1 and 9, 1 best speed, 9 best compression
     */
    public int getOptimalCompression()
    {
        return 1;
    }

    /**
     * Check if the device has the list font bug found in the WTK2.2.
     * @return true if so
     */
    public boolean hasListFontBug()
    {
        return listFontBug;
    }

    /**
     * Get the estimated speed of the CPU in MHz, considering that a Java
     * compiler is used. Usually, there is no Java compiler on phones, but
     * we do it this way because JIT will be used soon, and it allows a better
     * comparison with desktop computers.<br>
     * This algorithm performs a loop where each instruction should be performed in one cycle
     * on any modern CPU, if Java byte-code is compiled. It's not extremly reliable, but gives
     * a good idea of the speed.
     * @param maxi The maximum number of loops to perform to evaluate the speed of the CPU.
     * @return The estimated speed of the CPU multiplied by 10 to have one decimal figure.
     * This decimal figure is rounded to the nearest value. If maxi is to low to measure the time, returns 0.
     */
    private int getMHz(int maxi)
    {
        int i= 2;
        long duration, start= System.currentTimeMillis();
        while (--maxi != 0)
        {
            if( ( maxi & 1 ) == 0 )
            {
                i += 2;
            }
            // if maxi is even, add 2 to i, we don't add if it's odd.
        }
        duration= System.currentTimeMillis() - start;
        int mhz= 0;
        if (duration != 0)
        {
            // mhz =  number of CPU cycles for the loop : 6
            // 		* number of loops : i
            //		/ (number of cycles per second in 1MHz / number of ms in 1s / 100)
            //			= 1000000 / 1000 /100
            //			= 10 so we divide by 10
            //		/ duration in ms
            // so now mhz holds a value in MHz multiplied by 100 to have 2 decimal figures
            mhz= (int) (6l * (long)i / 10l / duration); // 6l = 6 as a long so that 6 times i will fit even if i is big
            // rounding the decimal figure
            if( ( mhz - ( mhz / 10 ) * 10 ) >= 5 )
            {
                mhz = mhz + 10;
            }
            mhz /= 10;
            // keeps one decimal figure instead of two
            // always return a non zero value (test successful)
            if( mhz == 0 )
            {
                mhz = 1;
            }
        }
        return mhz;
    }

	/**
	 * Use a suite to compute a base number which would allow to know PI simply.
	 * This test is used to determine the computational speed, so we return the time spent
	 * for the computation.
	 * @param max Number of loops.
	 * @return The time spent in ms.
	 */
	private int pi(long max)
	{
		int p= 0;
		long m= 100000000l;
		long start= System.currentTimeMillis();
		while (max > 0)
		{
			p += (int) (m / (max * max--));
		}
		long duration= System.currentTimeMillis() - start;
		if (p - 4 == 0)
		{
			// Stupid test (never true) to fool the optimizer, which would think p is useful.
			duration= 0;
		}
		/* we could compute PI this way :
		   double pi;
		   pi=Math.sqrt(6.0d*p/(double)m);
		*/
		return (int)duration;
	}

	/**
	 * Read <code>iter * SPEEDARRAY_SIZE</code> bytes in memory
	 * and return the number of ms needed to do it.
	 * @param iter Number of full reads of the <code>speedArray</code> array.
	 * @return Number of ms need to to the reads.
     * @param arrayP array
	 */
    private static long memoryReadSpeed(int iter, final int[] arrayP )
	{
		// We use a static array to fool the optimizer which can't now if this function is usefull.
		// In Java the four first variables (including parameters) in a static method are accessed faster
		// so wa have carrefully chosen those 4 first variables
		int i, j;
		int[] array = arrayP;
		int size;
		long duration;
		size= SPEEDARRAY_SIZE;
		// local copy of this constant, faster access.
		for (i= size; --i >= 0;)
		{
			// empty loop to initialize the cache (if any) and to fill the array
			array[i]= i;
		} /* endfor */
		duration= System.currentTimeMillis();
		for (j= iter + 1; --j != 0;)
		{
			for (i= size; --i >= 0;)
			{
				i= array[i];
			} /* endfor */
		} /* endfor */
		duration= System.currentTimeMillis() - duration;
		return duration;
	}

	/**
	 * Write <code>iter * SPEEDARRAY_SIZE</code> bytes in memory
	 * and return the number of ms needed to do it.
	 * @param iter Number of full writes of the <code>speedArray</code> array.
	 * @return Number of ms need to to the writes.
     * @param arrayP array
	 */
	private static long memoryWriteSpeed(int iter, final int[] arrayP )
	{
		// We use a static array to fool the optimizer which can now if this function is usefull.
		// In Java the four first variables (including parameters) in a static method are accessed faster
		// so wa have carrefully chosen those 4 first variables
		int i, j;
		// We use a local variable to go faster on some JVMs.
		final int[] array = arrayP;
		int size;
		long duration;
		size= SPEEDARRAY_SIZE;
		// local copy of this constant, faster access.
		for (i= size; --i >= 0;)
		{
			// empty loop to initialize the cache (if any)
			array[i]= i;
		} /* endfor */
		duration= System.currentTimeMillis();
		for (j= iter + 1; --j != 0;)
		{
			for (i= size; --i >= 0;)
			{
				array[i]= i;
			} /* endfor */
		} /* endfor */
		duration= System.currentTimeMillis() - duration;
		return duration;
	}

	/**
	 * Copy <code>iter * SPEEDARRAY_SIZE</code> bytes in memory
	 * and return the number of ms needed to do it.
	 * @param iter Number of full copies of the <code>speedArray</code> array.
	 * @param arrayP array
     * @return Number of ms need to to the copies.
	 */
	private static long memoryCopySpeed( int iter, final int[] arrayP )
	{
		// We use a static array to fool the optimizer which can now if this function is usefull.
		// In Java the four first variables (including parameters) in a static method are accessed faster
		// so wa have carrefully chosen those 4 first variables
		int i, j, size;
        final int[] array = arrayP;
        // We use a local variable to go faster on some JVMs.
        int[] arrayCopy = new int[ SPEEDARRAY_SIZE ];
		long duration;
		size= SPEEDARRAY_SIZE;
		// local copy of this constant, faster access.
		for (i= size; --i >= 0;)
		{
			// empty loop to initialize the cache (if any)
			array[i]= i;
		} /* endfor */
		i= 0;
		size--; // as we do an extra manual copy, we do one less copy during arraycopy()
		duration= System.currentTimeMillis();
		for (j= iter + 1; --j != 0;)
		{
			System.arraycopy(array, 0, arrayCopy, 0, size);
			array[i]= arrayCopy[i] + 1;
            if( ++i == size )
            {
                i = 0;
            }
		} /* endfor */
		duration= System.currentTimeMillis() - duration;
		return duration;
	}
}
