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

import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.List;
import javax.bluetooth.*;
import java.util.Hashtable;

import bm.core.mvc.AbstractView;
import bm.core.mvc.Controller;
import bm.core.mvc.ControllerEvent;
import bm.core.mvc.AbstractController;
import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.core.Application;
import bm.core.ResourceManager;

/*
 * File Information
 *
 * Created on       : 13-feb-2006 16:47:12
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Find and select bt services.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class SelectBTServicesView
    extends AbstractView
        implements CommandListener,
               DiscoveryListener
{
    public static final int SEARCH  = 10000;
    public static final int VIEW_ID = 90002;

    static final UUID[] UUIDS = { new UUID( BTTool.UUID_RFCOMM ) };

    static Log log = LogFactory.getLog( "SelectBTServicesView" );

    List    menu;
    boolean searching;

    RemoteDevice    device;
    private Command         search;
    private Command         cancelSearch;
    private Hashtable       services = new Hashtable( 10 );

    private final String threadLock = "T";

    public SelectBTServicesView( final Controller controller )
    {
        super( controller );

        final List menu;
        menu = new List(
                ResourceManager.getResource( "selectService.Title" ),
                List.IMPLICIT
        );

        search = new Command(
                ResourceManager.getResource( "global.Search" ),
                Command.SCREEN,
                SEARCH
        );
        cancelSearch = Application.getCommandFactory().getCommand(
                Command.CANCEL
        );

        menu.addCommand( search );
        menu.addCommand( Application.getCommandFactory().backCommand() );
        menu.setCommandListener( this );

        this.menu = menu;
    }

    public RemoteDevice getDevice()
    {
        return device;
    }

    public synchronized void setDevice( final RemoteDevice device )
    {
        this.device = device;
    }

    /**
     * Each view must have an id that identifies it.
     *
     * @return view id
     */
    public int getId()
    {
        return VIEW_ID;
    }

    public void show()
    {
        Application.getManager().getDisplay().setCurrent( menu );
    }

    /**
     * The view is about to hide and we shuld free up resources used by it.
     */
    public void release()
    {
        BTTool.cancelDiscovery();
    }

    /**
     * Add a command to the view. This method follows the IOC pattern, allowing
     * the container to configure the view by injecting commands into it.
     *
     * @param command the command
     */
    public void addCommand( final Command command )
    {
        menu.addCommand( command );
    }

    public void commandAction(
            final Command       command,
            final Displayable   displayable
    )
    {
        final List menu = this.menu;
        final Command search = this.search;
        final Command cancelSearch = this.cancelSearch;
        if( command == search )
        {
            menu.removeCommand( search );
            menu.addCommand( cancelSearch );
            new DiscoveryThread( this ).start();
        }
        else if( command == cancelSearch )
        {
            synchronized( threadLock )
            {
                searching = false;
            }
            BTTool.cancelDiscovery();
            menu.removeCommand( cancelSearch );
            menu.addCommand( search );
        }
        else
        {
            final ControllerEvent event = AbstractController.buildEvent(
                this,
                command,
                menu
            );
            controller.handle( event );
        }
    }

    public ServiceRecord getSelected()
    {
        final List menu = this.menu;
        if( menu.getSelectedIndex() >= 0 )
        {
            return (ServiceRecord) services.get( menu.getString(
                    menu.getSelectedIndex()
            ) );
        }
        else
        {
            return null;
        }
    }

    public void deviceDiscovered(
            final RemoteDevice  btDevice,
            final DeviceClass   cod
    )
    {
    }

    public void servicesDiscovered(
            final int               transID,
            final ServiceRecord[]   servRecord
    )
    {
        if( servRecord != null )
        {
            synchronized( threadLock )
            {
                final int count = servRecord.length;
                for( int i = 0; i < count; i++ )
                {
                    final String service = servRecord[i].getConnectionURL(
                            ServiceRecord.NOAUTHENTICATE_NOENCRYPT,
                            false
                    );
                    if( !services.containsKey( service ) )
                    {
                        services.put( service, servRecord[i] );
                        menu.append( service, null );
                    }
                }
            }
        }
    }

    public void serviceSearchCompleted( final int transID, final int respCode )
    {
        synchronized( threadLock )
        {
            searching = false;
            final List menu = this.menu;
            menu.setTitle( ResourceManager.getResource( "selectService.Title" ) );
            menu.removeCommand( cancelSearch );
            menu.addCommand( search );
        }
    }

    public void inquiryCompleted( final int discType )
    {
    }

    private class DiscoveryThread
        extends Thread
    {
        private SelectBTServicesView parent;
        private int     progress;

        public DiscoveryThread( final SelectBTServicesView parent )
        {
            this.parent = parent;
        }

        /** @noinspection FieldRepeatedlyAccessedInMethod*/
        public void run()
        {
            final SelectBTServicesView parent = this.parent;
            try
            {
                parent.searching = true;
                BTTool.registerListener( parent );
                BTTool.searchServices( device, UUIDS );
                while( parent.searching )
                {
                    parent.menu.setTitle(
                            ResourceManager.getResource( "global.Searching" ) + " " +
                            SelectBTDeviceView.PROGRESS[ progress++ ]
                    );
                    if( progress >= SelectBTDeviceView.PROGRESS.length )
                    {
                        progress = 0;
                    }
                    try
                    {
                        Thread.sleep( 200 );
                    }
                    catch( InterruptedException e )
                    {
                    }
                    Thread.yield();
                }
            }
            catch( BluetoothStateException e )
            {
                log.error( e );
                Application.getManager().showError( e );
            }
            finally
            {
                BTTool.unregisterListener( parent );
            }
        }
    }
}
