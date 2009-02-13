package bm.vm;

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
import bm.vm.lang.Block;

import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 07-oct-2007 0:55:28
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A method is a sequence of commands that operates on a ScriptingClass
 * Instance.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class MethodImpl
        implements Method
{
    protected ScriptingClass    clazz;
    protected Block             body;
    protected Vector            argNames;
    protected Vector            argClasses;

    protected transient String  name;

    public MethodImpl( final ScriptingClass clazz )
    {
        this.clazz = clazz;
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.MethodImpl";
    }

    /**
     * Get the class to which this method belongs.
     * @return scripting class
     */
    public ScriptingClass getClazz()
    {
        return clazz;
    }

    /**
     * Set the class to which this method belongs.
     *
     * @param clazz scripting class
     */
    public void setClazz( final ScriptingClass clazz )
    {
        this.clazz = clazz;
    }

    /**
     * Get the method name.
     *
     * @return method name
     */
    public String getMethodName()
    {
        return name;
    }

    /**
     * Set the method name.
     *
     * @param name name
     */
    public void setMethodName( String name )
    {
        this.name = name;
    }

    /**
     * Write object status to stream.
     *
     * @param out output stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public void serialize( SerializerOutputStream out )
            throws SerializationException
    {
        out.writeByte( (byte) 1 ); // version
        out.writeNullableObject( argNames );
        if( argClasses != null && argClasses.size() > 0 )
        {
            final int size = argClasses.size();
            for( int i = 0; i < size; i++ )
            {
                final ScriptingClass clazz = (ScriptingClass)
                        argClasses.elementAt( i );
                out.writeString( clazz.getName() );
            }
        }
        body.serialize( out );
    }

    /**
     * Read object status from stream.
     *
     * @param in input stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public void deserialize( SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // skip version
        argNames = (Vector) in.readNullableObject();
        if( argNames != null && argNames.size() > 0 )
        {
            final int size = argNames.size();
            argClasses = new Vector( size );
            for( int i = 0; i < size; i++ )
            {
                final String name = in.readString();
                try
                {
                    argClasses.addElement( clazz.getClassLoader().load( name ) );
                }
                catch( ScriptingClassNotFoundException e )
                {
                    throw new SerializationException(
                            Constants.ERR_MET_CLASS_NOT_FOUND,
                            e
                    );
                }
            }
        }
        else
        {
            argClasses = null;
        }
        body = new Block();
        body.setMethod( this );
        body.deserialize( in );
    }

    public Instance invoke(
            final Instance      target,
            final Instance[]    args
    )
            throws VirtualMachineException
    {
        if( target == null )
        {
            throw new InvocationException( 0, "Target can not be null" );
        }
        else
        {
            final Context context = new Context( target.context );
            if( argNames != null )
            {
                final int size = argNames.size();
                if( args == null || args.length != size )
                {
                    throw new InvocationException( 0, "Invalid arguments" );
                }
                for( int i = 0; i < size; i++ )
                {
                    final String name = (String) argNames.elementAt( i );
                    context.declare(
                            name,
                            (ScriptingClass) argClasses.elementAt( i )
                    );
                    context.set( name, args[i] );
                }
            }
            context.declare( "this", clazz );
            context.set( "this", target );
            body.setContext( context );
            return body.run();
        }
    }
}
