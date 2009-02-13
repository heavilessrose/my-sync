package bm.mvc;
/* -----------------------------------------------------------------------------
    OpenBaseMovil User Interface Library
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
import bm.core.ErrorHandler;
import bm.core.event.Event;
import bm.core.event.ProgressEvent;
import bm.core.mvc.View;
import bm.core.mvc.Controller;
import bm.core.mvc.Action;
import bm.ui.DataList;
import bm.ui.IconStore;
import bm.ui.IconMenuView;
import bm.ui.def.ControllerDef;
import bm.ui.def.IconSetDef;
import bm.ui.def.view.settings.SettingsViewDef;
import bm.ui.def.view.menu.MenuDef;
import bm.ui.def.view.listBrowser.ListBrowserDef;
import bm.ui.def.view.listBrowser.FieldDef;
import bm.ui.def.view.listBrowser.SpeedOptionDef;
import bm.ui.def.view.form.FormDef;
import bm.ui.def.view.ViewDef;
import bm.ui.def.view.AttachmentDef;
import bm.ui.def.view.ActionDef;
import bm.db.index.Index;
import bm.storage.RecordStoreFullException;
import bm.storage.RSException;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Item;
import javax.microedition.lcdui.Font;
import java.util.Hashtable;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 11-mar-2007 11:36:34
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Handles common view creation and caching.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public final class ViewFactory
{
    /**
     * When a view is first created the ViewFactory sends an Event with the
     * view attached with the key &quot;view&quot; so listeners can add or
     * remove features from the view.<br/>
     * The view name is also attached under &quot;viewName&quot;.<br/>
     * Register a listener at Event class for this kind of events in order to
     * receive such notifications.
     */
    public static final Integer EVENT_VIEW_CREATED  = new Integer( 1000 );
    public static final String  COMPILED_VIEWS      = "sys_compiled_views";

    static Hashtable    views = new Hashtable( 10 );
    static Vector       flags = new Vector( 10 );
    static Hashtable    controllers = new Hashtable( 10 );

    static Index index;

    static int indexOrder;
    static String resourceFile;

    private static ProgressEvent waitEvent;
    static
    {
        waitEvent = new ProgressEvent();
        waitEvent.setAnimate( true );
        waitEvent.setMessage( "" );
    }

    private static void sendWait()
    {
        final ProgressEvent waitEvent = ViewFactory.waitEvent;
        final String waitMessage = ResourceManager.getResource( "global.Wait" );
        waitEvent.setPhase( waitMessage );
        waitEvent.setTitle( waitMessage );
        waitEvent.dispatch();
    }

    public static void init( final int indexOrder, final String resourceFile )
    {
        ViewFactory.indexOrder = indexOrder;
        ViewFactory.resourceFile = resourceFile;
        ViewFactory.index = new Index(
                COMPILED_VIEWS,
                indexOrder,
                Index.KT_STRING,
                true,
                false
        );
        index.setSendProgressEvents( false );
        try
        {
            ViewFactory.index.open();
        }
        catch( RecordStoreFullException e )
        {
        }
        catch( RSException e )
        {
        }
    }

    public static void shutdown()
    {
        ViewFactory.index.close();
    }

    /**
     * Set a flag for view building.
     *
     * @param flag flag name
     */
    public static void setFlag( final String flag )
    {
        if( !flags.contains( flag ) )
        {
            flags.addElement( flag );
        }
    }

    /**
     * Unset a flag for view building.
     *
     * @param flag flag name
     */
    public static void unsetFlag( final String flag )
    {
        if( flags.contains( flag ) )
        {
            flags.removeElement( flag );
        }
    }

    /**
     * Check the presence of a flag.
     *
     * @param flag flag name
     * @return if the flag is set
     */
    public static boolean hasFlag( final String flag )
    {
        return flags.contains( flag );
    }

    /**
     * Get (and possibly load) the controller instance associated to a given id.
     * Throws RuntimeExceptions on errors.
     *
     * @param id controller id
     * @return controller
     */
    public static synchronized Controller getController( final String id )
    {
        Controller controller = (Controller) controllers.get( id );
        if( controller == null )
        {
            try
            {
                final byte[] data = (byte[]) index.find( "controller." + id );
                if( data == null )
                {
                    throw new IllegalArgumentException( "Missing controller: " + id );
                }

                ControllerDef controllerDef = new ControllerDef();
                controllerDef.read( data );
                final Class clazz = Class.forName( controllerDef.getClassName() );
                controller = (Controller) clazz.newInstance();
            }
            catch( Exception e )
            {
                throw new RuntimeException( "Error loading controller: " + e );
            }
            controllers.put( id, controller );
        }
        return controller;
    }

    /**
     * Remove a view instance from the cache.
     *
     * @param viewKey view key
     */
    public static void clearView( final String viewKey )
    {
        if( views.containsKey( viewKey ) )
        {
            views.remove( viewKey );
        }
    }

    /**
     * Get a SettingsView.
     *
     * @param name view definition name
     * @return the view
     */
    public static SettingsView getSettingsView( final String name )
    {
        return getSettingsView( name, name );
    }

    /**
     * Get a SettingsView.
     *
     * @param viewKey view key
     * @param name view definition name
     * @return the view
     */
    public static SettingsView getSettingsView(
            final String        viewKey,
            final String        name
    )
    {
        final Hashtable views = ViewFactory.views;
        if( views.containsKey( viewKey ) )
        {
            return (SettingsView) views.get( viewKey );
        }
        else
        {
            try
            {
                final byte[] data = (byte[]) index.find( "settingsView." + name );
                if( data == null )
                {
                    throw new IllegalArgumentException(
                            "The view does not exist"
                    );
                }

                final SettingsViewDef viewDef = new SettingsViewDef();
                viewDef.read( data );
                sendWait();
                final SettingsView menu = SettingsView.create( viewDef, name );
                configureView( viewKey, name, menu, viewDef );
                System.gc();
                return menu;
            }
            catch( Exception e )
            {
                throw new RuntimeException( "Error loading view: " + e );
            }
        }
    }

    /**
     * Get a Menu.
     *
     * @param name view definition name
     * @return the view
     */
    public static MenuView getMenu( final String name )
    {
        return getMenu( name, name );
    }

    /**
     * Get a Menu.
     *
     * @param viewKey view key
     * @param name view definition name
     * @return the view
     */
    public static MenuView getMenu(
            final String        viewKey,
            final String        name
    )
    {
        final Hashtable views = ViewFactory.views;
        if( views.containsKey( viewKey ) )
        {
            return (MenuView) views.get( viewKey );
        }
        else
        {
            try
            {
                final byte[] data = (byte[]) index.find( "menu." + name );
                if( data == null )
                {
                    throw new IllegalArgumentException(
                            "The view does not exist"
                    );
                }

                final MenuDef menuDef = new MenuDef();
                menuDef.read( data );
                sendWait();
                final MenuView menu;
                if( menuDef.getType() == MenuDef.NATIVE )
                {
                    menu = NativeMenuView.create(
                            getController( menuDef.getController() ),
                            menuDef,
                            name
                    );
                }
                else
                {
                    menu = IconMenuView.create(
                            getController( menuDef.getController() ),
                            menuDef
                    );
                }
                configureView( viewKey, name, menu, menuDef );
                System.gc();
                return menu;
            }
            catch( Exception e )
            {
                throw new RuntimeException( "Error loading view: " + e );
            }
        }
    }

    /**
     * Get a list browser.
     *
     * @param name view definition name
     * @return the view
     */
    public static ListBrowser getListBrowser( final String name )
    {
        return getListBrowser( name, name );
    }

    private static boolean isListBrowserDefaultNative()
    {
        try
        {
            final String val = (String) index.find( "listBrowserMode" );
            return val != null && val.equals( "native" );
        }
        catch( Exception e )
        {
            return false;
        }
    }

    /**
     * Get a list browser.
     *
     * @param viewKey view key
     * @param name view definition name
     * @return the view
     */
    public static ListBrowser getListBrowser(
            final String        viewKey,
            final String        name
    )
    {
        final Hashtable views = ViewFactory.views;
        if( views.containsKey( viewKey ) )
        {
            return (ListBrowser) views.get( viewKey );
        }
        else
        {
            final ListBrowserDef viewDef;
            try
            {
                final byte[] data = (byte[]) index.find( "listBrowser." + name );
                if( data == null )
                {
                    throw new IllegalArgumentException(
                            "The view does not exist"
                    );
                }
                viewDef = new ListBrowserDef();
                viewDef.read( data );
            }
            catch( Exception e )
            {
                throw new RuntimeException( "Error loading view: " + e );
            }
            sendWait();
            ListBrowser browser;
            final boolean nativeView =
                viewDef.getType() != null ?
                        viewDef.getType().byteValue() == ListBrowserDef.NATIVE :
                        isListBrowserDefaultNative();
            if( nativeView )
            {
                browser = new ListBrowserView(
                        getController( viewDef.getController() ),
                        ResourceManager.getResource( viewDef.getTitle() ),
                        viewDef.getId(),
                        viewDef.getPageSize() != null ?
                            viewDef.getPageSize().intValue() :
                            10
                );
            }
            else
            {
                final DataListView dlv = new DataListView(
                        getController( viewDef.getController() ),
                        ResourceManager.getResource( viewDef.getTitle() ),
                        viewDef.getId()
                );
                final DataList dataList = dlv.getDataList();
                if( viewDef.getColor() != null )
                {
                    dataList.setColor( viewDef.getColor().intValue() );
                }
                if( viewDef.getSelectedColor() != null )
                {
                    dataList.setSelectedColor(
                            viewDef.getSelectedColor().intValue()
                    );
                }
                if( viewDef.getBackground() != null )
                {
                    dataList.setBackground( viewDef.getBackground().intValue() );
                }
                if( viewDef.getSelectedBackground() != null )
                {
                    dataList.setSelectedBackground(
                            viewDef.getSelectedBackground().intValue()
                    );
                }
                if( viewDef.getMaxLines() != null )
                {
                    dataList.setMaxLines( viewDef.getMaxLines().intValue() );
                }
                browser = dlv;
            }

            final Vector fields = viewDef.getFields();
            final int fieldCount = fields.size();
            for( int i = 0; i < fieldCount; i++ )
            {
                final FieldDef field = (FieldDef) fields.elementAt( i );
                if( browser instanceof ListBrowserView )
                {
                    browser.addField( field.getName() );
                }
                else
                {
                    browser.addField(
                            field.getName(),
                            field.getLabel(),
                            field.getMode() != null ?
                                field.getMode().byteValue() :
                                DataList.PLAIN,
                            field.getTrueLabel(),
                            field.getFalseLabel(),
                            field.getResource()
                    );
                }
            }
            final String separator = viewDef.getSeparator();
            if( separator != null)
            {
                browser.setSeparator( separator );
            }
            if( browser instanceof ListBrowserView )
            {
                ((ListBrowserView) browser).setWrap(
                        viewDef.getWrap() != null &&
                        viewDef.getWrap().booleanValue()
                );
            }
            else
            {
                ((DataListView) browser).getDataList().setWrap(
                        viewDef.getWrap() != null &&
                        viewDef.getWrap().booleanValue()
                );
            }
            Font font = null;
            if( viewDef.getFontFace() != null )
            {
                font = Font.getFont(
                        viewDef.getFontFace().intValue(),
                        viewDef.getFontStyle() != null ?
                            viewDef.getFontStyle().intValue() :
                            Font.STYLE_PLAIN,
                        viewDef.getFontSize() != null ?
                            viewDef.getFontSize().intValue() :
                            Font.SIZE_SMALL
                );
            }
            else if( viewDef.getFontStyle() != null )
            {
                font = Font.getFont(
                        Font.FACE_PROPORTIONAL,
                        viewDef.getFontStyle().intValue(),
                        viewDef.getFontSize() != null ?
                            viewDef.getFontSize().intValue() :
                            Font.SIZE_SMALL
                );
            }
            else if( viewDef.getFontSize() != null )
            {
                font = Font.getFont(
                        Font.FACE_PROPORTIONAL,
                        Font.STYLE_PLAIN,
                        viewDef.getFontSize().intValue()
                );
            }
            if( font != null )
            {
                if( browser instanceof DataListView )
                {
                    ((DataListView) browser).getDataList().setFont( font );
                }
            }
            final Vector speedOptions = viewDef.getSpeedOptions();
            final int speedOptionCount = speedOptions.size();
            for( int i = 0; i < speedOptionCount; i++ )
            {
                final SpeedOptionDef option = (SpeedOptionDef)
                        speedOptions.elementAt( i );
                final String optionText = option.getText();
                final int code = option.getCode();

                boolean active = true;
                if( option.getOnlyIf() != null  )
                {
                    active = ViewFactory.hasFlag( option.getOnlyIf() );
                }
                else if( option.getUnless() != null )
                {
                    active = !ViewFactory.hasFlag( option.getUnless() );
                }
                if( active )
                {
                    if( browser instanceof ListBrowserView )
                    {
                        browser.addSpeedOption(
                                optionText,
                                code
                        );
                    }
                    else
                    {
                        ViewFactory.loadIconStore( option.getIconSet() );
                        if( option.getIconSet() != null )
                        {
                            browser.addSpeedOption(
                                    code,
                                    option.getIconSet(),
                                    option.getIcon() != null ?
                                        option.getIcon().intValue() :
                                        i,
                                    option.getHelp()
                            );
                        }
                        else
                        {
                            browser.addSpeedOption(
                                    code,
                                    optionText,
                                    option.getHelp()
                            );
                        }
                    }
                }
            }
            configureView( viewKey, name, browser, viewDef );
            System.gc();
            return browser;
        }
    }

    private static FormDef getFormDef( final String name )
    {
        try
        {
            final byte[] data = (byte[]) index.find( "form." + name );
            if( data == null )
            {
                throw new IllegalArgumentException(
                        "The view does not exist"
                );
            }
            final FormDef viewDef = new FormDef();
            viewDef.read( data );
            return viewDef;
        }
        catch( Exception e )
        {
            throw new RuntimeException( "Error loading view: " + e );
        }
    }

    /**
     * Get a DataForm.
     *
     * @param name view definition name
     * @param browsable if a DataForm is browsable
     * @param readOnly if a DataForm is readOnly
     * @return the view
     */
    public static DataFormView getDataForm(
            final String        name,
            final boolean       browsable,
            final boolean       readOnly
    )
    {
        if( views.containsKey( name ) )
        {
            return (DataFormView) views.get( name );
        }
        else
        {
            return getDataForm(
                    name,
                    name,
                    browsable,
                    readOnly,
                    getFormDef( name )
            );
        }
    }

    /**
     * Get a DataForm.
     *
     * @param name view definition name
     * @return the view
     */
    public static DataFormView getBrowsableDataForm( final String name )
    {
        if( views.containsKey( name ) )
        {
            return (DataFormView) views.get( name );
        }
        else
        {
            final FormDef def = getFormDef( name );
            return getDataForm(
                    name,
                    name,
                    true,
                    def.isReadOnly(),
                    def
            );
        }
    }

    /**
     * Get a DataForm.
     *
     * @param name view definition name
     * @return the view
     */
    public static DataFormView getDataForm( final String name )
    {
        if( views.containsKey( name ) )
        {
            return (DataFormView) views.get( name );
        }
        else
        {
            final FormDef def = getFormDef( name );
            return getDataForm(
                    name,
                    name,
                    false,
                    def.isReadOnly(),
                    def
            );
        }
    }

    /**
     * Get a DataForm.
     *
     * @param viewKey view key
     * @param name view definition name
     * @param browsable if a DataForm is browsable
     * @param readOnly if a DataForm is readOnly
     * @return the view
     */
    public static DataFormView getDataForm(
            final String        viewKey,
            final String        name,
            final boolean       browsable,
            final boolean       readOnly
    )
    {
        final Hashtable views = ViewFactory.views;
        if( views.containsKey( viewKey ) )
        {
            return (DataFormView) views.get( viewKey );
        }
        else
        {
            return getDataForm(
                    viewKey,
                    name,
                    browsable,
                    readOnly,
                    getFormDef( name )
            );
        }
    }

    private static DataFormView getDataForm(
            final String        viewKey,
            final String        name,
            final boolean       browsable,
            final boolean       readOnly,
            final FormDef       def
    )
    {
        sendWait();
        final DataFormView form = new DataFormView(
                getController( def.getController() ),
                name,
                browsable,
                readOnly,
                def
        );
        configureView( viewKey, name, form, def );
        System.gc();
        return form;
    }

    private static void configureView(
            final String    viewKey,
            final String    name,
            final View      view,
            final ViewDef   def
    )
    {
        addActions( view, def );
        addAttachments( view, def );
        views.put( viewKey, view );
        final Event event = new Event( EVENT_VIEW_CREATED );
        event.attach( "viewName", name );
        event.attach( "viewKey", viewKey );
        event.attach( "view", view );
        event.dispatch();
    }

    private static void addAttachments( final View view, final ViewDef def )
    {
        final Vector attachments = def.getAttachments();
        final int count = attachments.size();
        for( int i = 0; i < count; i++ )
        {
            final AttachmentDef attachment = (AttachmentDef)
                    attachments.elementAt( i );
            final String type = attachment.getType();
            if( type != null )
            {
                Object value;
                final String rawValue = attachment.getValue();
                if( type.equals( "int" ) )
                {
                    try
                    {
                        value = Integer.valueOf( rawValue );
                    }
                    catch( NumberFormatException e )
                    {
                        value = new Integer( 0 );
                    }
                }
                else if( type.equals( "boolean" ) )
                {
                    value = new Boolean(
                            rawValue != null && rawValue.equals( "true" )
                    );
                }
                else
                {
                    value = rawValue;
                }
                view.attach( attachment.getName(), value );
            }
        }
    }

    private static void addActions( final View view, final ViewDef def )
    {
        final Vector actions = def.getActions();
        final int count = actions.size();
        for( int i = 0; i < count; i++ )
        {
            final ActionDef actionDef = (ActionDef) actions.elementAt( i );
            boolean active;
            if( actionDef.getOnlyIf() != null )
            {
                active = flags.contains( actionDef.getOnlyIf() );
            }
            else if( actionDef.getUnless() != null )
            {
                active = !flags.contains( actionDef.getUnless() );
            }
            else
            {
                active = actionDef.isActive();
            }
            final String actionName = actionDef.getName();
            final boolean forItem = actionDef.getFor() != null;
            final int code = actionDef.getCode();
            Command command = Application.getCommandFactory().getCommand( code );
            if( command == null )
            {
                command = new Command(
                        ResourceManager.getResource( actionDef.getLabel() ),
                        forItem ?
                            Command.ITEM :
                            actionDef.getType() != null ?
                                actionDef.getType().intValue() :
                                Command.SCREEN,
                        actionDef.getOrder() != -1 ?
                            actionDef.getOrder() :
                            i + 1
                );
            }
            if( view instanceof DataFormView && forItem )
            {
                final Item item = ((DataFormView) view).getItem(
                        actionDef.getFor()
                );
                final Action action = new Action(
                        actionName,
                        command,
                        code,
                        item,
                        actionDef.getDefault() != null &&
                            actionDef.getDefault().booleanValue()
                );
                view.addAction( action, active );
            }
            else
            {
                view.addAction( new Action(
                        actionName,
                        command,
                        code,
                        null,
                        actionDef.getDefault() != null &&
                            actionDef.getDefault().booleanValue()
                ), active );
            }
        }
    }

    /**
     * Remove all cached views.
     */
    public static void clear()
    {
        views.clear();
    }

    public static void loadIconStore( final String id )
    {
        if( id != null )
        {
            if( !IconStore.hasIconSet( id ) )
            {
                try
                {
                    final byte[] data = (byte[]) index.find( "iconset." + id );
                    final IconSetDef iconSetDef = new IconSetDef();
                    iconSetDef.read( data );
                    IconStore.addIconSet(
                            iconSetDef.getSource(),
                            id,
                            iconSetDef.getWidth(),
                            iconSetDef.getHeight()
                    );
                }
                catch( Exception e )
                {
                    ErrorHandler.handleError( id, e );
                }
            }
        }
    }
}
