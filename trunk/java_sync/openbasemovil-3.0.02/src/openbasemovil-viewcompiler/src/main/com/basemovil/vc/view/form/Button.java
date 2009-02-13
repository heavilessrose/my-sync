package com.basemovil.vc.view.form;
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

import java.util.Map;
import java.util.HashMap;

import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import bm.core.mvc.ControllerEvent;
import com.basemovil.vc.ViewCompilerException;
import com.basemovil.vc.ViewCompiler;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 22:09:36
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A button.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Button
        extends Item
{
    static final byte BUTTON_TYPE_BACK      = 0;
    static final byte BUTTON_TYPE_CANCEL    = 1;
    static final byte BUTTON_TYPE_EXIT      = 2;
    static final byte BUTTON_TYPE_HELP      = 3;
    static final byte BUTTON_TYPE_OK        = 4;
    static final byte BUTTON_TYPE_STOP      = 5;
    static final byte BUTTON_TYPE_HOME      = 6;
    static final byte BUTTON_TYPE_CUSTOM    = 7;

    private static final Map types = new HashMap( 8 );
    static
    {
        types.put( "back", new Integer( BUTTON_TYPE_BACK ) );
        types.put( "cancel", new Integer( BUTTON_TYPE_CANCEL ) );
        types.put( "exit", new Integer( BUTTON_TYPE_EXIT ) );
        types.put( "help", new Integer( BUTTON_TYPE_HELP ) );
        types.put( "ok", new Integer( BUTTON_TYPE_OK ) );
        types.put( "stop", new Integer( BUTTON_TYPE_STOP ) );
        types.put( "home", new Integer( BUTTON_TYPE_HOME ) );
        types.put( "custom", new Integer( BUTTON_TYPE_CUSTOM ) );
    }

    protected String    label;
    protected String    image;
    protected int       code;
    protected String    buttonType;

    public Button()
    {
        type = BUTTON;
    }

    public String getLabel()
    {
        return label;
    }

    public void setLabel( final String label )
    {
        this.label = label;
    }

    public String getImage()
    {
        return image;
    }

    public void setImage( final String image )
    {
        this.image = image;
    }

    public int getCode()
    {
        return code;
    }

    public void setCode( final int code )
    {
        this.code = code;
    }

    public String getButtonType()
    {
        return buttonType;
    }

    public void setButtonType( final String buttonType )
    {
        this.buttonType = buttonType;
    }

    public void store( final SerializerOutputStream out )
            throws ViewCompilerException
    {
        super.store( out );
        try
        {
            out.writeByte( (byte) 1 );
            final Integer type = (Integer) types.get( buttonType );
            out.writeInt( type.intValue() );
            switch( type.intValue() )
            {
                case BUTTON_TYPE_BACK:
                    code = ControllerEvent.BACK;
                    break;

                case BUTTON_TYPE_CANCEL:
                    code = ControllerEvent.CANCEL;
                    break;

                case BUTTON_TYPE_EXIT:
                    code = ControllerEvent.EXIT;
                    break;

                case BUTTON_TYPE_HELP:
                    code = ControllerEvent.HELP;
                    break;

                case BUTTON_TYPE_HOME:
                    code = ControllerEvent.HOME;
                    break;

                case BUTTON_TYPE_OK:
                    code = ControllerEvent.ACCEPT;
                    break;

                case BUTTON_TYPE_STOP:
                    code = ControllerEvent.STOP;
                    break;
            }
            out.writeInt( code );
            out.writeNullableString( ViewCompiler.escape( label ) );
            out.writeNullableString( image );
        }
        catch( SerializationException e )
        {
            throw new ViewCompilerException( e );
        }
    }
}
