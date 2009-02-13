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

import bm.vm.Instance;
import bm.vm.VirtualMachineException;
import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;

import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 12-oct-2007 23:26:25
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Invokate a method on an object.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Invocation
    extends Command
{
    protected Expression    target;
    protected String        targetClass;
    protected String        methodName;
    protected Vector        args;

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.lang.Invocation";
    }

    public String getName()
    {
        return CommandFactory.INVOCATION;
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
        target.serialize( out );
        out.writeString( methodName );
        out.writeNullableString( targetClass );
        final int length = args != null ? args.size() : 0;
        out.writeInt( length );
        if( args != null )
        {
            for( int i = 0; i < length; i++ )
            {
                ((Expression) args.elementAt( i )).serialize( out );
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
        in.readByte(); // skip version
        target = new Expression();
        target.setParent( this );
        target.deserialize( in );
        methodName = in.readString();
        targetClass = in.readNullableString();
        final int length = in.readInt();
        args = new Vector( length );
        for( int i = 0; i < length; i++ )
        {
            final Expression exp = new Expression();
            exp.setParent( this );
            exp.deserialize( in );
            args.addElement( exp );
        }
    }

    /**
     * Run the command.<br/>
     *
     * @return the value returned by a termination command or null
     * @throws bm.vm.VirtualMachineException on errors
     */
    public Instance run()
            throws VirtualMachineException
    {
        target.setContext( getContext() );
        final Instance instance = target.run();
        if( instance == null )
        {
            throw new NullPointerException( "Instance is null" );
        }
        final Instance[] arguments = args != null ?
                                     new Instance[ args.size() ] :
                                     null;
        if( args != null )
        {
            final int length = args.size();
            for( int i = 0; i < length; i++ )
            {
                arguments[i] = ((Expression) args.elementAt( i )).run();
            }
        }
        return targetClass != null ?
               instance.invoke( targetClass, methodName, arguments ) :
               instance.invoke( methodName, arguments );
    }
}
