package bm.vm.lang;

/* -----------------------------------------------------------------------------
    bmScript Scripting language for Mobile Devices
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

import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.vm.Instance;
import bm.vm.VirtualMachineException;
import bm.vm.Context;
import bm.vm.Constants;

import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 10-oct-2007 14:05:09
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A block of commands that execute sequentially.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Block
    extends Command
{
    /**
     * Children list.
     */
    protected Vector children;

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.lang.Block";
    }

    public String getName()
    {
        return CommandFactory.BLOCK;
    }

    /**
     * Serializes command common properties, must be called by subclasses.
     *
     * @param out output stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public synchronized void serialize( SerializerOutputStream out )
            throws SerializationException
    {
        super.serialize( out );
        out.writeByte( (byte) 1 ); // version
        final int length = children != null ? children.size() : 0;
        out.writeInt( length );
        if( length > 0 )
        {
            for( int i = 0; i < length; i++ )
            {
                final Command child = (Command) children.elementAt( i );
                out.writeString( child.getName() );
                child.serialize( out );
            }
        }
    }

    /**
     * Read command common properties, must be called by subclasses.
     *
     * @param in input stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public synchronized void deserialize( SerializerInputStream in )
            throws SerializationException
    {
        super.deserialize( in );
        if( children == null )
        {
            children = new Vector( 10 );
        }
        else
        {
            children.removeAllElements();
        }
        in.readByte(); // skip version
        final int length = in.readInt();
        for( int i = 0; i < length; i++ )
        {
            try
            {
                final Command child = CommandFactory.createCommand( in.readString() );
                child.setParent( this );
                child.deserialize( in );
                children.addElement( child );
            }
            catch( Exception e )
            {
                e.printStackTrace();
                throw new SerializationException(
                        Constants.ERR_BLOCK_CREATE_COMMAND,
                        e
                );
            }
        }
    }

    /**
     * Add a new child to the list of children.
     * @param child child
     */
    public synchronized void add( final Command child )
    {
        if( children == null )
        {
            children = new Vector( 10 );
        }
        children.addElement( child );
    }

    /**
     * Run the command.<br/>
     * The default implementation runs children sequentially. If a child
     * that terminates execution is found, the value returned by the run method
     * of that child is immedately returned. Otherwise, the method returns null
     * when all children have been executed.
     *
     * @return the value returned by a termination children or null
     * @throws bm.vm.VirtualMachineException on errors
     */
    public Instance run()
            throws VirtualMachineException
    {
        final Context localContext = new Context( getContext() );
        final int length = children != null ? children.size() : 0;
        for( int i = 0; i < length; i++ )
        {
            //noinspection ConstantConditions
            final Command child = (Command) children.elementAt( i );
            child.setContext( localContext );
            if( child.terminateExcution )
            {
                terminated = true;
                return child.run();
            }
            else
            {
                final Instance retval = child.run();
                if( child.isTerminated() )
                {
                    terminated = true;
                    return retval;
                }
            }
        }
        return null;
    }
}
