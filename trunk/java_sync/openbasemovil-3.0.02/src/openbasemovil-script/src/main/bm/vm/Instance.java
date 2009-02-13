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

import bm.core.io.Serializable;
import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.tools.Tools;
import bm.vm.lang.Conversor;

import java.util.Hashtable;
import java.util.Enumeration;
/*
 * File Information
 *
 * Created on       : 07-oct-2007 0:56:03
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A live instance of a scripting class.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Instance
    implements Serializable
{
    protected ScriptingClass    scriptingClass;
    protected VirtualMachine    virtualMachine;
    protected Hashtable         properties = new Hashtable( 1 );
    protected Context           context;
    protected Hashtable         attachments = new Hashtable( 1 );

    public Instance(
            final ScriptingClass clazz,
            final VirtualMachine virtualMachine
    )
    {
        this.scriptingClass = clazz;
        this.virtualMachine = virtualMachine;
        context = new Context( virtualMachine.getContext() );
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.Instance";
    }

    public ScriptingClass getScriptingClass()
    {
        return scriptingClass;
    }

    public VirtualMachine getVirtualMachine()
    {
        return virtualMachine;
    }

    /**
     * Write object status to stream.
     *
     * @param out output stream
     * @throws bm.core.io.SerializationException on errors
     */
    public void serialize( SerializerOutputStream out )
            throws SerializationException
    {
        out.writeByte( (byte) 1 ); // version
        out.writeNullableObject( properties );
        out.writeNullableObject( attachments );
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
        properties = (Hashtable) in.readNullableObject();
        attachments = (Hashtable) in.readNullableObject();
    }

    public void set( final String name, final Object value )
    {
        final ScriptingClass propClass = scriptingClass.getPropertyClass( name );
        if( propClass != null )
        {
            if( !propClass.systemClass && !(value instanceof Instance) )
            {
                throw new IllegalArgumentException( "Only instances of classes can be assigned" );
            }
            if( value != null )
            {
                properties.put( name, value );
            }
            else
            {
                properties.remove( name );
            }
        }
        else
        {
            throw new IllegalArgumentException( "Invalid property: " + name );
        }
    }

    public Object get( final String name )
    {
        final ScriptingClass propClass = scriptingClass.getPropertyClass( name );
        if( propClass != null )
        {
            return properties.get( name );
        }
        else
        {
            throw new IllegalArgumentException( "Invalid property: " + name );
        }
    }

    /**
     * Set an attachment to this instance.<br/>
     * Attachments provide a way for custom classes to store persistent
     * properties that are not exposed through the normal mechanism.
     *
     * @param key key
     * @param value value, if null the attachment is removed
     */
    public void setAttachment( final Object key, final Object value )
    {
        if( value != null )
        {
            attachments.put( key, value );
        }
        else
        {
            attachments.remove( key );
        }
    }

    /**
     * Get an attachment.
     *
     * @param key key
     * @return value, null if not found
     */
    public Object getAttachment( final Object key )
    {
        return attachments.get( key );
    }

    public Instance invoke(
            final String     name,
            final Instance[] args
    )
            throws VirtualMachineException
    {
        return invoke( null, name, args );
    }

    public Instance invoke(
            final String     targetClass,
            final String     name,
            final Instance[] args
    )
            throws VirtualMachineException
    {
        final Method method = scriptingClass.getMethod( targetClass, name );
        if( method != null )
        {
            method.setMethodName( name );
            return method.invoke( this, args );
        }
        else
        {
            throw new InvocationException( 0, "Invalid method: " + name );
        }
    }

    public boolean equals( final Object o )
    {
        if( this == o )
        {
            return true;
        }
        if( o == null || getClass() != o.getClass() )
        {
            return false;
        }

        final Instance instance = (Instance) o;

        if( !scriptingClass.equals( instance.scriptingClass ) )
        {
            return false;
        }

        if( scriptingClass.hasMethod( "equals" ) )
        {
            try
            {
                return Conversor.toBoolean(
                        invoke( "equals", new Instance[] { (Instance) o } )
                );
            }
            catch( VirtualMachineException e )
            {
                return false;
            }
        }
        else
        {
            if( properties.size() != instance.properties.size() )
            {
                return false;
            }
            for( final Enumeration i = properties.keys(); i.hasMoreElements(); )
            {
                final String key = (String) i.nextElement();
                final boolean equals = Tools.objectEquals(
                        properties.get( key ),
                        instance.properties.get( key )
                );
                if( !equals )
                {
                    return false;
                }
            }
            return true;
        }
    }

    public int hashCode()
    {
        Integer hashCode = null;
        if( scriptingClass.hasMethod( "hashCode" ) )
        {
            try
            {
                hashCode = Conversor.toInteger( invoke( "hashCode", null ) );
            }
            catch( VirtualMachineException e )
            {
            }
        }
        if( hashCode == null )
        {
            int result;
            result = scriptingClass.hashCode();
            result = 31 * result + properties.hashCode();
            return result;
        }
        else
        {
            return hashCode.intValue();
        }
    }
}
