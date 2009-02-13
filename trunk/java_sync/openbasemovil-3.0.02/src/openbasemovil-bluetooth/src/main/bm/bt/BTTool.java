/*
 * Copyright (c) 2006 Elondra, S.L. All Rights Reserved.
 */
package bm.bt;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Bluetooth Library, Bluetooth utilties
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

import javax.bluetooth.*;
import java.util.Hashtable;
import java.util.Vector;
import java.util.Enumeration;

import bm.core.log.LogFactory;
import bm.core.log.Log;
import bm.core.ResourceManager;

/*
 * File Information
 *
 * Created on       : 13-feb-2006 12:58:47
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Add class description
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class BTTool
        implements DiscoveryListener
{
    public static final long UUID_SDP = 0x0001;
    public static final long UUID_RFCOMM = 0x0003;
    public static final long UUID_OBEX = 0x0008;
    public static final long UUID_HTTP = 0x000C;
    public static final long UUID_L2CAP = 0x0100;
    public static final long UUID_BNEP = 0x000F;
    public static final long UUID_SERIAL_PORT = 0x1101;
    public static final long UUID_SERVICE_DISCOVERY_SERVER_SERVICE_CLASS_ID = 0x1000;
    public static final long UUID_BROWSE_GROUP_DESCRIPTOR_SERVICE_CLASS_ID = 0x1001;
    public static final long UUID_PUBLIC_BROWSE_GROUP = 0x1002;
    public static final long UUID_OBEX_OBJECT_PUSH_PROFILE = 0x1105;
    public static final long UUID_OBEX_FILE_TRANSFER_PROFILE = 0x1106;
    public static final long UUID_PERSONAL_AREA_NETWORKING_USER = 0x1115;
    public static final long UUID_NETWORK_ACCESS_POINT = 0x1116;
    public static final long UUID_GROUP_NETWORK = 0x1117;

    public static final int MAJOR_IMAGING   = 0x600;
    public static final int MINOR_PRINTER   = 0x80;


    private static Log log = LogFactory.getLog( "BTTool" );
    /** @noinspection AnalyzingVariableNaming*/
    private static final BTTool instance = new BTTool();

    private LocalDevice      localDevice;
    private DiscoveryAgent   agent;
    private Hashtable        remoteDevices       = new Hashtable( 10 );
    private Vector           listeners           = new Vector( 10 );
    private boolean          inquiryStarted;
    private int[]            transactions;
    private int              transactionCount;
    private Integer          majorDevice;
    private Integer          minorDevice;
    private boolean          hasBluetooth;

    private String           apiVersion;
    private String           obexApiVersion;
    private int              maxConnectedDevices;
    private int              maxServiceSearches;

    private final Hashtable  discoveredDevices   = new Hashtable( 10 );
    private final Hashtable  discoveredServices  = new Hashtable( 10 );

    protected BTTool()
    {
        boolean hasBluetooth;
        try
        {
            Class.forName( "javax.bluetooth.LocalDevice" );
            hasBluetooth = true;
        }
        catch( ClassNotFoundException e )
        {
            hasBluetooth = false;
        }
        this.hasBluetooth = hasBluetooth;
        if( hasBluetooth )
        {
            try
            {
                final LocalDevice localDevice = LocalDevice.getLocalDevice();
                this.localDevice = localDevice;
                apiVersion = LocalDevice.getProperty( "bluetooth.api.version" );
                obexApiVersion = LocalDevice.getProperty( "obex.api.version" );
                try
                {
                    maxConnectedDevices = Integer.parseInt(
                            LocalDevice.getProperty(
                                    "bluetooth.connected.devices.max"
                            )
                    );
                }
                catch( NumberFormatException e )
                {
                    maxConnectedDevices = 0;
                }
                int maxServiceSearches;
                try
                {
                    maxServiceSearches = Integer.parseInt(
                            LocalDevice.getProperty(
                                    "bluetooth.sd.trans.max"
                            )
                    );
                }
                catch( NumberFormatException e )
                {
                    maxServiceSearches = 0;
                }
                this.maxServiceSearches = maxServiceSearches;
                agent = localDevice.getDiscoveryAgent();
                if( maxServiceSearches > 0 )
                {
                    transactions = new int[maxServiceSearches];
                    for( int i = 0; i < maxServiceSearches; i++ )
                    {
                        transactions[i] = -1;
                    }
                }
                else
                {
                    agent = null;
                    this.localDevice = null;
                }
//            localDevice.setDiscoverable( DiscoveryAgent.GIAC );
            }
            catch( BluetoothStateException e )
            {
                log.error( e );
            }
        }
    }

    /**
     * Bluetooth api version.
     * @return api version or null if no bluetooth
     */
    public static String getApiVersion()
    {
        return instance.apiVersion;
    }

    /**
     * Obex api version.
     * @return obex api version or null if no obex
     */
    public static String getObexApiVersion()
    {
        return instance.obexApiVersion;
    }

    /**
     * Maxium number of connected devices including parked devices.
     * @return maximum number of connected devices or 0 if no bt or not defined
     */
    public static int getMaxConnectedDevices()
    {
        return instance.maxConnectedDevices;
    }

    /**
     * Maxium number of simultaneous service searchse.
     * @return maximum number of service searches or 0 if no bt or not defined
     */
    public static int getMaxServiceSearches()
    {
        return instance.maxServiceSearches;
    }

    /**
     * Is the device bluetooth capable?
     * @return true if the device has a bluetooth api, false if not
     */
    public static boolean bluetoothCapable()
    {
        return instance.hasBluetooth;
    }

    /**
     * Register a discovery listener.
     * @param listener listener
     */
    public static void registerListener( final DiscoveryListener listener )
    {
        instance.listeners.addElement( listener );
    }

    /**
     * Unregister listener.
     * @param listener listener
     */
    public static void unregisterListener( final DiscoveryListener listener )
    {
        instance.listeners.removeElement( listener );
    }

    /**
     * Discover services, filtering for device type.
     * @param major major device type, optional might be null
     * @param minor minor device type, optional might be null
     * @throws BluetoothStateException on errors
     */
    public static synchronized void discover(
            final Integer major,
            final Integer minor
    )
            throws BluetoothStateException
    {
        final BTTool instance = BTTool.instance;
        if( instance.localDevice != null )
        {
            if( !instance.inquiryStarted )
            {
                instance.majorDevice = major;
                instance.minorDevice = minor;
                instance.remoteDevices.clear();
                instance.discoveredDevices.clear();
                fetchCachedDevices();
                fetchPreknownDevices();

                instance.inquiryStarted = instance.agent.startInquiry(
                        DiscoveryAgent.GIAC,
                        instance
                );
            }
        }
        else
        {
            throw new BluetoothStateException(
                    ResourceManager.getResource( "error.noLocalDevice" )
            );
        }
    }

    /**
     * Discover any kind of bluetooth devices.
     * @throws BluetoothStateException on errors
     */
    public static synchronized void discover()
            throws BluetoothStateException
    {
        discover( null, null );
    }

    /**
     * Test if a discovery hsa been started.
     * @return true if so
     */
    public static boolean isInquiryStarted()
    {
        return instance.inquiryStarted;
    }

    /**
     * Locate a remote device given it's uuid.
     * @param uuid device uuid
     * @return RemoteDevice or null if not found
     */
    public static RemoteDevice getRemoteDevice( final String uuid )
    {
        return (RemoteDevice) instance.remoteDevices.get( uuid );
    }

    /**
     * Get an enumeration of remote devices.
     * @return remote devices.
     */
    public static Enumeration getRemoteDevices()
    {
        return instance.remoteDevices.elements();
    }

    public static Enumeration getRemoteDevicesUuids()
    {
        return instance.remoteDevices.keys();
    }

    public static Vector searchServices(
            final RemoteDevice  rd,
            final UUID[]        uuids
    )
            throws BluetoothStateException
    {
        final BTTool instance = BTTool.instance;
        if( instance.localDevice != null )
        {
            final int transactionId = instance.agent.searchServices(
                    null, uuids, rd, instance
            );
            addTransaction( transactionId );
            final Vector services = new Vector(1);
            instance.discoveredServices.put(
                    Integer.toString( transactionId ),
                    services
            );
            synchronized( instance )
            {
                try
                {
                    instance.wait();
                }
                catch( InterruptedException e )
                {
                    log.error( e );
                }
            }
            return services;
        }
        else
        {
            throw new BluetoothStateException(
                    ResourceManager.getResource( "error.noLocalDevice" )
            );
        }
    }

    /**
     * Start a serach for printing devices.
     * @throws BluetoothStateException on errors
     */
    public static void findPrinters()
            throws BluetoothStateException
    {
        discover( new Integer( MAJOR_IMAGING ), new Integer( MINOR_PRINTER ) );
    }

    /**
     * Get the url for a printer bspp port.
     * @param rd remote device
     * @param security security flags
     * @param mustBeMaster must be master
     * @return printer port
     * @throws BluetoothStateException on errors
     */
    public static String getPrinterPort(
            final RemoteDevice  rd,
            final int           security,
            final boolean       mustBeMaster
    )
            throws BluetoothStateException
    {
        final UUID[] uuids = new UUID[]{ new UUID( UUID_RFCOMM ) };
        final Vector services = searchServices( rd, uuids );
        if( services != null && services.size() > 0 )
        {
            return ((ServiceRecord) services.elementAt( 0 )).getConnectionURL(
                    security,
                    mustBeMaster
            );
        }
        else
        {
            return null;
        }
    }

    public void deviceDiscovered( final RemoteDevice    remoteDevice,
                                  final DeviceClass     deviceClass
    )
    {
        if( majorDevice != null )
        {
            if( (deviceClass.getMajorDeviceClass() & majorDevice.intValue()) == 0 )
            {
                return;
            }
        }
        if( minorDevice  != null )
        {
            if( (deviceClass.getMinorDeviceClass() & minorDevice.intValue()) == 0 )
            {
                return;
            }
        }
        discoveredDevices.put(
            remoteDevice.getBluetoothAddress(),
            remoteDevice
        );
        //noinspection MethodCallInLoopCondition
        for( Enumeration i = listeners.elements(); i.hasMoreElements(); )
        {
            ((DiscoveryListener) i.nextElement()).deviceDiscovered(
                    remoteDevice,
                    deviceClass
            );
        }
    }

    private static synchronized void addTransaction( final int transaction )
            throws BluetoothStateException
    {
        final BTTool instance = BTTool.instance;
        final int length = instance.transactions.length;
        if( instance.transactionCount < length )
        {
            for( int i = 0; i < length; i++ )
            {
                if( instance.transactions[i] != -1 )
                {
                    instance.transactions[i] = transaction;
                    instance.transactionCount++;
                    break;
                }
            }
        }
        else
        {
            throw new BluetoothStateException(
                    ResourceManager.getResource( "error.tooManyConnections" )
            );
        }
    }

    private static synchronized void removeTransaction( final int transaction )
    {
        final BTTool instance = BTTool.instance;
        final int length = instance.transactions.length;
        for( int i = 0; i < length; i++ )
        {
            if( instance.transactions[i] == transaction )
            {
                instance.transactions[i] = -1;
                instance.transactionCount--;
                break;
            }
        }
    }

    private static void fetchPreknownDevices()
    {
        final BTTool instance = BTTool.instance;
        final RemoteDevice[] preknownDevices =
            instance.agent.retrieveDevices( DiscoveryAgent.PREKNOWN );
        final int prekLength = preknownDevices != null ? preknownDevices.length : 0;
        if( prekLength > 0 )
        {
            for( int i=0; i < prekLength; i++ )
            {
                //noinspection ConstantConditions
                instance.remoteDevices.put(
                    preknownDevices[i].getBluetoothAddress(),
                    preknownDevices[i]);
            }
        }
    }

    private static void fetchCachedDevices() {
        final RemoteDevice[] cachedDevices =
                instance.agent.retrieveDevices( DiscoveryAgent.CACHED );
        final int cachedLength = cachedDevices != null ? cachedDevices.length : 0;
        if( cachedLength > 0 )
        {
            for( int i=0; i < cachedLength; i++)
            {
                //noinspection ConstantConditions
                instance.remoteDevices.put(
                    cachedDevices[i].getBluetoothAddress(),
                    cachedDevices[i]
                );
            }
        }
    }

    public void servicesDiscovered( final int               transactionId,
                                    final ServiceRecord[]   serviceRecords
    )
    {
        final Vector services = (Vector) discoveredServices.get(
                Integer.toString( transactionId )
        );
        if( services != null && serviceRecords != null )
        {
            final int length = serviceRecords.length;
            for( int i = 0; i < length; i++ )
            {
                services.addElement( serviceRecords[i] );
            }
            //noinspection MethodCallInLoopCondition
            for( Enumeration i = listeners.elements(); i.hasMoreElements(); )
            {
                ((DiscoveryListener) i.nextElement()).servicesDiscovered(
                        transactionId,
                        serviceRecords
                );
            }
        }
    }

    public void serviceSearchCompleted(
            final int transactionId,
            final int responseCode
    )
    {
        discoveredServices.remove( Integer.toString( transactionId ) );
        removeTransaction( transactionId );
        //noinspection MethodCallInLoopCondition
        for( Enumeration i = listeners.elements(); i.hasMoreElements(); )
        {
            ((DiscoveryListener) i.nextElement()).serviceSearchCompleted(
                    transactionId,
                    responseCode
            );
        }
        synchronized( instance )
        {
            instance.notifyAll();
        }
    }

    public void inquiryCompleted( final int code )
    {
        final Hashtable discoveredDevices = this.discoveredDevices;
        //noinspection MethodCallInLoopCondition
        for( Enumeration i = discoveredDevices.keys(); i.hasMoreElements(); )
        {
            final String uuid = (String) i.nextElement();
            if( !remoteDevices.containsKey( uuid ) )
            {
                remoteDevices.put( uuid, discoveredDevices.get( uuid ) );
            }
        }
        //noinspection MethodCallInLoopCondition
        for( Enumeration i = listeners.elements(); i.hasMoreElements(); )
        {
            ((DiscoveryListener) i.nextElement()).inquiryCompleted(
                    code
            );
        }
        synchronized( this.discoveredDevices )
        {
            inquiryStarted = false;
        }
    }

    /**
     * Cancel the current service search.
     * @param transaction transaction number
     */
    public static synchronized void cancelServiceSearch( final int transaction )
    {
        instance.agent.cancelServiceSearch( transaction );
        removeTransaction( transaction );
    }

    /**
     * Cancel current device discovery search.
     */
    public static synchronized void cancelDiscovery()
    {
        final BTTool instance = BTTool.instance;
        if( instance.inquiryStarted )
        {
            instance.agent.cancelInquiry( instance );
            instance.inquiryStarted = false;
            final int length = instance.transactions.length;
            for( int i = 0; i < length; i++ )
            {
                instance.transactions[i] = -1;
            }
            instance.transactionCount = 0;
            //noinspection MethodCallInLoopCondition
            for( Enumeration i = instance.listeners.elements(); i.hasMoreElements(); )
            {
                ((DiscoveryListener) i.nextElement()).inquiryCompleted(
                        DiscoveryListener.INQUIRY_TERMINATED
                );
            }
        }
    }
}
