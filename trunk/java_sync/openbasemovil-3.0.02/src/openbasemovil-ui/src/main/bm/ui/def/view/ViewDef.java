package bm.ui.def.view;
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

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;

import java.io.ByteArrayInputStream;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 18:18:34
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Base view class.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public abstract class ViewDef
{
    protected int       id;
    protected String    title;
    protected String    controller;
    protected Vector    actions     = new Vector( 10 );
    protected Vector    attachments = new Vector( 10 );

    public int getId()
    {
        return id;
    }

    public String getTitle()
    {
        return title;
    }

    public String getController()
    {
        return controller;
    }

    public Vector getActions()
    {
        return actions;
    }

    public Vector getAttachments()
    {
        return attachments;
    }

    public void read( final byte[] data )
            throws SerializationException
    {
        final ByteArrayInputStream bais = new ByteArrayInputStream( data );
        final SerializerInputStream in = new SerializerInputStream( bais );

        in.readByte(); // skip version

        id = in.readInt();
        title = in.readString();
        controller = in.readNullableString();

        actions.removeAllElements();
        int count = in.readInt();
        if( count > 0 )
        {
            for( int i = 0; i < count; i++ )
            {
                final ActionDef action = new ActionDef();
                action.read( in );
                actions.addElement( action );
            }
        }

        attachments.removeAllElements();
        count = in.readInt();
        if( count > 0 )
        {
            for( int i = 0; i < count; i++ )
            {
                final AttachmentDef attachment = new AttachmentDef();
                attachment.read( in );
                attachments.addElement( attachment );
            }
        }

        read( in );
    }

    protected abstract void read( final SerializerInputStream in )
        throws SerializationException;
}
