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

import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.TextField;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;

import bm.core.mvc.AbstractView;
import bm.core.mvc.AbstractController;
import bm.core.mvc.ControllerEvent;
import bm.core.Application;
import bm.core.ResourceManager;

/*
 * File Information
 *
 * Created on       : 24-jul-2006 13:52:19
 * Created by       : javier
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Prompt View.
 *
 * @author <a href="mailto:javier.chacon@elondra.com">Javier Chacon</a>
 * @version $Revision$
 */
public class PromptView
    extends AbstractView
{
    private String      oldText;
    private TextField   textField;

    Boolean cancelled;

    public PromptView(
            final String title,
            final int maxSize,
            final int constraints
    )
    {
        super( null );
        setController( new PromptController() );

        textField = new TextField( null,
                                   null,
                                   maxSize,
                                   constraints
        );

        Form form = new Form( ResourceManager.getResource( title ) );
        form.append( textField );

        form.addCommand( Application.getCommandFactory().okCommand() );
        form.addCommand( Application.getCommandFactory().cancelCommand() );
        form.setCommandListener( this );

        displayable = form;
    }

    public void setText( final String text )
    {
        oldText = text;
        textField.setString( text );
    }

    public String getText()
    {
        return textField.getString();
    }

    public String getOldText()
    {
        return oldText;
    }

    public boolean isCancelled()
    {
        return cancelled != null && cancelled.booleanValue();
    }

    public class PromptController
        extends AbstractController
    {
        protected void process( final ControllerEvent event )
        {
            cancelled = new Boolean( event.getCode() == ControllerEvent.CANCEL );
        }
    }

    private class Waiter
        extends Thread
    {
        public void run()
        {
            while( cancelled == null )
            {
                try
                {
                    sleep( 500 );
                }
                catch( InterruptedException e )
                {
                }
            }
        }
    }

    public String prompt()
    {
        final Waiter waiter = new Waiter();
        final Display display = Application.getManager().getDisplay();
        final Displayable current = display.getCurrent();
        display.setCurrent( displayable );
        waiter.start();
        try
        {
            waiter.join();
        }
        catch( InterruptedException e )
        {
        }
        if( current != null )
        {
            display.setCurrent( current );
        }
        return cancelled.booleanValue() ? null : getText();
    }
}
