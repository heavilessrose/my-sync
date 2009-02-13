package com.basemovil.vc;
/* -----------------------------------------------------------------------------
    OpenBaseMovil View Compiler, generates the binary form of views from
    an XML file.
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

import com.basemovil.vc.view.View;
import com.basemovil.vc.view.Action;
import com.basemovil.vc.view.Attachment;
import com.basemovil.vc.view.listBrowser.SpeedOption;
import com.basemovil.vc.view.listBrowser.Field;
import com.basemovil.vc.view.listBrowser.ListBrowser;
import com.basemovil.vc.view.settings.Setting;
import com.basemovil.vc.view.settings.SettingsView;
import com.basemovil.vc.view.menu.Menu;
import com.basemovil.vc.view.menu.MenuItem;
import com.basemovil.vc.view.form.*;

import java.util.Map;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.io.*;

import org.xml.sax.EntityResolver;
import org.xml.sax.SAXException;
import org.xml.sax.InputSource;
import org.apache.commons.digester.Digester;
import bm.db.index.Index;
import bm.storage.Store;
import bm.storage.RSException;
import bm.storage.RecordStoreFullException;
import bm.storage.InvalidRecordIDException;
import bm.core.io.SerializationException;
import bm.core.io.SerializerOutputStream;
import bm.core.zlib.ZStream;
import bm.core.zlib.ZOutputStream;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 18:08:48
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * View Compiler.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ViewCompiler
{
    public static final Map APPEARANCES = new HashMap( 3 );
    static
    {
        APPEARANCES.put( "plain", new Integer( 0 ) );
        APPEARANCES.put( "hyperlink", new Integer( 1 ) );
        APPEARANCES.put( "button", new Integer( 3 ) );
    }

    private int         order = 30;
    private String      version;
    private String      listBrowser;
    private Hashtable   views = new Hashtable( 10 );
    private Hashtable   controllers = new Hashtable( 10 );
    private Hashtable   iconSets = new Hashtable( 10 );

    public int getOrder()
    {
        return order;
    }

    public void setOrder( final int order )
    {
        this.order = order;
    }

    public String getListBrowser()
    {
        return listBrowser;
    }

    public void setListBrowser( final String listBrowser )
    {
        this.listBrowser = listBrowser;
    }

    public String getVersion()
    {
        return version;
    }

    public void setVersion( final String version )
    {
        this.version = version;
    }

    public void add( final View view )
            throws Exception
    {
        if( views.containsKey( view.getName() ) )
        {
            throw new Exception( "Duplicate view: " + view.getName() );
        }
        else
        {
            view.setParent( this );
            views.put( view.getName(), view );
        }
    }

    public void add( final Controller controller )
            throws Exception
    {
        if( controllers.contains( controller.getId() ) )
        {
            throw new Exception( "Duplicate controller: " + controller.getId() );
        }
        else
        {
            controller.setParent( this );
            controllers.put( controller.getId(), controller );
        }
    }

    public void add( final IconSet iconSet )
            throws Exception
    {
        if( iconSets.contains( iconSet.getId() ) )
        {
            throw new Exception( "Duplicate iconSet: " + iconSet.getId() );
        }
        else
        {
            iconSet.setParent( this );
            iconSets.put( iconSet.getId(), iconSet );
        }
    }

    public static void main( String[] args )
    {
        try
        {
            final ViewCompiler compiler = new ViewCompiler();
            compiler.parse( new File( args[0] ) );
            compiler.store( new File( args[1] ) );
        }
        catch( Exception e )
        {
            e.printStackTrace();
        }
    }

    public void store( final File output )
            throws ViewCompilerException
    {
        System.out.println( "Creating view index" );
        System.out.println( "Index order: " + order );
        System.out.println( "Version: " + version );
        System.out.println( "listBrowser: " + listBrowser );
        System.out.println( "IconSets: " + iconSets.size() );
        System.out.println( "Controllers: " + controllers.size() );
        System.out.println( "Views: " + views.size() );

        final Index index = new Index( "sys_compiled_views", order, Index.KT_STRING, true, false );
        final Store store = Store.get( "sys_compiled_views", 1 );
        index.setRecordStore( store );
        if( listBrowser != null && listBrowser.equalsIgnoreCase( "native" ) )
        {
            try
            {
                index.insertObject( "listBrowserMode", "native" );
            }
            catch( Exception e )
            {
                throw new ViewCompilerException( e );
            }
        }
        for( final Iterator i = controllers.values().iterator(); i.hasNext(); )
        {
            final Controller controller = (Controller) i.next();
            controller.store( index );
        }
        for( final Iterator i = iconSets.values().iterator(); i.hasNext(); )
        {
            final IconSet iconSet = (IconSet) i.next();
            iconSet.store( index );
        }
        for( final Iterator i = views.values().iterator(); i.hasNext(); )
        {
            final View view = (View) i.next();
            view.store( index );
        }
        FileOutputStream fos = null;
        try
        {
            System.out.println( "Index size: " + store.getSize() + " bytes" );
            fos = new FileOutputStream( output );
            writeIndex( index, fos );
        }
        catch( Exception e )
        {
            throw new ViewCompilerException( e );
        }
        finally
        {
            if( fos != null ) try{ fos.close(); }catch( Exception e ){}
            System.out.println( "Compressed size: " + output.length() + " bytes" );
        }
    }

    public static void writeIndex(
            final Index        index,
            final OutputStream os
    )
            throws IOException,
                   RSException,
                   SerializationException
    {
        System.out.println( "Writing index" );
        final ZOutputStream zos = new ZOutputStream(
                os,
                ZStream.Z_BEST_COMPRESSION
        );
        final SerializerOutputStream out = new SerializerOutputStream( zos );

        final int numRecords;
        try
        {
            index.getRecordStore().open();
        }
        catch( RecordStoreFullException e )
        {
            throw new RSException( 0, e );
        }
        final Store rs = index.getRecordStore();
        out.writeInt( index.getOrder() );
        out.writeByte( (byte) index.getType() );
        out.writeBoolean( index.isCaseSensitive() );
        numRecords = rs.getNumRecords();
        System.out.println( "Writing " + numRecords + " records" );
        out.writeInt( numRecords );
        for( int i = 1; i <= numRecords; i++ )
        {
            try
            {
                out.writeBlob( rs.getRecord( i ) );
            }
            catch( InvalidRecordIDException e )
            {
            }
        }
        out.flush();
        zos.flush();
        zos.close();
        System.out.println( "Done" );
    }

    public void parse( final File input )
            throws IOException,
                   SAXException
    {
        final Digester d = new Digester();

        System.out.println( "Parsing file " + input.getAbsolutePath() );

        d.setEntityResolver(
                new EntityResolver()
                {
                    public InputSource resolveEntity(
                            String publicId,
                            String systemId
                    )
                            throws SAXException,
                                   IOException
                    {
                        InputSource is = null;
                        if( publicId != null )
                        {
                            is = resolve( publicId );
                        }
                        if( is == null && systemId != null )
                        {
                            is = resolve( systemId );
                        }
                        return is;
                    }

                    private InputSource resolve( final String id )
                    {
                        if( id.equals( "http://www.basemovil.com/dtd/bm_vdl_1.0.dtd" ) )
                        {
                            final InputStream is = getClass().getResourceAsStream(
                                    "/bm_vdl_1.0.dtd"
                            );
                            return is != null ? new InputSource( is ) : null;
                        }
                        else
                        {
                            return null;
                        }
                    }
                }
        );

        d.push( this );
        d.addSetProperties( "bm-vdl" );

        d.addObjectCreate( "bm-vdl/controller", Controller.class );
        d.addSetProperties( "bm-vdl/controller" );
        d.addSetNext( "bm-vdl/controller", "add", Controller.class.getName() );

        d.addObjectCreate( "bm-vdl/iconSet", IconSet.class );
        d.addSetProperties( "bm-vdl/iconSet" );
        d.addSetNext( "bm-vdl/iconSet", "add", IconSet.class.getName() );

        d.addObjectCreate( "bm-vdl/form", Form.class );
        d.addSetProperties( "bm-vdl/form" );

        d.addObjectCreate( "bm-vdl/form/text", Text.class );
        d.addSetProperties( "bm-vdl/form/text" );
        d.addCallMethod( "bm-vdl/form/text", "setText", 0 );
        d.addSetNext( "bm-vdl/form/text", "add", Item.class.getName() );

        d.addObjectCreate( "bm-vdl/form/date", Date.class );
        d.addSetProperties( "bm-vdl/form/date" );
        d.addCallMethod( "bm-vdl/form/date", "setText", 0 );
        d.addSetNext( "bm-vdl/form/date", "add", Item.class.getName() );

        d.addObjectCreate( "bm-vdl/form/time", Time.class );
        d.addSetProperties( "bm-vdl/form/time" );
        d.addCallMethod( "bm-vdl/form/time", "setText", 0 );
        d.addSetNext( "bm-vdl/form/time", "add", Item.class.getName() );

        d.addObjectCreate( "bm-vdl/form/timestamp", Timestamp.class );
        d.addSetProperties( "bm-vdl/form/timestamp" );
        d.addCallMethod( "bm-vdl/form/timestamp", "setText", 0 );
        d.addSetNext( "bm-vdl/form/timestamp", "add", Item.class.getName() );

        d.addObjectCreate( "bm-vdl/form/gauge", Gauge.class );
        d.addSetProperties( "bm-vdl/form/gauge" );
        d.addSetNext( "bm-vdl/form/gauge", "add", Item.class.getName() );

        d.addObjectCreate( "bm-vdl/form/spacer", Spacer.class );
        d.addSetProperties( "bm-vdl/form/spacer" );
        d.addSetNext( "bm-vdl/form/spacer", "add", Item.class.getName() );

        d.addObjectCreate( "bm-vdl/form/image", Image.class );
        d.addSetProperties( "bm-vdl/form/image" );
        d.addSetNext( "bm-vdl/form/image", "add", Item.class.getName() );

        d.addObjectCreate( "bm-vdl/form/choice", Choice.class );
        d.addSetProperties( "bm-vdl/form/choice" );

        d.addObjectCreate( "bm-vdl/form/choice/option", Option.class );
        d.addSetProperties( "bm-vdl/form/choice/option" );
        d.addSetNext( "bm-vdl/form/choice/option", "add", Option.class.getName() );

        d.addSetNext( "bm-vdl/form/choice", "add", Item.class.getName() );

        d.addObjectCreate( "bm-vdl/form/money", Money.class );
        d.addSetProperties( "bm-vdl/form/money" );
        d.addCallMethod( "bm-vdl/form/money", "setText", 0 );
        d.addSetNext( "bm-vdl/form/money", "add", Item.class.getName() );

        d.addObjectCreate( "bm-vdl/form/percent", Percent.class );
        d.addSetProperties( "bm-vdl/form/percent" );
        d.addCallMethod( "bm-vdl/form/percent", "setText", 0 );
        d.addSetNext( "bm-vdl/form/percent", "add", Item.class.getName() );

        d.addObjectCreate( "bm-vdl/form/integer", IntegerField.class );
        d.addSetProperties( "bm-vdl/form/integer" );
        d.addCallMethod( "bm-vdl/form/integer", "setText", 0 );
        d.addSetNext( "bm-vdl/form/integer", "add", Item.class.getName() );

        d.addObjectCreate( "bm-vdl/form/fixedpoint", FixedPoint.class );
        d.addSetProperties( "bm-vdl/form/fixedpoint" );
        d.addCallMethod( "bm-vdl/form/fixedpoint", "setText", 0 );
        d.addSetNext( "bm-vdl/form/fixedpoint", "add", Item.class.getName() );

        d.addObjectCreate( "bm-vdl/form/button", Button.class );
        d.addSetProperties( "bm-vdl/form/button" );
        d.addSetNext( "bm-vdl/form/button", "add", Item.class.getName() );

        d.addObjectCreate( "bm-vdl/form/action", Action.class );
        d.addSetProperties( "bm-vdl/form/action" );
        d.addSetNext( "bm-vdl/form/action", "add", Action.class.getName() );

        d.addObjectCreate( "bm-vdl/form/attachment", Attachment.class );
        d.addSetProperties( "bm-vdl/form/attachment" );
        d.addSetNext( "bm-vdl/form/attachment", "add", Attachment.class.getName() );

        d.addSetNext( "bm-vdl/form", "add", View.class.getName() );

        d.addObjectCreate( "bm-vdl/menu", Menu.class );
        d.addSetProperties( "bm-vdl/menu" );

        d.addObjectCreate( "bm-vdl/menu/menuItem", MenuItem.class );
        d.addSetProperties( "bm-vdl/menu/menuItem" );
        d.addSetNext( "bm-vdl/menu/menuItem", "add", MenuItem.class.getName() );

        d.addObjectCreate( "bm-vdl/menu/action", Action.class );
        d.addSetProperties( "bm-vdl/menu/action" );
        d.addSetNext( "bm-vdl/menu/action", "add", Action.class.getName() );

        d.addObjectCreate( "bm-vdl/menu/attachment", Attachment.class );
        d.addSetProperties( "bm-vdl/menu/attachment" );
        d.addSetNext( "bm-vdl/menu/attachment", "add", Attachment.class.getName() );

        d.addSetNext( "bm-vdl/menu", "add", View.class.getName() );


        d.addObjectCreate( "bm-vdl/settingsView", SettingsView.class );
        d.addSetProperties( "bm-vdl/settingsView" );

        d.addObjectCreate( "bm-vdl/settingsView/setting", Setting.class );
        d.addSetProperties( "bm-vdl/settingsView/setting" );
        d.addSetNext( "bm-vdl/settingsView/setting", "add", Setting.class.getName() );

        d.addObjectCreate( "bm-vdl/settingsView/action", Action.class );
        d.addSetProperties( "bm-vdl/settingsView/action" );
        d.addSetNext( "bm-vdl/settingsView/action", "add", Action.class.getName() );

        d.addObjectCreate( "bm-vdl/settingsView/attachment", Attachment.class );
        d.addSetProperties( "bm-vdl/settingsView/attachment" );
        d.addSetNext( "bm-vdl/settingsView/attachment", "add", Attachment.class.getName() );

        d.addSetNext( "bm-vdl/settingsView", "add", View.class.getName() );

        d.addObjectCreate( "bm-vdl/listBrowser", ListBrowser.class );
        d.addSetProperties( "bm-vdl/listBrowser" );

        d.addObjectCreate( "bm-vdl/listBrowser/field", Field.class );
        d.addSetProperties( "bm-vdl/listBrowser/field" );
        d.addSetNext( "bm-vdl/listBrowser/field", "add", Field.class.getName() );

        d.addObjectCreate( "bm-vdl/listBrowser/speedOption", SpeedOption.class );
        d.addSetProperties( "bm-vdl/listBrowser/speedOption" );
        d.addSetNext( "bm-vdl/listBrowser/speedOption", "add", SpeedOption.class.getName() );

        d.addObjectCreate( "bm-vdl/listBrowser/action", Action.class );
        d.addSetProperties( "bm-vdl/listBrowser/action" );
        d.addSetNext( "bm-vdl/listBrowser/action", "add", Action.class.getName() );

        d.addObjectCreate( "bm-vdl/listBrowser/attachment", Attachment.class );
        d.addSetProperties( "bm-vdl/listBrowser/attachment" );
        d.addSetNext( "bm-vdl/listBrowser/attachment", "add", Attachment.class.getName() );

        d.addSetNext( "bm-vdl/listBrowser", "add", View.class.getName() );

        d.parse( input );
    }

    /**
     * Parse escape characters in a text and return a parsed version of it.<br/>
     * This method substitutes \n, \r and \t for their respective characters.
     *
     * @param text input text
     * @return parsed text
     */
    public static String escape( final String text )
    {
        if( text != null )
        {
            final StringBuffer buffer = new StringBuffer( );
            final char[] chars = text.toCharArray();
            boolean escaping = false;
            for( int i = 0; i < chars.length; i++ )
            {
                if( escaping )
                {
                    switch( chars[i] )
                    {
                        case 'n':
                            buffer.append( '\n' );
                            break;

                        case 'r':
                            buffer.append( '\r' );
                            break;

                        case 't':
                            buffer.append( '\t' );
                            break;

                        default:
                            buffer.append( chars[i] );
                            break;
                    }
                    escaping = false;
                }
                else
                {
                    if( chars[i] == '\\' )
                    {
                        escaping = true;
                    }
                    else
                    {
                        buffer.append( chars[i] );
                    }
                }
            }
            return buffer.toString();
        }
        else
        {
            return text;
        }
    }
}
