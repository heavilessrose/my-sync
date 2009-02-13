/*
 * Copyright (c) 2006 Elondra S.L.. All Rights Reserved.
 */
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

import javax.microedition.lcdui.*;
import java.io.IOException;
import java.util.Hashtable;
import java.util.Enumeration;

import bm.core.mvc.AbstractView;
import bm.core.mvc.Controller;
import bm.core.mvc.ControllerEvent;
import bm.core.mvc.AbstractController;
import bm.core.Application;
import bm.core.ResourceManager;
import bm.db.ScrollSet;

/*
 * File Information
 *
 * Created on       : 31-mar-2006 11:27:46
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * BrowserView, browses rows of a scrollset, one by one.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public abstract class BrowserView
        extends AbstractView
        implements ScrollerView
{
    public static final int MOVE_PREVIOUS       = 41000;
    public static final int MOVE_NEXT           = 41001;

    private static final Byte STATE_0   = new Byte( (byte) 0 );
    private static final Byte STATE_N   = new Byte( (byte) 1 );
    private static final Byte STATE_S   = new Byte( (byte) 2 );
    private static final Byte STATE_NS  = new Byte( (byte) 3 );

    private static final Integer LOCK = new Integer( 0 );

    protected static Image       prevImage;
    protected static Image       nextImage;
    protected static Image       selectImage;

    protected Command     selectCommand;
    protected int         selectCommandId;

    private Hashtable   forms   = new Hashtable( 4 );
    private Hashtable   items   = new Hashtable( 4 );
    private Hashtable   values  = new Hashtable( 10 );
    private Byte        state = STATE_0;

    private ImageItem       previousN;
    private ImageItem       nextN;
    private ImageItem       selectButtonS;
    private ImageItem       previousNS;
    private ImageItem       nextNS;
    private ImageItem       selectButtonNS;

    private Command previousCommand;
    private Command nextCommand;

    protected byte            lastButton = 1;
    protected boolean         navigationEnabled;
    protected boolean         selectionEnabled;
    protected ScrollSet scrollSet;

    protected BrowserView(
            final Controller controller,
            final String        title,
            final int           selectCommandId
    )
    {
        super( controller );

        final Spacer spacerN = new Spacer( 3, 3 );
        spacerN.setLayout( Item.LAYOUT_LEFT | Item.LAYOUT_NEWLINE_AFTER );

        final Spacer spacerS = new Spacer( 3, 3 );
        spacerS.setLayout( Item.LAYOUT_LEFT | Item.LAYOUT_NEWLINE_AFTER );

        final Spacer spacerNS = new Spacer( 3, 3 );
        spacerNS.setLayout( Item.LAYOUT_LEFT | Item.LAYOUT_NEWLINE_AFTER );

        this.selectCommandId = selectCommandId;
        selectCommand = Application.getCommandFactory().getCommand(
                selectCommandId
        );

        initImages();
        initButtons();

        final Form form0   = new Form( title );
        final Form formN   = new Form( title );
        final Form formS   = new Form( title );
        final Form formNS  = new Form( title );

        formN.append( nextN );
        formN.append( previousN );
        formN.append( spacerN );

        formS.append( selectButtonS );
        formS.append( spacerS );

        formNS.append( nextNS );
        formNS.append( previousNS );
        formNS.append( selectButtonNS );
        formNS.append( spacerNS );

        form0.setCommandListener( this );
        formN.setCommandListener( this );
        formS.setCommandListener( this );
        formNS.setCommandListener( this );

        final Hashtable forms = this.forms;
        final Hashtable items = this.items;
        forms.put( STATE_0, form0 );
        forms.put( STATE_N, formN );
        forms.put( STATE_S, formS );
        forms.put( STATE_NS, formNS );

        items.put( STATE_0, new Hashtable( 10 ) );
        items.put( STATE_N, new Hashtable( 10 ) );
        items.put( STATE_S, new Hashtable( 10 ) );
        items.put( STATE_NS, new Hashtable( 10 ) );
    }

    public void setTitle( final String title )
    {
        //noinspection MethodCallInLoopCondition
        for( Enumeration i = forms.elements(); i.hasMoreElements(); )
        {
            ((Form) i.nextElement()).setTitle( title );
        }
    }

    protected void addStringItem(
            final String    id,
            final String    label,
            final String    text,
            final int       layout
    )
    {
        final StringItem item0  = newStringItem( label, text, layout );
        final StringItem itemN  = newStringItem( label, text, layout );
        final StringItem itemS  = newStringItem( label, text, layout );
        final StringItem itemNS = newStringItem( label, text, layout );

        values.put( id, text );

        final Hashtable forms = this.forms;
        final Hashtable items = this.items;

        ((Form) forms.get( STATE_0 ) ).append( item0 );
        ((Form) forms.get( STATE_N ) ).append( itemN );
        ((Form) forms.get( STATE_S ) ).append( itemS );
        ((Form) forms.get( STATE_NS ) ).append( itemNS );

        ((Hashtable) items.get( STATE_0 )).put( id, item0 );
        ((Hashtable) items.get( STATE_N )).put( id, itemN );
        ((Hashtable) items.get( STATE_S )).put( id, itemS );
        ((Hashtable) items.get( STATE_NS )).put( id, itemNS );
    }

    protected void setItemText( final String id, final String text )
    {
        values.put( id, text );
        ( (StringItem) ((Hashtable) items.get( state )).get( id ) ).setText(
                text
        );
    }

    private StringItem newStringItem(
            final String    label,
            final String    text,
            final int       layout
    )
    {
        final StringItem item = new StringItem( label, text );
        item.setLayout( layout );
        return item;
    }

    private void initButtons()
    {
        previousCommand = new Command(
                ResourceManager.getResource(
                        "global.Previous"
                ),
                Command.ITEM,
                100
        );
        nextCommand = new Command(
                ResourceManager.getResource(
                        "global.Next"
                ),
                Command.ITEM,
                100
        );

        final Command previousCommand = this.previousCommand;
        final Command nextCommand = this.nextCommand;
        previousN = newButton( prevImage, previousCommand );
        previousNS = newButton( prevImage, previousCommand );
        nextN = newButton( nextImage, nextCommand );
        nextNS = newButton( nextImage, nextCommand );
        selectButtonS = newButton( selectImage, selectCommand );
        selectButtonNS = newButton( selectImage, selectCommand );
    }

    private ImageItem newButton( final Image image, final Command command )
    {
        final ImageItem next = new ImageItem(
                null,
                image,
                Item.LAYOUT_LEFT,
                null
        );
        next.setDefaultCommand( command );
        next.setItemCommandListener( this );
        return next;
    }

    private void initImages()
    {
        synchronized( LOCK )
        {
            if( prevImage == null )
            {
                prevImage = getImage( "/prevdoc.png", "<" );
            }
            if( nextImage == null )
            {
                nextImage = getImage( "/nextdoc.png", ">" );
            }
            if( selectImage == null )
            {
                selectImage = getImage( "/select.png", "+" );
            }
        }
    }

    private Image getImage( final String resource, final String alt )
    {
        Image image;
        try
        {
            image = Image.createImage(
                    getClass().getResourceAsStream( resource )
            );
        }
        catch( IOException e )
        {
            image = Image.createImage( 14, 14 );
            final Graphics g = image.getGraphics();
            g.setColor( 215, 215, 215 );
            g.fillRect( 0, 0, 13, 13 );
            g.setColor( 0, 0, 0 );
            g.drawString( alt, 1, 1, Graphics.TOP | Graphics.LEFT );
        }
        return image;
    }

    public ScrollSet getScrollSet()
    {
        return scrollSet;
    }

    public void setScrollSet( final ScrollSet scrollSet )
    {
        this.scrollSet = scrollSet;
        setNavigationEnabled( scrollSet != null );
    }

    public boolean isNavigationEnabled()
    {
        return navigationEnabled;
    }

    public void setNavigationEnabled( final boolean navigationEnabled )
    {
        if( this.navigationEnabled != navigationEnabled )
        {
            this.navigationEnabled = navigationEnabled;
            Byte state = this.state;
            if( navigationEnabled )
            {
                if( state == STATE_0 )
                {
                    state = STATE_N;
                }
                else
                {
                    state = STATE_NS;
                }
            }
            else
            {
                if( state == STATE_NS )
                {
                    state = STATE_S;
                }
                else
                {
                    state = STATE_0;
                }
            }
            this.state = state;
            if( isShown() )
            {
                refresh();
                show();
            }
        }
    }

    protected void setSelectionEnabled( final boolean enabled )
    {
        if( this.selectionEnabled != enabled )
        {
            this.selectionEnabled = enabled;
            Byte state = this.state;
            if( enabled )
            {
                if( state == STATE_0 )
                {
                    state = STATE_S;
                }
                else
                {
                    state = STATE_NS;
                }
            }
            else
            {
                if( state == STATE_NS )
                {
                    state = STATE_N;
                }
                else
                {
                    state = STATE_0;
                }
            }
            this.state = state;
            if( isShown() )
            {
                refresh();
                show();
            }
        }
    }

    public boolean isShown()
    {
        final Displayable current =
                Application.getManager().getDisplay().getCurrent();
        //noinspection MethodCallInLoopCondition
        for( Enumeration i = forms.elements(); i.hasMoreElements(); )
        {
            if( current == i.nextElement() )
            {
                return true;
            }
        }
        return false;
    }

    public void addCommand( final Command command )
    {
        //noinspection MethodCallInLoopCondition
        for( Enumeration i = forms.elements(); i.hasMoreElements(); )
        {
            ((Form) i.nextElement()).addCommand( command );
        }
    }

    public void removeCommand( Command command )
    {
        //noinspection MethodCallInLoopCondition
        for( Enumeration i = forms.elements(); i.hasMoreElements(); )
        {
            ((Form) i.nextElement()).removeCommand( command );
        }
    }

    private void refresh()
    {
        final Hashtable current = ( (Hashtable) items.get( state ) );
        //noinspection MethodCallInLoopCondition
        for( Enumeration i = current.keys(); i.hasMoreElements(); )
        {
            final String id = (String) i.nextElement();
            final Item item = (Item) current.get( id );
            if( item instanceof StringItem )
            {
                ((StringItem) item).setText( (String) values.get( id ) );
            }
        }
    }

    public void show()
    {
        refresh();
        final Display display = Application.getManager().getDisplay();
        display.setCurrent( ((Form) forms.get( state )) );
//        if( navigationEnabled )
//        {
//            if( lastButton == 0 )
//            {
//                display.setCurrentItem( selectionEnabled ? previousNS : previousN );
//            }
//            else
//            {
//                display.setCurrentItem( selectionEnabled ? nextNS : nextN );
//            }
//        }
        AbstractView.setCurrent( this );
    }

    public void commandAction(
            final Command       command,
            final Displayable   displayable
    )
    {
        if( !handleAction( command ) )
        {
            if( command == selectCommand )
            {
                controller.handle( new ControllerEvent( selectCommandId, this ) );
            }
            else if( !handleCommand( command ) )
            {
                AbstractController.commandAction( this, command );
            }
        }
    }

    public void commandAction( final Command command, final Item item )
    {
        if( !handleAction( command ) )
        {
            if( command == previousCommand )
            {
                lastButton = 0;
                final ControllerEvent event = new ControllerEvent( this );
                event.setCode( MOVE_PREVIOUS );
                getController().handle( event );
            }
            else if( command == nextCommand )
            {
                lastButton = 1;
                final ControllerEvent event = new ControllerEvent( this );
                event.setCode( MOVE_NEXT );
                getController().handle( event );
            }
            else if( command == selectCommand )
            {
                controller.handle( new ControllerEvent( selectCommandId, this ) );
            }
        }
    }

    /**
     * Allow inheritors to provide custom commands, prior to calling default
     * AbstractController.commandAction method
     * @param command command executed
     * @return if the command has been consumed
     */
    protected boolean handleCommand( final Command command )
    {
        return false;
    }
}
