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
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import bm.core.tools.EnumerationUnion;
import bm.vm.sys.ScriptingObject;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 07-oct-2007 0:50:37
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A ScriptingClass is a definition of objects with certain properties and
 * methods that can be invoked.<br/>
 * Everything is case sensitive.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ScriptingClass
        implements Serializable
{
    protected Hashtable         properties;
    protected Hashtable         methods;
    protected String            name;
    protected boolean           systemClass;
    protected Vector            ancestors;
    protected VirtualMachine    vm;

    /**
     * Constructor.
     *
     * @param vm virtual machine
     */
    public ScriptingClass( final VirtualMachine vm )
    {
        this.vm = vm;
    }

    /**
     * Check if the class is a core system class.
     *
     * @return true if so
     */
    public boolean isSystemClass()
    {
        return systemClass;
    }

    void setSystemClass( final boolean systemClass )
    {
        this.systemClass = systemClass;
    }

    /**
     * Get the virtual machine under which this class exists.
     *
     * @return virtual machine
     */
    public VirtualMachine getVirtualMachine()
    {
        return vm;
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.ScriptingClass";
    }

    /**
     * Get this class name.
     *
     * @return class name
     */
    public String getName()
    {
        return name;
    }

    /**
     * Get the class loader assigned to this class.
     *
     * @return class loader
     */
    public ScriptingClassLoader getClassLoader()
    {
        return vm.getClassLoader();
    }

    /**
     * Write object status to stream.
     *
     * @param out output stream
     * @throws bm.core.io.SerializationException on errors
     */
    public void serialize( final SerializerOutputStream out )
            throws SerializationException
    {
        out.writeByte( (byte) 1 ); // version
        out.writeString( name );
        if( properties != null && properties.size() > 0 )
        {
            out.writeInt( properties.size() );
            for( final Enumeration i = properties.keys(); i.hasMoreElements(); )
            {
                final String name = (String) i.nextElement();
                final ScriptingClass clazz = (ScriptingClass)
                        properties.get( name );
                out.writeString( name );
                out.writeString( clazz.getName() );
            }
        }
        else
        {
            out.writeInt( 0 );
        }
        if( methods != null && methods.size() > 0 )
        {
            out.writeInt( methods.size() );
            for( final Enumeration i = methods.keys(); i.hasMoreElements(); )
            {
                final String name = (String) i.nextElement();
                final Method method = (Method) methods.get( name );
                out.writeString( name );
                method.serialize( out );
            }
        }
        else
        {
            out.writeInt( 0 );
        }
        if( ancestors != null && ancestors.size() > 0 )
        {
            final int size = ancestors.size();
            out.writeInt( size );
            for( int i = 0; i < size; i++ )
            {
                final ScriptingClass clazz = (ScriptingClass)
                        ancestors.elementAt( i );
                out.writeString( clazz.getName() );
            }
        }
        else
        {
            out.writeInt( 0 );
        }
    }

    /**
     * Read object status from stream.
     *
     * @param in input stream
     * @throws bm.core.io.SerializationException on errors
     */
    public void deserialize( final SerializerInputStream in )
            throws SerializationException
    {
        final ScriptingClassLoader classLoader = vm.getClassLoader();
        in.readByte(); // skip version
        name = in.readString();
        final int propCount = in.readInt();
        if( propCount > 0 )
        {
            properties = new Hashtable( propCount );
            for( int i = 0; i < propCount; i++ )
            {
                final String name = in.readString();
                final String className = in.readString();
                final ScriptingClass clazz;
                if(
                    className.equals( name ) ||
                        ( classLoader.getAlias( name ) != null &&
                          className.equals( classLoader.getAlias( name ) ) )
                )
                {
                    clazz = this;
                }
                else
                {
                    try
                    {
                        clazz = classLoader.load( className );
                    }
                    catch( ScriptingClassNotFoundException e )
                    {
                        throw new SerializationException(
                                Constants.ERR_SCL_CLASS_NOT_FOUND,
                                e
                        );
                    }
                }
                properties.put( name, clazz );
            }
        }
        else
        {
            properties = null;
        }
        final int methodCount = in.readInt();
        if( methodCount > 0 )
        {
            methods = new Hashtable( methodCount );
            for( int i = 0; i < methodCount; i++ )
            {
                final String name = in.readString();
                final Method method = new MethodImpl( this );
                method.deserialize( in );
                methods.put( name, method );
            }
        }
        else
        {
            methods = null;
        }
        final int ancestorCount = in.readInt();
        if( ancestorCount > 0 )
        {
            ancestors = new Vector( ancestorCount );
            for( int i = 0; i < ancestorCount; i++ )
            {
                final String name = in.readString();
                ScriptingClass clazz;
                try
                {
                    clazz = classLoader.load( name );
                }
                catch( ScriptingClassNotFoundException e )
                {
                    throw new SerializationException(
                            Constants.ERR_SCL_CLASS_NOT_FOUND,
                            e
                    );
                }
                ancestors.addElement( clazz );
            }
        }
        else
        {
            ancestors = null;
        }
    }

    /**
     * Get the property names defined in this class.
     *
     * @return enumeration of strings with property names
     */
    public Enumeration getPropertyNames()
    {
        final EnumerationUnion eu = new EnumerationUnion();
        if( properties != null )
        {
            eu.add( properties.keys() );
        }
        if( ancestors != null )
        {
            final int size = ancestors.size();
            for( int i = 0; i < size; i++ )
            {
                final ScriptingClass clazz = (ScriptingClass)
                        ancestors.elementAt( i );
                eu.add( clazz.getPropertyNames() );
            }
        }
        try
        {
            final ScriptingClass object = getClassLoader().load( "object" );
            eu.add( object.getPropertyNames() );
        }
        catch( ScriptingClassNotFoundException e )
        {
        }
        return eu;
    }

    /**
     * Get the method names defined in this class.
     *
     * @return enumeration of strings with method names
     */
    public Enumeration getMethodNames()
    {
        final EnumerationUnion eu = new EnumerationUnion();
        if( methods != null )
        {
            eu.add( methods.keys() );
        }
        if( ancestors != null )
        {
            final int size = ancestors.size();
            for( int i = 0; i < size; i++ )
            {
                final ScriptingClass clazz = (ScriptingClass)
                        ancestors.elementAt( i );
                eu.add( clazz.getMethodNames() );
            }
        }
        try
        {
            final ScriptingClass object = getClassLoader().load( "object" );
            eu.add( object.getMethodNames() );
        }
        catch( ScriptingClassNotFoundException e )
        {
        }
        return eu;
    }

    /**
     * Get the class of a given property.
     *
     * @param name property name
     * @return class or null if property is not defined in this class
     */
    public ScriptingClass getPropertyClass( final String name )
    {
        ScriptingClass propClass = null;
        if( properties != null )
        {
            propClass = (ScriptingClass) properties.get( name );
        }
        if( propClass == null && ancestors != null )
        {
            final int size = ancestors.size();
            for( int i = 0; i < size && propClass == null; i++ )
            {
                final ScriptingClass clazz = (ScriptingClass)
                        ancestors.elementAt( i );
                propClass = clazz.getPropertyClass( name );
            }
        }
        if( propClass == null )
        {
            try
            {
                final ScriptingClass object = getClassLoader().load( "object" );
                propClass = object.getPropertyClass( name );
            }
            catch( ScriptingClassNotFoundException e )
            {
            }
        }
        return propClass;
    }

    /**
     * Get a method.
     *
     * @param name method name
     * @return method or null if method is not defined in this class
     */
    public Method getMethod( final String name )
    {
        return getMethod( null, name );
    }

    /**
     * Get a method.
     *
     * @param targetClass target class, if null the current class
     * @param methodName method name
     * @return method or null if method is not defined in this class
     */
    public Method getMethod( final String targetClass, final String methodName )
    {
        Method method = null;
        if( (targetClass == null || targetClass.equals( name ) ) &&
            methods != null
                )
        {
            method = ( Method) methods.get( methodName );
        }
        if( method == null && ancestors != null )
        {
            final int size = ancestors.size();
            for( int i = 0; i < size && method == null; i++ )
            {
                final ScriptingClass clazz = (ScriptingClass)
                        ancestors.elementAt( i );
                method = clazz.getMethod( targetClass, methodName );
            }
        }
        if( method == null && !(this instanceof ScriptingObject ) )
        {
            try
            {
                final ScriptingClass object = getClassLoader().load( "object" );
                method = object.getMethod( methodName );
            }
            catch( ScriptingClassNotFoundException e )
            {
            }
        }
        return method;
    }

    /**
     * Check if the class has a property.
     *
     * @param name property name
     * @return true if so
     */
    public boolean hasProperty( final String name )
    {
        return getPropertyClass( name ) != null;
    }

    /**
     * Check if the class has a method.
     *
     * @param name name
     * @return true if so
     */
    public boolean hasMethod( final String name )
    {
        return getMethod( name ) != null;
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
        final ScriptingClass that = (ScriptingClass) o;
        return name.equals( that.name );
    }

    public int hashCode()
    {
        return name.hashCode();
    }

    public Vector getAncestors()
    {
        return ancestors;
    }
}
