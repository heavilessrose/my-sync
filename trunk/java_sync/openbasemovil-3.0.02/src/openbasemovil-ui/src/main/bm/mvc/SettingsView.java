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

import bm.core.ErrorHandler;
import bm.core.ResourceManager;
import bm.core.mvc.AbstractController;
import bm.core.mvc.ControllerEvent;
import bm.db.DBException;
import bm.db.Settings;
import bm.ui.def.view.settings.SettingsViewDef;
import bm.ui.def.view.settings.Setting;

import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.List;
import java.util.Hashtable;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 12-jul-2007 16:42:57
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A menu that sets and un sets settings flags.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 * @noinspection UnnecessaryTemporaryOnConversionToString
 */
public class SettingsView
    extends NativeMenuView
{
    Hashtable   options;
    String      name;

    public static SettingsView create(
            final SettingsViewDef   viewDef,
            final String            name
    )
    {
        final SettingsView settingsView = new SettingsView(
                ResourceManager.getResource( viewDef.getTitle() ),
                viewDef.getId()
        );
        settingsView.name = name;
        settingsView.build( viewDef );
        return settingsView;
    }

    private void build( final SettingsViewDef viewDef  )
    {
        final Vector items = viewDef.getItems();
        final int count = items.size();

        if( viewDef.getImageMap() != null )
        {
            loadImages(
                    name,
                    viewDef.getImageMap(),
                    viewDef.getImageMapBig(),
                    count
            );
        }
        options = new Hashtable( count );
        for( int i = 0; i < count; i++ )
        {
            final Setting setting = (Setting) items.elementAt( i );
            boolean active = true;
            if( setting.getOnlyIf() != null )
            {
                active = ViewFactory.flags.contains( setting.getOnlyIf() );
            }
            else if( setting.getUnless() != null )
            {
                active = !ViewFactory.flags.contains( setting.getUnless() );
            }
            if( active )
            {
                final Option option = new Option();
                option.index = i;
                option.key = setting.getKey();
                option.defaultValue = setting.isDefaultValue();
                option.title = setting.getTitle();
                final String text = ResourceManager.getResource(
                        option.title + "." +
                        getSetting( option.key, option.defaultValue )
                );
                int index;
                if( viewDef.getImageMap() != null )
                {
                    index = addOption(
                            text,
                            (Image) images.get( name + "." + i ),
                            i
                    );
                }
                else
                {
                    index = addOption( text, i );
                }
                options.put( new Integer( index ), option );
            }
        }
        helpId = viewDef.getHelpId();
    }

    protected SettingsView( final String title, final int viewId )
    {
        super( null, title, viewId );
        controller = new SettingsController();
    }

    private class SettingsController
        extends AbstractController
    {
        protected void process( ControllerEvent event )
        {
            try
            {
                switch( event.getCode() )
                {
                    case ControllerEvent.BACK:
                        goBack( event.getSource() );
                        break;

                    default:
                        final Option option = (Option) options.get(
                                new Integer( event.getIndex() )
                        );
                        if( option != null)
                        {
                            final String value = new Boolean(
                                    !getSetting(
                                            option.key,
                                            option.defaultValue
                                    )
                            ).toString();
                            Settings.setProperty( option.key, value );
                            final List list = (List) displayable;
                            final String text = ResourceManager.getResource(
                                    option.title + "." + value
                            );
                            list.set(
                                    event.getIndex(),
                                    text,
                                    (Image) images.get(
                                            name + "." + option.index
                                    )
                            );
                        }
                        show();
                        break;
                }
            }
            catch( DBException e )
            {
                ErrorHandler.handleError( this, e );
            }
        }
    }

    private class Option
    {
        String  title;
        String  key;
        boolean defaultValue;
        int     index;
    }

    public static boolean getSetting( final String key, final boolean defVal )
    {
        try
        {
            final String pref = Settings.getProperty( key );
            if( pref != null )
            {
                final Boolean dv = new Boolean( !defVal );
                if( pref.toLowerCase().equals( dv.toString().toLowerCase() ) )
                {
                    return dv.booleanValue();
                }
            }
        }
        catch( DBException e )
        {
        }
        return defVal;
    }
}
