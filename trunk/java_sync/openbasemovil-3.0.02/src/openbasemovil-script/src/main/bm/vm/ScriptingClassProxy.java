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

import java.util.Enumeration;
/*
 * File Information
 *
 * Created on       : 20-oct-2007 11:32:30
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A proxy is used when two loading classes make reference one to another, so
 * one can receive a proxy to the other that will be filled when the classes
 * have been fully loaded.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ScriptingClassProxy
    extends ScriptingClass
{
    private ScriptingClass clazz;

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.ScriptingClassProxy";
    }

    public ScriptingClassProxy( final VirtualMachine vm )
    {
        super( vm );
    }

    public ScriptingClass getClazz()
    {
        return clazz;
    }

    public void setClazz( final ScriptingClass clazz )
    {
        this.clazz = clazz;
    }

    public boolean isSystemClass()
    {
        return clazz.isSystemClass();
    }

    void setSystemClass( final boolean systemClass )
    {
        clazz.setSystemClass( systemClass );
    }

    /**
     * Get this class name.
     *
     * @return class name
     */
    public String getName()
    {
        return clazz.getName();
    }

    /**
     * Write object status to stream.
     *
     * @param out output stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public void serialize( final SerializerOutputStream out )
            throws SerializationException
    {
        clazz.serialize( out );
    }

    /**
     * Read object status from stream.
     *
     * @param in input stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public void deserialize( final SerializerInputStream in )
            throws SerializationException
    {
        clazz.deserialize( in );
    }

    /**
     * Get the property names defined in this class.
     *
     * @return enumeration of strings with property names
     */
    public Enumeration getPropertyNames()
    {
        return clazz.getPropertyNames();
    }

    /**
     * Get the method names defined in this class.
     *
     * @return enumeration of strings with method names
     */
    public Enumeration getMethodNames()
    {
        return clazz.getMethodNames();
    }

    /**
     * Get the class of a given property.
     *
     * @param name property name
     * @return class or null if property is not defined in this class
     */
    public ScriptingClass getPropertyClass( final String name )
    {
        return clazz.getPropertyClass( name );
    }

    /**
     * Get a method.
     *
     * @param name method name
     * @return method or null if method is not defined in this class
     */
    public Method getMethod( final String name )
    {
        return clazz.getMethod( name );
    }
}

